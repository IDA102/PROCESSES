/*_____________������� ��������� ������(dwCurrentState)_____________
(0x5)SERVICE_CONTINUE_PENDING--->����������� ������ ������ ���������.
(0x6)SERVICE_PAUSE_PENDING------>������������ ������ ������ ���������.
(0x7)SERVICE_PAUSED------------->������ ��������������.
(0x4)SERVICE_RUNNING------------>������ � ������� ���������.
(0x2)SERVICE_START_PENDING------>������ �����������.
(0x3)SERVICE_STOP_PENDING------->������ ���������������.
(0x1)SERVICE_STOPPED------------>������ �� � ������� ���������.
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
