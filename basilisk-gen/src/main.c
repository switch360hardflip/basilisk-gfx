#include <stdlib.h>
#include <string.h>
#include <libxml/parser.h>
#include <stdio.h>

#include <bsgen_string.h>
#include <bsgen_list.h>

#define BSGEN_MIN(a,b) (((a)<(b))?(a):(b))
#define BSGEN_MAX(a,b) (((a)>(b))?(a):(b))

typedef struct {
    char* name;
    char* type;
} bsgen_Field;

typedef struct {
    char* name;
    bsgen_List fields;
} bsgen_Structure;

typedef struct {
    char* name;
} bsgen_Function;

typedef struct {
    const char* path;
    bsgen_List structures;
    bsgen_List functions;
} bsgen_Library;

void bsgen_warning(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}



  /*==============================================================================
   * XML Reader
   *============================================================================*/

static char* bsgen_getProperty(xmlNode* node, const char* name) {
    xmlAttr* property = node->properties;
    while (property) {
        if (xmlStrcmp(property->name, name) == 0)
            return property->children->content;

        property = property->next;
    }

    return NULL;
}

static xmlNode* bsgen_getSingleChild(xmlNode* node, const char* name) {
    xmlNode* child = node->children;
    xmlNode* result = NULL;

    while (child) {
        if (xmlStrcmp(child->name, name) == 0) {
            if (result) {
                bsgen_warning("Node \"%s\" has more than one child with name \"%s\"\n", node->name, name);
                return NULL;
            }
            result = child;
        }

        child = child->next;
    }

    return result;
}

 /**
  Structure
  */
static bsgen_Field bsgen_readField(xmlNode* node) {
    xmlNode* child = node->children;

    xmlNode* type_node = bsgen_getSingleChild(node, "type");
    xmlNode* name_node = bsgen_getSingleChild(node, "name");

    if (!type_node || !name_node)
        return (bsgen_Field) { 0 };

    bsgen_Field field = {
        .name = xmlStrdup(name_node->children->content),
        .type = xmlStrdup(type_node->children->content),
    };

    return field;
}

static bsgen_Structure bsgen_readStructure(xmlNode* node) {
    char* structure_name = bsgen_getProperty(node, "name");

    if (!structure_name)
        return (bsgen_Structure) { 0 };

    xmlNode* child = node->children;

    bsgen_Structure structure = {
        .name = xmlStrdup(structure_name),
        .fields = BSGEN_LIST(bsgen_Field),
    };

    while (child) {
        if (child->type == XML_ELEMENT_NODE) {
            if (xmlStrcmp(child->name, "field") == 0) {
                bsgen_Field field = bsgen_readField(child);
                bsgen_pushBack(&structure.fields, &field);
            }
        }

        child = child->next;
    }

    return structure;
}

static void bsgen_readStructures(bsgen_Library* library, xmlNode* node) {
    xmlNode* child = node->children;

    while (child) {
        if (child->type == XML_ELEMENT_NODE) {
            if (xmlStrcmp(child->name, "structure") == 0) {
                bsgen_Structure structure = bsgen_readStructure(child);

                if (structure.name)
                    bsgen_pushBack(&library->structures, &structure);
            }
        }

        child = child->next;
    }
}

 /**
  Functions
  */


 /**
  Library
  */
static bsgen_Library bsgen_readLibrary(const char* path) {
    bsgen_Library library = {
        .path = path,
        .functions = BSGEN_LIST(bsgen_Function),
        .structures = BSGEN_LIST(bsgen_Structure),
    };

    xmlDoc* document = xmlReadFile(path, NULL, 0);
    xmlNode* node = xmlDocGetRootElement(document)->children;

    while (node) {
        if (xmlStrcmp(node->name, "structures") == 0) {
            bsgen_readStructures(&library, node);
        }

        node = node->next;
    }

    return library;
}



 /*==============================================================================
  * Code Generation
  *============================================================================*/

static void bsgen_generateHeader(bsgen_Library* library, const char* name, const char* header_id) {
    bsgen_String* string = NULL;

    string = bsgen_appendStringF(string, "#ifndef %s\n#define %s\n\n", header_id, header_id);

   /**
    Definitions
    */
    for (int i = 0; i < library->structures.count; i++) {
        bsgen_Structure* structure = bsgen_fetchUnit(&library->structures, i);
        bsgen_appendStringF(string, "typedef struct %s %s\n", structure->name, structure->name);
    }

    bsgen_appendString(string, "\n", 1);

   /**
    Declarations
    */
    for (int i = 0; i < library->structures.count; i++) {
        bsgen_Structure* structure = bsgen_fetchUnit(&library->structures, i);
        bsgen_appendStringF(string, "struct %s {\n", structure->name);
        
        for (int j = 0; j < structure->fields.count; j++) {
            bsgen_Field* field = bsgen_fetchUnit(&structure->fields, j);
            bsgen_appendStringF(string, "    %s %s;\n", field->type, field->name);
        }

        bsgen_appendString(string, "}\n\n", 3);
    }

   /**
    Functions
    */

    string = bsgen_appendStringF(string, "#endif");
    printf("%s\n", string->value);

    free(string);
}

int main(int argc, const char* argv[]) {
    bsgen_Library basilisk_core = bsgen_readLibrary("xml/basilisk-core.xml");

    bsgen_generateHeader(&basilisk_core, "basilisk.h", "BASILISK_H");

	return 0;
}