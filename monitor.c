#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 5
#define NUM_ITERATIONS 2
#define RESOURCE_COUNT 10

int available_resources = RESOURCE_COUNT;
pthread_mutex_t monitor_mutex = PTHREAD_MUTEX_INITIALIZER;

int decrease_count(int count) {
    printf("Iniciando decrease_count\n");

    pthread_mutex_lock(&monitor_mutex);
    printf("Mutex adquirido, entrando al monitor\n");

    if (available_resources < count) {
        printf("Recursos insuficientes\n");
        pthread_mutex_unlock(&monitor_mutex);
        printf("Mutex liberado\n");
        return -1;
    } else {
        printf("Recursos suficientes disponibles, consumiendo...\n");
        available_resources -= count;
        pthread_mutex_unlock(&monitor_mutex);
        printf("Mutex liberado\n");
        return 0;
    }
}

int increase_count(int count) {
    printf("Iniciando increase_count\n");

    pthread_mutex_lock(&monitor_mutex);
    printf("Mutex adquirido, entrando al monitor\n");

    available_resources += count;
    printf("Avisando al resto\n");

    pthread_mutex_unlock(&monitor_mutex);
    printf("Mutex liberado\n");

    return 0;
}

void* thread_function(void* arg) {
    int thread_id = (intptr_t)arg;

    printf("Iniciando thread %d\n", thread_id);

    for (int i = 0; i < NUM_ITERATIONS; i++) {
        printf("Iniciando iteracion %d\n", i + 1);

        int result = decrease_count(3);
        if (result == 0) {
            printf("%d - (!) Recurso tomado\n", thread_id);

            // Simular trabajo con el recurso
            printf("%d - Buenos dias! Recurso usado\n", thread_id);
            int sleep_time = rand() % 3 + 1;
            sleep(sleep_time);

            increase_count(3);

            printf("%d - Recurso devuelto!\n", thread_id);
        }
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    printf("Iniciando programa\n");

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i + 1;
        pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}