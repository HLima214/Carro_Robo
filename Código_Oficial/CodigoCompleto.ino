#include <DabbleESP32.h>
#include <Hlimas-project-1_inferencing.h>

// ---- Motores ----
#define IN1 27
#define IN2 26
#define IN3 25
#define IN4 33

// ---- Sensor ----
#define TRIG_PIN 14
#define ECHO_PIN 12
#define DIST_MAX_CM 100
#define INTERVALO_MS 100          // tem que ser igual à coleta (10 Hz)
#define DISTANCIA_EMERGENCIA_CM 8 // segurança extra, independente da IA

// ---- IA ----
#define CONFIANCA_MINIMA 0.70
#define CLASSIFICAR_A_CADA 5      // classifica a cada 5 amostras (0,5 s)

static float features[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];
int amostrasColetadas = 0;
int contadorClassificacao = 0;
bool bloquearFrente = false;
String classeAtual = "";

void moveForward();
void moveBackward();
void turnLeft();
void turnRight();
void stopMotors();
float medirDistanciaCm();
void classificar();

int ei_get_data(size_t offset, size_t length, float *out_ptr) {
    memcpy(out_ptr, features + offset, length * sizeof(float));
    return 0;
}

void setup() {
    Serial.begin(115200);

    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    Dabble.begin("HAREP");
}

void loop() {
    Dabble.processInput();

    // 1) Amostragem não bloqueante a 10 Hz com janela deslizante
    static unsigned long ultimo = 0;
    static float distanciaAtual = DIST_MAX_CM;

    if (millis() - ultimo >= INTERVALO_MS) {
        ultimo = millis();
        distanciaAtual = medirDistanciaCm();

        if (amostrasColetadas < EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE) {
            features[amostrasColetadas++] = distanciaAtual;
        } else {
            memmove(features, features + 1,
                    (EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE - 1) * sizeof(float));
            features[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE - 1] = distanciaAtual;
        }

        // 2) Janela cheia: classifica periodicamente
        if (amostrasColetadas == EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE) {
            contadorClassificacao++;
            if (contadorClassificacao >= CLASSIFICAR_A_CADA) {
                contadorClassificacao = 0;
                classificar();
            }
        }
    }

    // 3) Segurança: parada de emergência por distância
    bool emergencia = (distanciaAtual <= DISTANCIA_EMERGENCIA_CM);

    // 4) Controle: o resultado da IA interfere no comportamento
    bool frenteBloqueada = bloquearFrente || emergencia;

    if (GamePad.isUpPressed()) {
        if (frenteBloqueada) stopMotors();
        else moveForward();
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

void classificar() {
    signal_t signal;
    signal.total_length = EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE;
    signal.get_data = &ei_get_data;

    ei_impulse_result_t result;
    EI_IMPULSE_ERROR erro = run_classifier(&signal, &result, false);
    if (erro != EI_IMPULSE_OK) {
        Serial.printf("Erro na inferencia: %d\n", erro);
        return;
    }

    float melhorValor = 0;
    String melhorClasse = "";
    for (size_t i = 0; i < EI_CLASSIFIER_LABEL_COUNT; i++) {
        if (result.classification[i].value > melhorValor) {
            melhorValor = result.classification[i].value;
            melhorClasse = result.classification[i].label;
        }
    }

    if (melhorValor >= CONFIANCA_MINIMA) {
        classeAtual = melhorClasse;
        bloquearFrente = (classeAtual == "aproximando" ||
                          classeAtual == "proximo_parado");
    }

    Serial.printf("Classe: %s (%.2f) | frente bloqueada: %d\n",
                  classeAtual.c_str(), melhorValor, bloquearFrente);
}

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

void moveForward() {
    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void moveBackward() {
    digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
}

void turnLeft() {
    digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void turnRight() {
    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
}

void stopMotors() {
    digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}