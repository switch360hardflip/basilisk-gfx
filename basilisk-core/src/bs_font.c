#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include <basilisk-core.h>
#include <bs_internal.h>

#define BS_FLAGSET(flag, cmp) ((flag >> cmp) & 0x01)

static inline bs_U8 bs_memU8(void* data, bs_U32 offset) {
    return *((bs_U8*)data + offset);
}

static inline bs_U16 bs_memU16(void* data, bs_U32 offset) {
    const bs_U8* base = (bs_U8*)data + offset;
    return (bs_U16)(base[0] << 8 | base[1]);
}

static inline bs_U32 bs_memU32(void* data, bs_U32 offset) {
    const bs_U8* base = (bs_U8*)data + offset;
    return (bs_U32)(base[0] << 24 | base[1] << 16 | base[2] << 8 | base[3]);
}

static inline int bs_memcmpU32(const void* a, const void* b) {
    return memcmp(a, b, sizeof(bs_U32));
}

// is this really how you do it?
static void* bs_findTable(bs_TTF* ttf, const char tag[4]) {
    const char* table = NULL;
    for (int i = 0; i < ttf->buffer->len - 4; i++) {
        const char* ptr = ttf->buffer->value + i;

        if (memcmp(ptr, tag, 4) == 0) {
            bs_U32 offset = bs_memU32(ptr, 8);
            return ttf->buffer->value + offset;
        }
    }
    
    bs_throwBasilisk(BSX_FAILED_TO_QUERY);
    return NULL;
}

static void bs_head(bs_TTF* ttf) {
    ttf->head.buf = bs_findTable(ttf, "head");
    bs_U32 version = bs_memU32(ttf->head.buf, HEAD_VERSION);
    ttf->head.units_per_em = bs_memU16(ttf->head.buf, HEAD_UNITS_PER_EM);
    ttf->head.index_to_loc_format = bs_memU16(ttf->head.buf, HEAD_INDEX_TO_LOC_FORMAT);
}

static void bs_maxp(bs_TTF* ttf) {
    ttf->maxp.buf = bs_findTable(ttf, "maxp");
    ttf->maxp.num_glyphs = bs_memU16(ttf->maxp.buf, MAXP_NUM_GLYPHS);
}

static void bs_hhea(bs_TTF* ttf) {
    ttf->hhea.buf = bs_findTable(ttf, "hhea");
    ttf->hhea.num_of_long_hor_metrics = bs_memU16(ttf->hhea.buf, HHEA_NUM_OF_LONG_HOR_METRICS);
}

static inline int bs_findPair(uint8_t* base, int count, uint16_t left) {
    int lo = 0;
    int hi = count;

    while (lo < hi) {
        int mid = (lo + hi) / 2;

        bs_U16 mid_left = bs_memU16(base + mid * 6, 0);

        if (mid_left < left)
            lo = mid + 1;
        else
            hi = mid;
    }

    return lo;
}

static inline int bs_kernFormat0(bs_TTF* ttf, char* kern, int offset) {
    bs_U16 pairs_count = bs_memU16(kern, KERN_FORMAT0_PAIRS_COUNT + offset);

    bs_U8* base = kern + KERN_FORMAT0_PAIR_START + offset;

    for (int i = 0; i < pairs_count; i++) {
        bs_U8* p = base + i * sizeof(bs_U16) * 3;

        bs_U16 left = bs_memU16(p, KERN_FORMAT0_PAIR_LEFT);
        bs_U16 right = bs_memU16(p, KERN_FORMAT0_PAIR_RIGHT);
        bs_U16 value = bs_memU16(p, KERN_FORMAT0_PAIR_VALUE);

        bs_Glyph* left_glyph = NULL;
        int right_glyph_index = -1;

        for (int j = 0; j < ttf->glyphs.count; j++) {
            bs_Glyph* glyph = bs_fetchUnit(&ttf->glyphs, j);

            if (glyph->index == left)
                left_glyph = glyph;

            if (glyph->index == right)
                right_glyph_index = j;

            if (left_glyph && right_glyph_index > 0)
                break;
        }

        if (!left_glyph || right_glyph_index == -1)
            continue;

        bs_pushBack(&ttf->kerning_pairs, &(bs_KerningPair){
            .right_index = right_glyph_index,
            .right = right,
            .value = value,
        });

        left_glyph->pairs_count++;
    }

    return sizeof(bs_U16) * 4 + sizeof(bs_U16) * 3 * pairs_count;
}

void bs_kern(bs_TTF* ttf) {
    bs_except(BSX_FAILED_TO_QUERY);
    char* buf = bs_findTable(ttf, "kern");
    if (bs_caught())
        return;
    
    // TODO: these can be bs_U32 on OS X only fonts but idk how 2 check
    bs_U16 version = bs_memU16(buf, KERN_VERSION);
    bs_U16 subtables_count = bs_memU16(buf, KERN_NUMBER_OF_TABLES);

    int header_size =
        sizeof(bs_U32) +
        sizeof(bs_U16) * 3;
    if (version == 1) // AAT - Apple Advanced Typography
        header_size += sizeof(bs_U16);

    for (int i = 0, offset = 0; i < subtables_count; i++) {
        bs_U32 length = bs_memU32(buf, KERN_LENGTH + offset);
        bs_U16 coverage = bs_memU16(buf, KERN_COVERAGE + offset);
        // bs_U16 tuple_index = bs_memU16(buf, KERN_TUPLE_INDEX + offset); // AAT

        int vertical = (coverage & KERN_COVERAGE_VERTICAL) != 0;
        int cross_stream = (coverage & KERN_COVERAGE_CROSS_STREAM) != 0;
        int variation = (coverage & KERN_COVERAGE_VARIATION) != 0;

        bs_U16 format = (coverage & 0xFF00) >> 8;

        switch (format) {
        case 0: offset += bs_kernFormat0(ttf, buf, offset + header_size); break;
        case 1:
        case 2:
        case 3:
            bs_throwBasiliskF(BSX_NOT_IMPLEMENTED, "KERN format %d", format);
        default:
            bs_throwBasiliskF(BSX_CORRUPTED, "KERN format %d is not between 0..4", format);
        }
    }
}

static void bs_gpos(bs_TTF* ttf) {
    bs_except(BSX_FAILED_TO_QUERY);
    char* buf = bs_findTable(ttf, "gpos");
    bs_caught();
}

static void bs_hmtx(bs_TTF* ttf, bs_Glyph* glyph) {
    ttf->hmtx.buf = ttf->hmtx.buf == NULL ? bs_findTable(ttf, "hmtx") : ttf->hmtx.buf;

    bs_U32 offset = glyph->index * sizeof(bs_LongHorMetric);
    glyph->long_hor_metric.advance_width = bs_memU16(ttf->hmtx.buf, offset);
    glyph->long_hor_metric.left_side_bearing = (bs_I16)bs_memU16(ttf->hmtx.buf, offset + 2);
}

static bs_U32 bs_loca(bs_TTF* ttf, int id) {
    ttf->loca.buf = ttf->loca.buf == NULL ? bs_findTable(ttf, "loca") : ttf->loca.buf;

    bs_U32 offset = 0;
    if (ttf->head.index_to_loc_format == 0)
        offset = bs_memU16(ttf->loca.buf, id * sizeof(bs_U16)) * 2;
    else
        offset = bs_memU32(ttf->loca.buf, id * sizeof(bs_U32));

    return offset;
}

static int bs_glyfCoords(bs_Glyf* glyf, bool y, bs_U8* flags, bs_GlyfPt* pts, int offset, int num_points) {
    bs_U16 coord_prev = 0;
    for (int i = 0; i < num_points; i++) {
        bs_U8 flag = flags[i];
        bs_U16 coord;

	    // If coord is 8-bit
	    if (BS_FLAGSET(flag, GLYF_X_SHORT + y)) {
	        coord = bs_memU8(glyf->buf, offset);
	        offset += 1;

            if (!BS_FLAGSET(flag, GLYF_X_SAME + y)) {
                coord = -coord;
            }

	        coord += coord_prev;
	    } else {
	        if (BS_FLAGSET(flag, GLYF_X_SAME + y)) {
		        coord = coord_prev;
	        } else {
		        coord = bs_memU16(glyf->buf, offset);
		        coord += coord_prev;
		        offset += 2;
	        }
	    }

	    coord_prev = coord;
	    char *data = (char *)(pts + i);
	    bs_U16 *val = (bs_U16*)(data + y * sizeof(bs_U16));

	    *val = coord;
    }
    
    return offset;
}

static void bs_glyf(bs_TTF* ttf, bs_Glyph* glyph) {
    ttf->glyf.buf = bs_findTable(ttf, "glyf");

    // get location of glyph in ttf buffer
    bs_U32 loc = bs_loca(ttf, glyph->index);
    ttf->glyf.buf += loc;

    // get bounding boxes
    glyph->x_min = bs_memU16(ttf->glyf.buf, GLYF_XMIN);
    glyph->y_min = bs_memU16(ttf->glyf.buf, GLYF_YMIN);
    glyph->x_max = bs_memU16(ttf->glyf.buf, GLYF_XMAX);
    glyph->y_max = bs_memU16(ttf->glyf.buf, GLYF_YMAX);

    // parse contours
    int num_contours = bs_memU16(ttf->glyf.buf, GLYF_NUMBER_OF_CONTOURS);
    int* end_pts = _alloca(num_contours * sizeof(int));
    for (int i = 0; i < num_contours; i++) {
        end_pts[i] = bs_memU16(ttf->glyf.buf, GLYF_END_PTS_OF_CONTOURS + i * 2);
    }

    // num points is equal to end of last contour
    int num_points = end_pts[num_contours - 1] + 1;

    int instruction_offset = GLYF_INSTRUCTION_LENGTH(num_contours);
    int num_instructions = bs_memU16(ttf->glyf.buf, instruction_offset);

    int flag_offset = GLYF_FLAGS(instruction_offset, num_instructions);
    int flag_offset_original = flag_offset;

    int coord_offset = GLYF_XCOORDS(flag_offset, num_points);

    glyph->coords = bs_malloc(num_points * sizeof(bs_GlyfPt));
    glyph->contours = bs_malloc(num_contours * sizeof(uint16_t));
    glyph->num_contours = num_contours;
    glyph->num_points = num_points;

    for (int i = 0; i < num_contours; i++)
        glyph->contours[i] = end_pts[i];

    int num_repeats_total = 0;
    bs_U8* flags = _alloca(num_points);

    // Parse flags
    for (int i = 0; i < num_points; i++) {
        flags[i] = bs_memU8(ttf->glyf.buf, flag_offset++);

        // Add repeated flags
        if (BS_FLAGSET(flags[i], GLYF_REPEAT)) {
            int num_repeats = bs_memU8(ttf->glyf.buf, flag_offset++);
            int flag_repeated = flags[i];

            num_repeats_total += num_repeats - 1;

            assert((i + num_repeats) <= num_points);

            for (int j = 0; j < num_repeats; j++)
                flags[++i] = flag_repeated;
        }
    }

    coord_offset -= num_repeats_total;

    for (int i = 0; i < num_points; i++) {
        glyph->coords[i].on_curve = BS_FLAGSET(flags[i], GLYF_ON_CURVE);
    }

    // Get X and Y coords
    coord_offset = bs_glyfCoords(&ttf->glyf, 0, flags, glyph->coords, coord_offset, num_points);
    coord_offset = bs_glyfCoords(&ttf->glyf, 1, flags, glyph->coords, coord_offset, num_points);
}

static void bs_cmapFormat4(bs_TTF* ttf, bs_Glyph* glyph, char* subtable) {
    bs_U16 seg_count_x2 = bs_memU16(subtable, CMAP_FORMAT4_NUM_SEGMENTS_X2);
    bs_U16 seg_count = seg_count_x2 / 2;

    int index = -1;

    for (int j = 0; j < seg_count; j++) {
        bs_U16 end_code = bs_memU16(subtable, CMAP_FORMAT4_END_CODE + j * sizeof(bs_U16));

        if (end_code >= glyph->code) {
            index = j;
            break;
        }
    }

    if (index == -1) {
        bs_throwBasilisk(BSX_FAILED_TO_QUERY);
        return;
    }

    bs_U16* id_range_offset_address = (bs_U16*)(subtable + CMAP_FORMAT4_ID_RANGE_OFFSET(seg_count) + index * sizeof(bs_U16));
    bs_U16 id_range_offset = bs_memU16(subtable, CMAP_FORMAT4_ID_RANGE_OFFSET(seg_count) + index * sizeof(bs_U16));
    bs_U16 start_code = bs_memU16(subtable, CMAP_FORMAT4_START_CODE(seg_count) + index * sizeof(bs_U16));
    bs_I16 id_delta = bs_memU16(subtable, CMAP_FORMAT4_ID_DELTA(seg_count) + index * sizeof(bs_U16));
    bs_U16 end_code = bs_memU16(subtable, CMAP_FORMAT4_END_CODE + index * sizeof(bs_U16));

    int glyph_index;

    if (id_range_offset == 0) {
        glyph_index = (glyph->code + id_delta) & 0xFFFF;
    }
    else {
        bs_U8* addr =
            (bs_U8*)id_range_offset_address +
            id_range_offset +
            2 * (glyph->code - start_code);

        glyph_index = bs_memU16(addr, 0);

        if (glyph_index != 0)
            glyph_index = (glyph_index + id_delta) & 0xFFFF;
    }

    glyph->index = glyph_index;

    return;
}

static void bs_cmap(bs_TTF* ttf, bs_Glyph* glyph) {
    char* cmap = bs_findTable(ttf, "cmap");
    bs_U16 subtables_count = bs_memU16(cmap, CMAP_NUMBER_SUBTABLES);

    int subtable_offset = -1;
    for (int i = 0; i < subtables_count; i++) {
        bs_U32 platform_id          = bs_memU16(cmap, CMAP_PLATFORM_ID + CMAP_TABLE_SIZE * i);
        bs_U32 platform_specific_id = bs_memU16(cmap, CMAP_PLATFORM_SPECIFIC_ID + CMAP_TABLE_SIZE * i);
        subtable_offset             = bs_memU32(cmap, CMAP_OFFSET + CMAP_TABLE_SIZE * i);

        if (platform_id == 0) 
            break;
    }

    if (subtable_offset == -1)
        bs_throwBasiliskF(BSX_NOT_IMPLEMENTED, "TTF Only supports Unicode");

    char* subtable = cmap + subtable_offset;
    bs_U16 format = bs_memU16(subtable, CMAP_SUBTABLE_FORMAT);

    switch (format) {
    case 4: bs_cmapFormat4(ttf, glyph, subtable); break;
    default: bs_throwBasiliskF(BSX_NOT_IMPLEMENTED, "TTF format %d", format); break;
    }
}

static int bs_cmpFloats(const void *a, const void *b) {
    float f_a = *(float *)a;
    float f_b = *(float *)b;

    if (f_a < f_b) 
        return -1;
    else if (f_a > f_b) 
        return 1;

    return 0;
}

static int bs_glyphWindingOrder(bs_Glyph* glyph, bs_vec2 point, bs_vec2* pts, int num_pts) {
    int winding = 0;

    for (int i = 0; i < num_pts; ++i) {
        bs_vec2 pt1 = pts[i];
        bs_vec2 pt2 = pts[(i + 1) % num_pts];

        if (pt1.y <= point.y && pt2.y > point.y) {
            float intersect_x = pt1.x + (point.y - pt1.y) * (pt2.x - pt1.x) / (pt2.y - pt1.y);
            if (intersect_x > point.x)
                winding++;
        }
        else if (pt1.y > point.y && pt2.y <= point.y) {
            float intersect_x = pt1.x + (point.y - pt1.y) * (pt2.x - pt1.x) / (pt2.y - pt1.y);
            if (intersect_x > point.x)
                winding--;
        }
    }

    return winding;
}

static void bs_calculateContourCurves(bs_Glyph* glyph, int contour, bs_vec2* out, bs_U32* out_num_pts) {
#define BS_TTF_DETAIL 4

    uint16_t first = (contour == 0) ? 0 : glyph->contours[contour - 1] + 1;
    uint16_t last = glyph->contours[contour] + 1;

    for (int j = first; j < last; j++) {
        bs_GlyfPt curr = glyph->coords[j];
        bs_GlyfPt curr_off, next_off;

        int idx;
        if ((j + 1) >= last) 
            idx = (j + 1) - last + first;
        else
            idx = (j + 1);

        curr_off = glyph->coords[idx];
        if (!curr.on_curve) continue;

        bs_vec2 curr_off_v, next_off_v, curr_v = bs_v2(curr.x, curr.y);
        if (curr_off.on_curve) {
            out[(*out_num_pts)++] = curr_v;
            continue;
        }

        while (!(curr_off = glyph->coords[idx]).on_curve) {
            next_off = glyph->coords[((j + 2) >= last) ? ((j + 2) - last + first) : (j + 2)];
            if (next_off.on_curve) break;

            curr_off_v = bs_v2(curr_off.x, curr_off.y);
            next_off_v = bs_v2(next_off.x, next_off.y);

            bs_vec2 mid = bs_v2mid(curr_off_v, next_off_v);

            bs_vec2 elems[BS_TTF_DETAIL + 1];
            bs_v2QuadBez(curr_v, curr_off_v, mid, elems, BS_TTF_DETAIL);
            elems[BS_TTF_DETAIL] = mid;
            for (int i = 0; i < BS_TTF_DETAIL; i++) {
                out[(*out_num_pts)++] = elems[i];
            }

            curr_v = mid;
            j++;
            if ((j + 1) >= last) idx = (j + 1) - last + first;
            else                 idx = (j + 1);
        }

        curr_off = glyph->coords[((j + 1) >= last) ? ((j + 1) - last + first) : (j + 1)];
        next_off = glyph->coords[((j + 2) >= last) ? ((j + 2) - last + first) : (j + 2)];
        curr_off_v = bs_v2(curr_off.x, curr_off.y);
        next_off_v = bs_v2(next_off.x, next_off.y);

        double t = 0.0;
        double incr = 1.0 / (double)BS_TTF_DETAIL;

        for (int k = 0; k < BS_TTF_DETAIL; k++, t += incr) {
            bs_vec2 v;
            v.x = (1 - t) * (1 - t) * curr_v.x + 2 * (1 - t) * t * curr_off_v.x + t * t * next_off_v.x;
            v.y = (1 - t) * (1 - t) * curr_v.y + 2 * (1 - t) * t * curr_off_v.y + t * t * next_off_v.y;
            out[(*out_num_pts)++] = v;
        }
    }
    out[(*out_num_pts)++] = bs_v2(glyph->coords[first].x, glyph->coords[first].y);
}

#define AA_SAMPLES 4
#define AA_TOTAL (AA_SAMPLES * AA_SAMPLES)

void bs_rasterizeGlyph(bs_TTF* font, bs_Glyph* glyph, int width, int height, char* out_bmp, float scale) {
   // int num_pts = font->lookup[code].num_points;
    bs_vec2 out[BS_TTF_MAX_PTS]; // todo too much stack
    bs_U32 out_num_pts = 0;

    for (int i = 0; i < glyph->num_contours; i++) {
        bs_U32 num_pts = 0;
        bs_vec2* pts = out + out_num_pts;
        bs_calculateContourCurves(glyph, i, pts, &num_pts);
        out_num_pts += num_pts;

        for (int j = 0; j < num_pts; j++) {
            pts[j].x = (pts[j].x - glyph->x_min) * scale;
            pts[j].y = (pts[j].y - glyph->y_min) * scale;
        }

        for (int y = 0; y < glyph->height; y++) {
            float* intersections = _alloca(num_pts * sizeof(float));
            int num_intersections = 0;

            for (int j = 0; j < num_pts; j++) {
                bs_vec2 pt0 = pts[j];
                bs_vec2 pt1 = pts[(j + 1) % num_pts];

                if ((y < fmin(pt0.y, pt1.y)) || (y >= fmax(pt0.y, pt1.y))) 
                    continue;

                float dy = pt1.y - pt0.y;
                if (dy != 0) {
                    float dx = pt1.x - pt0.x;
                    float x_intersect = pt0.x + (y - pt0.y) * dx / dy;
                    intersections[num_intersections++] = x_intersect;
                }
            }

            qsort(intersections, num_intersections, sizeof(float), bs_cmpFloats);

            for (int j = 0; j < num_intersections - 1; j += 2) {
                int start = ceil(intersections[j]);
                int end = floor(intersections[j + 1]);

                for (int x = start; x <= end; x++) {
                    if (x >= 0 && x < glyph->width) {
                        int z = bs_glyphWindingOrder(glyph, bs_v2(x, y), pts, num_pts);

                        out_bmp[y * glyph->width + x] = z == -1 ? 255 : 0;
                    }
                }
            }
        }
    }
}

static inline bool bs_sameFont(bs_TTF* font, const char* path, const char* alphabet) {
    return
        font->name == path &&
        font->alphabet == alphabet;
}

void bs_glyph(bs_TTF* ttf, bs_U16 code) {
    //int index = bs_glyphIndex(ttf, code);

    bs_Glyph* glyph = bs_pushBack(&ttf->glyphs, &(bs_Glyph) {.code = code});

    bs_cmap(ttf, glyph);
    bs_glyf(ttf, glyph);
    bs_hmtx(ttf, glyph);
}

void bs_destroyTtf(bs_TTF* ttf) {
    free(ttf->buffer);
    //free(ttf->atlas);
}

void bs_ttf(bs_TTF* existing, const char* path, bs_U32 flags) {
    bs_TTF ttf = {
        .buffer = bs_loadFile(path),
        .glyphs = bs_list(sizeof(bs_Glyph), 64),
        .kerning_pairs = bs_list(sizeof(bs_KerningPair), 64),
    };

    ttf.table_count = bs_memU16(ttf.buffer->value, 4);

    bs_head(&ttf);
    bs_maxp(&ttf);
    bs_hhea(&ttf);
    //bs_gpos(&ttf);

    memcpy(existing, &ttf, sizeof(bs_TTF));
}



  /*==============================================================================
   * Font Format
   *============================================================================*/

bs_vec2 bs_textDimensions(bs_Font* font, char* name, int length) {
    float width = 0.0;
    float layout_scale = ((float)font->size / (float)font->units_per_em);

    for (int i = 0; i < length; i++) {
        char c = font->table[name[i]];
        int index = font->table[c];
        if (index >= font->atlas->count)
            index = 0;
        float spacing = font->glyphs[index].advance_width * layout_scale;
        width += spacing;
        // width += name[i] == ' ' ? font->spacing * layout_scale : bs_atlasSize(font->atlas, c).x;
    }
    return bs_v2(
        width,
        font->height);
}

void bs_destroyFont(bs_Font* font) {
    if (font->atlas)
        bs_destroyAtlas(font->atlas);
    //	if (font->fragment_shader)
    //		bs_destroyShader(font->fragment_shader);

    // TODO: make generic
    int id = font->head.id;
    int source_id = font->head.source_id;
    memset(font, 0, sizeof(bs_Font));
    font->head.source_id = source_id;
    font->head.id = id;
}

void bs_bindFont(bs_Font* font, bs_Sampler* sampler, int bind_set, int bind_point) {
    bs_bindImages(bind_set, bind_point, &(bs_ImageDescriptor) {
        .image = font->atlas->image,
        .layout = BS_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        .sampler = sampler,
    }, 1);
}

bs_Object* bs_loadFont(bs_Object* object, int package_id, const char* resource_name, const char* alphabet, float spacing, bs_U32 flags) {
    bs_Font* font = object->font;

    if (!font) return NULL;
    if (object->flags & BS_OBJECT_ALREADY_EXISTS && !(object->flags & BS_OBJECT_FORCE_DESTROY)) return NULL;

    bs_destroyFont(font);

    bs_Resource* resource = bs_loadResource(package_id, resource_name, 0);

    bs_BfntHeader* header = resource->data->value;
    if (header->magic != 0x746E6662) {
        bs_throwBasilisk(BSX_CORRUPTED);
        return NULL;
    }

    if (header->version != 1) {
        bs_throwBasilisk(BSX_NOT_SUPPORTED);
        return NULL;
    }

    font->spacing = spacing;
    font->alphabet = alphabet;
    font->atlas = bs_loadAtlasMemory(BS_ATLAS(-1, 0, 0), package_id, resource->name, resource->data->value + header->batl_offset, 0)->head;
    font->size = header->size;
    font->units_per_em = header->units_per_em;
    font->height = header->line_height;
    font->glyphs = bs_malloc(font->atlas->count * sizeof(*font->glyphs));
    font->pairs_count = header->kerning_pairs_count;

    if (font->units_per_em == 0)
        font->units_per_em = 1;

    unsigned char* offset = resource->data->value + sizeof(bs_BfntHeader);

    if (font->pairs_count > 0) {
        font->pairs = bs_malloc(font->pairs_count * sizeof(*font->pairs));

        for (int i = 0; i < font->pairs_count; i++) {
            bs_BfntKerningPair* pair = offset;
            font->pairs[i].value = pair->value;
            font->pairs[i].right = pair->right;

            offset += sizeof(bs_BfntKerningPair);
        }
    }

    for (int i = 0; i < header->glyphs_count; i++) {
        bs_BfntGlyph* glyph = offset;

        font->glyphs[i].advance_width = glyph->advance_width;
        font->glyphs[i].left_side_bearing = glyph->left_side_bearing;
        font->glyphs[i].kerning_pair_offset = glyph->kerning_pair_offset;
        font->glyphs[i].kerning_pair_count = glyph->kerning_pair_count;
        font->glyphs[i].y_offset = glyph->y_offset;
        font->min_y_shift = BS_MIN(font->min_y_shift, glyph->y_offset);
        offset += sizeof(bs_BfntGlyph);
    }

    memcpy(font->table, header->ascii_table, 256);

    resource->font = font;
    return object;
}