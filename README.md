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

---

## ⚡ Principais funcionalidades

* 🕹️ Movimentação para frente e para os lados (curvas)
* 📡 Detecção de obstáculos/movimento via sensor ultrassônico
* 📱 Controle remoto via aplicativo de celular (Dabble), conectado por Bluetooth ao ESP32

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

## 🛠️ Projeto mecânico e fabricação

* **Chassi:**
* Peças montadas manualmente pela equipe
* Motores soldados manualmente aos cabos de ligação
* **Carenagem:**

---

## 🔌 Hardware e eletrônica

* **Microcontrolador:** ESP32
* **Ponte H:**
* **Sensor:** ultrassônico HC-SR04 (detecção de obstáculos)
* **Alimentação:** baterias de lítio 18650 / power bank
* **Comunicação sem fio:** Bluetooth, via app Dabble no celular (ESP32 dispensa módulo HC-05/HM-10 separado)
* **Diagrama de conexões:** ver `Diagrama.ligacoes.png`

---

## 💻 Software

Código-fonte completo em `CodigoCarrinho.ino`.


* **Controle dos motores:**
* **Comunicação sem fio (Bluetooth/Dabble):**
* **Uso do sensor ultrassônico:**

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





