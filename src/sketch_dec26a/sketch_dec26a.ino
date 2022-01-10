#include <MsgPack.h>

#define send(x) {MsgPack::Packer packer; packer.serialize(x); unsigned int len = packer.size(); Serial.write((byte*)&len, 2);Serial.write(packer.data(), packer.size());}

#define send_array(x, y) for(int i = 0; i < (y); i++){send(x);while(!Serial.available());Serial.read();}

struct time_n_place_stamp{
  unsigned long time;
  long long place;
  float a;
  MSGPACK_DEFINE(time, place, a);
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  time_n_place_stamp neco = {15, 0, 15.2};
  send(neco);
}

void loop() {
  // put your main code here, to run repeatedly:
  time_n_place_stamp neco = {millis(), 0, 20.2};
  send(neco);
  delay(1000);
}
