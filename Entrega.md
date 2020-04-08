# T3: Scheduling com OpenMP

-   **Aluno**: Deivis Costa Pereira.
-   **Disciplina**: ELC139 - Programação Paralela.

## Programa

O programa [OpenMPDemoABC2.cpp](https://github.com/elc139/t3-oscaruno/blob/master/OpenMPDemoABC2.cpp) ilustra o efeito das opções de escalonamento disponíveis para a cláusula `schedule` do OpenMP, manipulando um array compartilhado.

A opção `runtime` faz uso da variável de ambiente `OMP_SCHEDULE`, a qual deve ser setada ao executar o programa. Desta forma o caso 4 -> `runtime` é o primeiro a ser executado para não sofrer efeitos das manipulações posteriores, do tipo de `schedule`, pela função [omp_set_schedule()](https://computing.llnl.gov/tutorials/openMP/#OMP_SET_SCHEDULE).

A seguir resultados para 4 Threads, Worksize de 67 e Chunks de 10. As colunas são, respectivamente, tipo de schedule, exclusão mútua ativa, utilização de Chunk, o array e quantidade de cada caracter.

``` bash
./OpenMPDemoABC2 4 67 10
runtime mutex nope  AAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBCCCCCCCCCCCCCCCCCDDDDDDDDDDDDDDDD A=17 B=17 C=17 D=16 
runtime nope  nope  C--A-AAAAAAAAAAAAAAABBBBBBBBBBBBBBBBCCCCCCCCCCCCCCCCDDDDDDDDDDDDDDD A=16 B=16 C=17 D=15 
static  mutex chunk AAAAAAAAAABBBBBBBBBBCCCCCCCCCCDDDDDDDDDDAAAAAAAAAABBBBBBBBBBCCCCCCC A=20 B=20 C=17 D=10 
static  mutex nope  AAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBCCCCCCCCCCCCCCCCCDDDDDDDDDDDDDDDD A=17 B=17 C=17 D=16 
dynamic mutex chunk DDDDDDDDDDCCCCCCCCCCAAAAAAAAAABBBBBBBBBBDDDDDDDDDDCCCCCCCCCCAAAAAAA A=17 B=10 C=20 D=20 
dynamic mutex nope  ACDBACDBACDBACDBACDBACDBACDBACDBACDBACDBACDBACDBACDBACDBACDBACDBACD A=17 B=16 C=17 D=17 
auto    mutex nope  CCCCCCCCDDDDDDDAAAAAAABBBBBCCCCCDDDDAAAABBBCCCDDDAABBCCDABCDABCDABC A=16 B=13 C=21 D=17 
guided  mutex chunk CCCCCCCCCCDDDDDDDDDDBBBBBBBBBBAAAAAAAAAACCCCCCCCCCDDDDDDDDDDBBBBBBB A=10 B=17 C=20 D=20 
guided  mutex nope  BBBBBBBBCCCCCCCDDDDDDAAAAABBBBBCCCCDDDDAAABBBCCDDAABBCDABCDABCDABCD A=13 B=21 C=17 D=16 
static  nope  chunk C--A-AAAAAAAABB-BBBBBBBC-CCCCCCCCDD-DDDDDDDAAAAAAAAABBBBBBBBBCCCCCC A=18 B=18 C=16 D=9 
static  nope  nope  D---AAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBCCCCCCCCCCCCCCCCDDDDDDDDDDDDDDD A=16 B=16 C=16 D=16 
dynamic nope  chunk D-A--AAAAAAAAB-BBBBBBBBCC-CCCCCCCD-DDDDDDDDAAAAAAAAABBBBBBBBBCCCCCC A=18 B=18 C=15 D=10 
dynamic nope  nope  D-A-DCABDCA-DC--DCA-DCABDC--DCA-DC-ADCA-B--CBCA-DCA-DCA-DCA-DCA-B-- A=13 B=5 C=15 D=14 
auto    nope  nope  B---AAAAAAAAC-CCCCD-DDDDDBB-BBA-AAACC-CDD-DBB-A-ACC-DB-A-DB---BA--- A=16 B=10 C=10 D=11 
guided  nope  chunk C-D--DDDDDDDDDA-AAAAAAACC-CCCCCCCB-BBBBBBBBDDDDDDDDDAAAAAAAAACCCCCC A=17 B=9 C=16 D=19 
guided  nope  nope  B-C--CCCCCCD-DDDDDA-AAAABB-BBCC-CCDD-DAA-ABB-CC-DD--BC--D-C-AC-DB-- A=9 B=9 C=16 D=13 
```

A opção `static` faz com que as threads sejam escalonadas(tenham atribuição de trabalho) de forma circular, isso é visível na execução com mutex e chunk onde cada thread tenta executar 10x seguindo a ordem A-B-C-D. A opção sem chunk tenta igualar as cargas de trabalho mas seguindo o mesmo pricípio.

A opção `dynamic` faz com que as threads sejam escalonadas(tenham atribuição de trabalho) sem um padrão definido, fazendo com que elas sejam escalonadas conforme disponibilidade, e executando o chunk setado. Quando não tem o chunk especificado o mesmo é setado para 1.

A opção `auto` deixa a cargo do compilador escolher o `schedule` a ser utilizado, na saída acima foi do tipo `guided`.

A opção `guided` assim como a `dynamic` támbem são escalonadas conforme disponibilidade, mas o tamanho do trabalho vai reduzindo a cada solicitação de mais blocos pela thread.

## Referências

- SPEH, Jaka. OpenMP: For & Scheduling.

    http://jakascorner.com/blog/2016/06/omp-for-scheduling.html

- SPEH, Jaka. OpenMP: Critical construct and zero matrix entries.

    http://jakascorner.com/blog/2016/07/omp-critical.html

- BARNEY, Blaise. OpenMP.

    https://computing.llnl.gov/tutorials/openMP/

- OpenMP API Specification - Cláusula Ordered

    https://www.openmp.org/wp-content/uploads/OpenMP-API-Specification-5.0.pdf#page=270

- Tutorial OpenMP - Cláusula Ordered

    http://www.inf.ufrgs.br/~afarah/files/openmp.pdf#page=28