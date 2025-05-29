/*
 * ADC.c
 *
 *  Created on: Jan 23, 2025
 *      Author: medo4
 */
#include "ADC.h"
#include "ADC_Config.h"
#include <avr/io.h>

/*------------possible parameter---------
prescaler = 2,4,8,16,32,64,128
 Voltage_Reference = AREF,AVCC,INTERNAL
*/
void ADC_INIT(uint8_t prescaler, uint8_t Voltage_Reference)
{
	/*
	-----------step---------------------
	1) ADC enable
	2) set prescaler
	3) reference voltage select
	4) left adjust of for data register
	*/

	ADCSRA |= (1<<ADEN); // ADC Enable
	ADC_PRESCALER_SET(prescaler); // set sampling frequency by prescaler bit
	ADC_voltage_Reference(Voltage_Reference); //set voltage reference //by default AVCC has been selected
	ADMUX &= ~(1<<ADLAR); //left adjust off // Though bu default left adjust off // this line use for more convenient for future developer
}
//########################################################################################
//########################################################################################


//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// This function selects sampling frequency of ADC by selecting the prescaler bit
// if we set ADC Prescaler 128 means (Division Factor 128 to maintain ADC sampling frequency below 250KHz)(16MHz/128=125KHz)
void ADC_PRESCALER_SET(uint8_t prescaler)
{
	ADCSRA &= (~(1<<ADPS2)) & (~(1<<ADPS1)) & (~(1<<ADPS0)) ; // clear ADPS2:ADPS0 for initialization

	switch(prescaler)
	{
		case 2:ADCSRA |= (1<<ADPS0);//-----------------------------------  0 0 1
			   break;
		case 4:ADCSRA |= (1<<ADPS1);//-----------------------------------  0 1 0
			   break;
		case 8:ADCSRA |= (1<<ADPS1) | (1<<ADPS0);//----------------------  0 1 1
			   break;
		case 16:ADCSRA |= (1<<ADPS2);//----------------------------------  1 0 0
			   break;
		case 32:ADCSRA |= (1<<ADPS2) | (1<<ADPS0);//---------------------  1 0 1
			   break;
		case 64:ADCSRA |= (1<<ADPS2) | (1<<ADPS1);//---------------------  1 1 0
			   break;
		case 128:ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);//-------  1 1 1
			   break;
		default:ADCSRA &= (~(1<<ADPS2)) & (~(1<<ADPS1)) & (~(1<<ADPS0));// 0 0 0
	}

}

//This function selects the voltage reference
//by default AVCC has been selected
void ADC_voltage_Reference(uint8_t Reference_Voltage)
{
	ADMUX &= (~(1<<REFS0)) & (~(1<<REFS1)); // clear REFS1:REFS0 for initialization
	ADMUX |= (1<<REFS0); // Default AVCC has been set


	if(Reference_Voltage==AREF){ADMUX &= (~(1<<REFS0)) & (~(1<<REFS1));}
	else if (Reference_Voltage==AVCC){ADMUX |= (1<<REFS0);}
	else if (Reference_Voltage==INTERNAL){ADMUX |= (1<<REFS1) | (1<<REFS0);}
	else{ADMUX |= (1<<REFS0);}

}
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@



//########################################################################################
//########################################################################################
uint16_t ADC_READ(uint8_t channel)
{
	ADMUX &= (~(1<<MUX4)) & (~(1<<MUX3)) & (~(1<<MUX2)) & (~(1<<MUX1)) & (~(1<<MUX0)); // set channel ADC0 default

	switch(channel)
	{
		case 0:ADMUX &= (~(1<<MUX4)) & (~(1<<MUX3)) & (~(1<<MUX2)) & (~(1<<MUX1)) & (~(1<<MUX0)); //---------- 0 0 0 0 0
			   break;
		case 1:ADMUX |= (1<<MUX0);//-------------------------------------------------------------------------- 0 0 0 0 1
			   break;
		case 2:ADMUX |= (1<<MUX1);//-------------------------------------------------------------------------- 0 0 0 1 0
		       break;
		case 3:ADMUX |= (1<<MUX1) | (1<<MUX0);//-------------------------------------------------------------- 0 0 0 1 1
			   break;
		case 4:ADMUX |= (1<<MUX2);//-------------------------------------------------------------------------- 0 0 1 0 0
			   break;
		case 5:ADMUX |= (1<<MUX2) | (1<<MUX0);//-------------------------------------------------------------- 0 0 1 0 1
			   break;
		case 6:ADMUX |= (1<<MUX2) | (1<<MUX1);//-------------------------------------------------------------- 0 0 1 1 0
			   break;
		case 7:ADMUX |= (1<<MUX2) | (1<<MUX1) | (1<<MUX0);//-------------------------------------------------- 0 0 1 1 1
			   break;
		default:ADMUX &= (~(1<<MUX4)) & (~(1<<MUX3)) & (~(1<<MUX2)) & (~(1<<MUX1)) & (~(1<<MUX0)); // set channel ADC0 default
	}
	ADCSRA |=(1<<ADSC); // ADC conversion start

	while((ADCSRA & (1<<ADIF)) == 0){}; // wait for end conversion ( after conversion ADIF bit will be set automatically )
	//uint8_t high,low;
	//high=ADCH;
	//low=ADCL;
	return ADCW;// when conversion finished all data will be stored in ADCW
}
// Calculate the actual voltage from the ADC value
float ADC_GetVoltage(uint16_t adc_value) {
    // Calculate voltage at the ADC pin
    float v_adc = (adc_value / ADC_RESOLUTION) * VREF;

    // Scale the voltage using the voltage divider ratio
    return v_adc ;
}



