#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define NUM_THREADS 2

typedef struct {
    int n, k, Resultado;
} ThreadDados;

int Josephus(int n, int k) {
    if (n == 1) return 0;
    return (Josephus(n - 1, k) + k) % n;
}

void *threadFunc_Josephus(void *arg) {
    // Código executado pela thread
    ThreadDados *dado = (ThreadDados *)arg;

    int n = dado->n;
    int k = dado->k;
    int Resultado = Josephus(n, k) + 1;

    dado -> Resultado = Resultado;

    // Termina a execução da thread
    pthread_exit(NULL);
}

int main() {
    struct timeval start, end;
    double tmp_Sequencial, tmp_Paralelo, Speedup;
    int i;

    pthread_t threads[NUM_THREADS];
    ThreadDados dado[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        int n, k;
        scanf("%d %d", &n, &k);

        dado[i].n = n;
        dado[i].k = k;
    }

    gettimeofday(&start, NULL);

    for (int i = 0; i < NUM_THREADS; i++) {
        dado[i].Resultado = Josephus(dado[i].n, dado[i].k) + 1;
    }

    gettimeofday(&end, NULL);
    tmp_Sequencial = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

    gettimeofday(&start, NULL);
    // Cria as threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, threadFunc_Josephus, (void *)&dado[i]);
    }

    // Aguarda a conclusão das threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&end, NULL);
    tmp_Paralelo = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

    for (int i = 0; i < NUM_THREADS; i++) {
        printf("Case %d: %d\n", i + 1, dado[i].Resultado);
    }

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
