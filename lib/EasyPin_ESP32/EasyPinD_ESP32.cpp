#include "EasyPinD_ESP32.h"

void EasyPinD::Init()
{
	gpio_config(&_cfg);
	Write(_state);
	
	return;
}

void EasyPinD::Mode(gpio_config_t config, gpio_state_t state)
{
	_cfg = config;
	_state = state;
	Init();
	
	return;
}

void EasyPinD::On()
{
	gpio_set_level(_pin, 1);
	_state = 1;
	
	return;
}

void EasyPinD::Off()
{
	gpio_set_level(_pin, 0);
	_state = 0;
	
	return;
}

void EasyPinD::Toggle()
{
	if(_state == 1) Off();
	else On();
	
	return;
}

void EasyPinD::Write(gpio_state_t state)
{
	gpio_set_level(_pin, state);
	_state = state;
	
	return;
}

gpio_state_t EasyPinD::Read()
{
	_state = gpio_get_level(_pin);
	
	return _state;
}

gpio_state_t EasyPinD::GetState()
{
	return _state;
}
