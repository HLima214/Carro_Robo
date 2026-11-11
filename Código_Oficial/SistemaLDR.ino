#define LDR_PIN 34
#define BUZZER_PIN 19
#define LED_PIN 18

#define LDR_INVERTIDO 1      // o seu módulo sobe no escuro
#define LIMIAR_ESCURO 2000   // abaixo disso = pouca luz (LED + buzzer)
#define HISTERESE 150        // evita ligar e desligar no limite

#define BUZZER_ON  HIGH      // troque para LOW se o seu módulo apitar com nível baixo
#define BUZZER_OFF LOW

bool poucaLuz = false;

void setup() {
    Serial.begin(115200);
    pinMode(LDR_PIN, INPUT);

    digitalWrite(BUZZER_PIN, BUZZER_OFF);
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    pinMode(LED_PIN, OUTPUT);

    // Teste na partida: LED aceso e um bip curto
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, BUZZER_ON);
    delay(150);
    digitalWrite(BUZZER_PIN, BUZZER_OFF);
    delay(150);
    digitalWrite(LED_PIN, LOW);

    Serial.println("Teste LDR + buzzer + LED vermelho");
}

void loop() {
    // Média de 16 leituras para reduzir ruído (0 a 4095)
    long soma = 0;
    for (int i = 0; i < 16; i++) {
        soma += analogRead(LDR_PIN);
        delay(2);
    }
    int bruto = soma / 16;
    int luz = LDR_INVERTIDO ? 4095 - bruto : bruto;   // quanto maior, mais claro

    // Decide se está com pouca luz (com histerese)
    if (!poucaLuz && luz < LIMIAR_ESCURO) {
        poucaLuz = true;
    } else if (poucaLuz && luz > LIMIAR_ESCURO + HISTERESE) {
        poucaLuz = false;
    }

    // LED vermelho: aceso enquanto estiver com pouca luz
    digitalWrite(LED_PIN, poucaLuz ? HIGH : LOW);

    // Buzzer: apita (150 ms ligado a cada 400 ms) enquanto estiver com pouca luz
    if (poucaLuz) {
        digitalWrite(BUZZER_PIN, (millis() % 400) < 150 ? BUZZER_ON : BUZZER_OFF);
    } else {
        digitalWrite(BUZZER_PIN, BUZZER_OFF);
    }

    // Mostra no Serial Monitor a cada 300 ms
    static unsigned long ultimoPrint = 0;
    if (millis() - ultimoPrint >= 300) {
        ultimoPrint = millis();
        Serial.printf("Bruto: %4d | Luz: %4d | %s\n",
                      bruto, luz, poucaLuz ? "POUCA LUZ (LED + buzzer)" : "claro");
    }
}