# MainECU

## Важное
Эта версия прошивки предназначена для работы с платами версии 1 и больше не развивается. В этой ветке будут производиться только устранения ошибок. Если вы только начинаете работу с Пикселем или у вас более свежие модификации железа, то ищите прошивку в ветке [board_v2](https://github.com/starfactorypixel/MainECU/tree/board_v2).

## Описание
Проект в Visual Studio Code + PlatformIO

Для сборки необходимо создать файл `platformio_local.ini` со следующим содержанием:
```ini
[env]
upload_port = COM3
upload_speed = 921600
monitor_port = COM3
build_flags = 
	;-DUSE_EMULATOR
	;-DNO_CAN_SEND
```
Где необходимо указать нужные настройки порта, скорости и опций сборки.
