#pragma once
#include <inttypes.h>
#include <EasyPinD_ESP32.h>

class SPI_CSCtrl
{
	public:

		enum pin_mask_t : uint8_t
		{
			PIN_CS_FLASH = (1 << 0),
			PIN_CS_EEPROM = (1 << 1),
			PIN_CS_E07 = (1 << 2),
			PIN_CS_CAN_RS = (1 << 3),
			PIN_CS_A9G_ExRST = (1 << 4),
			PIN_CS_A9G_ExPWR = (1 << 5),
			PIN_CS_LED_G = (1 << 6),
			PIN_CS_LED_R = (1 << 7)
		};


		void EnableFlash()
		{

		}
};
