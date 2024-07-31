/*
 * @Inst: FIUBA - CESE - 2024. Cohorte 22

 * @Subject:
 *      Sistemas Operativos de Proposito General
 *
 * @Program:
 *      reader
 *
 * @Brief:
 *      this program receive a string from a named fifo and put the
 *      payload into the files data.txt and log.txt depends on the
 *      header of the message received from.
 *      If the header message is SIGN: so put the payload into log.txt
 *      On the other hand, if the header message i DATA: so put the
 *      payload into data.txt
 *      All signal are blocked except SIGINT.
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
#include "reader.h"

/*** private variables ***/
static int rfifoFD = 0;
static int file_logFD = 0;
static int file_dataFD = 0;
volatile int sgnlQuit = 1;

/*** private functions ***/
static void signal_hndlr(int signal);
static void clear(void);

/*
 * @func:
 *      main
 *
 * @brief:
 *      entry point
 *
 * @param:
 *      none
 *
 * @return:
 *      int stts
 */
int main()
{
    /* local variables */
    int stts = EXIT_FAILURE;
    int rtrn = FAIL;
    ssize_t readBytes = 0;
    char buffer[RDR_BUFF_MAX];
    int offset = 0;
    int msg_lgth = 0;
    sigset_t blk_sgnl_mask;
    struct sigaction reader_sa;

    /* sigaction initialize */
    reader_sa.sa_handler = signal_hndlr;
    reader_sa.sa_flags = 0;
    sigfillset(&blk_sgnl_mask);
    sigdelset(&blk_sgnl_mask, SIGINT);
    reader_sa.sa_mask = blk_sgnl_mask;

    if (FAIL == sigprocmask(SIG_BLOCK, &reader_sa.sa_mask, NULL))
    {
        perror(" => Writer <= Sigprocmask Set Block Status");
        exit(EXIT_FAILURE);
    }

    if (FAIL == sigaction(SIGINT, &reader_sa, NULL))
    {
        perror(" => Writer <= Sigaction Set SIGUSR1 Status");
        exit(EXIT_FAILURE);
    }

    /* main flow code */
    printf(" *** Reader *** PID: %d\n", getpid());
    rtrn = fifo_create(namedFIFO, rightsFIFO);

    if (SUCCESS == rtrn)
    {
        printf(" => Reader <= Waiting for Writer...\n");
        rfifoFD = file_open(namedFIFO, RDR_FIFO_FLGS, RDR_FILE_MODE);

        if (FAIL == rfifoFD)
        {
            if (FAIL == fifo_remove(namedFIFO, rfifoFD))
            {
                perror(" => Reader <= fifo_remove Status");
            }
            exit(EXIT_FAILURE);
        }
        else
        {
            printf(" => Reader <= file_open. namedFIFO. Status Success\n");
        }

        file_logFD = file_open(LOGFILE, RDR_FILE_FLGS, RDR_FILE_MODE);

        if (FAIL == file_logFD)
        {
            perror(" => Reader <= file_open Status");
            exit(EXIT_FAILURE);
        }
        else
        {
            printf(" => Reader <= file_open. LOGFILE. Status Success\n");
        }

        file_dataFD = file_open(DATAFILE, RDR_FILE_FLGS, RDR_FILE_MODE);

        if (FAIL == file_logFD)
        {
            perror(" => Reader <= file_open Status");
            exit(EXIT_FAILURE);
        }
        else
        {
            printf(" => Reader <= file_open. DATAFILE. Status Success\n");
        }

        do
        {
            offset = 0;
            memset(buffer, '\0', sizeof(buffer));

            while ((readBytes = (file_recvd(rfifoFD, buffer + offset, 1)) > 0))
            {
                if (RDR_NL == buffer[offset])
                {
                    break;
                }

                offset += 1;
            }
            if (strncmp(rdata, buffer, strlen(rdata)) == 0)
            {
                msg_lgth = 0;

                for (int i = strlen(rdata) + 1; i < sizeof(buffer), buffer[i] != '\0'; i++)
                {
                    msg_lgth++;
                }
                if (file_send(file_dataFD, &buffer[strlen(rdata) + 1], msg_lgth) != msg_lgth)
                {
                    perror(" => Reader <= Sending Payload Text Status");
                    exit(EXIT_FAILURE);
                }
            }
            if (strncmp(rsUSR1, buffer, strlen(rsUSR1)) == 0)
            {
                msg_lgth = 0;

                for (int i = strlen(rsUSR1) - 2; i < sizeof(buffer), buffer[i] != '\0'; i++)
                {
                    msg_lgth++;
                }
                if (file_send(file_logFD, &buffer[strlen(rsUSR1) - 2], msg_lgth) != msg_lgth)
                {
                    perror(" => Reader <= Sending Payload Signal Status");
                    exit(EXIT_FAILURE);
                }
            }
            if (strncmp(rsUSR2, buffer, strlen(rsUSR2)) == 0)
            {
                msg_lgth = 0;

                for (int i = strlen(rsUSR2) - 2; i < sizeof(buffer), buffer[i] != '\0'; i++)
                {
                    msg_lgth++;
                }
                if (file_send(file_logFD, &buffer[strlen(rsUSR2) - 2], msg_lgth) != msg_lgth)
                {
                    perror(" => Reader <= Sending Payload Signal Status");
                    exit(EXIT_FAILURE);
                }
            }
            if (0 == readBytes)
            {
                sgnlQuit = 0;
            }

            stts = SUCCESS;

        } while (sgnlQuit);

        clear();

        if (FAIL == sigprocmask(SIG_UNBLOCK, &reader_sa.sa_mask, NULL))
        {
            perror(" => Writer <= Sigprocmask Set UnBlock Status");
            exit(EXIT_FAILURE);
        }
    }

    printf(" => Reader <= Exit Status: %d\n", stts);

    return (stts);
}

/*
 * @func:
 *      signal_hndlr
 *
 * @brief:
 *      signal handler to take actions
 *      in case a SIGINT was received.
 *
 * @param:
 *      int signal
 *
 * @return:
 *      void
 */
static void signal_hndlr(int signal)
{

    if (SIGINT == signal)
    {
        sgnlQuit = 0;
    }
}

/*
 * @func:
 *      clear
 *
 * @brief:
 *      a confortable clean function
 *      to close file descriptor
 *
 * @param:
 *      void
 *
 * @return:
 *      void
 */
static void clear(void)
{
    size_t writes = 0;
    u_int8_t stts_logFD = SUCCESS;
    u_int8_t stts_dataFD = SUCCESS;
    u_int8_t stts_fifoFD = SUCCESS;

    if (FAIL == close(file_logFD))
    {
        perror(" => Reader <= clear file_logFD. Close Status");
        stts_logFD = EXIT_FAILURE;
    }
    else
    {
        writes = write(STDOUT_FILENO, msg_lodFD, strlen(msg_lodFD));
        if (FAIL == writes)
        {
            perror(" => Reader <= clear. Status: Error writing to stdout");
        }
    }
    if (FAIL == close(file_dataFD))
    {
        perror(" => Reader <= clear file_dataFD. Close Status");
        stts_dataFD = EXIT_FAILURE;
    }
    else
    {
        writes = write(STDOUT_FILENO, msg_dataFD, strlen(msg_dataFD));
        if (FAIL == writes)
        {
            perror(" => Reader <= clear. Status: Error writing to stdout");
        }
    }
    if (FAIL == fifo_remove(namedFIFO, rfifoFD))
    {
        perror(" => Reader <= clear rfifoFD. fifo_remove Status");
        stts_fifoFD = EXIT_FAILURE;
    }
    else
    {
        writes = write(STDOUT_FILENO, msg_fifoFD, strlen(msg_fifoFD));
        if (FAIL == writes)
        {
            perror(" => Reader <= clear. Status: Error writing to stdout");
        }
    }
    if (stts_logFD && stts_dataFD && stts_dataFD)
    {
        exit(EXIT_FAILURE);
    }

    return;
}