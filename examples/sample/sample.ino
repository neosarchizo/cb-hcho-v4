#include <SoftwareSerial.h>
#include <AM1002_UART.h>

SoftwareSerial mySerial(2, 3);

AM1002_UART am1002(&mySerial);

char version[13];

void setup() {
  mySerial.begin(9600);
  Serial.begin(9600);

  while (!Serial)
  {
  }
}

void loop() {
  bool success = am1002.measure();

  if (success)
  {
    Serial.print("TVOC : ");
    Serial.println(am1002.getTvoc());

    Serial.print("PM 1.0 : ");
    Serial.println(am1002.getPm1p0());

    Serial.print("PM 2.5 : ");
    Serial.println(am1002.getPm2p5());

    Serial.print("PM 10 : ");
    Serial.println(am1002.getPm10());

    Serial.print("Temperature : ");
    Serial.println(am1002.getTemperature());

    Serial.print("Humidity : ");
    Serial.println(am1002.getHumidity());
  }

  success = am1002.requestSoftwareVersionNumber();

  if (success)
  {
    am1002.readSoftwareVersionNumber(version);
    Serial.print("Version : ");
    Serial.println(version);
  }

  delay(3000);
}