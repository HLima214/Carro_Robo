# 🤖 Little Mack

> Carrinho-robô desenvolvido para a disciplina **Project-based Maker Lab (FIAP)**.

![ESP32](https://img.shields.io/badge/Microcontrolador-ESP32-blue?style=for-the-badge&logo=espressif)
![C++](https://img.shields.io/badge/Linguagem-C%2B%2B%2FArduino-00599C?style=for-the-badge&logo=cplusplus)
![Status](https://img.shields.io/badge/Status-Em_Desenvolvimento-orange?style=for-the-badge)

---

## 👥 Integrantes:

* **Anny Carolina** — RM98295
* **Eric Carvalho** — RM550249
* **Henrique Lima** — RM551528
* **Pedro Menezes** — RM97432
* **Renato Izumi** — RM99242

---

## 🎯 Objetivo


Desenvolver um carrinho-robô funcional, controlado remotamente via smartphone e capaz de detectar obstáculos em seu trajeto, integrando os conhecimentos de projeto mecânico, eletrônica e programação trabalhados na disciplina.

**Evolução (CP2):** o mesmo carrinho foi evoluído com uma aplicação de **TinyML**. O sensor ultrassônico passou a gerar um dataset real, usado para treinar um modelo no **Edge Impulse** e embarcá-lo no ESP32, de modo que o resultado da classificação interfira no comportamento do carrinho.

---

## ⚡ Principais funcionalidades

* 🕹️ Movimentação para frente e para os lados (curvas)
* 📡 Detecção de obstáculos/movimento via sensor ultrassônico
* 📱 Controle remoto via aplicativo de celular (Dabble), conectado por Bluetooth ao ESP32
* 🧠 Classificação do comportamento do obstáculo (livre, aproximando, próximo parado, afastando) por um modelo de TinyML embarcado no ESP32
* 🛑 Bloqueio automático do comando "frente" quando o modelo detecta obstáculo se aproximando ou parado à frente
* 🔄 Manobra autônoma (ré e giro) após o bloqueio, devolvendo os controles ao usuário em seguida
* 🚨 Parada de emergência por distância mínima, independente do modelo

---

## 📂 Organização do repositório

| Arquivo/Pasta | Descrição |
| :--- | :--- |
| `CodigoCarrinho.ino` | Firmware do carrinho (ESP32) |
| `Diagrama.ligacoes.png` / `Diagrama.ligacoes.drawio.png` | Diagrama das conexões elétricas |
| `Documentacao_Tecnica_Carro_Robo.md` | Documentação técnica detalhada |
| `MicrosoftTeams-video.mp4` | Vídeo de teste do carrinho |
| `Organização/Precificação` | <!-- TODO: descrever o conteúdo desta pasta --> |
| `firmware/coleta/coleta.ino` | Firmware de coleta de dados (envia a distância medida pela Serial) |
| `coleta/coletar.py` | Script Python que lê a Serial e salva os CSVs do dataset |
| `dataset/` | CSVs coletados com o sensor do carrinho (`<classe>.<número>.csv`) |
| `firmware/carrinho_ia/carrinho_ia.ino` | Firmware final do carrinho com inferência embarcada (CP2) |

---

## 🛠️ Projeto mecânico e fabricação

* **Chassi:**
* Peças montadas manualmente pela equipe
* Motores soldados manualmente aos cabos de ligação
* **Carenagem:**

---

## 🔌 Hardware e eletrônica

* **Microcontrolador:** ESP32
* **Ponte H:** L298N (módulo HW-095)
* **Sensor:** ultrassônico HC-SR04 (detecção de obstáculos)
* **Alimentação:** baterias de lítio 18650 / power bank
* **Comunicação sem fio:** Bluetooth, via app Dabble no celular (ESP32 dispensa módulo HC-05/HM-10 separado)
* **Diagrama de conexões:** ver `Diagrama.ligacoes.png`
* **Ligações do sensor:** Trig → GPIO 14, Echo → GPIO 12
* **Ligações da ponte H:** IN1 → GPIO 27, IN2 → GPIO 26, IN3 → GPIO 25, IN4 → GPIO 33

---

## 💻 Software

Código-fonte completo em `CodigoCarrinho.ino`. A versão com TinyML (CP2) está em `firmware/carrinho_ia/carrinho_ia.ino`.


* **Controle dos motores:** os quatro pinos IN1 a IN4 da ponte H L298N são acionados com `digitalWrite`, por meio das funções `moveForward`, `moveBackward`, `turnLeft`, `turnRight` e `stopMotors`. Quando nenhum botão está pressionado, os motores são parados.
* **Comunicação sem fio (Bluetooth/Dabble):** a biblioteca DabbleESP32 expõe o ESP32 como o dispositivo Bluetooth "HAREP". No `loop()`, o código chama `Dabble.processInput()` e consulta os botões do módulo GamePad (`GamePad.isUpPressed()`, `isDownPressed()`, `isLeftPressed()` e `isRightPressed()`).
* **Uso do sensor ultrassônico:** o HC-SR04 mede a distância (limitada a 100 cm). No CP1, o carrinho parava ao detectar objeto a 15 cm ou menos. No CP2, as leituras alimentam o modelo de TinyML e mantêm uma parada de emergência a 8 cm.

---

## 🧠 TinyML com Edge Impulse (CP2)

Fluxo do projeto:

```
Sensor → Coleta de dados → CSV → Edge Impulse → Treinamento → Inferência → Biblioteca → ESP32 → Ação no carrinho
```

**Problema escolhido:** identificar o *comportamento do obstáculo* à frente do carrinho a partir de uma janela de 2 segundos de medições de distância. Um limite fixo de distância não diferencia um objeto que vem em direção ao carrinho de um que está se afastando.

### 📊 Dataset e coleta de dados

* **Sensor utilizado:** HC-SR04 (ultrassônico), o mesmo sensor do carrinho do CP1. A distância é calculada por `distância (cm) = tempo do eco (µs) × 0,0343 / 2`.
* **Dados coletados:** distância ao obstáculo à frente, em centímetros, a 10 Hz (uma leitura a cada 100 ms). Leituras acima de 100 cm, ou sem eco, são limitadas a 100 cm. A mesma função de medição é usada na coleta e na inferência.
* **Como a coleta foi realizada:** o firmware `firmware/coleta/coleta.ino` imprime a distância na Serial (115200 baud) e o script `coleta/coletar.py` (com `pyserial`) salva 15 segundos de leituras por execução em um CSV com as colunas `timestamp` (ms) e `dist` (cm). Em cada coleta, o movimento correspondente à classe foi feito na frente do sensor, variando velocidade, objeto e distância inicial.

```bash
pip install pyserial
python coletar.py livre
python coletar.py aproximando
python coletar.py proximo_parado
python coletar.py afastando
```

* **Classes definidas:**

| Classe | Situação representada | Ação do carrinho |
| :--- | :--- | :--- |
| `livre` | Nenhum obstáculo à frente (ou muito distante) | Anda normalmente |
| `aproximando` | Obstáculo vindo em direção ao sensor | Bloqueia o "frente" e executa a manobra |
| `proximo_parado` | Obstáculo parado a curta distância | Bloqueia o "frente" e executa a manobra |
| `afastando` | Obstáculo se afastando do sensor | Movimento liberado |

* **Arquivos CSV utilizados:** pasta `dataset/`, com arquivos no padrão `<classe>.<número>.csv`. O nome do arquivo foi usado como *label* no Edge Impulse. [PREENCHER: quantidade de arquivos por classe, ex.: livre.01.csv … livre.NN.csv]
* **Quantidade de dados:** **16 min 27 s** de dados coletados (987 s), ou seja, cerca de **9.870 leituras** de distância a 10 Hz. [PREENCHER: número de arquivos/amostras e de janelas de 2 s geradas, conforme o Edge Impulse]

### ⚙️ Projeto no Edge Impulse

* **Preparação/importação dos dados:** upload dos CSVs em *Data acquisition → Upload data*, com `timestamp` como coluna de tempo e `dist` como único eixo do sensor, e o label de cada classe informado manualmente. No primeiro upload, a opção "Infer from filename" não leu os labels e todas as amostras ficaram com um único label, o que impediu o treinamento. O problema foi resolvido removendo os dados e reenviando cada classe com o label explícito.
* **Divisão dos dados:** automática entre treino e teste, **74% / 26%**. Parte do conjunto de treino é reservada pelo Edge Impulse para validação.
* **Configuração do Impulse:**

| Parâmetro | Valor |
| :--- | :--- |
| Tipo de dado | Time series data (1 eixo: `dist`) |
| Window size | 2000 ms |
| Window increase | 200 ms |
| Frequência | 10 Hz |
| Processing block | Flatten |
| Learning block | Classification (Keras) |
| Saídas | afastando, aproximando, livre, proximo_parado |

* **Processamento utilizado:** o bloco **Flatten** reduz cada janela de 2 s (20 leituras) a estatísticas simples, como média, mínimo e máximo de `dist`. No *Feature explorer*, `livre` e `proximo_parado` formam regiões bem definidas, enquanto `afastando` e `aproximando` ficam sobrepostas.
* **Modelo escolhido:** rede neural densa (Keras) com 4 saídas, uma por classe, na versão **Quantized (int8)**, a mais leve para o ESP32. [PREENCHER: épocas, learning rate e arquitetura, conforme a tela Classifier]
* **Treinamento e resultados (conjunto de validação):**

| Métrica | Valor |
| :--- | :--- |
| Acurácia | **64,7%** |
| Loss | **0,75** |

Matriz de confusão (validação), com linhas sendo a classe real e colunas a classe prevista:

| Real \ Previsto | afastando | aproximando | livre | proximo_parado |
| :--- | :--- | :--- | :--- | :--- |
| **afastando** | **48,6%** | 12,3% | 38,0% | 1,1% |
| **aproximando** | 42,1% | **40,5%** | 0,8% | 16,5% |
| **livre** | 0,6% | 0% | **99,4%** | 0% |
| **proximo_parado** | 0% | 14,9% | 0% | **85,1%** |
| **F1 score** | 0,58 | 0,46 | 0,76 | 0,79 |

* **Testes de inferência (Model testing, dados nunca vistos no treino):**

| Métrica | Valor |
| :--- | :--- |
| Acurácia | **25,38%** |
| Área sob a curva ROC | 0,83 |
| Precisão média ponderada | 0,65 |
| Recall médio ponderado | 0,60 |
| F1 médio ponderado | 0,59 |

Matriz de confusão (teste), em que *uncertain* indica janelas em que nenhuma classe atingiu o limiar de confiança:

| Real \ Previsto | afastando | aproximando | livre | proximo_parado | uncertain |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **afastando** | **32,6%** | 9,8% | 22,0% | 0,4% | 35,2% |
| **aproximando** | 24,1% | **27,1%** | 0% | 10,5% | 38,3% |
| **livre** | 0% | 0% | **0%** | 0% | 100% |
| **proximo_parado** | 0% | 10,5% | 0% | **34,6%** | 54,9% |
| **F1 score** | 0,45 | 0,34 | 0,00 | 0,47 | — |

A classe com **melhor resultado no teste foi `proximo_parado`** (F1 0,47) e a de **pior resultado foi `livre`** (F1 0,00, com todas as amostras como *uncertain*).

* **Análise dos resultados:**
  * Na validação, `livre` (99,4%) e `proximo_parado` (85,1%) são bem reconhecidas, pois têm distância quase constante e em faixas bem diferentes.
  * `aproximando` e `afastando` são muito confundidas entre si. Média, mínimo e máximo não dependem da ordem das leituras, e uma janela com o objeto se aproximando tem os mesmos valores de uma com ele se afastando, em ordem invertida.
  * Em `afastando`, a distância termina saturada em 100 cm, e essas janelas ficam parecidas com `livre` (38% de confusão na validação).
  * A acurácia no teste (25,38%) ficou bem abaixo da validação (64,7%). Grande parte vem das janelas *uncertain*, que contam como erro. A validação também usa janelas sobrepostas dos mesmos arquivos do treino, o que a deixa otimista. A ROC de 0,83 indica que o modelo separa as classes melhor do que a acurácia sugere.

### 🔗 Integração no ESP32

1. No Edge Impulse: *Deployment → Arduino library*, com otimização **Quantized (int8)**.
2. Na Arduino IDE: *Sketch → Include Library → Add .ZIP Library*.
3. O firmware `firmware/carrinho_ia/carrinho_ia.ino` combina o controle Bluetooth (Dabble) com a inferência:
   * amostra a distância a cada 100 ms (`millis()`, sem `delay`) e mantém uma janela deslizante de 2 s;
   * executa `run_classifier()` a cada 0,5 s;
   * se a classe for `aproximando` ou `proximo_parado` com confiança de pelo menos 70%, **bloqueia o comando "frente"**;
   * ao tentar avançar bloqueado, o carrinho executa uma **manobra autônoma**: ré por 0,7 s e giro por 0,5 s, alternando esquerda e direita a cada manobra. Depois, limpa a janela do modelo, para e devolve os controles. O "frente" só volta a funcionar depois de o botão ser solto e apertado de novo;
   * mantém uma parada de emergência a 8 cm, independente do modelo.
4. **Configuração da Arduino IDE:**
   * core **esp32 2.0.14** (Espressif Systems);
   * biblioteca **DabbleESP32 1.5.x** (versões 1.6.x exigem o core 3.x, incompatível com o restante do projeto);
   * placa **ESP32 Dev Module**;
   * **Partition Scheme: Huge APP (3MB No OTA/1MB SPIFFS)**, necessário porque o modelo, o Bluetooth e o Dabble juntos ultrapassam a partição padrão.

### ⚠️ Limitações e melhorias

* A acurácia atual (64,7% na validação e 25,38% no teste) é limitada principalmente pelo bloco Flatten, que não preserva a direção do movimento.
* **Melhoria prioritária:** trocar o Flatten por **Raw data**, ou acrescentar uma feature de tendência (inclinação da distância na janela).
* Aumentar e diversificar o dataset (mais objetos, velocidades e coletas com o carrinho em movimento).
* A ré é feita sem sensor traseiro, então a manobra deve ser usada com espaço livre atrás do carrinho.
* A parada de emergência por distância foi mantida por segurança, já que o modelo pode errar.

---

## 🧪 Testes e resultados


**Resumo:** montamos as peças, codamos, testamos e o carrinho funcionou como esperado.

---

## 🎬 Evidências finais

* 📷 **Fotos do carrinho finalizado:**
<img width="3024" height="4032" alt="Image" src="https://github.com/user-attachments/assets/6dcb0c0e-8455-4fa4-af98-3e211c9a2363" />
<img width="3024" height="4032" alt="Image (1)" src="https://github.com/user-attachments/assets/d5d2d46f-9073-4255-b777-d2407a25de92" />
<img width="3024" height="4032" alt="Image (2)" src="https://github.com/user-attachments/assets/207f3182-05af-4fbb-8296-d0d24e22d4bb" />
<img width="3024" height="4032" alt="Image (3)" src="https://github.com/user-attachments/assets/13176556-652d-41bc-a9f9-bde6f775c3c9" />
