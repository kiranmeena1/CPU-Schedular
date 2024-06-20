#include "crow.h"
#include <vector>
#include <algorithm>

struct Process {
    int pid;
    int arrivalTime;
    int burstTime;
    int waitingTime;
    int completionTime;
    int turnaroundTime;
    int priority;
};

void findFCFS(std::vector<Process>& processes) {
    int currentTime = 0;
    for (auto& proc : processes) {
        if (currentTime < proc.arrivalTime) {
            currentTime = proc.arrivalTime;
        }
        proc.waitingTime = currentTime - proc.arrivalTime;
        currentTime += proc.burstTime;
        proc.completionTime = currentTime;
        proc.turnaroundTime = proc.completionTime - proc.arrivalTime;
    }
}

void findSJF(std::vector<Process>& processes) {
    std::sort(processes.begin(), processes.end(), [](Process a, Process b) {
        return a.burstTime < b.burstTime;
    });

    int currentTime = 0;
    for (auto& proc : processes) {
        if (currentTime < proc.arrivalTime) {
            currentTime = proc.arrivalTime;
        }
        proc.waitingTime = currentTime - proc.arrivalTime;
        currentTime += proc.burstTime;
        proc.completionTime = currentTime;
        proc.turnaroundTime = proc.completionTime - proc.arrivalTime;
    }
}

void findPriority(std::vector<Process>& processes) {
    std::sort(processes.begin(), processes.end(), [](Process a, Process b) {
        return a.priority < b.priority;
    });

    int currentTime = 0;
    for (auto& proc : processes) {
        if (currentTime < proc.arrivalTime) {
            currentTime = proc.arrivalTime;
        }
        proc.waitingTime = currentTime - proc.arrivalTime;
        currentTime += proc.burstTime;
        proc.completionTime = currentTime;
        proc.turnaroundTime = proc.completionTime - proc.arrivalTime;
    }
}

void findRoundRobin(std::vector<Process>& processes, int quantum) {
    int n = processes.size();
    std::vector<int> remainingTime(n);
    for (int i = 0; i < n; i++) {
        remainingTime[i] = processes[i].burstTime;
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
                    processes[i].completionTime = currentTime;
                    processes[i].waitingTime = currentTime - processes[i].burstTime - processes[i].arrivalTime;
                    processes[i].turnaroundTime = processes[i].completionTime - processes[i].arrivalTime;
                    remainingTime[i] = 0;
                }
            }
        }
    } while (!done);
}

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/schedule").methods("POST"_method)([](const crow::request& req) {
        auto x = crow::json::load(req.body);
        std::vector<Process> processes;

        for (auto& process : x["processes"]) {
            processes.push_back(Process{
                process["pid"].i(),
                process["arrivalTime"].i(),
                process["burstTime"].i(),
                0, 0, 0,  // waitingTime, completionTime, turnaroundTime
                process["priority"].i()
            });
        }

        int choice = x["choice"].i();
        int quantum = x["quantum"].i();

        switch (choice) {
            case 1:
                findFCFS(processes);
                break;
            case 2:
                findSJF(processes);
                break;
            case 3:
                findRoundRobin(processes, quantum);
                break;
            case 4:
                findPriority(processes);
                break;
        }

        crow::json::wvalue result;
        for (auto& proc : processes) {
            crow::json::wvalue p;
            p["pid"] = proc.pid;
            p["arrivalTime"] = proc.arrivalTime;
            p["burstTime"] = proc.burstTime;
            p["waitingTime"] = proc.waitingTime;
            p["completionTime"] = proc.completionTime;
            p["turnaroundTime"] = proc.turnaroundTime;
            result["processes"].push_back(p);
        }

        auto res = crow::response(result);
        res.add_header("Access-Control-Allow-Origin", "*");
        return res;
    });

    app.port(8080).multithreaded().run();
}
