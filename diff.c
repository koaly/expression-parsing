#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
typedef struct NodeDesc *Node;
typedef struct NodeDesc
{
    char kind;        // plus, minus, times, divide, number, var
    int val;          // number: value
    Node left, right; // plus, minus, times, divide: children
} NodeDesc;
static FILE *f;
static int ch;
static unsigned int val;
enum
{
    plus,
    minus,
    times,
    divide,
    mod,
    lparen,
    rparen,
    number,
    var,
    eof,
    illegal
};
static void SInit(char *filename)
{
    ch = EOF;
    f = fopen(filename, "r+t");
    if (f != NULL)
        ch = getc(f);
}
static void Number()
{
    val = 0;
    while (('0' <= ch) && (ch <= '9'))
    {
        val = val * 10 + ch - '0';
        ch = getc(f);
    }
}
static int SGet()
{
    register int sym;

    while ((ch != EOF) && (ch <= ' '))
        ch = getc(f);
    switch (ch)
    {
    case EOF:
        sym = eof;
        break;
    case '+':
        sym = plus;
        ch = getc(f);
        break;
    case '-':
        sym = minus;
        ch = getc(f);
        break;
    case '*':
        sym = times;
        ch = getc(f);
        break;
    case '/':
        sym = divide;
        ch = getc(f);
        break;
    case '%':
        sym = mod;
        ch = getc(f);
        break;
    case '(':
        sym = lparen;
        ch = getc(f);
        break;
    case ')':
        sym = rparen;
        ch = getc(f);
        break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        sym = number;
        Number();
        break;
    case 'x':
        sym = var;
        ch = getc(f);
        break;
    default:
        sym = illegal;
    }
    return sym;
}
static int sym;
static Node Expr();
static Node Factor()
{
    register Node res;
    assert((sym == number) || (sym == var) || (sym == lparen));
    if (sym == number)
    {
        res = malloc(sizeof(NodeDesc));
        res->kind = number;
        res->val = val;
        res->left = NULL;
        res->right = NULL;
        sym = SGet();
    }
    else if (sym == var)
    {
        res = malloc(sizeof(NodeDesc));
        res->kind = var;
        res->left = NULL;
        res->right = NULL;
        sym = SGet();
    }
    else
    {
        sym = SGet();
        res = Expr();
        assert(sym == rparen);
        sym = SGet();
    }
    return res;
}
static Node Term()
{
    register Node root, res;

    root = Factor();
    while ((sym == times) || (sym == divide) || (sym == mod))
    {
        res = malloc(sizeof(NodeDesc));
        res->kind = sym;

        sym = SGet();
        res->left = root;
        res->right = Factor();
        root = res;
    }
    return root;
}
static Node Expr()
{
    register Node root, res;

    root = Term();
    while ((sym == plus) || (sym == minus))
    {
        res = malloc(sizeof(NodeDesc));
        res->kind = sym;

        sym = SGet();
        res->left = root;
        res->right = Term();
        root = res;
    }
    return root;
}
static Node diff(Node root)
{
    Node res;
    if ((root->kind == number) || (root->kind == var))
    {
        res = malloc(sizeof(NodeDesc));
        if (root->kind == number)
            res->val = 0;
        else
            res->val = 1;
        res->kind = number;
        res->left = NULL;
        res->right = NULL;
        return res;
    }
    else if ((root->kind == plus) || (root->kind == minus))
    {
        res = malloc(sizeof(NodeDesc));
        res->kind = root->kind;
        res->left = diff(root->left);
        res->right = diff(root->right);
        return res;
    }
    else if (root->kind == times)
    {
        Node l, r;
        res = malloc(sizeof(NodeDesc));
        l = malloc(sizeof(NodeDesc));
        r = malloc(sizeof(NodeDesc));

        l->kind = times;
        l->left = diff(root->left);
        l->right = root->right;

        r->kind = times;
        r->left = root->left;
        r->right = diff(root->right);

        res->kind = plus;
        res->left = l;
        res->right = r;
        return res;
    }
}
static void Print(Node root, int level)
{
    register int i;
    if (root != NULL)
    {
        Print(root->right, level + 1);
        for (i = 0; i < level; i++)
            printf(" ");
        switch (root->kind)
        {
        case plus:
            printf("+\n");
            break;
        case minus:
            printf("-\n");
            break;
        case times:
            printf("*\n");
            break;
        case divide:
            printf("/\n");
            break;
        case number:
            printf("%ld\n", root->val);
            break;
        case var:
            printf("x ");
            break;
        }
        Print(root->left, level + 1);
    }
}
static void PrintPrefix(Node root)
{
    if (root != NULL)
    {
        switch (root->kind)
        {
        case plus:
            printf("+ ");
            break;
        case minus:
            printf("- ");
            break;
        case times:
            printf("* ");
            break;
        case divide:
            printf("/ ");
            break;
        case number:
            printf("%d ", root->val);
            break;
        case var:
            printf("x ");
            break;
        }
        PrintPrefix(root->left);
        PrintPrefix(root->right);
    }
}
static void PrintInfix(Node root)
{
    if (root != NULL)
    {
        if (root->kind != number && root->kind != var)
            printf("( ");
        PrintInfix(root->left);
        switch (root->kind)
        {
        case plus:
            printf("+ ");
            break;
        case minus:
            printf("- ");
            break;
        case times:
            printf("* ");
            break;
        case divide:
            printf("/ ");
            break;
        case number:
            printf("%d ", root->val);
            break;
        case var:
            printf("x ");
            break;
        }
        PrintInfix(root->right);
        if (root->kind != number && root->kind != var)
            printf(") ");
    }
}
static void PrintPostfix(Node root)
{
    if (root != NULL)
    {
        PrintPostfix(root->left);
        PrintPostfix(root->right);
        switch (root->kind)
        {
        case plus:
            printf("+ ");
            break;
        case minus:
            printf("- ");
            break;
        case times:
            printf("* ");
            break;
        case divide:
            printf("/ ");
            break;
        case number:
            printf("%d ", root->val);
            break;
        case var:
            printf("x ");
            break;
        }
    }
}
int main(int argc, char *argv[])
{
    register Node result, result_prime;

    if (argc == 2)
    {
        SInit(argv[1]);
        sym = SGet();
        result = Expr();
        printf(" Pre diff: ");
        PrintInfix(result);
        printf("\n");

        result_prime = diff(result);
        printf("Post diff: ");
        PrintInfix(result_prime);
        printf("\n");
        assert(sym == eof);
    }
    else
    {
        printf("usage: expreval <filename>\n");
    }
    return 0;
}