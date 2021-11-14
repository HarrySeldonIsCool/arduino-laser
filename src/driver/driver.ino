// Ukazkovy kod pro aparaturu online ukolu Science Challenge 2021
// Autor: Jakub Janousek
// pokud mate libovolne otazky ke kodu muzete napsat organizatorum email, nebojte se zeptat :)

// nastavení čísel projovacích pinů
// motor X ovlada britvu v pohybu doleva a doprava
// motor Y ovlada britvu v pohyu nahoru a dolu

// oba motory maji stejne parametry, maji 800 kroku na otacku, a jedna otacka pohne s britvou o 1,6 mm.
// pro ovladani krokovych motoru je vyuzito driveru A4988


// TODO:
// zmerit jak dlouho po posunuti je mozno zacit presne merit
// nechat si vypsat hodnoty pro 1600 poloh pripadne pro 640000 poloh
// zjistit, jak namerene hodnoty prevest na sirku svazku
// udelat GUI
#define ENABLE 8
#define X_STEP 2
#define Y_STEP 3
#define X_DIR  5
#define Y_DIR  6

// pin na ktery je privedena analogova hodnota z mereni vykonu laseru
// rozsah na merce je nastaven jako 0 az 30uW (mikro wattu)
#define LASER_PWR A3

int power = 0;

void setup() {
  // nastavení smeru pro vsechny piny
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
  } while(prev != analogRead(LASER_PWR));
  do {
    prev = analogRead(LASER_PWR);
    pohybOsy(false, Y_DIR, Y_STEP, 10);
  } while(prev != analogRead(LASER_PWR));
*/
  // Zahajime komunikaci s pocitacem na prenosove rychlost 115200 baudu, toto cislo je nutne take zvolit kdyz otevirame seriovy monitor.
  Serial.begin(115200);
  Serial.write(0);

}

void loop() {
  // precteme analogovou hodntu vystupu z laserove merky a vypiseme ji na seriovy port
  //Serial.print("Analog Val: ");
  int an_pwr = analogRead(LASER_PWR);
  //Serial.print(an_pwr);

  /* maximalni rozsah vykonove merky je 30uW,
     analogovy vystup je 0 az 5V,
     0V odpovida an hodnote 0, 5V odpovida 1023
  */
  double pwr = an_pwr / 1023.0 * 30.0;
  //Serial.print("\t Pwr (uW) = ");
  //Serial.println(pwr);

  char sent[2] = {an_pwr, an_pwr>>8};
  /* Jednoduchy kod pro ovladani z PC
      pouzivaji se klavesy WSAD pro zakladni pohyb motoru,
      AD ovladaji doleva a doprava (osa x)
      WS ovladaji nahoru a dolu (osa Y),
      jedno zmacknuti klavesy posune motorem o 200 kroku
  */
  if (Serial.available()) {
    char msg = Serial.read();
    if (msg == 'a') {
      Serial.write(int(200));
      targetedBlink(true,  X_DIR, X_STEP);
    }
    else if (msg == 'd') {
      Serial.println("Right 200");
      targetedBlink(false,  X_DIR, X_STEP);
    }
    if (msg == 's') {
      Serial.println("Down 200");
      targetedBlink(true,  Y_DIR, Y_STEP);
    }
    else if (msg == 'w') {
      Serial.write(sent);
    }
  }
  delay(250);
}

void targetedBlink(bool smer, byte dirPin, byte stepPin) {
  digitalWrite(dirPin, smer);
  digitalWrite(stepPin, HIGH);
  delay(250);
  digitalWrite(dirPin, LOW);
  digitalWrite(stepPin, LOW);
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
