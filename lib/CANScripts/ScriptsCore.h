#pragma once

class Script_010A: public ScriptInterface
{
	public:
		void Run(uint16_t id, StateDB::db_t &db_element, tx_t func) override
		{
			if(db_element.data[0] != 0x61) return;
			
			// Обработка включения заднего света.
			_tx_packet.id = 0x00E6;
			_tx_packet.raw_data_len = 2;
			_tx_packet.func_id = 0x01;
			if( db_element.data[1] == 0x02 || db_element.data[3] == 0x02 )
			{
				if(_is_active_reverse == false)
				{
					_tx_packet.data[0] = 0xFF;
					func(_tx_packet);
					
					_is_active_reverse = true;
				}
			}
			else
			{
				if(_is_active_reverse == true)
				{
					_tx_packet.data[0] = 0x00;
					func(_tx_packet);
					
					_is_active_reverse = false;
				}
			}
			
			
			// Обработка включения анимации зарядки.
			StateDB::db_t obj;
			db_obj->Get(0x0057, obj);
			int16_t power = *(int16_t*)(obj.data + 1);
			
			_tx_packet.id = 0x00EB;
			_tx_packet.raw_data_len = 2;
			_tx_packet.func_id = 0x01;
			if( (db_element.data[1] == 0x00 || db_element.data[3] == 0x00) && power < 0 )
			{
				if(_is_active_charging_anim == false)
				{
					_tx_packet.data[0] = 0xC1;
					func(_tx_packet);
					
					_is_active_charging_anim = true;
				}
			}
			else
			{
				if(_is_active_charging_anim == true)
				{
					_tx_packet.data[0] = 0x00;
					func(_tx_packet);
					
					_is_active_charging_anim = false;
				}
			}
			
			return;
		}
		
	private:
		bool _is_active_reverse = false;
		bool _is_active_charging_anim = false;
};


class ScriptHoodTrunk: public ScriptInterface
{
	public:
		void Run(uint16_t id, StateDB::db_t &db_element, tx_t func) override
		{
			if(db_element.data[0] != 0x65) return;
			if(db_element.data[1] != 0xFF) return;
			
			_tx_packet.raw_data_len = 1;
			_tx_packet.func_id = 0x02;

			switch(id)
			{
				// Кнопка 09, Кнопка открытия капота.
				case 0x012C:
				{
					_tx_packet.id = 0x0185;
					func(_tx_packet);
					
					break;
				}

				// Кнопка 10, Кнопка открытия багажника.
				case 0x012D:
				{
					_tx_packet.id = 0x0184;
					func(_tx_packet);

					break;
				}
			}
			
			return;
		}
};


class ScriptLeftRightDoor: public ScriptInterface
{
	public:
		void Run(uint16_t id, StateDB::db_t &db_element, tx_t func) override
		{
			if(db_element.data[0] != 0x65) return;
			if(db_element.data[1] != 0xFF) return;
			
			_tx_packet.raw_data_len = 1;
			_tx_packet.func_id = 0x03;

			switch(id)
			{
				// Кнопка 11, Кнопка открытия левой двери.
				case 0x012E:
				{
					_tx_packet.id = 0x0187;
					func(_tx_packet);
					
					break;
				}

				// Кнопка 15, Кнопка открытия правой двери.
				case 0x0132:
				{
					_tx_packet.id = 0x0188;
					func(_tx_packet);

					break;
				}
			}
			
			return;
		}
};


class ScriptHorn: public ScriptInterface
{
	public:
		void Run(uint16_t id, StateDB::db_t &db_element, tx_t func) override
		{
			if(db_element.data[0] != 0x65) return;
			
			_tx_packet.id = 0x018B;
			_tx_packet.raw_data_len = 2;
			_tx_packet.func_id = 0x01;
			_tx_packet.data[0] = db_element.data[1];
			func(_tx_packet);
			
			return;
		}
};


class ScriptPowerOnOff: public ScriptInterface
{
	public:
		void Run(uint16_t id, StateDB::db_t &db_element, tx_t func) override
		{
			if(db_element.data[0] != 0x65) return;
			
			bool motor = ((db_element.data[7] >> 4) == 0 || (db_element.data[7] & 0x0F) == 0);
			
			_tx_packet.raw_data_len = 2;
			_tx_packet.func_id = 0x01;
			if(motor == true)
			{
				if(_is_enable == false)
				{
					_tx_packet.data[0] = 0xFF;
					
					_tx_packet.id = 0x0186;
					func(_tx_packet);
					
					_tx_packet.id = 0x018A;
					func(_tx_packet);

					_is_enable = true;
				}
			}
			else
			{
				if(_is_enable == true)
				{
					_tx_packet.data[0] = 0x00;
					
					_tx_packet.id = 0x0186;
					func(_tx_packet);
					
					_tx_packet.id = 0x018A;
					func(_tx_packet);
					
					_is_enable = false;
				}
			}
			
			return;
		}
		
	private:
		bool _is_enable = false;
};


class ScriptThrottleCtrl : public ScriptInterface
{
	public:
		void Run(uint16_t id, StateDB::db_t &db_element, tx_t func) override
		{
			if(db_element.data[0] != 0x61) return;
			
			uint16_t value = ((uint16_t)(db_element.data[2]) << 8) | db_element.data[1];
			uint16_t value_map = MapClump(value, (uint16_t)650, (uint16_t)3000, (uint16_t)0, (uint16_t)1023);
			
			uint8_t data[] = {0x0A, 0x00, ((value_map >> 0) & 0xFF), ((value_map >> 8) & 0xFF)};
			data[1] = ++counter[0]; Send(0x0104, data, sizeof(data));
			data[1] = ++counter[1]; Send(0x0105, data, sizeof(data));
			data[1] = ++counter[2]; Send(0x0134, data, sizeof(data));
			data[1] = ++counter[3]; Send(0x0135, data, sizeof(data));
			
			return;
		}
	
	private:
		uint8_t counter[4];
};


class ScriptButtonsCtrl_CN2 : public ScriptInterface
{
	public:
		void Run(uint16_t id, StateDB::db_t &db_element, tx_t func) override
		{
			if(db_element.data[0] != 0x65) return;

			switch(db_element.data[1])
			{
				case 1:
				{
					_gear = (db_element.data[2] == 0x0F) ? 0x01 : 0x00;
					_gear_update = true;
					
					break;
				}
				case 2:
				{
					_gear = (db_element.data[2] == 0x0F) ? 0x02 : 0x00;
					_gear_update = true;
					
					break;
				}
				case 3:
				{
					_gear = (db_element.data[2] == 0x0F) ? 0x03 : 0x00;
					_gear_update = true;
					
					break;
				}
				case 4:
				{
					uint8_t ignition = (db_element.data[2] == 0x0F) ? 0xFF : 0x00;
					uint8_t data[] = {0x01, ignition};
					Send(0x010A, data, sizeof(data));
					Send(0x010B, data, sizeof(data));
					Send(0x013A, data, sizeof(data));
					Send(0x013B, data, sizeof(data));

					break;
				}
				case 5:
				{
					uint8_t brake = (db_element.data[2] == 0x0F) ? 0xFF : 0x00;
					uint8_t data[] = {0x01, brake};
					Send(0x0108, data, sizeof(data));
					Send(0x0109, data, sizeof(data));
					Send(0x0138, data, sizeof(data));
					Send(0x0139, data, sizeof(data));

					break;
				}
				case 6:
				{
					
					
					break;
				}
				case 7:
				{
					
					
					break;
				}
				case 8:
				{
					
					
					break;
				}
				default:
				{
					break;
				}
			}
			
			if(_gear_update == true)
			{
				uint8_t data[] = {0x01, _gear};
				Send(0x0106, data, sizeof(data));
				Send(0x0107, data, sizeof(data));
				Send(0x0136, data, sizeof(data));
				Send(0x0137, data, sizeof(data));

				_gear_update = false;
			}
			
			return;
		}

	private:

		uint8_t _gear = 0x00;
		bool _gear_update = false;
};

class ScriptButtonsCtrl_CN3 : public ScriptInterface
{
	public:
		void Run(uint16_t id, StateDB::db_t &db_element, tx_t func) override
		{
			if(db_element.data[0] != 0x65) return;

			switch(db_element.data[1])
			{
				case 9:
				{
					
					
					break;
				}
				case 10:
				{
					
					
					break;
				}
				case 11:
				{
					
					
					break;
				}
				case 12:
				{
					
					
					break;
				}
				case 13:
				{
					
					
					break;
				}
				case 14:
				{
					
					
					break;
				}
				case 15:
				{
					
					
					break;
				}
				case 16:
				{
					
					
					break;
				}
				default:
				{
					break;
				}
			}
			
			
			
			return;
		}
};

class ScriptButtonsCtrl_CN4 : public ScriptInterface
{
	public:
		void Run(uint16_t id, StateDB::db_t &db_element, tx_t func) override
		{
			if(db_element.data[0] != 0x65) return;

			switch(db_element.data[1])
			{
				case 17:
				{
					
					
					break;
				}
				case 18:
				{
					
					
					break;
				}
				case 19:
				{
					
					
					break;
				}
				case 20:
				{
					
					
					break;
				}
				case 21:
				{
					
					
					break;
				}
				case 22:
				{
					
					
					break;
				}
				case 23:
				{
					
					
					break;
				}
				case 24:
				{
					
					
					break;
				}
				default:
				{
					break;
				}
			}
			
			
			
			return;
		}
};

class ScriptButtonsCtrl_CN5 : public ScriptInterface
{
	public:
		void Run(uint16_t id, StateDB::db_t &db_element, tx_t func) override
		{
			if(db_element.data[0] != 0x65) return;

			switch(db_element.data[1])
			{
				case 25:
				{
					
					
					break;
				}
				case 26:
				{
					
					
					break;
				}
				case 27:
				{
					
					
					break;
				}
				case 28:
				{
					
					
					break;
				}
				case 29:
				{
					
					
					break;
				}
				case 30:
				{
					
					
					break;
				}
				case 31:
				{
					
					
					break;
				}
				case 32:
				{
					
					
					break;
				}
				default:
				{
					break;
				}
			}
			
			
			
			return;
		}
};