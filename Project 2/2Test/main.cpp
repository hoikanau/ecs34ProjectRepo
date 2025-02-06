#include <iostream>
#include <fstream>

using namespace std;

int main(){
    ifstream inFile;
    string line = "";
    inFile.open("input.csv");
    while(getline(inFile, line)){
        cout << line << "\n";
    }
    
}