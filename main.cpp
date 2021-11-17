// main.cpp

#include <fstream>
#include <iostream>
using namespace std;

int main() {
  std::ifstream inFile;
  std::string currentLine;

  inFile.open("Dictionary.txt");

  int lineNum = 1;
  while (getline(inFile, currentLine)) {
    cout << lineNum << " " << currentLine << endl;
    lineNum++;
  } // end while

  inFile.close();
} // end main
