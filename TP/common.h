/*
* @Inst: FIUBA - CESE - 2024. Cohorte 22

* @Subject:
*      Sistemas Operativos de Proposito General
*
* @Module:
*      common
*
* @Brief:
*      header of common.c
*
* @Prof:
*      Esp. Ing. Diego Essaya
*
* @Author:
*      Ing. Daniel David Albarracin
*
* @Date:
*      July 2024
*/
/* Includes */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

/* Constants */
#define namedFIFO "myFIFO"
#define rightsFIFO (mode_t)(S_IRWXU | S_IRWXG | S_IRWXO)

#define BUF_MAX 300
#define SUCCESS 0
#define FAIL -1

/* Declarated Public Functions */
int fifo_create(const char *fifo_name, mode_t fifo_rights);
int fifo_remove(const char *fifo_name, int fifo_fd);
int file_open(const char *file_name, int file_flags, mode_t file_mode);
ssize_t file_send(int file_fd, const void *file_buffer, size_t file_count);
ssize_t file_recvd(int file_fd, void *file_buffer, size_t file_count);
int make_msg(char *string, const char *header, char *payload, size_t header_count, size_t payload_count);
