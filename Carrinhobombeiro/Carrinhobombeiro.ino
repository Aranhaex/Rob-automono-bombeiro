#include <Servo.h> // Biblioteca para controlar o servo motor

// --- PINOS DOS SENSORES E ATUADORES ---
#define TRIG 12
#define ECHO A1
#define SERVO_PIN 2
#define BOMB_PIN A2
#define FLAME_SENSOR_PIN A0 // Pino analógico do sensor de chamas
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

// Função que retorna true se uma chama for detectada
bool isFlameDetected() {
  int flameValue = analogRead(FLAME_SENSOR_PIN);
  
  Serial.print(" | Leitura da Chama: ");
  Serial.print(flameValue);

  if (flameValue < FLAME_THRESHOLD) {
    return true; // Chama detectada!
  } else {
    return false; // Nenhuma chama detectada.
  }
}

// Nova função que combate o fogo continuamente
void combaterFogo() {
  Serial.println(" | *** FOGO DETECTADO! INICIANDO COMBATE... ***");
  digitalWrite(BOMB_PIN, LOW); // Liga a bomba
  
  
  // Este laço continua ENQUANTO a chama for detectada
  while (isFlameDetected()) {
    Serial.println(" | Jorrando agua e movendo o servo...");
  
    // Move o servo de 0 para 180 graus, verificando a chama em cada passo
    for (int pos = 70; pos <= 120; pos += 2) {
      myServo.write(pos);
      acionarSirene();
      if (!isFlameDetected()) break; // Se o fogo apagar no meio do caminho, sai do laço
      delay(30); 
    }
     
    // Se o fogo ainda estiver aceso, volta de 180 para 0 graus
    if (isFlameDetected()) {
      for (int pos = 120; pos >= 70; pos -= 2) {
        myServo.write(pos);
        acionarSirene();
        if (!isFlameDetected()) break; // Se o fogo apagar, sai do laço
        delay(30);

      }
    }
   noTone(BUZZER);
  }
  
  // Quando o laço 'while' terminar, significa que o fogo apagou
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