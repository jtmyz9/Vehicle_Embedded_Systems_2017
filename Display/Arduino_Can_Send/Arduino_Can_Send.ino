// demo: CAN-BUS Shield, send data
#include <mcp_can.h>
#include <SPI.h>

// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 9;

MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin

void setup()
{
    Serial.begin(9600);

    while (CAN_OK != CAN.begin(CAN_500KBPS))              // init can bus : baudrate = 500k
    {
        Serial.println("CAN BUS Shield init fail");
        Serial.println(" Init CAN BUS Shield again");
        delay(100);
    }
    Serial.println("CAN BUS Shield init ok!");
}

unsigned char stmp[2] = {0x2E, 0x00};
void loop()
{
    // send data:  id = 0x00, standrad frame, data len = 8, stmp: data buf
    for(int rpm = 0; rpm < 13500; rpm+=100) {
     stmp[1] = lowByte(rpm);
     stmp[0] = highByte(rpm);
     CAN.sendMsgBuf(0x00, 0, 2, stmp);
     delay(25);
    }   // send data per 100ms
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
