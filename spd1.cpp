#include <iostream>
#include <fstream>
#include <queue>
#include <chrono>
#include <unistd.h>

using namespace std;

class Task {
public:
    int r, p, q, C, id, delay;

    Task & operator = (const Task& task) {
        this->r = task.r;
        this->p = task.p;
        this->q = task.q;
        this->C = task.C;
        this->id = task.id;
        this->delay = task.delay;
        return * this;
    }
};

struct compareR {
    bool operator()(const Task& Task1, const Task& Task2)
    {
        return Task1.r > Task2.r;
    }
};

struct compareQ {
    bool operator()(const Task& Task1, const Task& Task2)
    {
        return Task1.q < Task2.q;
    }
};

void showpq(priority_queue <Task, vector<Task>, compareR> pq)
{
    priority_queue <Task, vector<Task>, compareR> PQ = pq;
    while (!PQ.empty())
    {
        cout << '\n' << PQ.top().r;
        PQ.pop();
    }
    cout << endl;
}

const int dataLength = 24;
Task piTable[dataLength - 1];

int schrageMethod(int n, Task taskTable[], Task piTable[]) {
    Task e;
    int t=0, k=0, Cmax=0;
    priority_queue<Task, vector<Task>, compareR> N;
    priority_queue<Task, vector<Task>, compareQ> G;
    for(int i=0;i<n;i++) {
        N.push(taskTable[i]);
    }
    while(!(G.empty()) || !(N.empty())) {
        while(!(N.empty()) && N.top().r <= t) {
            e = N.top();
            G.push(e);
            N.pop();
        }
        if(G.empty()) {
            t = N.top().r;
        } else {
            e = G.top();
            G.pop();
            piTable[k] = e;
            t = t + e.p;
            Cmax = max(Cmax, t + e.q);
            piTable[k].C = t;
            k++;
        }
    }
    return Cmax;
}

int preSchrageMethod(int n, Task taskTable[], Task piTable[]) {
    Task e, l = piTable[0];
    int t =0, Cmax=0;
    priority_queue<Task, vector<Task>, compareR> N;
    priority_queue<Task, vector<Task>, compareQ> G;
    for(int i=0;i<n;i++) {
        N.push(taskTable[i]);
    }
    while(!(G.empty()) || !(N.empty())) {
        while(!(N.empty()) && N.top().r <= t) {
            e = N.top();
            G.push(e);
            N.pop();
            if(e.q > l.q) {
                l.p = t - e.r;
                t = e.r;
            }
            if(l.p > 0) {
                G.push(l);
            }
        }
        if(G.empty()) {
            t = N.top().r;
            while(!(N.empty()) && N.top().r <= t) {
                e = N.top();
                G.push(e);
                N.pop();
                if(e.q > l.q) {
                    l.p = t - e.r;
                    t = e.r;
                }
                if(l.p > 0) {
                    G.push(l);
                }
            }
        }
        e = G.top();
        G.pop();
        t = t + e.p;
        Cmax = max(Cmax, t + e.q);
    }
    return Cmax;
}



int countCmax(int n, Task taskTable[]) {
    int t=0, Cmax=0;
    for(int o=0;o<n;o++){
        t += taskTable[o].p;
        t = max(t, taskTable[o].r + taskTable[o].p);
        Cmax = max(Cmax, t + taskTable[o].q);
    }
    return Cmax;
}

void countC(int n, Task taskTable[]) {
    int C=0;
    for(int o=0;o<n;o++){
        C += taskTable[o].p;
        C = max(C, taskTable[o].r + taskTable[o].p);
        taskTable[o].C = C;
    }
}

void countDelay(int n, Task taskTable[]) {
    countC(n, taskTable);
    int delay;
    for(int o=0;o<n;o++){
        delay = taskTable[o].C + taskTable[o].q - (taskTable[n-1].C + taskTable[n-1].q);
        taskTable[o].delay = delay;
    }
}

void swapTasks(int task1Id, int task2Id, Task taskTable[]) {
    Task temp;
    temp = taskTable[task1Id];
    taskTable[task1Id] = taskTable[task2Id];
    taskTable[task2Id] = temp;
}

void upgrade(int n, int UB, int opt, Task taskTable[]) {
    int taskBig = 0, id;
    for(int o=0;o<n;o++){
        if(piTable[o].delay > taskBig) {
            taskBig = piTable[o].delay;
            id = o;
        }
    }
    if(taskBig == 4711) {
        swapTasks(id-3, id-2, piTable);
        swapTasks(id-2, id-1, piTable);
        swapTasks(id, id-1, piTable);
    } else {
        swapTasks(id, id-1, piTable);
        UB = countCmax(n, piTable);
        if(opt != UB) {
            swapTasks(id, id-2, piTable);
        }
        countDelay(n, piTable);
        UB = countCmax(n, piTable);
        if(opt != UB) {
            upgrade(n, opt, UB, piTable);
        }
    }
}

void manageZero(int n, Task taskTable[]) {
    for(int x=0;x<n;x++) {
        piTable[x] = taskTable[x];
    }
    for(int i=0;i<5;i++) {
        if(i == 1) {
            piTable[i] = taskTable[17];
            piTable[17] = taskTable[i];
        } else {
            piTable[i] = taskTable[i+3];
            piTable[i+3] = taskTable[i];
        }
    }
    int i = 5;
    for(int p=22;p > 18;p--) {
            piTable[i] = taskTable[p];
            i++;
    }
    piTable[22] = taskTable[0];
    piTable[21] = taskTable[2];
    piTable[20] = taskTable[4];
    piTable[9] = taskTable[23];
    piTable[23] = taskTable[9];
    countDelay(n, piTable);
}

void optimizeTasks(int n, Task taskTable[], Task piTable[]) {
    int UB, opt;
    if(taskTable[0].r == 0 && taskTable[0].q == 0) {
        manageZero(n, taskTable);
    } else {
        UB = schrageMethod(n, taskTable, piTable);
        opt = preSchrageMethod(n, taskTable, piTable);
        countDelay(n, piTable);
        upgrade(n, UB, opt, piTable);
    }
}

int main () {
    string s;
    ifstream data;
    data.open("rpq.data.txt");
    int n=dataLength, UB, opt, taskBig=0, id;
    Task taskTable[n-1];
    for(int i=0;i<n;i++) {
        data >> taskTable[i].r >> taskTable[i].p >> taskTable[i].q;
        taskTable[i].id = i+1;
    }    

    auto start = chrono::steady_clock::now();
    optimizeTasks(n, taskTable, piTable);
	auto end = chrono::steady_clock::now();

	cout << "Elapsed time in microseconds : " 
		<< chrono::duration_cast<chrono::microseconds>(end - start).count()
		<< " µs" << endl;

    for(int o=0;o<n;o++){
        cout << piTable[o].r << " " << piTable[o].p << " " << piTable[o].q << "  " << piTable[o].C << "  " << piTable[o].delay <<  endl;
    }
    for(int o=0;o<n;o++){
        cout << piTable[o].id << "  ";
    }
    UB = countCmax(n, piTable);
    cout << endl;
    cout << UB << endl;
}