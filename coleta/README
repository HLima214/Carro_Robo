# Carro Robô com TinyML — CP2 (ESP32 + Edge Impulse)

Evolução do carrinho-robô controlado por Bluetooth (CP1) com uma aplicação de **TinyML**: o sensor ultrassônico do próprio carrinho coleta dados reais, o modelo é treinado no **Edge Impulse**, exportado como biblioteca Arduino e embarcado no **ESP32**. O resultado da inferência interfere no comportamento do carrinho (bloqueio do movimento para frente quando um obstáculo se aproxima ou está parado à frente).

> **Equipe:** SFI - Search For Innovation
- Anny Carolina - RM98295
- Eric Carvalho - RM550249
- Henrique Lima - RM551528
- Pedro Menezes - RM97432
- Renato Izumi – RM99242
- Bernardo Rocha - RM99209
> **Disciplina / Turma:** 4ESPX

## Sumário

1. [Visão geral](#1-visão-geral)
2. [Hardware utilizado](#2-hardware-utilizado)
3. [Dataset e coleta de dados](#3-dataset-e-coleta-de-dados)
4. [Projeto no Edge Impulse](#4-projeto-no-edge-impulse)
5. [Integração no ESP32](#5-integração-no-esp32)
6. [Como reproduzir](#6-como-reproduzir)
7. [Estrutura do repositório](#7-estrutura-do-repositório)
8. [Limitações e melhorias](#8-limitações-e-melhorias)

---

## 1. Visão geral

Fluxo do projeto:

```
Sensor → Coleta de dados → CSV → Edge Impulse → Treinamento → Inferência → Biblioteca → ESP32 → Ação no carrinho
```

**Problema escolhido:** identificar o *comportamento do obstáculo* à frente do carrinho a partir de uma janela de 2 segundos de medições de distância. Um limite fixo de distância (`if distancia < X`) não diferencia um objeto que está vindo em direção ao carrinho de um que está se afastando; o modelo de ML usa a evolução da distância no tempo para isso.

---

## 2. Hardware utilizado

| Componente | Função |
|---|---|
| ESP32 DevKit (ESP32 Dev Module) | Microcontrolador, Bluetooth (Dabble) e inferência do modelo |
| Sensor ultrassônico HC-SR04 | Medição de distância (único sensor usado no ML) |
| Ponte H L298N (HW-095) | Acionamento dos motores |
| 4 motores DC com rodas (chassi 4WD) | Locomoção |
| Alimentação | Bateria 9v para o ESP32 e 4 pilhas 1.5v para alimentação ponte H |

**Ligações do sensor:**

| HC-SR04 | ESP32 |
|---|---|
| Trig | GPIO 14 |
| Echo | GPIO 12 |
| VCC / GND | 3.3v / GND |

**Ligações da ponte H:** IN1 = GPIO 27, IN2 = GPIO 26, IN3 = GPIO 25, IN4 = GPIO 33.

---

## 3. Dataset e coleta de dados

### 3.1 Sensor utilizado

**HC-SR04 (ultrassônico)**, o mesmo sensor de proximidade já presente no carrinho do CP1. Ele emite um pulso ultrassônico e mede o tempo de retorno do eco; a distância é calculada por `distância (cm) = tempo (µs) × 0,0343 / 2`.

### 3.2 Dados coletados

- **Grandeza:** distância ao obstáculo mais próximo à frente do sensor, em centímetros.
- **Frequência de amostragem:** 10 Hz (uma leitura a cada 100 ms).
- **Faixa:** 0 a 100 cm. Leituras acima de 100 cm, ou sem eco (nada à frente), são limitadas a **100 cm**, para manter os dados consistentes. A mesma função de medição é usada na coleta e na inferência.
- **Formato:** série temporal univariada, salva em CSV com as colunas `timestamp` (ms) e `dist` (cm).

### 3.3 Como a coleta foi realizada

A coleta foi feita pelo próprio grupo, em duas etapas:

1. **Firmware de coleta** (`firmware/coleta/coleta.ino`): grava no ESP32 um sketch simples que mede a distância a cada 100 ms e imprime o valor na Serial (115200 baud).
2. **Script Python** (`coleta/coletar.py`): com o ESP32 conectado por USB, lê a Serial por 15 segundos e salva um CSV novo por execução, nomeado `<classe>.<número>.csv`.

```bash
pip install pyserial
python coletar.py livre
python coletar.py aproximando
python coletar.py proximo_parado
python coletar.py afastando
```

Durante cada coleta de 15 s, o movimento correspondente à classe foi executado na frente do sensor (com a mão e/ou objetos), variando velocidade, objeto e distância inicial. A coleta foi feita com o carrinho parado

### 3.4 Classes definidas

| Classe | Situação representada | Ação do carrinho |
|---|---|---|
| `livre` | Nenhum obstáculo à frente (ou muito distante) | Anda normalmente |
| `aproximando` | Obstáculo vindo em direção ao sensor | Bloqueia o movimento para frente |
| `proximo_parado` | Obstáculo parado a curta distância | Bloqueia o movimento para frente |
| `afastando` | Obstáculo se afastando do sensor | Movimento liberado |

### 3.5 Arquivos CSV utilizados

Os arquivos ficam na pasta [`dataset/`](dataset/) e seguem o padrão `<classe>.<número>.csv`. O nome do arquivo foi usado como *label* no Edge Impulse.

| Classe | Arquivos CSV | Nº de arquivos | Duração total |
|---|---|---|---|
| `livre` | livre.01.csv … livre.10.csv | 15 | 225 segundos de coleta |
| `aproximando` | aproximando.01.csv … aproximando.10.csv | 15 | 225 segundos de coleta |
| `proximo_parado` | proximo_parado.01.csv … proximo_parado.10.csv | 10 | 150 segundos de coleta |
| `afastando` | afastando.01.csv … afastando.10.csv | 26 | 390 segundos de coleta |

Exemplo do conteúdo de um arquivo:

```
timestamp,dist
0,100.0
102,100.0
205,43.2
...
```

### 3.6 Quantidade de amostras

- Cada arquivo tem 15 s de coleta a 10 Hz, ou seja, cerca de **150 medições por arquivo**.
- **Total de medições coletadas:** 9900.
- **Total de janelas (amostras) geradas no Edge Impulse:** 49

---

## 4. Projeto no Edge Impulse

### 4.1 Preparação e importação dos dados

1. Criação de um projeto no Edge Impulse (https://edgeimpulse.com).
2. **Data acquisition → Add data → Upload data**, enviando os CSVs de cada classe.
3. Leitura do CSV pelo assistente do Edge Impulse, com `timestamp` como coluna de tempo e `dist` como o único eixo do sensor.
4. Definição do *label* de cada lote de arquivos (`livre`, `aproximando`, `proximo_parado`, `afastando`).

> Observação: no primeiro upload a opção "Infer from filename" não leu os labels corretamente e todas as amostras ficaram com um único label, o que impediu o treinamento. O problema foi resolvido removendo os dados e reenviando cada classe com o label informado manualmente.

### 4.2 Divisão dos dados

Os dados foram divididos automaticamente entre treinamento e teste (*Automatically split between training and testing*), em torno de 74% / 26%.

Dentro do conjunto de treinamento, o Edge Impulse reserva uma parte para **validação** (métricas da seção 4.6).

### 4.3 Configuração do Impulse

| Parâmetro | Valor |
|---|---|
| Tipo de dado | Time series data |
| Eixo | `dist` (1 eixo) |
| Window size | 2000 ms |
| Window increase | 200 ms |
| Frequência | 10 Hz |
| Processing block | Flatten |
| Learning block | Classification (Keras) |
| Classes de saída | afastando, aproximando, livre, proximo_parado |

A janela de 2 s equivale a 20 leituras por amostra; o deslocamento de 200 ms gera janelas sobrepostas e aumenta a quantidade de amostras de treino.

### 4.4 Processamento utilizado

O bloco **Flatten** reduz cada janela a estatísticas simples (como média, mínimo e máximo de `dist`, entre outras). O *Feature explorer* mostra como as amostras ficam distribuídas nesse espaço de features:

![Feature explorer](/prints/feature_explorer.png)

*Feature explorer com eixos `dist Average`, `dist Minimum` e `dist Maximum`. As classes `livre` (verde) e `proximo_parado` (vermelho) formam regiões bem definidas, nos extremos de distância. As classes `afastando` (azul) e `aproximando` (laranja) ficam sobrepostas ao longo da trajetória intermediária.*

### 4.5 Modelo e treinamento

- **Modelo:** rede neural densa (Keras) do bloco *Classification*, com 4 neurônios de saída (uma por classe).
- **Hiperparâmetros:** n° de épocas: 150 | Learning rate: 0.0005
- **Versão do modelo:** Quantized (int8), a mais leve e rápida para o ESP32, e a mesma exportada para a biblioteca.

### 4.6 Resultados obtidos

Desempenho do último treinamento (conjunto de validação):

| Métrica | Valor |
|---|---|
| Acurácia | **64,7%** |
| Loss | **0,75** |

Matriz de confusão (validação), com as linhas sendo a classe real e as colunas a classe prevista:

| Real \ Previsto | afastando | aproximando | livre | proximo_parado |
|---|---|---|---|---|
| **afastando** | **48,6%** | 12,3% | 38,0% | 1,1% |
| **aproximando** | 42,1% | **40,5%** | 0,8% | 16,5% |
| **livre** | 0,6% | 0% | **99,4%** | 0% |
| **proximo_parado** | 0% | 14,9% | 0% | **85,1%** |
| **F1 score** | 0,58 | 0,46 | 0,76 | 0,79 |

![Matriz de confusão e métricas do treinamento](/prints/matriz_confusao.png)

### 4.7 Análise dos resultados

- **`livre` (99,4%) e `proximo_parado` (85,1%)** são bem reconhecidas. Ambas têm distância praticamente constante e em faixas bem diferentes (perto de 100 cm × perto do sensor), o que as torna fáceis de separar só com estatísticas da janela.
- **`aproximando` (40,5%) e `afastando` (48,6%)** têm o pior desempenho e são muito confundidas entre si (42,1% das amostras de `aproximando` foram classificadas como `afastando`). Esse resultado é coerente com o bloco de processamento escolhido: média, mínimo e máximo **não dependem da ordem temporal** das leituras, e uma janela em que o objeto se aproxima contém os mesmos valores de uma janela em que ele se afasta, apenas em ordem invertida. Com essas features, a diferença entre as duas classes é pequena.
- **`afastando` → `livre` (38%):** quando o objeto se afasta, a distância termina saturada em 100 cm, e janelas dessa parte final ficam parecidas com as de `livre`.
- A acurácia geral de 64,7% é, portanto, limitada principalmente pela escolha do processamento, e não apenas pela quantidade de dados.

### 4.8 Testes de inferência

> [PREENCHER após rodar os testes no Edge Impulse e adicionar os prints em `docs/`]

**Model testing (conjunto de teste, nunca visto no treino):**

| Métrica | Valor |
|---|---|
| Acurácia no teste | 25.38% |
| Classes com melhor/pior resultado | Melhor resultado: proximo_parado | Pior resultado: livre |

---

## 5. Integração no ESP32

1. No Edge Impulse: **Deployment → Arduino library**, com otimização **Quantized (int8)**.
2. Na Arduino IDE: **Sketch → Include Library → Add .ZIP Library**.
3. O firmware final (`firmware/carrinho_ia/carrinho_ia.ino`) combina o controle Bluetooth (Dabble) com a inferência:
   - amostra a distância a cada 100 ms (`millis()`, sem `delay`) e mantém uma janela deslizante de 2 s;
   - roda `run_classifier()` a cada 0,5 s;
   - se a classe prevista for `aproximando` ou `proximo_parado` com confiança ≥ 70%, **bloqueia o comando "frente"** do GamePad (ré e giros continuam liberados);
   - mantém ainda uma parada de emergência por distância (≤ 8 cm), independente do modelo.

**Configuração da Arduino IDE:** core `esp32` **2.0.14** (compatível com a biblioteca DabbleESP32), placa *ESP32 Dev Module* e **Partition Scheme: Huge APP (3MB No OTA/1MB SPIFFS)**, necessário porque o modelo, o Bluetooth e o Dabble juntos ultrapassam a partição padrão.

---

## 6. Como reproduzir

1. Montar o carrinho conforme a [seção 2](#2-hardware-utilizado).
2. Instalar na Arduino IDE o core `esp32` 2.0.14 e a biblioteca **DabbleESP32**.
3. Gravar `firmware/coleta/coleta.ino` e rodar `coleta/coletar.py` para cada classe (ou usar os CSVs já disponíveis em `dataset/`).
4. Criar um projeto no Edge Impulse, importar os CSVs e configurar o Impulse conforme a [seção 4.3](#43-configuração-do-impulse).
5. Treinar, exportar a biblioteca Arduino e instalá-la pelo `.zip`.
6. Gravar `firmware/carrinho_ia/carrinho_ia.ino` (ajustando o nome do header `<..._inferencing.h>`) e controlar o carrinho pelo app **Dabble** (módulo GamePad).

---

## 7. Estrutura do repositório

```
Carro_Robo/
├── README.md
├── dataset/                 # CSVs coletados (<classe>.<n>.csv)
├── coleta/
│   └── coletar.py           # script de coleta via Serial
├── firmware/
│   ├── coleta/coleta.ino    # firmware de coleta de dados
│   └── carrinho_ia/
│       └── carrinho_ia.ino  # carrinho + inferência embarcada
└── docs/                    # prints do Edge Impulse
```

---

## 8. Limitações e melhorias

- **Acurácia de 64,7%** na versão atual, com baixa separação entre `aproximando` e `afastando` (seção 4.7).
- **Melhoria prioritária:** trocar o bloco Flatten pelo **Raw data** (que preserva a ordem das leituras) ou acrescentar uma feature de tendência (inclinação da distância na janela), para que o modelo enxergue a *direção* do movimento.
- Aumentar e diversificar o dataset (mais objetos, velocidades e coletas com o carrinho em movimento).
- Um único sensor ultrassônico tem cone de leitura estreito e sofre com superfícies inclinadas ou macias; um segundo sensor ou a fusão com outro tipo de sensor tornaria o sistema mais robusto.
- A parada de emergência por distância foi mantida por segurança, já que o modelo pode errar.
