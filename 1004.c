#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define NUM_THREADS 2

typedef struct {
    int a;
    int b;
    int mult;
} ThreadDados;

void *threadFunc_Multiplicacao(void *arg) {
    // Código executado pela thread
    ThreadDados *dado = (ThreadDados *)arg;

    int a = dado->a;
    int b = dado->b;
    int mult = a * b;

    dado -> mult = mult;

    // Termina a execução da thread
    pthread_exit(NULL);
}

int Calcula_Mult(int a, int b) {
    ThreadDados dado;
    dado.a = a;
    dado.b = b;

    pthread_t thread;
    pthread_create(&thread, NULL, threadFunc_Multiplicacao, (void *)&dado);
    pthread_join(thread, NULL);

    return dado.mult;
}

int main() {
    struct timeval start, end;
    double tmp_Sequencial, tmp_Paralelo, Speedup;
    int a, b, i, Sequencial;
    a = 10;
    b = 10;

    //scanf("%d", &a);
    //scanf("%d", &b);

    gettimeofday(&start, NULL);
    Sequencial = Calcula_Mult(a, b);
    gettimeofday(&end, NULL);
    tmp_Sequencial = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

    gettimeofday(&start, NULL);
    pthread_t threads[NUM_THREADS];
    ThreadDados dado[NUM_THREADS];

    // Cria as threads
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, threadFunc_Multiplicacao, (void *)&dado[i]);
    }

    // Aguarda a conclusão das threads
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&end, NULL);
    tmp_Paralelo = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

    printf("PROD = %d\n", Sequencial);

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
