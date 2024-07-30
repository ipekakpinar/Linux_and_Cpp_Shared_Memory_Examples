#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <unistd.h>

const int SHM_KEY =1234;
const int SEM_WR_KEY = 5678;
const int SEM_RD_KEY = 91011;
const int SIZE = sizeof(int);



/*
struct sembuf {
    unsigned int sem_num;  //SEMAFOR DİZİN NO
    short          sem_op;   //OPERASYON TÜRÜ
    short          sem_flg;  //OPERASYON BAYRAKLARI
};

*/
void sem_wait(int sem_id, unsigned int sem_num) {
    sembuf op = {static_cast<unsigned short>(sem_num), -1, 0};
    semop(sem_id, &op, 1);
}

void sem_post(int sem_id, unsigned int sem_num) {
    sembuf op = {static_cast<unsigned short>(sem_num), 1, 0};
    semop(sem_id, &op, 1); //1=kaç tane sumbuf olacağı
}


int main(){

    srand(time(NULL));

    //key ile ilişkili memory aç
    int shm_id = shmget(SHM_KEY, SIZE, IPC_CREAT | 0666); //ipc create yoksa oluşturur varsa olan açılıt
    if (shm_id == -1) {
        std::cerr << "shmget hatasi" << std::endl;
        exit(1);
    }

    //2- bloğu işlem adres alanına eşle pointer döner
    void *shm_ptr = shmat(shm_id, nullptr, 0);
    if (shm_ptr == (void*)-1) {
        std::cerr << "shmat hatasi" << std::endl;
        exit(1);
    }

    //3-semafırları aç
    int sem_id = semget(SEM_WR_KEY, 2, IPC_CREAT | 0666); //Oluşturulacak semafor kümesindeki semafor sayısını belirtir.->2
    if (sem_id == -1) {
        std::cerr << "Semaphore olusturulamadi" << std::endl;
        exit(1);
    }

    semctl(sem_id, 0, SETVAL, 1); // sem_write
    semctl(sem_id, 1, SETVAL, 0); // sem_read
    /*
    int semctl(int semid, int semnum, int cmd, ...);
    semnum:

    Üzerinde işlem yapılacak semaforun dizin numarası. Semafor kümesindeki belirli bir semaforu hedefler.


    */


    for (int i = 0; i < 10; ++i) {
        sem_wait(sem_id, 0);

        int number = rand() % 100;
        *((int*)shm_ptr) = number;
        std::cout << "Producer sayi: " << number << std::endl;

        sem_post(sem_id, 1);
        sleep(1);
    }

    
    shmdt(shm_ptr);
    shmctl(shm_id, IPC_RMID, nullptr);
    semctl(sem_id, 0, IPC_RMID);

    return 0;

    

}
