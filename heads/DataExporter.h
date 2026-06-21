#ifndef DATA_EXPORTER_H
#define DATA_EXPORTER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Record.h"

using namespace std;

class DataExporter {
public:
    static void appendToCSV(const string& fullPath, const vector<Record>& records) {

        // Abre em modo de escrita sem sobrescrita
        ofstream file(fullPath, ios::app);
        
        if (!file.is_open()) {
            cerr << "[Error I/O]:" << fullPath << endl;
            return;
        }

        // Inicia o cabecalho
        file.seekp(0, ios::end);
        if (file.tellp() == 0) {
            file << "algorithm,constructionTime,executionTime,weightMST\n";
        }

        // Escreve os registros
        for (const auto& r : records) {
            file << r.toCSVLine() << "\n";
        }

        file.close();
    }
};

#endif // DATA_EXPORTER_H