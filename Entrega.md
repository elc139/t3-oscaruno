# T3: Scheduling com OpenMP

-   **Aluno**: Deivis Costa Pereira.
-   **Disciplina**: ELC139 - Programação Paralela.

## Programa

O programa [OpenMPDemoABC.cpp](https://github.com/elc139/t3-oscaruno/blob/master/OpenMPDemoABC.cpp) ilustra o efeito das opções de escalonamento disponíveis para a cláusula `schedule` do OpenMP, manipulando um array compartilhado.

A opção `runtime` faz uso da variável de ambiente `OMP_SCHEDULE`, a qual deve ser setada ao executar o programa. Desta forma o caso 4 -> `runtime` é o primeiro a ser executado para não sofrer efeitos das manipulações posteriores, do tipo de `schedule`, pela função [omp_set_schedule()](https://computing.llnl.gov/tutorials/openMP/#OMP_SET_SCHEDULE).

A seguir resultados para 4 Threads, Worksize de 67 e Chunks de 10. As colunas são, respectivamente, tipo de schedule, exclusão mútua ativa, utilização de Chunk, o array e quantidade de cada caracter.

``` bash
./OpenMPDemoABC 4 67 10
runtime mutex nope  ABCDABCDABCDABCDABCDABCDABCDABCDABCDABCDABCDABCDABCDABCDABCDABCDABC A=17 B=17 C=17 D=16 
runtime nope  nope  DBCADBCDABCDABCDABCDABCDABCADBCABDACBDACBDACBDACBACDBACDBACDBACD--- A=16 B=16 C=16 D=16 
static  mutex chunk DCBADCBADCBADCBADCBADCBADCBADCBADCBADCBACBACBACBACBACBACBACBABABABA A=20 B=20 C=17 D=10 
static  mutex nope  ABCDABCDABCDABCDABCDABCDABCDABCDABCDABCDABCDABCDABCDABCDABCDABCDABC A=17 B=17 C=17 D=16 
dynamic mutex chunk CADBCADBCADBCADBCADBCADBCADBCADBCADBCADBCADCADCADCADCADCADCADCACACA A=20 B=10 C=20 D=17 
dynamic mutex nope  CABDCABDCABDCABDCABDCABDCABDCABDCABDCABDCABDCABDCABDCABDCABDCABDCAB A=17 B=17 C=17 D=16 
auto    mutex nope  ABCDABCDABCDABCDABCDABCDABCDABCDABCDABCDABCDABCDABCDABCDABCDABCDABC A=17 B=17 C=17 D=16 
guided  mutex chunk CBADCBADCBADCBADCBADCBADCBADCBADCBADCBADCBACBACBACBACBACBACBACBCBCB A=17 B=20 C=20 D=10 
guided  mutex nope  CBADCBADCBADCBADCBADCBADCBADCBADCBADCBADCBADCBADCBADCBADCBADCBADCBA A=17 B=17 C=17 D=16 
static  nope  chunk BACBDCABDACBDBCADBCABCADBCADCBADCABDCABDCA-BCABCABCABCABCABA-BABAB- A=19 B=20 C=16 D=9 
static  nope  nope  DDCABDACBDCABDCABDCBADCABCDBACDBACDBACDBACDBACDBACDBACDBACDBAC-BA-- A=16 B=16 C=16 D=16 
dynamic nope  chunk DDCBACBDABCDABCDABCDBCDABCDABCDABCDABCDACBD-BCDBCDBCDBCDBCDBC-BCBC- A=9 B=19 C=19 D=17 
dynamic nope  nope  ABCADBCADBCADBCADBCDABCDABCADBCDABCADBCDABCADBCDABCADBCDABCDABCD--- A=16 B=16 C=16 D=16 
auto    nope  nope  DCDBACBDACBDACBDACBDCBDABCDABCDABCDACBDACBDACBDACBDACBDABCDABCDA--- A=15 B=16 C=16 D=17 
guided  nope  chunk BACDBACDBACDBCADBCADBCADBCADBADCBDABCDAB-DABDABDABDABDBADBAD-ADADA- A=19 B=17 C=9 D=19 
guided  nope  nope  BBDCABCDADBCABCDADCBADCBADCBACBDACBDACBDACBDACBDACBDACBDACBDACBD--- A=15 B=17 C=16 D=16
```

## Referências

- SPEH, Jaka. OpenMP: For & Scheduling.

    http://jakascorner.com/blog/2016/06/omp-for-scheduling.html

- SPEH, Jaka. OpenMP: Critical construct and zero matrix entries.

    http://jakascorner.com/blog/2016/07/omp-critical.html

- BARNEY, Blaise. OpenMP.

    https://computing.llnl.gov/tutorials/openMP/