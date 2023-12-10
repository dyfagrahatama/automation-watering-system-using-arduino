#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <DS3231.h>

///// < LCD > /////
LiquidCrystal_I2C lcd(0x27,20,4);

///// < RTC > /////
RTClib myRTC;

#define Relay_Sol 6

int Sensor_soil[3] = {A0, A1, A2};
int minute_now, jam_now;
int iter = 0;
bool state = false;
int bobot;

float value_sensor[3];

void setup() {
  Serial.begin(9600);
  Wire.begin();
  lcd.init();

  pinMode(Relay_Sol, OUTPUT);
}

void getTimeNow(){
  DateTime now = myRTC.now();

  String buff = String(now.minute(), DEC);
  minute_now = buff.toInt();
  buff = String(now.hour(), DEC);
  jam_now = buff.toInt();
}

void (* resetFunc) (void) = 0;

float getMoisture(int _pin_){
  float Moisture_;
  float MoistureValue;
  const int Atas = 700;
  const int Bawah = 280;

  Moisture_ = analogRead(_pin_);
  Serial.print("Nilai sensor : ");
  Serial.println(Moisture_);
  MoistureValue = map(Moisture_, Atas, Bawah, 100, 0);

  return MoistureValue;
}

void loop() {
  getTimeNow();

  Serial.print(jam_now);
  Serial.print(":");
  Serial.println(minute_now);

  for(int i=0; i<3; i++){
    value_sensor[i] = getMoisture(Sensor_soil[i]);
    if(value_sensor[i] > 50){
      bobot++;
    }
  }

  if(7 <= jam_now <= 9){
    state = true;
    Serial.println("Penyiraman pagi hari");
    if(!state){
      resetFunc();
    }
    digitalWrite(Relay_Sol, LOW);
  }
  else if(15 <= jam_now <= 17){
    state = true;
    Serial.println("Penyiraman sore hari");
    if(!state){
      resetFunc();
    }
    digitalWrite(Relay_Sol, LOW);
  }
  else if(bobot >= 2){
    state = true;
    Serial.println("Penyiraman dari sensor");
    if(!state){
      resetFunc();
    }
    digitalWrite(Relay_Sol, LOW);
  }
  else{
    state = false;
    if(state){
      resetFunc();
    }
    digitalWrite(Relay_Sol, HIGH);
  }
  delay(1000);
  bobot = 0;
}