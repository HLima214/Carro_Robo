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
#define INTERVALO_MS 100
#define DISTANCIA_EMERGENCIA_CM 8

// ---- IA ----
#define CONFIANCA_MINIMA 0.70
#define CLASSIFICAR_A_CADA 5

// ---- NOVO: manobra autônoma ----
#define TEMPO_RE_MS 700
#define TEMPO_VIRAR_MS 500

enum EstadoManobra { MANOBRA_NENHUMA, MANOBRA_RE, MANOBRA_VIRAR };  

static float features[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];
int amostrasColetadas = 0;
int contadorClassificacao = 0;
bool bloquearFrente = false;
String classeAtual = "";

EstadoManobra estadoManobra = MANOBRA_NENHUMA;  
unsigned long inicioManobra = 0;                
bool virarParaEsquerda = true;                  
bool aguardarSoltarFrente = false;              

void moveForward();
void moveBackward();
void turnLeft();
void turnRight();
void stopMotors();
float medirDistanciaCm();
void classificar();
void iniciarManobra();    
void executarManobra();   

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
    bool frenteBloqueada = bloquearFrente || emergencia;

    //manobra em andamento -> ignora o GamePad até terminar
    if (estadoManobra != MANOBRA_NENHUMA) {
        executarManobra();
        return;
    }

    //após a manobra, exige soltar o botão "frente" antes de voltar a andar
    if (aguardarSoltarFrente && !GamePad.isUpPressed()) {
        aguardarSoltarFrente = false;
    }

    // 4) Controle
    if (GamePad.isUpPressed()) {
        if (aguardarSoltarFrente) {
            stopMotors();
        }
        else if (frenteBloqueada) {
            iniciarManobra();   //em vez de só parar, inicia a manobra
        }
        else {
            moveForward();
        }
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


void iniciarManobra() {
    estadoManobra = MANOBRA_RE;
    inicioManobra = millis();
    moveBackward();
    Serial.println("Manobra: dando re");
}


void executarManobra() {
    unsigned long decorrido = millis() - inicioManobra;

    if (estadoManobra == MANOBRA_RE) {
        if (decorrido >= TEMPO_RE_MS) {
            estadoManobra = MANOBRA_VIRAR;
            inicioManobra = millis();
            if (virarParaEsquerda) turnLeft(); else turnRight();
            virarParaEsquerda = !virarParaEsquerda;  // alterna o lado a cada manobra
            Serial.println("Manobra: virando");
        }
    }
    else if (estadoManobra == MANOBRA_VIRAR) {
        if (decorrido >= TEMPO_VIRAR_MS) {
            stopMotors();
            estadoManobra = MANOBRA_NENHUMA;

            // Descarta a janela antiga (ela ainda diz "obstáculo") e recomeça a coletar
            amostrasColetadas = 0;
            contadorClassificacao = 0;
            bloquearFrente = false;
            classeAtual = "";

            aguardarSoltarFrente = true;
            Serial.println("Manobra concluida: controles liberados");
        }
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