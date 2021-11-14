#define ENABLE 8
#define X_STEP 2
#define Y_STEP 3
#define X_DIR  5
#define Y_DIR  6

#define LASER_PWR A3

#define CALL_TIME 50

#define STEPS 20

bool go;
int go2;

void setup() {
  pinMode(X_STEP, OUTPUT);
  pinMode(Y_STEP, OUTPUT);
  pinMode(X_DIR,  OUTPUT);
  pinMode(Y_DIR,  OUTPUT);
  pinMode(ENABLE, OUTPUT);
  // povoleni rizeni pro vsechny drivery
  digitalWrite(ENABLE, LOW);
  /*
  int prev = 0;
  do {
    prev = analogRead(LASER_PWR);
    pohybOsy(false, X_DIR, X_STEP, 10);
    delay(CALL_TIME);
  } while(prev != analogRead(LASER_PWR));
  do {
    prev = analogRead(LASER_PWR);
    pohybOsy(false, Y_DIR, Y_STEP, 10);
    delay(CALL_TIME);
  } while(prev != analogRead(LASER_PWR));
*/
  // Zahajime komunikaci s pocitacem na prenosove rychlost 115200 baudu, toto cislo je nutne take zvolit kdyz otevirame seriovy monitor.
  Serial.begin(115200);
  Serial.write(0);
  go = true;
  go2 = 1;
}

void loop() {
  if (go) {
  int a = analogRead(LASER_PWR);
  pohybOsy(true, X_DIR, X_STEP, go2*STEPS);
  delay(CALL_TIME);
  int b = analogRead(LASER_PWR);
  pohybOsy(true, Y_DIR, Y_STEP, go2*STEPS);
  delay(CALL_TIME);
  int c = analogRead(LASER_PWR);
  if (a == c){
     if (go2 == 1){
        go2 = -1;
        sent(10000);
     }
     else{
        go = false;
     }
  }
  else if (go2 == 1){
    sent(a+c-2*b);
  }
  }
}

void sent(int in){
  char sending[] = {in, in>>8};
  Serial.write(sending);
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
