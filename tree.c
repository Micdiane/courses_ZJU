#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
int targetNum[1024];
int currentNum[1024];
int currentLayer = 0;
int handle_next_display = 1;
int getBranchNum(Node T)
{
    switch(T->kind)
    {
        case INTVAL:
            return 0;
        case PLUS:
            return 2;
        case TIMES:
            return 2;
        case ROOT:
            return 1;
        default:
            return 0;
    }
}
Node create_intval_node(int val)
{
    Node n = (Node)malloc(sizeof(struct Node_));
    n->kind = INTVAL;
    n->u.intval.val = val;
    return n;
}

Node create_add_node(Node left, Node right)
{
    Node n = (Node)malloc(sizeof(struct Node_));
    n->kind = PLUS;
    n->u.binop.left = left;
    n->u.binop.right = right;
    return n;
}

Node create_mul_node(Node left, Node right)
{
    Node n = (Node)malloc(sizeof(struct Node_));
    n->kind = TIMES;
    n->u.binop.left = left;
    n->u.binop.right = right;
    return n;
}

Node create_root_node(Node root){
    Node n = (Node)malloc(sizeof(struct Node_));
    n->kind = ROOT;
    n->u.root.root = root;
    return n;

}
void gapProcess()
{
	for(int i=1;i<=currentLayer;i++)
	{
		if(i < currentLayer)
		{
			if(currentNum[i] <= targetNum[i])
			{
				printf("│   ");
			}
			else
			{
				printf("   ");
			}
		}
		else
		{
			if(currentNum[i] >= targetNum[i])
			{
				printf("└──");
			}
			else
			{
				printf("├──");
			}
		}
	}
}

void gapmanage(Node node,int degree){
    currentLayer++;
    targetNum[currentLayer] = getBranchNum(node);
    currentNum[currentLayer] = 1;
    switch (degree)
    {
    case 1:
        print_tree1(node->u.binop.left);
        break;
    case 2:
        print_tree1(node->u.binop.left);
        print_tree1(node->u.binop.right);
    }
    currentLayer--;

}

void print_tree1(Node node)
{
    if (node == NULL) return;

    gapProcess();
    currentNum[currentLayer]++;

    switch (node->kind) {
    case INTVAL:
        printf("INT: %d\n", node->u.intval.val);
        break;
    case PLUS:
        printf("PLUS:\n");
        gapmanage(node,2);
        break;
    case TIMES:
        printf("TIMES:\n");
        gapmanage(node,2);
        break;
    case ROOT:
        printf("ROOT:\n");
        currentLayer++;
        targetNum[currentLayer] = getBranchNum(node);
        currentNum[currentLayer] = 1;
        print_tree1(node->u.root.root);
        currentLayer--;
        break;
    default:
        printf("UNKNOWN NODE TYPE\n");
        break;
    }
}