// Lab 6 Sistos, Jose Reyes: Semaforo

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_THREADS 10
#define NUM_ITERATIONS 5
#define RESOURCE_COUNT 3

int available_resources = RESOURCE_COUNT;
sem_t resource_semaphore;

void* thread_function(void* arg) {
    int thread_id = (intptr_t)arg;

    printf("Iniciando thread %d\n", thread_id);

    for (int i = 0; i < NUM_ITERATIONS; i++) {
        printf("Iniciando iteracion %d\n", i + 1);

        sem_wait(&resource_semaphore);
        printf("%d - (!) Recurso tomado\n", thread_id);

        // Simular trabajo con el recurso
        printf("%d - Buenos dias! Recurso usado\n", thread_id);
        int sleep_time = rand() % 3 + 1;
        sleep(sleep_time);

        printf("%d - Recurso devuelto!\n", thread_id);
        sem_post(&resource_semaphore);
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    printf("Iniciando programa\n");

    sem_init(&resource_semaphore, 0, RESOURCE_COUNT);

    printf("Creando threads\n");

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i + 1;
        pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]);
    }

    printf("Esperando threads\n");

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&resource_semaphore);

    return 0;
}
