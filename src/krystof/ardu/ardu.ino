#include <math.h>
#include <MsgPack.h>

#define ENABLE 8
#define X_STEP 2
#define Y_STEP 3
#define X_DIR  5
#define Y_DIR  6
#define CALL_TIME 50

// pin na ktery je privedena analogova hodnota z mereni vykonu laseru
// rozsah na merce je nastaven jako 0 az 30uW (mikro wattu)
#define LASER_PWR A3
#define send(x) {\
  MsgPack::Packer packer; \
  packer.serialize(x); \
  unsigned int len = packer.size(); \
  Serial.write((byte*)&len, 2);\
  Serial.write(packer.data(), packer.size());\
}

#define derive_recieve(x) x recieve_ ## x(){\
  while(!Serial.available());
  byte _my_buffer_intern[2] = { }; \
  Serial.readBytes(_my_buffer_intern, 2);\
  MsgPack::Unpacker unpacker; \
  byte* _my_buffer_intern1 = new byte[*(unsigned*)&_my_buffer_intern];\
  Serial.readBytes(_my_buffer_intern1, *(unsigned*)&_my_buffer_intern);\
  unpacker.feed(_my_buffer_intern1, *(unsigned*)&_my_buffer_intern);\
  x neco_neco_neco_jinyho;\
  unpacker.deserialize(neco_neco_neco_jinyho);\
  return neco_neco_neco_jinyho;\
}

derive_recieve(byte);
derive_recieve(uint8_t);

#define send_array(x, y) for(int i = 0; i < (y); i++){send(x);while(!Serial.available());Serial.read();}

derive_recieve(int)

struct time_n_place_stamp{
  unsigned long time;
  long long place;
  float a;
  MSGPACK_DEFINE(time, place, a);
};

void setup() {
  // nastavení smeru pro vsechny piny
  pinMode(X_STEP, OUTPUT);
  pinMode(Y_STEP, OUTPUT);
  pinMode(X_DIR,  OUTPUT);
  pinMode(Y_DIR,  OUTPUT);
  pinMode(ENABLE, OUTPUT);
  // povoleni rizeni pro vsechny drivery
  digitalWrite(ENABLE, LOW);

  int neco;

  do{
    delay(1);
    neco = anPwr();
    delay(1);
    pohybX(true, 20);
  }while(neco != anPwr());
 
  do{
    delay(1);
    neco = anPwr();
    delay(1);
    pohybY(true, 20);
  }while(neco != anPwr());

  // Zahajime komunikaci s pocitacem na prenosove rychlost 115200 baudu, toto cislo je nutne take zvolit kdyz otevirame seriovy monitor.
  Serial.begin(115200);
  
  Serial.write(0);
  
}

void loop(){
  if (Serial.available()){
    int n = recieve_int();
    if (n == -1){
      pohybX(true, 1000);
      return;
    }
    while (!Serial.available());
    int dt = recieve_int();
    while (!Serial.available());
    int dx = recieve_int();
    measure(n, dt, dx);
  }
}

void pohybOsy(bool smer, byte dirPin, byte stepPin, int kroky) {
  // zapis smeru na prislusny pin DIR
  digitalWrite(dirPin, smer);
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

void pohybX(bool smer, int vzdalenost){ //vzdalenost v um
  pohybOsy(smer, X_DIR, X_STEP, (int)(vzdalenost >> 1));
}

void pohybY(bool smer, int vzdalenost){ //vzdalenost v um
  pohybOsy(smer, Y_DIR, Y_STEP, (int)(vzdalenost >> 1));
}

int anPwr(){
  return analogRead(LASER_PWR);
}

double measurePower(int n, int dt){  //vypočítá průměr z n měření za čas n * dt
  unsigned long sum = 0;
  unsigned long time = 0;
  for (int i = 0; i < n; i++){
    time = micros();
    sum += anPwr();
    delayMicroseconds(dt-micros()+time-12);
  }
  return sum / (double)n;
}
struct neco_je{
  float a_0;
  float n_0;
  float s_0;
};

struct neco_je getPeriod(int n, int dt){
  unsigned* data = new unsigned[n];
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
  unsigned currentMin = 10000;
  unsigned previousMax;
  unsigned neco1 = 0;
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
        previousMax = currentMax;
        currentMax = 0;
        count++;
      }
    }
    else if (val > currentMax){
      currentMax = val;
      currentMaxT = t;
    }
    if (val > mean){
      if (currentMin != 10000){
        neco1 += previousMax - currentMin;
      }
    }
    else if (val < currentMin){
      currentMin = val;
    }
  }
  if (firstMaxT == lastMaxT){
    Serial.write("not enough data");
  }

  float period = (float)dt*((float)lastMaxT - (float)firstMaxT) / (float)(count - 2);
  delete[] data;
  return {(float)neco1/(float)(count-2),2*PI/(float)period,fmod((float)time-(float)dt*(float)(n-lastMaxT), (float)period)*(float)period/2/PI};
}

void measure(int n, int dt, int dx){
  neco_je period = getPeriod(800, dt);
  send(period.a_0);
  send(period.n_0);
  send(period.s_0);

  for (int x = 0; x < n; x++){
    unsigned long time = micros();
    time_n_place_stamp a = {time, x*dx,(float)anPwr()};
    send(a);
    pohybX(false, dx);
    delayMicroseconds(dt+time-micros()-12);
  }
  stop_transfer();
}

void stop_transfer(){
  time_n_place_stamp stop = {0,0,10000.0};
  send(stop);
}