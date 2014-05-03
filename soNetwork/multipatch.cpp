/*
*	S.T.A.L.K.E.R. Online - network hooks for different patches
*	Developers:
*		009 (Михайлов Алексей)
*/

#include "multipatch.h"
// creatures
#include "Creatures/Client/CAI_Bloodsucker.h"
#include "Creatures/Client/CAI_Boar.h"
#include "Creatures/Client/CAI_Crow.h"
#include "Creatures/Client/CAI_Dog.h"
#include "Creatures/Client/CAI_Flesh.h"
#include "Creatures/Client/CAI_PseudoDog.h"
#include "Creatures/Client/CAI_Rat.h"
#include "Creatures/Client/CAI_Stalker.h"
#include "Creatures/Client/CAI_Trader.h"
#include "Creatures/Client/CBurer.h"
#include "Creatures/Client/CCat.h"
#include "Creatures/Client/CChimera.h"
#include "Creatures/Client/CController.h"
#include "Creatures/Client/CFracture.h"
#include "Creatures/Client/CPhantom.h"
#include "Creatures/Client/CPoltergeist.h"
#include "Creatures/Client/CPseudoGigant.h"
#include "Creatures/Client/CPsyDog.h"
#include "Creatures/Client/CPsyDogPhantom.h"
#include "Creatures/Client/CSnork.h"
#include "Creatures/Client/CTushkano.h"
#include "Creatures/Client/CZombie.h"
#include "Creatures/Server/CSE_ALifeCreatureCrow.h"
#include "Creatures/Server/CSE_ALifeCreaturePhantom.h"
#include "Creatures/Server/CSE_ALifeCreatureRat.h"
#include "Creatures/Server/CSE_ALifeHumanStalker.h"
#include "Creatures/Server/CSE_ALifeMonsterBase.h"
#include "Creatures/Server/CSE_ALifeSmartZone.h"
#include "Creatures/Server/CSE_ALifeTrader.h"
#include "Creatures/Server/CSE_ALifePsyDogPhantom.h"
#include "events.h"
#include "motions.h"

#include "../shared/log.h"

// extern
extern CLog* LogHandle;
extern BYTE	 PatchVersion;
extern CHook* HookHandle;

// vars
DWORD (_cdecl * lua_alife__create)(DWORD _alife,char* _section,float* _position,DWORD _lvertex,WORD _gvertex,WORD _parentid);
DWORD (_cdecl * lua_alife)();
DWORD (_cdecl * F_entity_Create)(char* section);
DWORD (_cdecl * ai)();
DWORD (_cdecl * CObjectItemClientServer_CActor_CSE_ALifeCreatureActor___client_object)();
DWORD pIPureServer__SendTo;
DWORD pCKinematicsAnimated__PlayCycle;
DWORD pCKinematicsAnimated__PlayCycleEx;
DWORD pCControlDirection__update_frame;
DWORD pCSE_ALifeCreatureCrow__UPDATE_Write;
DWORD pCSE_ALifeCreatureCrow__UPDATE_Read;
DWORD pCSE_ALifeCreaturePhantom__UPDATE_Write;
DWORD pCSE_ALifeCreaturePhantom__UPDATE_Read;
DWORD pCSE_ALifeCreatureRat__UPDATE_Write;
DWORD pCSE_ALifeCreatureRat__UPDATE_Read;
DWORD pCSE_ALifeHumanStalker__UPDATE_Write;
DWORD pCSE_ALifeHumanStalker__UPDATE_Read;
DWORD pCSE_ALifeMonsterBase__UPDATE_Write;
DWORD pCSE_ALifeMonsterBase__UPDATE_Read;
DWORD pCSE_ALifeTrader__UPDATE_Write;
DWORD pCSE_ALifeTrader__UPDATE_Read;
DWORD pCSE_ALifePsyDogPhantom__UPDATE_Write;
DWORD pCSE_ALifePsyDogPhantom__UPDATE_Read;
DWORD pUPDATE_Write_call_address;
DWORD pUPDATE_Read_call_address;
DWORD pCALifeSimulator__CALifeSimulator;
DWORD pCGameGraph__CGameGraph;
DWORD pCLevel__IsServer;
DWORD pCAI_Space__reinit;
DWORD pCAI_Space__load;
DWORD pCAI_Space__patrol_path_storage;
DWORD pCActor__NeedToDestroyObject;
DWORD pCActor__TimePassedAfterDeath;
DWORD pCDestroyer__CALifeSimulator__delete;
DWORD pCScriptStorage__load_file_into_namespace;  // CScriptStorage::load_file_into_namespace(char* file_path,char* namespace)
DWORD pCScriptBinder__reload;
DWORD pCScriptBinder__reinit;
DWORD pCScriptBinder__net_spawn;
DWORD pCActor__CActor;
DWORD pCActor__net_spawn;
DWORD pCSE_ALifeCreatureActor__CSE_ALifeCreatureActor;
DWORD CSE_ALifeCreatureActor__ftable;
DWORD CSE_ActorMP__ftable;
DWORD CAI_Bloodsucker__ftable;
DWORD CAI_Boar__ftable;
DWORD CAI_Crow__ftable;
DWORD CAI_Dog__ftable;
DWORD CAI_Flesh__ftable;
DWORD CAI_PseudoDog__ftable;
DWORD CAI_Rat__ftable;
DWORD CAI_Stalker__ftable;
DWORD CAI_Trader__ftable;
DWORD CBurer__ftable;
DWORD CCat__ftable;
DWORD CChimera__ftable;
DWORD CController__ftable;
DWORD CFracture__ftable;
DWORD CPhantom__ftable;
DWORD CPoltergeist__ftable;
DWORD CPseudoGigant__ftable;
DWORD CPsyDog__ftable;
DWORD CPsyDogPhantom__ftable;
DWORD CSnork__ftable;
DWORD CTushkano__ftable;
DWORD CZombie__ftable;
DWORD CActorMP__ftable;
DWORD gGameOffset;
DWORD gActorPointer;
char* LevelAlifeSpawnFile = "alife";
char* LevelScriptFile = "location_main";
char* LevelDirectory = "$level$";
void* CustomSyncData_Entity[0xFFFF];	// 0xFFFF - max of Entitys in game
DWORD g_actor;

DWORD pCSE_ALifeMonsterBase__on_death;
DWORD pgame_sv_mp__Player_AddMoney;
DWORD pgame_sv_mp__getPlayerGameState;
DWORD pgame_sv_mp;

// headers
void game_sv_mp__Create_fix(DWORD pClass,DWORD a2);
void CLevel__Load_GameSpecific_Before_fix();
void CControlDirection__update_frame(DWORD pClass);
DWORD F_entity_Create_fix(DWORD pClass);
DWORD CSE_ALifeObject__alife_fix();
DWORD CActor__net_Spawn_fix(DWORD CObject,DWORD CEntity);
DWORD CActor__net_Destroy_fix(DWORD CObject);
DWORD CActor__UpdateCL(DWORD pClass);
bool bind_object_fix();
DWORD CALifeGraphRegistry__update_fix(DWORD pClass);
int CSE_Abstract__on_death_fix(DWORD pClass, DWORD pKillerClass);
void CSE_ALifeMonsterBase__on_death(DWORD pClass, DWORD pKillerClass);

// patch 1.6.0.0

CHook_Patch_00::CHook_Patch_00()
{
	for(int i = 0;i < 0xFFFF;i++) CustomSyncData_Entity[i] = NULL;

	// wait modules load
	while((EngineOffset = GetModuleHandle(L"xrEngine.exe")) == NULL) {}
	while((CoreOffset = GetModuleHandle(L"xrCore.dll")) == NULL) {}
	while((GameOffset = GetModuleHandle(L"xrGame.dll")) == NULL) {}
	while((NetServerOffset = GetModuleHandle(L"xrNetServer.dll")) == NULL) {}
	while(true)
	{
		if((RenderOffset = GetModuleHandle(L"xrRender_R1.dll")) != NULL)
		{
			RenderVersion = 1;
			break;
		}
		if((RenderOffset = GetModuleHandle(L"xrRender_R2.dll")) != NULL)
		{
			RenderVersion = 2;
			break;
		}
		if((RenderOffset = GetModuleHandle(L"xrRender_R3.dll")) != NULL)
		{
			RenderVersion = 3;
			break;
		}
		if((RenderOffset = GetModuleHandle(L"xrRender_R4.dll")) != NULL)
		{
			RenderVersion = 4;
			break;
		}
	}
	while((PhysicsOffset = GetModuleHandle(L"xrPhysics.dll")) == NULL) {}

	PatchVersion = 0;
}

void CHook_Patch_00::InstallHooks()
{
	LogHandle->Write("patch 1.6.0.0 not compatible");
}

// patch 1.6.0.1

CHook_Patch_01::CHook_Patch_01()
{
	for(int i = 0;i < 0xFFFF;i++) CustomSyncData_Entity[i] = NULL;

	// wait modules load
	while((EngineOffset = GetModuleHandle(L"xrEngine.exe")) == NULL) {}
	while((CoreOffset = GetModuleHandle(L"xrCore.dll")) == NULL) {}
	while((GameOffset = GetModuleHandle(L"xrGame.dll")) == NULL) {}
	while((NetServerOffset = GetModuleHandle(L"xrNetServer.dll")) == NULL) {}
	while(true)
	{
		if((RenderOffset = GetModuleHandle(L"xrRender_R1.dll")) != NULL)
		{
			RenderVersion = 1;
			break;
		}
		if((RenderOffset = GetModuleHandle(L"xrRender_R2.dll")) != NULL)
		{
			RenderVersion = 2;
			break;
		}
		if((RenderOffset = GetModuleHandle(L"xrRender_R3.dll")) != NULL)
		{
			RenderVersion = 3;
			break;
		}
		if((RenderOffset = GetModuleHandle(L"xrRender_R4.dll")) != NULL)
		{
			RenderVersion = 4;
			break;
		}
	}

	PatchVersion = 1;
}

void CHook_Patch_01::InstallHooks()
{
	LogHandle->Write("patch 1.6.0.1 not compatible");
}

// patch 1.6.0.2

CHook_Patch_02::CHook_Patch_02()
{
	for(int i = 0;i < 0xFFFF;i++) CustomSyncData_Entity[i] = NULL;

	// wait modules load
	while((EngineOffset = GetModuleHandle(L"xrEngine.exe")) == NULL) Sleep(5);
	while((CoreOffset = GetModuleHandle(L"xrCore.dll")) == NULL) Sleep(5);
	while((GameOffset = GetModuleHandle(L"xrGame.dll")) == NULL) Sleep(5);
	while((NetServerOffset = GetModuleHandle(L"xrNetServer.dll")) == NULL) Sleep(5);
	while(true)
	{
		if((RenderOffset = GetModuleHandle(L"xrRender_R1.dll")) != NULL)
		{
			RenderVersion = 1;
			break;
		}
		if((RenderOffset = GetModuleHandle(L"xrRender_R2.dll")) != NULL)
		{
			RenderVersion = 2;
			break;
		}
		if((RenderOffset = GetModuleHandle(L"xrRender_R3.dll")) != NULL)
		{
			RenderVersion = 3;
			break;
		}
		if((RenderOffset = GetModuleHandle(L"xrRender_R4.dll")) != NULL)
		{
			RenderVersion = 4;
			break;
		}
		Sleep(5);
	}
	while((PhysicsOffset = GetModuleHandle(L"xrPhysics.dll")) == NULL) Sleep(5);

	PatchVersion = 2;
}

void CHook_Patch_02::InstallHooks()
{
	// link addresses and functions
	lua_alife__create = reinterpret_cast<DWORD (_cdecl *)(DWORD,char*,float*,DWORD,WORD,WORD)>((DWORD)(GameOffset) + 0x99490);
	lua_alife = reinterpret_cast<DWORD (_cdecl *)()>((DWORD)(GameOffset) + 0x97780);
	F_entity_Create = reinterpret_cast<DWORD (_cdecl *)(char*)>((DWORD)(GameOffset) + 0x3B4AB0);
	ai = reinterpret_cast<DWORD (_cdecl *)()>((DWORD)(GameOffset) + 0x39A30);
	CObjectItemClientServer_CActor_CSE_ALifeCreatureActor___client_object = reinterpret_cast<unsigned long (_cdecl *)()>((DWORD)(GameOffset) + 0x33E830);
	pIPureServer__SendTo = ((DWORD)NetServerOffset + 0xAC10);
	pCControlDirection__update_frame = ((DWORD)GameOffset + 0xE19A0);
	pCSE_ALifeCreatureCrow__UPDATE_Write = (DWORD)GameOffset + 0x403570;
	pCSE_ALifeCreatureCrow__UPDATE_Read = (DWORD)GameOffset + 0x403560;
	pCSE_ALifeCreaturePhantom__UPDATE_Write = (DWORD)GameOffset + 0x403570;
	pCSE_ALifeCreaturePhantom__UPDATE_Read = (DWORD)GameOffset + 0x403560;
	pCSE_ALifeCreatureRat__UPDATE_Write = (DWORD)GameOffset + 0x4035E0;
	pCSE_ALifeCreatureRat__UPDATE_Read = (DWORD)GameOffset + 0x403580;
	pCSE_ALifeHumanStalker__UPDATE_Write = (DWORD)GameOffset + 0x406720;
	pCSE_ALifeHumanStalker__UPDATE_Read = (DWORD)GameOffset + 0x406700;
	pCSE_ALifeMonsterBase__UPDATE_Write = (DWORD)GameOffset + 0x4036F0;
	pCSE_ALifeMonsterBase__UPDATE_Read = (DWORD)GameOffset + 0x403660;
	pCSE_ALifeTrader__UPDATE_Write = (DWORD)GameOffset + 0x403840;
	pCSE_ALifeTrader__UPDATE_Read = (DWORD)GameOffset + 0x403840;
	pCSE_ALifePsyDogPhantom__UPDATE_Write = (DWORD)GameOffset + 0x4036F0;
	pCSE_ALifePsyDogPhantom__UPDATE_Read = (DWORD)GameOffset + 0x4036E0;
	pUPDATE_Write_call_address = (DWORD)GameOffset + 0x34C7E8;
	pUPDATE_Read_call_address = (DWORD)GameOffset + 0x438911;
	pCALifeSimulator__CALifeSimulator = (DWORD)GameOffset + 0x96970;
	pCGameGraph__CGameGraph = (DWORD)GameOffset + 0xA8780;
	pCLevel__IsServer = (DWORD)GameOffset + 0x232E20;
	pCAI_Space__load = (DWORD)GameOffset + 0x227C60;
	pCAI_Space__reinit = (DWORD)GameOffset + 0x227A80;
	pCAI_Space__patrol_path_storage = (DWORD)GameOffset + 0x2268A0;
	pCActor__NeedToDestroyObject = (DWORD)GameOffset + 0x25F4C0;
	pCActor__TimePassedAfterDeath = (DWORD)GameOffset + 0x25F530;
	pCDestroyer__CALifeSimulator__delete = (DWORD)GameOffset + 0x387F90;
	pCScriptStorage__load_file_into_namespace = (DWORD)GameOffset + 0x225320;
	pCScriptBinder__reload = (DWORD)GameOffset + 0xC1A40;
	pCScriptBinder__reinit = (DWORD)GameOffset + 0xC1570;
	pCScriptBinder__net_spawn = (DWORD)GameOffset + 0xC15E0;
	pCActor__CActor = (DWORD)GameOffset + 0x264F60;
	pCActor__net_spawn = (DWORD)GameOffset + 0x26CB80;
	pCSE_ALifeCreatureActor__CSE_ALifeCreatureActor = (DWORD)GameOffset + 0x405FC0;
	CSE_ALifeCreatureActor__ftable = (DWORD)GameOffset + 0x597644; 
	CSE_ActorMP__ftable = (DWORD)GameOffset + 0x54C0BC;
	CAI_Bloodsucker__ftable = (DWORD)GameOffset + 0x51F194;
	CAI_Boar__ftable = (DWORD)GameOffset + 0x5207D4;
	CAI_Crow__ftable = (DWORD)GameOffset + 0x5219CC;
	CAI_Dog__ftable = (DWORD)GameOffset + 0x52E27C;
	CAI_Flesh__ftable = (DWORD)GameOffset + 0x53023C;
	CAI_PseudoDog__ftable = (DWORD)GameOffset + 0x53141C;
	CAI_Rat__ftable = (DWORD)GameOffset + 0x52FA8C;
	CAI_Stalker__ftable = (DWORD)GameOffset + 0x534774;
	CAI_Trader__ftable = (DWORD)GameOffset + 0x5334B4;
	CBurer__ftable = (DWORD)GameOffset + 0x5222D4;
	CCat__ftable = (DWORD)GameOffset + 0x52B7C4;
	CChimera__ftable = (DWORD)GameOffset + 0x52362C;
	CController__ftable = (DWORD)GameOffset + 0x52A37C;
	CFracture__ftable = (DWORD)GameOffset + 0x527FBC;
	CPhantom__ftable = (DWORD)GameOffset + 0x52DAF4;
	CPoltergeist__ftable = (DWORD)GameOffset + 0x5259FC;
	CPseudoGigant__ftable = (DWORD)GameOffset + 0x52471C;
	CPsyDog__ftable = (DWORD)GameOffset + 0x532BF4;
	CPsyDogPhantom__ftable = (DWORD)GameOffset + 0x5325F4;
	CSnork__ftable = (DWORD)GameOffset + 0x5290F4;
	CTushkano__ftable = (DWORD)GameOffset + 0x52C9B4;
	CZombie__ftable = (DWORD)GameOffset + 0x526F6C;
	CActorMP__ftable = (DWORD)GameOffset + 0x54BBBC;
	gGameOffset = (DWORD)GameOffset;
	gActorPointer = (DWORD)GameOffset + 0x64E2C0;
	
	// setup fixs
	// fix game_sv_mp::Create (call our func)
	/*
		mov eax, [esp+4+arg4]			8B 44 24 08
		push eax						50
		push esi						56
		call game_sv_mp__Create_fix
		pop eax							58
		pop eax							58
		pop eax							58
		jmp end_func
	*/
	Byte(((DWORD)GameOffset + 0x38B1C0 + 0),1,0x8B);
	Byte(((DWORD)GameOffset + 0x38B1C0 + 1),1,0x44);
	Byte(((DWORD)GameOffset + 0x38B1C0 + 2),1,0x24);
	Byte(((DWORD)GameOffset + 0x38B1C0 + 3),1,0x08);
	Byte(((DWORD)GameOffset + 0x38B1C0 + 4),1,0x50);
	Byte(((DWORD)GameOffset + 0x38B1C0 + 5),1,0x56);
	Call(((DWORD)GameOffset + 0x38B1C0 + 6),(DWORD)game_sv_mp__Create_fix);
	Byte(((DWORD)GameOffset + 0x38B1C0 + 11),1,0x58);
	Byte(((DWORD)GameOffset + 0x38B1C0 + 12),1,0x58);
	Byte(((DWORD)GameOffset + 0x38B1C0 + 13),1,0x58);
	Jmp(((DWORD)GameOffset + 0x38B1C0 + 14),((DWORD)GameOffset + 0x38B1E8));
	// fix game_sv_mp::~game_sv_mp (destroy ALifeSimulator)
	/*
		mov esi, ecx
		lea eax, [esi+4170h]
		push eax
		call CDestroyer__CHelper2_CALifeSimulator_____delete_data_1_
		add esp, 4
		jmp game_sv_GameState___game_sv_GameState
	*/
	DWORD end_of_func = (DWORD)malloc(24);
	Byte((end_of_func + 0),1,0x8B);	// mov esi, ecx
	Byte((end_of_func + 1),1,0xF1);
	Byte((end_of_func + 2),1,0x8D); // lea eax, [esi+4170h]
	Byte((end_of_func + 3),1,0x86);
	Byte((end_of_func + 4),1,0x70);
	Byte((end_of_func + 5),1,0x41);
	Byte((end_of_func + 6),1,0x00);
	Byte((end_of_func + 7),1,0x00);
	Byte((end_of_func + 8),1,0x50); // push eax
	Call((end_of_func + 9),(DWORD)pCDestroyer__CALifeSimulator__delete); // call CDestroyer__CHelper2_CALifeSimulator_____delete_data_1_
	Byte((end_of_func + 14),1,0x83); // add esp, 4
	Byte((end_of_func + 15),1,0xC4);
	Byte((end_of_func + 16),1,0x04);
	Byte((end_of_func + 17),1,0x8B); // mov ecx, esi
	Byte((end_of_func + 18),1,0xCE);
	Jmp((end_of_func + 19),((DWORD)GameOffset + 0x379620)); // jmp game_sv_GameState___game_sv_GameState
	Jmp(((DWORD)GameOffset + 0x391FDD),end_of_func);
	Byte(((DWORD)GameOffset + 0x391FB1),1,0xEB);
	// fix CALifeSimulator::CALifeSimulator (not check cmdline and load alife.spawn with "new" param)
	Jmp(((DWORD)GameOffset + 0x96A84),((DWORD)GameOffset + 0x96BA0));
	/*
		push 1
		push 0
		push ebp
		mov ecx, ebx
		call CALifeUpdateManager::load
	*/
	Byte(((DWORD)GameOffset + 0x96C82 + 0),1,0x6A);	// push 1
	Byte(((DWORD)GameOffset + 0x96C82 + 1),1,0x01);
	Byte(((DWORD)GameOffset + 0x96C82 + 2),1,0x6A);	// push 0
	Byte(((DWORD)GameOffset + 0x96C82 + 3),1,0x00);
	Byte(((DWORD)GameOffset + 0x96C82 + 4),1,0x68); // push offset LevelAlifeSpawnFile
	Address(((DWORD)GameOffset + 0x96C82 + 5),(DWORD)LevelAlifeSpawnFile);
	Byte(((DWORD)GameOffset + 0x96C82 + 9),1,0x8B);	// mov ecx, ebx
	Byte(((DWORD)GameOffset + 0x96C82 + 10),1,0xCB);
	Call(((DWORD)GameOffset + 0x96C82 + 11),((DWORD)GameOffset + 0xABE80));	// call CALifeUpdateManager::load
	Jmp(((DWORD)GameOffset + 0x96C82 + 16),((DWORD)GameOffset + 0x96CCA));
	// fix CALifeSpawnRegistry::load_0 (load spawn file from level directory)
	Address(((DWORD)GameOffset + 0xA6A73),(DWORD)LevelDirectory);
	// fix for CLevel::Load_GameSpecific_Before (call our func)
	/*
		jmp start_func
		call CLevel__Load_GameSpecific_Before_fix
	*/
	Jmp(((DWORD)GameOffset + 0x237CB5),((DWORD)GameOffset + 0x237CC3));
	Jmp(((DWORD)GameOffset + 0x237CC9),((DWORD)GameOffset + 0x237CD6));
	Call(((DWORD)GameOffset + 0x237D0C),(DWORD)CLevel__Load_GameSpecific_Before_fix);
	Byte(((DWORD)GameOffset + 0x237D11),8,0x90);
	// fix Actor() (allow to use in MP)
	Jmp(((DWORD)GameOffset + 0x26A790),((DWORD)GameOffset + 0x26A7CA));
	// fix game_sv_Deathmatch::on_death (disable Class dynamic cast)
	Jmp(((DWORD)GameOffset + 0x383258),((DWORD)GameOffset + 0x38326F));	
	// fix game_sv_mp::RespawnPlayer (not change some var of alife)
	Byte(((DWORD)GameOffset + 0x39153B),1,0xEB);
	Byte(((DWORD)GameOffset + 0x391543),4,0x90);
	// fix game_sv_mp::OnPlayerDisconnect (not change some var of alife)
	Byte(((DWORD)GameOffset + 0x38FA29),1,0xEB);
	Byte(((DWORD)GameOffset + 0x38FA2E),4,0x90);
	// fix 103503F0 (not change some var of alife)
	Byte(((DWORD)GameOffset + 0x3503FF),1,0xEB);
	Byte(((DWORD)GameOffset + 0x35040B),4,0x90);
	// fix xrServer::Process_event_destroy (disable Class dynamic cast)
	Jmp(((DWORD)GameOffset + 0x437A37),((DWORD)GameOffset + 0x437A4E));
	// fix CSE_ALifeObject::can_switch_online (always return 1)
	Jmp(((DWORD)GameOffset + 0x3BC6A1),((DWORD)GameOffset + 0x3BC6BB));
	// fix CSE_ALifeObject::can_switch_offline (always return 0)
	Jmp(((DWORD)GameOffset + 0x3BC6D1),((DWORD)GameOffset + 0x3BC6EF));
	Byte(((DWORD)GameOffset + 0x3BC6F0),1,0x00);
	// fix CSE_ALifeObject::alife (go to our CSE_ALifeObject::alife)
	Jmp(((DWORD)GameOffset + 0x4E0330),(DWORD)&CSE_ALifeObject__alife_fix);	
	// fix F_entity_Create (call our func)
	Byte(((DWORD)GameOffset + 0x3B4B1F + 0),1,0x50);	// push eax
	Call(((DWORD)GameOffset + 0x3B4B1F + 1),(DWORD)&F_entity_Create_fix);
	end_of_func = (DWORD)malloc(8);
	Byte((end_of_func + 0),1,0x83);	// add esp, 4h
	Byte((end_of_func + 1),1,0xC4);
	Byte((end_of_func + 2),1,0x04);
	Byte((end_of_func + 3),1,0x5E);	// pop esi
	Byte((end_of_func + 4),1,0x83);	// add esp, 8h
	Byte((end_of_func + 5),1,0xC4);
	Byte((end_of_func + 6),1,0x08);
	Byte((end_of_func + 7),1,0xC3);	// retn
	Jmp(((DWORD)GameOffset + 0x3B4B1F + 6),end_of_func);
	// fix CPsyDog::Think (disable CPsyDogAura::update_schedule)
	Byte(((DWORD)GameOffset + 0x1581F2),5,0x90);
	// fix CMonsterEnemyMemory::update (disable Sound detection)
	//Jmp(((DWORD)GameOffset + 0xD36F3),((DWORD)GameOffset + 0xD388A));
	// fix CBaseMonster::UpdateMemory (disable Sound detection)
	//Byte(((DWORD)GameOffset + 0xCA7D8),1,0xEB);
	// fix CScriptGameObject::GetSoundInfo (disable Sound detection)
	//Jmp(((DWORD)GameOffset + 0x1F0057),((DWORD)GameOffset + 0x1F011C));	
	// fix xrServer::Process_event_destroy (crash c_dest == c_from)
	Byte(((DWORD)GameOffset + 0x437836),1,0xEB);
	// fix CPHMovementControl::AllocateCharacterObject (create mp_actor with singleplayer physics)
	Byte(((DWORD)GameOffset + 0x4FE3E7),0x10,0x90);
	Byte(((DWORD)GameOffset + 0x4FE3F7),1,0x6A);	// push 1
	Byte(((DWORD)GameOffset + 0x4FE3F8),1,0x01);
	// fix CPHMovementControl::Calculate_0 (not call callback...SCollisionHitCallback::call)
	Jmp(((DWORD)GameOffset + 0x501469),((DWORD)GameOffset + 0x5014D9));	
	// fix CSE_ALifeTraderAbstract::specific_character (not check gametype)
	Byte(((DWORD)GameOffset + 0x4074BD),1,0xEB);
	// fix CMapLocation::UpdateSpot (think what CALifeSimulator not exist)
	Byte(((DWORD)GameOffset + 0x2A42DD),1,0xEB);
	// fix bind_object (if it server - work)
	Byte(((DWORD)GameOffset + 0xC1720),7,0x90);
	Call(((DWORD)GameOffset + 0xC1727),(DWORD)&bind_object_fix);
	Byte(((DWORD)GameOffset + 0xC172C),1,0x3C);
	Byte(((DWORD)GameOffset + 0xC172D),1,0x01);
	// fix CActor::net_Spawn (jump to our func)
	Byte(((DWORD)GameOffset + 0x26CC26),1,0x57); // push edi (Entity*)
	Byte(((DWORD)GameOffset + 0x26CC27),1,0x56); // push esi (CObject*)
	Call(((DWORD)GameOffset + 0x26CC28),(DWORD)&CActor__net_Spawn_fix);
	Byte(((DWORD)GameOffset + 0x26CC2D),1,0x83); // add esp, 8h
	Byte(((DWORD)GameOffset + 0x26CC2E),1,0xC4);
	Byte(((DWORD)GameOffset + 0x26CC2F),1,0x08);
	Jmp(((DWORD)GameOffset + 0x26CC30),((DWORD)GameOffset + 0x26CC38));
	// fix CActor::net_Destroy (jump to our func)
	Byte(((DWORD)GameOffset + 0x26F22E),1,0x56); // push esi (CObject*)
	Call(((DWORD)GameOffset + 0x26F22F),(DWORD)&CActor__net_Destroy_fix);
	Byte(((DWORD)GameOffset + 0x26F234),1,0x83); // add esp, 4h
	Byte(((DWORD)GameOffset + 0x26F235),1,0xC4);
	Byte(((DWORD)GameOffset + 0x26F236),1,0x04);
	Byte(((DWORD)GameOffset + 0x26F237),5,0x90);
	// fix CSpaceRestrictionHolder::collect_garbage (not see on time)	
	Jmp(((DWORD)GameOffset + 0x759DA),((DWORD)GameOffset + 0x75A89));
	// fix game_sv_mp::Update (disable max corpses check)
	Jmp(((DWORD)GameOffset + 0x38EEF9),((DWORD)GameOffset + 0x38F1A0));
	// fix CALifeGraphRegistry::update (need not flag & 8 but our check)
	end_of_func = (DWORD)malloc(22);
	Byte((end_of_func + 0),1,0x56);	// push esi
	Call((end_of_func + 1),(DWORD)&CALifeGraphRegistry__update_fix);
	Byte((end_of_func + 6),1,0x83);	// add esp, 4h
	Byte((end_of_func + 7),1,0xC4);
	Byte((end_of_func + 8),1,0x04);
	Byte((end_of_func + 9),1,0x85);	// test eax, eax
	Byte((end_of_func + 10),1,0xC0);
	Byte((end_of_func + 11),1,0x0F); // jnz 100A52DB
	Byte((end_of_func + 12),1,0x85);
	Address((end_of_func + 13),(((DWORD)GameOffset + 0xA52DB) - (end_of_func + 13) - 4));
	Jmp((end_of_func + 17),((DWORD)GameOffset + 0xA532C)); // jmp 100A532C
	Jmp(((DWORD)GameOffset + 0xA52D5),end_of_func);
	Byte(((DWORD)GameOffset + 0xA52DA),1,0x90);


	Byte(((DWORD)GameOffset + 0x26CDF6),2,0x90);
	


	// disable singleplayer checks
	// CEntity::net_Spawn
	Byte(((DWORD)GameOffset + 0x279C3D),6,0x90);
	// CEntity::net_Destroy
	Byte(((DWORD)GameOffset + 0x279191),2,0x90);
	// CEntity::Die
	Byte(((DWORD)GameOffset + 0x279666),2,0x90);
	// CEntityAlive::Die
	Byte(((DWORD)GameOffset + 0x27BFA3),2,0x90);
	// CEntityAlive::Hit
	Byte(((DWORD)GameOffset + 0x27BEC6),6,0x90);
	// 102ACC20
	Byte(((DWORD)GameOffset + 0x2ACC27),9,0x90);
	// CLevel::OnFrame
	Byte(((DWORD)GameOffset + 0x234A42),2,0x90);
	// CMapManager::RemoveMapLocationByObjectID
	Byte(((DWORD)GameOffset + 0x2A6461),2,0x90);
	// CInventoryOwner::net_Spawn
	//Byte(((DWORD)GameOffset + 0x234A42),6,0x90);


	// disable dedicated checks
	// game_sv_GameState::Create
	Byte(((DWORD)GameOffset + 0x37A34B),6,0x90);
	// game_sv_GameState::Update
	Byte(((DWORD)GameOffset + 0x37915A),2,0x90);
	// game_sv_GameState::~game_sv_GameState
	Byte(((DWORD)GameOffset + 0x379635),2,0x90);
	// game_sv_GameState::Update
	Byte(((DWORD)GameOffset + 0x37915A),2,0x90);
	// CAI_Space::Initialize
	Byte(((DWORD)GameOffset + 0x22788C),6,0x90);
	// CAI_Space::unload
	Byte(((DWORD)GameOffset + 0x2279AB),6,0x90);
	// CAI_Space::patrol_path_storage
	Byte(((DWORD)GameOffset + 0x2268AD),2,0x90);
	// CAI_Space::patrol_path_storage_raw
	Byte(((DWORD)GameOffset + 0x22680D),6,0x90);
	// CObjectFactory::register_script_classes
	Byte(((DWORD)GameOffset + 0x3471B8),2,0x90);
	// CLevel::CLevel
	Byte(((DWORD)GameOffset + 0x236E35),2,0x90);
	Byte(((DWORD)GameOffset + 0x236F00),6,0x90);
	// CLevel::~CLevel
	Byte(((DWORD)GameOffset + 0x235515),2,0x90);
	// CLevel::g_sv_Spawn
	Byte(((DWORD)GameOffset + 0x23B8C0),2,0x90);
	Byte(((DWORD)GameOffset + 0x23B7DC),2,0x90);
	// CLevel::Load_GameSpecific_After
	Byte(((DWORD)GameOffset + 0x2388DB),6,0x90);
	Byte(((DWORD)GameOffset + 0x238B33),6,0x90);
	// CLevel::Load_GameSpecific_Before
	Byte(((DWORD)GameOffset + 0x237D22),2,0x90);
	// CLevel::remove_objects
	Byte(((DWORD)GameOffset + 0x23A745),2,0x90);
	Byte(((DWORD)GameOffset + 0x23A782),2,0x90);
	Byte(((DWORD)GameOffset + 0x23A821),2,0x90);
	// CLevel::OnFrame
	Byte(((DWORD)GameOffset + 0x234CFB),2,0x90);
	// CActor::CActor
	Byte(((DWORD)GameOffset + 0x2655C8),2,0x90);
	// CGameObject::CGameObject
	Byte(((DWORD)GameOffset + 0x281601),2,0x90);
	// CGameObject::~CGameObject (maybe it)
	Byte(((DWORD)GameOffset + 0x280243),2,0x90);
	// CGameObject::net_Spawn
	Byte(((DWORD)GameOffset + 0x280EBF),2,0x90);
	Byte(((DWORD)GameOffset + 0x280EF6),2,0x90);
	// CGameObject::net_Relcase
	Byte(((DWORD)GameOffset + 0x27F827),2,0x90);
	// CGameObject::shedule_Update
	Byte(((DWORD)GameOffset + 0x27F589),2,0x90);
	// 10280230
	Byte(((DWORD)GameOffset + 0x280243),2,0x90);
	
	// correct classes ftables
	// CControlDirection::update_frame
	JoinInClassFtable(((DWORD)GameOffset + 0x51D950),(DWORD)&CControlDirection__update_frame,0x0);
	// CAI_Bloodsucker
	// CAI_Bloodsucker::NET_Export
	JoinInClassFtable((CAI_Bloodsucker__ftable + 0x38),(DWORD)&CAI_Bloodsucker__NET_Export,0x4);
	// CAI_Bloodsucker::NET_Import
	JoinInClassFtable((CAI_Bloodsucker__ftable + 0x3C),(DWORD)&CAI_Bloodsucker__NET_Import,0x4);
	// CAI_Bloodsucker::NeedToDestroyObject
	Address((CAI_Bloodsucker__ftable + 0xFC),pCActor__NeedToDestroyObject);
	// CAI_Bloodsucker::TimePassedAfterDeath
	Address((CAI_Bloodsucker__ftable + 0x290),pCActor__TimePassedAfterDeath);
	// CAI_Boar
	// CAI_Boar::NET_Export
	JoinInClassFtable((CAI_Boar__ftable + 0x38),(DWORD)&CAI_Boar__NET_Export,0x4);
	// CAI_Boar::NET_Import
	JoinInClassFtable((CAI_Boar__ftable + 0x3C),(DWORD)&CAI_Boar__NET_Import,0x4);
	// CAI_Boar::NeedToDestroyObject
	Address((CAI_Boar__ftable + 0xFC),pCActor__NeedToDestroyObject);
	// CAI_Boar::TimePassedAfterDeath
	Address((CAI_Boar__ftable + 0x290),pCActor__TimePassedAfterDeath);
	// CAI_Crow
	// CAI_Crow::NET_Export
	//JoinInClassFtable((CAI_Crow__ftable + 0x38),(DWORD)&CAI_Crow__NET_Export,0x4);
	// CAI_Crow::NET_Import
	//JoinInClassFtable((CAI_Crow__ftable + 0x3C),(DWORD)&CAI_Crow__NET_Import,0x4);
	// CAI_Dog
	// CAI_Dog::NET_Export
	JoinInClassFtable((CAI_Dog__ftable + 0x38),(DWORD)&CAI_Dog__NET_Export,0x4);
	// CAI_Dog::NET_Import
	JoinInClassFtable((CAI_Dog__ftable + 0x3C),(DWORD)&CAI_Dog__NET_Import,0x4);
	// CAI_Dog::NeedToDestroyObject
	Address((CAI_Dog__ftable + 0xFC),pCActor__NeedToDestroyObject);
	// CAI_Dog::TimePassedAfterDeath
	Address((CAI_Dog__ftable + 0x290),pCActor__TimePassedAfterDeath);
	// CAI_Flesh
	// CAI_Flesh::NET_Export
	JoinInClassFtable((CAI_Flesh__ftable + 0x38),(DWORD)&CAI_Flesh__NET_Export,0x4);
	// CAI_Flesh::NET_Import
	JoinInClassFtable((CAI_Flesh__ftable + 0x3C),(DWORD)&CAI_Flesh__NET_Import,0x4);
	// CAI_Flesh::NeedToDestroyObject
	Address((CAI_Flesh__ftable + 0xFC),pCActor__NeedToDestroyObject);
	// CAI_Flesh::TimePassedAfterDeath
	Address((CAI_Flesh__ftable + 0x290),pCActor__TimePassedAfterDeath);
	// CAI_Pseudodog
	// CAI_PseudoDog::NET_Export
	JoinInClassFtable((CAI_PseudoDog__ftable + 0x38),(DWORD)&CAI_PseudoDog__NET_Export,0x4);
	// CAI_PseudoDog::NET_Import
	JoinInClassFtable((CAI_PseudoDog__ftable + 0x3C),(DWORD)&CAI_PseudoDog__NET_Import,0x4);
	// CAI_PseudoDog::NeedToDestroyObject
	Address((CAI_PseudoDog__ftable + 0xFC),pCActor__NeedToDestroyObject);
	// CAI_PseudoDog::TimePassedAfterDeath
	Address((CAI_PseudoDog__ftable + 0x290),pCActor__TimePassedAfterDeath);
	// CAI_Rat
	// CAI_Rat::NET_Export
	JoinInClassFtable((CAI_Rat__ftable + 0x38),(DWORD)&CAI_Rat__NET_Export,0x4);
	// CAI_Rat::NET_Import
	JoinInClassFtable((CAI_Rat__ftable + 0x3C),(DWORD)&CAI_Rat__NET_Import,0x4);
	// CAI_Rat::NeedToDestroyObject
	Address((CAI_Rat__ftable + 0xFC),pCActor__NeedToDestroyObject);
	// CAI_Rat::TimePassedAfterDeath
	Address((CAI_Rat__ftable + 0x290),pCActor__TimePassedAfterDeath);
	// CAI_Stalker
	// CAI_Stalker::NET_Export
	JoinInClassFtable((CAI_Stalker__ftable + 0x38),(DWORD)&CAI_Stalker__NET_Export,0x4);
	// CAI_Stalker::NET_Import
	JoinInClassFtable((CAI_Stalker__ftable + 0x3C),(DWORD)&CAI_Stalker__NET_Import,0x4);
	// CAI_Stalker::NeedToDestroyObject
	Address((CAI_Stalker__ftable + 0xFC),pCActor__NeedToDestroyObject);
	// CAI_Stalker::TimePassedAfterDeath
	Address((CAI_Stalker__ftable + 0x290),pCActor__TimePassedAfterDeath);
	// CAI_Trader
	// CAI_Trader::NET_Export
	JoinInClassFtable((CAI_Trader__ftable + 0x38),(DWORD)&CAI_Trader__NET_Export,0x4);
	// CAI_Trader::NET_Import
	JoinInClassFtable((CAI_Trader__ftable + 0x3C),(DWORD)&CAI_Trader__NET_Import,0x4);
	// CAI_Trader::NeedToDestroyObject
	Address((CAI_Trader__ftable + 0xFC),pCActor__NeedToDestroyObject);
	// CAI_Trader::TimePassedAfterDeath
	Address((CAI_Trader__ftable + 0x290),pCActor__TimePassedAfterDeath);
	// CBurer
	// CBurer::NET_Export
	JoinInClassFtable((CBurer__ftable + 0x38),(DWORD)&CBurer__NET_Export,0x4);
	// CBurer::NET_Import
	JoinInClassFtable((CBurer__ftable + 0x3C),(DWORD)&CBurer__NET_Import,0x4);
	// CBurer::NeedToDestroyObject
	Address((CBurer__ftable + 0xFC),pCActor__NeedToDestroyObject);
	// CBurer::TimePassedAfterDeath
	Address((CBurer__ftable + 0x290),pCActor__TimePassedAfterDeath);
	// CCat
	// CCat::NET_Export
	JoinInClassFtable((CCat__ftable + 0x38),(DWORD)&CCat__NET_Export,0x4);
	// CCat::NET_Import
	JoinInClassFtable((CCat__ftable + 0x3C),(DWORD)&CCat__NET_Import,0x4);
	// CCat::NeedToDestroyObject
	Address((CCat__ftable + 0xFC),pCActor__NeedToDestroyObject);
	// CCat::TimePassedAfterDeath
	Address((CCat__ftable + 0x290),pCActor__TimePassedAfterDeath);
	// CChimera
	// CChimera::NET_Export
	JoinInClassFtable((CChimera__ftable + 0x38),(DWORD)&CChimera__NET_Export,0x4);
	// CChimera::NET_Import
	JoinInClassFtable((CChimera__ftable + 0x3C),(DWORD)&CChimera__NET_Import,0x4);
	// CChimera::NeedToDestroyObject
	Address((CChimera__ftable + 0xFC),pCActor__NeedToDestroyObject);
	// CChimera::TimePassedAfterDeath
	Address((CChimera__ftable + 0x290),pCActor__TimePassedAfterDeath);
	// CController
	// CController::NET_Export
	JoinInClassFtable((CController__ftable + 0x38),(DWORD)&CController__NET_Export,0x4);
	// CController::NET_Import
	JoinInClassFtable((CController__ftable + 0x3C),(DWORD)&CController__NET_Import,0x4);
	// CController::NeedToDestroyObject
	Address((CController__ftable + 0xFC),pCActor__NeedToDestroyObject);
	// CController::TimePassedAfterDeath
	Address((CController__ftable + 0x290),pCActor__TimePassedAfterDeath);
	// CFracture
	// CFracture::NET_Export
	JoinInClassFtable((CFracture__ftable + 0x38),(DWORD)&CFracture__NET_Export,0x4);
	// CFracture::NET_Import
	JoinInClassFtable((CFracture__ftable + 0x3C),(DWORD)&CFracture__NET_Import,0x4);
	// CFracture::NeedToDestroyObject
	Address((CFracture__ftable + 0xFC),pCActor__NeedToDestroyObject);
	// CFracture::TimePassedAfterDeath
	Address((CFracture__ftable + 0x290),pCActor__TimePassedAfterDeath);
	// CPhantom
	// CPhantom::NET_Export
	JoinInClassFtable((CPhantom__ftable + 0x38),(DWORD)&CPhantom__NET_Export,0x4);
	// CPhantom::NET_Import
	JoinInClassFtable((CPhantom__ftable + 0x3C),(DWORD)&CPhantom__NET_Import,0x4);
	// CPhantom::NeedToDestroyObject
	Address((CPhantom__ftable + 0xFC),pCActor__NeedToDestroyObject);
	// CPhantom::TimePassedAfterDeath
	Address((CPhantom__ftable + 0x290),pCActor__TimePassedAfterDeath);
	// CPoltergeist
	// CPoltergeist::NET_Export
	JoinInClassFtable((CPoltergeist__ftable + 0x38),(DWORD)&CPoltergeist__NET_Export,0x4);
	// CPoltergeist::NET_Import
	JoinInClassFtable((CPoltergeist__ftable + 0x3C),(DWORD)&CPoltergeist__NET_Import,0x4);
	// CPoltergeist::NeedToDestroyObject
	Address((CPoltergeist__ftable + 0xFC),pCActor__NeedToDestroyObject);
	// CPoltergeist::TimePassedAfterDeath
	Address((CPoltergeist__ftable + 0x290),pCActor__TimePassedAfterDeath);
	// CPseudoGigant
	// CPseudoGigant::NET_Export
	JoinInClassFtable((CPseudoGigant__ftable + 0x38),(DWORD)&CPseudoGigant__NET_Export,0x4);
	// CPseudoGigant::NET_Import
	JoinInClassFtable((CPseudoGigant__ftable + 0x3C),(DWORD)&CPseudoGigant__NET_Import,0x4);
	// CPseudoGigant::NeedToDestroyObject
	Address((CPseudoGigant__ftable + 0xFC),pCActor__NeedToDestroyObject);
	// CPseudoGigant::TimePassedAfterDeath
	Address((CPseudoGigant__ftable + 0x290),pCActor__TimePassedAfterDeath);
	// CPsyDog
	// CPsyDog::NET_Export
	JoinInClassFtable((CPsyDog__ftable + 0x38),(DWORD)&CPsyDog__NET_Export,0x4);
	// CPsyDog::NET_Import
	JoinInClassFtable((CPsyDog__ftable + 0x3C),(DWORD)&CPsyDog__NET_Import,0x4);
	// CPsyDog::NeedToDestroyObject
	Address((CPsyDog__ftable + 0xFC),pCActor__NeedToDestroyObject);
	// CPsyDog::TimePassedAfterDeath
	Address((CPsyDog__ftable + 0x290),pCActor__TimePassedAfterDeath);
	// CPsyDogPhantom
	// CPsyDogPhantom::NET_Export
	JoinInClassFtable((CPsyDogPhantom__ftable + 0x38),(DWORD)&CPsyDogPhantom__NET_Export,0x4);
	// CPsyDogPhantom::NET_Import
	JoinInClassFtable((CPsyDogPhantom__ftable + 0x3C),(DWORD)&CPsyDogPhantom__NET_Import,0x4);
	// CPsyDogPhantom::NeedToDestroyObject
	Address((CPsyDogPhantom__ftable + 0xFC),pCActor__NeedToDestroyObject);
	// CPsyDogPhantom::TimePassedAfterDeath
	Address((CPsyDogPhantom__ftable + 0x290),pCActor__TimePassedAfterDeath);
	// CSnork
	// CSnork::NET_Export
	JoinInClassFtable((CSnork__ftable + 0x38),(DWORD)&CSnork__NET_Export,0x4);
	// CSnork::NET_Import
	JoinInClassFtable((CSnork__ftable + 0x3C),(DWORD)&CSnork__NET_Import,0x4);
	// CSnork::NeedToDestroyObject
	Address((CSnork__ftable + 0xFC),pCActor__NeedToDestroyObject);
	// CSnork::TimePassedAfterDeath
	Address((CSnork__ftable + 0x290),pCActor__TimePassedAfterDeath);
	// CTushkano
	// CTushkano::NET_Export
	JoinInClassFtable((CTushkano__ftable + 0x38),(DWORD)&CTushkano__NET_Export,0x4);
	// CTushkano::NET_Import
	JoinInClassFtable((CTushkano__ftable + 0x3C),(DWORD)&CTushkano__NET_Import,0x4);
	// CTushkano::NeedToDestroyObject
	Address((CTushkano__ftable + 0xFC),pCActor__NeedToDestroyObject);
	// CTushkano::TimePassedAfterDeath
	Address((CTushkano__ftable + 0x290),pCActor__TimePassedAfterDeath);
	// CZombie
	// CZombie::NET_Export
	JoinInClassFtable((CZombie__ftable + 0x38),(DWORD)&CZombie__NET_Export,0x4);
	// CZombie::NET_Import
	JoinInClassFtable((CZombie__ftable + 0x3C),(DWORD)&CZombie__NET_Import,0x4);
	// CZombie::NeedToDestroyObject
	Address((CZombie__ftable + 0xFC),pCActor__NeedToDestroyObject);
	// CZombie::TimePassedAfterDeath
	Address((CZombie__ftable + 0x290),pCActor__TimePassedAfterDeath);
	// CWrapper
	// CWrapperAbstractMonster<class CSE_ALifeCreatureCrow>::UPDATE_Write
	//JoinInClassFtable(((DWORD)GameOffset + 0x596DE8),(DWORD)&CSE_ALifeCreatureCrow__UPDATE_Write,0x4);
	// CWrapperAbstractMonster<class CSE_ALifeCreatureCrow>::UPDATE_Read
	//JoinInClassFtable(((DWORD)GameOffset + 0x596DEC),(DWORD)&CSE_ALifeCreatureCrow__UPDATE_Read,0x4);
	// CWrapperAbstractMonster<class CSE_ALifeCreaturePhantom>::UPDATE_Write
	JoinInClassFtable(((DWORD)GameOffset + 0x597A48),(DWORD)&CSE_ALifeCreaturePhantom__UPDATE_Write,0x4);
	// CWrapperAbstractMonster<class CSE_ALifeCreaturePhantom>::UPDATE_Read
	JoinInClassFtable(((DWORD)GameOffset + 0x597A4C),(DWORD)&CSE_ALifeCreaturePhantom__UPDATE_Read,0x4);
	// CWrapperAbstractMonster<class CSE_ALifeCreatureRat>::UPDATE_Write
	JoinInClassFtable(((DWORD)GameOffset + 0x5953D8),(DWORD)&CSE_ALifeCreatureRat__UPDATE_Write,0x4);
	// CWrapperAbstractMonster<class CSE_ALifeCreatureRat>::UPDATE_Read
	JoinInClassFtable(((DWORD)GameOffset + 0x5953DC),(DWORD)&CSE_ALifeCreatureRat__UPDATE_Read,0x4);
	// CWrapperAbstractMonster<class CSE_ALifeHumanStalker>::UPDATE_Write
	JoinInClassFtable(((DWORD)GameOffset + 0x5973B0),(DWORD)&CSE_ALifeHumanStalker__UPDATE_Write,0x4);
	// CWrapperAbstractMonster<class CSE_ALifeHumanStalker>::UPDATE_Read
	JoinInClassFtable(((DWORD)GameOffset + 0x5973B4),(DWORD)&CSE_ALifeHumanStalker__UPDATE_Read,0x4);
	// CWrapperAbstractMonster<class CSE_ALifeMonsterBase>::UPDATE_Write
	JoinInClassFtable(((DWORD)GameOffset + 0x5971B0),(DWORD)&CSE_ALifeMonsterBase__UPDATE_Write,0x4);
	// CWrapperAbstractMonster<class CSE_ALifeMonsterBase>::UPDATE_Read
	JoinInClassFtable(((DWORD)GameOffset + 0x5971B4),(DWORD)&CSE_ALifeMonsterBase__UPDATE_Read,0x4);
	// CWrapperAbstractMonster<class CSE_ALifeTrader>::UPDATE_Write
	JoinInClassFtable(((DWORD)GameOffset + 0x59674C),(DWORD)&CSE_ALifeTrader__UPDATE_Write,0x4);
	// CWrapperAbstractMonster<class CSE_ALifeTrader>::UPDATE_Read
	JoinInClassFtable(((DWORD)GameOffset + 0x596750),(DWORD)&CSE_ALifeTrader__UPDATE_Read,0x4);
	// CWrapperAbstractMonster<class CSE_ALifePsyDogPhantom>::UPDATE_Write
	JoinInClassFtable(((DWORD)GameOffset + 0x598480),(DWORD)&CSE_ALifePsyDogPhantom__UPDATE_Write,0x4);
	// CWrapperAbstractMonster<class CSE_ALifePsyDogPhantom>::UPDATE_Read
	JoinInClassFtable(((DWORD)GameOffset + 0x598484),(DWORD)&CSE_ALifePsyDogPhantom__UPDATE_Read,0x4);
	// CWrapperAbstractMonster<class CSE_ALifeCreatureCrow>::NeedUpdate (call CSE_ALifeCreatureAbstract::alive)
	Address(((DWORD)GameOffset + 0x596E3C),((DWORD)GameOffset + 0xB1F50));
	// CWrapperAbstractMonster<class CSE_ALifeCreaturePhantom>::NeedUpdate (call CSE_ALifeCreatureAbstract::alive)
	Address(((DWORD)GameOffset + 0x597A9C),((DWORD)GameOffset + 0xB1F50));
	// CWrapperAbstractMonster<class CSE_ALifeCreatureRat>::NeedUpdate (call CSE_ALifeCreatureAbstract::alive)
	Address(((DWORD)GameOffset + 0x59542C),((DWORD)GameOffset + 0xB1F50));
	// CWrapperAbstractMonster<class CSE_ALifeHumanStalker>::NeedUpdate (call CSE_ALifeCreatureAbstract::alive)
	Address(((DWORD)GameOffset + 0x597404),((DWORD)GameOffset + 0xB1F50));
	// CWrapperAbstractMonster<class CSE_ALifeMonsterBase>::NeedUpdate (call CSE_ALifeCreatureAbstract::alive)
	Address(((DWORD)GameOffset + 0x597204),((DWORD)GameOffset + 0xB1F50));
	// CWrapperAbstractMonster<class CSE_ALifeTrader>::NeedUpdate (call CSE_ALifeCreatureAbstract::alive)
	Address(((DWORD)GameOffset + 0x59679C),((DWORD)GameOffset + 0xB1F50));
	// CWrapperAbstractMonster<class CSE_ALifePsyDogPhantom>::NeedUpdate (call CSE_ALifeCreatureAbstract::alive)
	Address(((DWORD)GameOffset + 0x5984D4),((DWORD)GameOffset + 0xB1F50));
	// CWrapperAbstractMonster<class CSE_ALifeCreatureActor>::NeedUpdate (call nullsub (return 0))
	Address(((DWORD)GameOffset + 0x59767C),((DWORD)GameOffset + 0x3BB6E0));
	// other
	// CActor::shedule_Update
	Address(((DWORD)GameOffset + 0x54C818),((DWORD)GameOffset + 0x27F550));
	// CActor::UpdateCL
	JoinInClassFtable(((DWORD)GameOffset + 0x54C880),(DWORD)&CActor__UpdateCL,0x0);
	// CActor::cam_Set
	//Address(((DWORD)GameOffset + 0x54CAEC),((DWORD)GameOffset + 0x3EF59));
	
	// CWrapperAbstractMonster<class CSE_ALifeMonsterBase>::on_death
	JoinInClassFtable(((DWORD)GameOffset + 0x5972D8),(DWORD)&CSE_ALifeMonsterBase__on_death,0x4);
	
	pCSE_ALifeMonsterBase__on_death = ((DWORD)GameOffset + 0x417CAD);
	pgame_sv_mp__Player_AddMoney = ((DWORD)GameOffset + 0x38ECB0);
	pgame_sv_mp__getPlayerGameState = ((DWORD)GameOffset + 0x378950);

	// death money
	/*
	int len = 0x403C6C - 0x403BC0;

	DWORD func_copy = (DWORD)malloc(len);
	DWORD oldp;
	if(VirtualProtect((LPVOID)((DWORD)GameOffset + 0x403BC0),len,PAGE_EXECUTE_READWRITE, &oldp))
	{
		LogHandle->Write("copy");
		memcpy((void*)func_copy, (void*)((DWORD)GameOffset + 0x403BC0), len);
	}
	Jmp(((DWORD)GameOffset + 0x403BC0), (DWORD)&CSE_Abstract__on_death_fix);
	*/
	


	

	// Render hooks
	switch(RenderVersion)
	{
	case 1:
		{
			// CKinematicsAnimated::PlayCycle
			JoinInClassFtableEx(((DWORD)RenderOffset + 0x9807C + 0x74),(DWORD)&CKinematicsAnimated__PlayCycle,0x14,pCKinematicsAnimated__PlayCycle);
			// CKinematicsAnimated::PlayCycle_Ex
			JoinInClassFtableEx(((DWORD)RenderOffset + 0x9807C + 0x70),(DWORD)&CKinematicsAnimated__PlayCycleEx,0x18,pCKinematicsAnimated__PlayCycleEx);
			JoinInClassFtable(((DWORD)RenderOffset + 0x9807C + 0x48),(DWORD)&CKinematicsAnimated__PlayCycleEx,0x18);
			break;
		}
	case 2:
		{
			// CKinematicsAnimated::PlayCycle
			JoinInClassFtableEx(((DWORD)RenderOffset + 0xBE7E4 + 0x74),(DWORD)&CKinematicsAnimated__PlayCycle,0x14,pCKinematicsAnimated__PlayCycle);
			// CKinematicsAnimated::PlayCycle_Ex
			JoinInClassFtableEx(((DWORD)RenderOffset + 0xBE7E4 + 0x70),(DWORD)&CKinematicsAnimated__PlayCycleEx,0x18,pCKinematicsAnimated__PlayCycleEx);
			JoinInClassFtable(((DWORD)RenderOffset + 0xBE7E4 + 0x48),(DWORD)&CKinematicsAnimated__PlayCycleEx,0x18);
			break;
		}
	case 3:
		{
			// CKinematicsAnimated::PlayCycle
			JoinInClassFtableEx(((DWORD)RenderOffset + 0xD9C34 + 0x74),(DWORD)&CKinematicsAnimated__PlayCycle,0x14,pCKinematicsAnimated__PlayCycle);
			// CKinematicsAnimated::PlayCycle_Ex
			JoinInClassFtableEx(((DWORD)RenderOffset + 0xD9C34 + 0x70),(DWORD)&CKinematicsAnimated__PlayCycleEx,0x18,pCKinematicsAnimated__PlayCycleEx);
			JoinInClassFtable(((DWORD)RenderOffset + 0xD9C34 + 0x48),(DWORD)&CKinematicsAnimated__PlayCycleEx,0x18);
			break;
		}
	case 4:
		{
			// CKinematicsAnimated::PlayCycle
			JoinInClassFtableEx(((DWORD)RenderOffset + 0xE6244 + 0x74),(DWORD)&CKinematicsAnimated__PlayCycle,0x14,pCKinematicsAnimated__PlayCycle);
			// CKinematicsAnimated::PlayCycle_Ex
			JoinInClassFtableEx(((DWORD)RenderOffset + 0xE6244 + 0x70),(DWORD)&CKinematicsAnimated__PlayCycleEx,0x18,pCKinematicsAnimated__PlayCycleEx);
			JoinInClassFtable(((DWORD)RenderOffset + 0xE6244 + 0x48),(DWORD)&CKinematicsAnimated__PlayCycleEx,0x18);
			break;
		}
	}

	// correct switch tables
	// CLevel::ClientReceive
	// create MotionsLoc
	void* motions_loc = malloc(14);
	/*
		push ebp
		call MotionsFunc
		add esp, 4h
		jmp 1023B669
	*/
	Byte(((DWORD)motions_loc + 0),1,0x55); // push ebp
	Call(((DWORD)motions_loc + 1),(DWORD)&CLevel__cl_Process_Motion); // call MotionsFunc
	Byte(((DWORD)motions_loc + 6),1,0x83);		// add esp, 4
	Byte(((DWORD)motions_loc + 7),1,0xC4);
	Byte(((DWORD)motions_loc + 8),1,0x4);
	Jmp(((DWORD)motions_loc + 9),((DWORD)GameOffset + 0x23B669));
	// create EventLoc
	void* event_loc = malloc(14);
	/*
		push ebp
		call EventFunc
		add esp, 4h
		jmp 1023B669
	*/
	Byte(((DWORD)event_loc + 0),1,0x55); // push ebp
	Call(((DWORD)event_loc + 1),(DWORD)&CLevel__cl_Process_CustomEvent); // call EventFunc
	Byte(((DWORD)event_loc + 6),1,0x83);		// add esp, 4
	Byte(((DWORD)event_loc + 7),1,0xC4);
	Byte(((DWORD)event_loc + 8),1,0x4);
	Jmp(((DWORD)event_loc + 9),((DWORD)GameOffset + 0x23B669));
	// change switch
	DWORD locations[33] =
	{
		((DWORD)(GameOffset) + 0x23AFCC),((DWORD)(GameOffset) + 0x23AEF7),((DWORD)(GameOffset) + 0x23B2AE),
		((DWORD)(GameOffset) + 0x23B2BB),((DWORD)(GameOffset) + 0x23B2CE),((DWORD)(GameOffset) + 0x23B300),
		((DWORD)(GameOffset) + 0x23B639),((DWORD)(GameOffset) + 0x23B276),((DWORD)(GameOffset) + 0x23B09F),
		((DWORD)(GameOffset) + 0x23AFDF),((DWORD)(GameOffset) + 0x23B325),((DWORD)(GameOffset) + 0x23B3CB),
		((DWORD)(GameOffset) + 0x23B630),((DWORD)(GameOffset) + 0x23AF44),((DWORD)(GameOffset) + 0x23B3D7),
		((DWORD)(GameOffset) + 0x23B3F7),((DWORD)(GameOffset) + 0x23B404),((DWORD)(GameOffset) + 0x23B42C),
		((DWORD)(GameOffset) + 0x23B48C),((DWORD)(GameOffset) + 0x23B3E4),((DWORD)(GameOffset) + 0x23B5F3),
		((DWORD)(GameOffset) + 0x23B622),((DWORD)(GameOffset) + 0x23B1DB),((DWORD)(GameOffset) + 0x23B5E6),
		((DWORD)(GameOffset) + 0x23B454),((DWORD)(GameOffset) + 0x23B46F),((DWORD)(GameOffset) + 0x23B480),
		((DWORD)(GameOffset) + 0x23B657),((DWORD)(GameOffset) + 0x23B661),((DWORD)(GameOffset) + 0x23B081),
		((DWORD)(GameOffset) + 0x23B669),(DWORD)motions_loc,(DWORD)event_loc		
	};
	BYTE jump_table[53] =
	{
		0,1,2,3,
		4,30,30,5,
		6,7,8,9,
		30,10,10,10,
		11,30,30,12,
		13,14,30,15,
		30,16,17,18,
		30,30,19,30,
		20,21,30,30,
		6,22,30,23,
		24,24,30,25,
		26,6,30,27,
		28,30,29,31,
		32
	};
	CreateSwitchTable(locations,33,jump_table,53,((DWORD)(GameOffset) + 0x23AEF3),((DWORD)(GameOffset) + 0x23AEEC),((DWORD)(GameOffset) + 0x23AEE2));

	LogHandle->Write("lua_alife__create = 0x%X",lua_alife__create);
	LogHandle->Write("lua_alife = 0x%X",lua_alife);
	LogHandle->Write("F_entity_Create = 0x%X",F_entity_Create);
	LogHandle->Write("ai = 0x%X",ai);
	LogHandle->Write("CObjectItemClientServer_CActor_CSE_ALifeCreatureActor___client_object = 0x%X",CObjectItemClientServer_CActor_CSE_ALifeCreatureActor___client_object);
	LogHandle->Write("pIPureServer__SendTo = 0x%X",pIPureServer__SendTo);
	LogHandle->Write("pCKinematicsAnimated__PlayCycle = 0x%X",pCKinematicsAnimated__PlayCycle);
	LogHandle->Write("pCKinematicsAnimated__PlayCycleEx = 0x%X",pCKinematicsAnimated__PlayCycleEx);
	LogHandle->Write("pCControlDirection__update_frame = 0x%X",pCControlDirection__update_frame);
	LogHandle->Write("pCSE_ALifeCreatureCrow__UPDATE_Write = 0x%X",pCSE_ALifeCreatureCrow__UPDATE_Write);
	LogHandle->Write("pCSE_ALifeCreatureCrow__UPDATE_Read = 0x%X",pCSE_ALifeCreatureCrow__UPDATE_Read);
	LogHandle->Write("pCSE_ALifeCreaturePhantom__UPDATE_Write = 0x%X",pCSE_ALifeCreaturePhantom__UPDATE_Write);
	LogHandle->Write("pCSE_ALifeCreaturePhantom__UPDATE_Read = 0x%X",pCSE_ALifeCreaturePhantom__UPDATE_Read);
	LogHandle->Write("pCSE_ALifeCreatureRat__UPDATE_Write = 0x%X",pCSE_ALifeCreatureRat__UPDATE_Write);
	LogHandle->Write("pCSE_ALifeCreatureRat__UPDATE_Read = 0x%X",pCSE_ALifeCreatureRat__UPDATE_Read);
	LogHandle->Write("pCSE_ALifeHumanStalker__UPDATE_Write = 0x%X",pCSE_ALifeHumanStalker__UPDATE_Write);
	LogHandle->Write("pCSE_ALifeHumanStalker__UPDATE_Read = 0x%X",pCSE_ALifeHumanStalker__UPDATE_Read);
	LogHandle->Write("pCSE_ALifeMonsterBase__UPDATE_Write = 0x%X",pCSE_ALifeMonsterBase__UPDATE_Write);
	LogHandle->Write("pCSE_ALifeMonsterBase__UPDATE_Read = 0x%X",pCSE_ALifeMonsterBase__UPDATE_Read);
	LogHandle->Write("pCSE_ALifeTrader__UPDATE_Write = 0x%X",pCSE_ALifeTrader__UPDATE_Write);
	LogHandle->Write("pCSE_ALifeTrader__UPDATE_Read = 0x%X",pCSE_ALifeTrader__UPDATE_Read);
	LogHandle->Write("pCSE_ALifePsyDogPhantom__UPDATE_Write = 0x%X",pCSE_ALifePsyDogPhantom__UPDATE_Write);
	LogHandle->Write("pCSE_ALifePsyDogPhantom__UPDATE_Read = 0x%X",pCSE_ALifePsyDogPhantom__UPDATE_Read);
	LogHandle->Write("pUPDATE_Write_call_address = 0x%X",pUPDATE_Write_call_address);
	LogHandle->Write("pUPDATE_Read_call_address = 0x%X",pUPDATE_Read_call_address);
	LogHandle->Write("pCALifeSimulator__CALifeSimulator = 0x%X",pCALifeSimulator__CALifeSimulator);
	LogHandle->Write("pCGameGraph__CGameGraph = 0x%X",pCGameGraph__CGameGraph);
	LogHandle->Write("pCLevel__IsServer = 0x%X",pCLevel__IsServer);
	LogHandle->Write("pCAI_Space__load = 0x%X",pCAI_Space__load);
	LogHandle->Write("pCAI_Space__reinit = 0x%X",pCAI_Space__reinit);
	LogHandle->Write("pCAI_Space__patrol_path_storage = 0x%X",pCAI_Space__patrol_path_storage);
	LogHandle->Write("pCActor__NeedToDestroyObject = 0x%X",pCActor__NeedToDestroyObject);
	LogHandle->Write("pCActor__TimePassedAfterDeath = 0x%X",pCActor__TimePassedAfterDeath);
	LogHandle->Write("pCDestroyer__CALifeSimulator__delete = 0x%X",pCDestroyer__CALifeSimulator__delete);
	LogHandle->Write("pCScriptStorage__load_file_into_namespace = 0x%X",pCScriptStorage__load_file_into_namespace);
	LogHandle->Write("pCScriptBinder__reload = 0x%X",pCScriptBinder__reload);
	LogHandle->Write("pCScriptBinder__reinit = 0x%X",pCScriptBinder__reinit);
	LogHandle->Write("pCScriptBinder__net_spawn = 0x%X",pCScriptBinder__net_spawn);
	LogHandle->Write("pCActor__CActor = 0x%X",pCActor__CActor);
	LogHandle->Write("pCSE_ALifeCreatureActor__CSE_ALifeCreatureActor = 0x%X",pCSE_ALifeCreatureActor__CSE_ALifeCreatureActor);
	LogHandle->Write("CSE_ALifeCreatureActor__ftable = 0x%X",CSE_ALifeCreatureActor__ftable);
	LogHandle->Write("CSE_ActorMP__ftable = 0x%X",CSE_ActorMP__ftable);
	LogHandle->Write("CAI_Bloodsucker__ftable = 0x%X",CAI_Bloodsucker__ftable);
	LogHandle->Write("CAI_Boar__ftable = 0x%X",CAI_Boar__ftable);
	LogHandle->Write("CAI_Crow__ftable = 0x%X",CAI_Crow__ftable);
	LogHandle->Write("CAI_Dog__ftable = 0x%X",CAI_Dog__ftable);
	LogHandle->Write("CAI_Flesh__ftable = 0x%X",CAI_Flesh__ftable);
	LogHandle->Write("CAI_PseudoDog__ftable = 0x%X",CAI_PseudoDog__ftable);
	LogHandle->Write("CAI_Rat__ftable = 0x%X",CAI_Rat__ftable);
	LogHandle->Write("CAI_Stalker__ftable = 0x%X",CAI_Stalker__ftable);
	LogHandle->Write("CAI_Trader__ftable = 0x%X",CAI_Trader__ftable);
	LogHandle->Write("CBurer__ftable = 0x%X",CBurer__ftable);
	LogHandle->Write("CCat__ftable = 0x%X",CCat__ftable);
	LogHandle->Write("CChimera__ftable = 0x%X",CChimera__ftable);
	LogHandle->Write("CController__ftable = 0x%X",CController__ftable);
	LogHandle->Write("CFracture__ftable = 0x%X",CFracture__ftable);
	LogHandle->Write("CPhantom__ftable = 0x%X",CPhantom__ftable);
	LogHandle->Write("CPoltergeist__ftable = 0x%X",CPoltergeist__ftable);
	LogHandle->Write("CPseudoGigant__ftable = 0x%X",CPseudoGigant__ftable);
	LogHandle->Write("CPsyDog__ftable = 0x%X",CPsyDog__ftable);
	LogHandle->Write("CPsyDogPhantom__ftable = 0x%X",CPsyDogPhantom__ftable);
	LogHandle->Write("CSnork__ftable = 0x%X",CSnork__ftable);
	LogHandle->Write("CTushkano__ftable = 0x%X",CTushkano__ftable);
	LogHandle->Write("CZombie__ftable = 0x%X",CZombie__ftable);
	LogHandle->Write("gGameOffset = 0x%X",gGameOffset);
	LogHandle->Write("gActorPointer = 0x%X",gActorPointer);
}

WORD CHook_Patch_02::CreateObject(char* section,char* name,float pos_x,float pos_y,float pos_z,WORD parentid)
{
	float position[3];
	position[0] = pos_x;
	position[1] = pos_y;
	position[2] = pos_z;

	unsigned long result = lua_alife__create(lua_alife(),section,position,0,0,parentid);

	unsigned short objectid = *(unsigned short*)(result + 0x32);

	// null custom sync
	CustomSyncData_Entity[objectid] = NULL;

	// return objectid
	return objectid;
}

void CHook_Patch_02::PlayCycle(DWORD pClass,DWORD MotionID)
{
	// base data
	DWORD pIRender_Visual = (DWORD)((CObject*)pClass)->Visual();
	DWORD pCKinematicsAnimated = pIRender_Visual + 0xB0;
	DWORD pObject = pClass;
	
	// anticrash
	for(DWORD mid = 0;mid < 4;mid++)
	{
		DWORD array_addr = pIRender_Visual + 0x44D8 + 0x104 * mid;
		for(DWORD i = 0;i < *(DWORD*)(array_addr + 0x100);i++)
		{
			DWORD item = *(DWORD*)(array_addr + 4*i);
			if(!item) return;
		}
	}

	_asm
	{
		push 0				// HZ = 0
		push pObject		
		push 0				// IBlend
		push 1				// HZ = 1
		push MotionID
		mov ecx, pCKinematicsAnimated
		call pCKinematicsAnimated__PlayCycle
	};
}

void CHook_Patch_02::PlayCycleEx(DWORD pClass,DWORD unk,DWORD MotionID)
{
	// base data
	DWORD pIRender_Visual = (DWORD)((CObject*)pClass)->Visual();
	DWORD pCKinematicsAnimated = pIRender_Visual + 0xB0;
	DWORD pObject = pClass;

	// anticrash
	for(DWORD mid = 0;mid < 4;mid++)
	{
		DWORD array_addr = pIRender_Visual + 0x44D8 + 0x104 * mid;
		for(DWORD i = 0;i < *(DWORD*)(array_addr + 0x100);i++)
		{
			DWORD item = *(DWORD*)(array_addr + 4*i);
			if(!item) return;
		}
	}

	_asm
	{
		push 0				// HZ = 0
		push pObject		
		push 0				// IBlend
		push 1				// HZ = 1
		push MotionID
		push unk
		mov ecx, pCKinematicsAnimated
		call pCKinematicsAnimated__PlayCycleEx
	};
}

void CHook_Patch_02::SendPacketToAll(NET_Packet& pack)
{
	IPureServer* pPureServer = (IPureServer*)*(DWORD*)((DWORD)g_pGameLevel + 0x48730);
	DWORD ClientList = ((DWORD)pPureServer + 0x34);
	DWORD CurrentClient = *(DWORD*)(ClientList + 0x4);
	DWORD LastClient = *(DWORD*)(ClientList + 0x8);

	while(CurrentClient != LastClient)
	{
		DWORD Client_id = *(DWORD*)(*(DWORD*)CurrentClient + 0x80C0);

		if(*(DWORD*)(*(DWORD*)CurrentClient + 0x8170))
		{
			_asm
			{
				push 0
				push 8
				push pack
				push Client_id
				mov ecx, pPureServer
				call pIPureServer__SendTo
			};
		}

		CurrentClient += 0x4;
	}
}

bool CHook_Patch_02::IsServer()
{
	DWORD CLevel = (DWORD)g_pGameLevel;
	bool result = false;

	_asm
	{
		mov ecx, CLevel
		call pCLevel__IsServer
		mov result, al
	};

	return result;
}

void CHook_Patch_02::LoadAImap()
{
	char path[520];
	IReader* chunk;
	DWORD g_pAI_Space = ai();
	if(xr_FS->exist(path,LevelDirectory,LevelAlifeSpawnFile,".spawn"))
	{
		IReader* spawn_file = xr_FS->r_open(path);
		if(spawn_file)
		{
			chunk = spawn_file->open_chunk(3);
			if(chunk)
			{
				// path storage
				_asm
				{
					push chunk
					mov ecx, g_pAI_Space
					call pCAI_Space__patrol_path_storage
				}
				// close
				chunk->close();
			}
			chunk = spawn_file->open_chunk(4);
			if(chunk)
			{
				// GameGraph load
				void* CGameGraph = Memory->mem_alloc(0x54);
				_asm
				{
					push chunk
					mov ecx, CGameGraph
					call pCGameGraph__CGameGraph
				}
				// reinit
				_asm
				{
					push CGameGraph
					mov ecx, g_pAI_Space
					call pCAI_Space__reinit
				}
			}
		}
	}
	// get game map name
	IPureClient* client = (IPureClient*)((DWORD)g_pGameLevel + 0x40110);
	const char* gamemap = client->net_SessionName();
	// load ai map
	_asm
	{
		push gamemap
		mov ecx, g_pAI_Space
		call pCAI_Space__load
	}
}

void CHook_Patch_02::LoadMapScript()
{
	char path[520];
	char* _path = path;
	DWORD pCScriptStorage = *(DWORD*)(ai() + 0x1C);

	if(xr_FS->exist(path,LevelDirectory,LevelScriptFile,".script"))
	{
		_asm
		{
			mov ecx, pCScriptStorage
			push LevelScriptFile
			push _path
			call pCScriptStorage__load_file_into_namespace
		}
		LogHandle->Write("level script exist");
	}
	else LogHandle->Write("level script (%s) not exist",_path);
}

// Other functions

void game_sv_mp__Create_fix(DWORD pClass,DWORD a2)
{
	PUSH_REG(ecx)

	pgame_sv_mp = pClass;

	// vars
	void* pCALifeSimulator = NULL;
	DWORD param_1 = 0;	

	// relink addresses
	HookHandle->LinkAddresses();

	// ALifeSimulator
	pCALifeSimulator = HookHandle->Memory->mem_alloc(0x2FC);
	if(pCALifeSimulator) 
	{
		param_1 = *(DWORD*)(pClass + 0x98);
		_asm
		{
			push 1
			push a2
			push param_1
			mov ecx, pCALifeSimulator
			call pCALifeSimulator__CALifeSimulator
		}
		*(DWORD*)(pClass + 0x4170) = (DWORD)pCALifeSimulator;
	}
	
	POP_REG(ecx)
}

void CLevel__Load_GameSpecific_Before_fix()
{
	PUSH_REG(ecx)

	// load aimap for client
	if(!HookHandle->IsServer()) // client
	{
		HookHandle->LoadAImap();
	}
	// load map script
	HookHandle->LoadMapScript();

	POP_REG(ecx)
}

void CControlDirection__update_frame(DWORD pClass)
{
	PUSH_REG(ecx)
	
	if(HookHandle->IsServer())
	{
		_asm
		{
			mov ecx, pClass
			call pCControlDirection__update_frame
		};
	}

	POP_REG(ecx)
}

DWORD F_entity_Create_fix(DWORD pClass)
{
	return pClass;
}

DWORD CSE_ALifeObject__alife_fix()
{
	return *(DWORD*)(ai() + 0x14);
}

DWORD CActor__net_Spawn_fix(DWORD CObject,DWORD CEntity)
{
	PUSH_REG(ecx)
	
	WORD flags = *(WORD*)(CEntity + 0x43);
	bool is_mp_actor = (*(DWORD*)(CObject) == CActorMP__ftable);
	
	if(is_mp_actor)
	{
		if(flags & 1)
		{
			if(flags & 8) *(DWORD*)(gActorPointer) = CObject;
		}
	}
	else
	{
		*(DWORD*)(gActorPointer) = CObject;
		g_actor = CObject;
		*(WORD*)(CEntity + 0x43) = 0x21;
	}

	POP_REG(ecx)
	return 1;
}

DWORD CActor__net_Destroy_fix(DWORD CObject)
{
	PUSH_REG(ecx)

	if(*(DWORD*)(gActorPointer) == CObject)
	{
		*(DWORD*)(gActorPointer) = g_actor;
	}

	POP_REG(ecx)
	return 1;
}

DWORD CActor__UpdateCL(DWORD pClass)
{
	PUSH_REG(ecx)

	((CObject*)pClass)->setVisible(0);

	POP_REG(ecx)
}

bool bind_object_fix()
{
	PUSH_REG(ecx)

	bool result;

	if(HookHandle->IsServer()) result = true;
	else result = false;

	POP_REG(ecx)
	return result;
}

DWORD CALifeGraphRegistry__update_fix(DWORD pClass)
{
	if(*(DWORD*)(pClass) == CSE_ALifeCreatureActor__ftable) return 1;
	return 0;
}

int CSE_Abstract__on_death_fix(DWORD pClass, DWORD pKillerClass) 
{
	LogHandle->Write("on_death: 0x%X, 0x%X",pClass,pKillerClass);
	return 5;
}

void CSE_ALifeMonsterBase__on_death(DWORD pClass, DWORD pKillerClass)
{
	PUSH_REG(ecx)

	DWORD pKillerFtable = (*(DWORD*)pKillerClass - (DWORD)gGameOffset);
	DWORD pDeadFtable = (*(DWORD*)pClass - (DWORD)gGameOffset);

	if((pKillerFtable == 0x54C134) && (pDeadFtable != 0x54C134)) {
		DWORD pClient = *(DWORD*)(pKillerClass + 0x38);
		LogHandle->Write("Player client 0x%X", pClient);

		DWORD playerState = *(DWORD*)(pClient + 0x8178);
		LogHandle->Write("Player state 0x%X", playerState);

		_asm
		{
			push 50
			push playerState
			mov ecx, pgame_sv_mp
			call pgame_sv_mp__Player_AddMoney
		}

		LogHandle->Write("sended...");
	}

	_asm
	{
		push pKillerClass
		mov ecx, pClass
		call pCSE_ALifeMonsterBase__on_death
	}

	POP_REG(ecx)
}