#pragma once
#include <inttypes.h>
#include <esp32-hal.h>
#include "driver/spi_master.h"
#include <EasyPinD_ESP32.h>

namespace SPI
{
	spi_device_handle_t spi;

	EasyPinD shift_oe(GPIO_NUM_16, {0, GPIO_MODE_OUTPUT, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_DISABLE}, 1);
	EasyPinD shift_latch(GPIO_NUM_17, {0, GPIO_MODE_OUTPUT, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_DISABLE}, 0);



//extern "C"
//{
	inline void SPI_Init()
	{
		spi_bus_config_t buscfg = 
		{
			.mosi_io_num = GPIO_NUM_23,
			.miso_io_num = GPIO_NUM_19,
			.sclk_io_num = GPIO_NUM_18,
			.quadwp_io_num = GPIO_NUM_NC,
			.quadhd_io_num = GPIO_NUM_NC,
			.max_transfer_sz = 4096,
		};
		
		spi_device_interface_config_t devcfg = 
		{
			.mode = 0,
			.clock_speed_hz = SPI_MASTER_FREQ_40M,
			.spics_io_num = GPIO_NUM_NC,
			.flags = SPI_DEVICE_HALFDUPLEX/* | SPI_DEVICE_BIT_LSBFIRST*/,
			.queue_size = 7,
		};
		
		esp_err_t q1 = spi_bus_initialize(VSPI_HOST, &buscfg, SPI_DMA_CH_AUTO);
		esp_err_t q2 = spi_bus_add_device(VSPI_HOST, &devcfg, &spi);

		Serial.printf("errinit: %d, %d\n", q1, q2);
	}
	
	inline void SPI_Config(/*const SPIManagerInterface::spi_config_t &config*/)
	{

	}

	inline void SPI_Write(uint8_t *data, uint16_t length)
	{
		spi_transaction_t transaction = 
		{
			.length = (length * 8U),
			.tx_buffer = data,
			.rx_buffer = nullptr,
		};
		esp_err_t q = spi_device_transmit(spi, &transaction);

		Serial.printf("err: %d, len: %d, data: ", q, transaction.length);
		Serial.write(data, length);
		Serial.println();
	}

	inline void SPI_Read(uint8_t *data, uint16_t length)
	{
		spi_transaction_t transaction = 
		{
			.length = (length * 8U),
			.tx_buffer = nullptr,
			.rx_buffer = data,
		};
		spi_device_transmit(spi, &transaction);
	}

	inline void SPI_WriteRead(uint8_t *tx_data, uint8_t *rx_data, uint16_t length)
	{
		spi_transaction_t transaction = 
		{
			.length = (length * 8U),
			.tx_buffer = tx_data,
			.rx_buffer = rx_data,
		};
		spi_device_transmit(spi, &transaction);
	}
//}




	enum pin_mask_t : uint8_t
	{
		PIN_CS_NONE = 0,
		PIN_CS_FLASH = (1 << 0),
		PIN_CS_EEPROM = (1 << 1),
		PIN_CS_E07 = (1 << 2),
		PIN_CS_CAN_RS = (1 << 3),
		PIN_CS_A9G_ExRST = (1 << 4),
		PIN_CS_A9G_ExPWR = (1 << 5),
		PIN_CS_LED_G = (1 << 6),
		PIN_CS_LED_R = (1 << 7)
	};

	void SelectDev(pin_mask_t dev)
	{
		uint8_t data[] = {dev};
		SPI_Write(data, sizeof(data));
		
		shift_latch.On();
		asm("nop\n nop\n");
		shift_latch.Off();

		shift_oe.Off();

		return;
	}






	
/*	
	SPIManager<3> manager(SPI_Config, SPI_Write, SPI_Read, SPI_WriteRead);
	SPI_ZD25Q80B flash({GPIOB, GPIO_PIN_12}, SPI_BAUDRATEPRESCALER_2);
	SPI_CAT25080 eeprom({GPIOA, GPIO_PIN_8}, SPI_BAUDRATEPRESCALER_8);
	SPI_HC595<2> hc595({GPIOB, GPIO_PIN_4}, {GPIOB, GPIO_PIN_3}, {GPIOB, GPIO_PIN_2}, SPI_BAUDRATEPRESCALER_8);
*/	


	inline void Setup()
	{
		shift_oe.Init();
		shift_latch.Init();

		SPI_Init();

/*
		manager.AddDevice(flash);
		manager.AddDevice(eeprom);
		manager.AddDevice(hc595);
		
		hc595.OutputEnable();
		
		uint8_t dev_id[3] = {0x00};
		flash.ReadDevID(dev_id);
		DEBUG_LOG_TOPIC("NOR", "manufacturer ID: 0x%02X, memory type: 0x%02X, memory density: 0x%02X\n", dev_id[0], dev_id[1], dev_id[2]);

		uint8_t unique_id[16] = {0x00};
		flash.ReadUniqueID(unique_id);
		DEBUG_LOG_ARRAY_HEX("NOR", unique_id, sizeof(unique_id));
		Logger.PrintNewLine();
*/		
		return;
	}
	
	inline void Loop(uint32_t &time)
	{
/*
		manager.Tick(current_time);

		static uint32_t last = 0;
		if(current_time - last > 1500)
		{
			last= current_time;
			//qwewqeq();
			//qweqwerrr();
		}
*/
		
		static uint8_t iter = 0;
		static uint32_t last_tick = 0;
		if(time - last_tick > 500)
		{
			last_tick = time;

			if(iter == 0)
				SelectDev(PIN_CS_LED_G);
			if(iter == 1)
				SelectDev(PIN_CS_LED_R);
			if(iter == 2)
				SelectDev(PIN_CS_NONE);
			if(iter == 3)
				SelectDev( (pin_mask_t)(PIN_CS_LED_G | PIN_CS_LED_R) );
			if(iter == 4)
				SelectDev(PIN_CS_NONE);
			
			if(++iter > 4) iter = 0;
		}
		
		
		
		
		
		time = (esp_timer_get_time() / 1000ULL);
		
		return;
	}
}
