/*
*	S.T.A.L.K.E.R. Online - Events sync
*	Developers:
*		009 (Михайлов Алексей)
*/

#include "../shared/hook.h"
#include "../shared/log.h"

extern CLog* LogHandle;
extern CHook* HookHandle;

void CLevel__cl_Process_CustomEvent(NET_Packet& pack)
{
	LogHandle->Write("called CLevel::cl_Process_CustomEvent");
}