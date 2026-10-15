#define TRIG_PIN 14
#define ECHO_PIN 12
#define DIST_MAX_CM 100
#define INTERVALO_MS 100   // 10 Hz

float medirDistanciaCm() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duracao = pulseIn(ECHO_PIN, HIGH, 30000);
    if (duracao == 0) return DIST_MAX_CM;

    float cm = duracao * 0.0343 / 2.0;
    if (cm > DIST_MAX_CM) cm = DIST_MAX_CM;
    return cm;
}

void setup() {
    Serial.begin(115200);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}

void loop() {
    static unsigned long ultimo = 0;
    if (millis() - ultimo >= INTERVALO_MS) {
        ultimo = millis();
        Serial.println(medirDistanciaCm());
    }
}