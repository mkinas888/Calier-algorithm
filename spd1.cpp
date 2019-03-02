#include <iostream>
#include <fstream>
#include <queue> 

using namespace std;

class Task {
public:
    int r, p, q;

    Task & operator = (const Task& task) {
        this->r = task.r;
        this->p = task.p;
        this->q = task.q;
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

void schrageMethod(int n, Task taskTable[], Task piTable[]) {
    Task e;
    int t=0, k=0, Cmax=0;
    priority_queue<Task, vector<Task>, compareR> N;
    priority_queue<Task, vector<Task>, compareQ> G;
    for(int i=0;i<n;i++) {
        N.push(taskTable[i]);
    }
    while(!(G.empty()) || !(N.empty())) {
        while(!(N.empty()) && N.top().r <= t) {
            cout << N.top().r << endl;
            e = N.top();
            G.push(e);
            N.pop();
        }
        if(G.empty()) {
            t = N.top().r;
            while(!(N.empty()) && N.top().r <= t) {
            cout << N.top().r << endl;
            e = N.top();
            G.push(e);
            N.pop();
        }
        }
        e = G.top();
        G.pop();
        piTable[k] = e;
        t = t + e.p;
        Cmax = max(Cmax, t + e.q);
        k = k+1;
    }
}

int main () {
    string s;
    ifstream data;
    data.open("rpq.data.txt");
    int n=24, UB;
    Task taskTable[n-1], piTable[n-1];
    for(int i=0;i<24;i++) {
        data >> taskTable[i].r >> taskTable[i].p >> taskTable[i].q;
    }
    schrageMethod(n, taskTable, piTable);
    for(int o=0;o<24;o++){
        cout << piTable[o].r << " " << piTable[o].p << " " << piTable[o].q <<  endl;
    }
}