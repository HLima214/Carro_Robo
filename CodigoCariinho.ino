#include <DabbleESP32.h>

#define IN1 27
#define IN2 26
#define IN3 25
#define IN4 33
#define ENA 5
#define ENB 32   // ajuste conforme sua ligação (6 não é um pino válido de saída no ESP32)

// SENSOR ULTRASSÔNICO
#define TRIG 18
#define ECHO 19

void moveForward();
void moveBackward();
void turnLeft();
void turnRight();
void stopMotors();

long getDistance();

void setup() {
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);

    // CONFIGURAÇÃO DO SENSOR
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);

    digitalWrite(ENA, HIGH); // habilita a ponte H (ou use PWM se quiser controlar velocidade)
    digitalWrite(ENB, HIGH);

    Dabble.begin("HAREP");
}

void loop() {
    Dabble.processInput();

    // Verifica a distância do sensor
    long distance = getDistance();

    // Se houver um obstáculo muito próximo, para o carro
    if (distance > 0 && distance <= 20) {
        stopMotors();
        return;
    }

    if (GamePad.isUpPressed()) {
        moveForward();
    }
    else if (GamePad.isDownPressed()) {
        moveBackward();
    }
    else if (GamePad.isLeftPressed()) {
        turnLeft();
    }
    else if (GamePad.isRightPressed()) {
        turnRight();
    }
    else {
        stopMotors();
    }
}

long getDistance() {

    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);

    digitalWrite(TRIG, LOW);

    long duration = pulseIn(ECHO, HIGH, 30000);

    long distance = duration * 0.034 / 2;

    return distance;
}

void moveForward() {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}

void moveBackward() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
}

void turnLeft() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}

void turnRight() {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
}

void stopMotors() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}
