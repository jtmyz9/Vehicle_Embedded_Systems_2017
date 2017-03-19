/*
 * ETC.c
 *
 * Created: 11/15/2016 7:59:29 PM
 * Author : Jason McLoud
 */ 


#include "ETC.h"
//vehicle definitions, I2C, CAN addr, etc.
#include "..\..\..\car_23.h"


//preprocessor debug compile
#define DEBUG 
//If Using throttle sensors over canbus
#define CAN_SENSOR

/*! \brief Flags for status information
 */
struct GLOBAL_FLAGS {
  //! True when PID control loop should run one time
  uint8_t pidTimer:1;
  uint8_t dummy:7;
} gFlags = {0, 0};

//! Parameters for ETC
struct PID_DATA pidData;

//Structures for CanBus
st_cmd_t TPS, APPS;
U8 volatile tpsbuff[4], appsbuff[4]; //make one buff if makes sense with can packet

#ifdef DEBUG
U8 buffer[8];
st_cmd_t msg;
#endif


//ADC Conversion sensor value constants, min~max
//TODO: on final pcb measure these vals, vals will change slightly due to power supply
//tps1: min 1.1033kOhm ->
struct ADC_VAL {
	uint16_t tps_min_pos;		//THIS WHOLE THING IS WRONG
	uint16_t tps_max_pos;
	uint16_t tps_min_neg;
	uint16_t tps_max_neg;
	uint16_t apps_min;
	uint16_t apps_max;
} sensorvals = {10184, 61458, 55521, 4200, 600, 400};

/*! Sampling Time Interval
 *
 * Specify the desired PID sample time interval
 * Timer interval defined in ETC.h
 * With a 8-bit counter (255 cylces to overflow), the time interval value is calculated as follows:
 * TIME_INTERVAL = ( desired interval [sec] ) * ( frequency [Hz] ) / 255
 * 16mhz = 16uS for overflow
 */

//Timer2, is the other 8bit counter
ISR(TIMER2_OVF_vect)
{
  static uint16_t i = 0;  
  if(i < TIME_INTERVAL)	i++;
  else
  {
		gFlags.pidTimer = TRUE;
		i = 0;
  }
}


/* Processor Initialization
*  calls to macros/functions to setup communication channels
*  and interfaces
*/
void Init(void)
{	
	//i2c initialization macro
	//i2c_init();
	
	/*
	* CAN Bus Enable section
	* MUST SET CAN_BAUDRATE in config.h 
	*/	
	Can_reset();						//reset can controller for initialization
	can_init(1);						//sets CANBT registers, and enables controller
	while ((CANGSTA & (1<<ENFG)) == 0); //! Wait for Enable OK
	CANGIT = 0xFF;                      //! Reset General errors and OVRTIM flag
	can_clear_all_mob();				//Clear all Mobs
	////end CAN Bus init
	

	
	/*
	 * PWM Initilization
	 * MUST SET USE_TIMER16 in Config.h file for following macros
	 * FOSC needs to be defined in Config.h for calculating prescalar val
	*/
	#ifndef USE_TIMER16
	#  error  You must define USE_TIMER16 in "config.h" file
	#endif
		
	DDRB |= (1<<DDB7);										//set bit 7 of port b as output
	PORTB = 0;
	
	Timer16_set_counter(0);// clear counter
	Timer16_set_mode_output_c(TIMER16_COMP_MODE_CLEAR_OC); //clear OCR on upcount, standard duty cycle
	Timer16_set_waveform_mode(TIMER16_WGM_PWM_PFC_ICR); // phase and freq correct, ICR == top
	Timer16_set_clock(TIMER16_CLKIO_BY_1);				//default clock, prescaler 0	
	Timer16_set_capture(36363); //220 Hz from 16 MHz clock
	Timer16_set_compare_c(0); // 0 % duty cycle	
	Timer16_set_counter(0);// clear counter
	
	
	/*
	* TIMER setup for controlling PID measurement Time Interval
	* NO outputs needed on PORTB, just ISR 
	* 8Bit Timer2, with no prescalar overflow in 16uS
	* MUST SET USE_TIMER8 in Config.h, for inline function defs
	* FOSC needs to be defined in Config.h for calculating prescalar val 
	*/
	
	#ifndef USE_TIMER8
	#  error  You must define USE_TIMER8 in "config.h" file
	#endif
	
	Timer8_set_clock(TIMER8_2_CLKIO_BY_1);
	//TCNT0 = (1<<CS10);			//clock source to be used by the Timer/Counter clkI/O
	Timer8_overflow_it_enable();	//TIMSK0 = (1<<TOIE0);
	Timer8_set_counter(0);				
	
	/*
	* PORTA config for enabling H-bridge
	*/	
	DDRA = (1<<DDA0) | (1<<DDA1);
	PORTA = (1<<PA0) & (0<<PA1);			//enable h-Bridge in Forward mode
	
	//setup PID control structure
	pid_Init(K_P * SCALING_FACTOR, K_I * SCALING_FACTOR , K_D * SCALING_FACTOR , &pidData);
	
#ifdef DEBUG
	uart_init(CONF_8BIT_NOPAR_1STOP,9600);
	uart_put_string("Processor Init in Debug Mode\n");
#endif
}


#ifdef CAN_SENSOR
/*! \brief Read APPS sensor(s)
 *	Reads accelerator sensor(s), constrains both sensors to same rangek
 * return: 16bit int ADC value
 */
int16_t Get_Accelerator_Position(void)
{	
	//literally no idea if this works
	//can_clear_all_mob();
	while(can_cmd(&APPS) != CAN_CMD_ACCEPTED);		//uses id value stored in struct for id, and sets acceptance mask to all 1
	while(can_get_status(&APPS) != CAN_STATUS_COMPLETED);
	
	//		Error checking, Vals within range, % error between 
	//		if error state return 0
	uint16_t appsPositive = (APPS.pt_data[0] << 8) | APPS.pt_data[1];
	appsPositive = (appsPositive < sensorvals.apps_min? sensorvals.apps_min: appsPositive);
	appsPositive = (appsPositive > sensorvals.apps_max? sensorvals.apps_max: appsPositive);
	
	uint16_t appsNegative = (APPS.pt_data[0] << 8) | APPS.pt_data[1];
	appsNegative = (appsNegative < sensorvals.apps_min? sensorvals.apps_min: appsNegative);
	appsNegative = (appsNegative > sensorvals.apps_max? sensorvals.apps_max: appsNegative);
	
	//map 2nd potentiometer value to same range as 1st
	map(appsNegative, sensorvals.apps_min, sensorvals.apps_max, sensorvals.apps_min, sensorvals.apps_max);
	
	
	return appsPositive;
}

/*! \brief Read Throttle Position Sensor(s)
 *	Reads Throttle sensor(s), constrains both sensors to same rangek
 *	return: 16bit int, combined sensor value
 */
int16_t Get_Throttle_Position(void)
{	
	//literally no idea if this works
	//can_clear_all_mob();
	while(can_cmd(&TPS) != CAN_CMD_ACCEPTED);		//uses id value stored in struct for id, and sets acceptance mask to all 1
	while(can_get_status(&TPS) != CAN_STATUS_COMPLETED);

	//		Error checking, Vals within range, % error between 
	//		if error state return 0
	uint16_t tpsPositive = (APPS.pt_data[0] << 8) | APPS.pt_data[1];
	tpsPositive = (tpsPositive < sensorvals.tps_min_pos? sensorvals.tps_min_pos: tpsPositive);
	tpsPositive = (tpsPositive > sensorvals.tps_max_pos? sensorvals.tps_max_pos: tpsPositive);
	
	uint16_t tpsNegative = (APPS.pt_data[0] << 8) | APPS.pt_data[1];
	tpsNegative = (tpsNegative < sensorvals.tps_min_neg? sensorvals.tps_min_neg: tpsNegative);
	tpsNegative = (tpsNegative > sensorvals.tps_max_neg? sensorvals.tps_max_neg: tpsNegative);
	
	//map 2nd potentiometer value to same range as 1st
	map(tpsNegative, sensorvals.tps_min_neg, sensorvals.tps_max_neg, sensorvals.tps_min_pos, sensorvals.tps_max_pos);
	
	/*
	* Sensor Check, sesnsor vals equal, else one sensor is invalid
	* Allowable 5% error between adc vals
	* return failure, main sends error code to datalogger
	*/
	if(abs(tpsNegative - tpsPositive) > 3250){
		return 0;
	}

	
	
	return  tpsPositive;
}

#else

/*
* Return Accelerator Position sensors connected to AIN1 and AIN2
* 0-5v Sensor attached to AIN1, 5-0v Sensor attached to AIN2
* Constrains both sensors to same range
* return: U16 value representing Pedal position
*/
int16_t Get_Accelerator_Position(void){
	U16 pos1, pos2;
	
	adc_init(AVCC_AS_VREF, NO_LEFT_ADJUST, 5);
	pos1 = adc_single_conversion(5);
	
	adc_init(AVCC_AS_VREF, NO_LEFT_ADJUST, 6);
	pos2 = adc_single_conversion(6);
	
	/*
	//map 2nd potentiometer value to same range as 1st
	map(pos2, sensorvals.apps_min, sensorvals.apps_max, sensorvals.apps_min, sensorvals.apps_max);
	
	/*
	* Sensor Check, sesnsor vals equal, else one sensor is invalid
	* Allowable 5% error between adc vals
	* return failure, main sends error code to datalogger
	
	if(abs(pos1 - pos2) > 3250){
		return 0;
	}
	
	return pos1;
	*/
	
	/*
	* Summation attempt
	* Assume sum of two pots is ~5v(1024 fir 10bit adc)
	* Approx 5% allowable error; i.e. 4.75-5.25
	* TODO: Tighten constraints if applicable solution
	*/
	
	//where 50 ~=.25V
	if((pos1 + pos2)-1024 < 50) return pos1;	//pos1 ->5v sensor
	else						return 0;		//just return zero for now, signal error condition
}

/*
* Return Throttle Position sensors connected to AIN3 and AIN4
* 0-5v Sensor attached to AIN3, 5-0v Sensor attached to AIN4
* Constrains both sensors to same range
* return: U16 value representing Throttle position
*/
int16_t Get_Throttle_Position(void){
	U16 pos1, pos2;
	
	adc_init(AVCC_AS_VREF, NO_LEFT_ADJUST, 2);
	pos1 = adc_single_conversion(2);
	
	adc_init(AVCC_AS_VREF, NO_LEFT_ADJUST, 3);
	pos2 = adc_single_conversion(3);
	
	/* MAP Potentiometer attempt
	//map 2nd potentiometer value to same range as 1st
	map(pos2, sensorvals.tps_min_neg, sensorvals.tps_max_neg, sensorvals.tps_min_pos, sensorvals.tps_max_pos);
	
	/*
	* Sensor Check, sesnsor vals equal, else one sensor is invalid
	* Allowable 5% error between adc vals
	* return failure, main sends error code to datalogger
	
	if(abs(pos1 - pos2) > 3250){
		return 0;
	}
	
	return pos1;
	*/
	
	/*
	* Summation attempt
	* Assume sum of two pots is ~5v(1024 fir 10bit adc)
	* Approx 5% allowable error; i.e. 4.75-5.25
	* TODO: Tighten constraints if applicable solution
	*/
	
	//where 50 ~=.25V
	if((pos1 + pos2)-1024 < 50) return pos1;	//pos1 ->5v sensor
	else						return 0;		//just return zero for now, signal error condition
}
#endif


/*! \brief Set control input to system
 * Set PWM output to HBridge
 * inputValue: value to set PWM 
 * Currently calls set_pwm macro
 */
void Set_Motor_Speed(int16_t inputValue)
{
 	//TODO: value scaling, WINDUP_MAX
	
	Timer16_set_pwm_c(inputValue);	//change duty cycle of TCC1C
}

#ifdef DEBUG

/*!/brief Send current measured vals and set value via CanBus
* ref: APPS val, measuredval: TPS, input: new val sent to TCCR1
* ATTN; Function in current form is for DEBUG
*/
void send_current_state(int16_t ref, int16_t measuredval, int16_t input){
		  buffer[0] = (U8) (ref >> 8) & 0xff;
			buffer[1] = (U8) ref &0xff;
  
		 buffer[2] = (U8) (measuredval >> 8) & 0xff;
		buffer[3] = (U8) measuredval &0xff;
		
		buffer[4] = (U8) (input >> 8) & 0xff;
		buffer[5] = (U8) input & 0xff;
		
		//send can message to MOb
		while(can_cmd(&msg)!= CAN_CMD_ACCEPTED);
		//loop until send successful
		while(can_get_status(&msg) != CAN_STATUS_COMPLETED){
			if(can_get_status(&msg) ==  CAN_STATUS_ERROR){
				 can_clear_all_mob();
				 break;
			}
		}
		
		//debug function
		uart_mini_printf("Pedal: %d\nThrottle: %d\nInput:%d\n", ref, measuredval, input); 
}

/*
* Debug send status function, includes uart status writing 
*/
void send_pid_status(int8_t error_code){
	uart_mini_printf("Current PID Status Code: %d\n", error_code);
}
#else
/*
* Send Status of PID controller to Data Logging Equipment
* INPUT:
*		input-> PID controller output
*		error_code-> error state of controller
*/
void send_pid_status(int8_t error_code){
	
}

#endif




/*! \brief Demo of PID controller
 */
int main(void)
{
	int16_t throttle, pedal, inputValue;
	
	
	Disable_interrupt();
	Init();		//processor init function
	Enable_interrupt();
	
	/*
	* Initialize CanBus Message Objects
	*/
#ifdef DEBUG
	//debug can bug message
	msg.id.std = 0x123;	
	msg.pt_data = &buffer[0];//point msg data to data buffer
	msg.cmd = CMD_TX;
	msg.dlc = 8;	
#endif

#ifdef CAN_SENSOR
		//Define Can structures for CANbus
		TPS.cmd = APPS.cmd = CMD_RX_MASKED; //recieve messages with masked id
		TPS.pt_data = &tpsbuff[0];
		APPS.pt_data = &appsbuff[0];
		TPS.dlc = APPS.dlc = 4;
		TPS.id.std = 0x122;
		TPS.ctrl.ide = 0;
		TPS.ctrl.rtr = 0;
		APPS.id.std = 0x111;
		APPS.ctrl.ide = 0;
		APPS.ctrl.rtr = 0;
#endif	
	
	while(TRUE){		
		throttle = Get_Throttle_Position();
		
		//set idle offsets to pedal vals
		pedal = Get_Accelerator_Position();
		
#ifdef DEBUG
		uart_mini_printf("");
		send_current_state(pedal,throttle,inputValue);
#endif		
		
		//check return code from throttle, either 0 or current value
		if(throttle == 0){
			send_pid_status(TPS_FAILURE);	
			break;
		}
		
		//check return code from pedal, either 0 or current value
		if(pedal == 0){
			send_pid_status(APPS_FAILURE);
			break;	
		}
		
		if(gFlags.pidTimer == TRUE){
			inputValue = pid_Controller(pedal,throttle, &pidData);	
			inputValue += STICTION_CONSTAST;	//account for stiction in the motor
			
			if(inputValue < 0) Set_Motor_Speed(0);		
			else Set_Motor_Speed(inputValue);
			gFlags.pidTimer = FALSE;
			
			
				
			}
#ifdef DEBUG
		send_pid_status(NORMAL_OPERATION);	
#endif	
		}
		
	/*
	* Error State Loop: Holds processor in idle state
	* Must be powered down to return to normal operation
	*/
	while(TRUE){
#ifdef DEBUG
		uart_mini_printf("Error State");
#endif
		
		Disable_interrupt();		//disable interrupts to prevent ISR execution
		Set_Motor_Speed(0);		//Set PWM to 0, shutdown motor; 0 = fully closed throttle
		Timer16_clear();		//turn off OSC
		
		
	}
	
	
	
	return 1;
}