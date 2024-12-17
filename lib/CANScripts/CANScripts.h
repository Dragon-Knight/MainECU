#pragma once

#include <inttypes.h>
#include <L2Wrapper.h>


typedef std::function<void(L2Wrapper::packet_v2_t &can_obj)> tx_t;


class ScriptInterface
{
	public:
		virtual ~ScriptInterface() = default;
		
		virtual void Run(uint16_t id, StateDB::db_t &db_element, tx_t func) = 0;
		
		static L2Wrapper *l2_obj;
		static StateDB *db_obj;
		
	protected:
		
		template <typename T> 
		T Map(T input, T input_min, T input_max, T output_min, T output_max)
		{
			return (((input - input_min) * (output_max - output_min)) / (input_max - input_min)) + output_min;
		}
		
		template <typename T> 
		T MapClump(T input, T input_min, T input_max, T output_min, T output_max)
		{
			if(input < input_min) input = input_min;
			if(input > input_max) input = input_max;
			
			return (((input - input_min) * (output_max - output_min)) / (input_max - input_min)) + output_min;
		}
		
		inline bool Send(uint16_t id, uint8_t data[8], uint8_t length)
		{
			return l2_obj->Send(id, data, length);
		}
		
		L2Wrapper::packet_v2_t _tx_packet;
		
};
L2Wrapper* ScriptInterface::l2_obj = nullptr;
StateDB* ScriptInterface::db_obj = nullptr;

#include "ScriptsCore.h"
#include "ScriptsLight.h"


class CANScripts
{
	public:

		CANScripts(L2Wrapper *l2_obj, StateDB *db_obj) : _l2_obj(l2_obj)
		{
			ScriptInterface::l2_obj = l2_obj;
			ScriptInterface::db_obj = db_obj;
			
			memset(&_obj, 0x00, sizeof(_obj));
			
			// Обработка 'запуска' двигателя
			_obj[0x0101] = new ScriptPowerOnOff();
			
			// Передача и фактическое направление вращения колёс
			_obj[0x010A] = new Script_010A();

			// Кнопка 01, Ближний свет.
			_obj[0x0124] = new ScriptSideLowHighBeam();
			
			// Кнопка 02, Габариты.
			_obj[0x0125] = new ScriptSideLowHighBeam();
			
			// Кнопка 03, Свет в салоне.
			_obj[0x0126] = new ScriptCabinLight();
			
			// Кнопка 04, Клаксон.
			_obj[0x0127] = new ScriptHorn();
			
			// Кнопка 05, Аварийка.
			_obj[0x0128] = new ScriptLeftRightHazard();
			
			// Кнопка 06, Вентилятор.
			_obj[0x0129] = nullptr;
			
			// Кнопка 07, Левый поворотник.
			_obj[0x012A] = new ScriptLeftRightHazard();
			
			// Кнопка 08, Педаль тормоза.
			_obj[0x012B] = new ScriptBrakeLight();
			
			// Кнопка 09, Кнопка открытия капота.
			_obj[0x012C] = new ScriptHoodTrunk();
			
			// Кнопка 10, Кнопка открытия багажника.
			_obj[0x012D] = new ScriptHoodTrunk();
			
			// Кнопка 11, Кнопка открытия левой двери.
			_obj[0x012E] = new ScriptLeftRightDoor();
			
			// Кнопка 12, Правый поворотник.
			_obj[0x012F] = new ScriptLeftRightHazard();
			
			// Кнопка 13, Концевик левая дверь.
			_obj[0x0130] = new ScriptCabinLight();
			
			// Кнопка 14, Концевик правая дверь.
			_obj[0x0131] = new ScriptCabinLight();
			
			// Кнопка 15, Кнопка открытия правой двери.
			_obj[0x0132] = new ScriptLeftRightDoor();
			
			// Кнопка 16, Дальний свет.
			_obj[0x0133] = new ScriptSideLowHighBeam();
			
			// Подрулевой переключатель 1, йййй.
			_obj[0x0134] = nullptr;

			// Подрулевой переключатель 2, йййй.
			_obj[0x0135] = nullptr;

			// Вход педали газа на плате IO.
			_obj[0x016C] = new ScriptThrottleCtrl();

			// Кнопки
			_obj[0x0224] = new ScriptButtonsCtrl_CN2();
			_obj[0x0225] = new ScriptButtonsCtrl_CN3();
			_obj[0x0226] = new ScriptButtonsCtrl_CN4();
			_obj[0x0227] = new ScriptButtonsCtrl_CN5();
			
			return;
		}

		void Processing(uint16_t id, StateDB::db_t &db_element)
		{
			if(id >= 2048) return;
			if(_obj[id] == 0 /*nullptr*/) return;

			_obj[id]->Run(id, db_element, [&](L2Wrapper::packet_v2_t &can_obj)
			{
				_l2_obj->Send(can_obj);
			});
			
			return;
		}

	private:
		ScriptInterface *_obj[2048];
		L2Wrapper *_l2_obj;
		
};
