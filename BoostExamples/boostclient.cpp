#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <iostream>

int main(){

    //1- shared memoryinin adını gir
    const char *shm_name = "boostshm";

    try{
        //shared memory objectini açma
        boost::interprocess::shared_memory_object shm(boost::interprocess::open_only, shm_name, boost::interprocess::read_write);

        //shared memoryi haritala
        boost::interprocess::mapped_region region(shm, boost::interprocess::read_write);
        //başlangıç adresini al
        void *addr = region.get_address();

        char *shmptr = static_cast<char*>(addr);
        char *temp;
        temp = shmptr;

        while(*temp != '%'){
            std::cout << *temp;
            temp++;
        }

        std::cout << std::endl;

        //servera okumanın bittiğini haber ver
        *shmptr = '#';

    }
    catch(std::exception &e){
        std::cerr<<e.what()<<std::endl;
        return 1;
    }

    return 0;
}
