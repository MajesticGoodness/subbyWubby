#include "replacementpair.h"

replacementpair::replacementpair() {}

replacementpair::replacementpair(char targetChar, char replacedChar) {
    targetChar_ = targetChar;
    replacedChar_ = replacedChar;
}

void replacementpair::setTargetChar(char targetChar) {
    targetChar_ = targetChar;
}

void replacementpair::setReplacedChar(char replacedChar) {
    replacedChar_ = replacedChar;
}

char replacementpair::getTargetChar() {
    return targetChar_;
}

char replacementpair::getReplacedChar() {
    return replacedChar_;
}
