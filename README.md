# Sistema de Processamento de Dados de Sensores Industriais

Este projeto consiste em um conjunto de três programas em C desenvolvidos para gerenciar, organizar e consultar dados de sensores de uma planta industrial inteligente. O sistema é capaz de processar um grande volume de leituras brutas, separá-las por sensor, ordená-las para consulta rápida e permitir buscas eficientes por instantes de tempo específicos.

## Estrutura do Projeto

O sistema é composto por três programas principais:

1.  **Programa 1 - Organizador de Dados (`prog1`)**: Processa um arquivo de entrada com leituras de múltiplos sensores, separa os dados em arquivos individuais por sensor e os ordena.
2.  **Programa 2 - Consultor de Medições (`prog2`)**: Realiza uma busca binária em um arquivo de sensor para encontrar a medição mais próxima de um timestamp fornecido.
3.  **Programa 3 - Gerador de Dados de Teste (`prog3`)**: Cria um arquivo de dados brutos com leituras aleatórias para simular o ambiente de produção, facilitando testes e validação.

## Principais Alterações Implementadas

Este trabalho representa uma evolução do sistema original, com as seguintes modificações chave:

1.  **Ordenação Decrescente (Programa 1)**
    * Os dados em cada arquivo de sensor agora são organizados em **ordem decrescente** de timestamp (do mais recente para o mais antigo). A função de comparação `qsort` foi ajustada para refletir essa nova lógica.

2.  **Busca Binária em Ordem Decrescente (Programa 2)**
    * O algoritmo de **busca binária foi adaptado** para funcionar corretamente com arquivos ordenados de forma decrescente. Isso garante que as consultas continuem sendo eficientes e precisas.

3.  **Tipos de Dados Dinâmicos na Geração (Programa 3)**
    * O gerador de testes agora aceita um argumento de linha de comando para especificar o tipo de dado de cada sensor, tornando a criação de cenários de teste mais flexível e realista. Os tipos suportados são:
        * `CONJ_Z`: Números inteiros.
        * `CONJ_Q`: Números racionais (ponto flutuante).
        * `TEXTO`: Strings de até 16 caracteres.
        * `BINARIO`: Valores booleanos (`true` ou `false`).

## Tecnologias Utilizadas

* **Linguagem**: C
* **Compilador**: GCC (ou compatível)

## Como Compilar e Executar

Siga os passos abaixo para compilar e utilizar o sistema.

### 1. Pré-requisitos

Certifique-se de ter um compilador C, como o GCC, instalado em seu sistema.

### 2. Compilação

Compile os três programas-fonte usando os seguintes comandos no seu terminal:

```bash
# Compila o organizador de dados
gcc programa1.c -o prog1

# Compila o consultor (a flag -lm é necessária para a biblioteca matemática)
gcc programa2.c -o prog2 -lm

# Compila o gerador de testes
gcc programa3.c -o prog3
```

### 3. Fluxo de Execução (Exemplo Prático)

O fluxo de trabalho típico é gerar dados, organizá-los e, em seguida, consultá-los.

**Passo 1: Gerar Dados de Teste (`prog3`)**

Crie um arquivo `dados_brutos.txt` com 2000 leituras para quatro sensores diferentes, cada um com um tipo de dado específico.

```bash
./prog3 dados_brutos.txt "01/01/2025 00:00:00" "01/01/2025 23:59:59" \
SensorTemp CONJ_Z \
SensorUmid CONJ_Q \
SensorStatus BINARIO \
SensorErro TEXTO
```

**Passo 2: Organizar os Dados (`prog1`)**

Processe o arquivo bruto para separar e ordenar os dados por sensor.

```bash
./prog1 dados_brutos.txt
```

* **Resultado**: Serão criados os arquivos `SensorTemp.txt`, `SensorUmid.txt`, `SensorStatus.txt` e `SensorErro.txt`. Cada arquivo conterá as leituras ordenadas em ordem **decrescente** pelo timestamp.

**Passo 3: Consultar uma Medição (`prog2`)**

Realize uma busca no arquivo `SensorTemp.txt` para encontrar a leitura mais próxima de um timestamp específico. Por exemplo, para o timestamp `1735740000` (equivalente a 01/01/2025 11:00:00):

```bash
./prog2 SensorTemp 1735740000
```

* **Resultado**: O programa exibirá a leitura (timestamp e valor) do `SensorTemp` que foi registrada no instante mais próximo ao fornecido.

---

## Detalhes dos Programas

### `prog1` - Organizador de Dados

* **Objetivo**: Ler um arquivo de dados brutos e criar arquivos separados e ordenados por sensor.
* **Sintaxe**: `./prog1 <arquivo_de_entrada>`
* **Exemplo**: `./prog1 dados_brutos.txt`

### `prog2` - Consultor de Medições

* **Objetivo**: Encontrar a leitura mais próxima de um timestamp em um arquivo de sensor ordenado.
* **Sintaxe**: `./prog2 <ID_SENSOR> <timestamp_unix>`
* **Exemplo**: `./prog2 SensorUmid 1735740000`

### `prog3` - Gerador de Dados de Teste

* **Objetivo**: Gerar um arquivo de teste com dados aleatórios para múltiplos sensores.
* **Sintaxe**: `./prog3 <arquivo_saida> "data_inicio" "data_fim" <sensor1> <tipo1> ...`
* **Exemplo**: `./prog3 dados.txt "01/01/2025 08:00:00" "01/01/2025 18:00:00" SensorTemp CONJ_Z SensorStatus BINARIO`