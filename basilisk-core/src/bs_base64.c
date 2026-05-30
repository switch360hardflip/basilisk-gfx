/*
 * Base64 encoding/decoding (RFC1341)
 * Copyright (c) 2005-2011, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#include <bs_types.h>
#include <bs_log.h>
#include <string.h>
#include <bs_base64.h>

static const unsigned char base64_table[65] =
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

bs_Result bs_encodeBase64(const bs_U8* src, bs_U64 len, bs_U64* out_len, bool use_new_lines, char** out) {
	bs_U8* out, * pos;
	const bs_U8* end, * in;
	bs_U64 olen;
	int line_len;

	olen = len * 4 / 3 + 4; // 3-byte blocks to 4-byte
	olen++;                 // null term.
	if (use_new_lines) {
		olen += olen / 72;  // line feeds
		olen++;             // bs - new line
	}

	if (olen < len) {
		*out = NULL;
		return BS_RESULT_INTEGER_OVERFLOW;
	}

	out = bs_malloc(olen);

	end = src + len;
	in = src;
	pos = out;
	if (use_new_lines)
		*pos++ = '\n';
	line_len = 0;
	while (end - in >= 3) {
		*pos++ = base64_table[in[0] >> 2];
		*pos++ = base64_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
		*pos++ = base64_table[((in[1] & 0x0f) << 2) | (in[2] >> 6)];
		*pos++ = base64_table[in[2] & 0x3f];
		in += 3;
		line_len += 4;
		if (use_new_lines && line_len >= 72) {
			*pos++ = '\n';
			line_len = 0;
		}
	}

	if (end - in) {
		*pos++ = base64_table[in[0] >> 2];
		if (end - in == 1) {
			*pos++ = base64_table[(in[0] & 0x03) << 4];
			*pos++ = '=';
		}
		else {
			*pos++ = base64_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
			*pos++ = base64_table[(in[1] & 0x0f) << 2];
		}
		*pos++ = '=';
		line_len += 4;
	}

	if (use_new_lines && line_len) 
		*pos++ = '\n';

	*pos = '\0';
	if (out_len) *out_len = pos - out;
	return out;
}

bs_Result bs_decodeBase64(const bs_U8* src, bs_U64 len, bs_U64* out_len, char** out) {
	bs_U8 dtable[256], * out, * pos, block[4], tmp;
	size_t i, count, olen;
	int pad = 0;

	memset(dtable, 0x80, 256);
	for (i = 0; i < sizeof(base64_table) - 1; i++)
		dtable[base64_table[i]] = (bs_U8)i;
	dtable['='] = 0;

	count = 0;
	for (i = 0; i < len; i++) {
		if (dtable[src[i]] != 0x80) count++;
	}

	if (count == 0 || count % 4) return NULL;

	olen = count / 4 * 3;
	pos = *out = bs_malloc(olen);

	count = 0;
	for (i = 0; i < len; i++) {
		tmp = dtable[src[i]];

		if (tmp == 0x80) continue;
		if (src[i] == '=') pad++;

		block[count] = tmp;
		count++;

		if (count == 4) {
			*pos++ = (block[0] << 2) | (block[1] >> 4);
			*pos++ = (block[1] << 4) | (block[2] >> 2);
			*pos++ = (block[2] << 6) | block[3];
			count = 0;
			if (pad) {
				if (pad == 1)      pos--;
				else if (pad == 2) pos -= 2;
				else {
					bs_free(*out);
					*out = NULL;
					return BS_RESULT_INVALID_BASE64_PADDING;
				}
				break;
			}
		}
	}

	*out_len = pos - *out;

	return BS_RESULT_OK;
}