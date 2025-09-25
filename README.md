# Reconstrução de chuveiros atmosféricos extensos com redes neurais

Esse repositório é um guia para pesquisadores do Grupo de Raios Cósmicos da UFABC.
Ao todo, foram trabalhados com três sistemas de tanques Cherenkov: um sistema triangular com três tanques, um sistema quadricular com 100 tanques e um sistema do Observatório Pierre Auger.

Após as simulações do CORSIKA, as análises devem seguir os seguintes passos:

## 1 - corsikaread.f

Esse arquivo, em Fortran, realiza a extração dos dados que queremos do arquivo bruto gerado pelo CORSIKA. É necessário, portanto, compilá-lo antes de tomar quaisquer outras ações conforme as intruções dentro do próprio arquivo. O próximo passo é criar um arquivo vazio com o nome de 'input', contendo o nome do arquivo gerado pela simulação. Por exemplo
'DAT401141', um próton primário com energia de $10^{14}$ eV. Depois, basta usar o código para executar o corsikaread:
```
./corsikaread <input >output
```
sendo o output o nome do arquivo de saída, recomendo usar o prefixo OUT. Para o caso do exemplo, ficaria 'OUT40141'. Assim é possível identificar e separar os arquivos com facilidade.

Para energias com thinning, basta seguir os mesmos passos com o arquivo corsikaread_thin.f.

## 2 - read_part.cc

Para cada sistema estudado existe um diferente arquivo de read_part. O objetivo é extrair dados das partículas que queremos dentro sistema. Portanto, estamos analisando quais e quantas partículas foram detectadas por cada tanque Cherenkov.

Para compilar, use o comando abaixo.
```
gcc -o read_part read_part.cc 
```

Como entrada, ele recebe o arquivo 'OUT401141' e retorna dois arquivos como saída: 'proton1E14_3det' e 'OUT401141_tratado'. O primeiro arquivo tem informações da simulação no cabeçalho, contendo a partícula primária e os ângulos de entrada e informações do que cada tanque detectou. No caso abaixo, temos um exemplo de um chuveiro iniciado por um próton de $10^{14}$ eV, as colunas se referem a quantidade de pósitrons, elétrons, múons mais e múons menos que foram detectadas respectivamente. A última coluna é o menor tempo de disparo, em nanosegundos, em que essas partículas atravessaram o tanque em questão. Com essas informações é possível reconstruir o chuveiro.

```
1.20000E+01EVTH 13 14 1.000e+05 20.263 17.833 50.726
TRIG1 1 4 1 0 7.06209e+04
TRIG2 1 4 0 0 7.06131e+04
TRIG3 3 2 0 0 7.06182e+04
```

O segundo arquivo tem informações de todos os elétrons e múons que foram gerados pelo chuveiro e que chegaram ao nível de observação, contendo os dados de posição. Esse arquivo, então, é essencial para construir um perfil lateral da partícula primária.

Para energias com thinning é necessário algumas mudanças no código, por isso há também arquivos read_part_thin.cc que, em essência, tem a mesma função do read_part.cc.

## 3 - lateral-distribution.cpp

Com o arquivo 'OUT401141_tratado' é possível construir perfis laterais e ajustar funções como a NKG. O arquivo 'lateral-distribution.cpp' constrói um histograma das partículas em função da distância radial do centro do eixo do chuveiro, considerando também o peso de todas as simulações. Há algumas funções NKG modificadas, basta escolher uma e gerar o arquivo PDF da distribuição lateral.

## 4 - analytics.ipynb

Esses notebooks têm todas as análises dos chuveiros e reconstruções das energias do primário e dos ângulos de entrada.

## 5 - NN.ipynb

Esses notebooks têm as reconstruções dos chuveiros utilizando redes neurais. O arquivo que gera os dados para estarem no padrão de 'NN.ipynb' está nomeado como 'data_NN.ipynb'.
