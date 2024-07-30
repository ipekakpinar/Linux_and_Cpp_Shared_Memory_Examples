#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>

int main(){
    const char *name = "/my_shm"; //sharedd memory ismi
    const int SIZE = 4096 ;//bellek boyutu
    const char *text = "hello linux";


    //1: SHARED MEMORY OLUŞTUR
    int shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if(shm_fd < 0){
        std::cerr << "shm_open hatasi" <<std::endl;
        exit(1);
    }

    //2. SHARED MEMORY BOYUTUNU AYARLA - en başta oluşturulunca sıfırdır
    if(ftruncate(shm_fd, SIZE) == -1){
        std::cerr << "shared memory boyuutu ayarlanırken hata " << std::endl;
        exit(1);
    }
    

    //3. SHARED MEMORYİ BELLEKLE EŞLEŞTİRME:
    char *shmptr = (char*) mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if(shmptr == MAP_FAILED){
        std::cerr << "shared memory belleğe eşleştirilemedi" << std::endl;
        close(shm_fd);
        exit(1);
    }


    //4: YAZMA İŞLEMİ
    strcpy(shmptr, text);

    //TErmination karakteri
    shmptr[strlen(text)] = '%';

    //5: SERVER CLİENTIN # İŞARETİNİ BEKLE
    while(*shmptr != '#'){
        sleep(1);
    }

    if(munmap(shmptr, SIZE)==-1){ //mappingi siler
        std::cerr << "shared memory ayrıştırılamadı" << std::endl;
    }

    close(shm_fd);

    //7. shared memoryi sil
    if(shm_unlink(name)==-1){
        std::cerr << "shared memoryi silinemedi" << std::endl;
    }

    return 0;


}
