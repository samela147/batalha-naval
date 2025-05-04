/* Batalha Naval ++: modificado
 * Inclui validacoes de tamanho, checagem de sobreposicao,
 * camada separada para efeitos e diagonal secundaria.
 */
#include <stdio.h>
#include <stdlib.h>

#define TAM 10
#define LIN 10
#define COL 10

char tab1D[TAM];
char tab2D[LIN][COL];
char efeito[LIN][COL];

/* ----- MOD 1: Tabuleiro 1D ----- */
void inicializaTabuleiro1D(void) {
    for (int i = 0; i < TAM; i++)
        tab1D[i] = '~';
}

int posicionaNavio1D(int inicio, int len) {
    if (len <= 0 || inicio < 0 || inicio + len > TAM)
        return 0;
    for (int i = inicio; i < inicio + len; i++)
        if (tab1D[i] != '~')
            return 0;  // sobreposicao
    for (int i = inicio; i < inicio + len; i++)
        tab1D[i] = 'N';
    return 1;
}

void imprimeTabuleiro1D(void) {
    printf("Tabuleiro 1D\n  ");
    for (int i = 0; i < TAM; i++) printf("%d ", i);
    printf("\n |");
    for (int i = 0; i < TAM; i++) printf("%c ", tab1D[i]);
    printf("|\n\n");
}

/* ----- MOD 2: Tabuleiro 2D com camada de efeitos ----- */
void inicializaTabuleiro2D(void) {
    for (int i = 0; i < LIN; i++) {
        for (int j = 0; j < COL; j++) {
            tab2D[i][j] = '~';
            efeito[i][j] = ' ';
        }
    }
}

int posicionaNavio2D(int linha, int col, int len, char orient) {
    if (len <= 0) return 0;
    int dr = 0, dc = 0;
    if      (orient == 'H')  dc = 1;
    else if (orient == 'V')  dr = 1;
    else if (orient == 'D')  { dr = 1; dc = 1; }
    else if (orient == '/')  { dr = 1; dc = -1; }
    else return 0;

    int fimL = linha + dr*(len-1);
    int fimC = col   + dc*(len-1);
    if (fimL < 0 || fimL >= LIN || fimC < 0 || fimC >= COL)
        return 0;

    // checar sobreposicao
    for (int k = 0; k < len; k++) {
        int l = linha + dr*k, c = col + dc*k;
        if (tab2D[l][c] != '~')
            return 0;
    }
    // posicionar navio
    for (int k = 0; k < len; k++) {
        int l = linha + dr*k, c = col + dc*k;
        tab2D[l][c] = 'N';
    }
    return 1;
}

void imprimeTabuleiro2D(void) {
    printf("Tabuleiro 2D\n   ");
    for (int j = 0; j < COL; j++) printf("%2d", j);
    printf("\n");
    for (int i = 0; i < LIN; i++) {
        printf("%2d ", i);
        for (int j = 0; j < COL; j++) {
            if (tab2D[i][j] == 'N')
                printf(" N");
            else if (efeito[i][j] == '*')
                printf(" *");
            else
                printf(" %c", tab2D[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

/* ----- MOD 3: Habilidades especiais marcando em 'efeito' ----- */
void aplicaConeNorte(int alvoL, int alvoC, int r) {
    for (int dl = -r; dl <= 0; dl++) {
        int largura = -dl;
        for (int dc = -largura; dc <= largura; dc++) {
            int l = alvoL + dl, c = alvoC + dc;
            if (l >= 0 && l < LIN && c >= 0 && c < COL
             && tab2D[l][c] == '~')
                efeito[l][c] = '*';
        }
    }
}

void aplicaCruz(int alvoL, int alvoC, int r) {
    for (int d = -r; d <= r; d++) {
        int l1 = alvoL + d, c1 = alvoC;
        if (l1 >= 0 && l1 < LIN && tab2D[l1][c1] == '~')
            efeito[l1][c1] = '*';
        int l2 = alvoL, c2 = alvoC + d;
        if (c2 >= 0 && c2 < COL && tab2D[l2][c2] == '~')
            efeito[l2][c2] = '*';
    }
}

void aplicaOctaedro(int alvoL, int alvoC, int r) {
    for (int dl = -r; dl <= r; dl++) {
        for (int dc = -r; dc <= r; dc++) {
            if (abs(dl) + abs(dc) <= r) {
                int l = alvoL + dl, c = alvoC + dc;
                if (l >= 0 && l < LIN && c >= 0 && c < COL
                 && tab2D[l][c] == '~')
                    efeito[l][c] = '*';
            }
        }
    }
}

/* ----- MAIN ----- */
int main(void) {
    /* Demo Modulo 1 */
    inicializaTabuleiro1D();
    posicionaNavio1D(4, 3);
    imprimeTabuleiro1D();

    /* Demo Modulo 2 + 3 */
    inicializaTabuleiro2D();
    posicionaNavio2D(2, 3, 4, 'H');
    posicionaNavio2D(5, 1, 3, 'V');
    posicionaNavio2D(0, 9, 5, '/'); // diagonal secundaria
    aplicaConeNorte(7, 7, 3);
    aplicaCruz(2, 2, 4);
    aplicaOctaedro(5, 2, 2);
    imprimeTabuleiro2D();

    return 0;
}

