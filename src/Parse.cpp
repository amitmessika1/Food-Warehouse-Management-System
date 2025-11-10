#include "Parse.h"
#include <fstream>
#include <iostream>

Parse :: Parse(const string &configFilePath) : m_configFilePath(configFilePath), m_numOfLines(0){
    std::ifstream file(m_configFilePath);
    int lineCount = 0;
    std::string line;
   
    // Count lines in the file
    while (std::getline(file, line)) {
        lineCount++;
    }    
    m_numOfLines = lineCount;

};

string Parse :: getLine(int lineNumber){
    std::ifstream configFile(m_configFilePath); 
    std::string line;
    
    // Move to the desired line number
    for (int i = 1; i < lineNumber; ++i) {
        std::getline(configFile, line);
    }
    
    // Read and return the line
    std::getline(configFile, line);
    return line;
}
   
int Parse :: getNumOfLines(){
    return m_numOfLines;
}
