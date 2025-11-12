#include <Servo.h> // Biblioteca para controlar o servo motor

// --- PINOS DOS SENSORES E ATUADORES ---
#define TRIG 12
#define ECHO A1
#define SERVO_PIN 2
#define BOMB_PIN A2
#define FLAME_LEFT_PIN A3
#define FLAME_CENTER_PIN A0 
#define FLAME_RIGHT_PIN A4
#define BUZZER 3
// --- LIMIAR DE DETECÇÃO DE CHAMA ---
// VALOR MENOR = MAIS SENSÍVEL. Ajuste conforme necessário.
#define FLAME_THRESHOLD 700 

// --- PINOS DOS MOTORES (PONTE H) ---
#define IN1 4
#define IN2 5
#define IN3 6
#define IN4 7
#define IN5 9
#define IN6 8
#define IN7 10
#define IN8 11

// Cria um objeto da classe Servo para controlar o motor
Servo myServo;  

void setup() {
  Serial.begin(9600);

  myServo.attach(SERVO_PIN); 
  myServo.write(90);         

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(IN5, OUTPUT);
  pinMode(IN6, OUTPUT);
  pinMode(IN7, OUTPUT);
  pinMode(IN8, OUTPUT);

  pinMode(BUZZER, OUTPUT);
  pinMode(BOMB_PIN, OUTPUT);
  digitalWrite(BOMB_PIN, HIGH);
}

Função que retorna true se UMA chama for detectada em qualquer sensor
bool isFlameDetected() {
  int leftValue = analogRead(FLAME_LEFT_PIN);
  int centerValue = analogRead(FLAME_CENTER_PIN);
  int rightValue = analogRead(FLAME_RIGHT_PIN);

  // Use a leitura do centro como principal para a detecção inicial
  if (centerValue < FLAME_THRESHOLD || leftValue < FLAME_THRESHOLD || rightValue < FLAME_THRESHOLD) {
    return true; 
  } else {
    return false; 
  }
}

// NOVO: Função para direcionar o servo para o foco principal
void focusOnFire() {
  // Lendo os valores dos 3 sensores
  int leftValue = analogRead(FLAME_LEFT_PIN);
  int centerValue = analogRead(FLAME_CENTER_PIN);
  int rightValue = analogRead(FLAME_RIGHT_PIN);
  
  // O sensor com o valor MAIS BAIXO é o que está mais próximo/direcionado para a chama
  
  // Se o fogo estiver à esquerda (leftValue é o menor de todos)
  if (leftValue <= centerValue && leftValue <= rightValue) {
    Serial.println("Foco na Esquerda!");
    // Ajuste este angulo (ex: 45 graus) para onde seu servo aponta para a esquerda
    myServo.write(45); 
  } 
  // Se o fogo estiver no centro (centerValue é o menor)
  else if (centerValue <= leftValue && centerValue <= rightValue) {
    Serial.println("Foco no Centro!");
    myServo.write(90); // Centro
  } 
  // Se o fogo estiver à direita (rightValue é o menor)
  else { // Fogo está à direita
    Serial.println("Foco na Direita!");
    // Ajuste este angulo (ex: 135 graus) para onde seu servo aponta para a direita
    myServo.write(135); 
  }
  
  // Dê tempo para o servo se mover antes de verificar novamente
  delay(100); 
}

// Função que combate o fogo continuamente (MODIFICADA)
void combaterFogo() {
  Serial.println(" | *** FOGO DETECTADO! INICIANDO COMBATE COM FOCO... ***");
  digitalWrite(BOMB_PIN, LOW); // Liga a bomba
  
  // Este laço continua ENQUANTO a chama for detectada
  while (isFlameDetected()) {
    focusOnFire(); // Continuamente foca o servo na direção certa
    acionarSirene();
  }
  
  // Quando o laço 'while' terminar, significa que o fogo apagou
  noTone(BUZZER);
  digitalWrite(BOMB_PIN, HIGH); // Desliga a bomba
  myServo.write(90);            // Retorna o servo para a posição central
  Serial.println(" | Fogo apagado! Acao finalizada.");
}


long getDistanceCM() {
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
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  digitalWrite(IN5, LOW);  digitalWrite(IN6, HIGH);
  digitalWrite(IN7, LOW);  digitalWrite(IN8, HIGH);
}

void moveBack() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  digitalWrite(IN5, HIGH);  digitalWrite(IN6, LOW);
  digitalWrite(IN7, HIGH);  digitalWrite(IN8, LOW);
  delay(250);
  stopMotors();
}

void stopMotors() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  digitalWrite(IN5, LOW); digitalWrite(IN6, LOW);
  digitalWrite(IN7, LOW); digitalWrite(IN8, LOW);
}

void turnRight90() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  digitalWrite(IN5, HIGH); digitalWrite(IN6, LOW);
  digitalWrite(IN7, HIGH); digitalWrite(IN8, LOW);
  delay(525);
  stopMotors();
}

// Variáveis para controle da sirene
int sireneFreq = 500;
int sireneStep = 50;
unsigned long lastSireneUpdate = 0;
const unsigned long sireneInterval = 10; // Intervalo entre passos da sirene (ms)

void acionarSirene() {
  unsigned long agora = millis();

  if (agora - lastSireneUpdate >= sireneInterval) {
    tone(BUZZER, sireneFreq);

    sireneFreq += sireneStep;

    // Inverte direção quando chega nos limites
    if (sireneFreq >= 2000 || sireneFreq <= 500) {
      sireneStep = -sireneStep;
    }

    lastSireneUpdate = agora;
  }
}


void loop() {
  long distance = getDistanceCM();
  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.print(" cm");

  bool fire = isFlameDetected();
  Serial.println(); // Pula uma linha para melhor leitura

  // CONDIÇÃO DE ATIVAÇÃO: Distância menor ou igual a 30cm E fogo detectado
  if (distance <= 50 && fire) {
    stopMotors();
    
    combaterFogo(); // Chama a nova função de combate contínuo
    
    // Apenas depois que o fogo for apagado, o robô desvia
    Serial.println("Girando para a direita apos apagar o fogo.");
    turnRight90();
  } 
  // CONDIÇÃO DE MOVIMENTO NORMAL: Distância maior que 40cm
  else if (distance > 40) {
    moveForward();
    digitalWrite(BOMB_PIN, HIGH); // Garante que a bomba está desligada
  }
  // CONDIÇÃO DE OBSTÁCULO SEM FOGO: Distância menor que 40cm mas sem fogo
  else if (distance < 20){
    stopMotors();
    Serial.println("Obstaculo muito próximo detectado, mas sem fogo. Desviando...");
    moveBack();
    turnRight90();
  }
  else {
    stopMotors();
    Serial.println("Obstaculo detectado, mas sem fogo. Desviando...");
    turnRight90();
  }

  delay(250);
}