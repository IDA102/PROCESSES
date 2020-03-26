#include "H.h"

enum status { Running, Stopping, Paused };
enum management_teams { Start, Stop, Restart };

//������� ������� ������ 
bool START_SERVICE(SC_HANDLE &descriptor)
{
	BOOL ret = StartService(descriptor, NULL, NULL);//3
	return ret;
	/*
	����� �� ����� ������� ������ �� ������� ��� ���������� ������ ��� � "ControlService" f STOP_SERVICE, �.�. ������� ���� ������� ���������������� ������� ������
	� ������������ ������ ������� ����� ���������, ��� ������ ������� �������(��� 20 ��� + ����� ��������� ���������, ����������� ��� ���). 
	�������� "����� ����������� ��������� �������, ������� StartService �� ���������� �������� �� ��� ���, ���� ������� ���������� ��  �������� �������������."
	*/
}

//������� ��������� ������
bool STOP_SERVICE(SC_HANDLE &descriptor)
{//GetLastError ��� �� ��������� �������, ���� �������� ����� �� ��������� ��� ���������� ������� ������
	SERVICE_STATUS status;
	BOOL ret = ControlService(descriptor, SERVICE_CONTROL_STOP, &status);
	if (ret) { while (!((status.dwCurrentState != 1)^(status.dwCurrentState != 3))); }
	if (!ret) { DWORD GLE = GetLastError(); if ((GLE == 3435973836) || (GLE == 1062)) return true; }/*3435973836 � 1062<--- ��� �������� ������, ���� ��������� ������� ���������� ��� ������������� ������*/
	return ret;
}

//������� ����������� ������
int RESTART_SERVICE(SC_HANDLE &descriptor)
{
	int k = 0;
	if (!STOP_SERVICE(descriptor)) { k = 2; return k;	}
	if (!START_SERVICE(descriptor)) { k = 1; return k;	}
	return k;
}

int main()
{
	setlocale(LC_ALL, "Russian");

	struct SERVICE
	{
		string NAME;
		string GROUP;
		string DECRIPTION;
		string IMAGE_PATH;
		size_t PID = 0;
		status STATUS;
	};
 
	SC_HANDLE discriptor = nullptr, discriptor_service = nullptr;//���������� ����� ��������� ������.
	bool err_discriptor,err_config_service;//���� ������
	ENUM_SERVICE_STATUS_PROCESS *SERVICE_BUFER;//��������� �� ����� � ������� ���������� ���������� "����" �����
	QUERY_SERVICE_CONFIG *SERVICE_CONFIG_BUFER;//��������� �� ����� � ������� ���������� ���������� ��������� ������
	DWORD dwBytesNeeded = 0;//������� ��������� ������ � ������ ��� ����������� ���� ���������
	DWORD dwBytesNeeded2 = 0;//������� ��������� ������ � ������ ��� ����������� ������ ������������ ��������
	DWORD dwServicesReturned = 0;//���������� ������������ �����
	DWORD dwResumeHandle = 0;

	//------����� �������� ����� ������� �� ��������, �� ��� �� ��� �����
	//LPQUERY_SERVICE_CONFIG lpqscBuf = (LPQUERY_SERVICE_CONFIG)LocalAlloc(LPTR, 4096);
	//LPBYTE lpqscBuf = (LPBYTE)LocalAlloc(LPTR, (size_t)pcbBytesNeeded);

	//��������� ���������� ���������
	if (discriptor = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS))//SC_MANAGER_ENUMERATE_SERVICE
	{
		//�������� ������ ��� ������ ������������ (������ ����� dwBytesNeeded)
		err_discriptor = EnumServicesStatusEx(
			discriptor,
			SC_ENUM_PROCESS_INFO,
			SERVICE_WIN32,//SERVICE_DRIVER
			SERVICE_STATE_ALL,
			NULL,
			0,
			&dwBytesNeeded,
			&dwServicesReturned,
			NULL,
			NULL
		);

		//�������� ������ � ������ ����� ����������� �����
		DWORD dwBytes = dwBytesNeeded + sizeof(ENUM_SERVICE_STATUS_PROCESS);
		//SERVICE_BUFER = new ENUM_SERVICE_STATUS_PROCESS[dwBytes];//���� ������������ ��� �������, �� �������� �������� delete
		SERVICE_BUFER = (ENUM_SERVICE_STATUS_PROCESS*)LocalAlloc(LPTR, dwBytes);
		if (SERVICE_BUFER == NULL)	{return FALSE;}

		err_discriptor = EnumServicesStatusEx(
			discriptor,
			SC_ENUM_PROCESS_INFO,
			SERVICE_WIN32,//SERVICE_DRIVER
			SERVICE_STATE_ALL,
			(LPBYTE)SERVICE_BUFER,
			dwBytes,
			&dwBytesNeeded,
			&dwServicesReturned,
			&dwResumeHandle,
			NULL
		);

		//�������� ������ ��� �������� ����� ������������ � �++++
		vector<SERVICE> SERVICE_VECTOR((size_t)dwServicesReturned);

		for (size_t i = 0 ; i < (size_t)dwServicesReturned; ++i)
		{

			//��������� ������
			try 
			{
				discriptor_service = OpenService(discriptor, SERVICE_BUFER[i].lpServiceName, SERVICE_ALL_ACCESS);
			}
			catch (const std::exception)
			{
				cout << "Read memory error in process descriptor--------cath(std::exception)" << endl;
			}
			catch (...)
			{
				std::cerr << "Read memory error in process descriptor!--------cath(...)\n";
				stop
			}
			
 			err_config_service = QueryServiceConfig
			(
				discriptor_service,
				NULL,
				NULL,
				&dwBytesNeeded2
			);

			//����������� ���� ���� � ������
			DWORD dwBytes2 = dwBytesNeeded2 + sizeof(SERVICE_CONFIG_BUFER);
			//SERVICE_CONFIG_BUFER = new QUERY_SERVICE_CONFIG[dwBytes2];//���� ������������ ��� �������, �� �������� �������� delete
			SERVICE_CONFIG_BUFER = (QUERY_SERVICE_CONFIG*)LocalAlloc(LPTR, dwBytes2);
			if (SERVICE_BUFER == NULL) { return FALSE; }

			err_config_service = QueryServiceConfig
			(
				discriptor_service,
				SERVICE_CONFIG_BUFER,
				dwBytes2,
				&dwBytesNeeded2
			);
			//��������� ������ ��������
			SERVICE_VECTOR[i].NAME = SERVICE_BUFER[i].lpServiceName;
			SERVICE_VECTOR[i].PID = SERVICE_BUFER[i].ServiceStatusProcess.dwProcessId;
			SERVICE_VECTOR[i].DECRIPTION = SERVICE_BUFER[i].lpDisplayName;
			SERVICE_VECTOR[i].STATUS = (status)SERVICE_BUFER[i].ServiceStatusProcess.dwServiceFlags;
			try
			{
			string cmp = SERVICE_CONFIG_BUFER->lpBinaryPathName;
			if (SERVICE_CONFIG_BUFER->lpBinaryPathName != NULL)
			{
				int  o = cmp.find(" -k ");
				if (o != -1)
				{
					SERVICE_VECTOR[i].GROUP = &cmp[o + 4]; 
					cmp[o] = '\0';
					SERVICE_VECTOR[i].IMAGE_PATH = cmp;
				}
				else SERVICE_VECTOR[i].IMAGE_PATH = cmp;
			}
			}
			catch (...)
			{
				std::cerr << "Read memory error in process descriptor in service --- " << i << endl;
			}
			CloseServiceHandle(discriptor_service);
			LocalFree(SERVICE_CONFIG_BUFER);
			stop		
		}

		//��������� ������
		discriptor_service = OpenService(discriptor, SERVICE_BUFER[0].lpServiceName, SERVICE_ALL_ACCESS);
		int k = START_SERVICE(discriptor_service);
		CloseServiceHandle(discriptor_service);
		stop

		//������������� ������
		discriptor_service = OpenService(discriptor, SERVICE_BUFER[0].lpServiceName, SERVICE_STOP);
		k = STOP_SERVICE(discriptor_service);
		CloseServiceHandle(discriptor_service);
		stop

		//������������� ������
		/*
		���� err ����� ������ --- 1(������ �������)
		���� err ����� ������ --- 2(������ ��������)
		*/
		discriptor_service = OpenService(discriptor, SERVICE_BUFER[0].lpServiceName, SERVICE_ALL_ACCESS);
		int err =  RESTART_SERVICE(discriptor_service);
		CloseServiceHandle(discriptor_service);
		stop

		LocalFree(SERVICE_BUFER);

	}
	else cout << "not creat descriptor" << endl;
	stop
	CloseServiceHandle(discriptor);//���������/����������� ���������� �����
	return 0;
}	
