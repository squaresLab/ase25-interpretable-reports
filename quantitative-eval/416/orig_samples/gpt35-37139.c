//FormAI DATASET v1.0 Category: Building a XML Parser ; Style: rigorous
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_DEPTH 100

struct XMLNode {
    char* name; // tag name
    char* value; // tag value
    int depth; // depth of the node in the XML tree
};

struct XMLParser {
    FILE* file; // file pointer
    int depth; // current depth of the parsed node
    struct XMLNode node; // current parsed node
};

// function to create a new XML node
struct XMLNode* new_node(const char* name, const char* value, int depth) {
    struct XMLNode* node = (struct XMLNode*)malloc(sizeof(struct XMLNode));
    node->name = (char*)malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(node->name, name);
    if (value != NULL) {
        node->value = (char*)malloc(sizeof(char) * (strlen(value) + 1));
        strcpy(node->value, value);
    }
    else {
        node->value = NULL;
    }
    node->depth = depth;
    return node;
}

// function to free an XML node
void free_node(struct XMLNode* node) {
    free(node->name);
    if (node->value != NULL) {
        free(node->value);
    }
    free(node);
}

// function to parse the next tag from the XML file
int parse_tag(struct XMLParser* parser) {
    int c;
    char tag_name[100], tag_value[100];
    int name_len = 0, value_len = 0;

    // skip whitespace
    do {
        c = fgetc(parser->file);
        if (c == EOF) {
            return 0;
        }
    } while (c == ' ' || c == '\t' || c == '\n');

    // parse tag name
    if (c == '<') {
        // skip the opening '<'
        c = fgetc(parser->file);

        while (c != ' ' && c != '>' && c != '/') {
            if (name_len < 99) {
                tag_name[name_len++] = c;
            }
            c = fgetc(parser->file);
        }
        tag_name[name_len] = '\0';

        // parse attributes
        while (c != '>') {
            if (c == '/') {
                // empty tag
                c = fgetc(parser->file);
                if (c != '>') {
                    return 0;
                }
                parser->node.name = tag_name;
                parser->node.value = NULL;
                parser->node.depth = parser->depth;
                return 1;
            }

            name_len = 0;
            value_len = 0;
            while (c != '=' && c != ' ') {
                c = fgetc(parser->file);
                if (c == EOF) {
                    return 0;
                }
            }
            if (c == ' ') {
                c = fgetc(parser->file);
            }
            while (c != '"' && c != '\'') {
                c = fgetc(parser->file);
                if (c == EOF) {
                    return 0;
                }
            }
            c = fgetc(parser->file);
            while (c != '"' && c != '\'') {
                if (value_len < 99) {
                    tag_value[value_len++] = c;
                }
                c = fgetc(parser->file);
                if (c == EOF) {
                    return 0;
                }
            }
            tag_value[value_len] = '\0';

            // skip whitespace after attribute
            do {
                c = fgetc(parser->file);
            } while (c == ' ' || c == '\t' || c == '\n');
        }

        // parse tag value
        name_len = 0;
        value_len = 0;
        while (1) {
            c = fgetc(parser->file);
            if (c == '<') {
                // end of tag
                break;
            }
            else if (c == '&') {
                // parse escape sequence
                char esc[10];
                int esc_len = 0;
                while (c != ';') {
                    c = fgetc(parser->file);
                    if (esc_len < 9) {
                        esc[esc_len++] = c;
                    }
                }
                esc[esc_len - 1] = '\0';
                if (strcmp(esc, "lt") == 0) {
                    tag_value[value_len++] = '<';
                }
                else if (strcmp(esc, "gt") == 0) {
                    tag_value[value_len++] = '>';
                }
                else if (strcmp(esc, "amp") == 0) {
                    tag_value[value_len++] = '&';
                }
                else if (strcmp(esc, "apos") == 0) {
                    tag_value[value_len++] = '\'';
                }
                else if (strcmp(esc, "quot") == 0) {
                    tag_value[value_len++] = '"';
                }
                else {
                    // unknown escape sequence
                    return 0;
                }
            }
            else {
                if (value_len < 99) {
                    tag_value[value_len++] = c;
                }
            }
            if (c == EOF) {
                return 0;
            }
        }
        tag_value[value_len] = '\0';

        // create new node
        parser->node.name = tag_name;
        parser->node.value = tag_value;
        parser->node.depth = parser->depth;
        return 1;
    }
    else {
        // invalid tag
        return 0;
    }
}

// recursive function to parse an XML tree
void parse_tree(struct XMLParser* parser) {
    while (parse_tag(parser)) {
        printf("%*s<%s", parser->node.depth * 2, "", parser->node.name);
        if (parser->node.value == NULL) {
            printf(" />\n");
        }
        else {
            printf(">%s</%s>\n", parser->node.value, parser->node.name);
        }
        if (strcmp(parser->node.name, "/") != 0 && strcmp(parser->node.name, "?xml") != 0) {
            parser->depth++;
            parse_tree(parser);
            parser->depth--;
        }
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("fopen");
        return 1;
    }

    struct XMLParser parser;
    parser.file = file;
    parser.depth = 0;
    parser.node.name = NULL;
    parser.node.value = NULL;

    parse_tree(&parser);

    fclose(file);
    return 0;
}