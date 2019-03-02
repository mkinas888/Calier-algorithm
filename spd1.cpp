#include <iostream>
#include <fstream>
#include <queue> 

using namespace std;

class Task {
public:
    int r, p, q;
    int C = r+p+q;

    Task & operator = (const Task& task) {
        this->r = task.r;
        this->p = task.p;
        this->q = task.q;
        this->C = task.C;
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
    return Cmax;
}

int calierMethod(int n, Task taskTable[], int UB) {
    int U, a=0, b=0, c=-1, suma;
    Task piTableX[n-1];
    U = schrageMethod(n, taskTable, piTable);
    if(U < UB) {
        UB = U;
        for(int i = 0;i<n;i++) {
            piTableX[i] = piTable[i];
        }
    }
    for(int i=n-1;i>0;i--) {
        if (U == piTable[i].C + piTable[i].q)
            {
                b = i;
                break;
            }
    }
    for(int a=0;a<b;a++) {
        suma=0;
        for(int i=1; i<=b;i++) {
            suma += piTable[i].p;
        }
        if(U == (piTable[a].r + suma + piTable[b].q)) {
            break;
        }
    }
    for(int i = b; i >= a; i--) {
        if(piTable[i].q < piTable[b].q) {
            c=i;
            break;
        }
    }
    if(c == -1) {
        return U;
    }
}

Task piTable[23];

int main () {
    string s;
    ifstream data;
    data.open("rpq.data.txt");
    int n=24, UB;
    Task taskTable[n-1];
    for(int i=0;i<24;i++) {
        data >> taskTable[i].r >> taskTable[i].p >> taskTable[i].q;
    }
    calierMethod(n, taskTable, 100000);
    // UB = schrageMethod(n, taskTable, piTable);
    // for(int o=0;o<24;o++){
    //     cout << piTable[o].r << " " << piTable[o].p << " " << piTable[o].q <<  endl;
    // }
    // cout << UB << endl;
}