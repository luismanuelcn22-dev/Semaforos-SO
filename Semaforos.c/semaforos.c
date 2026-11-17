/**
 * @file semaforos.c
 * @brief Programa con hilos (pthread) y semáforos POSIX donde la sección 
 *        crítica abarca del conteo 4000 al 6000 para evitar interrupciones.
 * @author Luis Manuel Cortés Nájera
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

// Declaración del semáforo global para la exclusión mutua en la sección crítica
sem_t sem_critico;

// Estructura o función para el hilo
void* rutina_hilo(void* arg) {
    int id_hilo = *((int*) arg);

    for (int i = 1; i <= 10000; i++) {
        // Al llegar a 4000, el hilo solicita acceso exclusivo (Sección Crítica)
        if (i == 4000) {
            sem_wait(&sem_critico);
        }

        // Impresión del conteo actual
        printf("Hilo %d -> Conteo: %d\n", id_hilo, i);

        // Al llegar a 6000, el hilo libera el semáforo para permitir que el otro continúe
        if (i == 6000) {
            sem_post(&sem_critico);
        }
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t hilo1, hilo2;
    int id1 = 1, id2 = 2;

    // Inicializar el semáforo con un valor de 1 (exclusión mutua binaria)
    // El segundo argumento '0' indica que el semáforo es compartido entre hilos del mismo proceso.
    if (sem_init(&sem_critico, 0, 1) != 0) {
        perror("Error al inicializar el semáforo");
        exit(EXIT_FAILURE);
    }

    // Crear los dos hilos
    if (pthread_create(&hilo1, NULL, rutina_hilo, &id1) != 0) {
        perror("Error al crear el hilo 1");
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&hilo2, NULL, rutina_hilo, &id2) != 0) {
        perror("Error al crear el hilo 2");
        exit(EXIT_FAILURE);
    }

    // Esperar a que ambos hilos concluyan su ejecución
    pthread_join(hilo1, NULL);
    pthread_join(hilo2, NULL);

    // Destruir el semáforo al finalizar
    sem_destroy(&sem_critico);

    printf("\n[INFO] Ambos hilos han finalizado la cuenta de 1 a 10000 exitosamente.\n");
    return 0;
}