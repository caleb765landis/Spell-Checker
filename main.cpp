// main.cpp

#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm> // count()
#include <stdlib.h> // abs()
#include "TimeInterval.h"

using namespace std;

struct node {
  string key;
  node* next = NULL;
};

int hashFunc(string word, int charVal);
void search(vector<node*> dictionary, string word);
bool suggest(std::vector<string> similarWords, string word);
int matchingScore (string suggestion, string word);

int main() {
  std::ifstream inFile;
  string currentWord;
  //vector<string> dictionary(400, "NULL");
  vector<node*> dictionary(26, NULL);

  inFile.open("Dictionary.txt");

  int lineNum = 1;
  while (getline(inFile, currentWord)) {
    currentWord.erase(currentWord.find_last_not_of(" \n\r\t\f\v") + 1);

    // hashKey is askii value of first character in word
    // (- 97 to make it easier to check which letter it is)
    int hashKey = tolower(currentWord[0]) - 97;
    int hashVal = hashFunc(currentWord, hashKey);

    node* newNode = new node();
    newNode -> key = currentWord;

    if (dictionary[hashVal] == NULL) {
      dictionary[hashVal] = newNode;
    } else {
      bool nextEmpty = false;
      node* currentDictNode = dictionary[hashVal];

      while (nextEmpty == false) {
        if (currentDictNode -> next == NULL) {
          currentDictNode -> next = newNode;
          nextEmpty = true;
        } else {
          currentDictNode = currentDictNode -> next;
        } // end if
      } // end while
    } // end if first letter has been hashed
  } // end while getline

  inFile.close();

  // debugging
  /*
  inFile.open("Dictionary.txt");
  while (getline(inFile, currentWord)) {
    string found = "false";
    currentWord.erase(currentWord.find_last_not_of(" \n\r\t\f\v") + 1);

    if (search(dictionary, currentWord) == true) {
      found = "true";
    }
    cout << lineNum << " " << currentWord << endl;
    cout << found << endl << endl;
    lineNum++;
  }
  inFile.close();
  */

  std::cout << "Please give a word: ";
  string word;
  std::cin >> word;

  search(dictionary, word);
} // end main


int hashFunc(string word, int charVal) {
  int hashVal = charVal;
  int wordLength = word.length();

  //for (int i = 1; i < wordLength; i++) {
    // using Horner's rule
    //hashVal = abs(37 * hashVal + tolower(word[0]));
    //hashVal = abs(37 * hashVal);
    //hashVal = abs(37 * hashVal + tolower(word[1]));
    hashVal = abs(hashVal);
    //cout << hashVal % 26 << endl;

  //} // end for
  return hashVal % 26;
} // end hashFunc

void search(vector<node*> dictionary, string word) {
  bool found = false;
  std::vector<string> similarWords;
  TimeInterval time = TimeInterval();
  time.start();

  int hashKey = tolower(word[0]) - 97;
  int hashVal = hashFunc(word, hashKey);
  node* currentDictNode = dictionary[hashVal];

  bool keepGoing = true;
  while (keepGoing) {
    if (currentDictNode -> key == word) {
      found = true;

      if (currentDictNode -> next != NULL) {
        currentDictNode = currentDictNode -> next;
      } else {
        keepGoing = false;
      }

    } else {
      if (currentDictNode -> next != NULL) {
        currentDictNode = currentDictNode -> next;
      } else {
        keepGoing = false;
      }

      if (word[1] == currentDictNode -> key[1]) {
        similarWords.push_back(currentDictNode -> key);
      } // end if words are similar
    } // end if
  } // end while

  time.stop();

  if (found == true) {
    std::cout << "True" << std::endl;
    for (int i = 0; i < similarWords.size(); i++) {
      std::cout << similarWords[i] << std::endl;
    } // end for
  } else {
    if (similarWords.size() == 0) {
      std::cout << "false" << std::endl;
    } else {
      if (suggest(similarWords, word)) {
        for (int i = 0; i < similarWords.size(); i++) {
          std::cout << similarWords[i] << std::endl;
        } // end for
      }
    }
  }

  std::cout << time.GetInterval() << " micro-sec" << std::endl;
} // end search

bool suggest(std::vector<string> similarWords, string word) {
  bool validSuggestion = false;
  string bestSuggestion;
  int bestScore = 0;

  for (int i = 0; i < similarWords.size(); i++) {
    int currentScore = matchingScore(similarWords[i], word);
    if (currentScore > bestScore) {
      bestScore = currentScore;
      bestSuggestion = similarWords[i];
    } // end if
  } // end for

  bool keepGoing = true;
  while (keepGoing) {
    std::cout << "Did you mean " << bestSuggestion << "? (y or n) ";
    string input;
    std::cin >> input;

    if (input == "y") {
      std::cout << "true" << std::endl;
      validSuggestion = true;
      keepGoing = false;
    } else if (input == "n") {
      std::cout << "false" << std::endl;
      keepGoing = false;
    } else {
      std::cout << "Please enter a valid response." << std::endl;
    } // end if
  } // end while

  return validSuggestion;
} // end suggest

int matchingScore (string suggestion, string word) {
  int lengthRatio = 1;
  string shortestWord;
  if (word.length() > suggestion.length()) {
    lengthRatio = word.length() / suggestion.length();
    shortestWord = suggestion;
  } else {
    lengthRatio = suggestion.length() / word.length();
    shortestWord = word;
  } // end length if

  int numCharacters = 1;
  for (int i = 0; i < word.size(); i++) {
    numCharacters = numCharacters + std::count(suggestion.begin(), suggestion.end(), word[i]);
  }

  int numPos = 1;
  for (int i = 0; i < shortestWord.size(); i++) {
    if (suggestion[i] == word[i]) {
      numPos++;
    }
  }

  return lengthRatio * numCharacters * numPos;
}





















// space
