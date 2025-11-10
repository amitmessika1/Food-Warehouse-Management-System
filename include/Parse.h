#pragma once
#include <string>
#include <vector>
#include "Customer.h"
#include "Volunteer.h"
using namespace std;

class Parse {
  
    public:
        Parse(const string &configFilePath);  
        string getLine(int lineNumber); 
        int getNumOfLines();  
        ~Parse() = default; //destructor
    
    private:
        string m_configFilePath;
        int m_numOfLines; 
};


