# Trabalho Prático: Árvore Geradora Mínima (MST) - UFC

Este projeto consiste em uma plataforma experimental de alta performance desenvolvida em C++17 para a análise comparativa de desempenho entre os algoritmos de Kruskal e Prim na resolução de Árvores Geradoras Mínimas (MST).

O pipeline realiza 1680 execuções individuais na memória RAM (2 tipos de grafos × 7 tamanhos de vértices × 6 níveis de densidade × 10 repetições para cada cenário), gerando métricas de tempo e peso da MST para análise estatística.

---

## 📁 Estrutura do Projeto

*   `src/core/`: Estruturas fundamentais (`UnionFind`, `Aresta`) e lógica dos algoritmos de Kruskal e Prim.
*   `src/generators/`: Geradores de grafos (Normal e Geométrico) otimizados diretamente na RAM.
*   `src/io/`: Módulo de exportação de métricas para CSV.
*   `src/main.cpp`: Orquestrador linear epipeline experimental.
*   `resultados/`: Pasta que centraliza o CSV de saída e os gráficos gerados.
*   `analise_experimento.py`: Script Python para consolidação estatística e plotagem de gráficos lado a lado.
*   `Makefile`: Script de compilação C++.

---

## 🛠️ Engenharia de Alta Performance e Otimizações

1.  **Parada Precoce no Prim:** O laço do algoritmo de Prim é interrompido imediatamente ao atingir exatamente $V - 1$ arestas na MST, evitando o processamento desnecessário de elementos remanescentes no Heap em grafos muito densos.
2.  **Otimização Geométrica de RAM:** As coordenadas cartesianas geométricas são geradas usando inteiros de 8 bits (`uint8_t`), economizando memória física de armazenamento temporário durante a geração. As distâncias euclidianas reais são computadas sob demanda na precisão `double`.
3.  **Pré-Alocação de Vetores (`reserve`):** Todas as inserções nos vetores de arestas e adjacências são precedidas de alocação de capacidade física, mitigando o custo de cópia por redimensionamento em execuções de grande porte ($V = 10.000$).
4.  **Gerenciamento Dinâmico de Escopo:** Vetores temporários de grafos são alocados no escopo interno do laço de repetições e desalocados automaticamente, prevenindo o pico de retenção de memória RAM (~2.4 GB).
5.  **Qualidade Aleatória:** Migração completa da antiga biblioteca `rand()` para o gerador pseudo-aleatório `std::mt19937` (Mersenne Twister) da biblioteca `<random>` do C++11.

---

## 🚀 Como Executar

### 1. Compilação do Projeto C++
Compile o código C++ com otimização máxima (`-O3`) executando o seguinte comando no terminal:
```powershell
g++ -Wall -Wextra -O3 -std=c++17 -I./src src/main.cpp src/core/estruturas.cpp src/core/algoritmos.cpp src/generators/gerador.cpp src/io/gerenciador_csv.cpp -o pipeline_mst
```

*Alternativa automatizada (caso possua a ferramenta `make` instalada):*
```powershell
make
```

### 2. Execução do Pipeline
Rode a execução do pipeline de testes para gerar as métricas de tempo em milissegundos e peso total da árvore:
```powershell
.\pipeline_mst.exe
```
Os dados serão salvos no arquivo `resultados/resultados_pipeline.csv`.

### 3. Geração de Gráficos e Análise Visual
Após o encerramento do pipeline, rode a análise estatística para gerar os painéis unificados com escala logarítmica:
```powershell
python analise_experimento.py
```
As seguintes imagens serão gravadas no diretório de saídas:
*   `resultados/grafo_normal.png`
*   `resultados/grafo_geometrico.png`
