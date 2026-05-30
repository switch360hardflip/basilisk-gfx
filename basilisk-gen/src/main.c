#include <stdlib.h>
#include <libxml/parser.h>

static void bsgen_build(const char* library) {
    xmlDoc* document;
    xmlNode* root, * first_child, * node;

    document = xmlReadFile("xml/basilisk-core.xml", NULL, 0);
    root = xmlDocGetRootElement(document);
    fprintf(stdout, "Root is <%s> (%i)\n", root->name, root->type);
    first_child = root->children;
    for (node = first_child; node; node = node->next) {
        fprintf(stdout, "\t Child is <%s> (%i)\n", node->name, node->type);
    }
    fprintf(stdout, "...\n");

}

int main(int argc, const char* argv[]) {
    bsgen_build("basilisk-core");

	return 0;
}