#include <iostream>
#include <fstream>

using namespace std;

int main () {
    string s;
    ifstream data;
    data.open("rpq.data.txt");
    int c=0,m=0;
    int R[100], P[100],Q[100];
    for(int i=0;i<24;i++) {
        data >> R[i] >> P[i] >> Q[i];
    }
    for(int o=0;o<24;o++){
        cout << R[o] << " " << P[o] << " " << Q[o] <<  endl;
    }
    // for(int k=0;k<24;k++) {
    //     m = max(m,[k]) + P[k];
    //     c = max(c, m+Q[k]);
    // }
    // cout << c << endl;
};