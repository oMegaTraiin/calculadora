#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "expressao.h"

#define PI 3.14159265358979323846

typedef struct Pilha
{
    float valor;
    struct Pilha *anterior;
} Pilha;

Pilha *push(Pilha **pilha, float valor)
{
    Pilha *P = malloc(sizeof(Pilha));
    if (P == NULL)
    {
        printf("Erro na alocação de memoria.\n");
        return NULL;
    }
    P->valor = valor;
    P->anterior = *pilha;
    return P;
}

Pilha *pop(Pilha **pilha)
{
    Pilha *P = NULL;
    if (*pilha)
    {
        P = *pilha;
        *pilha = P->anterior;
    }
    else
        printf("Pilha vazia.\n");
    return P;
}

int isOperator(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^';
}

int precedence(char op)
{
    switch (op)
    {
    case '^':
        return 4;
    case '*':
    case '/':
    case '%':
        return 3;
    case '+':
    case '-':
        return 2;
    default:
        return 0;
    }
}

float toRadians(float degrees)
{
    return degrees * PI / 180.0f;
}

char *normalizeDecimalSeparator(char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == ',')
        {
            str[i] = '.';
        }
    }
    return str;
}

char getMainOperator(const char *expr)
{
    int level = 0;
    for (int i = 0; expr[i]; i++)
    {
        if (expr[i] == '(')
            level++;
        else if (expr[i] == ')')
            level--;
        else if (level == 0 && isOperator(expr[i]))
            return expr[i];
    }
    return '\0';
}

char *getFormaInFixa(char *str)
{
    static char infixa[512];
    typedef struct
    {
        char expr[256];
    } Node;

    Node stack[64];
    int top = -1;
    char token[64];
    int i = 0;

    while (str[i])
    {
        while (isspace(str[i]))
            i++;

        int k = 0;
        while (str[i] && !isspace(str[i]))
            token[k++] = str[i++];
        token[k] = '\0';

        if (isdigit(token[0]) || token[0] == '.')
        {
            snprintf(stack[++top].expr, sizeof(stack[0].expr), "%s", token);
        }

        else if (!strcmp(token, "sen") || !strcmp(token, "cos") || !strcmp(token, "tg") || !strcmp(token, "log") || !strcmp(token, "raiz"))
        {
            snprintf(stack[top].expr, sizeof(stack[0].expr), "%s(%s)", token, stack[top].expr);
        }

        else if (isOperator(token[0]) && strlen(token) == 1)
        {
            char op = token[0];
            char *a = stack[top - 1].expr;
            char *b = stack[top].expr;

            char oa = getMainOperator(a), ob = getMainOperator(b);
            int pa = precedence(oa), pb = precedence(ob), p = precedence(op);

            char exprA[256], exprB[256];
            snprintf(exprA, sizeof(exprA), (oa && pa < p) ? "(%s)" : "%s", a);
            snprintf(exprB, sizeof(exprB), (ob && (pb < p || (pb == p && op != '^'))) ? "(%s)" : "%s", b);

            snprintf(stack[top - 1].expr, sizeof(stack[0].expr), "%s %c %s", exprA, op, exprB);
            top--;
        }
    }

    snprintf(infixa, sizeof(infixa), "%s", top >= 0 ? stack[top].expr : "");
    return infixa;
}

char *getFormaPosFixa(char *str)
{
    static char posfixa[512];
    posfixa[0] = '\0';

    char stack[64][32];
    int top = -1, j = 0;
    char token[255];
    int i = 0;

    while (str[i])
    {
        if (isspace(str[i]))
        {
            i++;
            continue;
        }

        if (isdigit(str[i]) || str[i] == '.' || str[i] == ',')
        {
            while (isdigit(str[i]) || str[i] == '.' || str[i] == ',')
                posfixa[j++] = (str[i] == ',') ? '.' : str[i++];
            posfixa[j++] = ' ';
        }

        else if (isalpha(str[i]))
        {
            int k = 0;
            while (isalpha(str[i]))
                token[k++] = str[i++];
            token[k] = '\0';
            strcpy(stack[++top], token);
        }

        else if (str[i] == '(')
        {
            strcpy(stack[++top], "(");
            i++;
        }

        else if (str[i] == ')')
        {
            while (top >= 0 && strcmp(stack[top], "(") != 0)
                j += sprintf(&posfixa[j], "%s ", stack[top--]);
            if (top >= 0 && strcmp(stack[top], "(") == 0)
                top--;                              
            if (top >= 0 && isalpha(stack[top][0]))
                j += sprintf(&posfixa[j], "%s ", stack[top--]);
            i++;
        }

        else if (isOperator(str[i]))
        {
            char op[2] = {str[i], '\0'};
            while (top >= 0 && isOperator(stack[top][0]) &&
                   precedence(stack[top][0]) >= precedence(op[0]))
            {
                j += sprintf(&posfixa[j], "%s ", stack[top--]);
            }
            strcpy(stack[++top], op);
            i++;
        }

        else
        {
            i++;
        }
    }

    while (top >= 0)
        j += sprintf(&posfixa[j], "%s ", stack[top--]);

    posfixa[j] = '\0';
    return posfixa;
}

float getValorPosFixa(char *StrPosFixa)
{
    Pilha *stack = NULL;
    char token[255];
    int i = 0;

    // DEBUG : printf("\n=== Início da avaliação pós-fixa ===\n");
    Pilha *node_a, *node_b, *node = NULL;

    while (StrPosFixa[i] != '\0')
    {
        if (isspace(StrPosFixa[i]))
        {
            i++;
            continue;
        }

        if (isdigit(StrPosFixa[i]) || StrPosFixa[i] == '.' || StrPosFixa[i] == ',')
        {
            int k = 0;
            while (isdigit(StrPosFixa[i]) || StrPosFixa[i] == '.')
                token[k++] = StrPosFixa[i++];
            token[k] = '\0';

            // DEBUG : printf("[DEBUG] Empilhando número: %.2f\n", atof(token));
            stack = push(&stack, atof(token));
        }
        else if (isalpha(StrPosFixa[i]))
        {
            int k = 0;
            while (isalpha(StrPosFixa[i]))
                token[k++] = StrPosFixa[i++];
            token[k] = '\0';

            node = pop(&stack);
            if (!node)
            {
                // DEBUG : printf("Erro: pilha vazia ao aplicar função '%s'\n", token);
                return 0.0f;
            }

            float a = node->valor;
            free(node);

            float res = 0.0f;
            if (strcmp(token, "sen") == 0)
                res = sinf(toRadians(a));
            else if (strcmp(token, "cos") == 0)
                res = cosf(toRadians(a));
            else if (strcmp(token, "tg") == 0)
                res = tanf(toRadians(a));
            else if (strcmp(token, "log") == 0)
                res = log10f(a);
            else if (strcmp(token, "raiz") == 0)
                res = sqrtf(a);
            else
            {
                // DEBUG : printf("Função desconhecida: %s\n", token);
                res = 0.0f;
            }

            // DEBUG : printf("[DEBUG] Aplicando função %s(%.2f) = %.2f\n", token, a, res);
            stack = push(&stack, res);
        }
        else if (isOperator(StrPosFixa[i]))
        {
            char op = StrPosFixa[i++];
            node_b = pop(&stack);
            node_a = pop(&stack);

            float a = node_a->valor;
            float b = node_b->valor;
            free(node_a);
            free(node_b);
            float r = 0.0f;
            switch (op)
            {
            case '+':
                r = a + b;
                break;
            case '-':
                r = a - b;
                break;
            case '*':
                r = a * b;
                break;
            case '/':
                r = a / b;
                break;
            case '%':
                r = fmodf(a, b);
                break;
            case '^':
                r = powf(a, b);
                break;
            default:
                // DEBUG : printf("Operador desconhecido: %c\n", op);
                r = 0.0f;
                break;
            }

            // DEBUG : printf("[DEBUG] Operação: %.2f %c %.2f = %.2f\n", a, op, b, r);
            stack = push(&stack, r);
        }
        else
        {
            // DEBUG : printf("[DEBUG] Ignorando caractere: '%c'\n", StrPosFixa[i]);
            i++;
        }
    }

    if (stack == NULL)
    {
        printf("Erro: pilha vazia no final\n");
        return 0.0f;
    }

    float result = stack->valor;
    // DEBUG : printf("[DEBUG] Resultado final: %.2f\n", result);
    free(stack);

    // DEBUG : printf("=== Fim da avaliação ===\n");
    return result;
}

float getValorInFixa(char *strInfixa)
{
    return getValorPosFixa(getFormaPosFixa(normalizeDecimalSeparator(strInfixa)));
}