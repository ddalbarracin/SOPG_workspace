/*
 * @Inst: FIUBA - CESE - 2024. Cohorte 22

 * @Subject:
 *      Sistemas Operativos de Proposito General
 *
 * @Module:
 *      writer
 *
 * @Brief:
 *      this program receive a string from standar input or a signal
 *      SIGUSR1 or SIGUSR2 and send a message throw named fifo
 *      previusly append a little header.
 *      All signal are blocked except SIGUSR1, SIGUSR2 and SIGQUIT.
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
#include "writer.h"

/*** private variables ***/
static int fifoFD = 0;
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
 *      void
 *
 * @return:
 *      int stts
 */
int main(void)
{
    /* local variables */
    int rtrn = EXIT_FAILURE;
    int stts = EXIT_FAILURE;

    int offset = 0;
    ssize_t readBytes = 0;
    char buffer[WTR_MAX_BUFF];
    char str_msg[WTR_MAX_BUFF];

    sigset_t blk_sgnl_mask;
    struct sigaction writer_sa;

    /* sigaction initialize */
    writer_sa.sa_handler = signal_hndlr;
    writer_sa.sa_flags = SA_RESTART;
    sigfillset(&blk_sgnl_mask);
    sigdelset(&blk_sgnl_mask, SIGUSR1);
    sigdelset(&blk_sgnl_mask, SIGUSR2);
    sigdelset(&blk_sgnl_mask, SIGQUIT);
    writer_sa.sa_mask = blk_sgnl_mask;

    if (FAIL == sigprocmask(SIG_BLOCK, &writer_sa.sa_mask, NULL))
    {
        perror(" => Writer <= Sigprocmask Set Block Status");
        exit(EXIT_FAILURE);
    }

    if (FAIL == sigaction(SIGUSR1, &writer_sa, NULL))
    {
        perror(" => Writer <= Sigaction Set SIGUSR1 Status");
        exit(EXIT_FAILURE);
    }

    if (FAIL == sigaction(SIGUSR2, &writer_sa, NULL))
    {
        perror(" => Writer <=  Sigaction Set SIGUSR2 Status");
        exit(EXIT_FAILURE);
    }

    if (FAIL == sigaction(SIGQUIT, &writer_sa, NULL))
    {
        perror(" => Writer <= Sigaction Set SIGUSR2 Status");
        exit(EXIT_FAILURE);
    }

    /* main flow code */
    printf(" *** Writer *** PID: %d\n", getpid());
    rtrn = fifo_create(namedFIFO, rightsFIFO);

    if (SUCCESS == rtrn)
    {
        printf(" => Writer <= Waiting for Reader...\n");
        fifoFD = file_open(namedFIFO, WTR_FIFO_FLGS, WRT_FILE_MODE);

        if (FAIL == fifoFD)
        {
            clear();
        }
        do
        {
            offset = 0;
            memset(buffer, '\0', sizeof(buffer));

            while (readBytes = (file_recvd(STDIN_FILENO, buffer + offset, 1)) > 0)
            {
                if (WRT_NL == buffer[offset])
                {
                    break;
                }
                offset += 1;
            }
            if (0 == readBytes)
            {
                if (FAIL == kill(getpid(), SIGQUIT))
                {
                    perror(" => Writer <= Sending SIGQUIT Signal");
                    exit(EXIT_FAILURE);
                }
            }
            else
            {

                buffer[offset] = '\n';
                int result = 0;
                result = make_msg(str_msg, data, buffer, (int)strlen(data), strlen(buffer));

                if (0 == result)
                {
                    perror(" => Writer <= make message Status");
                }
                if (file_send(fifoFD, str_msg, (strlen(str_msg))) != (strlen(str_msg)))
                {
                    perror(" => Writer <= Sending Payload Text Status");
                    exit(EXIT_FAILURE);
                }
            }
            stts = SUCCESS;

        } while (sgnlQuit);

        clear();

        if (FAIL == sigprocmask(SIG_UNBLOCK, &writer_sa.sa_mask, NULL))
        {
            perror(" => Writer <= Sigprocmask Set UnBlock Status");
            exit(EXIT_FAILURE);
        }
    }
    printf(" => Writer <= Exit Status: %d\n", stts);

    return (stts);
}

/*
 * @func:
 *      signal_hndlr
 *
 * @brief:
 *      signal handler to take actions
 *      in case a SIGUSR1, SIGUSR2 or
 *      SIGQUIT signal were received.
 *
 * @param:
 *      int signal
 *
 * @return:
 *      void
 */
void signal_hndlr(int signal)
{

    if (SIGUSR1 == signal)
    {
        file_send(fifoFD, wsUSR1, strlen(wsUSR1));
    }
    if (SIGUSR2 == signal)
    {
        file_send(fifoFD, wsUSR2, strlen(wsUSR2));
    }
    if (SIGQUIT == signal)
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
    u_int8_t stts_fifoFD = SUCCESS;

    if (FAIL == fifo_remove(namedFIFO, fifoFD))
    {
        perror(" => Writer <= Clear. fifo_remove Status");
    }
    else
    {
        writes = write(STDOUT_FILENO, msg_fifoFD, strlen(msg_fifoFD));
        if (FAIL == writes)
        {
            perror(" => Writer <= Clear. Status: Error writing to stdout");
            stts_fifoFD = EXIT_FAILURE;
        }
    }
    if (stts_fifoFD)
    {
        exit(EXIT_FAILURE);
    }

    return;
}
