#include <stdio.h>
#include <stdlib.h>

#define ROW 400
#define COL 400

int main() {
    double A[ROW][COL], B[ROW][COL], C[ROW][COL], D[ROW][COL];
    int i, j, k;

    // Initializing array
    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COL; j++) {
            A[i][j] = 4.0;
            B[i][j] = 3.0;
            C[i][j] = 8.0;
            D[i][j] = 0.0;
        }
    }

    // D = A*B
    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COL; j++) {
            for (k = 0; k < COL; k++) {
                D[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    // D = D+C
    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COL; j++) {
            D[i][j] += C[i][j];
        }
    }

    return 0;
}