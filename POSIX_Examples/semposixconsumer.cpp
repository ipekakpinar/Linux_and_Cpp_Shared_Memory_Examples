#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <semaphore.h>
#include <cstdlib>


const char *shm_name = "/numbers_shm";
const char *sem_write_name = "/sem_write";
const char *sem_read_name = "/sem_read";
const int SIZE = sizeof(int);

int main(){

    //1- shared memoryi aç
    int shm_fd = shm_open(shm_name, O_RDWR, 0666);
    if(shm_fd == -1){
        std::cerr<< "shm_open hata" << std::endl;
        exit(1);
    }


    //2- shared memoryi belleğe nbağla
    char *shmptr = (char*) mmap(0, SIZE, PROT_READ , MAP_SHARED, shm_fd, 0);
    if (shmptr == MAP_FAILED) {
        std::cerr << "mmap hata" << std::endl;
        exit(1);
    }


    sem_t *sem_write = sem_open(sem_write_name, O_CREAT, 0);
    sem_t *sem_read = sem_open(sem_read_name, O_CREAT, 0);

    if (sem_write == SEM_FAILED || sem_read == SEM_FAILED) {
        std::cerr << "Semaphore oluşturması başarısız" << std::endl;
        exit(1);
    }


    for (int i = 0; i < 10; ++i) {
        sem_wait(sem_read);

        int number = *((int*)shmptr);
        std::cout << "alınan sayi " << number << std::endl;

        sem_post(sem_write);
    }

    
    munmap(shmptr, SIZE);
    close(shm_fd);

    return 0;

}
