#############################################################################################
#	* FECHA: 12/11/2025 
#	* AUTOR:  SAMUEL ADRIAN MONTAÑA LINARES
#	* MATERIA: SISTEMAS OPERATIVOS - PONTIFICIA UNIVERSIDAD JAVERIANA
#	* DOCENTE: JOHN JAIRO CORREDOR FRANCO
#	* TEMA: Automatización de compilación para Actividad 1
#       * OBJETIVO: Compilar programas productor y consumidor automáticamente
#############################################################################################

CC = gcc
CFLAGS = -Wall -lrt -lpthread

all: productor consumidor

productor: producer.c compartido.h
	$(CC) $(CFLAGS) -o productor producer.c

consumidor: consumer.c compartido.h
	$(CC) $(CFLAGS) -o consumidor consumer.c

clean:
	rm -f productor consumidor
