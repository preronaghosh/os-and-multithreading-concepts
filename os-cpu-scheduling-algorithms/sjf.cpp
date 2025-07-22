/* Shortest Job First - CPU Scheduling Algorithm 

- This is a non preemptive algorithm
- More optimal than FCFS as the convoy effect can be avoided.
- Lesser average waiting time

** SRTF:
- The preemptive version of this algorithm is Shortest Remaining Time First algorithm
    - while a process P1 (with shortest CPU burst time) is in execution, another process P2
      may arrive that has an even shorter CPU burst time.
    - OS can suspend P1, run P2 and once completed, run P1 again
- A little more optimal and a bit lesser time average waiting time for all processes. 

*/

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Process {
    int pid;        // Process ID
    int arrival;    // Arrival Time
    int burst;      // Burst Time
    int waiting;    // Waiting Time
    int turnaround; // Turnaround Time
    bool completed; // To track if process is done
};

// Sort by arrival time
bool sortByArrival(Process a, Process b) {
    return a.arrival < b.arrival;
}

int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    vector<Process> processes(n);

    // Input process data
    for (int i = 0; i < n; i++) {
        processes[i].pid = i + 1;
        cout << "Enter arrival time and burst time for P" << i + 1 << ": ";
        cin >> processes[i].arrival >> processes[i].burst;
        processes[i].completed = false;
    }

    // Sort the vector of processes based on arrival time
    sort(processes.begin(), processes.end(), sortByArrival);

    int currentTime = 0, completed = 0;
    vector<Process> result;

    while (completed < n) {
        int idx = -1;
        int shortest = 1e9;

        // Pick the shortest job among the arrived and incomplete ones
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival <= currentTime && !processes[i].completed) {
                if (processes[i].burst < shortest) {
                    shortest = processes[i].burst;
                    idx = i;
                }
            }
        }

        if (idx != -1) {
            processes[idx].waiting = currentTime - processes[idx].arrival;
            currentTime += processes[idx].burst;
            processes[idx].turnaround = processes[idx].waiting + processes[idx].burst;
            processes[idx].completed = true;
            completed++;
            result.push_back(processes[idx]);
        } else {
            currentTime++; // If no process has arrived yet
        }
    }

    // Output
    cout << "\nPID\tArrival\tBurst\tWaiting\tTurnaround\n";
    for (auto &p : result) {
        cout << "P" << p.pid << "\t" << p.arrival << "\t" << p.burst << "\t"
             << p.waiting << "\t" << p.turnaround << "\n";
    }

    // Averages
    double avgWT = 0, avgTAT = 0;
    for (auto &p : result) {
        avgWT += p.waiting;
        avgTAT += p.turnaround;
    }
    cout << "\nAverage Waiting Time: " << avgWT / n;
    cout << "\nAverage Turnaround Time: " << avgTAT / n << endl;

    return 0;
}
