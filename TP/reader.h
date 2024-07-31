/*
* @Inst: FIUBA - CESE - 2024. Cohorte 22

* @Subject:
*      Sistemas Operativos de Proposito General
*
* @Module:
*      reader
*
* @Brief:
*      header of reader.c
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
/*** includes ***/
#include "common.h"
#include <errno.h>
#include <signal.h>

/*** Constants ***/
#define RDR_NL '\n'
#define RDR_BUFF_MAX 255
#define LOGFILE "log.txt"
#define DATAFILE "data.txt"
#define RDR_FIFO_FLGS (O_RDONLY)
#define RDR_FILE_FLGS (O_CREAT | O_WRONLY | O_TRUNC)
#define RDR_FILE_MODE ((mode_t)(S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP))

const char rdata[] = "DATA:";
const char rsUSR1[] = "SIGN:1\n";
const char rsUSR2[] = "SIGN:2\n";
const char msg_lodFD[] = " => Reader <= Clear. file_logFD. Close Status Success\n";
const char msg_dataFD[] = " => Reader <= Clear. file_dataFD. Close Status Success\n";
const char msg_fifoFD[] = " => Reader <= Clear. file_fifoFD. Close Status Success\n";