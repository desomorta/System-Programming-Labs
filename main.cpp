#include <pthread.h>
#include <mutex>
#include <dlfcn.h>
#include <fcntl.h>
#include <unistd.h>

typedef void(*readF)(char*,int);
typedef void(*initF)();

std::string a = "/home/desomorta/CLionProjects/SPOVM/lab5files/";
std::string filesArray[8] = {a+"f1.txt",a+"f2.txt",a+"f3.txt",a+"f4.txt",a+"f5.txt",a+"f6.txt",a+"f7.txt",a+"f8.txt"};
readF readFunc;
readF writeFunc;

char buffer[1000];

void* read(void*) {
    for(int i = 0;i < 8;i++) {
        int fileDes = open(filesArray[i].c_str(),NULL);
        readFunc(buffer,fileDes);
        close(fileDes);
    }
}

void* write(void*) {
    int fileDes = open("/home/desomorta/CLionProjects/SPOVM/lab5files/outFile.txt", O_WRONLY);
    for (int i = 0; i < 8; i++) {
        writeFunc(buffer,fileDes);
    }
    close(fileDes);
}

int main() {
    void *libDescriptor = dlopen("./libs.so", RTLD_LAZY);
    readFunc = (readF) dlsym(libDescriptor, "asyncRead");
    writeFunc = (readF) dlsym(libDescriptor, "asyncWrite");
    initF initFunc = (initF) (dlsym(libDescriptor, "initAio"));
    initF destroyFunc = (initF) dlsym(libDescriptor, "destroyAio");
    pthread_t readThread, writeThread;
    initFunc();
    pthread_create(&readThread, NULL, read, NULL);
    pthread_create(&writeThread, NULL, write, NULL);
    pthread_join(readThread, 0);
    pthread_join(writeThread, 0);
    destroyFunc();
    dlclose(&libDescriptor);
    return 0;
}