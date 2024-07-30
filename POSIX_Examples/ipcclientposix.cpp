#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>

int main(){
    const char *name = "/my_shm";
    const int SIZE = 4096;
    
    int shm_fd = shm_open(name, O_RDWR, 0666);
    if(shm_fd < 0){
        std::cerr << "shm_open hatasi" << std::endl;
        exit(1);
    }

 /*  
 
 HATA :: FTRUNCATE SADECE SHARED MEMORYİ İLK OLUŞTURURKEN KULLANILIR!!!!!
   //en başta 0 byte o yüzden boyutu ayarlamlıyız
    if(ftruncate(shm_fd, SIZE)==-1){
        std::cerr << "ftruncate hatasi "<< std::endl;
        exit(1);
    }
    */

    //file descriptorı bağlamalıyız bellek adresi döndürür
    char *shmptr = (char*) mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    // prot_write eklemezsek yazma iznimiz olmaz ve segöentation error core dumoed alırız

    /*mmap ile shared memory segmenti bellekle eşleştiriliyor ve MAP_SHARED bayrağı ile eşleştirilen bellek alanındaki değişikliklerin diğer süreçler tarafından da görülebilir olması sağlanıyor.*/
    if(shmptr == MAP_FAILED){
        std::cerr << "shared memory belleğe eşleştirilemedi" << std::endl;
        close(shm_fd);
        exit(1);
    }

    char *temp = shmptr;
    while(*temp != '%'){
        std::cout << *temp;
        temp++;
    }
    std::cout << std::endl;


    //servera okumanın bittiğini söyle
    *shmptr = '#';
    //shared memorye yazabilmemiz için =_RDWR İLE AÇMALIYIZ

    if(munmap(shmptr, SIZE)==-1){
        std::cout<<"mapping silinemedi"<<std::endl;
    }

    close(shm_fd);

    return 0;
}
