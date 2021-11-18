// main.cpp

#include <vector>
#include <fstream>
#include <iostream>
#include <stdlib.h> // abs()
using namespace std;

int hashFunc(string word, int charVal);
bool search(vector<string> dictionary, string word);

int main() {
  std::ifstream inFile;
  string currentWord;
  vector<string> dictionary(200, "NULL");

  inFile.open("Dictionary.txt");

  int lineNum = 1;

  //getline(inFile, currentWord);
  while (getline(inFile, currentWord)) {
    // hashKey is askii value of first character in word
    // (- 97 to make it easier to check which letter it is)
    int hashKey = tolower(currentWord[0]) - 97;

    bool keepGoing = true;
    while (keepGoing) {
      int hashVal = hashFunc(currentWord, hashKey);

      if (dictionary[hashVal] == "NULL") {
        dictionary[hashVal] = currentWord;
        keepGoing = false;
      } else {
        hashKey++;
      } // end if
    } // end while keepGoing
  } // end while getline

  inFile.close();
} // end main


int hashFunc(string word, int charVal) {
  int hashVal = charVal;
  int wordLength = word.length();

  for (int i = 1; i < wordLength; i++) {
    // using Horner's rule
    hashVal = abs(37 * hashVal + tolower(word[i]));
  } // end for
  return hashVal % 200;
} // end hashFunc

bool search(vector<string> dictionary, string word) {
  bool found = false;
  bool keepGoing = true;
  int hashKey = tolower(word[0]) - 97;

  while (keepGoing) {
    int hashVal = hashFunc(word, hashKey);

    if (dictionary[hashVal] == word) {
      found = true;
      keepGoing = false;
    } else if (dictionary[hashVal] == "NULL") {
      keepGoing = false;
    } else {
      hashKey++;
    } // end if
  } // end while

  return found;
} // end search





















// space
