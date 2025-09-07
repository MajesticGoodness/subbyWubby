#include <algorithm>
#include <iostream>
#include <fstream> 
#include <vector>
#include "ledder.h"
#include <sstream>
#include "console.h"
#include "replacementpair.h"

using namespace std;

void readDaFile(string fileName, vector<Ledder> &letterFreq);
void replaceRoutine(char targetLetter, char replacementLetter, vector<Ledder> &cipherText);
void loadCipherText(string &cipherTextFileName, vector<Ledder> &cipherText);
void printToConsole(vector<Ledder> &text);
void undoStep(replacementpair pair, vector<Ledder> &cipherText);
void gitInput(string &command, vector<char> &parameters);
int handleInput(string &command, vector<char> &parameters);
void getParameters(istringstream &line, vector<char> &parameters);
void getCipherFreqs(string &fileName, vector<Ledder> &cipherTextFreqs);
void sortEngLetterFreqs(string &fileName, vector<Ledder> &englishLettersFreqs);
void saveReplacementMap(string &fileName, vector<replacementpair> &replacementPairs);
void saveCipherText(string &fileName, vector<Ledder> &text);
void presentReplacementOptions(vector<Ledder> &letterFreqs);
void insertPair(vector<Ledder> letterFreqs);
void updateReplacementOptions(replacementpair pair, vector<Ledder> &letterFreqs);
void welcomeMsg();
void helpMsg();

void welcomeMsg() {
    cout << "This wittle tool helps decrypt a substitution cipher via a letter frequency attack." << '\n'
         << "A \'ciphertext.txt\' and \'english_letterFrequencies.csv\' is required"  << '\n'
         << "Your efforts are automatically saved at each step. Have fun! :)" << "\n\n";
}

void helpMsg() {
    cout << "Usage:" << '\n'
         << "\'replace a b\' replaces all instances of a in the ciphertext with b" << '\n'
         << "\'undo\' will undo the most recent replacement" << '\n'
         << "\'undo b\' will undo all of the b's that resulted from a prior replacement" << '\n'
         << "\'exit\' will terminate the program" << '\n'
         << "\'help\' to see this message again!" << "\n\n";
}

void updateReplacementOptions(replacementpair pair, vector<Ledder> &letterFreqs) {

    for (Ledder &currentLetter : letterFreqs) {
        if (pair.getTargetChar() == currentLetter.getChar()) {
            currentLetter.setDontShow();
        }
    }
}

void presentReplacementOptions(vector<Ledder> &letterFreqs) {

    for(Ledder currentLetter : letterFreqs) {
        if(!currentLetter.dontShow()) {
            cout << currentLetter.getChar() << " ";
        }
    }
}

void saveCipherText(string &fileName, vector<Ledder> &text) {
    ofstream outfile(fileName);

    if(outfile) {
        for(Ledder currentLetter : text) {
            outfile << currentLetter.getChar();
        }
    }
}

void saveReplacementMap(string &fileName, vector<replacementpair> &replacementPairs) {
    ofstream outfile(fileName);

    if(outfile) {
        for(replacementpair pair : replacementPairs) {
            outfile << pair.getTargetChar() << " --> " << pair.getReplacedChar() << '\n';
        }
    }

    else {
        cout << "Failed to save replacement map :( check read/write privileges!" << '\n';
    }
}

void printToConsole(vector<Ledder> &text) {

    cout << "\n";

    for (Ledder currentLetter : text) {
        cout << currentLetter.getChar();
    }

    cout << "\n\n";
}

void replaceRoutine(char targetLetter, char replacementLetter, vector<Ledder> &cipherText) {

    for (Ledder &currentLetter : cipherText) {

        if(currentLetter.getChar() == targetLetter && !currentLetter.swapped()) {
            currentLetter.setChar(replacementLetter);
            currentLetter.mark();
        }
    }

}

void loadCipherText(string &cipherTextFileName, vector<Ledder> &cipherText) {
    ifstream infile(cipherTextFileName);

    if (infile) {
        cout << "loading cipher text..." << '\n';

        char ch{};
        while (infile.get(ch)) {
            Ledder newLetter{ch};
            cipherText.push_back(newLetter);
        }

    }

    else {
        ostringstream errorMsg;
        errorMsg << "unable to load " << cipherTextFileName << ", check current directory and read/write permissions";
        throw runtime_error(errorMsg.str());
    }

}

void getCipherFreqs(string &fileName, vector<Ledder> &letterFreq) {
    ifstream infile(fileName);
    ofstream outfile("cipherFrequencies-sorted.txt");
    string alphabet{"abcdefghijkmnlopqrstuvwxyz"};
    int totalCharCount{};
    char ch{};

    // build a vector to hold our letter frequencies present in the cipher text

    for (int i = 0; i < 26; i++) {
        Ledder letter(alphabet[i]);
        letterFreq.push_back(letter);
    }

    if (infile && outfile) {
        cout << "computing cipher frequencies..." << '\n';

        while (infile.get(ch)) {
            // just iterating over our vector until we find the one that matches the one we just read from the file.
            for(Ledder &currentLetter : letterFreq) {
                if (currentLetter.getChar() == ch) {
                    ++currentLetter; // once we do, we increment its frequency counter
                    totalCharCount++; // keep track of the total char count so we can compute relative frequencies later
                }
            }
        }

        stable_sort(letterFreq.begin(), letterFreq.end());

        for(Ledder currentLetter : letterFreq) {
            float relFreq{};
            relFreq = currentLetter.calcRelativeFreq(totalCharCount);
            outfile << currentLetter.getChar() << ": " << relFreq << '\n';
        }

    }

    else {
        ostringstream errorMsg;
        errorMsg << "unable to read/write :( check permissions and that " << fileName << " exists!";
        throw runtime_error(errorMsg.str());
    }
}

void sortEngLetterFreqs(string &fileName, vector<Ledder> &englishLettersFreqs) {
    ifstream infile(fileName);
    ofstream outfile("englishFrequencies-sorted.txt");

    if (infile && outfile) {
        cout << "sorting english letter frequencies..." << '\n';

        char separatorComma{}; // used to read in the comma seperating data fields in the csv file
        char englishLetter{}; // stores the english letter we just read
        float charFreq{}; // stores the relative frequency of the letter
        string lineBuffer{}; // we'll be using this string to read one line at a time, then pass it into a istringstream to be able to use the '>>' operator

        getline(infile, lineBuffer); // read in the header of the csv file

        while (getline(infile, lineBuffer)) {
            istringstream tempBuffer(lineBuffer); // we're initializing a stringstream to be able to use the extraction operator to read stuff, because it automatically does conversions from strings into the correct data type for us :)
            tempBuffer >> englishLetter >> separatorComma >> charFreq;

            Ledder newLetter(englishLetter);
            newLetter.setRelFreq(charFreq);
            englishLettersFreqs.push_back(newLetter);
        }

        stable_sort(englishLettersFreqs.begin(), englishLettersFreqs.end());

        for (Ledder currentLetter : englishLettersFreqs) {
            outfile << currentLetter.getChar() << ": " << currentLetter.getRelativeFreq() << '\n';
        }

    }

    else {
        ostringstream errorMsg;
        errorMsg << "wasn't able to load " << fileName << "check that it exists and that you have read/write permissions";
        throw runtime_error(errorMsg.str());
    }

}

void readDaFile(string fileName, vector<Ledder> &letterFreq) {
    ifstream infile(fileName);
    ofstream cute("cipherFrequencies-sorted.txt");
    ofstream cute2("englishFrequencies-sorted.txt");
    int totalCharCount{};

    if (infile) {
        cout << "It's not fudged! :)" << '\n';
    }

    else {
        cout << "It's fudged." << '\n';
    }

    char ch{};
    while (infile.get(ch)) {
        // just iterating over our vector until we find the one that matches the one we just read from the file.
        for(Ledder &currentLetter : letterFreq) {
            if (currentLetter.getChar() == ch) {
                ++currentLetter; // once we do, we increment its frequency counter
                totalCharCount++; // keep track of the total char count so we can compute relative frequencies later
            }
        }
    }

    // sort our vector, then output our results to a file

    stable_sort(letterFreq.begin(), letterFreq.end());

    for(Ledder currentLetter : letterFreq) {
        float relFreq{};
        relFreq = currentLetter.calcRelativeFreq(totalCharCount);
        cute << currentLetter.getChar() << ": " << relFreq << '\n';
    }

    ifstream englishLetterFreqFile("english_letterFrequencies.csv");

    vector<Ledder> englishLettersFreqs{};

    if(englishLetterFreqFile) {
        cout << "we gucci! :)";
        string tempCharHolder{};
        string tempFreqHolder{};
        char separatorComma{}; // used to read in the comma seperating data fields in the csv file
        char englishLetter{};
        float charFreq{};
        string lineBuffer{};

        getline(englishLetterFreqFile, lineBuffer); // read in the header

        while (getline(englishLetterFreqFile, lineBuffer)) {
            istringstream tempBuffer(lineBuffer); // we're initializing a stringstream to be able to use the extraction operator to read stuff, because it automatically does conversions from strings into the correct data type for us :)
            tempBuffer >> englishLetter >> separatorComma >> charFreq;

            Ledder newLetter(englishLetter);
            newLetter.setRelFreq(charFreq);
            englishLettersFreqs.push_back(newLetter);
        }

        stable_sort(englishLettersFreqs.begin(), englishLettersFreqs.end());

        for (Ledder currentLetter : englishLettersFreqs) {
            cute2 << currentLetter.getChar() << ": " << currentLetter.getRelativeFreq() << '\n';
        }

    }

    else {
        cout << "we not gucci :(";
    }
}

void undoStep(replacementpair pair, vector<Ledder> &ciphertext) {
    char targetChar = pair.getReplacedChar();
    char replacement = pair.getTargetChar();

    for (Ledder &currentLetter : ciphertext) {
        if (currentLetter.getChar() == targetChar && currentLetter.swapped()) {
            currentLetter.setChar(replacement);
            currentLetter.unmark();
        }
    }
}

void gitInput(string &command, vector<char> &parameters) {
    string inputLine{};
    string line{};
    char inputChar{};

    getline(cin, line);
    istringstream stringy(line);

    while(!stringy.eof()) {
        stringy >> inputChar;
        inputChar = toupper(inputChar);

        if(stringy.eof()) break; // if we've reached the end of our input in our last read, we don't want to add the same character again!

        inputLine += inputChar;

        if(inputLine == "UNDO") {
            command = inputLine;
            getParameters(stringy, parameters);
        }

        if(inputLine == "REPLACE") {
            command = inputLine;
            getParameters(stringy, parameters);
        }

        if(inputLine == "EXIT") {
            command = inputLine;
        }

        if(inputLine == "HELP") {
            command = inputLine;
        }
    }

}

void getParameters(istringstream &line, vector<char> &parameters) {
    char parameter{};

    while (true) {
        line >> parameter;
        if(line.eof()) break; // if there was nothing to read just get us out of here
        if (parameter != '\0') parameters.push_back(parameter); // if a parameter has actually been passed we'd like to save it!
    }
}

int handleInput(string &command, vector<char> &parameters) {
    int result{};

    // default undo, just undo the previous step.
    if(command == "UNDO" && parameters.size() == 0) {
        result = 1;
    }

    // undo a specific letter pair
    if (command == "UNDO" && parameters.size() == 1) {
        result = 2;
    }

    // replace a specific letter pair
    if (command == "REPLACE" && parameters.size() == 2) {
        result = 3;
    }

    // get us out of here :)
    if (command == "EXIT") {
        result = 4;
    }

    // send help pls!
    if (command == "HELP") {
        result = 5;
    }

    return result;
}

int main() {
    string cipherTextFileName = "ciphertext.txt";
    string engLetterFreqFile = "english_letterFrequencies.csv";
    string decryptedCipherFn = "ciphertext-decrypted.txt";
    string savedMapFn = "decryptMap.txt";
    string command{}; // the command entered by the user. i hope it's a valid one! :)
    vector<Ledder> cipherText{}; // will hold the cipher text in a vector so we can easily modify it
    vector <Ledder> cipherTextFreqs{}; // hold the frequencies of the cipher text here, which we will also use to keep track of viable letters to replace
    vector<Ledder> englishLetterFreqs{}; // store the letter frequencies in here, which we will use to keep track of available replacement options :)

    vector<replacementpair> listOfReplacements{};
    bool getUsOutOfHere{false}; // used to terminate the proggy

    getCipherFreqs(cipherTextFileName, cipherTextFreqs);
    sortEngLetterFreqs(engLetterFreqFile, englishLetterFreqs);
    loadCipherText(cipherTextFileName, cipherText);

    cout << "ready for replacement!" << "\n\n";

    welcomeMsg();
    helpMsg();

    while (!getUsOutOfHere) {
        int result{};
        vector<char> parameters{};

        cout << "Cipher Target Letter Options: ";

        presentReplacementOptions(cipherTextFreqs);

        cout << '\n';

        cout << "Replacement Letter Options: ";

        presentReplacementOptions(englishLetterFreqs);

        cout << "\n\n";

        gitInput(command, parameters);
        result = handleInput(command, parameters);

        // just a classic ol' switch statement to handle various different user commands. see handleInput() for details
        switch (result) {
        case 1:
            // undo most recent replacement, then pop it off our vector
            if (listOfReplacements.size() > 0) {
                auto vectorIterator = --listOfReplacements.end();
                replacementpair pair = *vectorIterator;
                replacementpair invertedPair(pair.getReplacedChar(), pair.getTargetChar());
                updateReplacementOptions(pair, cipherTextFreqs);
                updateReplacementOptions(invertedPair, englishLetterFreqs);
                undoStep(pair, cipherText);
                listOfReplacements.pop_back();
                printToConsole(cipherText);
            }
            break;

        case 2:
            // undo a replacement, then delete it from our list of replacements
            for (auto vectorIterator = listOfReplacements.begin() ; vectorIterator != listOfReplacements.end(); vectorIterator++) {
                char replacedLetter = parameters[0];
                replacementpair pair{};
                pair = *vectorIterator;
                if(pair.getReplacedChar() == replacedLetter) {
                    replacementpair invertedPair(pair.getReplacedChar(), pair.getTargetChar());
                    updateReplacementOptions(pair, cipherTextFreqs);
                    updateReplacementOptions(invertedPair, englishLetterFreqs);
                    undoStep(pair, cipherText);
                    listOfReplacements.erase(vectorIterator);
                    printToConsole(cipherText);
                    break;
                }
            }
            break;
        case 3:
            // calls our replacement routine :)
            {
                char targetLetter = parameters[0];
                char replacementLetter = parameters[1];
                replacementpair pair(targetLetter, replacementLetter);
                replacementpair invertedPair(replacementLetter, targetLetter); // use this inverted pair to update the list of available replacement letters
                listOfReplacements.push_back(pair); // keep track of the replacements we've made
                replaceRoutine(targetLetter, replacementLetter, cipherText);
                updateReplacementOptions(pair, cipherTextFreqs); // remove the cipher letter from the list of options
                updateReplacementOptions(invertedPair, englishLetterFreqs); // remove the replacement letter from the list of options
                printToConsole(cipherText);
                break;
            }
        case 4:
            // terminates proggy
            getUsOutOfHere = true;
            break;
        case 5:
            // calls our help message!
            helpMsg();
            break;
        default:
            // what did you DO?!
            cout << "Unexpected input >:(" << '\n'
                 << "Try again :)" << '\n';
            break;
        }

        // save our stuffies automatically after each menu operation
        saveCipherText(decryptedCipherFn, cipherText);
        saveReplacementMap(savedMapFn, listOfReplacements);

    }

    cout << "Thankies for using!" << '\n';

    // char target{'r'};
    // char replacement{'e'};
    // replacementpair newPair(target, replacement);
    // replaceRoutine(target, replacement, cipherText);

    // listOfReplacements.push_back(newPair);
    // printToFile(cipherText);
    // undoStep(newPair, cipherText);
    // printToFile(cipherText);


    // char check;

    // cin >> check;

    // vector<Ledder> letterFreqs{};
    // string alphabet{"abcdefghijkmnlopqrstuvwxyz"};

    // for (int i = 0; i < 26; i++) {
    //     Ledder letter(alphabet[i]);
    //     letterFreqs.push_back(letter);
    // }

    // readDaFile(fileyname, letterFreqs);

    return 0;
}
