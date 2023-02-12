#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define ROW 400
#define COL 400
double A[ROW][COL], B[ROW][COL], C[ROW][COL], D[ROW][COL];

void *T1(void *arg){ //cal D[0,0] to D[99,399]
    int i, j, k;
    // D = A*B
    for (i = 0; i <= 99; i++) {
        for (j = 0; j <= 399; j++) {
            for (k = 0; k <= 399; k++) {
                D[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    // D = D+C
    for (i = 0; i <= 99; i++) {
        for (j = 0; j <= 399; j++) {
            D[i][j] += C[i][j];
        }
    }
}

void *T2(void *arg){ //cal D[100,0] to D[199,399]
    int i, j, k;
    // D = A*B
    for (i = 100; i <= 199; i++) {
        for (j = 0; j <= 399; j++) {
            for (k = 0; k <= 399; k++) {
                D[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    // D = D+C
    for (i = 100; i <= 199; i++) {
        for (j = 0; j <= 399; j++) {
            D[i][j] += C[i][j];
        }
    }
}

void *T3(void *arg){ //cal D[200,0] to D[299,399]
    int i, j, k;
    // D = A*B
    for (i = 200; i <= 299; i++) {
        for (j = 0; j <= 399; j++) {
            for (k = 0; k <= 399; k++) {
                D[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    // D = D+C
    for (i = 200; i <= 299; i++) {
        for (j = 0; j <= 399; j++) {
            D[i][j] += C[i][j];
        }
    }
}

void *T4(void *arg){ //cal D[300,0] to D[399,399]
    int i, j, k;
    // D = A*B
    for (i = 300; i <= 399; i++) {
        for (j = 0; j <= 399; j++) {
            for (k = 0; k <= 399; k++) {
                D[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    // D = D+C
    for (i = 300; i <= 399; i++) {
        for (j = 0; j <= 399; j++) {
            D[i][j] += C[i][j];
        }
    }
}

void *MainThread(void *arg){
    int i, j;
    double chk = (400*12)+8;
    void *tret;
    pthread_t tid1,tid2,tid3,tid4;
    pthread_create(&tid1, NULL, T1, NULL); //create and run T1
    pthread_create(&tid2, NULL, T2, NULL); //create and run T2
    pthread_create(&tid3, NULL, T3, NULL); //create and run T3
    pthread_create(&tid4, NULL, T4, NULL); //create and run T4
    pthread_join(tid1, &tret); //join T1 (waiting for and exit)
    pthread_join(tid2, &tret); //join T2 (waiting for and exit)
    pthread_join(tid3, &tret); //join T3 (waiting for and exit)
    pthread_join(tid4, &tret); //join T4 (waiting for and exit)
    //check
    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COL; j++) {
            if(D[i][j] != chk){
                printf("ERROR\n");
                return((void*)0);
            }
        }
    }
    printf("OK\n");
    return((void*)0);
}

int main() {
    pthread_t tid1;
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
    pthread_create(&tid1, NULL, MainThread, NULL); //create and run MainThread
    pthread_exit(NULL);

    return 0;
}