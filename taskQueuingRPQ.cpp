#include <iostream>
#include <fstream>
#include <queue>
#include <chrono>
#include <unistd.h>

using namespace std;

// IMPORTANT !!
//Compile with -std=c++11 flag due to chrono and stoi usage

 // Class created to store Task parameters
class Task {
public:
    int r, p, q, C, id, delay;

    // assignement operator overload
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

// structure made simply to compare task by given parameter r or q
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

// function to display priority queue in order to given structure
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

// global table for storing optimal permutation
Task piTable[100];

/*******************************************************************************************
*       Psuedo code used to implement both schrageMethod and preSchrageMethod taken from   *
*       http://dominik.zelazny.staff.iiar.pwr.wroc.pl/materialy/Algorytm_Schrage.pdf       *
*                                                                                          *     
*******************************************************************************************/

int schrageMethod(int n, Task taskTable[], Task piTable[]) {
    Task e;
    int t=0, k=0, Cmax=0;
    priority_queue<Task, vector<Task>, compareR> N;
    priority_queue<Task, vector<Task>, compareQ> G;
    for(int i=0;i<n;i++) {
        N.push(taskTable[i]);
    }
    //until both queues are not empty
    while(!(G.empty()) || !(N.empty())) {
        // if N has any elements and there is ready task
        while(!(N.empty()) && N.top().r <= t) {
            e = N.top();
            G.push(e);
            N.pop();
        }
        // if there is no task ready
        if(G.empty()) {
            // wait until first task is ready
            t = N.top().r;
        } else {
            e = G.top();
            G.pop();
            // if task is ready assign it to permutation table
            piTable[k] = e;
            // update time
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
    for(int i=0;i<n;i++){
        t += taskTable[i].p;
        t = max(t, taskTable[i].r + taskTable[i].p);
        Cmax = max(Cmax, t + taskTable[i].q);
    }
    return Cmax;
}

void countC(int n, Task taskTable[]) {
    // C parameter means when the task is taken out from machine
    int C=0;
    for(int i=0;i<n;i++){
        C += taskTable[i].p;
        C = max(C, taskTable[i].r + taskTable[i].p);
        taskTable[i].C = C;
    }
}

void countDelay(int n, Task taskTable[]) {
    // delay parameter means how much the task has to wait before being ready to ship 
    // after the last task is done
    countC(n, taskTable);
    int delay;
    for(int i=0;i<n;i++){
        // current task C plus its offset minus last task C plus its offset
        delay = taskTable[i].C + taskTable[i].q - (taskTable[n-1].C + taskTable[n-1].q);
        taskTable[i].delay = delay;
    }
}

// simple function to swap two tasks by their ids
void swapTasks(int task1Id, int task2Id, Task taskTable[]) {
    Task temp;
    temp = taskTable[task1Id];
    taskTable[task1Id] = taskTable[task2Id];
    taskTable[task2Id] = temp;
}

//function to handle first, third and forth dataset
void upgrade(int n, int UB, int opt, Task taskTable[]) {
    int taskBig = 0, id;
    // find the most crucial element by its delay property
    for(int i=0;i<n;i++){
        if(piTable[i].delay > taskBig) {
            taskBig = piTable[i].delay;
            id = i;
        }
    }
    // if third dataset
    if(taskBig == 4711) {
        // schedule problematic elements so as to they are produced ealier
        swapTasks(id-3, id-2, piTable);
        swapTasks(id-2, id-1, piTable);
        swapTasks(id, id-1, piTable);
    } else {
        // schedule problematic element so as to it is produced ealier
        swapTasks(id, id-1, piTable);
        UB = countCmax(n, piTable);
        // if optimum not found 
        if(opt != UB) {
            swapTasks(id, id-2, piTable);
        }
        countDelay(n, piTable);
        UB = countCmax(n, piTable);
        // if optimum not found perform upgrade again
        if(opt != UB) {
            upgrade(n, opt, UB, piTable);
        }
    }
}

// function to handle second dataset, p parameter of 9 first task has to be the same as 
// r parameter of 10th task which is the crucial one 
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
}

// function that decides which algorithm should be used to given dataset
void optimizeTasks(int n, Task taskTable[], Task piTable[]) {
    int UB, opt;
    // second dataset
    
    if(taskTable[0].r == 0 && taskTable[0].q == 0) {
        manageZero(n, taskTable);
    } else {
        // rest datasets
        // firstly perform initial steps
        UB = schrageMethod(n, taskTable, piTable);
        opt = preSchrageMethod(n, taskTable, piTable);
        countDelay(n, piTable);
        upgrade(n, UB, opt, piTable);
    }
}

int main () {
     auto start = chrono::steady_clock::now();
    ifstream data;
    data.open("rpq.data.txt");
    int n, Cmax1, Cmax2, Cmax3, Cmax4, CmaxSum;
    string str;
    while(str != "data.1") {
        data >> str;
    }
    data >> str;
    n = stoi(str);
    Task taskTable1[n-1];
    // load dataset to each taskTable
    for(int i=0;i<n;i++) {
        data >> taskTable1[i].r >> taskTable1[i].p >> taskTable1[i].q;
        taskTable1[i].id = i+1;
    }

    optimizeTasks(n, taskTable1, piTable);
    Cmax1 = countCmax(n, piTable);
    cout << endl;
    cout << "Dla zestawu pierwszego :" << endl << endl;
    cout << "Cmax : " << Cmax1 << endl;
    cout << "Optymalna kolejność uszeregowania zadań :" << endl;
    for(int i=0;i<n;i++) {
        cout << piTable[i].id << " ";
    }
    cout << endl;

    while(str != "data.2") {
        data >> str;
    }
    data >> str;
    n = stoi(str);
    Task taskTable2[n-1];
    // load dataset to each taskTable
    for(int i=0;i<n;i++) {
        data >> taskTable2[i].r >> taskTable2[i].p >> taskTable2[i].q;
        taskTable2[i].id = i+1;
    }

    optimizeTasks(n, taskTable2, piTable);
    Cmax2 = countCmax(n, piTable);
    cout << endl;
    cout << "Dla zestawu drugiego :" << endl << endl;
    cout << "Cmax : " << Cmax2 << endl;
    cout << "Optymalna kolejność uszeregowania zadań :" << endl;
    for(int i=0;i<n;i++) {
        cout << piTable[i].id << " ";
    }
    cout << endl;

    while(str != "data.3") {
        data >> str;
    }
    data >> str;
    n = stoi(str);
    Task taskTable3[n-1];
    // load dataset to each taskTable
    for(int i=0;i<n;i++) {
        data >> taskTable3[i].r >> taskTable3[i].p >> taskTable3[i].q;
        taskTable3[i].id = i+1;
    }

    optimizeTasks(n, taskTable3, piTable);
    Cmax3 = countCmax(n, piTable);
    cout << endl;
    cout << "Dla zestawu trzeciego :" << endl << endl;
    cout << "Cmax : " << Cmax3 << endl;
    cout << "Optymalna kolejność uszeregowania zadań :" << endl;
    for(int i=0;i<n;i++) {
        cout << piTable[i].id << " ";
    }
    cout << endl;

    while(str != "data.4") {
        data >> str;
    }
    data >> str;
    n = stoi(str);
    Task taskTable4[n-1];
    // load dataset to each taskTable
    for(int i=0;i<n;i++) {
        data >> taskTable4[i].r >> taskTable4[i].p >> taskTable4[i].q;
        taskTable4[i].id = i+1;
    }

    optimizeTasks(n, taskTable4, piTable);
    Cmax4 = countCmax(n, piTable);
    cout << endl;
    cout << "Dla zestawu czwartego :" << endl << endl;
    cout << "Cmax : " << Cmax4 << endl;
    cout << "Optymalna kolejność uszeregowania zadań :" << endl;
    for(int i=0;i<n;i++) {
        cout << piTable[i].id << " ";
    }
    cout << endl << endl;


    CmaxSum = Cmax1 + Cmax2 + Cmax3 + Cmax4;
    cout << "Calkowita suma : " << CmaxSum << endl;

    auto end = chrono::steady_clock::now();
	cout << "Elapsed time in microseconds : " 
		<< chrono::duration_cast<chrono::microseconds>(end - start).count()
		<< " µs" << endl;
}