#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define NUM_THREADS 2

typedef struct {
    int M, N, Resultado;
} ThreadDados;

long long int Fatorial(long long int n) {
    if (n <= 0) {
        return 1;
    }

    n *= Fatorial(n - 1);

    return n;
}

void *threadFunc_Fatorial(void *arg) {
    // Código executado pela thread
    ThreadDados *dado = (ThreadDados *)arg;

    int M = dado->M;
    int N = dado->N;
    long long int Resultado = (Fatorial(M) + Fatorial(N));

    dado->Resultado = Resultado;

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
        int M, N;
        scanf("%d %d", &M, &N);

        dado[i].M = M;
        dado[i].N = N;
    }

    gettimeofday(&start, NULL);

    for (int i = 0; i < NUM_THREADS; i++) {
        dado[i].Resultado = Fatorial(dado[i].M) + Fatorial(dado[i].N);
    }

    gettimeofday(&end, NULL);
    tmp_Sequencial = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

    gettimeofday(&start, NULL);
    // Cria as threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, threadFunc_Fatorial, (void *)&dado[i]);
    }

    // Aguarda a conclusão das threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&end, NULL);
    tmp_Paralelo = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

    for (int i = 0; i < NUM_THREADS; i++) {
        printf("%d\n", dado[i].Resultado);
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
