#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Process {
    int pid;
    int arrivalTime;
    int burstTime;
    int waitingTime;
    int completionTime;
    int turnaroundTime;
    int priority;
};

void findFCFS(Process proc[], int n) {
    int currentTime = 0;
    for (int i = 0; i < n; i++) {
        if (currentTime < proc[i].arrivalTime) {
            currentTime = proc[i].arrivalTime;
        }
        proc[i].waitingTime = currentTime - proc[i].arrivalTime;
        currentTime += proc[i].burstTime;
        proc[i].completionTime = currentTime;
        proc[i].turnaroundTime = proc[i].completionTime - proc[i].arrivalTime;
    }
}

void findSJF(Process proc[], int n) {
    vector<Process> processes(proc, proc + n);
    sort(processes.begin(), processes.end(), [](Process a, Process b) {
        return a.burstTime < b.burstTime;
    });

    int currentTime = 0;
    for (int i = 0; i < n; i++) {
        if (currentTime < processes[i].arrivalTime) {
            currentTime = processes[i].arrivalTime;
        }
        processes[i].waitingTime = currentTime - processes[i].arrivalTime;
        currentTime += processes[i].burstTime;
        processes[i].completionTime = currentTime;
        processes[i].turnaroundTime = processes[i].completionTime - processes[i].arrivalTime;
    }
    for (int i = 0; i < n; i++) {
        proc[i] = processes[i];
    }
}

void findPriority(Process proc[], int n) {
    vector<Process> processes(proc, proc + n);
    sort(processes.begin(), processes.end(), [](Process a, Process b) {
        return a.priority < b.priority;
    });

    int currentTime = 0;
    for (int i = 0; i < n; i++) {
        if (currentTime < processes[i].arrivalTime) {
            currentTime = processes[i].arrivalTime;
        }
        processes[i].waitingTime = currentTime - processes[i].arrivalTime;
        currentTime += processes[i].burstTime;
        processes[i].completionTime = currentTime;
        processes[i].turnaroundTime = processes[i].completionTime - processes[i].arrivalTime;
    }
    for (int i = 0; i < n; i++) {
        proc[i] = processes[i];
    }
}

void findRoundRobin(Process proc[], int n, int quantum) {
    int remainingTime[n];
    for (int i = 0; i < n; i++) {
        remainingTime[i] = proc[i].burstTime;
    }

    int currentTime = 0;
    bool done;

    do {
        done = true;
        for (int i = 0; i < n; i++) {
            if (remainingTime[i] > 0) {
                done = false;
                if (remainingTime[i] > quantum) {
                    currentTime += quantum;
                    remainingTime[i] -= quantum;
                } else {
                    currentTime += remainingTime[i];
                    proc[i].completionTime = currentTime;
                    proc[i].waitingTime = currentTime - proc[i].burstTime - proc[i].arrivalTime;
                    proc[i].turnaroundTime = proc[i].completionTime - proc[i].arrivalTime;
                    remainingTime[i] = 0;
                }
            }
        }
    } while (!done);
}

void displayProcessDetails(Process proc[], int n) {
    cout << "Process\tArrivalTime\tBurstTime\tWaitingTime\tCompletionTime\tTurnaroundTime" << endl;
    for (int i = 0; i < n; i++) {
        cout << proc[i].pid << "\t\t" << proc[i].arrivalTime << "\t\t" << proc[i].burstTime << "\t\t" << proc[i].waitingTime << "\t\t" << proc[i].completionTime << "\t\t" << proc[i].turnaroundTime << endl;
    }
    cout << endl;
}

void displayGanttChart(Process proc[], int n) {
    cout << "Gantt Chart:" << endl;
    for (int i = 0; i < n; i++) {
        cout << "|  P" << proc[i].pid << "  ";
       
    }
    cout << "|" << endl;

    cout<<"  ";

    for (int i = 0; i < n; i++) {
        
        cout << proc[i].completionTime << "\t";
    }
    cout << endl;
}

int main() {
    cout<<"enter the number of processes: ";
    int n ;
    cin>>n;
    Process proc[n];
    int choice, quantum;

    cout << "Enter Process Details (ID, Arrival, Burst, Priority)" << endl;
    for (int i = 0; i < n; i++) {
        cout << "Enter Process " << i + 1 << endl;
        cin >> proc[i].pid >> proc[i].arrivalTime >> proc[i].burstTime >> proc[i].priority;
    }

    cout << "Choose Scheduling Algorithm: 1-FCFS, 2-SJF, 3-Round Robin, 4-Priority" << endl;
    cin >> choice;

    if (choice == 3) {
        cout << "Enter Quantum Time for Round Robin: ";
        cin >> quantum;
    }

    switch (choice) {
        case 1:
            findFCFS(proc, n);
            break;
        case 2:
            findSJF(proc, n);
            break;
        case 3:
            findRoundRobin(proc, n, quantum);
            break;
        case 4:
            findPriority(proc, n);
            break;
        default:
            cout << "Invalid choice!" << endl;
            return 0;
    }

    displayProcessDetails(proc, n);
    displayGanttChart(proc, n);

    return 0;
}
