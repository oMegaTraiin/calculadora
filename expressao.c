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

typedef struct PilhaStr
{
    char str[512];
    struct PilhaStr *anterior;
} PilhaStr;

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

PilhaStr *pushStr(PilhaStr *pilha, const char *str)
{
    PilhaStr *P = malloc(sizeof(PilhaStr));
    if (!P)
    {
        printf("Erro de alocação de memória.\n");
        return pilha;
    }
    strcpy(P->str, str);
    P->anterior = pilha;
    return P;
}

PilhaStr *popStr(PilhaStr *pilha)
{
    if (pilha == NULL)
    {
        printf("Pilha vazia.\n");
        return NULL;
    }
    return pilha->anterior;
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
    PilhaStr *P = NULL;
    char token[512];
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
            PilhaStr *n = malloc(sizeof(PilhaStr));
            snprintf(n->str, sizeof(n->str), "%s", token);
            n->anterior = P;
            P = n;
        }
        else if (!strcmp(token, "sen") || !strcmp(token, "cos") || !strcmp(token, "tg") || !strcmp(token, "log") || !strcmp(token, "raiz"))
        {
            PilhaStr *a = P;
            P = P->anterior;
            PilhaStr *n = malloc(sizeof(PilhaStr));
            snprintf(n->str, sizeof(n->str), "%s(%s)", token, a->str);
            n->anterior = P;
            P = n;
            free(a);
        }
        else if (isOperator(token[0]) && strlen(token) == 1)
        {
            char op = token[0], exprA[256], exprB[256], oa, ob;
            PilhaStr *b = P;
            P = P->anterior;
            PilhaStr *a = P;
            P = P->anterior;
            int pa = precedence(oa = getMainOperator(a->str)), pb = precedence(ob = getMainOperator(b->str)), p = precedence(op);
            snprintf(exprA, sizeof(exprA), (oa && pa < p) ? "(%s)" : "%s", a->str);
            snprintf(exprB, sizeof(exprB), (ob && (pb < p || (pb == p && op != '^'))) ? "(%s)" : "%s", b->str);
            PilhaStr *n = malloc(sizeof(PilhaStr));
            snprintf(n->str, sizeof(n->str), "%s %c %s", exprA, op, exprB);
            n->anterior = P;
            P = n;
            free(a);
            free(b);
        }
    }

    static char final[512];
    snprintf(final, sizeof(final), "%s", P ? P->str : "");
    if (P)
        free(P);
    return final;
}

char *getFormaPosFixa(char *str)
{
    static char posfixa[512];
    posfixa[0] = '\0';
    PilhaStr *P = NULL;
    char token[512];
    int i = 0, j = 0;

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
            P = pushStr(P, token);
        }
        else if (str[i] == '(')
            P = pushStr(P, "("), i++;
        else if (str[i] == ')')
        {
            while (P && strcmp(P->str, "("))
                j += sprintf(&posfixa[j], "%s ", P->str), P = popStr(P);
            if (P)
                P = popStr(P);
            if (P && isalpha(P->str[0]))
                j += sprintf(&posfixa[j], "%s ", P->str), P = popStr(P);
            i++;
        }
        else if (isOperator(str[i]))
        {
            char op[2] = {str[i], '\0'};
            while (P && isOperator(P->str[0]) && precedence(P->str[0]) >= precedence(op[0]))
                j += sprintf(&posfixa[j], "%s ", P->str), P = popStr(P);
            P = pushStr(P, op);
            i++;
        }
        else
            i++;
    }

    while (P)
        j += sprintf(&posfixa[j], "%s ", P->str), P = popStr(P);

    posfixa[j] = '\0';
    return posfixa;
}

float getValorPosFixa(char *StrPosFixa)
{
    Pilha *stack = NULL;
    char token[64];
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

        // Número
        if (isdigit(StrPosFixa[i]) || StrPosFixa[i] == '.' || StrPosFixa[i] == ',')
        {
            int k = 0;
            while (isdigit(StrPosFixa[i]) || StrPosFixa[i] == '.')
                token[k++] = StrPosFixa[i++];
            token[k] = '\0';

            // DEBUG : printf("[DEBUG] Empilhando número: %.2f\n", atof(token));
            stack = push(&stack, atof(token));
        }
        // Função
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
        // Operadores
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