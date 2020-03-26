/*_____________Текущее состояние службы(dwCurrentState)_____________
(0x5)SERVICE_CONTINUE_PENDING--->Продолжение работы службы ожидается.
(0x6)SERVICE_PAUSE_PENDING------>Приостановка работы службы ожидается.
(0x7)SERVICE_PAUSED------------->Служба приостановлена.
(0x4)SERVICE_RUNNING------------>Служба в рабочем состоянии.
(0x2)SERVICE_START_PENDING------>Служба запускается.
(0x3)SERVICE_STOP_PENDING------->Служба останавливается.
(0x1)SERVICE_STOPPED------------>Служба не в рабочем состоянии.
*/
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define	  stop __asm nop
#include <Windows.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;
