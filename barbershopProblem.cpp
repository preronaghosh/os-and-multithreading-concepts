#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_CHAIRS 5

sem_t barberReady;
sem_t accessSeats;
sem_t customerReady;
int availableSeats = NUM_CHAIRS;

void* barber(void* arg) {
    while (true) {
        sem_wait(&customerReady); // Wait for a customer to arrive
        sem_wait(&accessSeats);   // Access the seat count
        availableSeats++;         // One customer leaves the waiting room
        sem_post(&barberReady);   // Barber is ready to cut hair
        sem_post(&accessSeats);   // Release the seat count

        // Cut hair
        std::cout << "Barber is cutting hair.\n";
        sleep(rand() % 3); // Simulate time taken to cut hair
    }
    return nullptr;
}

void* customer(void* arg) {
    while (true) {
        sem_wait(&accessSeats); // Access the seat count
        if (availableSeats > 0) {
            availableSeats--;   // Sit in a waiting chair
            std::cout << "Customer " << *(int*)arg << " is waiting.\n";
            sem_post(&customerReady); // Notify barber that there is a customer
            sem_post(&accessSeats);   // Release the seat count
            sem_wait(&barberReady);   // Wait for barber to be ready

            // Get haircut
            std::cout << "Customer " << *(int*)arg << " is getting a haircut.\n";
        } else {
            std::cout << "Customer " << *(int*)arg << " is leaving because no chairs are available.\n";
            sem_post(&accessSeats); // Release the seat count
        }
        sleep(rand() % 5); // Simulate time between customer arrivals
    }
    return nullptr;
}

int main() {
    pthread_t barberThread;
    pthread_t customerThreads[10];
    int customerIDs[10];

    // Initialize semaphores
    sem_init(&barberReady, 0, 0);
    sem_init(&accessSeats, 0, 1);
    sem_init(&customerReady, 0, 0);

    // Create barber thread
    pthread_create(&barberThread, nullptr, barber, nullptr);

    // Create customer threads
    for (int i = 0; i < 10; i++) {
        customerIDs[i] = i + 1;
        pthread_create(&customerThreads[i], nullptr, customer, &customerIDs[i]);
    }

    for (int i = 0; i < 10; i++) {
        pthread_join(customerThreads[i], nullptr);
    }

    // Join barber thread
    pthread_join(barberThread, nullptr);

    // Destroy semaphores
    sem_destroy(&barberReady);
    sem_destroy(&accessSeats);
    sem_destroy(&customerReady);

    return 0;
}
