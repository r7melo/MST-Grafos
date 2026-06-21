import os
import glob
import sys
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# ---------------------------------------------------------------------------
# Configurações fixas baseadas no seu experimento
# ---------------------------------------------------------------------------
VERTICES_LIST = [100, 500, 1000, 2500, 5000, 7500, 10000]
DENSITIES_LIST = [0.0, 0.2, 0.4, 0.6, 0.8, 1.0]
REPETICOES = 10 
LINHAS_POR_REPETICAO = 4  # Prim Normal, Kruskal Normal, Prim Geometric, Kruskal Geometric

PASTA_RESULTADOS = "resultados"

def localizar_csv_mais_recente():
    """Varre a pasta e encontra o último arquivo csv gravado com records_ timestamp."""
    # Procura arquivos que comecem com 'records_' e terminem em '.csv' na pasta atual ou em 'resultados'
    candidatos = glob.glob("records_*.csv") + glob.glob(os.path.join(PASTA_RESULTADOS, "records_*.csv"))
    
    if candidatos:
        # Pega o arquivo com a maior (mais recente) data de modificação
        arquivo_mais_recente = max(candidatos, key=os.path.getmtime)
        print(f"-> Arquivo mais recente detectado com sucesso: {arquivo_mais_recente}")
        return arquivo_mais_recente

    print(f"Erro crítico: Nenhum arquivo no formato 'records_*.csv' foi encontrado!")
    sys.exit(1)

def main():
    # Garante que a pasta para salvar os gráficos exista
    os.makedirs(PASTA_RESULTADOS, exist_ok=True)
    
    # Descobre o último arquivo gerado automaticamente
    arquivo_alvo = localizar_csv_mais_recente()

    print("Carregando base de dados do experimento...")
    df = pd.read_csv(arquivo_alvo)

    print("Mapeando cenários a partir das posições sequenciais...")
    n_rows = len(df)
    vertices_col = []
    densidade_col = []
    algoritmo_col = []
    tipo_grafo_col = []

    for idx in range(n_rows):
        run_idx = idx // LINHAS_POR_REPETICAO
        vertices_idx = min(run_idx // (REPETICOES * len(DENSITIES_LIST)), len(VERTICES_LIST) - 1)
        density_idx = (run_idx // REPETICOES) % len(DENSITIES_LIST)

        vertices_col.append(VERTICES_LIST[vertices_idx])
        densidade_col.append(int(DENSITIES_LIST[density_idx] * 100))

        algoritmo_bruto = str(df.loc[idx, "algorithm"])
        algoritmo_col.append("Prim" if "Prim" in algoritmo_bruto else "Kruskal")
        tipo_grafo_col.append("Geometric" if "Geometric" in algoritmo_bruto else "Normal")

    df["Vertices"] = vertices_col
    df["DensidadePercentual"] = densidade_col
    df["Algoritmo"] = algoritmo_col
    df["TipoGrafo"] = tipo_grafo_col

    # ---------------------------------------------------------------------------
    # 1. Validação de Corretude (Verifica se pesos batem)
    # ---------------------------------------------------------------------------
    df["run_idx"] = df.index // LINHAS_POR_REPETICAO
    divergencias = []
    for (tipo, run), grupo in df.groupby(["TipoGrafo", "run_idx"]):
        pesos = grupo["weightMST"].unique()
        if len(pesos) > 1:
            divergencias.append({
                "TipoGrafo": tipo,
                "Vertices": grupo["Vertices"].iloc[0],
                "DensidadePercentual": grupo["DensidadePercentual"].iloc[0],
                "pesos_encontrados": list(pesos)
            })
    
    if divergencias:
        df_div = pd.DataFrame(divergencias)
        df_div.to_csv(os.path.join(PASTA_RESULTADOS, "validacao_pesos.csv"), index=False)
        print(f"-> AVISO: Encontradas divergências de pesos. Verifique 'resultados/validacao_pesos.csv'.")
    else:
        print("-> OK: Prim e Kruskal produziram os mesmos pesos em todos os cenários!")

    df.drop(columns=["run_idx"], inplace=True)

    # ---------------------------------------------------------------------------
    # 2. Cálculo Estatístico (Média e Desvio Padrão)
    # ---------------------------------------------------------------------------
    print("Calculando médias e desvios padrões...")
    df_stats = (
        df.groupby(["TipoGrafo", "Vertices", "DensidadePercentual", "Algoritmo"])
        .agg(
            constructionTime_media=("constructionTime", "mean"),
            executionTime_media=("executionTime", "mean"),
            executionTime_desvio=("executionTime", "std"),
            weightMST_media=("weightMST", "mean"),
        )
        .reset_index()
    )
    df_stats["executionTime_desvio"] = df_stats["executionTime_desvio"].fillna(0.0)
    
    df_stats.to_csv(os.path.join(PASTA_RESULTADOS, "resumo_estatistico.csv"), index=False)
    print(f"-> Resumo estatístico salvo em '{PASTA_SAIDA if 'PASTA_SAIDA' in locals() else PASTA_RESULTADOS}/resumo_estatistico.csv'")

    # ---------------------------------------------------------------------------
    # 3. Geração dos Painéis de Gráficos (Escala Logarítmica)
    # ---------------------------------------------------------------------------
    sns.set_theme(style="whitegrid")
    cores_prim = ["#E0F3F8", "#ABD9E9", "#74ADD1", "#4575B4", "#313695", "#1A1D60"]
    cores_kruskal = ["#FEE0D2", "#FCBBA1", "#FC9272", "#FB6A4A", "#DE2D26", "#A50F15"]
    vertices_reais = sorted(df_stats["Vertices"].unique())

    for tipo in df_stats["TipoGrafo"].unique():
        print(f"Renderizando painel gráfico para a topologia: {tipo}...")
        df_filtrado = df_stats[df_stats["TipoGrafo"] == tipo]
        densidades_unicas = sorted(df_filtrado["DensidadePercentual"].unique())

        fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 8), sharey=True)

        # Gráfico do Prim
        df_prim = df_filtrado[df_filtrado["Algoritmo"] == "Prim"]
        for idx, densidade in enumerate(densidades_unicas):
            df_sub = df_prim[df_prim["DensidadePercentual"] == densidade].sort_values("Vertices")
            if not df_sub.empty:
                ax1.errorbar(
                    df_sub["Vertices"], df_sub["executionTime_media"],
                    label=f"{densidade}%", color=cores_prim[idx % len(cores_prim)],
                    linestyle="--", marker="s", linewidth=2.5, markersize=6, capsize=3
                )
        ax1.set_title("Algoritmo de Prim", fontsize=13, fontweight="bold", pad=12)
        ax1.set_xlabel("Número de Vértices", fontsize=11, labelpad=8)
        ax1.set_ylabel("Tempo Médio de Execução (ms) - Escala Log", fontsize=11, labelpad=8)
        ax1.set_yscale("log")
        ax1.set_xticks(vertices_reais)
        ax1.set_xticklabels([str(v) for v in vertices_reais], rotation=45)
        ax1.legend(title="Densidade", loc="upper left")

        # Gráfico do Kruskal
        df_kruskal = df_filtrado[df_filtrado["Algoritmo"] == "Kruskal"]
        for idx, densidade in enumerate(densidades_unicas):
            df_sub = df_kruskal[df_kruskal["DensidadePercentual"] == densidade].sort_values("Vertices")
            if not df_sub.empty:
                ax2.errorbar(
                    df_sub["Vertices"], df_sub["executionTime_media"],
                    label=f"{densidade}%", color=cores_kruskal[idx % len(cores_kruskal)],
                    linestyle="-", marker="o", linewidth=2.5, markersize=6, capsize=3
                )
        ax2.set_title("Algoritmo de Kruskal", fontsize=13, fontweight="bold", pad=12)
        ax2.set_xlabel("Número de Vértices", fontsize=11, labelpad=8)
        ax2.set_yscale("log")
        ax2.set_xticks(vertices_reais)
        ax2.set_xticklabels([str(v) for v in vertices_reais], rotation=45)
        ax2.legend(title="Densidade", loc="upper left")

        nome_topologia = "Grafo Aleatório (Normal)" if tipo == "Normal" else "Grafo Geométrico"
        plt.suptitle(f"Análise de Desempenho dos Algoritmos - {nome_topologia}", fontsize=15, fontweight="bold", y=0.98)
        plt.tight_layout()
        
        plt.savefig(os.path.join(PASTA_RESULTS:=PASTA_RESULTADOS, f"grafo_{tipo.lower()}.png"), dpi=300)
        plt.close()
        print(f"-> Gráfico salvo em '{PASTA_RESULTS}/grafo_{tipo.lower()}.png'")

    print("\n[CONCLUÍDO] Processamento finalizado com sucesso!")

if __name__ == "__main__":
    main()