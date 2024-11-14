#pragma once
#include <inttypes.h>
#include "driver/gpio.h"

/*
#define GPIO_MODE_INPUT			GPIO_MODE_INPUT
#define GPIO_MODE_OUTPUT_PP		GPIO_MODE_OUTPUT
#define GPIO_MODE_OUTPUT_OD		GPIO_MODE_OUTPUT_OD

#define GPIO_NOPULL
#define GPIO_PULLUP
#define GPIO_PULLDOWN

#define GPIO_SPEED_FREQ_LOW
#define GPIO_SPEED_FREQ_MEDIUM
#define GPIO_SPEED_FREQ_HIGH
	#define GPIO_TypeDef	uint8_t
	#define GPIO_PinState	bool
	#define GPIO_PIN_RESET	false
	#define GPIO_PIN_SET	true
	typedef struct
	{
		gpio_num_t Pin;
		uint32_t Mode;
		uint32_t Pull;
		uint32_t Speed;
	} GPIO_InitTypeDef;
*/

typedef int gpio_state_t;

class EasyPinD
{
	public:
		
		// EasyPinD pin(GPIO_NUM_18, {0, GPIO_MODE_OUTPUT, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_DISABLE}, 0);
		EasyPinD(gpio_num_t pin, gpio_config_t config, gpio_state_t init = 0) : _pin(pin), _cfg(config), _state(init)
		{
			_cfg.pin_bit_mask = (1ULL << pin);
			_cfg.intr_type = GPIO_INTR_DISABLE;
		}

		void Init();
		void Mode(gpio_config_t config, gpio_state_t state = 0);
		void On();
		void Off();
		void Toggle();
		void Write(gpio_state_t state);
		gpio_state_t Read();
		gpio_state_t GetState();
		
	private:
		
		gpio_num_t _pin;
		gpio_config_t _cfg;
		gpio_state_t _state;
};
