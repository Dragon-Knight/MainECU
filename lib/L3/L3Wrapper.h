/*
	Обёртка пакета L3.
*/

#pragma once

class L3Wrapper
{
	public:
		using packet_t = L3Packet<64>;
		using callback_event_t = bool (*)(packet_t &request, packet_t &response);
		using callback_error_t = void (*)(packet_t &request, int8_t code);
		
		L3Wrapper(uint8_t transport, L3Driver &driver) : _driver(&driver)
		{
			this->_transport = transport;
			
			return;
		}
		
		void Init()
		{
			this->_driver->Init();
		}
		
		void RegCallback(callback_event_t event, callback_error_t error = nullptr)
		{
			this->_callback_event = event;
			this->_callback_error = error;
		}
		
		void SetUrgent()
		{
			this->_urgent_data = true;
			
			return;
		}
		
		// В будущем будет заменён на прерывание приёма байта.
		void IncomingByte()
		{
			if( this->_driver->ReadAvailable() > 0 )
			{
				byte incomingByte = this->_driver->ReadByte();
				
				if( _rx_packet.PutPacketByte(incomingByte) == true )
				{
					if( _rx_packet.IsReceived() == true )
					{
						if( this->_callback_event(_rx_packet, _tx_packet) == true )
						{
							// Установка транспорта ( перенести в L3Packet.h ? )
							_tx_packet.Transport(this->_transport);
							
							// Флаг ответа.
							_tx_packet.Direction(1);
							
							// Флаг необходимости передать срочное сообщение.
							if(this->_urgent_data == true) _tx_packet.Urgent(1);
							this->_urgent_data = false;
							
							_tx_packet.Prepare();
							
							// Отправка ответа.
							byte txbyte;
							while( _tx_packet.GetPacketByte(txbyte) == true )
							{
								this->_driver->SendByte(txbyte);
							}
							
							// Очистка пакета.
							_tx_packet.Init();
						}
						_rx_packet.Init();
					}
					
					if(_rx_packet.GetError() < 0)
					{
						this->_callback_error(_rx_packet, _rx_packet.GetError());
						_rx_packet.Init();
					}
				}
				else
				{
					this->_callback_error(_rx_packet, _rx_packet.GetError());
					_rx_packet.Init();
					
					// Или метод FlushBuffer() ?
					while(this->_driver->ReadAvailable() > 0){ this->_driver->ReadByte(); }
				}
			}
			
			return;
		}
		
		void Send(uint8_t type, uint16_t param, byte *data, uint8_t length)
		{
			_tx_packet.Transport(this->_transport);
			_tx_packet.Type(type);
			_tx_packet.Param(param);
			for(int8_t i = 0; i < length; ++i)
			{
				_tx_packet.Data1(data[i]);
			}
			_tx_packet.Prepare();
			
			byte txbyte;
			while( _tx_packet.GetPacketByte(txbyte) == true )
			{
				this->_driver->SendByte(txbyte);
			}
			
			_tx_packet.Init();
			
			return;
		}
	
	private:
		packet_t _rx_packet;
		packet_t _tx_packet;
		callback_event_t _callback_event;
		callback_error_t _callback_error;
		
		L3Driver *_driver;
		
		uint8_t _transport;
		bool _urgent_data = false;
};
