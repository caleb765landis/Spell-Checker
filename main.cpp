// main.cpp

#include <vector>
#include <fstream>
#include <iostream>
using namespace std;

int hashFunc(string word, int charVal);

int main() {
  std::ifstream inFile;
  std::string currentWord;

  inFile.open("Dictionary.txt");

  int lineNum = 1;

  while (getline(inFile, currentWord)) {
    string currentWordCopy = currentWord;
    int firstCharVal = tolower(currentWordCopy[0]) - 97;
    int hashVal = hashFunc(currentWord, firstCharVal);

    cout << lineNum << " " << currentWord << endl;
    cout << firstCharVal << " " << hashVal << endl << endl;
    lineNum++;
  } // end while

  inFile.close();
} // end main

int hashFunc(string word, int charVal) {
  int hashVal = charVal;
  int wordLength = word.length();
  for (int i = 0; i < wordLength; i++) {
    // using Horner's rule
    hashVal = abs(37 * hashVal + tolower(word[i]));
  } // end for
  return hashVal % 26;
} // end hashFunc
