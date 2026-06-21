import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import os
import glob

def gerar_analise_grafos_lado_a_lado_limpo():
    # Detecta o arquivo de resultados mais recente no formato 'records_*.csv'
    arquivos = glob.glob('resultados/records_*.csv')
    if arquivos:
        arquivo_csv = max(arquivos, key=os.path.getmtime)
        print(f"Arquivo de métricas detectado automaticamente: {arquivo_csv}")
    else:
        arquivo_csv = 'resultados/resultados_pipeline.csv'

    if not os.path.exists(arquivo_csv):
        print(f"Erro Critico: O arquivo '{arquivo_csv}' nao foi encontrado.")
        return

    print("Carregando base de dados do experimento...")
    df = pd.read_csv(arquivo_csv, sep=',')

    # Se o arquivo CSV for o gerado diretamente pelo C++ original
    if 'algorithm' in df.columns:
        print("Mapeando colunas do formato bruto do C++...")
        vertices_list = [100, 500, 1000, 2500, 5000, 7500, 10000]
        densities_list = [0.0, 0.2, 0.4, 0.6, 0.8, 1.0]

        n_rows = len(df)
        vertices_col = []
        densities_col = []
        algoritmo_col = []
        tipo_grafo_col = []

        for idx in range(n_rows):
            run_idx = idx // 4
            vertices_idx = min(run_idx // 60, len(vertices_list) - 1)
            density_idx = (run_idx // 10) % 6
            
            vertices_col.append(vertices_list[vertices_idx])
            densities_col.append(int(densities_list[density_idx] * 100))

            algo_name = df.loc[idx, 'algorithm']
            if 'Prim' in str(algo_name):
                algoritmo_col.append('Prim')
            else:
                algoritmo_col.append('Kruskal')
                
            if 'Geometric' in str(algo_name):
                tipo_grafo_col.append('Geometric')
            else:
                tipo_grafo_col.append('Normal')

        df['Vertices'] = vertices_col
        df['DensidadePercentual'] = densities_col
        df['Algoritmo'] = algoritmo_col
        df['TipoGrafo'] = tipo_grafo_col
        df['TempoMilissegundos'] = df['executionTime']

    print("Calculando a media aritmetica das repeticoes para colapsar os dados...")
    df_medio = df.groupby(
        ['TipoGrafo', 'Vertices', 'DensidadePercentual', 'Algoritmo']
    )['TempoMilissegundos'].mean().reset_index()

    sns.set_theme(style="whitegrid")
    tipos_grafos = df_medio['TipoGrafo'].unique()
    vertices_reais = sorted(df_medio['Vertices'].unique())

    cores_prim = ['#E0F3F8', '#ABD9E9', '#74ADD1', '#4575B4', '#313695', '#1A1D60']
    cores_kruskal = ['#FEE0D2', '#FCBBA1', '#FC9272', '#FB6A4A', '#DE2D26', '#A50F15']

    for tipo in tipos_grafos:
        print(f"Renderizando painel lado a lado corrigido para o Grafo Tipo: {tipo}...")
        df_filtrado = df_medio[df_medio['TipoGrafo'] == tipo]

        fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(18, 9), sharey=True)
        
        densidades_unicas = sorted(df_filtrado['DensidadePercentual'].unique())

        df_prim = df_filtrado[df_filtrado['Algoritmo'] == 'Prim']
        for idx, densidade in enumerate(densidades_unicas):
            df_sub = df_prim[df_prim['DensidadePercentual'] == densidade].sort_values(by='Vertices')
            if not df_sub.empty:
                ax1.plot(
                    df_sub['Vertices'], 
                    df_sub['TempoMilissegundos'], 
                    label=f'{densidade}%', 
                    color=cores_prim[idx], 
                    linestyle='--', 
                    marker='s', 
                    linewidth=2.5, 
                    markersize=6
                )
        
        ax1.set_title('Algoritmo de Prim', fontsize=13, fontweight='bold', pad=12)
        ax1.set_xlabel('Numero de Vertices', fontsize=11, labelpad=8)
        ax1.set_ylabel('Tempo de Execucao Medio (Milissegundos)', fontsize=11, labelpad=8)
        ax1.set_yscale('log')
        ax1.set_xticks(vertices_reais)
        ax1.set_xticklabels([str(v) for v in vertices_reais], rotation=45)
        ax1.legend(title='Densidade', loc='upper left', fontsize=9, title_fontproperties={'weight': 'bold'})

        df_kruskal = df_filtrado[df_filtrado['Algoritmo'] == 'Kruskal']
        for idx, densidade in enumerate(densidades_unicas):
            df_sub = df_kruskal[df_kruskal['DensidadePercentual'] == densidade].sort_values(by='Vertices')
            if not df_sub.empty:
                ax2.plot(
                    df_sub['Vertices'], 
                    df_sub['TempoMilissegundos'], 
                    label=f'{densidade}%', 
                    color=cores_kruskal[idx], 
                    linestyle='-', 
                    marker='o', 
                    linewidth=2.5, 
                    markersize=6
                )
        
        ax2.set_title('Algoritmo de Kruskal', fontsize=13, fontweight='bold', pad=12)
        ax2.set_xlabel('Numero de Vertices', fontsize=11, labelpad=8)
        ax2.set_yscale('log')
        ax2.set_xticks(vertices_reais)
        ax2.set_xticklabels([str(v) for v in vertices_reais], rotation=45)
        ax2.legend(title='Densidade', loc='upper left', fontsize=9, title_fontproperties={'weight': 'bold'})

        plt.suptitle(f'Analise de Desempenho dos algoritmos', 
                     fontsize=15, fontweight='bold', y=0.98)
        
        plt.tight_layout()

        nome_saida = f'resultados/grafo_{tipo.lower()}.png'
        plt.savefig(nome_saida, dpi=300)
        plt.close()
        print(f"-> Painel unificado salvo com sucesso: '{nome_saida}'")

    print("\n[CONCLUIDO] Graficos gerados perfeitamente retilineos e sem quebras!")

if __name__ == '__main__':
    gerar_analise_grafos_lado_a_lado_limpo()