#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>

int main() {
    int shmID;
    char *shmptr, *temp;
    key_t key = 9876;

    // Shared memory al
    shmID = shmget(key, 40, 0666);
    /*
    int shmflg:

Purpose: This parameter specifies the permissions and flags for the shared memory segment.
Details:
The permissions are specified using an octal number, similar to file permissions in Unix (e.g., 0666).
0 indicates no special flags.
The next three digits (666) specify the permissions:
The first digit (6): read and write permissions for the owner.
The second digit (6): read and write permissions for the group.
The third digit (6): read and write permissions for others.
You can also include additional flags:
IPC_CREAT: Create the segment if it doesn't already exist.
IPC_EXCL: Used with IPC_CREAT, ensures that this call creates the segment. If the segment already exists, the call fails.
    */
    if(shmID < 0) {
        std::cerr << "shmget error" << std::endl;
        exit(1);
    }

    // Shared memory'i attachle
    shmptr = (char*) shmat(shmID, NULL, 0);
    if(shmptr == (char*) -1) {
        std::cerr << "shmat error" << std::endl;
        exit(1);
    }

    // Read işlemi
    // i. read
    // a. headi koru
    temp = shmptr;
    // b. read
    while(*temp != '%') { // terminating character gelene kadar oku
        std::cout << *temp;
        temp++;
    }
    std::cout << std::endl;

    // Server'a okumanın bittiğini söyle
    *shmptr = '#';

    // Detach
    shmdt(shmptr);

    return 0;
}
