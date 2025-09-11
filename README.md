# What even is this?

This is a little program to help you attack a substitution cipher. It works with any cipher text provided, just edit the ```ciphertext.txt``` file.

## Wait, why do we even need this? Can't I just CTRL+F my way to victory?

You could! That's what I tried ;)

But you might run into the problem of accidentally remapping letters you've already decrypted. For example:

* map 'w' onto 'i' ✅
* 'map 'i' onto 's' 💥 oops! i just lost my mappings from 'w' onto 'i'!

This program keeps track of letter states of characters, so this unfortunate problem doesn't happen to you :)

## Usage

Make sure you sort the letter frequencies of cipher text. The program will automatically do this for you and save it to a file called 'cipherFrequencies-sorted'. Now just try your best to line these up with the expected letter frequencies that occur in English (or any other target language) and you're good to go!

```replace a b``` will replace all occurances of 'a' with 'b'.
```undo``` will undo the most recent step.
```undo a``` will undo all mappings that resulted in 'a'.
```exit``` gets us out of here!
```help``` shows this helpful message :)

Your efforts are automatically saved at each step to a ```decryptMap.txt``` file, as well as a ```ciphertext-decrypted.txt``` file.

GL HF!