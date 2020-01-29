#ifndef _TP4054_H_
#define _TP4054_H_

#include <Arduino.h>

#define BATTERY_FULL_CHARGE_ADC 3812	// 1.68V 1680mV (1.5Mohm and 1Mohm)
#define BATTERY_ZERO_CHARGE_ADC 2657 	// 1.16V 1160mV (1.5Mohm and 1Mohm)
#define BATTERY_CHARGE_DIFFERENCE_ADC 115

typedef enum chargeStatus{
	TP4054_ERROR = 0,
	TP4054_CHARGING = 1,
	TP4054_AC_PRESENT = 2,
	TP4054_UNDER_VOLTAGE_LOCKOUT = 3
}chargeStatus_t;

class TP4054
{

public:

	TP4054();

	~TP4054();

	TP4054(uint8_t chrg_out, uint8_t chrg_in, uint8_t v_bat);

	float getVoltage();

	uint32_t getPercentage();

	chargeStatus_t getStatus();

private:

uint8_t _chrg_out_pin;
uint8_t _chrg_in_pin;
uint8_t _vbat_pin;
};

#endif
