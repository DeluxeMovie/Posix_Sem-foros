/*********************************************************************************************
*	* FECHA: 12/11/2025 
*	* AUTOR:  SAMUEL ADRIAN MONTAÑA LINARES
*	* MATERIA: SISTEMAS OPERATIVOS - PONTIFICIA UNIVERSIDAD JAVERIANA
*	* DOCENTE: JOHN JAIRO CORREDOR FRANCO
*	* TEMA: Sincronización con Pthreads - Múltiples Productores y Spooler
*   * OBJETIVO: Implementar buffer compartido con mutex y variables condición
*********************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

// Tamaño máximo del buffer
#define MAX_BUFFERS 10

// Buffer compartido y variables globales
char buf[MAX_BUFFERS][100]; // Buffer para almacenar mensajes
int buffer_index; // Índice para escribir en el buffer
int buffer_print_index; // Índice para leer del buffer

// Mecanismos de sincronización
pthread_mutex_t buf_mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex para proteger el buffer
pthread_cond_t buf_cond = PTHREAD_COND_INITIALIZER; // Condición: buffer no lleno
pthread_cond_t spool_cond = PTHREAD_COND_INITIALIZER; // Condición: buffer no vacío

// Contadores de estado del buffer
int buffers_available = MAX_BUFFERS; // Espacios disponibles en buffer
int lines_to_print = 0; // Líneas pendientes por imprimir
int producers_finished = 0; // Contador de productores terminados

// Prototipos de funciones
void *producer(void *arg);
void *spooler(void *arg);

/*********************************************************************************************
* FUNCIÓN: main
* OBJETIVO: Crear hilos productores y spooler, y coordinar su ejecución
*********************************************************************************************/
int main(int argc, char **argv) {
    pthread_t tid_producer[10], tid_spooler;
    int i, r;

    printf("<--- INICIANDO SISTEMA DE HILOS --->\n");

    
    buffer_index = buffer_print_index = 0; // Inicializar índices del buffer
    
    // Crear hilo spooler (impresor) PRIMERO
    if ((r = pthread_create(&tid_spooler, NULL, spooler, NULL)) != 0) {
        fprintf(stderr, "Error = %d (%s)\n", r, strerror(r)); 
        exit(1);
    }

    // Crear 10 hilos productores
    int thread_no[10];
    for (i = 0; i < 10; i++) {
        thread_no[i] = i;
        if ((r = pthread_create(&tid_producer[i], NULL, producer, (void *) &thread_no[i])) != 0) {
            fprintf(stderr, "Error = %d (%s)\n", r, strerror(r)); 
            exit(1);
        }
    }

    // Esperar a que todos los hilos productores terminen
    for (i = 0; i < 10; i++) {
        if ((r = pthread_join(tid_producer[i], NULL)) == -1) {
            fprintf(stderr, "Error = %d (%s)\n", r, strerror(r)); 
            exit(1);
        }
    }

    printf("\nTodos los productores han terminado.\n");
    
    // Esperar a que se impriman todas las líneas pendientes
    /*while (lines_to_print) {
        printf("Esperando que se impriman %d líneas pendientes...\n", lines_to_print);
        sleep(1);
    }*/
    
   
    producers_finished = 1; // Marcar que todos los productores terminaron
    
    
    pthread_cond_signal(&spool_cond); // Señalar al spooler una última vez por si está esperando
    
    
    sleep(2); // Esperar un poco antes de cancelar
    
    // Cancelar el hilo spooler (está en un bucle infinito)
    if ((r = pthread_cancel(tid_spooler)) != 0) {
        fprintf(stderr, "Error cancelando spooler = %d (%s)\n", r, strerror(r)); 
    } 

    printf("\n<--- FIN DEL PROGRAMA --->\n");
    return 0;
}

/*********************************************************************************************
* FUNCIÓN: producer
* OBJETIVO: Generar mensajes y colocarlos en el buffer compartido
*********************************************************************************************/
void *producer(void *arg) {
    int i, r;
    int my_id = *((int *) arg); // ID del hilo productor
    int count = 0; // Contador de mensajes por hilo

    for (i = 0; i < 5; i++) {  // Reducido a 5 mensajes por hilo para prueba
        // Adquirir mutex para acceder al buffer
        if ((r = pthread_mutex_lock(&buf_mutex)) != 0) {
            fprintf(stderr, "Error = %d (%s)\n", r, strerror(r)); 
            exit(1);
        }
        
        // Esperar si no hay espacio disponible en el buffer
        while (!buffers_available) {
            printf("Productor %d: Buffer lleno, esperando...\n", my_id);
            pthread_cond_wait(&buf_cond, &buf_mutex);
        }

        // Escribir en el buffer
        int j = buffer_index;
        buffer_index = (buffer_index + 1) % MAX_BUFFERS; // Buffer circular
        buffers_available--; // Un espacio menos disponible

        
        sprintf(buf[j], "Thread %d: Mensaje %d\n", my_id, ++count); // Crear mensaje y colocarlo en el buffer
        lines_to_print++;       // Una línea más por imprimir

        printf("Productor %d: Escribió mensaje en -> [%d]\n", my_id, j);

        
        pthread_cond_signal(&spool_cond); // Señalar al spooler que hay datos disponibles

        // Liberar el mutex
        if ((r = pthread_mutex_unlock(&buf_mutex)) != 0) {
            fprintf(stderr, "Error = %d (%s)\n", r, strerror(r)); 
            exit(1);
        }
        
        
        sleep(1); // Simular tiempo de producción
    }
    return NULL;
}

/*********************************************************************************************
* FUNCIÓN: spooler  
* OBJETIVO: Imprimir mensajes desde el buffer compartido
*********************************************************************************************/
void *spooler(void *arg) {
    int r;
    
    while (1) {  
        // Adquirir mutex para acceder al buffer
        if ((r = pthread_mutex_lock(&buf_mutex)) != 0) {
            fprintf(stderr, "Error = %d (%s)\n", r, strerror(r)); 
            exit(1);
        }
        
        // Esperar si no hay líneas para imprimir
        while (!lines_to_print && !producers_finished) {
            pthread_cond_wait(&spool_cond, &buf_mutex);
        }

        // Si no hay líneas y los productores terminaron, salir
        if (!lines_to_print && producers_finished) {
            pthread_mutex_unlock(&buf_mutex);
            break;
        }

        
        printf("\t IMPRESO -> %s", buf[buffer_print_index]); // Leer e imprimir del buffer
        lines_to_print--; // Una línea menos por imprimir

        
        buffer_print_index = (buffer_print_index + 1) % MAX_BUFFERS; // Actualizar índice de lectura (buffer circular)
        buffers_available++; // Un espacio más disponible

        
        pthread_cond_signal(&buf_cond); // Señalar a los productores que hay espacio disponible

        // Liberar el mutex
        if ((r = pthread_mutex_unlock(&buf_mutex)) != 0) {
            fprintf(stderr, "Error = %d (%s)\n", r, strerror(r)); 
            exit(1);
        }
       
    }
    
    return NULL;
}
