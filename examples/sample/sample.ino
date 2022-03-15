#include <SoftwareSerial.h>
#include "CB_HCHO_V4.h"

SoftwareSerial mySerial(2, 3);

CB_HCHO_V4 hcho(&mySerial);

void setup()
{

  mySerial.begin(9600);
  Serial.begin(9600);

  while (!Serial)
  {
  }
}

void loop()
{
  bool success = hcho.read();

  if (success)
  {
    Serial.print("HCHO : ");
    Serial.println(hcho.getHcho());

    Serial.print("VOC : ");
    Serial.println(hcho.getVoc());

    Serial.print("Temp : ");
    Serial.println(hcho.getTemp());

    Serial.print("Humidity : ");
    Serial.println(hcho.getHumidity());

    Serial.print("TVOC : ");
    Serial.println(hcho.getTvoc());

    Serial.print("Sensor status : ");
    Serial.println(hcho.getSensorStatus());

    Serial.print("Auto calibration switch : ");
    Serial.println(hcho.getAutoCalibrationSwitch());
  }

  delay(3000);
}
