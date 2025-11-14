### Componentes Principales

#### - Mecanismos de Sincronización -
- **Mutex `buf_mutex`**: Protege el acceso al buffer compartido
- **Variable condición `buf_cond`**: Señala cuando hay espacio disponible
- **Variable condición `spool_cond`**: Señala cuando hay datos para imprimir

#### - Estructura de Datos Compartidos -
- **Buffer circular**: 10 slots para mensajes
- **Contadores**: Espacios disponibles y líneas pendientes
- **Índices**: Para gestión circular del buffer

## - Estructura de Archivos -

| Archivo | Tipo | Descripción |
|---------|------|-------------|
| [`posixSincro.c`](./posixSincro.c) | Código Fuente | Sistema completo de hilos productores y spooler |
| [`Makefile`](./Makefile) | Script | Automatización de compilación |

## - Funcionalidades Implementadas -

### - Características Principales -
- **10 hilos productores** que generan mensajes concurrentemente
- **1 hilo spooler** que imprime los mensajes del buffer
- **Buffer circular** compartido entre todos los hilos
- **Sincronización fina** con mutex y variables condición
- **Gestión segura** de recursos compartidos

### - Especificaciones Técnicas -
- **Tamaño del buffer**: 10 mensajes
- **Hilos productores**: 10 hilos concurrentes
- **Mensajes por productor**: 5 mensajes cada uno
- **Total de mensajes**: 50 mensajes generados

```bash
Para compilación
$ make
Ejecutar el siguiente comando
$ ./sincroPosix
