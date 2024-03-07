%{
#include <stdio.h>
#include "tree.h"
void yyerror(const char *s);
extern int yylex(void);
extern FILE* yyin;
struct Node_* root;
%}


%union {
    int val;
    struct Node_* node;
}

%token <val> INT
%type <node> Exp Factor Term
%token ADD MUL

%%

Calc : 
    | Exp { 
        root = create_root_node($1);
    }
    ;

Exp : Factor { $$ = $1; // 传递intval_node
    }

    | Exp ADD Factor { $$ = create_add_node($1, $3);
    }
    ;

Factor : Term { $$ = $1; // 传递intval_node
    
    }
    | Factor MUL Term { $$ = create_mul_node($1, $3);
     }
    ;

Term: INT { $$ = create_intval_node($1);
 }
    ;
%%
int main(int argc, char** argv) {
    if (argc > 1) {
        FILE* file = fopen(argv[1], "r");
        if (!file) {
            printf("error: cannot open file %s\n", argv[1]);
            return 1;
        }
        yyin = file;
    }
    yyparse();
    print_tree1(root);
    return 0;
}

void yyerror(const char *s) {
    printf("error: %s\n", s);
}
