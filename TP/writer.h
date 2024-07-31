/*
* @Inst: FIUBA - CESE - 2024. Cohorte 22

* @Subject:
*      Sistemas Operativos de Proposito General
*
* @Module:
*      writer
*
* @Brief:
*      header of writer.c
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
/*** includes  ***/
#include "common.h"
#include <errno.h>
#include <signal.h>

/*** Constants  ***/
#define WRT_NL '\n'
#define WTR_MAX_BUFF 255
#define WTR_FIFO_FLGS (O_WRONLY)
#define WTR_FILE_FLGS (O_RDONLY)
#define WRT_FILE_MODE ((mode_t)(S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP))

const char data[] = "DATA: ";
const char wsUSR1[] = "SIGN:1\n";
const char wsUSR2[] = "SIGN:2\n";
const char msg_fifoFD[] = " => Writer <= Clear. file_fifoFD. Close Status Success\n";