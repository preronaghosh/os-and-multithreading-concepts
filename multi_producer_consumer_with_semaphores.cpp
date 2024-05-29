#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

sem_t empty;
sem_t full;
sem_t mutex;

void* producer(void* arg) {
    int item;
    while (true) {
        sem_wait(&empty);
        sem_wait(&mutex); // enter critical section
        item = rand() % 100;
        buffer[in] = item;
        in = (in+1) % 5;
        std::cout << "Produced: " << item << std::endl;
        sem_post(&mutex); // leave critical section
        sem_post(&full); // signal to consumer that a value has been updated
        sleep(rand() % 3);
    }
    return nullptr;
}

void* consumer(void* arg) {
    int item;
    while (true) {
        sem_wait(&full); // wait for an item to be produced
        sem_wait(&mutex); // enter critical section
        item = buffer[out];
        out = (out+1) % 5;
        std::cout << "Consumed: " << item << std::endl;
        sem_post(&mutex); // leave critical section
        sem_post(&empty); 
        sleep(rand() % 3);
    }
    
    return nullptr;
}

int main() {
    pthread_t prod_thread, cons_thread;

    // Initialize semaphores
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);

    // Create producer and consumer threads
    pthread_create(&prod_thread, nullptr, producer, nullptr);
    pthread_create(&cons_thread, nullptr, consumer, nullptr);

    // Join threads (in practice, you'd have some termination condition)
    pthread_join(prod_thread, nullptr);
    pthread_join(cons_thread, nullptr);

    // Destroy semaphores
    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);

    return 0;
}
