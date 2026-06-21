"""
Script de consolidacao estatistica e geracao de graficos comparativos
para o benchmark dos algoritmos de Prim e Kruskal (MST).

Le o CSV bruto exportado pelo pipeline em C++ (formato:
algorithm,constructionTime,executionTime,weightMST), reconstroi os
parametros do cenario (Vertices, Densidade, Topologia) a partir da
posicao sequencial das linhas, calcula media e desvio padrao das
repeticoes, valida a corretude cruzada (Prim e Kruskal devem produzir
o mesmo weightMST na mesma instancia) e gera:

  - resultados/resumo_estatistico.csv   -> tabela consolidada (media + desvio padrao)
  - resultados/grafo_normal.png         -> painel Prim vs Kruskal (grafo aleatorio)
  - resultados/grafo_geometrico.png     -> painel Prim vs Kruskal (grafo geometrico)
  - resultados/validacao_pesos.csv      -> divergencias de weightMST entre os algoritmos (se houver)
"""

import os
import glob
import sys

import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# ---------------------------------------------------------------------------
# Parametros fixos do experimento (devem refletir exatamente o loop do C++)
# ---------------------------------------------------------------------------
VERTICES_LIST = [100, 500, 1000, 2500, 5000, 7500, 10000]
DENSITIES_LIST = [0.0, 0.2, 0.4, 0.6, 0.8, 1.0]
REPETICOES = 10          # numero de repeticoes por cenario (V, Densidade)
LINHAS_POR_REPETICAO = 4  # Prim Normal, Kruskal Normal, Prim Geometric, Kruskal Geometric

PASTA_RESULTADOS = "resultados"


def localizar_csv_mais_recente():
    """Localiza o CSV bruto mais recente gerado pelo pipeline em C++."""
    candidatos = glob.glob(os.path.join(PASTA_RESULTADOS, "records_*.csv"))
    if candidatos:
        arquivo = max(candidatos, key=os.path.getmtime)
        print(f"Arquivo de metricas detectado automaticamente: {arquivo}")
        return arquivo

    # Fallback para um nome fixo, caso o padrao timestamp nao seja usado
    fallback = os.path.join(PASTA_RESULTADOS, "resultados_pipeline.csv")
    if os.path.exists(fallback):
        print(f"Usando arquivo de fallback: {fallback}")
        return fallback

    print(f"Erro critico: nenhum CSV encontrado em '{PASTA_RESULTADOS}/'.")
    sys.exit(1)


def decodificar_parametros(df: pd.DataFrame) -> pd.DataFrame:
    """
    Reconstroi as colunas Vertices, DensidadePercentual, Algoritmo e
    TipoGrafo a partir da posicao sequencial (idx) de cada linha no CSV
    bruto, seguindo o mapeamento fixo definido no pipeline em C++:

        run_idx       = idx // 4               -> indice da execucao do grafo
        vertices_idx  = min(run_idx // 60, len(VERTICES_LIST) - 1)
        density_idx   = (run_idx // 10) % 6
    """
    n_rows = len(df)
    vertices_col = []
    densidade_col = []
    algoritmo_col = []
    tipo_grafo_col = []

    for idx in range(n_rows):
        run_idx = idx // LINHAS_POR_REPETICAO
        vertices_idx = min(run_idx // (REPETICOES * len(DENSITIES_LIST)),
                            len(VERTICES_LIST) - 1)
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
    return df


def validar_corretude(df: pd.DataFrame):
    """
    Verifica se Prim e Kruskal produziram o mesmo weightMST quando
    executados sobre a mesma instancia (mesmo cenario + mesma repeticao).
    Em caso de divergencia, salva um CSV com os casos problematicos.
    """
    print("Validando corretude cruzada (weightMST Prim == weightMST Kruskal)...")

    # Identifica a repeticao dentro de cada bloco de 4 linhas (Prim/Kruskal x Normal/Geometric)
    df["run_idx"] = df.index // LINHAS_POR_REPETICAO

    divergencias = []
    for (tipo, run_idx), grupo in df.groupby(["TipoGrafo", "run_idx"]):
        pesos = grupo["weightMST"].unique()
        if len(pesos) > 1:
            divergencias.append({
                "TipoGrafo": tipo,
                "run_idx": run_idx,
                "Vertices": grupo["Vertices"].iloc[0],
                "DensidadePercentual": grupo["DensidadePercentual"].iloc[0],
                "pesos_encontrados": list(pesos),
            })

    if divergencias:
        df_div = pd.DataFrame(divergencias)
        caminho = os.path.join(PASTA_RESULTADOS, "validacao_pesos.csv")
        df_div.to_csv(caminho, index=False)
        print(f"-> ATENCAO: {len(divergencias)} divergencia(s) de weightMST encontrada(s). "
              f"Detalhes salvos em '{caminho}'.")
    else:
        print("-> OK: Prim e Kruskal produziram o mesmo weightMST em todos os cenarios.")

    df.drop(columns=["run_idx"], inplace=True)


def calcular_estatisticas(df: pd.DataFrame) -> pd.DataFrame:
    """Agrupa por cenario e algoritmo, calculando media e desvio padrao
    do tempo de construcao e do tempo de execucao ao longo das repeticoes."""
    print("Calculando media e desvio padrao das repeticoes por cenario...")

    df_stats = (
        df.groupby(["TipoGrafo", "Vertices", "DensidadePercentual", "Algoritmo"])
        .agg(
            constructionTime_media=("constructionTime", "mean"),
            constructionTime_desvio=("constructionTime", "std"),
            executionTime_media=("executionTime", "mean"),
            executionTime_desvio=("executionTime", "std"),
            weightMST_media=("weightMST", "mean"),
            n_repeticoes=("executionTime", "count"),
        )
        .reset_index()
        .sort_values(["TipoGrafo", "Algoritmo", "DensidadePercentual", "Vertices"])
    )

    df_stats["executionTime_desvio"] = df_stats["executionTime_desvio"].fillna(0.0)
    df_stats["constructionTime_desvio"] = df_stats["constructionTime_desvio"].fillna(0.0)

    caminho = os.path.join(PASTA_RESULTADOS, "resumo_estatistico.csv")
    df_stats.to_csv(caminho, index=False)
    print(f"-> Resumo estatistico salvo em '{caminho}'.")

    return df_stats


def gerar_paineis_comparativos(df_stats: pd.DataFrame):
    """Gera, para cada topologia (Normal/Geometric), um painel lado a lado
    com Prim (tons frios, tracejado, marcador quadrado) e Kruskal (tons
    quentes, linha continua, marcador circular), em escala log no eixo Y."""

    sns.set_theme(style="whitegrid")

    cores_prim = ["#E0F3F8", "#ABD9E9", "#74ADD1", "#4575B4", "#313695", "#1A1D60"]
    cores_kruskal = ["#FEE0D2", "#FCBBA1", "#FC9272", "#FB6A4A", "#DE2D26", "#A50F15"]

    vertices_reais = sorted(df_stats["Vertices"].unique())

    for tipo in df_stats["TipoGrafo"].unique():
        print(f"Renderizando painel comparativo para o Grafo Tipo: {tipo}...")
        df_filtrado = df_stats[df_stats["TipoGrafo"] == tipo]
        densidades_unicas = sorted(df_filtrado["DensidadePercentual"].unique())

        fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(18, 9), sharey=True)

        # --- Prim ---
        df_prim = df_filtrado[df_filtrado["Algoritmo"] == "Prim"]
        for idx, densidade in enumerate(densidades_unicas):
            df_sub = df_prim[df_prim["DensidadePercentual"] == densidade].sort_values("Vertices")
            if not df_sub.empty:
                ax1.errorbar(
                    df_sub["Vertices"],
                    df_sub["executionTime_media"],
                    yerr=df_sub["executionTime_desvio"],
                    label=f"{densidade}%",
                    color=cores_prim[idx % len(cores_prim)],
                    linestyle="--",
                    marker="s",
                    linewidth=2.5,
                    markersize=6,
                    capsize=3,
                )

        ax1.set_title("Algoritmo de Prim", fontsize=13, fontweight="bold", pad=12)
        ax1.set_xlabel("Numero de Vertices", fontsize=11, labelpad=8)
        ax1.set_ylabel("Tempo Medio de Execucao (ms) - Escala Log", fontsize=11, labelpad=8)
        ax1.set_yscale("log")
        ax1.set_xticks(vertices_reais)
        ax1.set_xticklabels([str(v) for v in vertices_reais], rotation=45)
        ax1.legend(title="Densidade", loc="upper left", fontsize=9,
                   title_fontproperties={"weight": "bold"})

        # --- Kruskal ---
        df_kruskal = df_filtrado[df_filtrado["Algoritmo"] == "Kruskal"]
        for idx, densidade in enumerate(densidades_unicas):
            df_sub = df_kruskal[df_kruskal["DensidadePercentual"] == densidade].sort_values("Vertices")
            if not df_sub.empty:
                ax2.errorbar(
                    df_sub["Vertices"],
                    df_sub["executionTime_media"],
                    yerr=df_sub["executionTime_desvio"],
                    label=f"{densidade}%",
                    color=cores_kruskal[idx % len(cores_kruskal)],
                    linestyle="-",
                    marker="o",
                    linewidth=2.5,
                    markersize=6,
                    capsize=3,
                )

        ax2.set_title("Algoritmo de Kruskal", fontsize=13, fontweight="bold", pad=12)
        ax2.set_xlabel("Numero de Vertices", fontsize=11, labelpad=8)
        ax2.set_yscale("log")
        ax2.set_xticks(vertices_reais)
        ax2.set_xticklabels([str(v) for v in vertices_reais], rotation=45)
        ax2.legend(title="Densidade", loc="upper left", fontsize=9,
                   title_fontproperties={"weight": "bold"})

        nome_topologia = "Grafo Aleatorio (Normal)" if tipo == "Normal" else "Grafo Geometrico"
        plt.suptitle(f"Analise de Desempenho dos Algoritmos - {nome_topologia}",
                     fontsize=15, fontweight="bold", y=0.98)
        plt.tight_layout()

        nome_saida = os.path.join(PASTA_RESULTADOS, f"grafo_{tipo.lower()}.png")
        plt.savefig(nome_saida, dpi=300)
        plt.close()
        print(f"-> Painel salvo com sucesso: '{nome_saida}'")


def main():
    os.makedirs(PASTA_RESULTADOS, exist_ok=True)

    arquivo_csv = localizar_csv_mais_recente()

    print("Carregando base de dados do experimento...")
    df = pd.read_csv(arquivo_csv, sep=",")

    print("Mapeando colunas do formato bruto do C++ (Vertices, Densidade, Algoritmo, Topologia)...")
    df = decodificar_parametros(df)

    validar_corretude(df)

    df_stats = calcular_estatisticas(df)

    gerar_paineis_comparativos(df_stats)

    print("\n[CONCLUIDO] Estatisticas calculadas e graficos gerados com sucesso!")


if __name__ == "__main__":
    main()