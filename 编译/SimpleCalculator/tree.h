#ifndef TREE_H
#define TREE_H

typedef enum {
    INTVAL,
    PLUS,
    ROOT,
    TIMES
} nodekind;
typedef struct Node_ *Node;
struct Node_{
    nodekind kind;
    union
    {
        struct {int val;} intval;
        struct {Node left; Node right;} binop; 
        struct {Node root;} root;
    }u;  
} ;

Node create_intval_node(int val);
Node create_add_node(Node left, Node right);
Node create_mul_node(Node left, Node right);
Node create_root_node(Node root);
void print_tree(Node n,int d);
void print_tree1(Node node);

#endif // TREE_H
