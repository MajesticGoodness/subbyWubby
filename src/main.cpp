#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

void readDaFile(string fileName);

void readDaFile(string fileName) {
    vector<int> charFreqs(26);
    ifstream infile(fileName);

    if (infile) {
        cout << "It's not fudged! :)" << '\n';
    }

    else {
        cout << "It's fudged." << '\n';
    }

    char ch{};
    int indexNumber{};
    // taken from Roberts Programming Abstractions in C++ (2014), pg. 205.
    while (infile.get(ch)) {
        if (isalpha(ch)) {
            indexNumber = toupper(ch) - 'A';
            charFreqs[indexNumber]++;
        }
    }

    for(ch = 'A'; ch <= 'Z'; ch++ ) {
        cout << ch << ": " << charFreqs[ch - 'A'] << '\n';
    }

}

int main() {
    string fileyname = "ciphertext.txt";
    readDaFile(fileyname);
    char check;

    cin >> check;

    return 0;
}
