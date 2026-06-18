#ifndef GERENCIADOR_CSV_H
#define GERENCIADOR_CSV_H

#include <string>

namespace IO {
    class GerenciadorCSV {
    public:
        static void inicializar(const std::string& caminho_arquivo);
        
        static void salvarLinha(const std::string& tipo_grafo, int vertices, int densidade, int repeticao,
                                const std::string& algoritmo, double tempo_ms, double peso_total);
                                
    private:
        static std::string arquivo_destino;
    };
}
#endif