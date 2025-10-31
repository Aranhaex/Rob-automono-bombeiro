// Controle de dois motores DC com ESP32 via teclado (Monitor Serial)
// Motor A -> D13 e D12
// Motor B -> D14 e D27

const int motorA1 = 13;
const int motorA2 = 12;
const int motorB1 = 14;
const int motorB2 = 27;

void setup() {
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);

  Serial.begin(115200);
  Serial.println("Controle de motores pronto!");
  Serial.println("Digite:");
  Serial.println("1 - Motor A frente");
  Serial.println("2 - Motor A trás");
  Serial.println("3 - Motor B frente");
  Serial.println("4 - Motor B trás");
}

void loop() {
  // Verifica se há dados disponíveis no monitor serial
  if (Serial.available() > 0) {
    char comando = Serial.read(); // lê o caractere enviado

    switch (comando) {
      case '1':
        Serial.println("Motor A -> Frente");
        moverFrenteA();
        delay(1000);
        pararA();
        break;

      case '2':
        Serial.println("Motor A -> Trás");
        moverTrasA();
        delay(1000);
        pararA();
        break;

      case '3':
        Serial.println("Motor B -> Frente");
        moverFrenteB();
        delay(1000);
        pararB();
        break;

      case '4':
        Serial.println("Motor B -> Trás");
        moverTrasB();
        delay(1000);
        pararB();
        break;

      default:
        Serial.println("Comando inválido. Use 1, 2, 3 ou 4.");
        break;
    }
  }
}

// ===== Funções para Motor A =====
void moverFrenteA() {
  digitalWrite(motorA1, HIGH);
  digitalWrite(motorA2, LOW);
}

void moverTrasA() {
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, HIGH);
}

void pararA() {
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, LOW);
}

// ===== Funções para Motor B =====
void moverFrenteB() {
  digitalWrite(motorB1, HIGH);
  digitalWrite(motorB2, LOW);
}

void moverTrasB() {
  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, HIGH);
}

void pararB() {
  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, LOW);
}
