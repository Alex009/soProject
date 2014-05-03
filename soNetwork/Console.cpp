/*
*	S.T.A.L.K.E.R. Online - Console controll
*	Developers:
*		009 (Михайлов Алексей)
*/

#include <windows.h>

#include "Console.h"
#include "../shared/hook.h"
#include "../shared/log.h"

extern CLog* LogHandle;
extern CHook* HookHandle;

void ConsoleThread(void*)
{
	LogHandle->Write("Console controll thread started");
	while(true)
	{
		/*
			if(GetAsyncKeyState(VK_NUMPAD0))
			{
				while(GetAsyncKeyState(VK_NUMPAD0)) Sleep(10);
				LogHandle->Write("try execute console command mm off");
				HookHandle->Console->Execute("main_menu off");
			}
			if(GetAsyncKeyState(VK_NUMPAD1))
			{
				while(GetAsyncKeyState(VK_NUMPAD1)) Sleep(10);
				LogHandle->Write("try execute console command mm on");
				HookHandle->Console->Execute("main_menu on");
			}
			if(GetAsyncKeyState(VK_NUMPAD2))
			{
				while(GetAsyncKeyState(VK_NUMPAD2)) Sleep(10);
				LogHandle->Write("try execute console command start server");
				HookHandle->Console->Execute("start server(all/single/alife/new) client(localhost)");
			}
			if(GetAsyncKeyState(VK_NUMPAD3))
			{
				while(GetAsyncKeyState(VK_NUMPAD3)) Sleep(10);
				LogHandle->Write("try execute console command disconnect");
				HookHandle->Console->Execute("disconnect");
			}
		*/
		Sleep(10);
	}
	LogHandle->Write("Console controll thread completed");
}