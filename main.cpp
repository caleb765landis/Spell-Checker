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

// prototypes
int hashFunc(string word, int charVal);
void search(vector<node*> dictionary, string word);
bool suggest(std::vector<string> similarWords, string word);
int matchingScore (string suggestion, string word);

int main() {
  // clear screen to start the program
  system("clear");

  // create dictionary
  vector<node*> dictionary(26, NULL);

  std::ifstream inFile;
  inFile.open("Dictionary.txt");

  // build dictionary from "Dictionary.txt"
  string currentWord;
  while (getline(inFile, currentWord)) {
    // erases whitespaces before inserting into hash table
    currentWord.erase(currentWord.find_last_not_of(" \n\r\t\f\v") + 1);

    // hashKey is askii value of first character in word
    // (- 97 to make it easier to check which letter it is)
    int hashKey = tolower(currentWord[0]) - 97;
    int hashVal = hashFunc(currentWord, hashKey);

    node* newNode = new node();
    newNode -> key = currentWord;

    // insert word node into dictionary at location where first letter hashes to
    // if current hash spot is null, insert node here
    if (dictionary[hashVal] == NULL) {
      dictionary[hashVal] = newNode;

    // otherwise go through chain until node can be inserted at end
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

  // get word and search dictionary for it
  std::cout << "Please give a word: ";
  string word;
  std::cin >> word;
  search(dictionary, word);
} // end main

// hash function to return hash value therefore location in dictionary
int hashFunc(string word, int charVal) {
  int hashVal = charVal;

  // using Horner's rule
  hashVal = 37 * hashVal + tolower(word[0]);

  return hashVal % 26;
} // end hashFunc

// search array for word
void search(vector<node*> dictionary, string word) {
  // similar words have same first and second characters
  // used for suggestions
  std::vector<string> similarWords;
  bool found = false;

  // start time to search for suggestions
  TimeInterval time = TimeInterval();
  time.start();

  // start with first node at this location
  int hashKey = tolower(word[0]) - 97;
  int hashVal = hashFunc(word, hashKey);
  node* currentDictNode = dictionary[hashVal];

  // search for word
  bool keepGoing = true;
  while (keepGoing) {
    // if word is found return true
    if (currentDictNode -> key == word) {
      found = true;

      // keep going through chain to get rest of suggestions
      if (currentDictNode -> next != NULL) {
        currentDictNode = currentDictNode -> next;
      } else {
        keepGoing = false;
      } // end if

    // otherwise keep going through chain to get rest of suggestions
    // and add suggestions to similarWords
    } else {
      if (currentDictNode -> next != NULL) {
        currentDictNode = currentDictNode -> next;
      } else {
        keepGoing = false;
      } // end if

      if (word[1] == currentDictNode -> key[1]) {
        similarWords.push_back(currentDictNode -> key);
      } // end if words are similar
    } // end if
  } // end while

  time.stop();

  // if word is found return true and suggestions
  if (found == true) {
    std::cout << "True" << std::endl;
    for (int i = 0; i < similarWords.size(); i++) {
      std::cout << similarWords[i] << std::endl;
    } // end for

  // otherwise search for best suggestion
  } else {
    // no resemblance
    if (similarWords.size() == 0) {
      std::cout << "false" << std::endl;

    } else {
      // if suggestion was correct, return rest of suggestions
      if (suggest(similarWords, word)) {
        for (int i = 0; i < similarWords.size(); i++) {
          std::cout << similarWords[i] << std::endl;
        } // end for
      } // end if suggestion
    } // end if resemblance
  } // end if word is found

  // print time to get all suggestions
  std::cout << time.GetInterval() << " micro-sec" << std::endl;
} // end search

// use matchingScore to get best suggestion
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
  string shortestWord;

  // ratio of word lengths
  int lengthRatio = 1;
  if (word.length() > suggestion.length()) {
    lengthRatio = word.length() / suggestion.length();
    shortestWord = suggestion;
  } else {
    lengthRatio = suggestion.length() / word.length();
    shortestWord = word;
  } // end length if

  // number of matching characters
  int numCharacters = 1;
  for (int i = 0; i < word.size(); i++) {
    numCharacters = numCharacters + std::count(suggestion.begin(), suggestion.end(), word[i]);
  } // end for

  // number of matching positions
  int numPos = 1;
  for (int i = 0; i < shortestWord.size(); i++) {
    if (suggestion[i] == word[i]) {
      numPos++;
    } // end if
  } // end for

  return lengthRatio * numCharacters * numPos;
} // end matchingScore
