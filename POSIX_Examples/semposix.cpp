//PRODUCER

#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <semaphore.h>
#include <cstdlib>
#include <ctime>

const char *shm_name = "/numbers_shm";
const char *sem_write_name = "/sem_write";
const char *sem_read_name = "/sem_read";
const int SIZE = sizeof(int);

int main(){
    srand(time(NULL));

    //1-SHARED MEMORY OLUŞTUR
    int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    if(shm_fd == -1){
        std::cerr<< "shm_open hata" << std::endl;
        exit(1);
    }

    //2- shared memorynin boyutunu ayarla
    ftruncate(shm_fd, SIZE);


    //3- shared memooryi bellekle eşleştirme
    char *shmptr = (char*) mmap(NULL, SIZE, PROT_READ | PROT_WRITE , MAP_SHARED, shm_fd, 0);
    if (shmptr == MAP_FAILED) {
        std::cerr << "mmap hata" << std::endl;
        exit(1);
    }

    sem_t *sem_write = sem_open(sem_write_name, O_CREAT, 0666, 1);//1 ile başlattık
    sem_t *sem_read = sem_open(sem_read_name, O_CREAT, 0666, 0);

    if (sem_write == SEM_FAILED || sem_read == SEM_FAILED) {
        std::cerr << "Semaphore oluşturması başarısız" << std::endl;
        exit(1);
    }

    for(int i=0; i<10; ++i){
        sem_wait(sem_write);

        int num = rand()%100;
        *((int*)shmptr) = num;
        std::cout << "gonderilen sayi " << num << std::endl;

        sem_post(sem_read);
        sleep(1);
    }


    //temizlik
    shm_unlink(shm_name);
    sem_unlink(sem_write_name);
    sem_unlink(sem_read_name);
    munmap(shmptr, SIZE);
    close(shm_fd);

    return 0;


}
