

const int motorA1 = 13;
const int motorA2 = 12;
const int motorB1 = 14;
const int motorB2 = 27;
const int motorC1 = 26;
const int motorC2 = 25;

void setup() {
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);
  pinMode(motorC1, OUTPUT);
  pinMode(motorC2, OUTPUT);

  Serial.begin(115200);


  Serial.println("Controle de motores pronto!");
  Serial.println("Digite:");
  Serial.println("1 - Motor A frente");
  Serial.println("2 - Motor A trás");
  Serial.println("3 - Motor B frente");
  Serial.println("4 - Motor B trás");
  Serial.println("5 - Motor C frente");
  Serial.println("6 - Motor C trás");
}

void loop() {
  // Verifica se há dados disponíveis no monitor serial
  if (Serial.available() > 0) {
    char comando = Serial.read(); // lê o caractere enviado

    switch (comando) {
      case '1':
        Serial.println("Motor A -> Frente");
        moverFrenteA();
        delay(125);
        pararA();
        break;

      case '2':
        Serial.println("Motor A -> Trás");
        moverTrasA();
        delay(125);
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

        case '5':
        Serial.println("Motor C -> Frente");
        moverFrenteC();
        delay(1000);
        pararC();
        break;

      case '6':
        Serial.println("Motor C -> Trás");
        moverTrasC();
        delay(1000);
        pararC();
        break;

      default:
        Serial.println("Comando inválido. Use 1, 2, 3, 4, 5 ou 6.");
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

void moverFrenteC() {
  digitalWrite(motorC1, HIGH);
  digitalWrite(motorC2, LOW);
}

void moverTrasC() {
  digitalWrite(motorC1, LOW);
  digitalWrite(motorC2, HIGH);
}

void pararC() {
  digitalWrite(motorC1, LOW);
  digitalWrite(motorC2, LOW);
}
