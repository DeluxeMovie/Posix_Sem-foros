/*********************************************************************************************
*	* FECHA: 12/11/2025 
*	* AUTOR:  SAMUEL ADRIAN MONTAÑA LINARES
*	* MATERIA: SISTEMAS OPERATIVOS - PONTIFICIA UNIVERSIDAD JAVERIANA
*	* DOCENTE: JOHN JAIRO CORREDOR FRANCO
*	* TEMA: Sincronización con Semáforos POSIX
*       * OBJETIVO: Definir estructuras compartidas para problema Productor-Consumidor
*********************************************************************************************/

#ifndef COMPARTIDO_H
#define COMPARTIDO_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>


#define BUFFER_SIZE 5 // Tamaño del buffer circular

// Nombres de recursos del sistema
#define SEM_VACIO_NAME "/vacio"
#define SEM_LLENO_NAME "/lleno" 
#define SHM_NAME "/memoria_compartida"

// Estructura para datos compartidos en memoria que contiene buffer circular y índices para coordinación

typedef struct {
    int bus[BUFFER_SIZE]; // Buffer circular para datos
    int entrada; // Índice donde el productor inserta
    int salida;  // Índice donde el consumidor extrae
} compartir_datos;

#endif 
