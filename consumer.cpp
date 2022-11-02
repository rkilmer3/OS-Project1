/*
Rosa Kilmer
CS33211
consumer.cpp
Consumer program for Producer-Consumer assignment
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

    for (int i = 0; i < 5; ++i){
        sem_wait(full);
        sleep(1); //Sleep for 1 second
        sem_wait(mutex); //Unlock mutex
        if(*table > 0){
            --(*table); //CONSUME
            std::cout << "Item consumed" << std::endl << "There are now " << *table << "items on the table.\n";
        }
        else{
            std::cout << "Your table is empty!\n";
        }
        sem_post(mutex); //Close our mutex
        sem_post(empty);
    }


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