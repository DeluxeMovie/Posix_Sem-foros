/*********************************************************************************************
*	* FECHA: 12/11/2025 
*	* AUTOR:  SAMUEL ADRIAN MONTAÑA LINARES
*	* MATERIA: SISTEMAS OPERATIVOS - PONTIFICIA UNIVERSIDAD JAVERIANA
*	* DOCENTE: JOHN JAIRO CORREDOR FRANCO
*	* TEMA: Implementación del Consumidor con Semáforos POSIX
*       * OBJETIVO: Crear proceso consumidor que extrae datos del buffer compartido
*********************************************************************************************/

#include "compartido.h"

int main() {

    printf("<--- INICIANDO PROCESO CONSUMIDOR --->\n");

    // Abrir semáforos existentes y no crear nuevos
    sem_t *vacio = sem_open(SEM_VACIO_NAME, 0);
    sem_t *lleno = sem_open(SEM_LLENO_NAME, 0);

    if (vacio == SEM_FAILED || lleno == SEM_FAILED) {
        perror("ERROR: sem_open falló");
        exit(EXIT_FAILURE);
    }

    // Abrir segmento existente de memoria compartida
    int fd_compartido = shm_open(SHM_NAME, O_RDWR, 0644);
    if (fd_compartido < 0) {
        perror("ERROR: shm_open falló");
        exit(EXIT_FAILURE);
    }
    printf("Memoria compartida abierta correctamente\n");

    // Mapear memoria compartida existente
    compartir_datos *compartir = mmap(NULL, sizeof(compartir_datos), PROT_READ | PROT_WRITE, MAP_SHARED, fd_compartido, 0);
    if (compartir == MAP_FAILED) {
        perror("ERROR: mmap falló");
        exit(EXIT_FAILURE);
    }

    compartir->salida = 0; // Inicializar índice de salida del consumidor

    // Bucle principal: consumir 10 elementos
    for (int i = 1; i <= 10; i++) {

        sem_wait(lleno); // Esperar si no hay elementos disponibles
        int item = compartir->bus[compartir->salida]; // SECCIÓN CRÍTICA: Extraer elemento del buffer
        printf("Consumidor -> Consume %d de posición [%d]\n", item, compartir->salida);
        compartir->salida = (compartir->salida + 1) % BUFFER_SIZE; // Actualizar índice de salida (buffer circular)
        sem_post(vacio); // Señalar que hay espacio disponible (incrementa vacio)

        sleep(2); // Simular tiempo de procesamiento (más lento que productor)
    }

    printf("\n<<< CONSUMIDOR TERMINÓ DE CONSUMIR 10 ELEMENTOS >>>\n");

    // Limpieza de recursos
    munmap(compartir, sizeof(compartir_datos));
    close(fd_compartido);
    sem_close(lleno);
    sem_unlink(SEM_LLENO_NAME);
    shm_unlink(SHM_NAME);
    
    printf("Consumidor Liberado\n");
    return 0;
}
