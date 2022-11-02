/*
Rosa Kilmer
CS33211
producer.c
Producer program for Producer-Consumer assignment
*/ 

#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(){
    int shareMem;
    int* table;

    //Allocate the shared memory
    shareMem = shm_open("table",O_CREAT | O_RDWR, 0666); //Create the table
    ftruncate(shareMem, sizeof(int)); //Setting the size of the shared memory
    table = static_cast<int*>(mmap(0,sizeof(int),PROT_READ | PROT_WRITE, MAP_SHARED, shareMem, 0)); //Mapping object to address
    sem_t* full = sem_open("full",O_CREAT,0666,0);
    sem_t* empty = sem_open("empty",O_CREAT,0666,3);
    sem_t* mutex = sem_open("mutex",O_CREAT, 0666, 1);

    std::cout << "Process is running!\n" << std::endl;

    for (int i = 0; i < 5; ++i) {
        sem_wait(empty);
        sleep(1); // sleep for 1 sec

        sem_wait(mutex); // unlock
        if (*table < 2) { // if table isn't full: (that is, the table does not have 2 items))
            ++(*table);    // add new item in the table
            std::cout << "Produced an item." << std::endl << "Table contains: " << *table << " items\n";
        }
        else {
            std::cout << "Table is full!\n";
        }
        sem_post(mutex); // close the mutex
        sem_post(full);
    }
    sleep(3);   // sleep for 1 sec
    std::cout << "Press enter to exit the completed  process.";

    //Closing semaphores

    sem_close(full);
    sem_close(empty);
    sem_close(mutex);

    //Unlink semaphores

    sem_unlink("full");
    sem_unlink("empty");
    sem_unlink("mutex");

    //Deallocate the shared memory
    munmap(table, sizeof(int));
    close(shareMem);
    shm_unlink("table");
    return 0;
}