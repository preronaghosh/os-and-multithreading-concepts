/* First come first served Algorithm 

Theory:
- the process that arrives first is executed first - similar to standing in a queue.
- Implemented with a FIFO structure (queue)
- Process at the head of the queue gets scheduled next
- Non-preemptive: Once a process starts execution, it runs till completion.

How It Works:
- When a process arrives, it is added to the ready queue.
- The scheduler picks the process at the front of the queue.
- That process runs until it's done.
- Then the next process is picked.

Pros:
- Simple to implement
- Fair system due to handling of processes in the order of arrival

Cons:
- poor average waiting time - longer processes might come up before shorter ones
- No way of prioritizing emergency processes (higher priority)

---------------------------------------------------------------------------------------

** Notes:
- Gantt chart is a bar chart that illustrates a particular schedule, including the start 
and finish times of each of the participating processes

*/

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Process {
    int pid;        // unique id for the process
    int arrival;    // time at which the process requested CPU use for the first time
    int burst;      // Burst Time : total execution time required by the process on the CPU
    int waiting;    // Waiting Time = arrival time - start time (how long this process waited in the queue)
    int turnaround; // Turnaround Time = total time from arrival to completion
};

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
    }

    // Sort by arrival time
    sort(processes.begin(), processes.end(), sortByArrival);

    int currentTime = 0;

    // Calculate waiting and turnaround times
    for (int i = 0; i < n; i++) {
        if (currentTime < processes[i].arrival)
            currentTime = processes[i].arrival; // CPU idle until arrival

        processes[i].waiting = currentTime - processes[i].arrival;
        currentTime += processes[i].burst;
        processes[i].turnaround = processes[i].waiting + processes[i].burst;
    }

    // Output
    cout << "\nPID\tArrival\tBurst\tWaiting\tTurnaround\n";
    for (auto p : processes) {
        cout << "P" << p.pid << "\t" << p.arrival << "\t"
             << p.burst << "\t" << p.waiting << "\t" << p.turnaround << "\n";
    }

    // Calculate average WT and TAT
    double avgWT = 0, avgTAT = 0;
    for (auto p : processes) {
        avgWT += p.waiting;
        avgTAT += p.turnaround;
    }
    avgWT /= n;
    avgTAT /= n;

    cout << "\nAverage Waiting Time: " << avgWT;
    cout << "\nAverage Turnaround Time: " << avgTAT << endl;

    return 0;
}
