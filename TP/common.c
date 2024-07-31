/*
 * @Inst: FIUBA - CESE - 2024. Cohorte 22

 * @Subject:
 *      Sistemas Operativos de Proposito General
 *
 * @Module:
 *      common
 *
 * @Brief:
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
/*** includes headers  ***/
#include "common.h"

/*
 * @func:
 *      fifo_create
 *
 * @brief:
 *      overlaps mkfifo system call
 *      and catch errors
 *
 * @param:
 *      const char *fifo_name
 *      mode_t fifo_rights
 *
 * @return:
 *      int stts
 */
int fifo_create(const char *fifo_name, mode_t fifo_rights)
{

    int rtrn = 0;

    rtrn = mkfifo(fifo_name, fifo_rights);

    if (SUCCESS != rtrn)
    {
        if (EEXIST == errno)
        {
            perror(" -- Common -- fifo_create. Status");
            rtrn = SUCCESS;
        }
        else
        {
            perror(" -- Common -- fifo_create. Status: Error to create Named FIFO");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        printf(" -- Common -- fifo_create. Status: Success\n");
    }

    return (rtrn);
}

/*
 * @func:
 *      fifo_remove
 *
 * @brief:
 *      overlaps close and unlink
 *      system calls to named fifo
 *      and catch errors
 *
 * @param:
 *      const char *fifo_name
 *      int fifo_fd
 *
 * @return:
 *      int stts
 */
int fifo_remove(const char *fifo_name, int fifo_fd)
{
    int stts = SUCCESS;

    if (FAIL == close(fifo_fd))
    {
        if (ENOENT == errno)
        {
            perror(" -- Common -- fifo_remove. Close Status");
            stts = SUCCESS;
        }
        else
        {
            perror(" -- Common -- fifo_remove. Close Status");
            stts = FAIL;
        }
    }
    if (FAIL == unlink(fifo_name))
    {
        if (ENOENT == errno)
        {
            perror(" -- Common -- fifo_remove. Unlink Status");
            stts = SUCCESS;
        }
        else
        {
            perror(" -- Common -- fifo_remove. Unlink Status");
            stts = FAIL;
        }

        return (stts);
    }
}

/*
 * @func:
 *      file_open
 *
 * @brief:
 *      overlaps open system call
 *      and catch errors
 *
 * @param:
 *      const char *file_name
 *      int file_flags
 *      mode_t file_mode
 *
 * @return:
 *      int stts
 */
int file_open(const char *file_name, int file_flags, mode_t file_mode)
{
    int rtrn = SUCCESS;

    rtrn = open(file_name, file_flags, file_mode);

    printf(" -- Common -- file_open: %s\n", file_name);

    if (FAIL == rtrn)
    {
        perror(" -- Common -- file_open: Status");
    }
    else
    {
        printf(" -- Common -- file_open. Status: Success\n");
    }

    return (rtrn);
}

/*
 * @func:
 *      file_send
 *
 * @brief:
 *      overlaps write system call
 *      and catch errors
 *
 * @param:
 *      int file_fd
 *      const void *file_buffer
 *      size_t file_count
 *
 * @return:
 *      ssize_t writes
 */
ssize_t file_send(int file_fd, const void *file_buffer, size_t file_count)
{
    size_t writes = 0;
    const char msg[] = " -- Common -- file_send. Sending: ";

    writes = write(file_fd, file_buffer, file_count);

    if (FAIL == writes)
    {
        perror(" -- Common -- file_send. Status: Error writing to file");
    }
    else
    {
        writes = write(STDOUT_FILENO, msg, strlen(msg));
        if (FAIL == writes)
        {
            perror(" -- Common -- file_send. Status: Error writing to stdout");
        }
        else
        {
            writes = write(STDOUT_FILENO, file_buffer, file_count);
        }
        if (FAIL == writes)
        {
            perror(" -- Common -- file_send. Status: Error writing to stdout");
        }
    }

    return (writes);
}

/*
 * @func:
 *      make_msg
 *
 * @brief:
 *      make message to send via named FiFO.
 *      Concatenate Header and Payload
 *
 * @param:
 *      char *string
 *      const char *header
 *      char *payload
 *      size_t header_count
 *      size_t payload_count
 *
 * @return:
 *      counts
 */
int make_msg(char *string, const char *header, char *payload, size_t header_count, size_t payload_count)
{
    int size = 0;

    size = (int)((header_count) + (payload_count + 1));
    memset(string, '\0', size);

    if (string != strncpy(string, header, header_count))
    {
        perror(" -- Common -- make_msg. strncpy Status");
        size = 0;
    }
    if (string != strncat(string, payload, payload_count))
    {
        perror(" -- Common -- make_msg. strncat Status");
        size = 0;
    }
    return (size);
}

/*
 * @func:
 *      file_recvd
 *
 * @brief:
 *
 * @param:
 *      int file_fd
 *      void *file_buffer
 *      size_t file_count
 *
 * @return:
 *      ssize_t reads
 */
ssize_t file_recvd(int file_fd, void *file_buffer, size_t file_count)
{
    size_t reads = 0;
    const char msg[] = " -- Common -- file_recvd. Status: Receiving:";

    reads = read(file_fd, file_buffer, file_count);

    if (reads == FAIL)
    {
        perror(" -- Common -- file_recvd. Status: Error receiving from file");
    }
    if (reads == 0)
    {
        perror(" -- Common -- file_recvd. Status: EOF");
    }

    return (reads);
}