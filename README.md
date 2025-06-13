# Avaliador de expressões numéricas

## Objetivo

Este trabalho prático tem por objetivo desenvolver um programa em linguagem C capaz de avaliar
expressões matemáticas escritas, com suporte a operações básicas e funções matemáticas especiais. O
sistema também deverá ser capaz de converter e avaliar expressões entre as formas infixada e pós-fixada,
utilizando pilhas como estrutura fundamental.

---

## O que deve ser feito

1. traduzir a expressão da notação infixada para notação pós-fixada;
2. traduzir a expressão da notação pós-fixada para notação infixada;
3. para operações com dois operandos, ser usados +, -, *, /, % e ^ para as operações matemáticas básicas;
4. ser usados raiz, sen, cos, tg e log para raiz quadrada, seno, cosseno, tangente e logaritmo de base 10;
5. os arcos a serem considerados para sen, cos e tg devem ser considerados em graus;
6. as operações indicadas no item D devem ser aplicadas a somente um operando;
7. avaliar expressões de forma a obter os valores constantes na seguinte tabela:

---

## Como Executar

1. Clone este repositório:
```bash
git clone https://github.com/oMegaTraiin/calculadora.git
```

2. Compile os arquivos:
```bash
gcc main.c func.c -o main.exe
```

3. Execute o programa:
```bash
./main.exe
```

---