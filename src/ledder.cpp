#include "ledder.h"

Ledder::Ledder() {}
Ledder::Ledder(char ch) {
    cuteChar_ = ch; // nothing interesting here, just setting the character within our little structure to the one passed as a parameter to our constructor
}

void Ledder::setChar(char ch) {
    cuteChar_ = ch;
}

void Ledder::setFreq(int freq) {
    cuteFreq_ = freq;
}

void Ledder::setRelFreq(float relFreq) {
    relativeFreq_ = relFreq;
}

void Ledder::mark() {
    alreadySwapped_ = true;
}

void Ledder::unmark() {
    alreadySwapped_ = false;
}

void Ledder::setDontShow() {
    if (dontShow_ == false) dontShow_ = true;
    else dontShow_ = false;
}

void Ledder::unsetDontShow() {
    dontShow_ = false;
}

bool Ledder::dontShow() {
    return dontShow_;
}

bool Ledder::swapped() {
    return alreadySwapped_;
}

char Ledder::getChar() {
    return cuteChar_;
}

int Ledder::getFreq() {
    return cuteFreq_;
}

float Ledder::getRelativeFreq() {
    return relativeFreq_;
}

float Ledder::calcRelativeFreq(int totalChars) {
    relativeFreq_ = float(cuteFreq_) / float(totalChars);
    return relativeFreq_;
}

Ledder& Ledder::operator++(){
    cuteFreq_++;
    return *this;
}

bool operator==(Ledder letter1, Ledder letter2) {
    return (letter1.cuteChar_ == letter2.cuteChar_);
}

bool operator<(Ledder letter1, Ledder letter2) {
    if (letter1.cuteFreq_ == 0 && letter2.cuteFreq_ == 0) {
        return (letter1.relativeFreq_ < letter2.relativeFreq_);
    }
    return (letter1.cuteFreq_ < letter2.cuteFreq_);
}

// just defining few setters here.


