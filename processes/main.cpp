#include "H.h"

void main()
{

	SC_HANDLE ret,res;
	bool rec;
	LPCSTR lpMachineName;
	LPCSTR lpDatabaseName;
	DWORD  dwDesiredAccess;

	DWORD pcbBytesNeeded = 0;
	DWORD lpServicesReturned = 0;
	DWORD lpResumeHandle = 0;


	ENUM_SERVICE_STATUS *lpservice;

	DWORD dwBytesNeeded = 0;
	DWORD dwServicesReturned = 0;
	DWORD dwResumeHandle = 0;
	BOOL bSuccess = FALSE;


	LPQUERY_SERVICE_CONFIG lpqscBuf = (LPQUERY_SERVICE_CONFIG)LocalAlloc(LPTR, 4096);


	if (ret = OpenSCManager(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE))//SC_MANAGER_ALL_ACCESS
	{
		//res =OpenService(ret, "qwe", SC_MANAGER_ALL_ACCESS);
		//QueryServiceConfig(ret, lpqscBuf, 4096, &dwBytesNeeded);

		rec = EnumServicesStatusEx(
			ret,
			SC_ENUM_PROCESS_INFO,
			SERVICE_WIN32,
			SERVICE_STATE_ALL,
			NULL,
			0,
			&dwBytesNeeded,
			&dwServicesReturned,
			&dwResumeHandle,
			NULL
		);

		//LPBYTE lpqscBuf = (LPBYTE)LocalAlloc(LPTR, (size_t)pcbBytesNeeded);
		DWORD dwBytes = dwBytesNeeded + sizeof(ENUM_SERVICE_STATUS);
		lpservice = new ENUM_SERVICE_STATUS[dwBytes];
		rec = EnumServicesStatusEx(
			ret,
			SC_ENUM_PROCESS_INFO,
			SERVICE_WIN32,
			SERVICE_STATE_ALL,
			(LPBYTE)lpservice,
			dwBytes,
			&dwBytesNeeded,
			&dwServicesReturned,
			&dwResumeHandle,
			NULL
		);



	}
	else cout << "not creat descriptor" << endl;


	stop
	LocalFree(lpqscBuf);
	CloseServiceHandle(ret);
}	



//hsv = CreateService(hsc,"giveio", "giveio",	SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, SERVICE_ERROR_IGNORE,	filepath,	NULL, NULL, NULL, NULL, NULL)) != NULL) 