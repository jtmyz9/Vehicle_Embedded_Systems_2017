/*
 * mlx90614
 * functions to read/write to the mlx90614 series of thermopile sensors
 * Written by Matthew Nevels
 * matthew@enevels.com
 * 2/27/10
 */
 
#include "i2cmaster.h"  //for TwoWire interface
#include <util/crc16.h> 	    //for computing pec (Packet Error Code)
#include <util/delay.h> //for time delays
#include <avr/io.h>
#include "mlx90614.h"

unsigned int mlx_read(unsigned int address, unsigned int command);
void mlx_write(unsigned char address, unsigned char command, unsigned int newvalue);
float mlx_getObjTempK(unsigned char address);
float mlx_getObjTempC(unsigned char address);
float mlx_getObjTempF(unsigned char address);
float mlx_getAmbTempK(unsigned char address);
float mlx_getAmbTempC(unsigned char address);
unsigned int mlx_getAddress(void);
int mlx_setAddress(unsigned char oldaddress, unsigned char newaddress);

//read two bytes from address using command
unsigned int mlx_read(unsigned int address, unsigned int command){
  unsigned char low,high,pec;
  int dev = address<<1; //shift 1 so we can + read/write bit on next line
  i2c_start_wait(dev+I2C_WRITE);//start i2c read from SMBus address
  i2c_write((char)command);			//command (mlx90614 command + address)
  i2c_rep_start(dev+I2C_READ);	//repeated start read from SMBus address
  low = i2c_readAck(); 			//Read low byte and then send ack
  high = i2c_readAck(); 		//Read high byte and then send ack
  pec = i2c_readNak();			//Read pec and then send nak
  i2c_stop();					//stop i2c
  return low + (high << 8);
}

//Write two bytes to address using command
void mlx_write(unsigned char address, unsigned char command, unsigned int newvalue){
  unsigned char high,low,pec,arr[6];
  //Erase location at address
  high = 0x00;
  low = 0x00;
  int dev = address<<1; //shift 1 so we can OR the read/write bit
  arr[5]=0x00;		    //must be 0 for pec calculation
  arr[4]=dev+I2C_WRITE;	//address
  arr[3]=command;		//command (mlx90614 command + address)
  arr[2]=0x00;			//Low byte
  arr[1]=0x00;			//High byte
  arr[0]=0x00;			//must be 0 for pec calculation
  //pec=PEC_calculation(arr);//Calculate CRC

  i2c_start_wait(dev+I2C_WRITE); //start i2c write to SMBus address
  i2c_write(command);	//command (mlx90614 command + address)  
  i2c_write(0x00);  	//Low byte
  i2c_write(0x00);  	//High byte
  i2c_write(pec); 		//pec
  i2c_stop();			//stop i2c
  _delay_ms(10);		//wait for sensor to write
  
  //Write newvalue to location at address
  high = (newvalue >> 8);                    //separate high byte
  low = (newvalue - ((newvalue >> 8) << 8)); //separate low byte
  
  arr[5]=0x00;		    //must be 0 for pec calculation
  arr[4]=dev+I2C_WRITE;	//address
  arr[3]=command;		//command (mlx90614 command + address)
  arr[2]=low;			//Low byte
  arr[1]=high;			//High byte
  arr[0]=0x00;			//must be 0 for pec calcuation
  //pec=PEC_calculation(arr);//Calculate CRC

  i2c_start_wait(dev+I2C_WRITE); //start i2c write to SMBus address
  i2c_write(command);	//command (mlx90614 command + address)
  i2c_write(low);  		//Low byte
  i2c_write(high);  	//High byte
  i2c_write(pec); 		//packet error code
  i2c_stop();			//stop i2c
  _delay_ms(10);		//wait for sensor to write
}


//read object temperature in Kelvin
float mlx_getObjTempK(unsigned char address){
  float tempFactor = 0.02; // 0.02 degrees per LSB
  return tempFactor * mlx_read(address,0x07); //0x07 is Melexis command to access RAM address 0x07
}

//read object temperature in Kelvin
float mlx_getObjTempC(unsigned char address){
  return mlx_getObjTempK(address) - 273.15;
}

//read object temperature in Celsius
float mlx_getObjTempF(unsigned char address){
  return 9. * mlx_getObjTempC(address) / 5. + 32.;
}

//read ambient temperature in Farenheit
float mlx_getAmbTempK(unsigned char address){
  float tempFactor = 0.02; // 0.02 degrees per LSB
  return tempFactor * mlx_read(address,0x06); //0x06 is Melexis command to access RAM address 0x06
}

//read ambient temperature in Celsius
float mlx_getAmbTempC(unsigned char address){
  return mlx_getAmbTempK(address) - 273.15;
}

//read ambient temperature in Farenheit
float mlx_getAmbTempF(unsigned char address){
  return 9. * mlx_getAmbTempC(address) / 5. + 32.;
}

//read current SMBus address 
//reads from address 0, assuming there are no other mlx90614s on the bus
unsigned int mlx_getAddress(void){
  unsigned int address = mlx_read(0,0x2E); //0x2E is Melexis command to access EEPROM address 0x0E
  return (address - ((address >> 8) << 8)); //return low byte only
}

//set SMBus address (mlx90614 uses 7 bit address)
int mlx_setAddress(unsigned char oldaddress, unsigned char newaddress){
	if(newaddress > 127) return -1;//Address is 7 bits in MLX90614
	mlx_write(oldaddress,0x2E,newaddress);//0x2E is Melexis command to access EEPROM address 0x0E
	return mlx_getAddress();
}
	
//read emissivity value
float mlx_getEmissivity(unsigned char address){
  float emiss = mlx_read(address,0x24);//0x24 is Melexis command to access EEPROM address 0x04
  return (emiss+1)/65536;
}

//set emissivity value
float mlx_setEmissivity(unsigned char address, float newvalue){
    if (newvalue < 0 || newvalue >1) return -1;
    newvalue = newvalue*65536.-1.; 	//Calculate Emissivity parameter
    mlx_write(address,0x24,(unsigned int)newvalue);//0x24 is Melexis command to access EEPROM address 0x04
	return mlx_getEmissivity(address);
}
