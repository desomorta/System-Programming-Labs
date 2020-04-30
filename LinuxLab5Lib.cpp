#include <sys/types.h>
#include <aio.h>
#include <pthread.h>
#include <cstring>

pthread_mutex_t wr,re;
aiocb inpFile, outpFile;

extern "C" {
    void initAio() {
    pthread_mutex_init(&wr, NULL);
    pthread_mutex_init(&re, NULL);
    pthread_mutex_lock(&wr);
    inpFile.aio_offset = 0;
    inpFile.aio_nbytes = 200;
    outpFile.aio_offset = 0;
}

void destroyAio() {
    pthread_mutex_destroy(&wr);
    pthread_mutex_destroy(&re);
}

void asyncRead(char *buffer, int fileDescriptor) {
    while (aio_error(&inpFile));
    pthread_mutex_lock(&re);
    inpFile.aio_fildes = fileDescriptor;
    inpFile.aio_buf = buffer;
    memset(buffer, 0, 1000);
    aio_read(&inpFile);
    while (aio_error(&inpFile));
    pthread_mutex_unlock(&wr);
}

void asyncWrite(char *buffer, int fileDescriptor) {
    outpFile.aio_fildes = fileDescriptor;
    pthread_mutex_lock(&wr);
    while (aio_error(&outpFile));
    outpFile.aio_buf = buffer;
    outpFile.aio_nbytes = strlen(buffer);
    aio_write(&outpFile);
    while (aio_error(&outpFile));
    outpFile.aio_offset += strlen(buffer);
    pthread_mutex_unlock(&re);
}

}
