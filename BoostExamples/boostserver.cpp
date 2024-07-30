#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>
#include <cstring>
#include <iostream>

int main(){

    //shared memory ismi
    const char *shm_name = "boostshm";
    const char *text = "hello linux";


    try{

        // 1- Paylaşılan bellek oluştur

        boost::interprocess::shared_memory_object shm(boost::interprocess::create_only, shm_name, boost::interprocess::read_write);
        shm.truncate(4096);
         /*
            shared_memory_object nesnesi oluşturuldu.
            bu nesne, belirli bir isim ve erişim moduyla paylaşılabilir bellek segmentini temsil etmektedir.
            shm = boost::interprocess::shared_memory_object sınıfının bir nesnesi

            -boost::interprocess::shared_memory_object sınıfı:
            Boost.Interprocess kütüphanesinde paylaşılan bellek segmentlerini temsil eder

            -boost::interprocess::create_only bayrağı:
            Bu bayrak, paylaşılan bellek segmentinin yalnızca oluşturulması gerektiğini belirtir.
            Eğer belirtilen isimde bir segment zaten mevcutsa, bu işlem başarısız olur. 
            yeni bir segment oluşturmak için kullanılır.

            -shm_name değişkeni:
            Paylaşılan bellek segmentinin ismini belirten bir karakter dizisidir.
            Bu isim, süreçlerin bu paylaşılan bellek segmentine erişmek için kullanacağı isimdir.

            -boost::interprocess::read_write bayrağı:
            segment, hem veri okumak hem de veri yazmak için kullanılabilir.



            ---shm.truncate(4096);
             paylaşılan bellek segmentinin boyutunu ayarlamak için kullanılır.
             byte cinsinden
        */


       //  2- Paylaşılan belleği harita

        boost::interprocess::mapped_region region(shm, boost::interprocess::read_write);
        void *addr = region.get_address();


        /*
        
            shared_memory_object nesnesi (shm) ile temsil edilen paylaşılan bellek segmenti, bir mapped_region nesnesi (region) ile bellekle eşleştirilir. 
            Bu işlem, paylaşılan bellek segmentinin süreç belleğinde kullanılabilir hale gelmesini sağlar.


            -void *addr = region.get_address();
             eşleştirilen bellek bölgesinin başlangıç adresi alınır ve bir işaretçi (addr) olarak saklanır. 
             Bu adres, paylaşılan bellekteki verilere erişmek için kullanılır.
        */



       char *shmptr = static_cast<char*>(addr);
       /*
       static_cast<char*>(addr) ifadesi:

            static_cast<...>(...): Bu, C++'da tür dönüştürme (type casting) yapmak için kullanılan bir operatördür. 
            Bu operatör, belirtilen türdeki bir işaretçiyi veya değeri başka bir türe güvenli bir şekilde dönüştürür.
       */
       std::strcpy(shmptr, text);
       shmptr[std::strlen(text)] = '%';

       //clientın okumasını bekle
       while(*shmptr != '#'){
        sleep(1000);
       }


       //SHARED MEMORYİ SİLME
       boost::interprocess::shared_memory_object::remove(shm_name);


    }

    catch(const std::exception &e){
        std::cerr << e.what() << std::endl; //.what() hata mesajını detaylı verir
        return 1;
    }

    return 0;

}
