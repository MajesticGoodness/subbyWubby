/* Cute class to make coupling letters with their frequencies a little easier on my tiny wittle brain uwu */

#ifndef LEDDER_H
#define LEDDER_H

class Ledder {

public:
    Ledder();
    Ledder(char ch);

    // just a few setters to make modifying the structure outside of the class easy
    void setChar(char ch);
    void setFreq(int freq);
    void setRelFreq(float relFreq);
    void mark(); // we'll call this method to flip alreadySwapped_ to TRUE on the character, letting us know we shouldn't swap it if we encounter it again
    void unmark(); // if we ever need to undo a substitution, we need to make sure that letter's alreadySwapped_ field is set back to FALSE
    void setDontShow(); // sets dontShow_ to TRUE
    void unsetDontShow(); // sets dontShow_ back to FALSE

    bool swapped(); // returns the state of the letter--has it already been swapped?
    bool dontShow(); // should we show this letter as an option for target/replacement?
    char getChar(); // returns the character for us; alternatively we could just overload the comparison operators...i might do that actually.
    int getFreq(); // returns the frequency counter of our letter
    float getRelativeFreq(); // returns the relative freq for us
    float calcRelativeFreq(int totalChars); // calculates the relative freq of the letter, given the total number of characters of the text it appears in

    // overloaded operators to make our life a little easier

    Ledder& operator++(); // just the prefix increment is totally fine
    friend bool operator==(Ledder letter1, Ledder letter2); // useful when we iterate over characters to replace them. assumes both letters being compared are the same case, so be careful please
    friend bool operator<(Ledder letter1, Ledder letter2); // helpful comparator for sorting purposes!


private:
    char cuteChar_{}; // the character itself will be stored here!
    int cuteFreq_{}; // the number of times this character appears in the file of interest is a lot easier to count as an integer
    float relativeFreq_{}; // once we have the frequency counted, we can divide it by the total number of characters to get a percentage
    bool alreadySwapped_{}; // during our decryption process, we'd like to take care not to decrypt stuff we've already decrypted! don't ask me how i know :(
    bool dontShow_{}; // when we're presenting options to the console, we want to make sure to only present those which haven't been used yet. we definitely could've used alreadySwapped_ for this but i wanted to be explicit
};

bool operator==(Ledder letter1, Ledder letter2);
bool operator<(Ledder letter1, Ledder letter2);

#endif // LEDDER_H
