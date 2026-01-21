#include <iostream>
#include <fstream>
#include <cstring>
#include "zipfian_generator.h"

using namespace std;
using namespace ycsbc;

int main(int argc, char** argv) {
    if(argc!=5) return 0;
    // Usage: zipf_gen [outfile] [num_keys] [max_key] [theta]
    string filename = argv[1];
    int n = stoi(argv[2]);
    int range = stoi(argv[3]);
    float theta = stof(argv[4]);
    // int arr[100]={0};
    std::ofstream out;
    ZipfianGenerator generator(0, range, theta);
    // uint64_t last=0;
    out.open(filename, std::ios::app);
    for(int i=0;i<n;i++) {
            uint64_t temp=generator.Next();
            out << to_string(temp);
            out << "\n";
            // if(temp != last) {
            //     last=temp;
            //     cout << last << endl;
            // }
            // arr[temp/10]+=1;
    }
    // for(int j=0;j<100;j++) {
    //         cout << j*10 << "~" << (j+1)*10  << " | " << arr[j] << " | ";
    //         for(int k=0;k<arr[j];k+=10) {
    //                 cout << "*";
    //         }
    //         cout << endl;
    // }
    return 0;
}