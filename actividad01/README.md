
### - Archivos de la Actividad -

| Archivo | Descripción |
|---------|-------------|
| [`producer.c`](./producer.c) | Proceso productor que genera datos y los coloca en el buffer |
| [`consumer.c`](./consumer.c) | Proceso consumidor que extrae datos del buffer compartido |
| [`compartido.h`](./compartido.h) | Definiciones comunes y estructura de datos compartidos |
| [`Makefile`](./Makefile) | Script de compilación automatizada |

##  Componentes de Sincronización

### - Semáforos POSIX -
- **`/vacio`**: Controla espacios disponibles en el buffer (inicial: BUFFER_SIZE)
- **`/lleno`**: Controla elementos listos para consumir (inicial: 0)

### - Memoria Compartida -
- **Estructura**: `compartir_datos` con buffer circular y índices
- **Tamaño**: Buffer de 5 elementos (configurable)
- **Coordinación**: Índices `entrada` y `salida` para acceso circular

##  Compilación y Ejecución

###  Compilar los programas
Para comilación se muestran los siguientes comandos
```bash
$ make

En una consola se ejecuta el siguiente comando
$ ./consumidor

En la otra consola se ejecuta este comnado
$ ./productor
