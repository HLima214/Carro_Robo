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

<!-- TODO: 2-3 frases sobre o objetivo do projeto -->

Desenvolver um carrinho-robô funcional, controlado remotamente via smartphone e capaz de detectar obstáculos em seu trajeto, integrando os conhecimentos de projeto mecânico, eletrônica e programação trabalhados na disciplina.

---

## ⚡ Principais funcionalidades

* 🕹️ Movimentação para frente e para os lados (curvas)
* 📡 Detecção de obstáculos/movimento via sensor ultrassônico
* 📱 Controle remoto via aplicativo de celular (Dabble), conectado por Bluetooth ao ESP32

---

## 📸 Foto do carrinho finalizado

<!-- TODO: adicionar imagem final, ex: ![little mack](./docs/foto-final.jpg) -->

---

## 📂 Organização do repositório

| Arquivo/Pasta | Descrição |
| :--- | :--- |
| `CodigoCarrinho.ino` | Firmware do carrinho (ESP32) |
| `Diagrama.ligacoes.png` / `Diagrama.ligacoes.drawio.png` | Diagrama das conexões elétricas |
| `Documentacao_Tecnica_Carro_Robo.md` | Documentação técnica detalhada |
| `MicrosoftTeams-video.mp4` | Vídeo de teste do carrinho |
| `Organização/Precificação` | <!-- TODO: descrever o conteúdo desta pasta --> |

---

## 📋 Requisitos, planejamento e evolução

<!-- TODO: requisitos definidos, esboço/proposta inicial, MVP, backlog, principais decisões e mudanças ao longo do projeto -->

---

## 🛠️ Projeto mecânico e fabricação

* **Chassi:** <!-- TODO: qual kit foi comprado (2WD ou 4WD), link do fornecedor -->
* Peças montadas manualmente pela equipe
* Motores soldados manualmente aos cabos de ligação
* **Carenagem:** <!-- TODO: material e processo usado na carenagem -->

---

## 🔌 Hardware e eletrônica

* **Microcontrolador:** ESP32
* **Ponte H:** <!-- TODO: L298N ou L293D? -->
* **Sensor:** ultrassônico HC-SR04 (detecção de obstáculos)
* **Alimentação:** baterias de lítio 18650 / power bank
* **Comunicação sem fio:** Bluetooth, via app Dabble no celular (ESP32 dispensa módulo HC-05/HM-10 separado)
* **Diagrama de conexões:** ver `Diagrama.ligacoes.png`

---

## 💻 Software

Código-fonte completo em `CodigoCarrinho.ino`.

<!-- TODO: breve explicação de como o código funciona -->

* **Controle dos motores:**
* **Comunicação sem fio (Bluetooth/Dabble):**
* **Uso do sensor ultrassônico:**

---

## 🧪 Testes e resultados

| Teste | Esperado | Resultado | Ação |
| :--- | :--- | :--- | :--- |
| <!-- TODO --> | | | |

**Resumo:** montamos as peças, codamos, testamos e o carrinho funcionou como esperado.

---

## 🎬 Evidências finais

* 📷 **Fotos do carrinho finalizado:** <!-- TODO -->
* 🎥 **Vídeo de funcionamento:** `MicrosoftTeams-video.mp4`
* 📱 **Demonstração do controle remoto (app Dabble):** <!-- TODO -->
* 📏 **Demonstração do sensor:** <!-- TODO -->
* 🚀 **Instruções básicas de uso:** <!-- TODO -->
