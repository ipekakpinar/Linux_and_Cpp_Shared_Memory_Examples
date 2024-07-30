#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <cstring>

int main() {
    int shmID;
    char *shmptr, *temp;
    key_t key = 9876;
    const char *text = "hello linux"; // shared memorydeki info
    
    // 1: SHARED MEMORY OLUŞTUR
    shmID = shmget(key, 40, IPC_CREAT | 0666);
    if(shmID < 0) {
        std::cerr << "shared memory oluşturulamadı" << std::endl;
        exit(0);
    }

    // 2: SHARED MEMORYİ EKLE YA DA ADRESİ AL
    shmptr = (char*) shmat(shmID, NULL, 0);
    if(shmptr == (char*) -1) { // shmptr char old için casting yaparız
        std::cerr << "shared memory bağlanılamadı" << std::endl;
        exit(0);
    }

    // 3: WRITE (SHARED MEMORY'E)
    // i. write
    // step a: shared memorynin başını saklarız
    temp = shmptr;
    // step b: write/read
    int len = strlen(text);
    for(int i = 0; i < len; i++) {
        *temp = *text;
        temp++;
        text++;
    }

    // step c: sona terminating char koy
    *temp = '%';

    // ii. server clientı beklesin
    while(*shmptr != '#') {
        sleep(1);
    }

    // 4: DETACH SHARED MEMORY
    shmdt(shmptr);

    // 5: SHARED MEMORY'İ SİL
    shmctl(shmID, IPC_RMID, NULL);

    return 0;
}
