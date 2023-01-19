#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#define TAM 5
#define Entrada 0
#define Saida (TAM-1)

typedef struct ripley_s {
    
    //O v�rtice onde a Ripley est�
    int v;

    //Cores de cada v�rtice
    char cor[TAM];

    //Pilha com os v�rtices que Ripley viu
    int S[TAM];

    //Auxiliar que conta o tamanho da pilha
    int aux;
} ripley_t;



int** CriaMatriz(int** matriz) {
    int i;

    matriz = (int**)malloc(TAM * sizeof(int*));
    for (i = 0; i < TAM; i++) {
        matriz[i] = (int*)malloc(TAM * sizeof(int));
    }
    return matriz;
}


void LiberaMatriz(int** matriz) {
    int i;

    for (i = 0; i < TAM; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

void PrintMatriz(int** matriz) {
    int i, j;
    printf("\nMatriz de Adjacencias:\n");
    for (i = 0; i < TAM; i++) {
        printf("\t|");
        for (j = 0; j < TAM; j++) {
            if (matriz[i][j] == 2) printf("\033[1;32mA \033[1;37m");
            else printf("%d ", matriz[i][j]);
        }
        printf("|\n");
    }
}

int** aleatorizaLabirinto(int** matriz) {

    srand(time(NULL));
    int i, j, robson;
    int grausEnt = 0, grausSaid = 0;
    //deixa todos 0
    for (i = 0; i < TAM; i++) {
        for (j = 0; j < TAM; j++) {
            matriz[i][j] = 0;
        }
    }
    //coloca pelo menos uma aresta em um
    for (i = 0; i < TAM; i++) {
        j = (rand() % (TAM - 1));
        if (j == i) {
            j++;
        }
        matriz[i][j] = matriz[j][i] = 1;
    }
    //mais alguns
    for (robson = 0; robson < TAM; robson++) {
        i = (rand() % TAM);
        j = (rand() % TAM);
        if (i != j) {
            matriz[i][j] = matriz[j][i] = 1;
        }
    }

    //fazer com que a entrada e saida n�o estejam diretamentes ligados
    matriz[Entrada][Saida] = matriz[Saida][Entrada] = 0;

    for (i = 0; i < TAM; i++) {
        grausEnt += matriz[Entrada][i];
    }
    if (grausEnt == 0) {
        j = (rand() % (TAM - 2));
        if (j == 0) j = 3;
        matriz[Entrada][j] = matriz[j][Entrada] = 1;
    }
    for (i = 0; i < TAM; i++) {
        grausSaid += matriz[Saida][i];
    }
    if (grausSaid == 0) {
        j = (rand() % (TAM - 2));
        if (j == 0) j = 3;
        matriz[Saida][j] = matriz[j][Saida] = 1;
    }


    return matriz;
}

int** aleatorizaAlien(int** matriz) {

    int i, j, existe = 1;
    while (existe) {
        i = (rand() % TAM);
        j = (rand() % TAM);
        if (matriz[i][j] == 1) {
            existe = 0;
        }
    }
    matriz[i][j] = matriz[j][i] = 2;
    return matriz;
}

int** voltaAlien(int** matriz) {
    int i, j;
    for (i = 0; i < TAM; i++) {
        for (j = 0; j < TAM; j++) {
            if (matriz[i][j] == 2) {
                matriz[i][j] = matriz[j][i] = 1;
            }
        }
    }

    return matriz;
}

ripley_t* RipleyAnda(ripley_t* ripley, int** matriz) {

    int i, j, v;

    //Marca todos os v�rtices adjacentes como "gray"
    for (i = 0; i < TAM; i++) {
        if (matriz[ripley->v][i] > 0) {
            if (ripley->cor[i] == 'w') {
                ripley->cor[i] = 'g';
                ripley->S[ripley->aux] = i;
                ripley->aux++;
                printf("ripley achou a sala %d\n", i);
            }
        }
    }

    //Desempilha o �ltimo v�rtice e o visita, caso poss�vel

    if (ripley->aux != 0) {
        v = ripley->S[ripley->aux - 1];
    }
    else {
        //happy birthday
        printf("ripley foi para a sala %d\n", Saida);
        ripley->cor[TAM - 1] = 'b';
        return ripley;
    }

    if (matriz[ripley->v][v] == 2) {
        printf("ripley nao pode ir para a sala %d por causa do alien!!\n", v);
    }

    //ripley visita a �ltima sala colocada na pilha, caso poss�vel
    if (matriz[ripley->v][v] == 1) {
        ripley->v = v;
        printf("ripley foi para a sala %d\n", v);
        ripley->cor[ripley->v] = 'b';
        ripley->aux--;

        return ripley;
    }
    else {
        for (i = (Saida); i >= 0; i--) {
            //Se o v�rtice n�o tiver sido visitado ainda e tiver conex�o com o v�rtice atual, o visita e o retira da pilha
            if (matriz[ripley->v][i] == 1 && ripley->cor[i] == 'g')
            {
                ripley->v = i;
                ripley->cor[ripley->v] = 'b';
                printf("ripley foi para a sala %d\n", i);

                //Retira da pilha
                for (j = 0; j < TAM; j++) {
                    if (ripley->S[j] == i) {
                        ripley->aux--;
                        //Move todos os elementos da pilha
                        for (j = j; j < (TAM - 1); j++) {
                            ripley->S[j] = ripley->S[j + 1];
                        }
                    }
                }
                return ripley;
            }
        }

        //Sem op��es, ripley decide voltar a algum v�rtice j� visitado
        for (i = (Saida); i >= 0; i--) {
            if (matriz[ripley->v][i] == 1) {
                printf("ripley voltou para a sala %d\n", i);
                ripley->v = i;
                return ripley;
            }
        }
    }

    printf("Ripley espera pacientemente na sala %d, ate que o alien saia\n", ripley->v);
    return ripley;
}


int main(int argc, char** argv)
{
    
    int** grafico = NULL;
    int Ripley = Entrada;
    ripley_t* ripley = (ripley_t*)malloc(sizeof(ripley_t));
    ripley->aux = 0;


    int i;

    srand(time(NULL));
    grafico = CriaMatriz(grafico);
    grafico = aleatorizaLabirinto(grafico);

    printf("\n\033[1;32m         aaaaa  ll      ii eeeeee   nn      nn\n        aa   aa ll      ii ee       nnnn    nn\n        aa   aa ll      ii ee       nn nn   nn\n        aaaaaaa ll      ii eeeeee   nn  nn  nn\n        aa   aa ll      ii ee       nn   nn nn\n        aa   aa ll      ii ee       nn    nnnn\n        aa   aa llllll  ii eeeeee   nn      nn\n");
    printf("\n\033[1;31mPress enter...\n\033[1;37m");
    getchar();

    //Inicializa o vetor
    for (i = 0; i < TAM; i++) {
        ripley->cor[i] = 'w';
    }
    ripley->cor[0] = 'b';
    ripley->v = 0;


    while (Ripley != (Saida)) {
        grafico = aleatorizaAlien(grafico);
        PrintMatriz(grafico);
        RipleyAnda(ripley, grafico);
        printf("\033[1;31mPress enter...\033[1;37m");
        getchar();
        if (ripley->cor[TAM - 1] == 'b') {
            
        printf("\033[1;34mhh   hh   aaaaa   pp pppp  pp pppp   yy    yy\n");
        printf("hh   hh  aa   aa  pp    pp pp     pp  yy  yy\n");
        printf("hh   hh  aa   aa  pp    pp pp     pp   yyyy\n");
        printf("hhhhhhh  aaaaaaa  pp pppp  pp pppp      yy\n");
        printf("hh   hh  aa   aa  pp       pp           yy\n");
        printf("hh   hh  aa   aa  pp       pp           yy\n");
        printf("hh   hh  aa   aa  pp       pp           yy\n");
        printf("bb bbbb    ii   rr rrrr     tttttt  hh   hh  dd dddd      aaaaa    yy    yy\n");
        printf("bb    bb   ii   rr    rr      tt    hh   hh  dd    dd    aa   aa    yy  yy\n");
        printf("bb    bb   ii   rr    rr      tt    hh   hh  dd     dd   aa   aa     yyyy\n");
        printf("bb bbbb    ii   rr rrrr       tt    hhhhhhh  dd     dd   aaaaaaa      yy\n");
        printf("bb    bb   ii   rr rr         tt    hh   hh  dd     dd   aa   aa      yy\n");
        printf("bb    bb   ii   rr  rr        tt    hh   hh  dd    dd    aa   aa      yy\n");
        printf("bb bbbb    ii   rr    rr      tt    hh   hh  dd dddd     aa   aa      yy\n");
            //printf("\nHappy Birthday!!\n");
            Ripley = Saida;
        }
        grafico = voltaAlien(grafico);
    }

    LiberaMatriz(grafico);
    printf("\n\033[1;31mPress the last enter... \033[1;37m");
    getchar();
    return 0;

}
