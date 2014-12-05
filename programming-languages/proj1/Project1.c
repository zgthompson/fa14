#include <stdlib.h>
#include <stdio.h>
#include "lex.h"
#include "SetLimits.h"

int main (int argc, char * argv[])
{
    SetLimits();
    enum token_type t;
    if (argc < 2)
    {
        printf ("format: proj1 <filename>\n");
        exit (1);
    }
    lex_state ls;
    init_lex (&ls, argv[1]);
    while ((t = get_token(&ls)) != EOFT) {
        printf ("\t%-12s%s\n", token_names[t], get_lexeme(&ls));
    }
    end_lex(&ls);
    return 0;
}
