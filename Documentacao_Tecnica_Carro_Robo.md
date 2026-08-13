## CHASSI – CARRO ROBÔ 4WD DOCUMENTAÇÃO TÉCNICA

Documento de requisitos e concepção mecânica

| Projeto | Carro robô com tração nas quatro rodas |
| --- | --- |
| Versão | 1.0 – conceito |
| Data | 13/08/2026 |
| Status | Projeto preliminar / pré-protótipo |

## 1. Objetivo

Este documento consolida os requisitos mecânicos e de integração definidos para o protótipo de um carro robô 4WD. O projeto utiliza quatro motores DC, cada um acoplado diretamente a uma roda, mantendo a região central livre para bateria, placa controladora, drivers e demais componentes eletrônicos.

## 2. Ficha de requisitos

| Requisito | Especificação |
| --- | --- |
| Tipo de veículo | Carro robô 4WD, tração independente nas quatro |
|   | rodas |
| Dimensões do chassi | 256 mm (comprimento) × 150 mm (largura) |
| Altura de referência | 65 mm – dimensão de envelope de projeto a validar |
|   | com rodas/motores |
| Quantidade de rodas | 4 |
| Diâmetro de roda | ≈ 60–70 mm; referência adotada: 65 mm |
| Quantidade de motores | 4 motores DC, um por roda |
| Tensão dos motores | 3–6 V DC |
| Controle dos motores | 4 canais independentes; mínimo de 2 pontes H |
|   | duplas |
| Driver recomendado | 2 × DRV8833 ou 2 × L298N |
| Placa controladora | Arduino Uno R3 ou ESP32; seleção final depende |
|   | dos demais requisitos |
| Alimentação | Bateria dimensionada para os 4 motores + eletrônica, |
|   | a definir após seleção final |
| Distribuição de peso | Bateria e eletrônica posicionadas próximas ao centro |
|   | geométrico |
| Carenagem | Cobertura superior removível, acompanhando o |
|   | perímetro do chassi |
| Acesso | Carenagem com fixação removível para manutenção |
|   | e troca da bateria |

## 3. Arquitetura física e posicionamento

A disposição segue o croqui fornecido: Motor 1 e Motor 2 ficam no lado esquerdo; Motor 3 e Motor 4 ficam no lado direito. Cada motor aciona diretamente a respectiva roda. A área central é reservada para a bateria e a eletrônica.

| Componente | Posição prevista |
| --- | --- |
| Roda 1 / Motor 1 | Canto dianteiro/esquerdo do chassi |
| Roda 2 / Motor 2 | Canto traseiro/esquerdo do chassi |
| Roda 3 / Motor 3 | Canto dianteiro/direito do chassi |


| Roda 4 / Motor 4 | Canto traseiro/direito do chassi |
| --- | --- |
| Bateria | Região central, preferencialmente baixa e próxima ao |
|   | centro geométrico |
| Placa controladora | Região central/superior, com acesso aos sinais e |
|   | alimentação |
| Drivers de motor | Próximos aos motores ou à controladora, mantendo |
|   | fiação curta e organizada |

## 4. Croqui do chassi – vista superior

O croqui abaixo representa a concepção atual do chassi, incluindo as quatro rodas, os quatro motores e o espaço central destinado à bateria/eletrônica.

## 5. Croqui do chassi – vista lateral


## 6. Carenagem / cobertura

- A carenagem deve ser uma cobertura superior removível, protegendo bateria, placa controladora, drivers e cabeamento.

- A cobertura deve acompanhar o envelope de 256 × 150 mm do chassi, sem interferir na rotação das rodas.

- Recomenda-se fixação por parafusos, insertos térmicos ou encaixes mecânicos, permitindo acesso rápido à bateria.

- Devem ser previstos recortes para interruptor, carregamento, USB/programação e eventual antena/sensor.

- A altura definitiva da carenagem deve ser definida depois que bateria, placa e drivers forem escolhidos, mantendo a altura global dentro do envelope de projeto.

## 7. Requisitos mecânicos

- A estrutura deve suportar o peso dos quatro motores, quatro rodas, bateria e eletrônica sem deformação significativa.

- Os pontos de fixação dos motores devem ser dimensionados de acordo com o modelo comercial escolhido; o STL atual possui suportes genéricos.

- As rodas podem ultrapassar levemente a largura de 150 mm do chassi, favorecendo a estabilidade e evitando contato da lateral da carenagem com o solo.

- O centro de massa deve permanecer próximo ao centro geométrico do veículo, com a bateria posicionada preferencialmente no centro e o mais baixa possível.

- Deve existir folga suficiente entre rodas, carenagem e chassi para evitar atrito durante curvas e irregularidades do piso.

## 8. Requisitos elétricos e de controle

- Os quatro motores DC devem ser comandados por quatro canais de potência.

- Para quatro motores independentes, recomenda-se utilizar duas pontes H duplas, como dois módulos DRV8833.

- O L298N pode ser utilizado como alternativa, porém apresenta perdas maiores e é menos eficiente para motores pequenos de 3–6 V.

- A placa controladora deve possuir GPIOs/PWM suficientes para os quatro canais de motor e para os sensores previstos.

- A alimentação dos motores deve ser separada/adequadamente desacoplada da lógica de controle, com aterramento comum e proteção contra ruído.

- A corrente máxima da bateria e dos drivers deve ser dimensionada pela corrente de partida/bloqueio dos motores, e não somente pela corrente nominal.

## 9. Critérios de validação do protótipo

- Dimensões externas conferidas: 256 × 150 mm.

- Altura/envelope conferidos após instalação de rodas e motores.

- Quatro motores instalados e alinhados.

- Quatro rodas livres de interferência mecânica.

- Bateria instalada na região central.

- Controladora e drivers instalados com acesso aos conectores.

- Carenagem removível sem desmontar motores ou rodas.

- Teste de tração simultânea das quatro rodas.

- Teste térmico dos drivers e motores em operação.


## 10. Observações de projeto

As dimensões apresentadas são de concepção e devem ser refinadas a partir dos componentes comerciais efetivamente selecionados. Em especial, os padrões de furação dos motores, dimensões da bateria, placa controladora e drivers ainda precisam ser definidos para que o chassi passe de um modelo conceitual para uma peça pronta para fabricação.

Documento preparado a partir do croqui e dos requisitos fornecidos para o protótipo.
