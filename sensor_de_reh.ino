
const int LED_VERMELHO = 11;
const int LED_AZUL = 10;
const int LED_VERDE = 9;

const int PIEZO = 8;
const int TRIG = 3;
const int ECHO = 2;

// Limites de distancia em centimetros
const int DISTANCIA_SEGURA = 50;
const int DISTANCIA_PERIGO = 20;

const int FREQUENCIA_SOM = 1200;

unsigned long ultimoBip = 0;
unsigned long ultimaLeitura = 0;
int intervaloBip = 0;

void setup() {
  Serial.begin(9600);

  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);

  pinMode(PIEZO, OUTPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  ledsOff();
}

void loop() {
  unsigned long agora = millis();

  // Atualiza a distancia a cada 70 ms
  if (agora - ultimaLeitura >= 70) {
    ultimaLeitura = agora;

    int distancia = sensor_morcego(TRIG, ECHO);

    Serial.print("Distancia: ");
    Serial.print(distancia);
    Serial.println(" cm");

    if (distancia == 0) {
      // Nenhuma leitura valida
      ledsOff();
      intervaloBip = 0;
      noTone(PIEZO);
    }
    else if (distancia > DISTANCIA_SEGURA) {
      // Distancia segura - Verde
      acendeVerde();
      intervaloBip = 0;
      noTone(PIEZO);
    }
    else if (distancia > DISTANCIA_PERIGO) {
      // Perto - Amarelo
      acendeAmarelo();
      intervaloBip = map(distancia, 21, 50, 150, 600);
    }
    else {
      // Muito perto - Vermelho
      acendeVermelho();
      intervaloBip = map(
        constrain(distancia, 2, 20),
        2, 20, 80, 300
      );
    }
  }

  // Controla o bip sem parar o programa
  if (intervaloBip > 0) {
    if (agora - ultimoBip >= intervaloBip) {
      tone(PIEZO, FREQUENCIA_SOM, 50);
      ultimoBip = agora;
    }
  }
}

// Medicao de distancia pelo ultrassom
int sensor_morcego(int pinotrig, int pinoecho) {
  digitalWrite(pinotrig, LOW);
  delayMicroseconds(2);

  digitalWrite(pinotrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinotrig, LOW);

  unsigned long duracao = pulseIn(pinoecho, HIGH, 25000);

  if (duracao == 0) {
    return 0;
  }

  return duracao / 58;
}

void ledsOff() {
  digitalWrite(LED_VERMELHO, LOW);
  digitalWrite(LED_AZUL, LOW);
  digitalWrite(LED_VERDE, LOW);
}

void acendeVermelho() {
  digitalWrite(LED_VERMELHO, HIGH);
  digitalWrite(LED_AZUL, LOW);
  digitalWrite(LED_VERDE, LOW);
}

void acendeVerde() {
  digitalWrite(LED_VERMELHO, LOW);
  digitalWrite(LED_AZUL, LOW);
  digitalWrite(LED_VERDE, HIGH);
}

void acendeAmarelo() {
  digitalWrite(LED_VERMELHO, HIGH);
  digitalWrite(LED_AZUL, LOW);
  digitalWrite(LED_VERDE, HIGH);
}
