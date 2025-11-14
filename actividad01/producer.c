/*********************************************************************************************
*	* FECHA: 12/11/2025 
*	* AUTOR:  SAMUEL ADRIAN MONTAÑA LINARES
*	* MATERIA: SISTEMAS OPERATIVOS - PONTIFICIA UNIVERSIDAD JAVERIANA
*	* DOCENTE: JOHN JAIRO CORREDOR FRANCO
*	* TEMA: Implementación del Productor con Semáforos POSIX
*       * OBJETIVO: Crear proceso productor que genera datos en buffer compartido
*********************************************************************************************/

#include "compartido.h"

int main() {
    printf("<--- INICIANDO PROCESO PRODUCTOR --->\n");
    
    // Semáforos para sincronización
    sem_t *vacio = sem_open(SEM_VACIO_NAME, O_CREAT, 0644, BUFFER_SIZE);  // vacio: controla espacios disponibles (inicialmente BUFFER_SIZE)
    sem_t *lleno = sem_open(SEM_LLENO_NAME, O_CREAT, 0644, 0);  // lleno: controla elementos consumibles (inicialmente 0)
    
    if (vacio == SEM_FAILED || lleno == SEM_FAILED) {
        perror("ERROR: sem_open falló");
        exit(EXIT_FAILURE);
    }

    // Crear y configurar memoria compartida
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0644);
    if (shm_fd < 0) {
        perror("ERROR: shm_open falló");
        exit(EXIT_FAILURE);
    }
    
    // Ajustar tamaño del segmento de memoria
    if (ftruncate(shm_fd, sizeof(compartir_datos)) == -1) {
        perror("ERROR: ftruncate falló");
        exit(EXIT_FAILURE);
    }
    printf("Memoria compartida creada (%ld bytes)\n", sizeof(compartir_datos));

    // Mapear memoria compartida al espacio de direcciones
    compartir_datos *compartir = mmap(NULL, sizeof(compartir_datos), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (compartir == MAP_FAILED) {
        perror("ERROR: mmap falló");
        exit(EXIT_FAILURE);
    }

    // Inicializar estructura de datos compartidos
    compartir->entrada = 0;
    compartir->salida = 0;

    // Bucle principal: producir 10 elementos
    for (int i = 1; i <= 10; i++) {

        sem_wait(vacio);  // Esperar si no hay espacio disponible (decrementa vacio)

        // SECCIÓN CRÍTICA: Insertar elemento en buffer
        compartir->bus[compartir->entrada] = i;
        printf("Productor -> Produce %d en posición [%d]\n", i, compartir->entrada);
        compartir->entrada = (compartir->entrada + 1) % BUFFER_SIZE;  // Actualizar índice de entrada
        sem_post(lleno); // Señalar que hay nuevo elemento disponible (incrementa lleno)
        sleep(1);  // Simular tiempo de producción
    }

    printf("\n<<< PRODUCTOR TERMINÓ DE PRODUCIR 10 ELEMENTOS >>>\n");

    // Limpieza de recursos
    munmap(compartir, sizeof(compartir_datos));
    close(shm_fd);
    sem_close(vacio);
    sem_close(lleno);
    sem_unlink(SEM_VACIO_NAME);
    printf("Productor Liberado\n");
    return 0;
}
