#include "TP4054.hpp"

TP4054::TP4054(){

};

TP4054::~TP4054(){

};

TP4054::TP4054(uint8_t const chrg_out_pin, uint8_t const chrg_in_pin, uint8_t const vbat_pin){
	_chrg_out_pin = chrg_out_pin;
	_chrg_in_pin = chrg_in_pin;
	_vbat_pin = vbat_pin;

	pinMode(_chrg_out_pin, INPUT); // put output in High-Z
	pinMode(_chrg_in_pin, INPUT);
	pinMode(_vbat_pin, INPUT);
};

float TP4054::getVoltage(){
#ifdef NRF52840_XXAA
	// Set the analog reference to 1.8V (default = 3.6V)
  	analogReference(AR_INTERNAL_1_8);
  	// Set the resolution to 12-bit (0..4095)
  	analogReadResolution(12); // Can be 8, 10, 12 or 14
    // Let the ADC settle
  	delay(1);
#endif

	float vbat = analogRead(_vbat_pin);

#ifdef NRF52840_XXAA
  	// Set the ADC back to the default settings
  	analogReference(AR_DEFAULT);
  	analogReadResolution(10);
#endif

  	vbat = vbat * (1800.0F/4096.0F); // 0..1.8V range 12 bit resolution

	return vbat;
};

uint32_t TP4054::getPercentage() {
#ifdef NRF52840_XXAA
	// Set the analog reference to 1.8V (default = 3.6V)
  	analogReference(AR_INTERNAL_1_8);
  	// Set the resolution to 12-bit (0..4095)
  	analogReadResolution(12); // Can be 8, 10, 12 or 14
    // Let the ADC settle
  	delay(1);
#endif

	uint32_t vbat = analogRead(_vbat_pin);

#ifdef NRF52840_XXAA
  	// Set the ADC back to the default settings
  	analogReference(AR_DEFAULT);
  	analogReadResolution(10);
#endif

    // if(isCharging) {
    //     vbat -= BATTERY_CHARGE_DIFFERENCE_ADC;
    // }

    return constrain(map(vbat, BATTERY_ZERO_CHARGE_ADC, BATTERY_FULL_CHARGE_ADC, 0, 100), 0, 100);
}

chargeStatus_t TP4054::getStatus(){
	chargeStatus_t status = TP4054_ERROR;

	// check if we're charging, set chrg_out_pin HIGH
	// and check chrg_in_pin for LOW value
	pinMode(_chrg_out_pin, OUTPUT);
	delay(10);
	digitalWrite(_chrg_out_pin, HIGH);
	delay(10);
	if(digitalRead(_chrg_in_pin) == LOW){
		// strong pull down -> charging
		pinMode(_chrg_out_pin, INPUT);
		status = TP4054_CHARGING;
	} else if (digitalRead(_chrg_in_pin) == HIGH){
		// we're in AC or LVCU set to high-Z
		pinMode(_chrg_out_pin, INPUT);
		delay(10);
		if (digitalRead(_chrg_in_pin) == LOW) {
			// weak pull down -> done
			status = TP4054_AC_PRESENT;
		} else if (digitalRead(_chrg_in_pin) == HIGH) {
			// high-z -> UVLO
			status = TP4054_UNDER_VOLTAGE_LOCKOUT;
		}
	}

	pinMode(_chrg_out_pin, INPUT);

	return status;
};
