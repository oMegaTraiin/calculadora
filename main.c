#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "expressao.h"

int main()
{
    // Nesse teste iremos testa a função
    // traduzir a expressão da notação infixada para notação pós-fixada;

    // Valor: (3 + 4) * 5
    // Resultado esperado: 3 4 + 5 *

    printf("Teste de tradução de infixa para posfixa:\n");
    Expressao T1;
    strcpy(T1.inFixa, "(3 + 4) * 5");
    strcpy(T1.posFixa, getFormaPosFixa(T1.inFixa));
    printf("Expressão Infixa: %s\n", T1.inFixa);
    printf("Expressão Posfixa: %s\n", T1.posFixa);
    printf("====================================================\n");


    // Nesse teste iremos testa a função
    // traduzir a expressão da notação pós-fixada para notação infixada;

    // Valor: 7 2 * 4 +
    // Resultado esperado: 7 * 2 + 4

    printf("Teste de tradução de posfixa para infixa:\n");
    Expressao T2;
    strcpy(T2.posFixa, "7 2 * 4 +");
    strcpy(T2.inFixa, getFormaInFixa(T2.posFixa));
    printf("Expressão Posfixa: %s\n", T2.posFixa);
    printf("Expressão Infixa: %s\n", T2.inFixa);
    printf("====================================================\n");

    // Nesse teste iremos testa a função
    // executar os calculos e obter os resultados esperados;

    // Matematica Basica

    // Calculo: 8 + (5 * (2 + 4))
    // Resultado esperado: 38

    printf("Teste de calculo com matematica basica:\n");
    Expressao T3;
    strcpy(T3.inFixa, "8 + (5 * (2 + 4))");
    printf("Expressão: %s\n", T3.inFixa);
    printf("Resultado: %.2f\n", getValorInFixa(T3.inFixa));
    printf("====================================================\n");

    // Calculo: (6 / 2 + 3) * 4
    // Resultado esperado: 24

    printf("Teste de calculo com matematica basica:\n");
    Expressao T4;
    strcpy(T4.inFixa, "(6 / 2 + 3) * 4");
    printf("Expressão: %s\n", T4.inFixa);
    printf("Resultado: %.2f\n", getValorInFixa(T4.inFixa));
    printf("====================================================\n");

    // Nesse teste iremos testa a função
    // executar os calculos e obter os resultados esperados;

    // Matematica Avançada

    // Calculo: log(2 + 3) / 5
    // Resultado esperado: Aprox. 0.14

    printf("Teste de calculo com matematica avançada:\n");
    Expressao T5;
    strcpy(T5.inFixa, "log(2 + 3) / 5");
    printf("Expressão: %s\n", T5.inFixa);
    printf("Resultado: %.2f\n", getValorInFixa(T5.inFixa));
    printf("====================================================\n");

    // Calculo: (45 + 60) * cos(30)
    // Resultado esperado: Aprox. 90,93

    printf("Teste de calculo com matematica avançada:\n");
    Expressao T6;
    strcpy(T6.inFixa, "(45 + 60) * cos(30)");
    printf("Expressão: %s\n", T6.inFixa);
    printf("Resultado: %.2f\n", getValorInFixa(T6.inFixa));
    printf("====================================================\n");

    return 0;
}