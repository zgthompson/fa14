#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "syn.h"
#include "SetLimits.h"

char *remove_extension(char *s) {

    char *new_s;

    if (s == NULL || (new_s = malloc(strlen(s) + 1)) == NULL) return NULL;

    strcpy(new_s, s);
    char *last_dot = strrchr(new_s, '.');
    if (last_dot != NULL) *last_dot = '\0';

    return new_s;
};


bool has_proper_extension(char *filename) {
    char *ch = filename;
    while (*ch != '\0') ++ch;
    return (*(--ch) == 'n' && *(--ch) == 'i' && *(--ch) == '.');
}


char *listname_from_basename(char *basename) {
    char *listname;

    if (basename == NULL || (listname = malloc(strlen(basename) + 5)) == NULL) return NULL;

    strcpy(listname, basename);
    char *ch = strchr(listname, '\0');
    if (ch != NULL) {
        *ch++ = '.';
        *ch++ = 'l';
        *ch++ = 's';
        *ch++ = 't';
        *ch++ = '\0';
    }

    return listname;
}

char *debugname_from_basename(char *basename) {
    char *debugname;

    if (basename == NULL || (debugname= malloc(strlen(basename) + 5)) == NULL) return NULL;

    strcpy(debugname, basename);
    char *ch = strchr(debugname, '\0');
    if (ch != NULL) {
        *ch++ = '.';
        *ch++ = 'd';
        *ch++ = 'b';
        *ch++ = 'g';
        *ch++ = '\0';
    }

    return debugname;
}

int main (int argc, char * argv[])
{
    SetLimits();

    if (argc < 2)
    {
        printf ("format: proj1 <filename>\n");
        exit(1);
    }

    char *filename = argv[1];


    if (!has_proper_extension(filename)) {
        printf("input file must have .in extension\n");
        exit(1);
    }

    char *basename = remove_extension(filename);
    char *listname = listname_from_basename(basename);
    char *debugname = debugname_from_basename(basename);

    FILE *input_file = fopen(filename, "r");
    FILE *output_file = fopen(listname, "w");
    FILE *debug_file = fopen(debugname, "w");

    if (input_file == NULL || output_file == NULL || debug_file == NULL) {
        printf ("error opening files\n");
        exit(1);
    }

    fprintf(output_file, "Input file: %s\n", filename);

    syn_state ss;

    init_syn(&ss, input_file, output_file, debug_file);
    parse_syn(&ss);
    end_syn(&ss);

    fclose(input_file);
    fclose(output_file);

    return 0;
}
