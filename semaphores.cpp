#include <semaphore.h>
using namespace std; 

#define MAX 1
sem_t full, empty, mutex;

int buffer[MAX]; 
int fill = 0, use = 0; // pointers to buffer positions to write into and read from

void put(int value) {
    buffer[fill] = value;
    fill = (fill + 1) % MAX;
}

int get() {
    int val = buffer[use];
    use = (use + 1) % MAX;
    return val;
}

void producer() {
    // will produce data into buffer 10 times
    for(int i=0; i<10; i++) {
        sem_wait(&empty);
        sem_wait(&mutex); // start of critical section
        put(i+1);
        sem_post(&mutex); // end of critical section
        cout << "Producer wrote data: " << (i+1) << endl;
        sem_post(&full);
    }

}

void consumer() {
    // will read from buffer 10 times
    for(int i=0; i<10; i++) {
        // should read only when producer has written data into the buffer
        sem_wait(&full);
        sem_wait(&mutex); // start of critical section
        int val = get();
        sem_post(&mutex); // end of critical section
        cout << "Consumer received data: " << val << endl;
        sem_post(&empty);
    }
}

int main() {
    sem_init(full, 0, 0);
    sem_init(empty, 0, MAX);

    sem_wait(mutex, 0, 1); // binary semaphore as a mutex

}