#include <stdlib.h>
#include <string.h>
#include <libxml/parser.h>
#include <stdio.h>

#include <bsgen_string.h>
#include <bsgen_list.h>

#define BSGEN_MIN(a,b) (((a)<(b))?(a):(b))
#define BSGEN_MAX(a,b) (((a)>(b))?(a):(b))

typedef struct {
    char* value;
} bsgen_MacroLine;

typedef struct {
    char* name;
    bsgen_List lines;
} bsgen_Macro;

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
    char* type;
} bsgen_Param;

typedef struct {
    char* name;
    char* return_value;
    bsgen_List params;
} bsgen_Function;

typedef struct {
    const char* path;
    bsgen_List macros;
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
  Macro
  */
static void bsgen_printMacroLines(bsgen_Macro* m) {

    for (int k = 0; k < m->lines.count; k++) {
        bsgen_MacroLine* l = bsgen_fetchUnit(&m->lines, k);
        printf("%d: %s\n", k, l->value);
    }
}

static bsgen_Macro bsgen_readMacro(xmlNode* node) {
    xmlNode* name_node = bsgen_getSingleChild(node, "name");

    if (!name_node) {
        bsgen_warning("Name is a required property of \"%s\"\n", node->name);
        return (bsgen_Macro) { 0 }; // todo warning
    }

    bsgen_Macro macro = {
        .name = xmlStrdup(name_node->children->content),
        .lines = BSGEN_LIST(bsgen_MacroLine),
    };

    xmlNode* child = node->children;
    while (child) {
        if (child->type == XML_ELEMENT_NODE) {
            if (xmlStrcmp(child->name, "line") == 0) {
                bsgen_MacroLine line = {
                    .value = xmlStrdup(child->children->content),
                };

                bsgen_pushBack(&macro.lines, &line);
            }
        }

        child = child->next;
    }

    return macro;
}

static void bsgen_readMacros(bsgen_Library* library, xmlNode* node) {
    xmlNode* child = node->children;

    while (child) {
        if (child->type == XML_ELEMENT_NODE) {
            if (xmlStrcmp(child->name, "macro") == 0) {
                bsgen_Macro macro = bsgen_readMacro(child);

                if (macro.name)
                    bsgen_pushBack(&library->macros, &macro);
            }
        }

        child = child->next;
    }
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
static bsgen_Param bsgen_readParam(xmlNode* node) {
    xmlNode* child = node->children;

    xmlNode* type_node = bsgen_getSingleChild(node, "type");
    xmlNode* name_node = bsgen_getSingleChild(node, "name");

    if (!type_node || !name_node)
        return (bsgen_Param) { 0 };

    bsgen_Param param = {
        .name = xmlStrdup(name_node->children->content),
        .type = xmlStrdup(type_node->children->content),
    };

    return param;
}

static bsgen_Function bsgen_readFunction(xmlNode* node) {
    char* function_name = bsgen_getProperty(node, "name");

    if (!function_name)
        return (bsgen_Function) { 0 };

    xmlNode* child = node->children;
    xmlNode* return_value = bsgen_getSingleChild(node, "return");
    if (!return_value)
        return (bsgen_Function) { 0 };

    bsgen_Function function = {
        .name = xmlStrdup(function_name),
        .return_value = xmlStrdup(return_value->children->content),
        .params = BSGEN_LIST(bsgen_Param),
    };

    while (child) {
        if (child->type == XML_ELEMENT_NODE) {
            if (xmlStrcmp(child->name, "param") == 0) {
                bsgen_Param param = bsgen_readParam(child);
                bsgen_pushBack(&function.params, &param);
            }
        }

        child = child->next;
    }

    return function;
}

static void bsgen_readFunctions(bsgen_Library* library, xmlNode* node) {
    xmlNode* child = node->children;

    while (child) {
        if (child->type == XML_ELEMENT_NODE) {
            if (xmlStrcmp(child->name, "function") == 0) {
                bsgen_Function function = bsgen_readFunction(child);

                if (function.name)
                    bsgen_pushBack(&library->functions, &function);
            }
        }

        child = child->next;
    }
}

 /**
  Library
  */
static bsgen_Library bsgen_readLibrary(const char* path) {
    bsgen_Library library = {
        .path = path,
        .macros = BSGEN_LIST(bsgen_Macro),
        .functions = BSGEN_LIST(bsgen_Function),
        .structures = BSGEN_LIST(bsgen_Structure),
    };

    xmlDoc* document = xmlReadFile(path, NULL, 0);
    xmlNode* node = xmlDocGetRootElement(document)->children;

    while (node) {
        if (xmlStrcmp(node->name, "macros") == 0)
            bsgen_readMacros(&library, node);
        else if (xmlStrcmp(node->name, "structures") == 0)
            bsgen_readStructures(&library, node);
        else if (xmlStrcmp(node->name, "functions") == 0)
            bsgen_readFunctions(&library, node);

        node = node->next;
    }

    return library;
}



 /*==============================================================================
  * Code Generation
  *============================================================================*/

static void bsgen_generateHeader(bsgen_Library* library, const char* name, const char* header_id, const char* api_prefix) {
    bsgen_String* string = NULL;

    string = bsgen_appendStringF(string, "#ifndef %s\n#define %s\n\n", header_id, header_id);

   /**
    Macros
    */
    for (int i = 0; i < library->macros.count; i++) {
        bsgen_Macro* macro = bsgen_fetchUnit(&library->macros, i);

        if (macro->lines.count > 0)
            string = bsgen_appendStringF(string, "%s \\\n", macro->name);
        else
            string = bsgen_appendStringF(string, "%s\n", macro->name);

        for (int j = 0; j < macro->lines.count; j++) {
            bsgen_MacroLine* line = bsgen_fetchUnit(&macro->lines, j);

            if (j == (macro->lines.count - 1))
                string = bsgen_appendStringF(string, "    %s\n", line->value);
            else
                string = bsgen_appendStringF(string, "    %s \\\n", line->value);
        }

        string = bsgen_appendString(string, "\n", 1);
    }

    string = bsgen_appendString(string, "\n", 1);

   /**
    Definitions
    */
    for (int i = 0; i < library->structures.count; i++) {
        bsgen_Structure* structure = bsgen_fetchUnit(&library->structures, i);
        string = bsgen_appendStringF(string, "typedef struct %s %s\n", structure->name, structure->name);
    }

    string = bsgen_appendString(string, "\n", 1);

   /**
    Declarations
    */
    for (int i = 0; i < library->structures.count; i++) {
        bsgen_Structure* structure = bsgen_fetchUnit(&library->structures, i);
        string = bsgen_appendStringF(string, "struct %s {\n", structure->name);
        
        for (int j = 0; j < structure->fields.count; j++) {
            bsgen_Field* field = bsgen_fetchUnit(&structure->fields, j);
            string = bsgen_appendStringF(string, "    %s %s;\n", field->type, field->name);
        }

        string = bsgen_appendString(string, "}\n\n", 3);
    }

   /**
    Functions
    */
    for (int i = 0; i < library->functions.count; i++) {
        bsgen_Function* function = bsgen_fetchUnit(&library->functions, i);

        string = bsgen_appendStringF(string, "%s %s\n%s(", api_prefix, function->return_value, function->name);

        for (int j = 0; j < function->params.count; j++) {
            bsgen_Param* param = bsgen_fetchUnit(&function->params, j);
            string = bsgen_appendStringF(string, "\n    %s %s", param->type, param->name);
            if (j != (function->params.count - 1))
                string = bsgen_appendString(string, ",", 1);
        }

        string = bsgen_appendString(string, ");\n\n", 4);
    }

    string = bsgen_appendStringF(string, "#endif");
    printf("%s\n", string->value);

    free(string);
}

int main(int argc, const char* argv[]) {
    bsgen_Library basilisk_core = bsgen_readLibrary("xml/basilisk-core.xml");

    bsgen_generateHeader(&basilisk_core, "basilisk.h", "BASILISK_H", "BSAPI");

	return 0;
}