//Unit Test for ETC: Send TPS and APPS test data over CANbus
//Prints all sent analog vals, and returned PID vals to serial for calibration

#include <SPI.h>
#include <mcp_can.h>

const int SPI_CS_PIN = 9;

MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin



//TODO: Two pots per signal, send raw vals to 
int APS = A3;    // Single Pot for APPS
int TPS = A5;    // Single Pot for TPS
unsigned short tpsval, appsval;
void setup() {
  Serial.begin(115200);

    while (CAN_OK != CAN.begin(CAN_250KBPS))              // init can bus : baudrate = 500k
    {
        Serial.println("CAN BUS Shield init fail");
        Serial.println(" Init CAN BUS Shield again");
        delay(100);
    }
    Serial.println("CAN BUS Shield init ok!");
}

//CANBus packets come in as array of U8, 
//will need to combine two vals for full analog range

unsigned char stmp[8] = {0, 1, 2, 3, 4, 5, 6, 7};
unsigned char tps[4] = {0,0,0,0};
void loop() {

  tpsval = analogRead(TPS);
  appsval = analogRead(APS);
  
   unsigned char len = 0;
   unsigned char buf[8];

  stmp[0] = (unsigned char) (appsval >> 8) & 0xff;
  stmp[1] = (unsigned char) appsval &0xff;
  
  tps[0] = (unsigned char) (tpsval >> 8) & 0xff;
  tps[1] = (unsigned char) tpsval &0xff;

  Serial.println(tpsval);
    // send APPS:  id = 0x00, standrad frame, data len = 8, stmp: data buf
    CAN.sendMsgBuf(0x111, 0, 4, stmp);
    delay(100);                       // send data per 100ms
    // send TPS:  id = 0x00, standrad frame, data len = 8, stmp: data buf
    CAN.sendMsgBuf(0x122, 0, 4,tps);
    delay(100);                       // send data per 100ms
    if(CAN_MSGAVAIL == CAN.checkReceive())            // check if data coming
    {
       CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
   unsigned int canId = CAN.getCanId();
        
        Serial.println("-----------------------------");
        Serial.print("Get data from ID: ");
        Serial.println(canId, HEX);

        for(int i = 0; i<len; i++)    // print the data
        {
            Serial.print(buf[i]);
            Serial.print("\t");
        }
        Serial.println();
    }
}
