#ifndef __CB_HCHO_V4_H__
#define __CB_HCHO_V4_H__

#include "Arduino.h"
#include <stdint.h>
#include <stdbool.h>

#define CB_HCHO_V4_STX_SND 0x11
#define CB_HCHO_V4_STX_RCV 0x16

#define CB_HCHO_V4_CMD_READ 0x1
#define CB_HCHO_V4_CMD_AUTO_CALIBRATION 0x3

#define CB_HCHO_V4_POS_STX 0
#define CB_HCHO_V4_POS_LEN 1
#define CB_HCHO_V4_POS_CMD 2
#define CB_HCHO_V4_POS_DAT 3

#define CB_HCHO_V4_SENSOR_STATUS_NORMAL_WORKING 0
#define CB_HCHO_V4_SENSOR_STATUS_ANORMALY 1
#define CB_HCHO_V4_SENSOR_STATUS_RECOVERY_PROCESS 2
#define CB_HCHO_V4_SENSOR_STATUS_HIGH_FORMALDEHYDE 3
#define CB_HCHO_V4_SENSOR_STATUS_HIGH_ALCOHOL 4

#define CB_HCHO_V4_AUTO_CALIBRATION_OFF 0
#define CB_HCHO_V4_AUTO_CALIBRATION_ON 1

#define CB_HCHO_V4_ACS_MANUAL 0
#define CB_HCHO_V4_ACS_AUTO 1
#define CB_HCHO_V4_ACS_ZERO_CLEARING 2

class CB_HCHO_V4 {
public:
    CB_HCHO_V4(Stream *uart = &Serial);

    bool read(void);
    bool setAutoCalibration(uint8_t mode);

    float getHcho(void);
    float getVoc(void);
    float getTemp(void);
    float getHumidity(void);
    float getTvoc(void);
    uint8_t getSensorStatus(void);
    uint8_t getAutoCalibrationSwitch(void);

private:
    void clearBuffer(void);

    int16_t recvData(uint8_t cmd, uint32_t timeout = 1000);

    Stream *uart;
    uint8_t rxBuffer[256];
    uint8_t idxBuffer;
    uint8_t txBuffer[256];
    uint8_t data[256];

    float hcho;
    float voc;
    float temp;
    float humidity;
    float tvoc;
    uint8_t sensorStatus;
    uint8_t autoCalibrationSwitch;
};

#endif // __CB_HCHO_V4_H__
