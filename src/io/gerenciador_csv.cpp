#include "gerenciador_csv.h"
#include <fstream>

namespace IO {
    std::string GerenciadorCSV::arquivo_destino = "";

    void GerenciadorCSV::inicializar(const std::string& caminho_arquivo) {
        arquivo_destino = caminho_arquivo;
        std::ofstream arquivo(arquivo_destino);
        arquivo << "TipoGrafo,Vertices,DensidadePercentual,Repeticao,Algoritmo,TempoMilissegundos,PesoTotalMST\n";
        arquivo.close();
    }

    void GerenciadorCSV::salvarLinha(const std::string& tipo_grafo, int vertices, int densidade, int repeticao,
                                     const std::string& algoritmo, double tempo_ms, double peso_total) {
        std::ofstream arquivo(arquivo_destino, std::ios::app); 
        if (arquivo.is_open()) {
            arquivo << tipo_grafo << "," << vertices << "," << densidade << "," << repeticao << "," << algoritmo << "," << tempo_ms << "," << peso_total << "\n";
            arquivo.close();
        }
    }
}