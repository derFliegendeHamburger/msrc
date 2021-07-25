#include "escCell.h"

EscCell::EscCell() {}

uint8_t EscCell::setCellCount(float voltage)
{                   // 0     1    2     3     4       5     6       7     8       9     10    11   
    float level[] = { 1.4,  5.7, 10,  14.3,  18.6,  22.9,  27.2,  31.5,  35.8,  40.1,  44.4,  48.7  };

          // {0, 4.2, 8.4, 12.6, 16.8, 21, 25.2, 29.4, 33.6, 33.6, 42, 42};
    int8_t cont = 11;
    while (voltage < level[cont] && cont > 0)
        cont--;
    return cont + 1;
}

float *EscCell::cellVoltageP()
{
    return &cellVoltage_;
}
