#include "CB_HCHO_V4.h"

CB_HCHO_V4::CB_HCHO_V4(Stream *uart)
    : uart(uart)
{
}

void CB_HCHO_V4::clearBuffer(void)
{
    idxBuffer = 0;
}

int16_t CB_HCHO_V4::recvData(uint8_t cmd, uint32_t timeout)
{
    clearBuffer();
    uint32_t start = millis();

    uint8_t b;

    while (millis() - start < timeout)
    {
        while (uart->available())
        {
            b = uart->read();

            switch (idxBuffer)
            {
            case 0: // STX
            {
                if (b != CB_HCHO_V4_STX_RCV)
                {
                    return -1;
                }
                rxBuffer[idxBuffer++] = b;
                break;
            }
            case 1: // LEN
            {
                if (b < 1)
                {
                    idxBuffer = 0;
                    return -1;
                }
                rxBuffer[idxBuffer++] = b;
                break;
            }
            case 2: // CMD
            {
                if (b != cmd)
                {
                    idxBuffer = 0;
                    return -1;
                }
                rxBuffer[idxBuffer++] = b;
                break;
            }
            default:
            {
                rxBuffer[idxBuffer++] = b;

                uint8_t len = rxBuffer[CB_HCHO_V4_POS_LEN];

                if (idxBuffer >= len + 3)
                {
                    idxBuffer = 0;

                    uint8_t cs = 0;

                    for (uint8_t i = 0; i < len + 2; i++)
                    {
                        cs += rxBuffer[i];
                    }

                    cs = 256 - cs;

                    if (cs != rxBuffer[len + 2])
                    {
                        return -1;
                    }

                    uint8_t c = rxBuffer[CB_HCHO_V4_POS_CMD];

                    uint8_t dataLength = rxBuffer[CB_HCHO_V4_POS_LEN] - 1;

                    if (dataLength == 0)
                    {
                        return dataLength;
                    }

                    for (uint8_t i = 0; i < dataLength; i++)
                    {
                        *(data + i) = rxBuffer[CB_HCHO_V4_POS_DAT + i];
                    }

                    return dataLength;
                }
                break;
            }
            }
        }
    }

    return -1;
}

bool CB_HCHO_V4::read(void)
{
    txBuffer[0] = 0x11;
    txBuffer[1] = 0x01;
    txBuffer[2] = CB_HCHO_V4_CMD_READ;
    txBuffer[3] = 0xed;

    uart->write(txBuffer, 4);
    
    int16_t len = recvData(CB_HCHO_V4_CMD_READ);

    if (len != 12)
    {
        return false;
    }

    hcho = ((data[0] << 8) + data[1]) / (float)1000;
    voc = ((data[2] << 8) + data[3]) / (float)1000;
    temp = ((data[4] << 8) + data[5]) / (float)10;
    humidity = ((data[6] << 8) + data[7]) / (float)10;
    tvoc = ((data[8] << 8) + data[9]) / (float)1000;
    sensorStatus = data[10];
    autoCalibrationSwitch = data[11];

    return true;
}

bool CB_HCHO_V4::setAutoCalibration(uint8_t mode)
{
    if (
        mode != CB_HCHO_V4_ACS_MANUAL &&
        mode != CB_HCHO_V4_ACS_AUTO &&
        mode != CB_HCHO_V4_ACS_ZERO_CLEARING)
    {
        return false;
    }

    txBuffer[0] = 0x11;
    txBuffer[1] = 0x02;
    txBuffer[2] = CB_HCHO_V4_CMD_AUTO_CALIBRATION;
    txBuffer[3] = mode;

    txBuffer[4] = 0;
    txBuffer[4] -= txBuffer[0];
    txBuffer[4] -= txBuffer[1];
    txBuffer[4] -= txBuffer[2];
    txBuffer[4] -= txBuffer[3];

    uart->write(txBuffer, 5);
    
    int16_t len = recvData(CB_HCHO_V4_CMD_AUTO_CALIBRATION);

    if (len != 1)
    {
        return false;
    }

    return mode == data[0];
}


float CB_HCHO_V4::getHcho(void) {
    return hcho;
}

float CB_HCHO_V4::getVoc(void) {
    return voc;
}

float CB_HCHO_V4::getTemp(void) {
    return temp;
}

float CB_HCHO_V4::getHumidity(void) {
    return humidity;
}

float CB_HCHO_V4::getTvoc(void) {
    return tvoc;
}

uint8_t CB_HCHO_V4::getSensorStatus(void) {
    return sensorStatus;
}

uint8_t CB_HCHO_V4::getAutoCalibrationSwitch(void) {
    return autoCalibrationSwitch;
}
