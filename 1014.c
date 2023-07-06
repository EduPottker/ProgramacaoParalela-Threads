#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define NUM_THREADS 2

typedef struct {
    int X;
    double Y, Consumo;
} ThreadDados;

void *threadFunc_Consumo(void *arg) {
    // Código executado pela thread
    ThreadDados *dado = (ThreadDados *)arg;

    int X = dado->X;
    double Y = dado->Y;
    double Consumo = X / Y;

    dado -> Consumo = Consumo;

    // Termina a execução da thread
    pthread_exit(NULL);
}

double Calcular_Consumo(int X, double Y) {
    ThreadDados dado;
    dado.X = X;
    dado.Y = Y;

    pthread_t thread;
    pthread_create(&thread, NULL, threadFunc_Consumo, (void *)&dado);
    pthread_join(thread, NULL);

    return dado.Consumo;
}

int main() {
    struct timeval start, end;
    double Y, Sequencial, tmp_Sequencial, tmp_Paralelo, Speedup;
    int X, i;
    X = 4554;
    Y = 464.6;

    if (Y == 0.0) {
        Y = 1.0;
    }

    //scanf("%d", &X);
    //scanf("%d", &Y);

    gettimeofday(&start, NULL);
    Sequencial = Calcular_Consumo(X, Y);
    gettimeofday(&end, NULL);
    tmp_Sequencial = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

    gettimeofday(&start, NULL);
    pthread_t threads[NUM_THREADS];
    ThreadDados dado[NUM_THREADS];

    // Cria as threads
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, threadFunc_Consumo, (void *)&dado[i]);
    }

    // Aguarda a conclusão das threads
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&end, NULL);
    tmp_Paralelo = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    printf("%.3f km/l \n", Sequencial);

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
