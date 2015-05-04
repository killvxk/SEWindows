#pragma once
#include <WinIoCtl.h>

#define IOCTL_STOP_ALL				CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1001, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_START_ALL				CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1002, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SET_INTERCEPT_MODE	CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1003, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_START_PROCMONITOR		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1004, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_PAUSE_PROCMONITOR		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1005, METHOD_BUFFERED, FILE_ANY_ACCESS)  
#define IOCTL_START_REGMONITOR		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1006, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_PAUSE_REGMONITOR		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1007, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_START_FILEMONITOR		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1008, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_PAUSE_FILEMONITOR		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1009, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_STOP_UNLOAD			CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1010, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_PERMIT_UNLOAD			CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1011, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SET_NOTIFY_MODE		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1012, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_TRANSFER_SYSROOT		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1013, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_REMOVE_HOOK			CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1014, METHOD_BUFFERED, FILE_ANY_ACCESS) 
#define IOCTL_START_SVCMONITOR		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1015, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_STOP_SVCMONITOR		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1016, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_TRANSFER_INJECT_DLL	CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1017, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define	IOCTL_FROM_R3MSG			CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1100, METHOD_BUFFERED, FILE_ANY_ACCESS) 

typedef BOOLEAN(*NotifyProc)(Param& opdata);

class ScmDrvCtrl;
class CCommunicateDriv
{
public:
	CCommunicateDriv(void);
	~CCommunicateDriv(void);
	BOOLEAN	InitDriver(TCHAR* lpsz_service_name, TCHAR* lpsz_path, TCHAR* lpsz_altitude, TCHAR* lpsz_portname, TCHAR* lpszLink_name);
	BOOLEAN	OnExitProcess(TCHAR* lpsz_service_name);
	BOOLEAN	UninsDriver(TCHAR* lpsz_service_name);
	BOOLEAN	TransferSysroot(TCHAR* lpsz_sysroot);
	BOOLEAN	TransferInjectDll(TCHAR* lpsz_sysroot);
	BOOLEAN	StartProcMon(BOOLEAN Start);
	BOOLEAN	StartRegMon(BOOLEAN Start);
	BOOLEAN	StartFileMon(BOOLEAN Start);
	BOOLEAN	StartSvcMon(BOOLEAN Start);
	BOOLEAN	StopUnload();
	BOOLEAN	PermitUnload();
	BOOLEAN	StartToWork();
	BOOLEAN	RemoveHook();
	BOOLEAN	SetMode(BOOLEAN bIsNotityMode);
	VOID	CloseComplete();
protected:
	HANDLE		m_port;
	HANDLE		m_completion;
	NotifyProc  m_funcNotify;
	HANDLE		m_threads[THREAD_COUNT];
	PSCANNER_MESSAGE m_msg[THREAD_COUNT];
	void*		m_pMagPoint[REQUEST_COUNT*THREAD_COUNT];
	int			m_iMsgCount;
public:
	static  DWORD WINAPI MonitorProc(LPVOID param);
	VOID		SetNotify(NotifyProc notify);
	TCHAR		m_errStr[100];
	ScmDrvCtrl* m_pScmDrvCtrl;
};