# Trabalho Prático: Árvore Geradora Mínima (MST) - UFC

Este projeto consiste em uma plataforma experimental desenvolvida em **C++17** com análise estatística em **Python (Pandas, Matplotlib, Seaborn)** para realizar a análise comparativa de desempenho entre os algoritmos de **Kruskal** e **Prim** na resolução de Árvores Geradoras Mínimas (MST).

O pipeline realiza **1.680 execuções individuais diretamente na memória RAM**:
$$\text{Total Execuções} = 2 \text{ tipos de grafos} \times 7 \text{ tamanhos de vértices} \times 6 \text{ níveis de densidade} \times 10 \text{ repetições para cada cenário}$$

O programa gera métricas de tempo de construção do grafo, tempo de execução dos algoritmos (em milissegundos) e peso total da MST para posterior consolidação estatística.

---

## 📁 Estrutura do Projeto

A organização dos diretórios no repositório é a seguinte:

*   **[`heads/`](heads/)**: Módulos de cabeçalhos e lógica C++.
    *   [`BinaryHeap.h`](heads/BinaryHeap.h): Min-Heap binário customizado utilizado pelo Prim.
    *   [`UnionFind.h`](heads/UnionFind.h): Estrutura Disjoint-Set com compressão de caminhos para o Kruskal.
    *   [`Kruskal.h`](heads/Kruskal.h): Implementação do algoritmo de Kruskal $O(E \log V)$.
    *   [`Prim.h`](heads/Prim.h): Implementação do algoritmo de Prim com Min-Heap.
    *   [`RandomGraphGenerator.h`](heads/RandomGraphGenerator.h): Gerador de grafos aleatórios Normais e Geométricos.
    *   [`Record.h`](heads/Record.h): Estrutura para armazenamento das métricas coletadas.
    *   [`DataExporter.h`](heads/DataExporter.h): Módulo de exportação de registros em CSV.
    *   [`Timer.h`](heads/Timer.h): Medidor de tempo de execução baseado em `std::chrono`.
*   **[`sources/`](sources/)**: Contém o ponto de entrada principal do pipeline [`main.cpp`](sources/main.cpp).
*   **[`tests/`](tests/)**: Testes unitários para validação de componentes específicos do sistema.
*   **[`resultados/`](resultados/)**: Pasta que centraliza os relatórios CSV gerados e os gráficos resultantes.
*   **[`builds/`](builds/)**: Diretório destinado a armazenar os binários gerados na compilação.
*   **[`analise_experimento.py`](analise_experimento.py)**: Script Python para gerar os relatórios gráficos consolidados.

---

## 🛠️ Engenharia de Otimizações

1.  **Parada Precoce no Prim/Kruskal:** Estrutura projetada para otimizar as buscas no Heap e na estrutura Union-Find, evitando processamentos desnecessários ao atingir exatamente $V - 1$ arestas na árvore geradora.
2.  **Otimização de RAM no Gerador Geométrico:** Geração cartesiana mantendo coordenadas de vértices em limites compactos e gerando pesos em precisão `uint8_t` para evitar sobrecarga de cache.
3.  **Gerenciamento Dinâmico de Escopo:** Liberação explícita de estruturas internas do grafo a cada iteração para evitar vazamento ou picos de memória física durante testes com $V = 10.000$.

---

## 🚀 Como Executar o Projeto

### 1. Compilação do Programa Principal
Compile o código do pipeline a partir do diretório raiz executando o comando abaixo:
```powershell
g++ -Wall -O3 sources/main.cpp -o builds/main
```

### 2. Execução do Experimento
Execute o pipeline para rodar a bateria de testes e coletar os tempos:
```powershell
# No Windows:
.\builds\main.exe

# No Linux/macOS:
./builds/main
```
> [!NOTE]  
> A execução gera um arquivo dinâmico no diretório de resultados contendo a marcação do instante de execução (ex: `resultados/records_1782025424061215000.csv`).

### 3. Geração de Gráficos de Desempenho
Após a conclusão do pipeline C++, execute a análise visual com o Python:
```bash
python analise_experimento.py
```
> [!TIP]  
> O script Python detecta automaticamente o arquivo de métricas mais recente (`records_*.csv`) na pasta `resultados/` e mapeia seus dados brutos de forma transparente.

Os gráficos unificados comparativos serão gravados em:
*   `resultados/grafo_normal.png`
*   `resultados/grafo_geometrico.png`

---

## 🧪 Executando Testes Unitários

Para validar componentes individuais do projeto de forma isolada, você pode compilar e rodar os testes da pasta `tests/` utilizando os comandos listados a seguir:

### 1. Disjoint Set / Union-Find
* **Compilação e Execução**:
  ```powershell
  g++ -Wall -O3 heads/UnionFind.h tests/test_unionfind.cpp -o builds/test_union_find
  .\builds\test_union_find
  ```
* **Referência**: [Introduction to Disjoint Set (Union-Find Data Structure)](https://www.geeksforgeeks.org/dsa/introduction-to-disjoint-set-data-structure-or-union-find-algorithm/)

### 2. Binary Heap
* **Compilação e Execução**:
  ```powershell
  g++ -Wall -O3 tests/test_binaryheap.cpp -o builds/test_binary_heap
  .\builds\test_binary_heap
  ```
* **Referência**: [C++ Program to Implement Binary Heap](https://www.geeksforgeeks.org/cpp/cpp-program-to-implement-binary-heap/)

### 3. Algoritmo de Kruskal
* **Compilação e Execução**:
  ```powershell
  g++ -Wall -O3 tests/test_kruskal.cpp -o builds/test_kruskal
  .\builds\test_kruskal
  ```
* **Referência**: [Kruskal's Minimum Spanning Tree (MST) Algorithm](https://www.geeksforgeeks.org/dsa/kruskals-minimum-spanning-tree-algorithm-greedy-algo-2/)

### 4. Algoritmo de Prim
* **Compilação e Execução**:
  ```powershell
  g++ -Wall -O3 tests/test_prim.cpp -o builds/test_prim
  .\builds\test_prim
  ```
* **Referência**: [Prim's Algorithm for Minimum Spanning Tree (MST)](https://www.geeksforgeeks.org/dsa/prims-minimum-spanning-tree-mst-greedy-algo-5/)

### 5. Gerador de Grafos (Random Graph Generator)
* **Compilação e Execução**:
  ```powershell
  g++ -Wall -O3 tests/test_generator.cpp -o builds/test_generator
  .\builds\test_generator
  ```

### 6. Cronometragem (Timer)
* **Compilação e Execução**:
  ```powershell
  g++ -Wall -O3 tests/test_timer.cpp -o builds/test_timer
  .\builds\test_timer
  ```
