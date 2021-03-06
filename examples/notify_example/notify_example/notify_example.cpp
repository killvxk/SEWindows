/*
  notify_example : print file create

  --dir-------notify_example.exe
         |----sewindows.sys
         |----sewindows.dll
*/

#include <stdio.h>
#include <tchar.h>
#include <locale.h>
#include <windows.h>
#include "sewindows.h"

#pragma comment(lib,"Advapi32.lib") 
#pragma comment(lib,"User32.lib") 

typedef BOOLEAN(*fsewin_init)();
typedef BOOLEAN(*fsewin_setoption)(int mode, int type);
typedef BOOLEAN(*fsewin_register_opt)(struct sewin_operations *ops);

fsewin_init         monitor_sewin_init;
fsewin_setoption    monitor_sewin_setoption;
fsewin_register_opt monitor_sewin_register_opt;

BOOLEAN  monitor_svc_change(WCHAR *user_name, WCHAR *process, WCHAR *file_path)
{
    wprintf(_T("monitor_svc_change\n User=%s, Process=%s, file=%s\n"), user_name, process, file_path);
    return FALSE;
}

BOOLEAN  disk_read(WCHAR *user_name, WCHAR *process, WCHAR *file_path)
{
	wprintf(_T("disk_read\n User=%s, Process=%s, file=%s\n"), user_name, process, file_path);
	return TRUE;
}

BOOLEAN  disk_write(WCHAR *user_name, WCHAR *process, WCHAR *file_path)
{
	wprintf(_T("disk_write\n User=%s, Process=%s, file=%s\n"), user_name, process, file_path);
	return TRUE;
}

BOOLEAN  disk_format(WCHAR *user_name, WCHAR *process, WCHAR *file_path)
{
	wprintf(_T("disk_format\n User=%s, Process=%s, file=%s\n"), user_name, process, file_path);
	return FALSE;
}


BOOLEAN  monitor_svc_delete(WCHAR *user_name, WCHAR *process, WCHAR *file_path)
{
	wprintf(_T("monitor_svc_delete\n  User=%s, Process=%s, file=%s\n"), user_name, process, file_path);
	return FALSE;
}

BOOLEAN  monitorprocess_kill(WCHAR *user_name, WCHAR *process, WCHAR *file_path)
{
	wprintf(_T("monitorprocess_kill\n  User=%s, Process=%s, file=%s\n"), user_name, process, file_path);
	return FALSE;
}

BOOLEAN  monitor_svc_create(WCHAR *user_name, WCHAR *process, WCHAR *file_path, WCHAR *bin)
{
	wprintf(_T("monitor_svc_create \n User=%s, Process=%s, sername=%s\n, bin=%s\n"), user_name, process, file_path, bin);
	return FALSE;
}
BOOLEAN  monitor_svc_driver(WCHAR *user_name, WCHAR *process, WCHAR *file_path, WCHAR *bin)
{
	wprintf(_T("monitor_svc_driver\n  User=%s, Process=%s, driver=%s\n, bin=%s\n"), user_name, process, file_path, bin);
	return FALSE;
}
int _tmain(int argc, TCHAR * argv[])
{
    int   ret   = 0;
    sewin_operations ops;
    HMODULE handle;

    setlocale(LC_ALL, "chs");

    // step1. loadLibrary sewindows.dll
    handle                     = LoadLibrary(_T("sewindows.dll"));
    monitor_sewin_init         = (fsewin_init)GetProcAddress(handle, "sewin_init");
    monitor_sewin_setoption    = (fsewin_setoption)GetProcAddress(handle, "sewin_setoption");
    monitor_sewin_register_opt = (fsewin_register_opt)GetProcAddress(handle, "sewin_register_opt");

    if (monitor_sewin_init == NULL || monitor_sewin_setoption == NULL || monitor_sewin_register_opt == NULL)
    {
        return -1;
    }

#ifdef	DEBUG
	DebugBreak();
#endif
    // step2. init sewindows
    BOOLEAN bret = monitor_sewin_init();
    if ( !bret )
    {
        return -2;
    }

    // step3. set options
    //monitor_sewin_setoption(SEWIN_MODE_INTERCEPT, SEWIN_TYPE_FILE|SEWIN_TYPE_PROC|SEWIN_TYPE_REG);
	monitor_sewin_setoption(SEWIN_MODE_INTERCEPT, SEWIN_TYPE_SCVDRV | SEWIN_TYPE_FILE | SEWIN_TYPE_PROC | SEWIN_TYPE_REG);
    
    // step4. register callbak functions
    memset(&ops, 0x00, sizeof(struct sewin_operations));
	ops.service_change = monitor_svc_change;
	ops.service_delete = monitor_svc_delete;
	ops.service_create = monitor_svc_create;
	ops.driver_load = monitor_svc_driver;
	ops.disk_format = disk_format;
	ops.disk_read = disk_read;
	ops.disk_write = disk_write;

	ops.process_kill = monitorprocess_kill;
    monitor_sewin_register_opt(&ops);

    printf("Start Working (Ctrl + 'C' to exists) ...\n");

    while(1){
        Sleep(3000);
    }

    return 0;
}
