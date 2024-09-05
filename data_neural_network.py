import os
import re

# Localização dos arquivos
pasta = '/home/orfei/mestrado/progs'

# Função para formatar a string
def formatar_string(s):
    match = re.match(r'([a-zA-Z]+)([0-9]+(?:\.[0-9]+)?[Ee][0-9]+)', s)
    if match:
        palavra = match.group(1).capitalize()
        numero = match.group(2)
        return f'{palavra} {numero}'
    else:
        return s

# Função para calcular a média de uma lista
def calcular_media(lista):
    return sum(lista) / len(lista) if lista else 0

# Função para normalizar os valores de uma lista
def normalizar_lista(lista, minimo, maximo):
    return [(x - minimo) / (maximo - minimo) if maximo != minimo else 0 for x in lista]

dataframes = []

for arquivo in os.listdir(pasta):
    if arquivo.endswith('_array'):  
        print(f'Processando arquivo: {arquivo}')
        try:
            with open(os.path.join(pasta, arquivo), 'r') as f:
                linhas = f.readlines()
            
            df = []
            for linha in linhas:
                if 'TRIG' in linha:
                    partes = linha.split()
                    if len(partes) == 2:
                        trig = partes[0]
                        count = int(partes[1])
                        df.append((trig, count))
            
            # Agrupamento por TRIG
            agrupado = {}
            for trig, count in df:
                if trig not in agrupado:
                    agrupado[trig] = []
                agrupado[trig].append(count)
            
            # Calculo da média de densidade por TRIG
            densidade_media = {trig: calcular_media(contagens) for trig, contagens in agrupado.items()}
            
            # Nome formatado do arquivo
            nome_formatado = formatar_string(arquivo)
            composicao, energia = nome_formatado.split()
            
            # Adiciona as informações ao dataframe
            for trig, densidade in densidade_media.items():
                dataframes.append((trig, composicao, energia, densidade))
        
        except Exception as e:
            print(f'Erro ao processar o arquivo {arquivo}: {e}')

# Normalização da densidade
if dataframes:
    densidades = [item[3] for item in dataframes]
#    density_min = min(densidades)
#    density_max = max(densidades)
    density_min = 0
    density_max = 264.17
    
    dataframes_normalized = [
        (trig, composicao, energia, densidade, (densidade - density_min) / (density_max - density_min))
        for trig, composicao, energia, densidade in dataframes
    ]
    
    # Salva o resultado em um arquivo de texto
    with open('data_neural_network.txt', 'w') as f:
        f.write("TRIG,composition,energy,density_normalized\n")
        for trig, composicao, energia, densidade, densidade_norm in dataframes_normalized:
            f.write(f"{trig},{composicao},{energia},{densidade_norm}\n")
else:
    print("Nenhum dado foi processado. Verifique os arquivos na pasta.")

