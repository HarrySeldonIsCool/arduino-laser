#define ENABLE 8
#define X_STEP 2
#define Y_STEP 3
#define X_DIR  5
#define Y_DIR  6
#define CALL_TIME 50

// pin na ktery je privedena analogova hodnota z mereni vykonu laseru
// rozsah na merce je nastaven jako 0 az 30uW (mikro wattu)
#define LASER_PWR A3

void setup() {
  // nastavení smeru pro vsechny piny
  pinMode(X_STEP, OUTPUT);
  pinMode(Y_STEP, OUTPUT);
  pinMode(X_DIR,  OUTPUT);
  pinMode(Y_DIR,  OUTPUT);
  pinMode(ENABLE, OUTPUT);
  // povoleni rizeni pro vsechny drivery
  digitalWrite(ENABLE, LOW);

  // Zahajime komunikaci s pocitacem na prenosove rychlost 115200 baudu, toto cislo je nutne take zvolit kdyz otevirame seriovy monitor.
  Serial.begin(115200);
  
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
  Serial.write(0);
  
}

void loop(){
  if (Serial.available()){
  int n = *(int*)Serial.readString().c_str();
  if (n == -1){
    pohybX(true, 1000);
    return;
  }
  while (!Serial.available());
  int dt = *(int*)Serial.readString().c_str();
  while (!Serial.available());
  int dx = *(int*)Serial.readString().c_str();
  measure(n, dt, dx);
  }
}

void pohybOsy(boolean smer, byte dirPin, byte stepPin, int kroky) {
  // zapis smeru na prislusny pin DIR
  digitalWrite (dirPin, smer);
  delay(50);
  // smycka pro provedeni predaneho mnozstvíi kroku
  for (int i = 0; i < kroky; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds (800);
    digitalWrite(stepPin, LOW);
    delayMicroseconds (800);
  }
}
//odsud nahoru je zkopirovane z ukazkoveho kodu

void pohybX(boolean smer, int vzdalenost){ //vzdalenost v um
  pohybOsy(smer, X_DIR, X_STEP, (int)(vzdalenost/2));
}

int anPwr(){
  return analogRead(LASER_PWR);
}

double measurePower(int n, int dt){  //vypočítá pruůměr z n měření za čas n * dt
  unsigned long sum = 0;
  unsigned long time = 0;
  for (int i = 0; i < n; i++){
    time = micros();
    sum += anPwr();
    delayMicroseconds(dt+time-micros()-12);
  }
  return sum / (double)n;
}

int getPeriod(int n, int dt){ //vrátí ((doba jednoho kmitu)/dt), tedy kolikrat zmerit vykon s rozdilem casu dt
//je zvlastni, ze pro ruzna dt je (getPeriod(n, dt) * dt)(, coz je vypocitana doba kmitu) ruzna, asi kvuli pomalosti arduina
//taky je zvlastni, ze prvni 3 az pet mereni vykonu funkci measurePower je vykon vzdycky mensi
  unsigned* data = new unsigned[n]; //naměřený výkon
  float mean = 0; //průměr
  unsigned long time = 0;
  for (int t = 0; t < n; t++){
    time = micros();
    data[t] = anPwr();
    mean += data[t];
    delayMicroseconds(dt+time-micros()-12);
  }

  mean /= (float)n;

  unsigned currentMax = 0; //nejvyšší hodnota při daném kmitu
  unsigned currentMaxT; //při kolikátém měření naměřil nejvyšší hodnotu při současném kmitu kmitu
  unsigned firstMaxT; //při kolikátém měření naměřil nejvyšší hodnotu při prvním kmitu
  unsigned lastMaxT; //při kolikátém měření naměřil nejvyšší hodnotu při posledním kmitu
  unsigned count = 0; //počet kmitů
  for (int t = 0; t < n; t++){
    unsigned val = data[t];
    if (val < mean){
      if (currentMax != 0){ //pokud data[t - 1] > průměr
        if (count == 1){ // pokud už výkon jednou klesl pod průměr, zaručuje, že výkon při firstMaxT'tém měření je nejvyšší výkon při kmitu
          firstMaxT = currentMaxT;
        }
        lastMaxT = currentMaxT;
        currentMax = 0;
        count++;
      }
    }
    else if (val > currentMax){
      currentMax = val;
      currentMaxT = t;
    }
  }
  if (firstMaxT == lastMaxT){
    Serial.write("not enough data");
  }
  /*Serial.write((char*)&(count-2));
  Serial.write((char*)&firstMaxT);
  Serial.write((char*)&lastMaxT);*/

  int period = dt*(lastMaxT - firstMaxT) / (count - 2);
  //Serial.write((char*)&period);
  delete[] data;
  return period;
}

void measure(int n, int dt, int dx){
  int period = getPeriod(800, dt);
  for (int x = 0; x < n; x++){
    double power = measurePower(period, dt);
    Serial.write((char*)&power);
    pohybX(false, dx);
  }
}

void stop_transmit(){
  Serial.write((char*)&10000.2);
}