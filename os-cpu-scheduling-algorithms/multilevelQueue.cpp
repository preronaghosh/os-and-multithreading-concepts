/* Multilevel Queue Scheduling 

*** Definition:
Multi-Level Queue Scheduling is a CPU scheduling algorithm that divides the ready queue 
into multiple separate queues, each for a different type or priority of process. 
Each queue has its own scheduling algorithm, and processes are permanently assigned to a 
queue based on some property (e.g., priority, memory needs, process type like system/user).

*** History:
- To implement the round robin + priority scheduling algorithm, we use a single queue.
  Searching for next highest priority process in a queue may require O(n) time, which 
  is very high for a high value of N.

- Hence, we define a single queue for every priority level. Push the process with priority 1
  to queue 1, priority 2 to queue 2 and so on. This way, scheduler just takes the queue.front()
  from the highest priority queue. If it is empty, go to the next highest priority queue.

- In addition, there must be scheduling between the queues, which is commonly implemented as 
  Fixed Priority Preemptive Scheduling. This is called a Queue scheduling policy which decides 
  which queue to select next (e.g., fixed priority, time-slicing among queues).

*** Advantages:
- Separates different types of processes cleanly.
- Efficient handling of real-time vs background tasks.

*** Disadvantages:
- Starvation of lower-priority queues.
- Rigid — processes can't move between queues dynamically.

*/

#include <iostream>
#include <queue>
using namespace std;

struct Process {
    int pid;
    int burst;
};

int main() {
    queue<Process> systemQueue; // High priority queue - FCFS
    queue<Process> userQueue;   // Low priority queue - Round Robin (time quantum = 4)

    int n1, n2;
    cout << "Enter number of system processes: ";
    cin >> n1;
    for (int i = 0; i < n1; ++i) {
        Process p;
        cout << "System Process " << i + 1 << " burst time: ";
        cin >> p.burst;
        p.pid = i + 1;
        systemQueue.push(p);
    }

    cout << "Enter number of user processes: ";
    cin >> n2;
    for (int i = 0; i < n2; ++i) {
        Process p;
        cout << "User Process " << i + 1 << " burst time: ";
        cin >> p.burst;
        p.pid = n1 + i + 1;
        userQueue.push(p);
    }

    int timeQuantum = 4;
    cout << "\n=== Execution Order ===\n";

    // Always prioritize system queue
    while (!systemQueue.empty() || !userQueue.empty()) {
        // Serve all system processes (FCFS)
        while (!systemQueue.empty()) {
            Process p = systemQueue.front();
            systemQueue.pop();
            cout << "System Process " << p.pid << " executed for " << p.burst << " units\n";
        }

        // Serve user processes (Round Robin)
        if (!userQueue.empty()) {
            Process p = userQueue.front();
            userQueue.pop();

            if (p.burst > timeQuantum) {
                cout << "User Process " << p.pid << " executed for " << timeQuantum << " units\n";
                p.burst -= timeQuantum;
                userQueue.push(p); // Reinsert for next round
            } else {
                cout << "User Process " << p.pid << " executed for " << p.burst << " units (finished)\n";
            }
        }
    }

    return 0;
}
