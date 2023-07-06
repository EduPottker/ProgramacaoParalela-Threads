#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define NUM_THREADS 2

typedef struct {
    int A, B, C ,D, Diff;
} ThreadDados;

void *threadFunc_Diferenca(void *arg) {
    // Código executado pela thread
    ThreadDados *dado = (ThreadDados *)arg;

    int A = dado->A;
    int B = dado->B;
    int C = dado->C;
    int D = dado->D;
    int Diff = (A * B - C * D);

    dado -> Diff = Diff;

    // Termina a execução da thread
    pthread_exit(NULL);
}

int Calcular_Diferenca(int A, int B, int C, int D) {
    ThreadDados dado;
    dado.A = A;
    dado.B = B;
    dado.C = C;
    dado.D = D;

    pthread_t thread;
    pthread_create(&thread, NULL, threadFunc_Diferenca, (void *)&dado);
    pthread_join(thread, NULL);

    return dado.Diff;
}

int main() {
    struct timeval start, end;
    double tmp_Sequencial, tmp_Paralelo, Speedup;
    int A, B, C, D, i, Sequencial;
    A = 5;
    B = 5;
    C = 2;
    D = 2;

    //scanf("%d", &A);
    //scanf("%d", &B);
    //scanf("%d", &C);
    //scanf("%d", &D);

    gettimeofday(&start, NULL);
    Sequencial = Calcular_Diferenca(A, B, C, D);
    gettimeofday(&end, NULL);
    tmp_Sequencial = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

    gettimeofday(&start, NULL);
    pthread_t threads[NUM_THREADS];
    ThreadDados dado[NUM_THREADS];

    // Cria as threads
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, threadFunc_Diferenca, (void *)&dado[i]);
    }

    // Aguarda a conclusão das threads
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&end, NULL);
    tmp_Paralelo = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    printf("DIFERENCA = %d\n", Sequencial);

    // Calcula e exibe o speedup
    if (tmp_Paralelo > 0) {
        Speedup = tmp_Sequencial / tmp_Paralelo;
    } else {
        Speedup = tmp_Sequencial;
    }

    printf("Sequencial: %.6f\n", tmp_Sequencial);
    printf("Paralelo: %.6f\n", tmp_Paralelo);
    printf("Speedup: %.6f\n", Speedup);

    return 0;
}
