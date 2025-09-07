#ifndef REPLACEMENTPAIR_H
#define REPLACEMENTPAIR_H

// a simple structure to keep track of the substitutions we've done

class replacementpair
{
public:
    replacementpair();
    replacementpair(char targetChar, char replacedChar);

    void setTargetChar(char targetChar);
    void setReplacedChar(char replacedChar);

    char getTargetChar();
    char getReplacedChar();

private:
    char targetChar_{};
    char replacedChar_{};
};

#endif // REPLACEMENTPAIR_H
