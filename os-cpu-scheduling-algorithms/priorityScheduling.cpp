/* Priority based scheduling algorithm 

- Priority Scheduling is a CPU scheduling algorithm where each process is assigned a priority, 
- The CPU is allocated to the process with the highest priority (usually, the lowest number indicates the highest priority).

- SJF is a type of priority scheduling algorithm. CPU burst time is thwe priority parameter there.

- It can be either preemptive or non-preemptive type.
  In the preemptive kind, if a new process P2 with higher priority arrives
  at the queue while a high priority process P1 is running, OS will preempt 
  P1 and start executing P2. 
  
** Cons:
- Can cause indefinite blocking of processes (starvation)
- Some low priority processes may never get alloted the CPU

- The solution to Starvation is Aging. Slowly increasing the priority of some 
  low priority processes that have been queued a while back.

- Another solution to starvation is round robin + priority scheduling
- Execute higher priority processes first and run round robin algorithm 
  for processes with same priority.

*/

// Non pre-emptive priority scheduling code
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Process {
    int pid;        // Process ID
    int burst;      // Burst Time
    int priority;   // Priority (lower value = higher priority)
    int waiting;    // Waiting Time
    int turnaround; // Turnaround Time
};

bool comparePriority(Process a, Process b) {
    return a.priority < b.priority;
}

int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;
    vector<Process> p(n);

    for (int i = 0; i < n; ++i) {
        cout << "Enter burst time and priority for process " << i + 1 << ": ";
        cin >> p[i].burst >> p[i].priority;
        p[i].pid = i + 1;
    }

    // Sort based on priority
    sort(p.begin(), p.end(), comparePriority);

    // Calculate waiting and turnaround times
    p[0].waiting = 0;
    p[0].turnaround = p[0].burst;
    for (int i = 1; i < n; ++i) {
        p[i].waiting = p[i - 1].waiting + p[i - 1].burst;
        p[i].turnaround = p[i].waiting + p[i].burst;
    }

    // Output
    cout << "\nPID\tPriority\tBurst\tWaiting\tTurnaround\n";
    for (const auto& proc : p) {
        cout << proc.pid << "\t" << proc.priority << "\t\t" << proc.burst
             << "\t" << proc.waiting << "\t" << proc.turnaround << endl;
    }

    return 0;
}
