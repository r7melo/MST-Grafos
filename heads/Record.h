#ifndef RECORD_H
#define RECORD_H

#include <string>

using namespace std;

struct Record {
    string nameRecord;
    double constructionTime;
    double executionTime;
    int weightMST;

    // Retorna a linha formatada para o arquivo CSV
    string toCSVLine() const {
        return nameRecord + "," + 
               to_string(constructionTime) + "," + 
               to_string(executionTime) + "," + 
               to_string(weightMST);
    }
};

#endif // RECORD_H