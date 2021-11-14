#define ENABLE 8

#define X_STEP 2
#define Y_STEP 3

#define X_DIR  5
#define Y_DIR  6

#define LASER_PWR A3

int cas = 0;
int mam = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(X_STEP, OUTPUT);
  pinMode(Y_STEP, OUTPUT);
  pinMode(X_DIR,  OUTPUT);
  pinMode(Y_DIR,  OUTPUT);
  pinMode(ENABLE, OUTPUT);
  // povoleni rizeni pro vsechny drivery
  digitalWrite(ENABLE, LOW);

  /*int prev = 0;
  do {
    prev = analogRead(LASER_PWR);
    pohybOsy(false, X_DIR, X_STEP, 10);
  } while(prev != analogRead(LASER_PWR));
  do {
    prev = analogRead(LASER_PWR);
    pohybOsy(false, Y_DIR, Y_STEP, 10);
  } while(prev != analogRead(LASER_PWR));*/
  // Zahajime komunikaci s pocitacem na prenosove rychlost 115200 baudu, toto cislo je nutne take zvolit kdyz otevirame seriovy monitor.
  Serial.begin(115200);
  Serial.write(0);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (mam < 10){
    if (cas == analogRead(LASER_PWR)){
      Serial.write(micros());
      pohybOsy(true, X_DIR, X_STEP, 80);
      mam++;
    }
    cas = analogRead(LASER_PWR);
  }
  delayMicroseconds(50);
}

void pohybOsy(boolean smer, byte dirPin, byte stepPin, int kroky) {
  // zapis smeru na prislusny pin DIR
  digitalWrite (dirPin, smer);
  delay(50);
  // smycka pro provedeni predaneho mnozstvi kroku
  for (int i = 0; i < kroky; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds (800);
    digitalWrite(stepPin, LOW);
    delayMicroseconds (800);
  }
}
