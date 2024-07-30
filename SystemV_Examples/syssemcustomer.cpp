#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <cstdlib>

const int SHM_KEY = 1234;
const int SEM_WRITE_KEY = 5678;
const int SEM_READ_KEY = 91011;
const int SIZE = sizeof(int);



void sem_wait(int sem_id, unsigned int sem_num) {
    sembuf op = {static_cast<unsigned short>(sem_num), -1, 0};
    semop(sem_id, &op, 1);
}

void sem_post(int sem_id, unsigned int sem_num) {
    sembuf op = {static_cast<unsigned short>(sem_num), 1, 0};
    semop(sem_id, &op, 1);
}

int main() {
    
    int shm_id = shmget(SHM_KEY, SIZE, 0666);
    if (shm_id == -1) {
        std::cerr << "shmget hatasi" << std::endl;
        exit(EXIT_FAILURE);
    }

    void *shm_ptr = shmat(shm_id, nullptr, 0);
    if (shm_ptr == (void*)-1) {
        std::cerr << "shmat hatasi" << std::endl;
        exit(EXIT_FAILURE);
    }



    
    int sem_id = semget(SEM_WRITE_KEY, 2, 0666);
    if (sem_id == -1) {
        std::cerr << "Semaphorea erişilemedi" << std::endl;
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 10; ++i) {
        sem_wait(sem_id, 1);

        int number = *((int*)shm_ptr);
        std::cout << "consumer sayi: " << number << std::endl;

        sem_post(sem_id, 0);
    }

   
    shmdt(shm_ptr);

    return 0;
}
