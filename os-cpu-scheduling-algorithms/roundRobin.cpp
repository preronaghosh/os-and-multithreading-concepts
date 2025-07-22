/* Round Robin (RR) Scheduling algorithm 

- It is a preemptive scheduling algorithm designed for time-sharing systems. 
- Each process is given a fixed time slot (called a time quantum). 
- If a process doesn’t finish in that time, it’s preempted and moved to the back of the ready queue.
- Processes are scheduled in order of arrival from queue.

Pros: 
- Fairness: Every process gets an equal chance in cyclic order


*** Important Points regarding deciding a quantum time:

- If the time quantum is very less, most processes will finish execution before the alloted slot,
  cpu resources will be released automatically. This closely represents FCFS algorithm.
- If the time quantum is too high, too many context switches will be required.
  That adds extra work for the OS

- Quantum time is usually set to 10ms to 100ms (context switch time is usually 10 microseconds)

*/

#include <iostream>
#include <queue>
#include <vector>
using namespace std;

struct Process {
    int pid;
    int arrival;
    int burst;
    int remaining;
    int waiting;
    int turnaround;
    int finish;
};

int main() {
    int n, tq;
    cout << "Enter number of processes: ";
    cin >> n;

    cout << "Enter time quantum: ";
    cin >> tq;

    vector<Process> processes(n);
    queue<int> readyQueue;

    // Input
    for (int i = 0; i < n; i++) {
        processes[i].pid = i + 1;
        cout << "Enter arrival and burst time for P" << i + 1 << ": ";
        cin >> processes[i].arrival >> processes[i].burst;
        processes[i].remaining = processes[i].burst;
        processes[i].waiting = 0;
        processes[i].turnaround = 0;
        processes[i].finish = -1;
    }

    int time = 0, completed = 0;
    vector<bool> inQueue(n, false);

    while (completed < n) {
        // Enqueue all processes that have arrived
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival <= time && !inQueue[i] && processes[i].remaining > 0) {
                readyQueue.push(i);
                inQueue[i] = true;
            }
        }

        if (!readyQueue.empty()) {
            int idx = readyQueue.front();
            readyQueue.pop();

            int runTime = min(tq, processes[idx].remaining);
            processes[idx].remaining -= runTime;
            time += runTime;

            // Enqueue new arrivals that came during this time slice
            for (int i = 0; i < n; i++) {
                if (processes[i].arrival <= time && !inQueue[i] && processes[i].remaining > 0) {
                    readyQueue.push(i);
                    inQueue[i] = true;
                }
            }

            if (processes[idx].remaining > 0) {
                readyQueue.push(idx); // Not done yet, re-enqueue
            } else {
                processes[idx].finish = time;
                completed++;
            }

        } else {
            time++; // CPU is idle
        }
    }

    // Calculate Waiting and Turnaround Times
    double avgWT = 0, avgTAT = 0;
    cout << "\nPID\tArrival\tBurst\tWaiting\tTurnaround\n";
    for (auto &p : processes) {
        p.turnaround = p.finish - p.arrival;
        p.waiting = p.turnaround - p.burst;
        avgWT += p.waiting;
        avgTAT += p.turnaround;
        cout << "P" << p.pid << "\t" << p.arrival << "\t" << p.burst << "\t"
             << p.waiting << "\t" << p.turnaround << "\n";
    }

    cout << "\nAverage Waiting Time: " << avgWT / n;
    cout << "\nAverage Turnaround Time: " << avgTAT / n << endl;

    return 0;
}
