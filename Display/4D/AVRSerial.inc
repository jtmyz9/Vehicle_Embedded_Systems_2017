var serBuffer[50]; //100 bytes of buffer
/*
func AVRSerialInit()
    COM1_RX_pin(PA3); //replace these with the proper pins
    COM1_TX_pin(PA4);
    com_SetBaud(COM1,960); //baudnum 6 = 9600

    com1_I1_Init(serBuffer,100,0);
endfunc
*/
func AVRSerialInit()

    com_SetBaud(COM0,960); //baudnum 6 = 9600

    com_Init(serBuffer,100,0);
endfunc
