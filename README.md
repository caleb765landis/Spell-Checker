# Spell Checker
## A simple spell checker program demonstrating hashing.

To run the Spell_Checker program, use "make run".

Once the program starts, the terminal will be cleared, and users will be asked to input a word.

If the word is found in the dictionary, the program will return "True" as well as all words with the same first two letters as the inputted word.

If the word is not found, the program will suggest a word based on a matching-score.

If the suggestion was what the user meant, the program will return "true" as well as the rest of the suggested words again.

Otherwise, the program will return false if there are no valid suggestions or the best suggestion was not what the user meant.
