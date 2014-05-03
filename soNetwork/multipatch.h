/*
*	S.T.A.L.K.E.R. Online - network hooks for different patches
*	Developers:
*		009 (�������� �������)
*/

#pragma once

#include "../shared/hook.h"

class CHook_Patch_00: public CHook
{
public:
	CHook_Patch_00(void);
	~CHook_Patch_00(void);

	void InstallHooks();
};

class CHook_Patch_01: public CHook
{
public:
	CHook_Patch_01(void);
	~CHook_Patch_01(void);

	void InstallHooks();
};

class CHook_Patch_02: public CHook
{
public:
	CHook_Patch_02(void);
	~CHook_Patch_02(void);

	void InstallHooks();
	WORD CreateObject(char* section,char* name,float pos_x,float pos_y,float pos_z,WORD parentid);
	void PlayCycle(DWORD pClass,DWORD MotionID);
	void PlayCycleEx(DWORD pClass,DWORD unk,DWORD MotionID);
	void SendPacketToAll(NET_Packet& pack);
	bool IsServer();
	void LoadAImap();
	void LoadMapScript();
};