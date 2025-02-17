#include "escHW3.h"

EscHW3::EscHW3(Stream &serial, uint8_t alphaRpm) : alphaRpm_(alphaRpm), serial_(serial) {}

void EscHW3::update()
{
    static uint32_t tsEsc_ = 0;
    while (serial_.available() >= 10)
    {
        if (serial_.read() == 0x9B)
        {
            uint8_t data[9];
            uint8_t cont = serial_.readBytes(data, 9);
            if (cont == 9 && data[3] == 0 && data[5] == 0)
            {
                thr_ = data[4]; // 0-255
                pwm_ = data[6]; // 0-255
                uint16_t rpmCycle = (uint16_t)data[7] << 8 | data[8];
                if (rpmCycle <= 0)
                    rpmCycle = 1;
                float rpm = 60000000.0 / rpmCycle;
                rpm_ = calcAverage(alphaRpm_ / 100.0F, rpm_, rpm);
                tsEsc_ = millis();
#if defined(DEBUG_ESC_HW_V3) || defined(DEBUG_ESC)
                uint32_t pn =
                    (uint32_t)data[0] << 16 | (uint16_t)data[1] << 8 | data[2];
                DEBUG_SERIAL.print("N:");
                DEBUG_SERIAL.print(pn);
                DEBUG_SERIAL.print(" R:");
                DEBUG_SERIAL.println(rpm);
#endif
            }
        }
    }
    if (millis() - tsEsc_ > 150)
    {
        pwm_ = 0;
        thr_ = 0;
        rpm_ = 0;
    }
#ifdef SIM_SENSORS
    rpm_ = 12345.67;
#endif
}

uint8_t *EscHW3::thrP()
{
    return &thr_;
}

uint8_t *EscHW3::pwmP()
{
    return &pwm_;
}

float *EscHW3::rpmP()
{
    return &rpm_;
}
