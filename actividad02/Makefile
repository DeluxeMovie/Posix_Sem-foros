#############################################################################################
#	* FECHA: 12/11/2025 
#	* AUTOR:  SAMUEL ADRIAN MONTAÑA LINARES
#	* MATERIA: SISTEMAS OPERATIVOS - PONTIFICIA UNIVERSIDAD JAVERIANA
#	* DOCENTE: JOHN JAIRO CORREDOR FRANCO
#	* TEMA: Automatización de compilación para Actividad 2
#   * OBJETIVO: Compilar programa de sincronización con Pthreads
#############################################################################################

CC = gcc
CFLAGS = -Wall -lpthread

all: sincroPosix

sincroPosix: posixSincro.c
	$(CC) $(CFLAGS) -o sincroPosix posixSincro.c

clean:
	rm -f sincroPosix

