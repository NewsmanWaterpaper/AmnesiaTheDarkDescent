/*
 * Copyright © 2009-2020 Frictional Games
 * 
 * This file is part of Amnesia: The Dark Descent.
 * 
 * Amnesia: The Dark Descent is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version. 

 * Amnesia: The Dark Descent is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Amnesia: The Dark Descent.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "LuxScriptHandler.h"

#include "LuxMap.h"
#include "LuxPlayer.h"
#include "LuxPlayerHelpers.h"
#include "LuxPlayerHands.h"
#include "LuxHandObject_LightSource.h"
#include "LuxMapHandler.h"
#include "LuxMapHelper.h"
#include "LuxInputHandler.h"
#include "LuxInventory.h"
#include "LuxMoveState_Normal.h"
#include "LuxSaveHandler.h"
#include "LuxSavedGameTypes.h"
#include "LuxPlayerState.h"
#include "LuxMusicHandler.h"
#include "LuxDebugHandler.h"
#include "LuxJournal.h"
#include "LuxEffectHandler.h"
#include "LuxHelpFuncs.h"
#include "LuxMessageHandler.h"
#include "LuxCompletionCountHandler.h"
#include "LuxGlobalDataHandler.h"
#include "LuxHintHandler.h"
#include "LuxEndingsHandler.h"
#include "LuxProgressLogHandler.h"
#include "LuxPauseMessageButtonPrompt.h"
#include "LuxLoadScreenHandler.h"
#include "LuxInsanityHandler.h"
#include "LuxCredits.h"
#include "LuxDemoEnd.h"
#include "LuxRankScreen.h"

#include "LuxProp_Object.h"
#include "LuxProp_SwingDoor.h"
#include "LuxProp_Lamp.h"
#include "LuxProp_Lever.h"
#include "LuxProp_Wheel.h"
#include "LuxProp_MoveObject.h"
#include "LuxProp_Item.h"
#include "LuxProp_LevelDoor.h"
#include "LuxProp_Button.h"
#include "LuxProp_NPC.h"
#include "LuxProp_MultiSlider.h"
#include "LuxEnemy_ManPig.h"
#include "LuxEnemy_Wraith.h"

#include "LuxArea_Sticky.h"

#include "LuxEnemy.h"
#include "LuxEnemyPathfinder.h"
#include "LuxItemType.h"

#include "LuxInteractConnections.h"

#include "LuxAchievementHandler.h"



//////////////////////////////////////////////////////////////////////////
// CONSTRUCTORS
//////////////////////////////////////////////////////////////////////////

int cLuxScriptHandler::mlRopeIdCount =0;


string gsScriptNull="";

//-----------------------------------------------------------------------

cLuxScriptHandler::cLuxScriptHandler() : iLuxUpdateable("LuxScriptHandler")
{
	mpLowLevelSystem = gpBase->mpEngine->GetSystem()->GetLowLevel();

	
	InitScriptFunctions();
}

//-----------------------------------------------------------------------

cLuxScriptHandler::~cLuxScriptHandler()
{
}

//-----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------


void cLuxScriptHandler::OnStart()
{
	
}

//-----------------------------------------------------------------------


void cLuxScriptHandler::Reset()
{

}

//-----------------------------------------------------------------------

void cLuxScriptHandler::Update(float afTimeStep)
{
}

//-----------------------------------------------------------------------


void cLuxScriptHandler::OnDraw(float afFrameTime)
{
	
}


//-----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// PRIVATE METHODS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------



//-----------------------------------------------------------------------


//////////////////////////////////////////////////////////////////////////
// SCRIPT FUNCTION HELPERS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

void cLuxScriptHandler::AddFunc(const tString& asFunc, void *apFuncPtr)
{
	mpLowLevelSystem->AddScriptFunc(asFunc,apFuncPtr);
}

//-----------------------------------------------------------------------

static iLuxArea* ToArea(iLuxEntity *apEntity)
{
	return static_cast<iLuxArea*>(apEntity);
}

static cLuxArea_Sticky* ToStickyArea(iLuxEntity *apEntity)
{
	return static_cast<cLuxArea_Sticky*>(apEntity);
}

static iLuxProp* ToProp(iLuxEntity *apEntity)
{
	return static_cast<iLuxProp*>(apEntity);
}

static cLuxProp_Object* ToObject(iLuxEntity *apEntity)
{
	return static_cast<cLuxProp_Object*>(apEntity);
}

static cLuxProp_Lamp* ToLamp(iLuxEntity *apEntity)
{
	return static_cast<cLuxProp_Lamp*>(apEntity);
}
static cLuxProp_SwingDoor* ToSwingDoor(iLuxEntity *apEntity)
{
	return static_cast<cLuxProp_SwingDoor*>(apEntity);
}
static cLuxProp_Lever* ToLever(iLuxEntity *apEntity)
{
	return static_cast<cLuxProp_Lever*>(apEntity);
}
static cLuxProp_Wheel* ToWheel(iLuxEntity *apEntity)
{
	return static_cast<cLuxProp_Wheel*>(apEntity);
}
static cLuxProp_MoveObject* ToMoveObject(iLuxEntity *apEntity)
{
	return static_cast<cLuxProp_MoveObject*>(apEntity);
}
static iLuxEnemy* ToEnemy(iLuxEntity *apEntity)
{
	return static_cast<iLuxEnemy*>(apEntity);
}
static cLuxEnemy_ManPig* ToManPig(iLuxEntity *apEntity)
{
	return dynamic_cast<cLuxEnemy_ManPig*>(apEntity);
}
static cLuxEnemy_Wraith* ToWraith(iLuxEntity* apEntity)
{
	return dynamic_cast<cLuxEnemy_Wraith*>(apEntity);
}
static cLuxProp_LevelDoor* ToLevelDoor(iLuxEntity *apEntity)
{
	return static_cast<cLuxProp_LevelDoor*>(apEntity);
}
static cLuxProp_Button* ToButton(iLuxEntity *apEntity)
{
	return static_cast<cLuxProp_Button*>(apEntity);
}

static cLuxProp_NPC* ToNPC(iLuxEntity *apEntity)
{
	return static_cast<cLuxProp_NPC*>(apEntity);
}

static cLuxProp_MultiSlider* ToMultiSlider(iLuxEntity *apEntity)
{
	return static_cast<cLuxProp_MultiSlider*>(apEntity);
}


//-----------------------------------------------------------------------

bool cLuxScriptHandler::GetEntities(const tString& asName,tLuxEntityList &alstEntities, eLuxEntityType aType, int alSubType)
{
	cLuxMap *pMap = gpBase->mpMapHandler->GetCurrentMap();
	if(pMap==NULL)
	{
		Error("GetEntities(..) failed! No map was set!\n");
		return false;
	}

	///////////////////
	// Exact match
	if(cString::CountCharsInString(asName,"*")==0)
	{
		iLuxEntity *pEntity = pMap->GetEntityByName(asName,aType, alSubType);
		if(pEntity==NULL)
		{
			Warning("Entity '%s' with type %d and subtype %d does not exist!\n", asName.c_str(), aType, alSubType);
			return false;
		}
		        
		alstEntities.push_back(pEntity);
	}
	///////////////////
	// Wild card
	else
	{
		tStringVec vWantedStrings;
		tString sSepp = "*";
		cString::GetStringVec(asName,vWantedStrings,&sSepp);

        cLuxEntityIterator it = pMap->GetEntityIterator();
		while(it.HasNext())
		{
			iLuxEntity *pEntity = it.Next();
			if(LuxIsCorrectType(pEntity,aType, alSubType))
			{
				bool bContainsStrings = true;
				int lLastPos = -1;
				
				//Iterate wanted strings and name make sure they exist and show up in correct order.
				for(size_t i=0; i<vWantedStrings.size(); ++i)
				{
					int lPos = cString::GetFirstStringPos(pEntity->GetName(), vWantedStrings[i]);
                    if(lPos <= lLastPos) 
					{
						bContainsStrings = false;
						break;
					}
				}

				if(bContainsStrings) alstEntities.push_back(pEntity);	
			}
		}

		if(alstEntities.empty())
		{
			Warning("Could not find any entities with string '%s'\n", asName.c_str());
			return false;
		}
	}

	return true;
}

//-----------------------------------------------------------------------

iLuxEntity* cLuxScriptHandler::GetEntity(const tString& asName, eLuxEntityType aType, int alSubType)
{
	cLuxMap *pMap = gpBase->mpMapHandler->GetCurrentMap();
	if(pMap==NULL)
	{
		Error("GetEntity(..) failed! No map was set!\n");
		return NULL;
	}


	iLuxEntity *pEntity = pMap->GetEntityByName(asName);
	if(pEntity==NULL)
	{
		Warning("Entity '%s' does not exist!\n", asName.c_str());
		return NULL;
	}

	if(LuxIsCorrectType(pEntity,aType, alSubType)==false)
	{
		Warning("Entity '%s' is not correct type! (%d %d)\n", asName.c_str(), aType, alSubType);
		return NULL;
	}

	return pEntity;
}

//-----------------------------------------------------------------------

bool cLuxScriptHandler::GetParticleSystems(const tString& asName, std::list<cParticleSystem*>& alstParticleSystems)
{
	cLuxMap* pMap = gpBase->mpMapHandler->GetCurrentMap();
	if (pMap == NULL)
	{
		Error("GetParticleSystems(..) failed! No map was set!\n");
		return false;
	}

	///////////////////
	// Exact match
	if (cString::CountCharsInString(asName, "*") == 0)
	{
		cParticleSystem* pPS = pMap->GetWorld()->GetParticleSystem(asName);
		if (pPS == NULL)
		{
			Warning("Particle system '%s' does not exist!\n", asName.c_str());
			return false;
		}

		alstParticleSystems.push_back(pPS);
	}
	///////////////////
	// Wild card
	else
	{
		tStringVec vWantedStrings;
		tString sSepp = "*";
		cString::GetStringVec(asName, vWantedStrings, &sSepp);

		cParticleSystemIterator it = pMap->GetWorld()->GetParticleSystemIterator();
		while (it.HasNext())
		{
			cParticleSystem* pPS = it.Next();
			bool bContainsStrings = true;
			int lLastPos = -1;

			//Iterate wanted strings and name make sure they exist and show up in correct order.
			for (size_t i = 0; i < vWantedStrings.size(); ++i)
			{
				int lPos = cString::GetFirstStringPos(pPS->GetName(), vWantedStrings[i]);
				if (lPos <= lLastPos)
				{
					bContainsStrings = false;
					break;
				}
			}

			if (bContainsStrings) alstParticleSystems.push_back(pPS);
		}

		if (alstParticleSystems.empty())
		{
			Warning("Could not find any particle systems with string '%s'\n", asName.c_str());
			return false;
		}
	}

	return true;
}

//-----------------------------------------------------------------------

iPhysicsBody* cLuxScriptHandler::GetBodyInEntity(iLuxEntity* apEntity, const tString& asName)
{
	if(apEntity == NULL){
		return NULL;
	}
	if(apEntity->GetBodyNum()==0){
		Error("Entity '%s' contains no bodies!\n",apEntity->GetName().c_str());
		return NULL;
	}

	iPhysicsBody *pBody = NULL;
	
	if(asName != "" && apEntity->GetBodyNum()>1)
	{
		for(int i= 0; i < apEntity->GetBodyNum(); ++i)
		{
			tString sBodyName = cString::Sub(apEntity->GetBody(i)->GetName(), (int)apEntity->GetName().size() +1);
			if(sBodyName == asName){
				pBody = apEntity->GetBody(i);
				break;
			}
		}
	}
	else
	{
		pBody = apEntity->GetBody(0);
	}

	if(pBody==NULL)
	{
		Error("Body '%s' could not be found in entity '%s'!\n",asName.c_str(),apEntity->GetName().c_str());
	}

	return pBody;
}

//-----------------------------------------------------------------------

#define BEGIN_SET_PROPERTY(aType, aSubType)\
	tLuxEntityList lstEntities;\
	if(GetEntities(asName, lstEntities,aType, aSubType)==false) return;\
	for(tLuxEntityListIt it = lstEntities.begin(); it != lstEntities.end(); ++it)\
	{\
	iLuxEntity *pEntity = *it;

#define END_SET_PROPERTY }

#define BEGIN_ITERATE_PARTICLESYSTEM()\
	std::list<cParticleSystem *> lstParticleSystems;\
	if(GetParticleSystems(asName, lstParticleSystems)==false) return;\
	for(std::list<cParticleSystem *>::iterator it = lstParticleSystems.begin(); it != lstParticleSystems.end(); ++it)\
	{\
	cParticleSystem * pParticleSystem = *it;

#define END_ITERATE_PARTICLESYSTEM }

//-----------------------------------------------------------------------

void cLuxScriptHandler::InitScriptFunctions()
{
	AddFunc("void Print(string &in asString)", (void *)Print);
	AddFunc("void AddDebugMessage(string &in asString, bool abCheckForDuplicates)",(void *)AddDebugMessage);

	AddFunc("void ProgLog(string &in asLevel, string &in asMessage)", (void *)ProgLog);
	AddFunc("void SetProgLogTimerActive(bool abTimerActive)", (void*)SetProgLogTimerActive);
	AddFunc("void ResetProgLogTimer()", (void*)ResetProgLogTimer);
	AddFunc("void DisplayCounterTimer(bool abDisplayTimer)", (void*)DisplayCounterTimer);
	AddFunc("void DisplayCounterSettings(float afX, float afY, float afZ, float afR, float afG, float afB, float afA, float afTimerSize, string& asTimerAlign)", (void*)DisplayCounterSettings);
	AddFunc("int GetCurrentInGameHour()", (void*)GetCurrentInGameHour);
	AddFunc("int GetCurrentInGameMin()", (void*)GetCurrentInGameMin);
	AddFunc("int GetCurrentInGameSec()", (void*)GetCurrentInGameSec);

	AddFunc("bool ScriptDebugOn()",(void *)ScriptDebugOn);

	AddFunc("float RandFloat(float afMin, float afMax)",(void *)RandFloat);
	AddFunc("int RandInt(int alMin, int alMax)",(void *)RandInt);
	AddFunc("bool StringContains(string &in asString, string &in asSubString)", (void *)StringContains);
	AddFunc("string& StringSub(string &in asString, int alStart, int alCount)", (void *)StringSub);

	AddFunc("void AddTimer(string &in asName, float afTime, string &in asFunction)",(void *)AddTimer);
	AddFunc("void RemoveTimer(string &in asName)",(void *)RemoveTimer);
	AddFunc("float GetTimerTimeLeft(string &in asName)",(void *)GetTimerTimeLeft);

	AddFunc("void SetLocalVarInt(string &in asName, int alVal)",(void *)SetLocalVarInt);
	AddFunc("void SetLocalVarFloat(string &in asName, float afVal)",(void *)SetLocalVarFloat);
	AddFunc("void SetLocalVarBool(string &in asName, bool abVal)", (void*)SetLocalVarBool);
	AddFunc("void SetLocalVarString(string &in asName, string &in asVal)",(void *)SetLocalVarString);
	
	AddFunc("void AddLocalVarInt(string &in asName, int alVal)",(void *)AddLocalVarInt);
	AddFunc("void AddLocalVarFloat(string &in asName, float afVal)",(void *)AddLocalVarFloat);
	AddFunc("void AddLocalVarString(string &in asName, string &in asVal)",(void *)AddLocalVarString);

	AddFunc("int GetLocalVarInt(string &in asName)",(void *)GetLocalVarInt);
	AddFunc("float GetLocalVarFloat(string &in asName)",(void *)GetLocalVarFloat);
	AddFunc("bool GetLocalVarBool(string &in asName)", (void*)GetLocalVarBool);
	AddFunc("string& GetLocalVarString(string &in asName)",(void *)GetLocalVarString);

	AddFunc("void SetGlobalVarInt(string &in asName, int alVal)",(void *)SetGlobalVarInt);
	AddFunc("void SetGlobalVarFloat(string &in asName, float afVal)",(void *)SetGlobalVarFloat);
	AddFunc("void SetGlobalVarBool(string &in asName, bool abVal)", (void*)SetGlobalVarBool);
	AddFunc("void SetGlobalVarString(string &in asName, string &in asVal)",(void *)SetGlobalVarString);

	AddFunc("void AddGlobalVarInt(string &in asName, int alVal)",(void *)AddGlobalVarInt);
	AddFunc("void AddGlobalVarFloat(string &in asName, float afVal)",(void *)AddGlobalVarFloat);
	AddFunc("void AddGlobalVarString(string &in asName, string &in asVal)",(void *)AddGlobalVarString);

	AddFunc("int GetGlobalVarInt(string &in asName)",(void *)GetGlobalVarInt);
	AddFunc("float GetGlobalVarFloat(string &in asName)",(void *)GetGlobalVarFloat);
	AddFunc("bool GetGlobalVarBool(string &in asName)", (void*)GetGlobalVarBool);
	AddFunc("string& GetGlobalVarString(string &in asName)",(void *)GetGlobalVarString);

	AddFunc("void StartCredits(string &in asMusic, bool abLoopMusic, string &in asTextCat, string &in asTextEntry, int alEndNum)",(void *)StartCredits);
	AddFunc("void StartCreditsAndRankScreen(string& asMusicCredits, bool abLoopMusicCredits, string& asTextCat, string& asTextEntry, bool abCreditsBackground, string& asMusicRank, bool abLoopMusicRank, float afRankMusicVol, float afRankMusicFade, string& asImageName)", (void*)StartCreditsAndRankScreen);
	AddFunc("void SetEnding(string& asEnding, bool abCompletedOnHardMode)", (void*)SetEnding);
	AddFunc("void ForceExitDirectlyToMenu()", (void*)ForceExitDirectlyToMenu);
	AddFunc("void AddKeyPart(int alKeyPart)", (void *)AddKeyPart);
	AddFunc("bool HardModeEnabled()", (void*)HardModeEnabled);
	AddFunc("string& GetPreviousEnding()", (void*)GetPreviousEnding);
	AddFunc("bool GetEndingCompletion(string& asEndingType)", (void*)GetEndingCompletion);
	AddFunc("int GetGameClears()", (void*)GetGameClears);

	AddFunc("void StartDemoEnd()",(void *)StartDemoEnd);
	AddFunc("void StartRankScreen(string& asMusic, bool abLoopMusic, float afRankMusicVol, float afRankMusicFade, string& asImageName)", (void*)StartRankScreen);

	AddFunc("void AutoSave()", (void *)AutoSave);
	AddFunc("void DoHardModeSave()", (void*)DoHardModeSave);
	AddFunc("void CheckPoint(string &in asName,string &in asStartPos ,string &in asCallback, string &in asDeathHintCat, string &in asDeathHintEntry)", (void *)CheckPoint);

	AddFunc("void ChangeMap(string &in asMapName, string &in asStartPos, string &in asStartSound, string &in asEndSound)",(void *)ChangeMap);
	AddFunc("void ClearSavedMaps()",(void *)ClearSavedMaps);
	AddFunc("void CreateDataCache()",(void *)CreateDataCache);
	AddFunc("void DestroyDataCache()",(void *)DestroyDataCache);
	AddFunc("void SetMapDisplayNameEntry(string &in asNameEntry)",(void *)SetMapDisplayNameEntry);
	AddFunc("void SetSkyBoxActive(bool abActive)",(void *)SetSkyBoxActive);
	AddFunc("void SetSkyBoxTexture(string &in asTexture)",(void *)SetSkyBoxTexture);
	AddFunc("void SetSkyBoxColor(float afR, float afG, float afB, float afA)",(void *)SetSkyBoxColor);

	AddFunc("void UnlockAchievement(string &in asName)", (void *)UnlockAchievement);

	AddFunc("void SetFogActive(bool abActive)",(void *)SetFogActive);
	AddFunc("void SetFogColor(float afR, float afG, float afB, float afA)",(void *)SetFogColor);
	AddFunc("void SetFogProperties(float afStart, float afEnd, float afFalloffExp, bool abCulling)",(void *)SetFogProperties);

	AddFunc("void SetupLoadScreen(string &in asTextCat, string &in asTextEntry, int alRandomNum, string &in asImageFile)",(void *)SetupLoadScreen);
	
	AddFunc("void FadeIn(float afTime)",(void *)FadeIn);
	AddFunc("void FadeOut(float afTime)",(void *)FadeOut);
	AddFunc("void FadeImageTrailTo(float afAmount, float afSpeed)",(void *)FadeImageTrailTo);
	AddFunc("void FadeSepiaColorTo(float afAmount, float afSpeed)",(void *)FadeSepiaColorTo);
	//AddFunc("void FadeBlackAndWhiteAmountTo(float afAmount, float afSpeed)", (void *)FadeBlackAndWhiteAmountTo);
	AddFunc("void FadeRadialBlurTo(float afSize, float afSpeed)",(void *)FadeRadialBlurTo);
	AddFunc("void SetRadialBlurStartDist(float afStartDist)",(void *)SetRadialBlurStartDist);

	AddFunc("void StartEffectFlash(float afFadeIn, float afWhite, float afFadeOut)",(void *)StartEffectFlash);
	AddFunc("void StartCustomColorEffectFlash(float afR, float afG, float afB, float afA, float afFadeIn, float afFadeOut)", (void*)StartCustomColorEffectFlash);
	AddFunc("void StartEffectEmotionFlash(string &in asTextCat, string &in asTextEntry, string &in asSound)",(void *)StartEffectEmotionFlash);

	AddFunc("void SetInDarknessEffectsActive(bool abX)",(void *)SetInDarknessEffectsActive);

	AddFunc("void AddEffectVoice(string &in asVoiceFile, string &in asEffectFile, string &in asTextCat, string &in asTextEntry, bool abUsePostion,  string &in asPosEnitity, float afMinDistance, float afMaxDistance)",(void *)AddEffectVoice);
	AddFunc("void StopAllEffectVoices(float afFadeOutTime)",(void *)StopAllEffectVoices);
	AddFunc("bool GetEffectVoiceActive()",(void *)GetEffectVoiceActive);
	AddFunc("void SetEffectVoiceOverCallback(string &in asFunc)", (void *)SetEffectVoiceOverCallback);
	AddFunc("void StartScreenShake(float afAmount, float afTime, float afFadeInTime,float afFadeOutTime)",(void *)StartScreenShake);
	AddFunc("bool GetFlashbackIsActive()", (void *)GetFlashbackIsActive);

	AddFunc("void ShowScreenImage(string &in asImageName, float afX, float afY, float afScale, bool abUseRelativeCoordinates, float afDuration, float afFadeIn, float afFadeOut)", (void*)ShowScreenImage);
	AddFunc("void HideScreenImageImmediately()", (void*)HideScreenImageImmediately);
	AddFunc("void HideScreenImageWithFade(float afFadeOut)", (void*)HideScreenImageWithFade);

	AddFunc("void SetInsanitySetEnabled(string &in asSet, bool abX)", (void *)SetInsanitySetEnabled);
	AddFunc("void StartRandomInsanityEvent()", (void *)StartRandomInsanityEvent);
	AddFunc("void StartInsanityEvent(string &in asEventName)", (void *)StartInsanityEvent);
	AddFunc("void StopCurrentInsanityEvent()", (void *)StopCurrentInsanityEvent);
	AddFunc("void InsanityEventIsActive()", (void *)InsanityEventIsActive);

	AddFunc("void StartPlayerSpawnPS(string &in asSPSFile)", (void *)StartPlayerSpawnPS);
	AddFunc("void StopPlayerSpawnPS()", (void *)StartPlayerSpawnPS);

	AddFunc("void PlayGuiSound(string &in asSoundFile, float afVolume)",(void *)PlayGuiSound);

	AddFunc("void SetPlayerActive(bool abActive)",(void *)SetPlayerActive);
	AddFunc("void ChangePlayerStateToNormal()",(void *)ChangePlayerStateToNormal);
	AddFunc("void SetPlayerCrouching(bool abCrouch)",(void *)SetPlayerCrouching);
	AddFunc("bool GetPlayerCrouching()", (void *)GetPlayerCrouching);
	AddFunc("void AddPlayerBodyForce(float afX, float afY, float afZ, bool abUseLocalCoords)",(void *)AddPlayerBodyForce);
	AddFunc("void ShowPlayerCrossHairIcons(bool abX)",(void *)ShowPlayerCrossHairIcons);
	
	AddFunc("void SetPlayerPos(float afX, float afY, float afZ)",(void *)SetPlayerPos);
	AddFunc("float GetPlayerPosX()",(void *)GetPlayerPosX);
	AddFunc("float GetPlayerPosY()",(void *)GetPlayerPosY);
	AddFunc("float GetPlayerPosZ()",(void *)GetPlayerPosZ);
	
	AddFunc("void SetPlayerSanity(float afSanity)",(void *)SetPlayerSanity);
	AddFunc("void AddPlayerSanity(float afSanity)",(void *)AddPlayerSanity);
	AddFunc("float GetPlayerSanity()",(void *)GetPlayerSanity);
	AddFunc("void SetPlayerHealth(float afHealth)",(void *)SetPlayerHealth);
	AddFunc("void AddPlayerHealth(float afHealth)",(void *)AddPlayerHealth);
	AddFunc("float GetPlayerHealth()",(void *)GetPlayerHealth);
	AddFunc("int GetPlayerHealthLevel()", (void*)GetPlayerHealthLevel);
	AddFunc("void SetPlayerLampOil(float afOil)",(void *)SetPlayerLampOil);
	AddFunc("void AddPlayerLampOil(float afOil)",(void *)AddPlayerLampOil);
	AddFunc("float GetPlayerLampOil()",(void *)GetPlayerLampOil);

	AddFunc("float GetPlayerSpeed()",(void *)GetPlayerSpeed);
	AddFunc("float GetPlayerYSpeed()",(void *)GetPlayerYSpeed);
	AddFunc("void MovePlayerForward(float afAmount)",(void *)MovePlayerForward);
	AddFunc("void SetPlayerPermaDeathSound(string &in asSound)",(void *)SetPlayerPermaDeathSound);

	AddFunc("void SetSanityDrainDisabled(bool abX)",(void *)SetSanityDrainDisabled);
	AddFunc("void GiveSanityBoost()",(void *)GiveSanityBoost);
	AddFunc("void GiveSanityBoostSmall()", (void *)GiveSanityBoostSmall);
	AddFunc("void GiveSanityDamage(float afAmount, bool abUseEffect)",(void *)GiveSanityDamage);

	AddFunc("void GivePlayerDamage(float afAmount, string &in asType, bool abSpinHead, bool abLethal)",(void *)GivePlayerDamage);
	AddFunc("void FadePlayerFOVMulTo(float afX, float afSpeed)",(void *)FadePlayerFOVMulTo);
	AddFunc("void FadePlayerAspectMulTo(float afX, float afSpeed)",(void *)FadePlayerAspectMulTo);
	AddFunc("void FadePlayerPitchTo(float afX, float afSpeedMul, float afMaxSpeed)", (void*)FadePlayerPitchTo);
	AddFunc("void FadePlayerRollTo(float afX, float afSpeedMul, float afMaxSpeed)",(void *)FadePlayerRollTo);
	AddFunc("void MovePlayerHeadPos(float afX, float afY, float afZ, float afSpeed, float afSlowDownDist)",(void *)MovePlayerHeadPos);

	AddFunc("void StartPlayerLookAt(string &in asEntityName, float afSpeedMul, float afMaxSpeed,string &in asAtTargetCallback)",(void *)StartPlayerLookAt);
	AddFunc("void StopPlayerLookAt()",(void *)StopPlayerLookAt);

	AddFunc("void SetPlayerMoveSpeedMul(float afMul)",(void *)SetPlayerMoveSpeedMul);
	AddFunc("void SetPlayerRunSpeedMul(float afMul)",(void *)SetPlayerRunSpeedMul);
	AddFunc("void SetPlayerLookSpeedMul(float afMul)",(void *)SetPlayerLookSpeedMul);
	AddFunc("void SetPlayerJumpForceMul(float afMul)",(void *)SetPlayerJumpForceMul);
	AddFunc("void SetPlayerJumpDisabled(bool abX)",(void *)SetPlayerJumpDisabled);
	AddFunc("void SetPlayerCrouchDisabled(bool abX)",(void *)SetPlayerCrouchDisabled);
	AddFunc("void SetPlayerFallDamageDisabled(bool abX)",(void *)SetPlayerFallDamageDisabled);

	AddFunc("void TeleportPlayer(string &in asStartPosName)",(void *)TeleportPlayer);
	AddFunc("void SetLanternActive(bool abX, bool abUseEffects)",(void *)SetLanternActive);
	AddFunc("bool GetLanternActive()",(void *)GetLanternActive);
	AddFunc("void SetLanternDisabled(bool abX)",(void *)SetLanternDisabled);
	AddFunc("void SetLanternLitCallback(string &in asCallback)",(void *)SetLanternLitCallback);
	AddFunc("void SetLanternFlickerActive(bool abX)", (void*)SetLanternFlickerActive);
	AddFunc("bool GetLanternFlickerActive()", (void*)GetLanternFlickerActive);
	AddFunc("void SetMessage(string &in asTextCategory, string &in asTextEntry, float afTime)",(void *)SetMessage);
	AddFunc("void SetCustomTextMessage(string &in asTextCategory, string &in asTextEntry, float afX, float afY, float afZ, float afR, float afG, float afB, float afTime)", (void*)SetCustomTextMessage);
	AddFunc("void StartCustomPauseMessage(string &in asMainTextCategory, string &in asMainTextEntry, bool abMultipleButtons, string &in asButton1TextCat, string &in asButton1TextEnt, string &in asButton2TextCat, string &in asButton2TextEnt, string &in asCallback)", (void*)StartCustomPauseMessage);
	AddFunc("void SetDeathHint(string &in asTextCategory, string &in asTextEntry)",(void *)SetDeathHint);
	AddFunc("void DisableDeathStartSound()",(void *)DisableDeathStartSound);

	AddFunc("void SetLantern(int alLantern)", (void*)SetLantern);
	AddFunc("int GetLantern()", (void*)GetLantern);

	AddFunc("void AddNote(string &in asNameAndTextEntry, string &in asImage)",(void *)AddNote);
	AddFunc("void AddDiary(string &in asNameAndTextEntry, string &in asImage)",(void *)AddDiary);
	AddFunc("void ReturnOpenJournal(bool abOpenJournal)",(void *)ReturnOpenJournal);

	AddFunc("void AddQuest(string &in asName, string &in asNameAndTextEntry)",(void *)AddQuest);
	AddFunc("void CompleteQuest(string &in asName, string &in asNameAndTextEntry)",(void *)CompleteQuest);
	AddFunc("bool QuestIsCompleted(string &in asName)",(void *)QuestIsCompleted);
	AddFunc("bool QuestIsAdded(string &in asName)",(void *)QuestIsAdded);
	AddFunc("void SetNumberOfQuestsInMap(int alNumberOfQuests)",(void *)SetNumberOfQuestsInMap);

	AddFunc("void GiveHint(string &in asName, string &in asMessageCat, string &in asMessageEntry, float afTimeShown)",(void *)GiveHint);
	AddFunc("void RemoveHint(string &in asName)", (void *)RemoveHint);
	AddFunc("void BlockHint(string &in asName)", (void *)BlockHint);
	AddFunc("void UnBlockHint(string &in asName)", (void *)UnBlockHint);

	AddFunc("void ExitInventory()",(void *)ExitInventory);
	AddFunc("void SetInventoryDisabled(bool abX)",(void *)SetInventoryDisabled);
	AddFunc("void SetInventoryMessage(string &in asTextCategory, string &in asTextEntry, float afTime)",(void *)SetInventoryMessage);
	
	AddFunc("void GiveItem(string &in asName, string &in asType, string &in asSubTypeName, string &in asImageName, float afAmount)",(void *)GiveItem);
	AddFunc("void GiveItemFromFile(string& asName, string& asFileName)",(void *)GiveItemFromFile);
	AddFunc("void RemoveItem(string &in asName)",(void *)RemoveItem);
	AddFunc("bool HasItem(string &in asName)",(void *)HasItem);

	AddFunc("void AddCombineCallback(string &in asName, string &in asItemA, string &in asItemB, string &in asFunction, bool abAutoDestroy)",(void *)AddCombineCallback);
	AddFunc("void RemoveCombineCallback(string &in asName)",(void *)RemoveCombineCallback);

	AddFunc("void AddUseItemCallback(string &in asName, string &in asItem, string &in asEntity, string &in asFunction, bool abAutoDestroy)",(void *)AddUseItemCallback);
	AddFunc("void RemoveUseItemCallback(string &in asName)",(void *)RemoveUseItemCallback);

	AddFunc("void PreloadParticleSystem(string& asPSFile)",(void *)PreloadParticleSystem);
	AddFunc("void PreloadSound(string& asSoundFile)",(void *)PreloadSound);

	AddFunc("void CreateParticleSystemAtEntity(string &in asPSName, string &in asPSFile, string &in asEntity, bool abSavePS)",(void *)CreateParticleSystemAtEntity);
	AddFunc("void CreateParticleSystemAtEntityExt(	string &in asPSName, string &in asPSFile, string &in asEntity, bool abSavePS, float afR, float afG, float afB, float afA, bool abFadeAtDistance, float afFadeMinEnd, float afFadeMinStart, float afFadeMaxStart, float afFadeMaxEnd)", (void *)CreateParticleSystemAtEntityExt);
	AddFunc("void DestroyParticleSystem(string &in asName)",(void *)DestroyParticleSystem);
	AddFunc("void DestroyParticleSystemInstantly(string &in asName)", (void*)DestroyParticleSystemInstantly);
	AddFunc("void SetParticleSystemActive(string &in asName, bool abActive)", (void*)SetParticleSystemActive);
	
	AddFunc("void PlaySoundAtEntity(string &in asSoundName, string &in asSoundFile, string &in asEntity, float afFadeSpeed, bool abSaveSound)",(void *)PlaySoundAtEntity);
	AddFunc("void FadeInSound(string& asSoundName, float afFadeTime, bool abPlayStart)",(void *)FadeInSound);
	AddFunc("void StopSound(string &in asSoundName, float afFadeTime)",(void *)StopSound);
	AddFunc("void PlayMusic(string &in asMusicFile, bool abLoop, float afVolume, float afFadeTime, int alPrio, bool abResume)",(void *)PlayMusic);
	AddFunc("void StopMusic(float afFadeTime, int alPrio)",(void *)StopMusic);
	AddFunc("void FadeGlobalSoundVolume(float afDestVolume, float afTime)",(void *)FadeGlobalSoundVolume);
	AddFunc("void FadeGlobalSoundSpeed(float afDestSpeed, float afTime)",(void *)FadeGlobalSoundSpeed);
	
	AddFunc("void SetFogAreaVisible(string& asFogAreaName, bool abVisible)", (void*)SetFogAreaVisible);
	AddFunc("void SetLightVisible(string &in asLightName, bool abVisible)",(void *)SetLightVisible);
	AddFunc("void FadeLightTo(string &in asLightName, float afR, float afG, float afB, float afA, float afRadius, float afTime)",(void *)FadeLightTo);
	AddFunc("void FadeLightToExt(string& asLightName, float afR, float afG, float afB, float afA, float afRadius, float afBrightness, float afTime)", (void*)FadeLightToExt);
	AddFunc("void FadeLightBrightnessTo(string& asLightName, float afBrightness, float afTime)", (void*)FadeLightBrightnessTo);
	AddFunc("void SetLightFlickerActive(string& asLightName, bool abActive)", (void *)SetLightFlickerActive);
	AddFunc("void SetLampFlickerActive(string &in asName, bool abActive)", (void*)SetLampFlickerActive);

	AddFunc("float GetLightColorR(string& asLightName)", (void*)GetLightColorR);
	AddFunc("float GetLightColorB(string& asLightName)", (void*)GetLightColorB);
	AddFunc("float GetLightColorG(string& asLightName)", (void*)GetLightColorG);
	AddFunc("float GetLightAlpha(string& asLightName)", (void*)GetLightAlpha);
	AddFunc("float GetLightBrightness(string& asLightName)", (void*)GetLightBrightness);

	AddFunc("void SetEntityActive(string &in asName, bool abActive)",(void *)SetEntityActive);
	AddFunc("void SetEntityVisible(string &in asName, bool abVisible)",(void *)SetEntityVisible);
	AddFunc("bool GetEntityExists(string &in asName)",(void *)GetEntityExists);
	AddFunc("void SetEntityPos(string &in asName, float afX, float afY, float afZ)",(void *)SetEntityPos);
	AddFunc("float GetEntityPosX(string &in asName)",(void *)GetEntityPosX);
	AddFunc("float GetEntityPosY(string &in asName)",(void *)GetEntityPosY);
	AddFunc("float GetEntityPosZ(string &in asName)",(void *)GetEntityPosZ);
	AddFunc("float GetEntityRotationX(string &in asName, int body)", (void*)GetEntityRotationX);
	AddFunc("float GetEntityRotationY(string &in asName, int body)", (void*)GetEntityRotationY);
	AddFunc("float GetEntityRotationZ(string &in asName, int body)", (void*)GetEntityRotationZ);
	AddFunc("void SetEntityRotation(string &in asName, float afrX, float afrY, float afrZ, int body)", (void*)SetEntityRotation);
	AddFunc("void SetEntityRotationAndPosition(string &in asName, float afrX, float afrY, float afrZ, float afpX, float afpY, float afpZ, int body)", (void*)SetEntityRotationAndPosition);
	
	AddFunc("float GetBonePosX(string &in asEntity, string &in asBone)", (void*)GetBonePosX);
	AddFunc("float GetBonePosY(string &in asEntity, string &in asBone)", (void*)GetBonePosY);
	AddFunc("float GetBonePosZ(string &in asEntity, string &in asBone)", (void*)GetBonePosZ);

	AddFunc("float GetBoneRotX(string &in asEntity, string &in asBone)", (void*)GetBoneRotX);
	AddFunc("float GetBoneRotY(string &in asEntity, string &in asBone)", (void*)GetBoneRotY);
	AddFunc("float GetBoneRotZ(string &in asEntity, string &in asBone)", (void*)GetBoneRotZ);

	AddFunc("void AttachPropToBone(string &in asChildEntityName, string &in asParentEntityName, string &in asParentBoneName, float fPosX, float fPosY, float fPosZ, float fRotX, float fRotY, float fRotZ)", (void*)AttachPropToBone);
	AddFunc("void DetachPropFromBone(string &in asChildEntityName)", (void*)DetachPropFromBone);

	AddFunc("void SetEntityCustomFocusCrossHair(string &in asName, string &in asCrossHair)",(void *)SetEntityCustomFocusCrossHair);
	AddFunc("void CreateEntityAtArea(string &in asEntityName, string &in asEntityFile, string &in asAreaName, bool abFullGameSave)",(void *)CreateEntityAtArea);
	AddFunc("void ReplaceEntity(string &in asName, string &in asBodyName, string &in asNewEntityName, string &in asNewEntityFile, bool abFullGameSave)",(void *)ReplaceEntity);
	AddFunc("void PlaceEntityAtEntity(string &in asName, string &in asTargetEntity, string &in asTargetBodyName, bool abUseRotation)",(void *)PlaceEntityAtEntity);
	AddFunc("void SetEntityPlayerLookAtCallback(string &in asName, string &in asCallback, bool abRemoveWhenLookedAt)",(void *)SetEntityPlayerLookAtCallback);
	AddFunc("void SetEntityPlayerInteractCallback(string &in asName, string &in asCallback, bool abRemoveOnInteraction)",(void *)SetEntityPlayerInteractCallback);
	AddFunc("void SetEntityCallbackFunc(string &in asName, string &in asCallback)", (void *)SetEntityCallbackFunc);
	AddFunc("void SetEntityConnectionStateChangeCallback(string& asName, string& asCallback)", (void *)SetEntityConnectionStateChangeCallback);
	AddFunc("void SetEntityInteractionDisabled(string& asName, bool abDisabled)", (void *)SetEntityInteractionDisabled);
	AddFunc("bool GetEntitiesCollide(string &in asEntityA, string &in asEntityB)",(void *)GetEntitiesCollide);
	AddFunc("float GetEntitiesDistance(string &in asEntityA, string &in asEntityB)", (void*)GetEntitiesDistance);
	
	AddFunc("void SetPropEffectActive(string &in asName, bool abActive, bool abFadeAndPlaySounds)", (void *)SetPropEffectActive);
	AddFunc("void SetPropActiveAndFade(string &in asName, bool abActive, float afFadeTime)",(void *)SetPropActiveAndFade);
	AddFunc("void SetPropStaticPhysics(string &in asName, bool abX)", (void *)SetPropStaticPhysics);
	AddFunc("bool GetPropIsInteractedWith(string &in asName)", (void *)GetPropIsInteractedWith);
	AddFunc("void RotatePropToSpeed(string &in asName, float afAcc, float afGoalSpeed, float afAxisX, float afAxisY, float afAxisZ, bool abResetSpeed, string &in asOffsetArea)", (void *)RotatePropToSpeed);	
	AddFunc("void StopPropMovement(string &in asName)", (void *)StopPropMovement);	

	AddFunc("void AddAttachedPropToProp(string& asPropName, string& asAttachName, string& asAttachFile, float fPosX, float fPosY, float fPosZ, float fRotX, float fRotY, float fRot)",(void *)AddAttachedPropToProp);
	AddFunc("void AttachPropToProp(string& asPropName, string& asAttachName, string& asAttachFile, float fPosX, float fPosY, float fPosZ, float fRotX, float fRotY, float fRot)",(void *)AttachPropToProp);
	AddFunc("void RemoveAttachedPropFromProp(string& asPropName, string& asAttachName)",(void *)RemoveAttachedPropFromProp);

	AddFunc("void SetLampLit(string &in asName, bool abLit, bool abEffects)",(void *)SetLampLit); 
	AddFunc("void SetSwingDoorLocked(string &in asName, bool abLocked, bool abEffects)",(void *)SetSwingDoorLocked);
	AddFunc("void SetSwingDoorClosed(string &in asName, bool abClosed, bool abEffects)",(void *)SetSwingDoorClosed);
	AddFunc("void SetSwingDoorDisableAutoClose(string &in asName, bool abDisableAutoClose)",(void *)SetSwingDoorDisableAutoClose);
	AddFunc("void SetLevelDoorLocked(string &in asName, bool abLocked)", (void *)SetLevelDoorLocked);
	AddFunc("void SetLevelDoorLockedSound(string &in asName, string &in asSound)", (void *)SetLevelDoorLockedSound);
	AddFunc("void SetLevelDoorLockedText(string &in asName, string &in asTextCat, string &in asTextEntry)", (void *)SetLevelDoorLockedText);
	AddFunc("bool GetSwingDoorLocked(string &in asName)",(void *)GetSwingDoorLocked);
	AddFunc("bool GetSwingDoorClosed(string &in asName)",(void *)GetSwingDoorClosed);
	AddFunc("int GetSwingDoorState(string &in asName)",(void *)GetSwingDoorState);
	AddFunc("void SetSwingDoorOpenAmount(string &in asName, float afOpenAmount, float afDuration, bool abOpenTowardsMaxAngle)", (void*)SetSwingDoorOpenAmount);
	AddFunc("void SetPropObjectStuckState(string &in asName, int alState)",(void *)SetPropObjectStuckState);
	AddFunc("void SetWheelAngle(string &in asName, float afAngle, bool abAutoMove)",(void *)SetWheelAngle);
	AddFunc("void SetWheelStuckState(string &in asName, int alState, bool abEffects)",(void *)SetWheelStuckState);
	AddFunc("void SetLeverStuckState(string &in asName, int alState, bool abEffects)",(void *)SetLeverStuckState);
	AddFunc("void SetWheelInteractionDisablesStuck(string &in asName, bool abX)",(void *)SetWheelInteractionDisablesStuck);
	AddFunc("void SetLeverInteractionDisablesStuck(string &in asName, bool abX)",(void *)SetLeverInteractionDisablesStuck);
	AddFunc("int GetLeverState(string &in asName)",(void *)GetLeverState);
	
	AddFunc("void SetMultiSliderStuckState(string &in asName, int alStuckState, bool abEffects)",(void *)SetMultiSliderStuckState);
	AddFunc("void SetMultiSliderCallback(string &in asName, string &in asCallback)",(void *)SetMultiSliderCallback);
	
	AddFunc("void SetButtonSwitchedOn(string &in asName, bool abSwitchedOn, bool abEffects)",(void *)SetButtonSwitchedOn);
	AddFunc("void SetAllowStickyAreaAttachment(bool abX)", (void *)SetAllowStickyAreaAttachment);
	AddFunc("void AttachPropToStickyArea(string &in asAreaName, string &in asProp)", (void *)AttachPropToStickyArea);
	AddFunc("void AttachBodyToStickyArea(string& asAreaName, string& asBody)", (void *)AttachBodyToStickyArea);
	AddFunc("void DetachFromStickyArea(string &in asAreaName)", (void *)DetachFromStickyArea);
	AddFunc("void SetNPCAwake(string &in asName, bool abAwake, bool abEffects)",(void *)SetNPCAwake);
	AddFunc("void SetNPCFollowPlayer(string &in asName, bool abX)",(void *)SetNPCFollowPlayer);

	AddFunc("void AttachAreaToProp(string &in asAreaName, string &in asProp, int alBody)", (void*)AttachAreaToProp);

	AddFunc("void SetEnemyDisabled(string &in asName, bool abDisabled)",(void *)SetEnemyDisabled);
	AddFunc("void SetEnemyIsHallucination(string &in asName, bool abX)",(void *)SetEnemyIsHallucination);
	AddFunc("void FadeEnemyToSmoke(string &in asName, bool abPlaySound)",(void *)FadeEnemyToSmoke);
	AddFunc("void SetEnemyDisableTriggers(string &in asName, bool abX)",(void *)SetEnemyDisableTriggers);
	AddFunc("void ShowEnemyPlayerPosition(string &in asName)",(void *)ShowEnemyPlayerPosition);
	AddFunc("void SetEnemyEndOfPatrolCallback(string& asName, string &in asFunc, bool abRemoveWhenCalled)", (void*)SetEnemyEndOfPatrolCallback);
	AddFunc("void ForceEnemyWaitState(string &in asName)", (void*)ForceEnemyWaitState);
	AddFunc("void AlertEnemyOfPlayerPresence(string &in asName)",(void *)AlertEnemyOfPlayerPresence);
	AddFunc("void MakeEnemySearchPosition(string& asName, float afX, float afY, float afZ)", (void*)MakeEnemySearchPosition);
	AddFunc("void AddEnemyPatrolNode(string &in asEnemyName, string &in asNodeName, float afWaitTime, string &in asAnimation)",(void *)AddEnemyPatrolNode);
	AddFunc("void PlayEnemyAnimation(string &in asEnemyName, string &in asAnimName, bool abLoop, float afFadeTime)", (void*)PlayEnemyAnimation);
	AddFunc("void ClearEnemyPatrolNodes(string &in asEnemyName)",(void *)ClearEnemyPatrolNodes);
	AddFunc("void SetEnemySanityDecreaseActive(string &in asName, bool abX)",(void *)SetEnemySanityDecreaseActive);
	AddFunc("void TeleportEnemyToNode(string &in asEnemyName, string &in asNodeName, bool abChangeY)",(void *)TeleportEnemyToNode);
	AddFunc("void TeleportEnemyToEntity(string &in asEnemyName, string &in asTargetEntity, string &in asTargetBody, bool abChangeY)",(void *)TeleportEnemyToEntity);
	AddFunc("float GetEnemyPlayerDistance(string &in asEnemyName)", (void*)GetEnemyPlayerDistance);
	AddFunc("bool GetPlayerCanSeeEnemy(string &in asEnemyName)", (void*)GetPlayerCanSeeEnemy);

	AddFunc("string& GetManPigCurrentPose(string& asName)", (void*)GetManPigCurrentPose);
	AddFunc("void ChangeManPigPose(string&in asName, string&in asPoseType)",(void *)ChangeManPigPose);
	AddFunc("void ChangeEnemyPatrolSpeed(string& asName, string& asSpeedType)", (void*)ChangeEnemyPatrolSpeed);
	AddFunc("void ChangeEnemyState(string& asName, string& asState)", (void*)ChangeEnemyState);
	AddFunc("void SetManPigToFlee(string& asName, bool abX)", (void*)SetManPigToFlee);
	AddFunc("void SetManPigToThreatenOnAlert(string& asName, bool abX)", (void*)SetManPigToThreatenOnAlert);
	AddFunc("void SetManPigIdleBehavior(string& asName, string& asBehaviorType)", (void*)SetManPigIdleBehavior);
	AddFunc("void SetTeslaPigFadeDisabled(string&in asName, bool abX)",(void *)SetTeslaPigFadeDisabled);
	AddFunc("void SetTeslaPigSoundDisabled(string&in asName, bool abX)",(void *)SetTeslaPigSoundDisabled);
	AddFunc("void SetTeslaPigEasyEscapeDisabled(string&in asName, bool abX)",(void *)SetTeslaPigEasyEscapeDisabled);
	AddFunc("void ForceTeslaPigSighting(string&in asName)",(void *)ForceTeslaPigSighting);

	AddFunc("void SetWraithStealthDashMode(string&in asName, bool abX)", (void*)SetWraithStealthDashMode);
	AddFunc("void SetWraithStealthDashAtWill(string&in asName, bool abX)", (void*)SetWraithStealthDashAtWill);
	AddFunc("void SetWraithCanMelee(string&in asName, bool abX)", (void*)SetWraithCanMelee);
	AddFunc("void SetWraithStealthDashExitDistance(string& asName, float afNodeDistance)", (void*)SetWraithStealthDashExitDistance);
	AddFunc("void SetWraithStealthDashEnterDistance(string& asName, float afNodeDistance)", (void*)SetWraithStealthDashEnterDistance);
	AddFunc("void SetWraithHeatRayDamageLightAmount(string& asName, float afLightAmount)", (void*)SetWraithHeatRayDamageLightAmount);
	AddFunc("void SetWraithHeatRayDamageSpeed(string& asName, float afDamageSpeed)", (void*)SetWraithHeatRayDamageSpeed);
	AddFunc("void SetWraithBurnDamageMinTime(string& asName, float afMinTime)", (void*)SetWraithBurnDamageMinTime);
	AddFunc("void SetWraithBurnDamageMaxTime(string& asName, float afMaxTime)", (void*)SetWraithBurnDamageMaxTime);
	AddFunc("void SetWraithBurnDamageTime(string& asName, float afMinTime, float afMaxTime)", (void*)SetWraithBurnDamageTime);
	AddFunc("void SetWraithMinBurnDamage(string& asName, float afMinDamage)", (void*)SetWraithMinBurnDamage);
	AddFunc("void SetWraithMaxBurnDamage(string& asName, float afMaxDamage)", (void*)SetWraithMaxBurnDamage);
	AddFunc("void SetWraithMaxBurnDamage(string& asName, float afMaxDamage)", (void*)SetWraithMaxBurnDamage);
	AddFunc("void SetWraithAttackType(string& asName, string& asAttackType)", (void*)SetWraithAttackType);
	AddFunc("bool GetWraithInStealthDashMode(string& asEnemyName)", (void*)GetWraithInStealthDashMode);
	AddFunc("float GetWraithStealthDashNodesLeft(string& asEnemyName)", (void*)GetWraithStealthDashNodesLeft);
	
	AddFunc("string& GetEnemyStateName(string &in asName)",(void *)GetEnemyStateName);
	AddFunc("string& GetEnemyPreviousState(string &in asName)", (void*)GetEnemyPreviousState);
	AddFunc("void SetEnemyBlind(string&in asName, bool abX)", (void*)SetEnemyBlind);
	AddFunc("void SetEnemyDeaf(string&in asName, bool abX)", (void*)SetEnemyDeaf);
	AddFunc("float GetEnemyHealth(string& asName)", (void*) GetEnemyHealth);
	AddFunc("void SetEnemyHealth(string &in asName, float afHealth)", (void*)SetEnemyHealth);
	AddFunc("float GetEnemyHearVolume(string& asName)", (void*)GetEnemyHearVolume);
	AddFunc("void SetEnemyHearVolume(string &in asName, float afHearVolume)", (void*)SetEnemyHearVolume);
	AddFunc("float GetEnemyRunSpeedMul(string& asName)", (void*)GetEnemyRunSpeedMul);
	AddFunc("void SetEnemyRunSpeedMul(string &in asName, float afRunSpeedMul)", (void*)SetEnemyRunSpeedMul);
	AddFunc("float GetEnemySearchTime(string& asName)", (void*)GetEnemySearchTime);
	AddFunc("void SetEnemySearchTime(string& asName, float afSearchTime)", (void*)SetEnemySearchTime);
	AddFunc("void SetEnemyRegenHealthSpeed(string& asName, float afRegenHealthSpeed)", (void*)SetEnemyRegenHealthSpeed);
	AddFunc("float GetEnemyRegenHealthSpeed(string& asName)", (void*)GetEnemyRegenHealthSpeed);
	AddFunc("void SetManPigFleeHealth(string& asName, float afFleeHealth)", (void*)SetManPigFleeHealth);
	AddFunc("float GetManPigFleeHealth(string& asName)", (void*)GetManPigFleeHealth);
	AddFunc("void SetManPigCanTrackTeleport(string& asName, bool abX)", (void*)SetManPigCanTrackTeleport);

	AddFunc("void SetPropHealth(string &in asName, float afHealth)",(void *)SetPropHealth);
	AddFunc("void AddPropHealth(string &in asName, float afHealth)",(void *)AddPropHealth);
	AddFunc("float GetPropHealth(string &in asName)",(void *)GetPropHealth);
	AddFunc("void ResetProp(string &in asName)",(void *)ResetProp);
	AddFunc("void PlayPropAnimation(string &in asProp, string &in asAnimation, float afFadeTime, bool abLoop, string &in asCallback)",(void *)PlayPropAnimation);
	AddFunc("void StopPropAnimation(string &in asProp)", (void*)StopPropAnimation);
	AddFunc("void PlayCurrentAnimation(string &in asProp, float afFadeTime, bool abLoop)", (void *)PlayCurrentAnimation);
	AddFunc("void PauseCurrentAnimation(string &in asProp, float afFadeTime)", (void *)PauseCurrentAnimation);
	AddFunc("void SetPropAnimationSpeed(string &in asProp, float afSpeed)", (void *)SetPropAnimationSpeed);
	AddFunc("void SetPropAnimationPosition(string &in asProp, float afPos)", (void *)SetPropAnimationPosition);
	
	AddFunc("void SetMoveObjectState(string &in asName, float afState)",(void *)SetMoveObjectState);
	AddFunc("void SetMoveObjectStateExt(string &in asName, float afState, float afAcc, float afMaxSpeed, float afSlowdownDist, bool abResetSpeed)",(void *)SetMoveObjectStateExt);


	AddFunc("void AddPropForce(string &in asName, float afX, float afY, float afZ, string &in asCoordSystem)",(void *)AddPropForce);
	AddFunc("void AddPropImpulse(string &in asName, float afX, float afY, float afZ, string &in asCoordSystem)",(void *)AddPropImpulse);
	AddFunc("void AddBodyForce(string &in asName, float afX, float afY, float afZ, string &in asCoordSystem)",(void *)AddBodyForce);
	AddFunc("void AddBodyImpulse(string &in asName, float afX, float afY, float afZ, string &in asCoordSystem)",(void *)AddBodyImpulse);
	AddFunc("void BreakJoint(string &in asName)", (void *)BreakJoint);
	AddFunc("void SetBodyMass(string &in asName, float afMass)", (void *)SetBodyMass);
	AddFunc("float GetBodyMass(string &in asName)", (void *)GetBodyMass);


	AddFunc("void AddEntityCollideCallback(string &in asParentName, string &in asChildName, string &in asFunction, bool abDeleteOnCollide, int alStates)",(void *)AddEntityCollideCallback);
	AddFunc("void RemoveEntityCollideCallback(string &in asParentName, string &in asChildName)", (void *)RemoveEntityCollideCallback);

	//AddFunc("void CreateRope(string &in asName,string &in asStartArea, string &in asEndArea, string &in asStartBody, string &in asEndBody,float afMinTotalLength, float afMaxTotalLength,float afSegmentLength, float afDamping,float afStrength, float afStiffness, string &in asMaterial, float afRadius, float afLengthTileAmount, float afLengthTileSize, string &in asSound,float afSoundStartSpeed, float afSoundStopSpeed,bool abAutoMove, float afAutoMoveAcc, float afAutoMoveMaxSpeed)",(void *)CreateRope);

	AddFunc("void InteractConnectPropWithRope(string &in asName, string& asLeverName, string& asPropName, bool abInteractOnly, float afSpeedMul,float afMinSpeed, float afMaxSpeed, bool abInvert, int alStatesUsed)",(void *)InteractConnectPropWithRope);
	AddFunc("void InteractConnectPropWithMoveObject(string &in asName, string &in asPropName, string &in asMoveObjectName, bool abInteractOnly,bool abInvert, int alStatesUsed)",(void *)InteractConnectPropWithMoveObject);
	AddFunc("void ConnectEntities(string &in asName, string &in asMainEntity, string &in asConnectEntity, bool abInvertStateSent, int alStatesUsed, string &in asCallbackFunc)",(void *)ConnectEntities); 

	AddFunc("float MathSin(float afX)",(void *)ScriptSin);
	AddFunc("float MathCos(float afX)",(void *)ScriptCos);
	AddFunc("float MathTan(float afX)",(void *)ScriptTan);
	AddFunc("float MathAsin(float afX)",(void *)ScriptAsin);
	AddFunc("float MathAcos(float afX)",(void *)ScriptAcos);
	AddFunc("float MathAtan(float afX)",(void *)ScriptAtan);
	AddFunc("float MathAtan2(float afX, float afY)",(void *)ScriptAtan2);
	AddFunc("float MathSqrt(float afX)",(void *)ScriptSqrt);
	AddFunc("float MathPow(float afBase, float afExp)",(void *)ScriptPow);
	AddFunc("float MathMin(float afA, float afB)",(void *)ScriptMin);
	AddFunc("float MathMax(float afA, float afB)",(void *)ScriptMax);
	AddFunc("float MathClamp(float afX, float afMin, float afMax)",(void *)ScriptClamp);
	AddFunc("float MathAbs(float afX)",(void *)ScriptAbs);

	AddFunc("int StringToInt(string&in asString)",(void *)ScriptStringToInt);
	AddFunc("float StringToFloat(string&in asString)",(void *)ScriptStringToFloat);
	AddFunc("bool StringToBool(string&in asString)",(void *)ScriptStringToBool);

}
//-----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// SCRIPT FUNCTION ENTITY PROPERTIES
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::Print(string& asString)
{
	Log("%s", asString.c_str());
}

void __stdcall cLuxScriptHandler::AddDebugMessage(string& asString, bool abCheckForDuplicates)
{
	gpBase->mpDebugHandler->AddMessage(cString::To16Char(asString),abCheckForDuplicates);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::ProgLog(string& asLevel, string& asMessage)
{
	tString sLowType = cString::ToLowerCase(asLevel);
	eLuxProgressLogLevel level = eLuxProgressLogLevel_Low;
	if(sLowType == "medium") level = eLuxProgressLogLevel_Medium;
	if(sLowType == "high") level = eLuxProgressLogLevel_High;

    gpBase->mpProgressLogHandler->AddLog(level, asMessage);	
}

void __stdcall cLuxScriptHandler::SetProgLogTimerActive(bool abTimerActive)
{
	gpBase->mpProgressLogHandler->SetProgLogCounterActive(abTimerActive);
}

void __stdcall cLuxScriptHandler::ResetProgLogTimer()
{
	gpBase->mpProgressLogHandler->ResetProgLogCounter();
}

void __stdcall cLuxScriptHandler::DisplayCounterTimer(bool abDisplayTimer)
{
	gpBase->mpProgressLogHandler->DisplayCounterHUD(abDisplayTimer);
}

void __stdcall cLuxScriptHandler::DisplayCounterSettings(float afX, float afY, float afZ, float afR, float afG, float afB, float afA, float afTimerSize, string& asTimerAlign)
{
	cLuxProgressLogHandler *pProgLog = gpBase->mpProgressLogHandler;

	cVector3f newPos(afX,afY,afZ);
	cColor newTextColor(afR,afG,afB,afA);
	cVector2f newSize(afTimerSize);
	
	eFontAlign newAlign = eFontAlign_Center;
	tString sAlignType = cString::ToLowerCase(asTimerAlign);
	if(sAlignType == "left") newAlign = eFontAlign_Left;
	if(sAlignType == "right") newAlign = eFontAlign_Right;

	pProgLog->SetCounterHUDPosition(newPos);
	pProgLog->SetCounterHUDColor(newTextColor);
	pProgLog->SetCounterHUDSize(newSize);
	pProgLog->SetCounterHUDAlign(newAlign);
}
//-----------------------------------------------------------------------
int __stdcall cLuxScriptHandler::GetCurrentInGameHour()
{
	return gpBase->mpProgressLogHandler->GetCurrentInGameHour();
}

int __stdcall cLuxScriptHandler::GetCurrentInGameMin()
{
	return gpBase->mpProgressLogHandler->GetCurrentInGameMinute();
}

int __stdcall cLuxScriptHandler::GetCurrentInGameSec()
{
	return gpBase->mpProgressLogHandler->GetCurrentInGameSecond();
}
//-----------------------------------------------------------------------

bool __stdcall cLuxScriptHandler::ScriptDebugOn()
{
	return gpBase->mpDebugHandler->GetScriptDebugOn();
}

//-----------------------------------------------------------------------

float __stdcall cLuxScriptHandler::RandFloat(float afMin, float afMax)
{
	return cMath::RandRectf(afMin, afMax);
}

int __stdcall cLuxScriptHandler::RandInt(int alMin, int alMax)
{
	return cMath::RandRectl(alMin, alMax);
}

//-----------------------------------------------------------------------

bool __stdcall cLuxScriptHandler::StringContains(string& asString, string& asSubString)
{
	return cString::GetFirstStringPos(asString, asSubString)>=0;
}

//-----------------------------------------------------------------------

string gsGlobalTemp="";

string& __stdcall cLuxScriptHandler::StringSub(string& asString, int alStart, int alCount)
{
	gsGlobalTemp = cString::Sub(asString, alStart, alCount);
	return gsGlobalTemp;
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::AddTimer(string& asName, float afTime, string& asFunction)
{
	cLuxMap *pMap = gpBase->mpMapHandler->GetCurrentMap();
	if(pMap==NULL) return;

	pMap->AddTimer(asName,afTime, asFunction);
}
//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::RemoveTimer(string& asName)
{
	cLuxMap *pMap = gpBase->mpMapHandler->GetCurrentMap();
	if(pMap==NULL) return;

	pMap->RemoveTimer(asName);
}

//-----------------------------------------------------------------------

float __stdcall cLuxScriptHandler::GetTimerTimeLeft(string& asName)
{
	cLuxMap *pMap = gpBase->mpMapHandler->GetCurrentMap();
	if(pMap==NULL) return 0;

	cLuxEventTimer *pTimer = pMap->GetTimer(asName);
	if(pTimer)
		return pTimer->mfCount;
	else
		return 0;
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetLocalVarInt(string& asName, int alVal)
{
	SetLocalVarString(asName, cString::ToString(alVal));
}

void __stdcall cLuxScriptHandler::SetLocalVarFloat(string& asName, float afVal)
{
	SetLocalVarString(asName, cString::ToString(afVal));
}

void __stdcall cLuxScriptHandler::SetLocalVarBool(string& asName, bool abVal)
{
	SetLocalVarString(asName, cString::ToString(abVal));
}

void __stdcall cLuxScriptHandler::SetLocalVarString(string& asName, const string& asVal)
{
	cLuxMap *pMap = gpBase->mpMapHandler->GetCurrentMap();
	if(pMap==NULL) return;

	cLuxScriptVar* pVar = pMap->GetVar(asName);
	if(pVar==NULL)
	{
		Error("Couldn't find local var '%s'\n",asName.c_str());
		return;
	}
	pVar->msVal = asVal;
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::AddLocalVarInt(string& asName, int alVal)
{
	cLuxMap *pMap = gpBase->mpMapHandler->GetCurrentMap();
	if(pMap==NULL) return;

	cLuxScriptVar* pVar = pMap->GetVar(asName);
	if(pVar==NULL) {
		Error("Couldn't find local var '%s'\n",asName.c_str());
		return;
	}

	pVar->msVal = cString::ToString(cString::ToInt(pVar->msVal.c_str(),0)+alVal);
}

void __stdcall cLuxScriptHandler::AddLocalVarFloat(string& asName, float afVal)
{
	cLuxMap *pMap = gpBase->mpMapHandler->GetCurrentMap();
	if(pMap==NULL) return;

	cLuxScriptVar* pVar = pMap->GetVar(asName);
	if(pVar==NULL) {
		Error("Couldn't find local var '%s'\n",asName.c_str());
		return;
	}

	pVar->msVal = cString::ToString(cString::ToFloat(pVar->msVal.c_str(),0)+afVal);
}

void __stdcall cLuxScriptHandler::AddLocalVarString(string& asName, string& asVal)
{
	cLuxMap *pMap = gpBase->mpMapHandler->GetCurrentMap();
	if(pMap==NULL) return;

	cLuxScriptVar* pVar = pMap->GetVar(asName);
	if(pVar==NULL)
	{
		Error("Couldn't find local var '%s'\n",asName.c_str());
		return;
	}
	pVar->msVal += asVal;
}

//-----------------------------------------------------------------------

int __stdcall cLuxScriptHandler::GetLocalVarInt(string& asName)
{
	string sVal = GetLocalVarString(asName);
	if(sVal=="") return 0;

	return cString::ToInt(sVal.c_str(),0);
}

float __stdcall cLuxScriptHandler::GetLocalVarFloat(string& asName)
{
	string sVal = GetLocalVarString(asName);
	if(sVal=="") return 0;

	return cString::ToFloat(sVal.c_str(),0);
}

bool __stdcall cLuxScriptHandler::GetLocalVarBool(string& asName)
{
	string sVal = GetLocalVarString(asName);
	if (sVal == "") return false;

	return cString::ToBool(sVal.c_str(), 0);
}

string gsGetLocalVarNullString = "";
string& __stdcall cLuxScriptHandler::GetLocalVarString(string& asName)
{
	cLuxMap *pMap = gpBase->mpMapHandler->GetCurrentMap();
	if(pMap==NULL) return gsGetLocalVarNullString;

	cLuxScriptVar* pVar = pMap->GetVar(asName);
	if(pVar==NULL)
	{
		Error("Couldn't find local var '%s'\n",asName.c_str());
		return gsScriptNull;
	}
	return pVar->msVal;
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetGlobalVarInt(string& asName, int alVal)
{
	SetGlobalVarString(asName, cString::ToString(alVal));
}

void __stdcall cLuxScriptHandler::SetGlobalVarFloat(string& asName, float afVal)
{
	string t =  cString::ToString(afVal);
	SetGlobalVarString(asName, t);
}

void __stdcall cLuxScriptHandler::SetGlobalVarBool(string& asName, bool abVal)
{
	SetGlobalVarString(asName, cString::ToString(abVal));
}

void __stdcall cLuxScriptHandler::SetGlobalVarString(string& asName, const string& asVal)
{
	cLuxScriptVar* pVar = gpBase->mpGlobalDataHandler->GetVar(asName);
	if(pVar==NULL)
	{
		Error("Couldn't find Global var '%s'\n",asName.c_str());
		return;
	}
	pVar->msVal = asVal;
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::AddGlobalVarInt(string& asName, int alVal)
{
	cLuxScriptVar* pVar = gpBase->mpGlobalDataHandler->GetVar(asName);
	if(pVar==NULL) {
		Error("Couldn't find Global var '%s'\n",asName.c_str());
		return;
	}

	pVar->msVal = cString::ToString(cString::ToInt(pVar->msVal.c_str(),0)+alVal);
}

void __stdcall cLuxScriptHandler::AddGlobalVarFloat(string& asName, float afVal)
{
	cLuxScriptVar* pVar = gpBase->mpGlobalDataHandler->GetVar(asName);
	if(pVar==NULL) {
		Error("Couldn't find Global var '%s'\n",asName.c_str());
		return;
	}

	pVar->msVal = cString::ToString(cString::ToFloat(pVar->msVal.c_str(),0)+afVal);
}

void __stdcall cLuxScriptHandler::AddGlobalVarString(string& asName, string& asVal)
{
	cLuxScriptVar* pVar = gpBase->mpGlobalDataHandler->GetVar(asName);
	if(pVar==NULL)
	{
		Error("Couldn't find global var '%s'\n",asName.c_str());
		return;
	}
	pVar->msVal += asVal;
}

//-----------------------------------------------------------------------

int __stdcall cLuxScriptHandler::GetGlobalVarInt(string& asName)
{
	string sVal = GetGlobalVarString(asName);
	if(sVal=="") return 0;

	return cString::ToInt(sVal.c_str(),0);
}

float __stdcall cLuxScriptHandler::GetGlobalVarFloat(string& asName)
{
	string sVal = GetGlobalVarString(asName);
	if(sVal=="") return 0;

	return cString::ToFloat(sVal.c_str(),0);
}

bool __stdcall cLuxScriptHandler::GetGlobalVarBool(string& asName)
{
	string sVal = GetGlobalVarString(asName);
	if (sVal == "") return false;

	return cString::ToBool(sVal.c_str(), 0);
}

string& __stdcall cLuxScriptHandler::GetGlobalVarString(string& asName)
{
	cLuxScriptVar* pVar = gpBase->mpGlobalDataHandler->GetVar(asName);
	if(pVar==NULL)
	{
		Error("Couldn't find global var '%s'\n",asName.c_str());
		return gsScriptNull;
	}
	return pVar->msVal;
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::StartCredits(string& asMusic, bool abLoopMusic, string& asTextCat, string& asTextEntry, int alEndNum)
{
	gpBase->mpCredits->Setup(asMusic, abLoopMusic, asTextCat, asTextEntry, alEndNum);
	gpBase->mpEngine->GetUpdater()->SetContainer("Credits");
}

void __stdcall cLuxScriptHandler::StartCreditsAndRankScreen(string& asMusicCredits, bool abLoopMusicCredits, string& asTextCat, string& asTextEntry, bool abCreditsBackground, string& asMusicRank, bool abLoopMusicRank, float afRankMusicVol, float afRankMusicFade, string& asImageName)
{
	gpBase->mpCredits->SetupWithRankScreen(asMusicCredits, abLoopMusicCredits, asTextCat, asTextEntry, true, abCreditsBackground);
	gpBase->mpRankScreen->Setup(asMusicRank, abLoopMusicRank, afRankMusicVol, afRankMusicFade, asImageName);
	gpBase->mpEngine->GetUpdater()->SetContainer("Credits");
}

//-----------------------------------------------------------------------
void __stdcall cLuxScriptHandler::SetEnding(string& asEnding, bool abCompletedOnHardMode)
{
	gpBase->mpEndingsHandler->AddEndingLog(asEnding, abCompletedOnHardMode);
}

string& __stdcall cLuxScriptHandler::GetPreviousEnding()
{
	return gpBase->mpEndingsHandler->GetPreviousEnding();
}

bool __stdcall cLuxScriptHandler::GetEndingCompletion(string& asEndingType)
{
	return gpBase->mpEndingsHandler->GetEndingCompleted(asEndingType);
}

int __stdcall cLuxScriptHandler::GetGameClears()
{
	return gpBase->mpEndingsHandler->GetTotalGameClears();
}

//-----------------------------------------------------------------------
bool __stdcall cLuxScriptHandler::HardModeEnabled()
{
	if (gpBase->mbHardMode == true)
	{
		return true;
	}
	else if (gpBase->mbHardMode == false || gpBase->mpEndingsHandler->mbAllowHardmode == false)
	return false;
}
//-----------------------------------------------------------------------
void __stdcall cLuxScriptHandler::ForceExitDirectlyToMenu()
{
	gpBase->mpEngine->GetUpdater()->BroadcastMessageToAll(eUpdateableMessage_Reset);

	gpBase->mpLoadScreenHandler->DrawMenuScreen();
	gpBase->mpEngine->GetUpdater()->SetContainer("MainMenu");
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::AddKeyPart(int alKeyPart)
{
	gpBase->mpCredits->AddKeyPart(alKeyPart);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::StartDemoEnd()
{
	if(gpBase->mpDemoEnd)
		gpBase->mpEngine->GetUpdater()->SetContainer("DemoEnd");
}

void __stdcall cLuxScriptHandler::StartRankScreen(string& asMusic, bool abLoopMusic, float afRankMusicVol, float afRankMusicFade, string& asImageName)
{
	gpBase->mpRankScreen->Setup(asMusic, abLoopMusic,afRankMusicVol, afRankMusicFade, asImageName);
	gpBase->mpEngine->GetUpdater()->SetContainer("RankScreen");
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::AutoSave()
{
	gpBase->mpSaveHandler->AutoSave(); 
} 

void __stdcall cLuxScriptHandler::DoHardModeSave()
{
	gpBase->mpSaveHandler->HardModeSave();
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::CheckPoint(string& asName,string& asStartPos ,string& asCallback, string &asDeathHintCat, string &asDeathHintEntry)
{
	gpBase->mpMapHandler->GetCurrentMap()->SetCheckPoint(asName, asStartPos, asCallback);
	gpBase->mpPlayer->GetHelperDeath()->SetHint(asDeathHintCat, asDeathHintEntry);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::ChangeMap(string& asMapName, string& asStartPos, string& asStartSound, string& asEndSound)
{
	gpBase->mpMapHandler->ChangeMap(asMapName, asStartPos, asStartSound, asEndSound);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::ClearSavedMaps()
{
	gpBase->mpMapHandler->ClearSaveMapCollection();
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::CreateDataCache()
{
	gpBase->mpMapHandler->CreateDataCache();
}

void __stdcall cLuxScriptHandler::DestroyDataCache()
{
	gpBase->mpMapHandler->DestroyDataCache();
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetMapDisplayNameEntry(string& asNameEntry)
{
	cLuxMap *pMap = gpBase->mpMapHandler->GetCurrentMap();
	if(pMap)	pMap->SetDisplayNameEntry(asNameEntry);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetSkyBoxActive(bool abActive)
{
	cWorld *pWorld = gpBase->mpMapHandler->GetCurrentMap()->GetWorld();

	pWorld->SetSkyBoxActive(abActive);
}

void __stdcall cLuxScriptHandler::SetSkyBoxTexture(string& asTexture)
{
	cWorld *pWorld = gpBase->mpMapHandler->GetCurrentMap()->GetWorld();

    iTexture *pTexture;
	if(asTexture != "")
		pTexture = gpBase->mpEngine->GetResources()->GetTextureManager()->CreateCubeMap(asTexture,true);
	else
		pTexture = NULL;
    
	pWorld->SetSkyBox(pTexture, true);
}

void __stdcall cLuxScriptHandler::SetSkyBoxColor(float afR, float afG, float afB, float afA)
{
	cWorld *pWorld = gpBase->mpMapHandler->GetCurrentMap()->GetWorld();

	pWorld->SetSkyBoxColor(cColor(afR, afG, afB, afA));
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetFogActive(bool abActive)
{
	cWorld *pWorld = gpBase->mpMapHandler->GetCurrentMap()->GetWorld();

	pWorld->SetFogActive(abActive);
}

void __stdcall cLuxScriptHandler::SetFogColor(float afR, float afG, float afB, float afA)
{
	cWorld *pWorld = gpBase->mpMapHandler->GetCurrentMap()->GetWorld();

	pWorld->SetFogColor(cColor(afR, afG, afB, afA));
}

void __stdcall cLuxScriptHandler::SetFogProperties(float afStart, float afEnd, float afFalloffExp, bool abCulling)
{
	cWorld *pWorld = gpBase->mpMapHandler->GetCurrentMap()->GetWorld();

	pWorld->SetFogStart(afStart);
	pWorld->SetFogEnd(afEnd);
	pWorld->SetFogFalloffExp(afFalloffExp);
	pWorld->SetFogCulling(abCulling);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetupLoadScreen(string &asTextCat, string &asTextEntry, int alRandomNum, string &asImageFile)
{
	gpBase->mpLoadScreenHandler->SetupLoadText(asTextCat, asTextEntry, alRandomNum, asImageFile);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::FadeIn(float afTime)
{
	gpBase->mpEffectHandler->GetFade()->FadeIn(afTime);
}

void __stdcall cLuxScriptHandler::FadeOut(float afTime)
{
	gpBase->mpEffectHandler->GetFade()->FadeOut(afTime);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::FadeImageTrailTo(float afAmount, float afSpeed)
{
	gpBase->mpEffectHandler->GetImageTrail()->FadeTo(afAmount, afSpeed);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::FadeSepiaColorTo(float afAmount, float afSpeed)
{
	gpBase->mpEffectHandler->GetSepiaColor()->FadeTo(afAmount, afSpeed);
}

//-----------------------------------------------------------------------

//void __stdcall cLuxScriptHandler::FadeBlackAndWhiteAmountTo(float afAmount, float afSpeed)
//{
//	gpBase->mpEffectHandler->GetBlackAndWhiteAmount()->FadeTo(afAmount, afSpeed);
//}


//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::FadeRadialBlurTo(float afSize, float afSpeed)
{
	gpBase->mpEffectHandler->GetRadialBlur()->FadeTo(afSize, afSpeed);
}

void __stdcall cLuxScriptHandler::SetRadialBlurStartDist(float afStartDist)
{
	gpBase->mpEffectHandler->GetRadialBlur()->SetBlurStartDist(afStartDist);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::StartEffectFlash(float afFadeIn, float afWhite, float afFadeOut)
{
	gpBase->mpEffectHandler->GetFlash()->Start(afFadeOut, afWhite, afFadeOut);
}
//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::StartCustomColorEffectFlash(float afR, float afG, float afB, float afA, float afFadeIn, float afFadeOut)
{
	gpBase->mpPlayer->GetHelperHudEffect()->Flash(cColor(afR, afG, afB, afA), eGuiMaterial_Alpha, afFadeIn, afFadeOut);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::StartEffectEmotionFlash(string &asTextCat, string &asTextEntry, string &asSound)
{
	gpBase->mpEffectHandler->GetEmotionFlash()->Start(asTextCat, asTextEntry, asSound);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::ShowScreenImage(string& asImageName, float afX, float afY, float afScale, bool abUseRelativeCoordinates, float afDuration, float afFadeIn, float afFadeOut)
{
	gpBase->mpEffectHandler->GetScreenImage()->ShowImage(asImageName, afX, afY, afScale, abUseRelativeCoordinates, afDuration, afFadeIn, afFadeOut);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::HideScreenImageImmediately()
{
	gpBase->mpEffectHandler->GetScreenImage()->HideImmediately();
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::HideScreenImageWithFade(float afFadeOut)
{
	gpBase->mpEffectHandler->GetScreenImage()->HideWithFade(afFadeOut);
}


//-----------------------------------------------------------------------


void __stdcall cLuxScriptHandler::SetInDarknessEffectsActive(bool abX)
{
	gpBase->mpPlayer->GetHelperInDarkness()->SetActive(abX);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::AddEffectVoice(string& asVoiceFile, string& asEffectFile,
												string& asTextCat, string& asTextEntry, bool abUsePostion, 
												string& asPosEntity, float afMinDistance, float afMaxDistance)
{
	cVector3f vPos(0);
	if(abUsePostion)
	{
		iLuxEntity *pEntity = GetEntity(asPosEntity,eLuxEntityType_LastEnum,-1);
		if(pEntity && pEntity->GetBodyNum()>0)
		{
			vPos = pEntity->GetBody(0)->GetLocalPosition();
		}
	}
    
	gpBase->mpEffectHandler->GetPlayVoice()->AddVoice(asVoiceFile, asEffectFile, asTextCat, asTextEntry, abUsePostion, vPos, afMinDistance, afMaxDistance);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::StopAllEffectVoices(float afFadeOutTime)
{
	gpBase->mpEffectHandler->GetPlayVoice()->StopVoices(1.0f/afFadeOutTime);
}

//-----------------------------------------------------------------------

bool __stdcall cLuxScriptHandler::GetEffectVoiceActive()
{
	return gpBase->mpEffectHandler->GetPlayVoice()->IsActive();
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::StartPlayerSpawnPS(string& asSPSFile)
{
	gpBase->mpPlayer->GetHelperSpawnPS()->Start(asSPSFile);
}

void __stdcall cLuxScriptHandler::StopPlayerSpawnPS()
{
	gpBase->mpPlayer->GetHelperSpawnPS()->Stop();
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetEffectVoiceOverCallback(string& asFunc)
{
	gpBase->mpEffectHandler->GetPlayVoice()->SetOverCallback(asFunc);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::StartScreenShake(float afAmount, float afTime, float afFadeInTime,float afFadeOutTime)
{
	gpBase->mpEffectHandler->GetScreenShake()->Start(afAmount, afTime, afFadeInTime, afFadeOutTime);
}

//-----------------------------------------------------------------------

bool __stdcall cLuxScriptHandler::GetFlashbackIsActive()
{
	return gpBase->mpPlayer->GetHelperFlashback()->IsActive();
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetInsanitySetEnabled(string& asSet, bool abX)
{
	if(abX)	gpBase->mpInsanityHandler->EnableSet(asSet);
	else	gpBase->mpInsanityHandler->DisableSet(asSet);

}

void __stdcall cLuxScriptHandler::StartRandomInsanityEvent()
{
	gpBase->mpInsanityHandler->StartEvent();
}

void __stdcall cLuxScriptHandler::StartInsanityEvent(string& asEventName)
{
	gpBase->mpInsanityHandler->StartEvent(asEventName);
}

void __stdcall cLuxScriptHandler::StopCurrentInsanityEvent()
{
	gpBase->mpInsanityHandler->StopCurrentEvent();
}

bool __stdcall cLuxScriptHandler::InsanityEventIsActive()
{
	return gpBase->mpInsanityHandler->GetCurrentEvent() >= 0;
}

#pragma optimize("", off)
void __stdcall cLuxScriptHandler::UnlockAchievement(string& asName)
{
	bool bUnlockHardmode = false;

	if (asName == "Benefactor")
	{
		gpBase->mpAchievementHandler->UnlockAchievement(eLuxAchievement_Benefactor);
		bUnlockHardmode = true;
	}
	else if (asName == "Survivor")
	{
		gpBase->mpAchievementHandler->UnlockAchievement(eLuxAchievement_Survivor);
		bUnlockHardmode = true;
	}
	else if (asName == "Sacrifice")
	{
		gpBase->mpAchievementHandler->UnlockAchievement(eLuxAchievement_Sacrifice);
		bUnlockHardmode = true;
	}
	else if (asName == "Quitter")
	{
		gpBase->mpAchievementHandler->UnlockAchievement(eLuxAchievement_Quitter);
	}
	else if (asName == "Egotist")
	{
		gpBase->mpAchievementHandler->UnlockAchievement(eLuxAchievement_Egotist);
	}
	else if (asName == "Altruist")
	{
		gpBase->mpAchievementHandler->UnlockAchievement(eLuxAchievement_Altruist);
	}
	else if (asName == "Vacillator")
	{
		gpBase->mpAchievementHandler->UnlockAchievement(eLuxAchievement_Vacillator);
	}

	if (gpBase->mbHardMode)
	{
		if (bUnlockHardmode)
		{
			gpBase->mpAchievementHandler->UnlockAchievement(eLuxAchievement_Masochist);
		}
	}
}
#pragma optimize("",on)

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::PlayGuiSound(string& asSoundEntFile, float afVolume)
{
	tString sExt = cString::GetFileExt(asSoundEntFile);

	//Sound entity!
	if(sExt == "" || sExt == "snt")
	{
		gpBase->mpHelpFuncs->PlayGuiSoundData(asSoundEntFile, eSoundEntryType_Gui, afVolume);
	}
	//Normal sound
	else
	{
		cSoundHandler *pSoundHandler = gpBase->mpEngine->GetSound()->GetSoundHandler();
		pSoundHandler->PlayGui(asSoundEntFile,false, afVolume);
		
	}
	
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetPlayerActive(bool abActive)
{
	gpBase->mpPlayer->SetActive(abActive);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::ChangePlayerStateToNormal()
{
	gpBase->mpPlayer->ChangeState(eLuxPlayerState_Normal);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetPlayerCrouching(bool abCrouch)
{
	gpBase->mpPlayer->ChangeMoveState(eLuxMoveState_Normal);

	cLuxMoveState_Normal *pState = static_cast<cLuxMoveState_Normal*>(gpBase->mpPlayer->GetMoveStateData(eLuxMoveState_Normal));
	pState->SetCrouch(abCrouch);
}
//-----------------------------------------------------------------------
bool __stdcall cLuxScriptHandler::GetPlayerCrouching()
{
	cLuxMoveState_Normal* pMoveNormal = static_cast<cLuxMoveState_Normal*>(gpBase->mpPlayer->GetCurrentMoveStateData());

	return pMoveNormal->IsCrouching();
}
//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::AddPlayerBodyForce(float afX, float afY, float afZ, bool abUseLocalCoords)
{
	iCharacterBody *pBody = gpBase->mpPlayer->GetCharacterBody();

	cVector3f vForce;
	if(abUseLocalCoords)
	{
		vForce = pBody->GetForward()*afZ + pBody->GetRight()*afY + pBody->GetUp()*afY;
	}
	else
	{
		vForce = cVector3f(afX, afY, afZ);
	}

	pBody->AddForce(vForce);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::ShowPlayerCrossHairIcons(bool abX)
{
	gpBase->mpPlayer->SetScriptShowFocusIconAndCrossHair(abX);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetPlayerPos(float afX, float afY, float afZ)
{
	return gpBase->mpPlayer->GetCharacterBody()->SetFeetPosition(cVector3f(afX, afY, afZ));
}

//-----------------------------------------------------------------------

float __stdcall cLuxScriptHandler::GetPlayerPosX()
{
	return gpBase->mpPlayer->GetCharacterBody()->GetFeetPosition().x;
}

//-----------------------------------------------------------------------

float __stdcall cLuxScriptHandler::GetPlayerPosY()
{
	return gpBase->mpPlayer->GetCharacterBody()->GetFeetPosition().y;
}

//-----------------------------------------------------------------------

float __stdcall cLuxScriptHandler::GetPlayerPosZ()
{
	return gpBase->mpPlayer->GetCharacterBody()->GetFeetPosition().z;
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetPlayerSanity(float afSanity)
{
	gpBase->mpPlayer->SetSanity(afSanity);
}

void __stdcall cLuxScriptHandler::AddPlayerSanity(float afSanity)
{
	gpBase->mpPlayer->AddSanity(afSanity);
}

float __stdcall cLuxScriptHandler::GetPlayerSanity()
{
	return gpBase->mpPlayer->GetSanity();
}

void __stdcall cLuxScriptHandler::SetPlayerHealth(float afHealth)
{
	gpBase->mpPlayer->SetHealth(afHealth);
}

void __stdcall cLuxScriptHandler::AddPlayerHealth(float afHealth)
{
	gpBase->mpPlayer->AddHealth(afHealth);
}

float __stdcall cLuxScriptHandler::GetPlayerHealth()
{
	return gpBase->mpPlayer->GetHealth();
}

	
int __stdcall cLuxScriptHandler::GetPlayerHealthLevel()
{
	return gpBase->mpPlayer->GetHealthLevel();
}

void __stdcall cLuxScriptHandler::SetPlayerLampOil(float afOil)
{
	gpBase->mpPlayer->SetLampOil(afOil);
}

void __stdcall cLuxScriptHandler::AddPlayerLampOil(float afOil)
{
	gpBase->mpPlayer->AddLampOil(afOil);
}

float __stdcall cLuxScriptHandler::GetPlayerLampOil()
{
	return gpBase->mpPlayer->GetLampOil();
}

//-----------------------------------------------------------------------

float __stdcall cLuxScriptHandler::GetPlayerSpeed()
{
	return gpBase->mpPlayer->GetCharacterBody()->GetVelocity(1.0f/60.0f).Length();
}

//-----------------------------------------------------------------------

float __stdcall cLuxScriptHandler::GetPlayerYSpeed()
{
	return gpBase->mpPlayer->GetCharacterBody()->GetVelocity(1.0f/60.0f).y;
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::MovePlayerForward(float afAmount)
{
	gpBase->mpPlayer->GetCharacterBody()->Move(eCharDir_Forward, afAmount);
}

void __stdcall cLuxScriptHandler::SetPlayerPermaDeathSound(string& asSound)
{
	gpBase->mpPlayer->SetCurrentPermaDeathSound(asSound);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetSanityDrainDisabled(bool abX)
{
	gpBase->mpPlayer->SetSanityDrainDisabled(abX);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::GiveSanityBoost()
{
	if(gpBase->mpPlayer->GetSanity() < 25.0f)
		gpBase->mpPlayer->AddSanity(100.0f - gpBase->mpPlayer->GetSanity());
	else if(gpBase->mpPlayer->GetSanity() < 50.0f)
		gpBase->mpPlayer->AddSanity(90.0f - gpBase->mpPlayer->GetSanity());
	else if(gpBase->mpPlayer->GetSanity() < 75.0f)
		gpBase->mpPlayer->AddSanity(80.0f - gpBase->mpPlayer->GetSanity());
	else
		gpBase->mpPlayer->AddSanity(5.0f);

}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::GiveSanityBoostSmall()
{
	cLuxPlayer* pPlayer = gpBase->mpPlayer;

	if(pPlayer->GetSanity() < 25.0f)
		pPlayer->AddSanity(20.0f);
	else if(pPlayer->GetSanity() < 50.0f)
		pPlayer->AddSanity(15.0f);
	else if(pPlayer->GetSanity() < 75.0f)
		pPlayer->AddSanity(10.0f);
	else
		pPlayer->AddSanity(5.0f);
}


//-----------------------------------------------------------------------

 void __stdcall cLuxScriptHandler::GiveSanityDamage(float afAmount, bool abUseEffect)
{
	if(abUseEffect)
		gpBase->mpPlayer->GiveSanityDamage(afAmount);
	else
		gpBase->mpPlayer->LowerSanity(afAmount, false);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::GivePlayerDamage(float afAmount, string& asType, bool abSpinHead, bool abLethal)
{
	tString sLowType = cString::ToLowerCase(asType);
	eLuxDamageType type = eLuxDamageType_BloodSplat;
	if(sLowType == "claws") type = eLuxDamageType_Claws;
	if(sLowType == "slash") type = eLuxDamageType_Slash;
	if (sLowType == "blank") type = eLuxDamageType_Blank;
	
	gpBase->mpPlayer->GiveDamage(afAmount, 1, type, abSpinHead,abLethal);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::FadePlayerFOVMulTo(float afX, float afSpeed)
{
	gpBase->mpPlayer->FadeFOVMulTo(afX, afSpeed);
}

void __stdcall cLuxScriptHandler::FadePlayerAspectMulTo(float afX, float afSpeed)
{
	gpBase->mpPlayer->FadeAspectMulTo(afX, afSpeed);
}

void __stdcall cLuxScriptHandler::FadePlayerPitchTo(float afX, float afSpeedMul, float afMaxSpeed)
{
	gpBase->mpPlayer->FadePitchTo(cMath::ToRad(afX), afSpeedMul, cMath::ToRad(afMaxSpeed));
}

void __stdcall cLuxScriptHandler::FadePlayerRollTo(float afX, float afSpeedMul, float afMaxSpeed)
{
	gpBase->mpPlayer->FadeRollTo(cMath::ToRad(afX), afSpeedMul, cMath::ToRad(afMaxSpeed));
}

void __stdcall cLuxScriptHandler::MovePlayerHeadPos(float afX, float afY, float afZ, float afSpeed, float afSlowDownDist)
{
	gpBase->mpPlayer->MoveHeadPosAdd(eLuxHeadPosAdd_Script, cVector3f(afX, afY, afZ), afSpeed, afSlowDownDist);
}


//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::StartPlayerLookAt(string& asEntityName, float afSpeedMul, float afMaxSpeed, string & asAtTargetCallback)
{
	iLuxEntity *pEntity = GetEntity(asEntityName, eLuxEntityType_LastEnum, -1);
	if(pEntity==NULL) return;

	cVector3f vPos =0;
	if(pEntity->GetBodyNum()>0)
		vPos = pEntity->GetBody(0)->GetLocalPosition();
	else if(pEntity->GetMeshEntity())
		vPos = pEntity->GetMeshEntity()->GetBoundingVolume()->GetWorldCenter();
	else
	{
		Error("Entity %s does not have body or mesh entity! Cannot make player look at it!", asEntityName.c_str());
		return;
	}

	gpBase->mpPlayer->GetHelperLookAt()->SetTarget(vPos, afSpeedMul, afMaxSpeed, asAtTargetCallback);
	gpBase->mpPlayer->GetHelperLookAt()->SetActive(true);
}

void __stdcall cLuxScriptHandler::StopPlayerLookAt()
{
	gpBase->mpPlayer->GetHelperLookAt()->SetActive(false);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetPlayerMoveSpeedMul(float afMul)
{
	gpBase->mpPlayer->SetScriptMoveSpeedMul(afMul);
}

void __stdcall cLuxScriptHandler::SetPlayerRunSpeedMul(float afMul)
{
	gpBase->mpPlayer->SetScriptRunSpeedMul(afMul);
}

void __stdcall cLuxScriptHandler::SetPlayerLookSpeedMul(float afMul)
{
	gpBase->mpPlayer->SetLookSpeedMul(afMul);
}

void __stdcall cLuxScriptHandler::SetPlayerJumpForceMul(float afMul)
{
	gpBase->mpPlayer->SetScriptJumpForceMul(afMul);
}

void __stdcall cLuxScriptHandler::SetPlayerJumpDisabled(bool abX)
{
	gpBase->mpPlayer->SetJumpDisabled(abX);
}

void __stdcall cLuxScriptHandler::SetPlayerCrouchDisabled(bool abX)
{
	gpBase->mpPlayer->SetCrouchDisabled(abX);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetPlayerFallDamageDisabled(bool abX)
{
	gpBase->mpPlayer->SetNoFallDamage(abX);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::TeleportPlayer(string &asStartPosName)
{
	cLuxNode_PlayerStart *pNode = gpBase->mpMapHandler->GetCurrentMap()->GetPlayerStart(asStartPosName);
	if(pNode==NULL)
	{
		Error("Start pos '%s' could not be found!\n", asStartPosName.c_str());
		return;
	}

    gpBase->mpPlayer->PlaceAtStartNode(pNode);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetLanternActive(bool abX, bool abUseEffects)
{
	gpBase->mpPlayer->GetHelperLantern()->SetActive(abX, abUseEffects);
}

//-----------------------------------------------------------------------

bool __stdcall cLuxScriptHandler::GetLanternActive()
{
	return gpBase->mpPlayer->GetHelperLantern()->IsActive();
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetLanternDisabled(bool abX)
{
	gpBase->mpPlayer->GetHelperLantern()->SetDisabled(abX);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetLanternLitCallback(string &asCallback)
{
	cLuxMap *pMap = gpBase->mpMapHandler->GetCurrentMap();

	pMap->SetLanternLitCallback(asCallback);
}
//-----------------------------------------------------------------------
void __stdcall cLuxScriptHandler::SetLanternFlickerActive(bool abX)
{
	iLuxHandObject* object = gpBase->mpPlayer->GetHands()->GetHandObject("lantern" + cString::ToString(gpBase->mpPlayer->GetHelperLantern()->GetLantern()));

	if (object)
	{
		cLuxHandObject_LightSource* lantern = (cLuxHandObject_LightSource*)object;

		lantern->SetFlickering(abX);
	}
}
bool  __stdcall cLuxScriptHandler::GetLanternFlickerActive()
{
	iLuxHandObject* object = gpBase->mpPlayer->GetHands()->GetHandObject("lantern" + cString::ToString(gpBase->mpPlayer->GetHelperLantern()->GetLantern()));
	cLuxHandObject_LightSource* lantern = (cLuxHandObject_LightSource*)object;
	
	return lantern->GetFlickering();
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetMessage(string &asTextCategory, string &asTextEntry, float afTime)
{
	gpBase->mpMessageHandler->SetMessage(kTranslate(asTextCategory, asTextEntry), afTime);
}

void __stdcall cLuxScriptHandler::SetCustomTextMessage(string& asTextCategory, string& asTextEntry, float afX, float afY, float afZ, float afR, float afG, float afB, float afTime)
{
	gpBase->mpMessageHandler->SetCustomMessageActive(true);
	gpBase->mpMessageHandler->SetCustomMessagePos(cVector3f(afX,afY,afZ));
	gpBase->mpMessageHandler->SetCustomMessageColor(cColor(afR, afG, afB));
	gpBase->mpMessageHandler->SetMessage(kTranslate(asTextCategory, asTextEntry), afTime);
}
//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::StartCustomPauseMessage(string &asMainTextCategory, string &asMainTextEntry, bool abMultipleButtons, string &asButton1TextCat, string &asButton1TextEnt, string &asButton2TextCat, string &asButton2TextEnt, string& asCallback)
{
	tWString sMainText = kTranslate(asMainTextCategory, asMainTextEntry);
	tWString sButton1Text = kTranslate(asButton1TextCat, asButton1TextEnt);
	tWString sButton2Text = kTranslate(asButton2TextCat, asButton2TextEnt);

	gpBase->mpPauseMessageButtonPrompt->StartPauseMessage(sMainText, abMultipleButtons, sButton1Text, sButton2Text, asCallback);
}


//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetDeathHint(string &asTextCategory, string &asTextEntry)
{
	gpBase->mpPlayer->GetHelperDeath()->SetHint(asTextCategory, asTextEntry);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::DisableDeathStartSound()
{
	gpBase->mpPlayer->GetHelperDeath()->DisableStartSound();
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetLantern(int alLantern)
{
	gpBase->mpPlayer->GetHelperLantern()->SetLantern(alLantern);
}

//-----------------------------------------------------------------------

int __stdcall cLuxScriptHandler::GetLantern()
{
	return gpBase->mpPlayer->GetHelperLantern()->GetLantern();
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::AddNote(string& asNameAndTextEntry, string& asImage)
{
	gpBase->mpJournal->AddNote(asNameAndTextEntry, asImage);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::AddDiary(string& asNameAndTextEntry, string& asImage)
{
	int lReturnNum=0;
	gpBase->mpJournal->AddDiary(asNameAndTextEntry, asImage,lReturnNum);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::ReturnOpenJournal(bool abOpenJournal)
{
	cLuxItemType_Diary::mbShowJournalOnPickup = abOpenJournal;
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::AddQuest(string& asName, string& asNameAndTextEntry)
{
	tString sTextEntry = "Quest_"+asNameAndTextEntry+"_Text";

	if(gpBase->mpJournal->AddQuestNote(asName, asNameAndTextEntry))
	{
		gpBase->mpProgressLogHandler->AddLog(eLuxProgressLogLevel_High, "Added Memento "+ asName);
		
		gpBase->mpHintHandler->Add("QuestAdded", kTranslate("Hints", "QuestAdded"), 0);
		
		gpBase->mpMessageHandler->StarQuestAddedMessage();
		
		//tWString sMess = kTranslate("Quest", "QuestAdded") + _W("\n") + kTranslate("Journal", sTextEntry);
		//gpBase->mpMessageHandler->SetMessage(sMess, 0);	
		//gpBase->mpHelpFuncs->PlayGuiSoundData("quest_added", eSoundEntryType_Gui);
	}
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::CompleteQuest(string& asName, string& asNameAndTextEntry)
{
	tString sTextEntry = "Quest_"+asNameAndTextEntry+"_Text";

	gpBase->mpJournal->AddQuestNote(asName, asNameAndTextEntry);
	if(gpBase->mpJournal->DisableQuestNote(asName))
	{
		gpBase->mpProgressLogHandler->AddLog(eLuxProgressLogLevel_High, "Completed Quest "+ asName);

		//tWString sMess = kTranslate("Quest", "QuestCompleted") + _W("\n") + kTranslate("Journal", sTextEntry);
		//gpBase->mpMessageHandler->SetMessage(sMess, 0);
		//gpBase->mpHelpFuncs->PlayGuiSoundData("quest_completed", eSoundEntryType_Gui);
		
		cLuxMap *pMap = gpBase->mpMapHandler->GetCurrentMap();
		if(pMap) pMap->AddCompletionAmount(gpBase->mpCompletionCountHandler->mlQuestCompletionValue, 6.0f);

		if (asName == "02Web")
		{
			gpBase->mpAchievementHandler->UnlockAchievement(eLuxAchievement_Alchemist);
		}
		else if (asName == "SewerFlooded")
		{
			gpBase->mpAchievementHandler->UnlockAchievement(eLuxAchievement_Pipeworker);
		}
		else if (asName == "21FindOrb")
		{
			gpBase->mpAchievementHandler->UnlockAchievement(eLuxAchievement_Restorer);
		}
	}
}

//-----------------------------------------------------------------------

bool __stdcall cLuxScriptHandler::QuestIsCompleted(string& asName)
{
	cLuxQuestNote *pQuest = gpBase->mpJournal->GetQuestNote(asName);
	if(pQuest==NULL) return false;

	return pQuest->mbActive==false;
}

bool __stdcall cLuxScriptHandler::QuestIsAdded(string& asName)
{
	cLuxQuestNote *pQuest = gpBase->mpJournal->GetQuestNote(asName);
	if(pQuest==NULL) return false;
	return true;
}

void __stdcall cLuxScriptHandler::SetNumberOfQuestsInMap(int alNumberOfQuests)
{
	cLuxMap *pMap = gpBase->mpMapHandler->GetCurrentMap();
	if(pMap==NULL) return;

	pMap->SetNumberOfQuests(alNumberOfQuests);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::GiveHint(string& asName, string& asMessageCat, string& asMessageEntry, float afTimeShown)
{
	gpBase->mpHintHandler->Add(asName, kTranslate(asMessageCat, asMessageEntry), afTimeShown);
}

void __stdcall cLuxScriptHandler::RemoveHint(string &asName)
{
	gpBase->mpHintHandler->Remove(asName);
}

void __stdcall cLuxScriptHandler::BlockHint(string& asName)
{
	gpBase->mpHintHandler->Block(asName);
}

void __stdcall cLuxScriptHandler::UnBlockHint(string& asName)
{
	gpBase->mpHintHandler->UnBlock(asName);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::ExitInventory()
{
	if(gpBase->mpInputHandler->GetState() != eLuxInputState_Inventory) return;

	gpBase->mpInventory->ExitPressed();
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetInventoryDisabled(bool abX)
{
	gpBase->mpInventory->SetDisabled(abX);
}

void __stdcall cLuxScriptHandler::SetInventoryMessage(string &asTextCategory, string &asTextEntry, float afTime)
{
	gpBase->mpInventory->SetMessageText(kTranslate(asTextCategory, asTextEntry), afTime);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::GiveItem(string& asName, string& asType, string& asSubTypeName, string& asImageName, float afAmount)
{
	eLuxItemType type = gpBase->mpInventory->GetItemTypeFromString(asType);
	gpBase->mpInventory->AddItem(asName,type,asSubTypeName,asImageName, afAmount, "", "");
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::GiveItemFromFile(string& asName, string& asFileName)
{
	cLuxMap *pMap = gpBase->mpMapHandler->GetCurrentMap();
	if(pMap==NULL) return;

	pMap->ResetLatestEntity();
	pMap->CreateEntity(asName,asFileName, cMatrixf::Identity,1);
	iLuxEntity *pEntity = pMap->GetLatestEntity();
	if(pEntity)
	{
		if(pEntity->GetEntityType() == eLuxEntityType_Prop)
		{
			iLuxProp *pProp = static_cast<iLuxProp*>(pEntity);
			if(pProp->GetPropType() == eLuxPropType_Item)
			{
				cLuxProp_Item *pItem = static_cast<cLuxProp_Item*>(pProp);
				gpBase->mpInventory->AddItem(	asName, pItem->GetItemType(), pItem->GetSubItemTypeName(), pItem->GetImageFile(), pItem->GetAmount(), "", "");
			}
		}
		
		pMap->DestroyEntity(pEntity);
	}
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::RemoveItem(string& asName)
{
	gpBase->mpInventory->RemoveItem(asName);
}

bool __stdcall cLuxScriptHandler::HasItem(string& asName)
{
	return gpBase->mpInventory->GetItem(asName)!=NULL;
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::AddCombineCallback(string& asName, string& asItemA, string& asItemB, string& asFunction, bool abAutoDestroy)
{
	gpBase->mpInventory->AddCombineCallback(asName, asItemA, asItemB, asFunction, abAutoDestroy);
}

void __stdcall cLuxScriptHandler::RemoveCombineCallback(string& asName)
{
	gpBase->mpInventory->RemoveCombineCallback(asName);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::AddUseItemCallback(string& asName, string& asItem, string& asEntity, string& asFunction, bool abAutoDestroy)
{
	cLuxMap *pMap = gpBase->mpMapHandler->GetCurrentMap();
	if(pMap==NULL) return;

    pMap->AddUseItemCallback(asName, asItem, asEntity, asFunction, abAutoDestroy);
}

void __stdcall cLuxScriptHandler::RemoveUseItemCallback(string& asName)
{
	cLuxMap *pMap = gpBase->mpMapHandler->GetCurrentMap();
	if(pMap==NULL) return;

	pMap->RemoveUseItemCallback(asName);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::PreloadParticleSystem(string& asPSFile)
{
	cResources *pResources = gpBase->mpEngine->GetResources();
	pResources->GetParticleManager()->Preload(asPSFile);	
}

void __stdcall cLuxScriptHandler::PreloadSound(string& asSoundFile)
{
	cResources *pResources = gpBase->mpEngine->GetResources();
	pResources->GetSoundEntityManager()->Preload(asSoundFile);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::CreateParticleSystemAtEntity(string& asPSName, string& asPSFile, string& asEntity, bool abSavePS)
{
	CreateParticleSystemAtEntityExt(asPSName, asPSFile, asEntity, abSavePS, 1,1,1,1, false, 1,2,100,110);
}

void __stdcall cLuxScriptHandler::CreateParticleSystemAtEntityExt(	string& asPSName, string& asPSFile, string& asEntity, bool abSavePS, 
																	float afR, float afG, float afB, float afA,
																	bool abFadeAtDistance, float afFadeMinEnd, float afFadeMinStart, 
																	float afFadeMaxStart, float afFadeMaxEnd)
{
	cLuxMap *pMap = gpBase->mpMapHandler->GetCurrentMap();
	if(pMap==NULL) return;

	cParticleSystem *pPS=NULL;

	//////////////////////////
	// Player
	if(asEntity == "Player")
	{
		pPS = pMap->GetWorld()->CreateParticleSystem(asPSName,asPSFile,1.0f);
		if(pPS)
		{
			pPS->SetPosition(gpBase->mpPlayer->GetCharacterBody()->GetPosition());
			pPS->SetIsSaved(abSavePS);
			//gpBase->mpPlayer->GetCharacterBody()->
		}
	}
	//////////////////////////
	// Normal entity
	else
	{
		iLuxEntity* pEntity = GetEntity(asEntity, eLuxEntityType_LastEnum, -1);
		if(pEntity==NULL) return;	

		pPS = pMap->GetWorld()->CreateParticleSystem(asPSName,asPSFile,1.0f);
		if(pPS)
		{
			if(abSavePS==false) pEntity->GetAttachEntity()->AddChild(pPS);
			else				pPS->SetMatrix(pEntity->GetAttachEntity()->GetWorldMatrix());
			pPS->SetIsSaved(abSavePS);
		}
	}

	//////////////////////////
	// Settings
	if(pPS)
	{
		pPS->SetColor(cColor(afR, afG, afB, afA));
		pPS->SetFadeAtDistance(abFadeAtDistance);
		pPS->SetMinFadeDistanceEnd(afFadeMinEnd);
		pPS->SetMinFadeDistanceStart(afFadeMinStart);
		pPS->SetMaxFadeDistanceStart(afFadeMaxStart);
		pPS->SetMaxFadeDistanceEnd(afFadeMaxEnd);
	}
}


void __stdcall cLuxScriptHandler::DestroyParticleSystem(string& asName)
{
	cLuxMap *pMap = gpBase->mpMapHandler->GetCurrentMap();
	if(pMap==NULL) return;

	bool bFound = false;

	cParticleSystemIterator it = pMap->GetWorld()->GetParticleSystemIterator();
	while(it.HasNext())
	{
		cParticleSystem *pPS = it.Next();
		if(pPS->GetName() == asName)
		{
			pPS->Kill();

			bFound = true;
		}
	}
	if(bFound==false) Error("Could not find particle system '%s'\n", asName.c_str());
}

void __stdcall cLuxScriptHandler::SetParticleSystemActive(string& asName, bool bActive)
{
	BEGIN_ITERATE_PARTICLESYSTEM()
		pParticleSystem->SetVisible(bActive);
		pParticleSystem->SetActive(bActive);
	END_ITERATE_PARTICLESYSTEM
}

void __stdcall cLuxScriptHandler::DestroyParticleSystemInstantly(string& asName)
{
	cLuxMap* pMap = gpBase->mpMapHandler->GetCurrentMap();
	if (pMap == NULL) return;

	bool bFound = false;

	cParticleSystemIterator it = pMap->GetWorld()->GetParticleSystemIterator();
	while (it.HasNext())
	{
		cParticleSystem* pPS = it.Next();
		if (pPS->GetName() == asName)
		{
			pPS->KillInstantly();

			bFound = true;
		}
	}
	if (bFound == false) Error("Could not find particle system '%s'\n", asName.c_str());
}

//-----------------------------------------------------------------------


void __stdcall cLuxScriptHandler::PlaySoundAtEntity(string& asSoundName, string& asSoundFile, string& asEntity, float afFadeTime, bool abSaveSound)
{
	float fFadeSpeed = afFadeTime ==0 ? 0 : 1.0f/afFadeTime;

	bool bRemoveWhenOver = true;//abSaveSound ? false : true;

	//////////////////////////
	// Player
	if(asEntity == "Player")
	{
		cLuxMap *pMap = gpBase->mpMapHandler->GetCurrentMap();
		if(pMap==NULL) return;

		cSoundEntity *pSound= pMap->GetWorld()->CreateSoundEntity(asSoundName, asSoundFile,bRemoveWhenOver);
		if(pSound)
		{
			pSound->SetForcePlayAsGUISound(true);
			
			pSound->SetIsSaved(abSaveSound);
			if(afFadeTime >0) pSound->FadeIn(fFadeSpeed);
		}
	}
	//////////////////////////
	// Normal entity
	else
	{
		iLuxEntity* pEntity = GetEntity(asEntity, eLuxEntityType_LastEnum, -1);
		if(pEntity==NULL) return;

		cLuxMap *pMap = gpBase->mpMapHandler->GetCurrentMap();
		if(pMap==NULL) return;

		cSoundEntity *pSound= pMap->GetWorld()->CreateSoundEntity(asSoundName, asSoundFile,bRemoveWhenOver);
		if(pSound)
		{
			if(abSaveSound==false)
			{
				pEntity->GetAttachEntity()->AddChild(pSound);
			}
			else
			{
				pSound->SetPosition(pEntity->GetAttachEntity()->GetWorldPosition());
			}
			
			pSound->SetIsSaved(abSaveSound);
			if(afFadeTime >0) pSound->FadeIn(fFadeSpeed);
		}
	}
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::FadeInSound(string& asSoundName, float afFadeTime, bool abPlayStart)
{
	cLuxMap *pMap = gpBase->mpMapHandler->GetCurrentMap();
	if(pMap==NULL) return;

	float fFadeSpeed = afFadeTime ==0 ? 0 : 1.0f/afFadeTime;
	
	cSoundEntity *pSound = pMap->GetWorld()->GetSoundEntity(asSoundName);
	if(pSound)
	{
		if(fFadeSpeed <=0)	pSound->Play(abPlayStart);
		else				pSound->FadeIn(fFadeSpeed);
	}
	else
	{
		Error("Could not find sound entity '%s'\n", asSoundName.c_str());
	}
}

//-----------------------------------------------------------------------


void __stdcall cLuxScriptHandler::StopSound(string& asSoundName, float afFadeTime)
{
	cLuxMap *pMap = gpBase->mpMapHandler->GetCurrentMap();
	if(pMap==NULL) return;

	float fFadeSpeed = afFadeTime ==0 ? 0 : 1.0f/afFadeTime;
	bool bFound = false;

	cSoundEntityIterator it = pMap->GetWorld()->GetSoundEntityIterator();
    while(it.HasNext())
	{
		cSoundEntity *pSound = it.Next();
        if(pSound->GetName() == asSoundName)
		{
			if(fFadeSpeed <=0)	pSound->Stop(true);
			else				pSound->FadeOut(fFadeSpeed);

			bFound = true;
		}
	}
	if(bFound==false) Error("Could not find sound entity '%s'\n", asSoundName.c_str());
}


//-----------------------------------------------------------------------
void __stdcall cLuxScriptHandler::SetFogAreaVisible(string& asFogAreaName, bool abVisible)
{
	cLuxMap* pMap = gpBase->mpMapHandler->GetCurrentMap();
	if (pMap == NULL) return;

	cFogArea* pFog = pMap->GetWorld()->GetFogArea(asFogAreaName);
	if (pFog == NULL)
	{
		Error("Could not find fog area '%s'\n", asFogAreaName.c_str());
		return;
	}

	pFog->SetVisible(abVisible);
}

void __stdcall cLuxScriptHandler::SetLightVisible(string& asLightName, bool abVisible)
{
	cLuxMap *pMap = gpBase->mpMapHandler->GetCurrentMap();
	if(pMap==NULL) return;

	iLight *pLight = pMap->GetWorld()->GetLight(asLightName);
	if(pLight==NULL)
	{
		Error("Could not find light '%s'\n", asLightName.c_str());
		return;
	}

	pLight->SetVisible(abVisible);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::FadeLightTo(string& asLightName, float afR, float afG, float afB, float afA, float afRadius, float afTime)
{
	cLuxMap *pMap = gpBase->mpMapHandler->GetCurrentMap();
	if(pMap==NULL) return;

	iLight *pLight = pMap->GetWorld()->GetLight(asLightName);
	if(pLight==NULL)
	{
		Error("Could not find light '%s'\n", asLightName.c_str());
		return;
	}

	pLight->SetFlickerActive(false);
	
    cColor newColor(
		afR >=0 ? afR : pLight->GetDiffuseColor().r,	
		afG >=0 ? afG : pLight->GetDiffuseColor().g,
		afB >=0 ? afB : pLight->GetDiffuseColor().b,
		afA >=0 ? afA : pLight->GetDiffuseColor().a);
	
	float fNewRadius = afRadius >=0 ? afRadius : pLight->GetRadius();

	pLight->SetVisible(true);
    pLight->FadeTo(newColor, fNewRadius, afTime);
}

//-----------------------------------------------------------------------
void __stdcall cLuxScriptHandler::FadeLightToExt(string& asLightName, float afR, float afG, float afB, float afA, float afRadius, float afBrightness, float afTime)
{
	cLuxMap* pMap = gpBase->mpMapHandler->GetCurrentMap();
	if (pMap == NULL) return;

	iLight* pLight = pMap->GetWorld()->GetLight(asLightName);
	if (pLight == NULL)
	{
		Error("Could not find light '%s'\n", asLightName.c_str());
		return;
	}

	pLight->SetFlickerActive(false);

	cColor newColor(
		afR >= 0 ? afR : pLight->GetDiffuseColor().r,
		afG >= 0 ? afG : pLight->GetDiffuseColor().g,
		afB >= 0 ? afB : pLight->GetDiffuseColor().b,
		afA >= 0 ? afA : pLight->GetDiffuseColor().a);

	float fNewRadius = afRadius >= 0 ? afRadius : pLight->GetRadius();

	pLight->SetVisible(true);
	pLight->FadeTo(newColor, fNewRadius, afTime);
	pLight->FadeBrightnessTo(afBrightness, afTime);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::FadeLightBrightnessTo(string& asLightName, float afBrightness, float afTime)
{
	cLuxMap* pMap = gpBase->mpMapHandler->GetCurrentMap();
	if (pMap == NULL) return;

	iLight* pLight = pMap->GetWorld()->GetLight(asLightName);
	if (pLight == NULL)
	{
		Error("Could not find light '%s'\n", asLightName.c_str());
		return;
	}

	//pLight->SetFlickerActive(false);

	pLight->SetVisible(true);
	pLight->FadeBrightnessTo(afBrightness, afTime);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetLightFlickerActive(string& asLightName, bool abActive)
{
	cLuxMap *pMap = gpBase->mpMapHandler->GetCurrentMap();
	if(pMap==NULL) return;

	iLight *pLight = pMap->GetWorld()->GetLight(asLightName);
	if(pLight==NULL)
	{
		Error("Could not find light '%s'\n", asLightName.c_str());
		return;
	}

	pLight->SetFlickerActive(abActive);
}

//-----------------------------------------------------------------------
void __stdcall cLuxScriptHandler::SetLampFlickerActive(string& asName, bool abActive)
{
	cLuxMap* pMap = gpBase->mpMapHandler->GetCurrentMap();
	if (pMap == NULL) return;

	BEGIN_SET_PROPERTY(eLuxEntityType_Prop, eLuxPropType_Lamp)

		cLuxProp_Lamp* pLamp = ToLamp(pEntity);
	pLamp->SetFlickerActive(abActive);

	END_SET_PROPERTY
}

//-----------------------------------------------------------------------

float __stdcall cLuxScriptHandler::GetLightColorR(string& asLightName)
{
	cLuxMap* pMap = gpBase->mpMapHandler->GetCurrentMap();
	if (pMap == NULL) return 0;

	iLight* pLight = pMap->GetWorld()->GetLight(asLightName);
	if (pLight == NULL)
	{
		Error("Could not find light '%s'\n", asLightName.c_str());
		return 0;
	}

	return pLight->GetColor().r;
}

float __stdcall cLuxScriptHandler::GetLightColorB(string& asLightName)
{
	cLuxMap* pMap = gpBase->mpMapHandler->GetCurrentMap();
	if (pMap == NULL) return 0;

	iLight* pLight = pMap->GetWorld()->GetLight(asLightName);
	if (pLight == NULL)
	{
		Error("Could not find light '%s'\n", asLightName.c_str());
		return 0;
	}

	return pLight->GetColor().b;
}

float __stdcall cLuxScriptHandler::GetLightColorG(string& asLightName)
{
	cLuxMap* pMap = gpBase->mpMapHandler->GetCurrentMap();
	if (pMap == NULL) return 0;

	iLight* pLight = pMap->GetWorld()->GetLight(asLightName);
	if (pLight == NULL)
	{
		Error("Could not find light '%s'\n", asLightName.c_str());
		return 0;
	}

	return pLight->GetColor().g;
}

float __stdcall cLuxScriptHandler::GetLightAlpha(string& asLightName)
{
	cLuxMap* pMap = gpBase->mpMapHandler->GetCurrentMap();
	if (pMap == NULL) return 0;

	iLight* pLight = pMap->GetWorld()->GetLight(asLightName);
	if (pLight == NULL)
	{
		Error("Could not find light '%s'\n", asLightName.c_str());
		return 0;
	}

	return pLight->GetColor().a;
}

float __stdcall cLuxScriptHandler::GetLightBrightness(string& asLightName)
{
	cLuxMap* pMap = gpBase->mpMapHandler->GetCurrentMap();
	if (pMap == NULL) return 0;

	iLight* pLight = pMap->GetWorld()->GetLight(asLightName);
	if (pLight == NULL)
	{
		Error("Could not find light '%s'\n", asLightName.c_str());
		return 0;
	}

	return pLight->GetBrightness();
}

//-----------------------------------------------------------------------


void __stdcall cLuxScriptHandler::PlayMusic(string& asMusicFile, bool abLoop, float afVolume, float afFadeTime, int alPrio, bool abResume)
{
	gpBase->mpMusicHandler->Play(asMusicFile, abLoop, afVolume, afFadeTime,alPrio,abResume, false);
}

void __stdcall cLuxScriptHandler::StopMusic(float afFadeTime, int alPrio)
{
	gpBase->mpMusicHandler->Stop(afFadeTime, alPrio);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::FadeGlobalSoundVolume(float afDestVolume, float afTime)
{
	bool abDestroy = afDestVolume==1;
	cSoundHandler *pSoundHandler = gpBase->mpEngine->GetSound()->GetSoundHandler();

	cMultipleSettingsHandler* pHandler = pSoundHandler->GetGlobalVolumeSettingsHandler();
	cMultipleSettingsHandler::cGSEntry* pEntry = pHandler->GetEntry(eLuxGlobalVolumeType_Script, true);

	float fSpeed = 1000.0f;
	if(afTime > 0) fSpeed = fabs(afDestVolume-pEntry->GetVal()) / afTime;

	pSoundHandler->FadeGlobalVolume(afDestVolume, fSpeed, eSoundEntryType_World, eLuxGlobalVolumeType_Script, abDestroy);
}

void __stdcall cLuxScriptHandler::FadeGlobalSoundSpeed(float afDestSpeed, float afTime)
{
	bool abDestroy = afDestSpeed==1;
	cSoundHandler *pSoundHandler = gpBase->mpEngine->GetSound()->GetSoundHandler();

	cMultipleSettingsHandler* pHandler = pSoundHandler->GetGlobalSpeedSettingsHandler();
	cMultipleSettingsHandler::cGSEntry* pEntry = pHandler->GetEntry(eLuxGlobalVolumeType_Script, true);

	float fSpeed = 1000.0f;
	if(afTime > 0) fSpeed = fabs(afDestSpeed-pEntry->GetVal()) / afTime;
	pSoundHandler->FadeGlobalSpeed(afDestSpeed, fSpeed, eSoundEntryType_World, eLuxGlobalVolumeType_Script, abDestroy);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetEntityActive(string& asName, bool abActive)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_LastEnum,-1)

		pEntity->SetActive(abActive);

	END_SET_PROPERTY
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetEntityVisible(string& asName, bool abVisible)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_LastEnum,-1)
		if (pEntity->GetMeshEntity() == NULL) continue;

		pEntity->GetMeshEntity()->SetVisible(abVisible);

	END_SET_PROPERTY
}

//-----------------------------------------------------------------------

bool __stdcall cLuxScriptHandler::GetEntityExists(string& asName)
{
	return gpBase->mpMapHandler->GetCurrentMap()->GetEntityByName(asName)!=NULL;
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetEntityPos(string& asName, float afX, float afY, float afZ)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_LastEnum,-1)
		
		if (pEntity->GetEntityType() == eLuxEntityType_Enemy)
		{
			iLuxEnemy *pEnemy = ToEnemy(pEntity);
			pEnemy->GetCharacterBody()->SetFeetPosition(cVector3f(afX, afY, afZ));
		}
		else
		{
			if(pEntity->GetBodyNum() == 0) continue;
		
			pEntity->GetBody(0)->SetWorldPosition(cVector3f(afX, afY, afZ));
		}

	END_SET_PROPERTY
}

//-----------------------------------------------------------------------

float __stdcall cLuxScriptHandler::GetEntityPosX(string& asName)
{
	iLuxEntity* pEntity = GetEntity(asName, eLuxEntityType_LastEnum, -1);
	if (pEntity==NULL) return 0;

	if (pEntity->GetEntityType() == eLuxEntityType_Enemy)
	{
		iLuxEnemy *pEnemy = ToEnemy(pEntity);
		return pEnemy->GetCharacterBody()->GetFeetPosition().x;
	}
	else
	{
		if (pEntity->GetBodyNum() == 0)
		{
			Error("Could not get position of entity '%s' because it has no physics body!\n", asName.c_str());
			return 0;
		}
		return pEntity->GetBody(0)->GetWorldPosition().x;
	}
}

//-----------------------------------------------------------------------

float __stdcall cLuxScriptHandler::GetEntityPosY(string& asName)
{
	iLuxEntity* pEntity = GetEntity(asName, eLuxEntityType_LastEnum, -1);
	if (pEntity==NULL) return 0;

	if (pEntity->GetEntityType() == eLuxEntityType_Enemy)
	{
		iLuxEnemy *pEnemy = ToEnemy(pEntity);
		return pEnemy->GetCharacterBody()->GetFeetPosition().y;
	}
	else
	{
		if (pEntity->GetBodyNum() == 0)
		{
			Error("Could not get position of entity '%s' because it has no physics body!\n", asName.c_str());
			return 0;
		}
		return pEntity->GetBody(0)->GetWorldPosition().y;
	}
}

//-----------------------------------------------------------------------

float __stdcall cLuxScriptHandler::GetEntityPosZ(string& asName)
{
	iLuxEntity* pEntity = GetEntity(asName, eLuxEntityType_LastEnum, -1);
	if (pEntity==NULL) return 0;

	if (pEntity->GetEntityType() == eLuxEntityType_Enemy)
	{
		iLuxEnemy *pEnemy = ToEnemy(pEntity);
		return pEnemy->GetCharacterBody()->GetFeetPosition().z;
	}
	else
	{
		if (pEntity->GetBodyNum() == 0)
		{
			Error("Could not get position of entity '%s' because it has no physics body!\n", asName.c_str());
			return 0;
		}
		return pEntity->GetBody(0)->GetWorldPosition().z;
	}
}

//-----------------------------------------------------------------------

static eLuxFocusCrosshair StringToCrossHair(const tString &asCrossHair)
{
	tString sLowCross = cString::ToLowerCase(asCrossHair);
	
	if(sLowCross=="default")	return eLuxFocusCrosshair_Default;
	if(sLowCross=="grab")		return eLuxFocusCrosshair_Grab;
	if(sLowCross=="push")		return eLuxFocusCrosshair_Push;
	if(sLowCross=="ignite")		return eLuxFocusCrosshair_Ignite;
	if(sLowCross=="pick")		return eLuxFocusCrosshair_Pick;
	if(sLowCross=="leveldoor")	return eLuxFocusCrosshair_LevelDoor;
	if(sLowCross=="ladder")		return eLuxFocusCrosshair_Ladder;
	if (sLowCross == "note")		return eLuxFocusCrosshair_Note;
	if (sLowCross == "talk")		return eLuxFocusCrosshair_Talk;

    Error("CrossHair type '%s' does not exist!\n", asCrossHair.c_str());
	return eLuxFocusCrosshair_Default;
}

void __stdcall cLuxScriptHandler::SetEntityCustomFocusCrossHair(string& asName, string &asCrossHair)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_LastEnum,-1)

		pEntity->SetCustomFocusCrossHair(StringToCrossHair(asCrossHair));

	END_SET_PROPERTY
}
//-----------------------------------------------------------------------
float __stdcall cLuxScriptHandler::GetEntityRotationX(string& asName, int body) {
	iLuxEntity* pEntity = GetEntity(asName, eLuxEntityType_LastEnum, -1);

	if (pEntity == NULL) return 0;

	if (pEntity->GetBodyNum() == 0)
	{
		Error("Could not get rotation of entity '%s' because it has no physics body!\n", asName.c_str());
		return 0;
	}

	if (pEntity->GetBody(body) == NULL)
	{
		Error("'%s' does not have the body requested!\n", asName.c_str());
		return 0;
	}

	cVector3f fRot = cMath::MatrixToEulerAngles(pEntity->GetBody(body)->GetLocalMatrix(), eEulerRotationOrder_XYZ);

	return fRot.x;

}

//-----------------------------------------------------------------------

float __stdcall cLuxScriptHandler::GetEntityRotationY(string& asName, int body) {
	iLuxEntity* pEntity = GetEntity(asName, eLuxEntityType_LastEnum, -1);

	if (pEntity == NULL) return 0;

	if (pEntity->GetBodyNum() == 0)
	{
		Error("Could not get rotation of entity '%s' because it has no physics body!\n", asName.c_str());
		return 0;
	}

	if (pEntity->GetBody(body) == NULL)
	{
		Error("'%s' does not have the body requested!\n", asName.c_str());
		return 0;
	}

	cVector3f fRot = cMath::MatrixToEulerAngles(pEntity->GetBody(body)->GetLocalMatrix(), eEulerRotationOrder_XYZ);

	return fRot.y;

}

//-----------------------------------------------------------------------

float __stdcall cLuxScriptHandler::GetEntityRotationZ(string& asName, int body) {
	iLuxEntity* pEntity = GetEntity(asName, eLuxEntityType_LastEnum, -1);

	if (pEntity == NULL) return 0;

	if (pEntity->GetBodyNum() == 0)
	{
		Error("Could not get rotation of entity '%s' because it has no physics body!\n", asName.c_str());
		return 0;
	}

	if (pEntity->GetBody(body) == NULL)
	{
		Error("'%s' does not have the body requested!\n", asName.c_str());
		return 0;
	}

	cVector3f fRot = cMath::MatrixToEulerAngles(pEntity->GetBody(body)->GetLocalMatrix(), eEulerRotationOrder_XYZ);

	return fRot.z;
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetEntityRotation(string& asName, float afrX, float afrY, float afrZ, int body)
{
	cVector3f mvRot = cVector3f(afrX, afrY, afrZ);

	iLuxEntity* pEntity = GetEntity(asName, eLuxEntityType_LastEnum, -1);

	if (pEntity == NULL) return;

	if (pEntity->GetBodyNum() == 0)
	{
		Error("Could not set rotation of entity '%s' because it has no physics body!\n", asName.c_str());
		return;
	}

	if (pEntity->GetBody(body) == NULL)
	{
		Error("'%s' does not have the body requested!\n", asName.c_str());
		return;
	}

	cMatrixf pMatrix = pEntity->GetBody(body)->GetWorldMatrix();

	cMatrixf mtxTrans2 = cMath::MatrixRotate(mvRot, eEulerRotationOrder_XYZ);
	mtxTrans2.SetTranslation(pMatrix.GetTranslation());
	cMatrixf mtxTrans = mtxTrans2;

	pEntity->GetBody(body)->SetMatrix(mtxTrans);
}

void __stdcall cLuxScriptHandler::SetEntityRotationAndPosition(string& asName, float afrX, float afrY, float afrZ, float afpX, float afpY, float afpZ, int body)
{
	cVector3f mvRot = cVector3f(afrX, afrY, afrZ);
	cVector3f mvPos = cVector3f(afpX, afpY, afrZ);

	iLuxEntity* pEntity = GetEntity(asName, eLuxEntityType_LastEnum, -1);

	if (pEntity == NULL) return;

	if (pEntity->GetBodyNum() == 0)
	{
		Error("Could not set rotation of entity '%s' because it has no physics body!\n", asName.c_str());
		return;
	}

	if (pEntity->GetBody(body) == NULL)
	{
		Error("'%s' does not have the body requested!\n", asName.c_str());
		return;
	}

	cMatrixf pMatrix = pEntity->GetBody(body)->GetWorldMatrix();

	cMatrixf mtxTrans2 = cMath::MatrixRotate(mvRot, eEulerRotationOrder_XYZ);
	mtxTrans2.SetTranslation(mvPos);
	cMatrixf mtxTrans = mtxTrans2;

	pEntity->GetBody(body)->SetMatrix(mtxTrans);
}
//-----------------------------------------------------------------------

float __stdcall cLuxScriptHandler::GetBonePosX(string& asEntity, string& asBoneName)
{
	iLuxEntity* pEntity = GetEntity(asEntity, eLuxEntityType_LastEnum, -1);
	if (pEntity == NULL) { return 0; }
	cBoneState* bone = pEntity->GetMeshEntity()->GetBoneStateFromName(asBoneName);;
	if (bone == NULL)
	{
		Error("Could not get position of bone '%s' because it doesn't exist!\n", asBoneName.c_str());
		return 0;
	}

	return bone->GetWorldPosition().x;
}

//-----------------------------------------------------------------------

float __stdcall cLuxScriptHandler::GetBonePosY(string& asEntity, string& asBoneName)
{
	iLuxEntity* pEntity = GetEntity(asEntity, eLuxEntityType_LastEnum, -1);
	if (pEntity == NULL) { return 0; }
	cBoneState* bone = pEntity->GetMeshEntity()->GetBoneStateFromName(asBoneName);;
	if (bone == NULL)
	{
		Error("Could not get position of bone '%s' because it doesn't exist!\n", asBoneName.c_str());
		return 0;
	}

	return bone->GetWorldPosition().y;
}

//-----------------------------------------------------------------------

float __stdcall cLuxScriptHandler::GetBonePosZ(string& asEntity, string& asBoneName)
{
	iLuxEntity* pEntity = GetEntity(asEntity, eLuxEntityType_LastEnum, -1);
	if (pEntity == NULL) { return 0; }
	cBoneState* bone = pEntity->GetMeshEntity()->GetBoneStateFromName(asBoneName);
	if (bone == NULL)
	{
		Error("Could not get position of bone '%s' because it doesn't exist!\n", asBoneName.c_str());
		return 0;
	}

	return bone->GetWorldPosition().z;
}

//-----------------------------------------------------------------------

float __stdcall cLuxScriptHandler::GetBoneRotX(string& asEntity, string& asBoneName)
{
	iLuxEntity* pEntity = GetEntity(asEntity, eLuxEntityType_LastEnum, -1);
	if (pEntity == NULL) { return 0; }
	cBoneState* bone = pEntity->GetMeshEntity()->GetBoneStateFromName(asBoneName);
	if (bone == NULL)
	{
		Error("Could not get rotation of bone '%s' because it doesn't exist!\n", asBoneName.c_str());
		return 0;
	}
	cVector3f fRot = cMath::MatrixToEulerAngles(bone->GetWorldMatrix().GetRotation(), eEulerRotationOrder_XYZ);
	return fRot.x;
}

//-----------------------------------------------------------------------

float __stdcall cLuxScriptHandler::GetBoneRotY(string& asEntity, string& asBoneName)
{
	iLuxEntity* pEntity = GetEntity(asEntity, eLuxEntityType_LastEnum, -1);
	if (pEntity == NULL) { return 0; }
	cBoneState* bone = pEntity->GetMeshEntity()->GetBoneStateFromName(asBoneName);
	if (bone == NULL)
	{
		Error("Could not get rotation of bone '%s' because it doesn't exist!\n", asBoneName.c_str());
		return 0;
	}
	cVector3f fRot = cMath::MatrixToEulerAngles(bone->GetWorldMatrix().GetRotation(), eEulerRotationOrder_XYZ);
	return fRot.y;
}

//-----------------------------------------------------------------------

float __stdcall cLuxScriptHandler::GetBoneRotZ(string& asEntity, string& asBoneName)
{
	iLuxEntity* pEntity = GetEntity(asEntity, eLuxEntityType_LastEnum, -1);
	if (pEntity == NULL) { return 0; }
	cBoneState* bone = pEntity->GetMeshEntity()->GetBoneStateFromName(asBoneName);
	if (bone == NULL)
	{
		Error("Could not get rotation of bone '%s' because it doesn't exist!\n", asBoneName.c_str());
		return 0;
	}
	cVector3f fRot = cMath::MatrixToEulerAngles(bone->GetWorldMatrix().GetRotation(), eEulerRotationOrder_XYZ);
	return fRot.z;
}
//----------------------------------------------------------------------
void __stdcall cLuxScriptHandler::DetachPropFromBone(string& asChildEntityName)
{
	iLuxProp* pChildProp = ToProp(GetEntity(asChildEntityName, eLuxEntityType_Prop, -1));
	if (pChildProp == NULL)
	{
		Error("Could not find child prop '%s'\n", asChildEntityName.c_str());
		return;
	}

	pChildProp->DetachFromParentBone();
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::AttachPropToBone(string& asChildEntityName, string& asParentEntityName, string& asParentBoneName, float fPosX, float fPosY, float fPosZ, float fRotX, float fRotY, float fRotZ)
{
	iLuxProp* pChildProp = ToProp(GetEntity(asChildEntityName, eLuxEntityType_Prop, -1));
	if (pChildProp == NULL)
	{
		Error("Could not find child prop '%s'\n", asChildEntityName.c_str());
		return;
	}

	iLuxEntity* pParentEntity = GetEntity(asParentEntityName, eLuxEntityType_LastEnum, -1);

	if (pParentEntity == NULL)
	{
		Error("Could not find parent entity '%s'\n", asParentEntityName.c_str());
		return;
	}

	cMeshEntity* pChildMeshEntity = pChildProp->GetMeshEntity();

	if (pChildMeshEntity == NULL)
	{
		Error("Child entity '%s' does not have a mesh entity\n", asChildEntityName.c_str());
		return;
	}

	cMeshEntity* pParentMeshEntity = pParentEntity->GetMeshEntity();
	

	if (pParentMeshEntity == NULL)
	{
		Error("Parent entity '%s' does not have a mesh entity\n", asParentEntityName.c_str());
		return;
	}
	
	/*if (pParentMeshEntity->GetSubMeshEntityNum() >= 1)
	{
		pParentMeshEntity = pParentEntity->GetMeshEntity()->GetSubMeshEntity(1);
	}*/

	cBoneState* pParentBone = pParentMeshEntity->GetBoneStateFromName(asParentBoneName);

	if (pParentBone == NULL)
	{
		Error("Did not find bone '%s' in parent entity '%s'\n", asParentBoneName.c_str(), asParentEntityName.c_str());
		return;
	}

	cMatrixf mtxTransform = cMath::MatrixRotate(cMath::Vector3ToRad(cVector3f(fRotX, fRotY, fRotZ)), eEulerRotationOrder_XYZ);
	mtxTransform.SetTranslation(cVector3f(fPosX, fPosY, fPosZ));

	pChildProp->SetParentBone(pParentBone, mtxTransform, asParentEntityName, asParentBoneName);
}
//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::CreateEntityAtArea(string& asEntityName, string& asEntityFile, string& asAreaName, bool abFullGameSave)
{
	cLuxMap *pMap = gpBase->mpMapHandler->GetCurrentMap();

	iLuxArea *pArea = ToArea(GetEntity(asAreaName,eLuxEntityType_Area, -1));
	if(pArea == NULL) return;

	pMap->ResetLatestEntity();
	pMap->CreateEntity(asEntityName, asEntityFile, pArea->GetBody()->GetWorldMatrix(),1);

	iLuxEntity *pEntity = pMap->GetLatestEntity();
	if(pEntity && pEntity->GetName() == asEntityName)
	{
		pEntity->SetFullGameSave(abFullGameSave);	
	}
	else
	{
		Error("Could not create entity '%s' from file '%s'!\n", asEntityName.c_str(), asEntityFile.c_str());
	}
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::ReplaceEntity(string& asName, string& asBodyName, string& asNewEntityName, string& asNewEntityFile, bool abFullGameSave)
{
	cLuxMap *pMap = gpBase->mpMapHandler->GetCurrentMap();

	iLuxEntity *pEntity = GetEntity(asName,eLuxEntityType_LastEnum, -1);
	if(pEntity == NULL) return;

	iPhysicsBody *pBody = GetBodyInEntity(pEntity, asBodyName);
	if(pBody == NULL) return;

	cMatrixf mtxTransform = pBody->GetWorldMatrix();

	pMap->DestroyEntity(pEntity);

	pMap->ResetLatestEntity();
	pMap->CreateEntity(asNewEntityName, asNewEntityFile, mtxTransform,1);

	iLuxEntity *pNewEntity = pMap->GetLatestEntity();
	if(pNewEntity && pNewEntity->GetName() == asNewEntityName)
	{
		pNewEntity->SetFullGameSave(abFullGameSave);	
	}
	else
	{
		Error("Could not create entity '%s' from file '%s'!\n", asNewEntityName.c_str(), asNewEntityFile.c_str());
	}
}

void __stdcall cLuxScriptHandler::PlaceEntityAtEntity(string& asName, string& asTargetEntity, string& asTargetBodyName, bool abUseRotation)
{
	cLuxMap *pMap = gpBase->mpMapHandler->GetCurrentMap();

	iLuxEntity *pEntity = GetEntity(asName,eLuxEntityType_LastEnum, -1);
	if(pEntity == NULL) return;
	if(pEntity->GetBodyNum() == 0)
	{
		Error("Entity '%s' has no body that can be placed at '%s'!\n", asName.c_str(), asTargetEntity.c_str());
		return;
	}

	iPhysicsBody *pBody = GetBodyInEntity(pEntity, "");
	if(pBody == NULL) return;

	iLuxEntity *pTargetEntity = GetEntity(asTargetEntity,eLuxEntityType_LastEnum, -1);
	if(pEntity == NULL) return;

	iPhysicsBody *pTargetBody = GetBodyInEntity(pTargetEntity, asTargetBodyName);
	if(pTargetBody == NULL) return;

	cMatrixf mtxTransform;
	if (abUseRotation)
	{
		mtxTransform = pTargetBody->GetWorldMatrix();
	}
	else
	{
		mtxTransform = pBody->GetWorldMatrix();
		mtxTransform.SetTranslation(pTargetBody->GetWorldPosition());
	}

	pBody->SetWorldMatrix(mtxTransform);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetEntityPlayerLookAtCallback(string& asName, string& asCallback, bool abRemoveWhenLookedAt)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_LastEnum,-1)

		pEntity->SetPlayerLookAtCallback(asCallback, abRemoveWhenLookedAt);

	END_SET_PROPERTY
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetEntityPlayerInteractCallback(string& asName, string& asCallback, bool abRemoveOnInteraction)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_LastEnum,-1)

		pEntity->SetPlayerInteractCallback(asCallback, abRemoveOnInteraction);

	END_SET_PROPERTY
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetEntityCallbackFunc(string& asName, string& asCallback)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_LastEnum,-1)

		pEntity->SetCallbackFunc(asCallback);

	END_SET_PROPERTY
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetEntityConnectionStateChangeCallback(string& asName, string& asCallback)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_LastEnum,-1)

		pEntity->SetConnectionStateChangeCallback(asCallback);

	END_SET_PROPERTY
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetEntityInteractionDisabled(string& asName, bool abDisabled)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Prop,-1)

		iLuxProp *pProp = ToProp(pEntity);
		pProp->SetInteractionDisabled(abDisabled);
	
	END_SET_PROPERTY

}

//-----------------------------------------------------------------------

bool __stdcall cLuxScriptHandler::GetEntitiesCollide(string& asEntityA, string& asEntityB)
{
	iLuxEntity *pEntityA = GetEntity(asEntityA, eLuxEntityType_LastEnum,-1);
	if(pEntityA==NULL) return false;

	iLuxEntity *pEntityB = GetEntity(asEntityB, eLuxEntityType_LastEnum,-1);
	if(pEntityB==NULL) return false;

	return pEntityA->CheckEntityCollision(pEntityB, gpBase->mpMapHandler->GetCurrentMap());
}

float __stdcall cLuxScriptHandler::GetEntitiesDistance(string& asEntityA, string& asEntityB)
{
	iLuxEntity* pEntityA = GetEntity(asEntityA, eLuxEntityType_LastEnum, -1);
	iLuxEntity* pEntityB = GetEntity(asEntityB, eLuxEntityType_LastEnum, -1);
	if (pEntityA == NULL || pEntityB == NULL) { return 0.001f; }
	return gpBase->mpMapHelper->CheckDistance(pEntityA->GetBody(0)->GetWorldPosition(), pEntityB->GetBody(0)->GetWorldPosition());
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetPropEffectActive(string& asName, bool abActive, bool abFadeAndPlaySounds)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Prop,-1)

		iLuxProp *pProp = ToProp(pEntity);
		pProp->SetEffectsActive(abActive, abFadeAndPlaySounds);
	
	END_SET_PROPERTY
}

//-----------------------------------------------------------------------


void __stdcall cLuxScriptHandler::SetPropActiveAndFade(string& asName, bool abActive, float afFadeTime)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Prop,-1)

		iLuxProp *pProp = ToProp(pEntity);
		pProp->SetActive(abActive);
		if(abActive)
		{
			pProp->FadeInMeshEntity(afFadeTime);
		}
		else if(pProp->GetMeshEntity())
		{
			gpBase->mpMapHandler->GetCurrentMap()->AddDissolveEntity(pProp->GetMeshEntity(), afFadeTime);
		}
			

	END_SET_PROPERTY
}

void __stdcall cLuxScriptHandler::SetPropStaticPhysics(string& asName, bool abX)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Prop,-1)

		iLuxProp *pProp = ToProp(pEntity);
		pProp->SetStaticPhysics(abX);

	END_SET_PROPERTY
}

//-----------------------------------------------------------------------

bool __stdcall cLuxScriptHandler::GetPropIsInteractedWith(string& asName)
{
	iLuxProp *pProp = ToProp(GetEntity(asName, eLuxEntityType_Prop, -1));
	if(pProp==NULL) return false;

    return pProp->IsInteractedWith();
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::RotatePropToSpeed(string& asName, float afAcc, float afGoalSpeed, float afAxisX, float afAxisY, float afAxisZ, bool abResetSpeed, string& asOffsetArea)
{
	iLuxArea *pArea = NULL;
	if(asOffsetArea != "")
	{
		pArea = ToArea(GetEntity(asOffsetArea,eLuxEntityType_Area, -1));
	}


	BEGIN_SET_PROPERTY(eLuxEntityType_Prop,-1)

		iLuxProp *pProp = ToProp(pEntity);
		pProp->SetFullGameSave(true);
		if(pArea)
		{
			cMatrixf mtxInvLoad = cMath::MatrixInverse(pProp->GetMainBody()->GetWorldMatrix());
			cVector3f vLocalOffset = cMath::MatrixMul(mtxInvLoad, pArea->GetPosition());

			pProp->RotateAtSpeed(afAcc, afGoalSpeed, cVector3f(afAxisX, afAxisY, afAxisZ), abResetSpeed, true, 
								pArea->GetPosition(), vLocalOffset);
		}
		else
		{
			pProp->RotateAtSpeed(afAcc, afGoalSpeed, cVector3f(afAxisX, afAxisY, afAxisZ), abResetSpeed, false, 0,0);
		}

	END_SET_PROPERTY
}

//-----------------------------------------------------------------------

void __stdcall  cLuxScriptHandler::StopPropMovement(string& asName)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Prop,-1)

		iLuxProp *pProp = ToProp(pEntity);
		pProp->StopMove();
		

	END_SET_PROPERTY
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::AttachPropToProp(string& asPropName, string& asAttachName, string& asAttachFile, float afPosX, float afPosY, float afPosZ, float afRotX, float afRotY, float afRotZ)
{
	tString asName = asPropName;

	cMatrixf mtxTransform = cMath::MatrixRotate(cMath::Vector3ToRad(cVector3f(afRotX, afRotY, afRotZ)), eEulerRotationOrder_XYZ);
	mtxTransform.SetTranslation(cVector3f(afPosX, afPosY, afPosZ));

	BEGIN_SET_PROPERTY(eLuxEntityType_Prop,-1)

		iLuxProp *pProp = ToProp(pEntity);
		pProp->AddAndAttachProp(asAttachName, asAttachFile, mtxTransform);

	END_SET_PROPERTY
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::AddAttachedPropToProp(string& asPropName, string& asAttachName, string& asAttachFile, float afPosX, float afPosY, float afPosZ, float afRotX, float afRotY, float afRotZ)
{
	Warning("AddAttachedPropToProp is deprectated, use AttachPropToProp instead!\n");
	AttachPropToProp(asPropName, asAttachName, asAttachFile, afPosX, afPosY, afRotZ, afRotX, afRotY, afRotZ);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::RemoveAttachedPropFromProp(string& asPropName, string& asAttachName)
{
	tString asName = asPropName;

	BEGIN_SET_PROPERTY(eLuxEntityType_Prop,-1)

		iLuxProp *pProp = ToProp(pEntity);
		if(pProp->DestroyAttachedProp(asAttachName)==false)
		{
			Error("Could not find attached prop '%s' in '%s'\n", asPropName.c_str(), asAttachName.c_str());
		}

	END_SET_PROPERTY
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetLampLit(string& asName, bool abLit, bool abEffects)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Prop,eLuxPropType_Lamp)

		cLuxProp_Lamp *pLamp = ToLamp(pEntity);
		pLamp->SetLit(abLit, abEffects);

	END_SET_PROPERTY
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetSwingDoorLocked(string& asName, bool abLocked, bool abEffects)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Prop,eLuxPropType_SwingDoor)
    
		cLuxProp_SwingDoor *pSwingDoor = ToSwingDoor(pEntity);
		pSwingDoor->SetLocked(abLocked, abEffects);

	END_SET_PROPERTY
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetSwingDoorClosed(string& asName, bool abClosed, bool abEffects)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Prop,eLuxPropType_SwingDoor)

		cLuxProp_SwingDoor *pSwingDoor = ToSwingDoor(pEntity);
		pSwingDoor->SetClosed(abClosed, abEffects);

	END_SET_PROPERTY
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetSwingDoorDisableAutoClose(string& asName, bool abDisableAutoClose)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Prop,eLuxPropType_SwingDoor)

		cLuxProp_SwingDoor *pSwingDoor = ToSwingDoor(pEntity);
		pSwingDoor->SetDisableAutoClose(abDisableAutoClose);

	END_SET_PROPERTY
}

//-----------------------------------------------------------------------
void __stdcall cLuxScriptHandler::SetSwingDoorOpenAmount(string& asName, float afOpenAmount, float afDuration, bool abOpenTowardsMaxAngle)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Prop, eLuxPropType_SwingDoor)

		cLuxProp_SwingDoor* pSwingDoor = ToSwingDoor(pEntity);
	pSwingDoor->SetOpenAmount(afOpenAmount, afDuration, abOpenTowardsMaxAngle);

	END_SET_PROPERTY
}

//-----------------------------------------------------------------------


bool __stdcall cLuxScriptHandler::GetSwingDoorLocked(string &asName)
{
	cLuxProp_SwingDoor *pSwingDoor = ToSwingDoor(GetEntity(asName,eLuxEntityType_Prop,eLuxPropType_SwingDoor));
	if(pSwingDoor==NULL) return false;

    return pSwingDoor->GetLocked();
}

bool __stdcall cLuxScriptHandler::GetSwingDoorClosed(string &asName)
{
	cLuxProp_SwingDoor *pSwingDoor = ToSwingDoor(GetEntity(asName,eLuxEntityType_Prop,eLuxPropType_SwingDoor));
	if(pSwingDoor==NULL) return false;

	return pSwingDoor->GetClosed();
}

int __stdcall cLuxScriptHandler::GetSwingDoorState(string &asName)
{
	cLuxProp_SwingDoor *pSwingDoor = ToSwingDoor(GetEntity(asName,eLuxEntityType_Prop,eLuxPropType_SwingDoor));
	if(pSwingDoor==NULL) return 0;
	
	return pSwingDoor->GetDoorState();
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetLevelDoorLocked(string& asName, bool abLocked)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Prop,eLuxPropType_LevelDoor)

		cLuxProp_LevelDoor *pLevelDoor = ToLevelDoor(pEntity);
		pLevelDoor->SetLocked(abLocked);

	END_SET_PROPERTY
}

void __stdcall cLuxScriptHandler::SetLevelDoorLockedSound(string& asName, string& asSound)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Prop,eLuxPropType_LevelDoor)

		cLuxProp_LevelDoor *pLevelDoor = ToLevelDoor(pEntity);
		pLevelDoor->SetLockedSound(asSound);

	END_SET_PROPERTY
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetLevelDoorLockedText(string& asName, string& asTextCat, string& asTextEntry)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Prop,eLuxPropType_LevelDoor)

		cLuxProp_LevelDoor *pLevelDoor = ToLevelDoor(pEntity);
		pLevelDoor->SetLockedText(asTextCat, asTextEntry);

	END_SET_PROPERTY
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetPropObjectStuckState(string& asName, int alState)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Prop,eLuxPropType_Object)

		cLuxProp_Object *pObject = ToObject(pEntity);
		pObject->SetStuckState(alState);

	END_SET_PROPERTY
}

void __stdcall cLuxScriptHandler::SetWheelAngle(string& asName, float afAngle, bool abAutoMove)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Prop,eLuxPropType_Wheel)

		cLuxProp_Wheel *pWheel = ToWheel(pEntity);
		pWheel->SetAngle(afAngle, abAutoMove);

	END_SET_PROPERTY
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetWheelStuckState(string& asName, int alState, bool afEffects)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Prop,eLuxPropType_Wheel)

		cLuxProp_Wheel *pWheel = ToWheel(pEntity);
		pWheel->SetStuckState(alState, afEffects);

	END_SET_PROPERTY
}

//-----------------------------------------------------------------------


void __stdcall cLuxScriptHandler::SetLeverStuckState(string& asName, int alState, bool afEffects)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Prop,eLuxPropType_Lever)

		cLuxProp_Lever *pLever = ToLever(pEntity);
		pLever->SetStuckState(alState, afEffects);

	END_SET_PROPERTY
}
//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetWheelInteractionDisablesStuck(string& asName, bool abX)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Prop,eLuxPropType_Wheel)

		cLuxProp_Wheel *pWheel = ToWheel(pEntity);
		pWheel->SetInteractionDisablesStuck(abX);

	END_SET_PROPERTY
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetLeverInteractionDisablesStuck(string& asName, bool abX)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Prop,eLuxPropType_Lever)

		cLuxProp_Lever *pLever = ToLever(pEntity);
		pLever->SetInteractionDisablesStuck(abX);

	END_SET_PROPERTY
}

//-----------------------------------------------------------------------

int __stdcall cLuxScriptHandler::GetLeverState(string& asName)
{
	cLuxProp_Lever *pLever = ToLever(GetEntity(asName, eLuxEntityType_Prop, eLuxPropType_Lever));
	if(pLever==NULL) return 0;

	return pLever->GetLeverState();
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetMultiSliderStuckState(string& asName, int alStuckState, bool abEffects)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Prop,eLuxPropType_MultiSlider)

		cLuxProp_MultiSlider *pSlider = ToMultiSlider(pEntity);
		pSlider->SetStuckState(alStuckState, abEffects);

	END_SET_PROPERTY
}

void __stdcall cLuxScriptHandler::SetMultiSliderCallback(string& asName, string& asCallback)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Prop,eLuxPropType_MultiSlider)

		cLuxProp_MultiSlider *pSlider = ToMultiSlider(pEntity);
		pSlider->SetChangeStateCallback(asCallback);

	END_SET_PROPERTY
}


//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetButtonSwitchedOn(string& asName, bool abSwitchedOn, bool abEffects)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Prop,eLuxPropType_Button)

		cLuxProp_Button *pButton = ToButton(pEntity);
		pButton->SetSwitchedOn(abSwitchedOn, abEffects);

	END_SET_PROPERTY
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetAllowStickyAreaAttachment(bool abX)
{
	cLuxArea_Sticky::SetAllowAttachment(abX);
}

void __stdcall cLuxScriptHandler::AttachPropToStickyArea(string& asAreaName, string& asProp)
{
	cLuxArea_Sticky *pStickyArea = ToStickyArea(GetEntity(asAreaName,eLuxEntityType_Area,eLuxAreaType_Sticky));
	iLuxProp *pProp = ToProp(GetEntity(asProp,eLuxEntityType_Prop,-1));
	
	if(pProp==NULL || pStickyArea==NULL) return;

	pStickyArea->AttachBody(pProp->GetMainBody() ? pProp->GetMainBody() : pProp->GetBody(0));	
}

void __stdcall cLuxScriptHandler::AttachBodyToStickyArea(string& asAreaName, string& asBody)
{
	cLuxArea_Sticky *pStickyArea = ToStickyArea(GetEntity(asAreaName,eLuxEntityType_Area,eLuxAreaType_Sticky));
	if(pStickyArea==NULL) return;
	
	cLuxMap *pMap = gpBase->mpMapHandler->GetCurrentMap();
	iPhysicsBody *pBody = pMap->GetPhysicsWorld()->GetBody(asBody);
	if(pBody==NULL)
	{
		Error("Could not find body '%s'\n", asBody.c_str());
		return;
	}

	pStickyArea->AttachBody(pBody);
}

void __stdcall cLuxScriptHandler::DetachFromStickyArea(string& asAreaName)
{
	cLuxArea_Sticky *pStickyArea = ToStickyArea(GetEntity(asAreaName,eLuxEntityType_Area,eLuxAreaType_Sticky));
	
	if(pStickyArea) pStickyArea->DetachBody();
}


void __stdcall cLuxScriptHandler::AttachAreaToProp(string& asAreaName, string& asProp, int alBody)
{
	iLuxArea* pArea = ToArea(GetEntity(asAreaName, eLuxEntityType_Area, -1));

	if (pArea == NULL)
	{
#if 1
		Error("WARNING: AttachAreaToProp Area Not Found:'%s'\n", asAreaName.c_str());
#endif
		return;
	}

	pArea->AttachToBody(asProp, alBody);
}
//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetNPCAwake(string& asName, bool abAwake, bool abEffects)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Prop,eLuxPropType_NPC)

		cLuxProp_NPC *pNPC = ToNPC(pEntity);
		pNPC->SetAwake(abAwake, abEffects);

	END_SET_PROPERTY
}

void __stdcall cLuxScriptHandler::SetNPCFollowPlayer(string& asName, bool abX)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Prop,eLuxPropType_NPC)

		cLuxProp_NPC *pNPC = ToNPC(pEntity);
		pNPC->SetFollowPlayer(abX);

	END_SET_PROPERTY
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetEnemyDisabled(string& asName, bool abDisabled)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy,-1)

		iLuxEnemy *pEnemy = ToEnemy(pEntity);
		pEnemy->SetDisabled(abDisabled);
	
	END_SET_PROPERTY
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetEnemyIsHallucination(string& asName, bool abX)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy,-1)

		iLuxEnemy *pEnemy = ToEnemy(pEntity);
		pEnemy->SetHallucination(abX);

	END_SET_PROPERTY
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::FadeEnemyToSmoke(string& asName, bool abPlaySound)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy,-1)

		iLuxEnemy *pEnemy = ToEnemy(pEntity);
		pEnemy->FadeToSmoke(abPlaySound);

	END_SET_PROPERTY
}

void __stdcall cLuxScriptHandler::ShowEnemyPlayerPosition(string& asName)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy,-1)

		iLuxEnemy *pEnemy = ToEnemy(pEntity);
		pEnemy->ShowPlayerPosition();
		
		eLuxEnemyState state = pEnemy->GetCurrentEnemyState();
		if(	state != eLuxEnemyState_Hunt ||
			state != eLuxEnemyState_AttackMeleeLong ||
			state != eLuxEnemyState_AttackMeleeShort ||
			state != eLuxEnemyState_BreakDoor)
		{
			pEnemy->ChangeState(eLuxEnemyState_Hunt);
		}
	
	END_SET_PROPERTY
}

void __stdcall cLuxScriptHandler::SetEnemyEndOfPatrolCallback(string& asName, string& asFunc, bool abRemoveWhenCalled)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy, -1)

		iLuxEnemy* pEnemy = ToEnemy(pEntity);
	pEnemy->SetEndOfPathCallback(asFunc, abRemoveWhenCalled);

	END_SET_PROPERTY
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::ForceEnemyWaitState(string& asName)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy, -1)

		iLuxEnemy* pEnemy = ToEnemy(pEntity);
	pEnemy->ChangeState(eLuxEnemyState_Wait);

	END_SET_PROPERTY
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::AlertEnemyOfPlayerPresence(string& asName)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy,-1)

		iLuxEnemy *pEnemy = ToEnemy(pEntity);
		pEnemy->ShowPlayerPosition();
		
		eLuxEnemyState state = pEnemy->GetCurrentEnemyState();
		if(	state != eLuxEnemyState_Hunt &&
			state != eLuxEnemyState_AttackMeleeLong &&
			state != eLuxEnemyState_AttackMeleeShort &&
			state != eLuxEnemyState_BreakDoor)
		{
			pEnemy->ChangeState(eLuxEnemyState_Search);
		}
	
	END_SET_PROPERTY
}
void __stdcall cLuxScriptHandler::MakeEnemySearchPosition(string& asName, float afX, float afY, float afZ)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy, -1)

		iLuxEnemy* pEnemy = ToEnemy(pEntity);

	cVector3f searchPos(afX, afY, afZ);

	eLuxEnemyState state = pEnemy->GetCurrentEnemyState();
	if (state != eLuxEnemyState_Hunt &&
		state != eLuxEnemyState_AttackMeleeLong &&
		state != eLuxEnemyState_AttackMeleeShort &&
		state != eLuxEnemyState_BreakDoor)
	{
		pEnemy->ChangeState(eLuxEnemyState_Search);
		pEnemy->SetSpecialSearchNode(searchPos);
		pEnemy->SetSpecialSearchNodeActive(true);
	}

	END_SET_PROPERTY
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetEnemyDisableTriggers(string& asName, bool abX)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy,-1)

		iLuxEnemy *pEnemy = ToEnemy(pEntity);
		pEnemy->SetDisableTriggers(abX);

	END_SET_PROPERTY
}


//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::AddEnemyPatrolNode(string& asName, string& asNodeName, float afWaitTime, string& asAnimation)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy,-1)
		
		iLuxEnemy *pEnemy = ToEnemy(pEntity);
		cAINodeContainer *pAINodeCont = pEnemy->GetPathFinder()->GetNodeContainer();
		if(pAINodeCont==NULL)
		{
			Error("There is no node container in enemy '%s'! (probably no nodes in map!)\n", pEnemy->GetName().c_str());
			continue;
		}

		cAINode *pNode = pAINodeCont->GetNodeFromName(asNodeName);
		
		if(pNode==NULL)
		{
			Error("Could not find node '%s' for enemy '%s'\n", asNodeName.c_str(), pEntity->GetName().c_str());
			continue;
		}

		pEnemy->AddPatrolNode(pNode, afWaitTime, asAnimation);
	
	END_SET_PROPERTY
}

void __stdcall cLuxScriptHandler::ClearEnemyPatrolNodes(string& asName)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy,-1)
		
		iLuxEnemy *pEnemy = ToEnemy(pEntity);
		pEnemy->ClearPatrolNodes();

	END_SET_PROPERTY
}

void __stdcall cLuxScriptHandler::PlayEnemyAnimation(string& asName, string& asAnimName, bool abLoop, float afFadeTime)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy, -1)

		iLuxEnemy* pEnemy = ToEnemy(pEntity);
		pEnemy->PlayAnim(asAnimName, abLoop, afFadeTime);

	END_SET_PROPERTY
}

void __stdcall cLuxScriptHandler::SetEnemyBlind(string& asName, bool abX)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy, -1)

		iLuxEnemy* pEnemy = ToEnemy(pEntity);
		pEnemy->SetBlind(abX);

	END_SET_PROPERTY
}

void __stdcall cLuxScriptHandler::SetEnemyDeaf(string& asName, bool abX)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy, -1)

		iLuxEnemy* pEnemy = ToEnemy(pEntity);
	pEnemy->SetDeaf(abX);

	END_SET_PROPERTY
}


void __stdcall cLuxScriptHandler::SetEnemySanityDecreaseActive(string& asName, bool abX)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy,-1)
		
		iLuxEnemy *pEnemy = ToEnemy(pEntity);
		pEnemy->SetSanityDecreaseActive(abX);

	END_SET_PROPERTY
}

void __stdcall cLuxScriptHandler::TeleportEnemyToNode(string & asName, string & asNodeName, bool abChangeY)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy,-1)

		iLuxEnemy *pEnemy = ToEnemy(pEntity);
		cAINode *pNode = pEnemy->GetPathFinder()->GetNodeContainer()->GetNodeFromName(asNodeName);
		if(pNode==NULL){
			Error("Could not find node '%s' for enemy '%s'\n", asNodeName.c_str(), pEnemy->GetName().c_str());
			continue;
		}
		
		cVector3f vNodePos = pNode->GetPosition();
		if(abChangeY==false) vNodePos.y = pEnemy->GetCharacterBody()->GetFeetPosition().y;

		pEnemy->GetCharacterBody()->SetFeetPosition(vNodePos);

	END_SET_PROPERTY
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::TeleportEnemyToEntity(string & asName, string & asTargetEntity, string & asTargetBody, bool abChangeY)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy,-1)

		iLuxEnemy *pEnemy = ToEnemy(pEntity);
		iLuxEntity *pTargetEntity = GetEntity(asTargetEntity, eLuxEntityType_LastEnum, -1);
		if(pTargetEntity==NULL){
			Error("Could not find entity '%s' for enemy '%s' to be teleported to.\n", asTargetEntity.c_str(), pEnemy->GetName().c_str());
			continue;
		}
		
		cVector3f vTargetPos;
		if (pTargetEntity->GetEntityType() == eLuxEntityType_Enemy)
		{
			vTargetPos = ToEnemy(pTargetEntity)->GetCharacterBody()->GetFeetPosition();
		}
		else
		{
			iPhysicsBody* pTargetBody = GetBodyInEntity(pTargetEntity, asTargetBody);
			if (pTargetBody == NULL) return;

			vTargetPos = pTargetBody->GetWorldPosition();
		}
		if(abChangeY==false) vTargetPos.y = pEnemy->GetCharacterBody()->GetFeetPosition().y;

		pEnemy->GetCharacterBody()->SetFeetPosition(vTargetPos);

	END_SET_PROPERTY
}
//-----------------------------------------------------------------------

float __stdcall cLuxScriptHandler::GetEnemyPlayerDistance(string& asEnemyName)
{
	iLuxEnemy* pEnemy = ToEnemy(GetEntity(asEnemyName, eLuxEntityType_Enemy, -1));
	if (pEnemy == NULL)
	{
		Error("Can't find enemy '%s'!\n", asEnemyName.c_str());
		return 0;
	}

	return pEnemy->DistToPlayer();
}


bool __stdcall cLuxScriptHandler::GetPlayerCanSeeEnemy(string& asEnemyName)
{
	iLuxEnemy* pEnemy = ToEnemy(GetEntity(asEnemyName, eLuxEntityType_Enemy, -1));
	if (pEnemy == NULL)
	{
		Error("Can't find enemy '%s'!\n", asEnemyName.c_str());
		return false;
	}

	return pEnemy->GetIsSeenByPlayer();
}


//-----------------------------------------------------------------------

string& __stdcall cLuxScriptHandler::GetManPigCurrentPose(string& asName)
{

	cLuxEnemy_ManPig* pEnemy = ToManPig(GetEntity(asName, eLuxEntityType_Enemy, -1));
	if (pEnemy == NULL)
	{
		Error("Can't find enemy '%s'!\n", asName.c_str());
		return gsScriptNull;
	}

	return pEnemy->GetCurrentPoseSuffix();
}

void __stdcall cLuxScriptHandler::ChangeManPigPose(string& asName, string& asPoseType)
{
	eLuxEnemyPoseType pose = eLuxEnemyPoseType_LastEnum;
	if(asPoseType == "Biped")			pose =eLuxEnemyPoseType_Biped;
	else if(asPoseType == "Quadruped")	pose =eLuxEnemyPoseType_Quadruped;

	if(pose == eLuxEnemyPoseType_LastEnum)
	{
		Error("Could not set pose '%s' for enemy '%s'. Pose does not exist!\n", asPoseType.c_str(), asName.c_str());
		return;
	}

	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy,-1)

		cLuxEnemy_ManPig *pEnemy = ToManPig(pEntity);
		if (!pEnemy) continue;
		pEnemy->ChangePose(pose);

	END_SET_PROPERTY
}
//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::ChangeEnemyPatrolSpeed(string& asName, string& asSpeedType)
{
	eLuxEnemyMoveSpeed speed = eLuxEnemyMoveSpeed_LastEnum;
	if (asSpeedType == "Run")			speed = eLuxEnemyMoveSpeed_Run;
	else if (asSpeedType == "Walk")	speed = eLuxEnemyMoveSpeed_Walk;

	if (speed == eLuxEnemyMoveSpeed_LastEnum)
	{
		Error("Could not set Patrol Speed '%s' for enemy '%s'. Patrol Speed  does not exist!\n", asSpeedType.c_str(), asName.c_str());
		return;
	}

	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy, -1)

		iLuxEnemy* pEnemy = ToEnemy(pEntity);
	if (!pEnemy) continue; 
	pEnemy->SetPatrolSpeed(speed);

	END_SET_PROPERTY
}
//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::ChangeEnemyState(string& asName, string& asState)
{
	eLuxEnemyState state = eLuxEnemyState_LastEnum;
		
	if (asState == "Patrol")
	{
		state = eLuxEnemyState_Patrol;
	}
	else if (asState == "Investigate")
	{
		state = eLuxEnemyState_Investigate;
	}
	else if (asState == "Hunt")
	{
		state = eLuxEnemyState_Hunt;
	}
	else if (asState == "Idle")
	{
		state = eLuxEnemyState_Idle;
	}
	else if (asState == "Wait")
	{
		state = eLuxEnemyState_Wait;
	}
	else if (asState == "Alert")
	{
		state = eLuxEnemyState_Alert;
	}
	else if (asState == "HuntPause")
	{
		state = eLuxEnemyState_HuntPause;
	}
	else if (asState == "HuntWander")
	{
		state = eLuxEnemyState_HuntWander;
	}
	else if (asState == "Hurt")
	{
		state = eLuxEnemyState_Hurt;
	}
	else if (asState == "Track")
	{
		state = eLuxEnemyState_Track;
	}

	if (state == eLuxEnemyState_LastEnum)
	{
		Error("Could not set State '%s' for enemy '%s'. State does not exist!\n", asState.c_str(), asName.c_str());
		return;
	}

	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy, -1)

		iLuxEnemy* pEnemy = ToEnemy(pEntity);
	if (!pEnemy) continue;
	pEnemy->ChangeState(state);

	END_SET_PROPERTY
}
//-----------------------------------------------------------------------
void __stdcall cLuxScriptHandler::SetManPigToFlee(string& asName, bool abX)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy, -1)

		cLuxEnemy_ManPig* pEnemy = ToManPig(pEntity);
	if (!pEnemy) continue;
	pEnemy->SetToFlee(abX);

	END_SET_PROPERTY
}
//-----------------------------------------------------------------------
void __stdcall cLuxScriptHandler::SetManPigToThreatenOnAlert(string& asName, bool abX)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy, -1)

		cLuxEnemy_ManPig* pEnemy = ToManPig(pEntity);
	if (!pEnemy) continue;
	pEnemy->ThreatenOnAlertEnabled(abX);

	END_SET_PROPERTY
}

//-----------------------------------------------------------------------
void __stdcall cLuxScriptHandler::SetManPigIdleBehavior(string& asName, string& asBehaviorType)
{
	eLuxIdleBehavior behavior = eLuxIdleBehavior_LastEnum;
	if (asBehaviorType == "Stalk")
	{
		behavior = eLuxIdleBehavior_Stalk;
	}
	else if (asBehaviorType == "Track")
	{
		behavior = eLuxIdleBehavior_Track;
	}
	else if (asBehaviorType == "None")
	{
		behavior = eLuxIdleBehavior_None;
	}

	if (behavior == eLuxEnemyPoseType_LastEnum)
	{
		Error("Could not set Idle Behavior type '%s' for enemy '%s'. Idle Behavior type does not exist!\n", asBehaviorType.c_str(), asName.c_str());
		return;
	}

	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy, -1)

		cLuxEnemy_ManPig* pEnemy = ToManPig(pEntity);
	if (!pEnemy) continue;
	pEnemy->SetBehaviorType(behavior);

	END_SET_PROPERTY
}
//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetTeslaPigFadeDisabled(string& asName, bool abX)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy,-1)

		cLuxEnemy_ManPig *pEnemy = ToManPig(pEntity);
		if (!pEnemy) continue;
		pEnemy->SetTeslaFadeDisabled(abX);
	
	END_SET_PROPERTY
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetTeslaPigSoundDisabled(string& asName, bool abX)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy,-1)

		cLuxEnemy_ManPig *pEnemy = ToManPig(pEntity);
		if (!pEnemy) continue;
		pEnemy->SetTeslaSoundDisabled(abX);
	
	END_SET_PROPERTY
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetTeslaPigEasyEscapeDisabled(string& asName, bool abX)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy,-1)

		cLuxEnemy_ManPig *pEnemy = ToManPig(pEntity);
		if (!pEnemy) continue;
		pEnemy->SetTeslaEasyEscapeDisabled(abX);
	
	END_SET_PROPERTY
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::ForceTeslaPigSighting(string& asName)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy,-1)

		cLuxEnemy_ManPig *pEnemy = ToManPig(pEntity);
		if (!pEnemy) continue;
		pEnemy->ForceTeslaSighting();
	
	END_SET_PROPERTY
}
//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetWraithStealthDashMode(string& asName, bool abX)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy, -1)

		cLuxEnemy_Wraith* pEnemy = ToWraith(pEntity);
	if (!pEnemy) continue;
	pEnemy->SetToStealthMode(abX);

	END_SET_PROPERTY
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetWraithStealthDashAtWill(string& asName, bool abX)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy, -1)

		cLuxEnemy_Wraith* pEnemy = ToWraith(pEntity);
	if (!pEnemy) continue;
	pEnemy->SetCanStealthDashAtWill(abX);

	END_SET_PROPERTY
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetWraithCanMelee(string& asName, bool abX)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy, -1)

		cLuxEnemy_Wraith* pEnemy = ToWraith(pEntity);
	if (!pEnemy) continue;
	pEnemy->SetCanMeele(abX);

	END_SET_PROPERTY
}
//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetWraithStealthDashModeLength(string& asName, int alNodes)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy, -1)

		cLuxEnemy_Wraith* pEnemy = ToWraith(pEntity);
	if (!pEnemy) continue;
	pEnemy->SetStealthModeNodesLength(alNodes);

	END_SET_PROPERTY
}
//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetWraithStealthDashEnterDistance(string& asName, float afNodeDistance)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy, -1)

		cLuxEnemy_Wraith* pEnemy = ToWraith(pEntity);
	if (!pEnemy) continue;
	pEnemy->SetEnterStealthModeNodeDistance(afNodeDistance);

	END_SET_PROPERTY
}
//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetWraithStealthDashExitDistance(string& asName, float afNodeDistance)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy, -1)

		cLuxEnemy_Wraith* pEnemy = ToWraith(pEntity);
	if (!pEnemy) continue;
	pEnemy->SetExitStealthModeNodeDistance(afNodeDistance);

	END_SET_PROPERTY
}
//-----------------------------------------------------------------------
void __stdcall cLuxScriptHandler::SetWraithHeatRayDamageSpeed(string& asName, float afDamageSpeed)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy, -1)

		cLuxEnemy_Wraith* pEnemy = ToWraith(pEntity);
	if (!pEnemy) continue;
	pEnemy->SetHeatRayDamageSpeed(afDamageSpeed);

	END_SET_PROPERTY
}

void __stdcall cLuxScriptHandler::SetWraithHeatRayDamageLightAmount(string& asName, float afLightAmount)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy, -1)

		cLuxEnemy_Wraith* pEnemy = ToWraith(pEntity);
	if (!pEnemy) continue; 
	pEnemy->SetHeatRayDamageLightAmount(afLightAmount);

	END_SET_PROPERTY
}
//-----------------------------------------------------------------------
void __stdcall cLuxScriptHandler::SetWraithBurnDamageTime(string& asName, float afMinTime, float afMaxTime)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy, -1)

		cLuxEnemy_Wraith* pEnemy = ToWraith(pEntity);
	if (!pEnemy) continue;
	pEnemy->SetBurnDamageMinTime(afMinTime);
	pEnemy->SetBurnDamageMaxTime(afMaxTime);

	END_SET_PROPERTY
}

void __stdcall cLuxScriptHandler::SetWraithBurnDamageMinTime(string& asName, float afMinTime)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy, -1)

		cLuxEnemy_Wraith* pEnemy = ToWraith(pEntity);
	if (!pEnemy) continue;
	pEnemy->SetBurnDamageMinTime(afMinTime);

	END_SET_PROPERTY
}

void __stdcall cLuxScriptHandler::SetWraithBurnDamageMaxTime(string& asName, float afMaxTime)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy, -1)

		cLuxEnemy_Wraith* pEnemy = ToWraith(pEntity);
	if (!pEnemy) continue;
	pEnemy->SetBurnDamageMinTime(afMaxTime);

	END_SET_PROPERTY
}
//-----------------------------------------------------------------------
void __stdcall cLuxScriptHandler::SetWraithBurnDamage(string& asName, float afMinDamage, float afMaxDamage)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy, -1)

		cLuxEnemy_Wraith* pEnemy = ToWraith(pEntity);
	if (!pEnemy) continue;
	pEnemy->SetBurnDamageMin(afMinDamage);
	pEnemy->SetBurnDamageMax(afMaxDamage);

	END_SET_PROPERTY
}

void __stdcall cLuxScriptHandler::SetWraithMinBurnDamage(string& asName, float afMinDamage)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy, -1)

		cLuxEnemy_Wraith* pEnemy = ToWraith(pEntity);
	if (!pEnemy) continue;
	pEnemy->SetBurnDamageMin(afMinDamage);

	END_SET_PROPERTY
}

void __stdcall cLuxScriptHandler::SetWraithMaxBurnDamage(string& asName, float afMaxDamage)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy, -1)

		cLuxEnemy_Wraith* pEnemy = ToWraith(pEntity);
	if (!pEnemy) continue;
	pEnemy->SetBurnDamageMax(afMaxDamage);

	END_SET_PROPERTY
}
//-----------------------------------------------------------------------
void __stdcall cLuxScriptHandler::SetWraithAttackType(string& asName, string& asAttackType)
{
	eLuxAttackType attack = eLuxAttackType_LastEnum;
	tString sLowAttack = cString::ToLowerCase(asAttackType);

	if (sLowAttack == "archvile")			attack = eLuxAttackType_Archvile;
	else if (sLowAttack == "heatray")	attack = eLuxAttackType_HeatRay;
	else if (sLowAttack == "melee")	attack = eLuxAttackType_Melee;

	if (attack == eLuxEnemyMoveSpeed_LastEnum)
	{
		Error("Could not set Attack Type '%s' for enemy '%s'.  Attack Type  does not exist!\n", asAttackType.c_str(), asName.c_str());
		return;
	}

	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy, -1)

		cLuxEnemy_Wraith* pEnemy = ToWraith(pEntity);
	if (!pEnemy) continue;
	pEnemy->SetAttackType(attack);

	END_SET_PROPERTY
		
}
//-----------------------------------------------------------------------

bool __stdcall cLuxScriptHandler::GetWraithInStealthDashMode(string& asEnemyName)
{
	cLuxEnemy_Wraith* pEnemy = ToWraith(GetEntity(asEnemyName, eLuxEntityType_Enemy, -1));
	if (pEnemy == NULL)
	{
		Error("Can't find enemy '%s'!\n", asEnemyName.c_str());
		return false;
	}

	return pEnemy->GetIsInStealthMode();
}
//-----------------------------------------------------------------------

float __stdcall cLuxScriptHandler::GetWraithStealthDashNodesLeft(string& asEnemyName)
{
	cLuxEnemy_Wraith* pEnemy = ToWraith(GetEntity(asEnemyName, eLuxEntityType_Enemy, -1));
	if (pEnemy == NULL)
	{
		Error("Can't find enemy '%s'!\n", asEnemyName.c_str());
		return 0;
	}

	return pEnemy->GetStealthModeNodesLeft();
}


//-----------------------------------------------------------------------

string& __stdcall cLuxScriptHandler::GetEnemyStateName(string& asName)
{
	iLuxEnemy *pEnemy = ToEnemy(GetEntity(asName, eLuxEntityType_Enemy, -1));
	if( pEnemy==NULL )
	{
		Error("Can't find enemy '%s'!\n", asName.c_str());
		return gsScriptNull;
	}

	return pEnemy->GetCurrentEnemyStateName();
}

string& __stdcall cLuxScriptHandler::GetEnemyPreviousState(string& asName)
{
	iLuxEnemy* pEnemy = ToEnemy(GetEntity(asName, eLuxEntityType_Enemy, -1));
	if (pEnemy == NULL)
	{
		Error("Can't find enemy '%s'!\n", asName.c_str());
		return gsScriptNull;
	}

	return pEnemy->GetPreviousEnemyStateName();
}
//-----------------------------------------------------------------------
float __stdcall cLuxScriptHandler::GetEnemyHealth(string& asName)
{
	iLuxEnemy* pEnemy = ToEnemy(GetEntity(asName, eLuxEntityType_Enemy, -1));
	if (pEnemy == NULL)
	{
		Error("Can't find enemy '%s'!\n", asName.c_str());
		return 0;
	}

	return pEnemy->GetHealth();
}

void __stdcall cLuxScriptHandler::SetEnemyHealth(string& asName, float afHealth)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy, -1)

		iLuxEnemy* pEnemy = ToEnemy(pEntity);
	pEnemy->SetHealth(afHealth);

	END_SET_PROPERTY
}
//-----------------------------------------------------------------------
float __stdcall cLuxScriptHandler::GetEnemyHearVolume(string& asName)
{
	iLuxEnemy* pEnemy = ToEnemy(GetEntity(asName, eLuxEntityType_Enemy, -1));
	if (pEnemy == NULL)
	{
		Error("Can't find enemy '%s'!\n", asName.c_str());
		return 0;
	}

	return pEnemy->GetHearVolume();
}

void __stdcall cLuxScriptHandler::SetEnemyHearVolume(string& asName, float afHearVolume)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy, -1)

		iLuxEnemy* pEnemy = ToEnemy(pEntity);
	pEnemy->SetHearVolume(afHearVolume);

	END_SET_PROPERTY
}
//-----------------------------------------------------------------------
float __stdcall cLuxScriptHandler::GetEnemyRunSpeedMul(string& asName)
{
	iLuxEnemy* pEnemy = ToEnemy(GetEntity(asName, eLuxEntityType_Enemy, -1));
	if (pEnemy == NULL)
	{
		Error("Can't find enemy '%s'!\n", asName.c_str());
		return 0;
	}

	return pEnemy->GetRunSpeedMul();
}

void __stdcall cLuxScriptHandler::SetEnemyRunSpeedMul(string& asName, float afRunSpeedMul)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy, -1)

		iLuxEnemy* pEnemy = ToEnemy(pEntity);
	pEnemy->SetRunSpeedMul(afRunSpeedMul);

	END_SET_PROPERTY
}
//-----------------------------------------------------------------------
float __stdcall cLuxScriptHandler::GetEnemySearchTime(string& asName)
{
	iLuxEnemy* pEnemy = ToEnemy(GetEntity(asName, eLuxEntityType_Enemy, -1));
	if (pEnemy == NULL)
	{
		Error("Can't find enemy '%s'!\n", asName.c_str());
		return 0;
	}

	return pEnemy->GetSearchTime();
}

void __stdcall cLuxScriptHandler::SetEnemySearchTime(string& asName, float afSearchTime)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy, -1)

		iLuxEnemy* pEnemy = ToEnemy(pEntity);
	pEnemy->SetSearchTime(afSearchTime);

	END_SET_PROPERTY
}
//-----------------------------------------------------------------------
void __stdcall cLuxScriptHandler::SetEnemyRegenHealthSpeed(string& asName, float afRegenHealthSpeed)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy, -1)

		iLuxEnemy* pEnemy = ToEnemy(pEntity);
	pEnemy->SetRegenHealthSpeed(afRegenHealthSpeed);

	END_SET_PROPERTY
}

float __stdcall cLuxScriptHandler::GetEnemyRegenHealthSpeed(string& asName)
{
	iLuxEnemy* pEnemy = ToEnemy(GetEntity(asName, eLuxEntityType_Enemy, -1));
	if (pEnemy == NULL)
	{
		Error("Can't find enemy '%s'!\n", asName.c_str());
		return 0;
	}

	return pEnemy->GetRegenHealthSpeed();
}

void __stdcall cLuxScriptHandler::SetManPigFleeHealth(string& asName, float afFleeHealth)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy, -1)

		cLuxEnemy_ManPig* pEnemy = ToManPig(pEntity);
	if (!pEnemy) continue;
	pEnemy->SetFleeHealth(afFleeHealth);

	END_SET_PROPERTY
}

float __stdcall cLuxScriptHandler::GetManPigFleeHealth(string& asName)
{
	iLuxEnemy* pEnemy = ToEnemy(GetEntity(asName, eLuxEntityType_Enemy, -1));
	if (pEnemy == NULL)
	{
		Error("Can't find enemy '%s'!\n", asName.c_str());
		return 0;
	}

	return pEnemy->GetFleeHealth();
}

void __stdcall cLuxScriptHandler::SetManPigCanTrackTeleport(string& asName, bool abX)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Enemy, -1)

		cLuxEnemy_ManPig* pEnemy = ToManPig(pEntity);
	if (!pEnemy) continue;
	pEnemy->SetTrackTeleport(abX);

	END_SET_PROPERTY
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetMoveObjectState(string& asName, float afState)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Prop, eLuxPropType_MoveObject)

		cLuxProp_MoveObject *pMoveObj = ToMoveObject(pEntity);
		pMoveObj->MoveToState(afState);

	END_SET_PROPERTY
}

void __stdcall cLuxScriptHandler::SetMoveObjectStateExt(string& asName, float afState, float afAcc, float afMaxSpeed, float afSlowdownDist, bool abResetSpeed)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Prop, eLuxPropType_MoveObject)

		cLuxProp_MoveObject *pMoveObj = ToMoveObject(pEntity);
		pMoveObj->MoveToState(afState, afAcc, afMaxSpeed, afSlowdownDist, abResetSpeed);

	END_SET_PROPERTY
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetPropHealth(string& asName, float afHealth)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Prop, -1)

		iLuxProp *pProp = ToProp(pEntity);
		pProp->SetHealth(afHealth);	

	END_SET_PROPERTY
}

void __stdcall cLuxScriptHandler::AddPropHealth(string& asName, float afHealth)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Prop, -1)

		iLuxProp *pProp = ToProp(pEntity);
		pProp->SetHealth(pProp->GetHealth() + afHealth);	

	END_SET_PROPERTY
}

float __stdcall cLuxScriptHandler::GetPropHealth(string& asName)
{
	iLuxProp *pProp = ToProp(GetEntity(asName, eLuxEntityType_Prop, -1));
	if(pProp==NULL) return 0;

	return pProp->GetHealth();
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::ResetProp(string& asName)
{
	BEGIN_SET_PROPERTY(eLuxEntityType_Prop, -1)

		iLuxProp *pProp = ToProp(pEntity);
		pProp->ResetProperties();	

	END_SET_PROPERTY
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::PlayPropAnimation(string& asProp, string& asAnimation, float afFadeTime, bool abLoop, string &asCallback)
{
	tString asName = asProp;
	BEGIN_SET_PROPERTY(eLuxEntityType_Prop, -1)

		iLuxProp *pProp = ToProp(pEntity);
		pProp->PlayAnimation(asAnimation, afFadeTime, abLoop, asCallback);

	END_SET_PROPERTY
}

//-----------------------------------------------------------------------
void __stdcall cLuxScriptHandler::StopPropAnimation(string& asProp)
{
	tString asName = asProp;
	BEGIN_SET_PROPERTY(eLuxEntityType_Prop, -1)

		iLuxProp* pProp = ToProp(pEntity);
	pProp->StopAnimation();

	END_SET_PROPERTY
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::PlayCurrentAnimation(string& asProp, float afFadeTime, bool abLoop)
{
	tString asName = asProp;
	BEGIN_SET_PROPERTY(eLuxEntityType_Prop, -1)

		iLuxProp* pProp = ToProp(pEntity);
	pProp->PlayCurrentAnimation(afFadeTime, abLoop);

	END_SET_PROPERTY
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::PauseCurrentAnimation(string& asProp, float afFadeTime)
{
	tString asName = asProp;
	BEGIN_SET_PROPERTY(eLuxEntityType_Prop, -1)

		iLuxProp* pProp = ToProp(pEntity);
	pProp->PauseCurrentAnimation(afFadeTime);

	END_SET_PROPERTY
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetPropAnimationSpeed(string& asProp, float afSpeed)
{
	tString asName = asProp;
	BEGIN_SET_PROPERTY(eLuxEntityType_Prop, -1)

		iLuxProp* pProp = ToProp(pEntity);
	pProp->SetAnimationSpeed(afSpeed);

	END_SET_PROPERTY
}

void __stdcall cLuxScriptHandler::SetPropAnimationPosition(string& asProp, float afPos)
{
	tString asName = asProp;
	BEGIN_SET_PROPERTY(eLuxEntityType_Prop, -1)

		iLuxProp* pProp = ToProp(pEntity);
	pProp->SetAnimationPosition(afPos);

	END_SET_PROPERTY
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::AddEntityCollideCallback(string& asName, string& asChildName, string& asFunction, bool abDeleteOnCollide, int alStates)
{
	iLuxEntity* pChild = GetEntity(asChildName, eLuxEntityType_LastEnum, -1);
	if(pChild==NULL) return;
	if(asName == "Player")
	{
		gpBase->mpPlayer->AddCollideCallback(pChild, asFunction, abDeleteOnCollide,alStates);	
	}
	else
	{
		BEGIN_SET_PROPERTY(eLuxEntityType_LastEnum, -1)

			pEntity->AddCollideCallback(pChild, asFunction, abDeleteOnCollide, alStates);
		
		END_SET_PROPERTY
	}
}

void __stdcall cLuxScriptHandler::RemoveEntityCollideCallback(string& asName, string& asChildName)
{
	//Just to check so entity exist!
	GetEntity(asChildName, eLuxEntityType_LastEnum, -1);
	
	if(asName == "Player")
	{
		gpBase->mpPlayer->RemoveCollideCallback(asChildName);
	}
	else
	{
		BEGIN_SET_PROPERTY(eLuxEntityType_LastEnum, -1)

			pEntity->RemoveCollideCallback(asChildName);

		END_SET_PROPERTY
	}
}

//-----------------------------------------------------------------------

cVector3f VecToCoordSystem(iPhysicsBody *apBody, const cVector3f& avVec, const tString& asCoordSystem)
{
	if(cString::ToLowerCase(asCoordSystem)=="local")
	{
		return cMath::MatrixMul(apBody->GetLocalMatrix().GetRotation(),avVec);
	}
	else
	{
		return avVec;
	}
}

void __stdcall cLuxScriptHandler::AddPropForce(string& asName, float afX, float afY, float afZ, string& asCoordSystem)
{
	iLuxProp* pProp = ToProp(GetEntity(asName, eLuxEntityType_Prop, -1));
	if(pProp==NULL) return;
	cVector3f vVec(afX, afY, afZ);
    
	for(int i=0; i<pProp->GetBodyNum(); ++i)
	{
		iPhysicsBody *pBody = pProp->GetBody(i);
        pBody->AddForce(VecToCoordSystem(pBody, vVec, asCoordSystem));
	}
}

void __stdcall cLuxScriptHandler::AddPropImpulse(string& asName, float afX, float afY, float afZ, string& asCoordSystem)
{
	iLuxProp* pProp = ToProp(GetEntity(asName, eLuxEntityType_Prop, -1));
	if(pProp==NULL) return;
	cVector3f vVec(afX, afY, afZ);

	for(int i=0; i<pProp->GetBodyNum(); ++i)
	{
		iPhysicsBody *pBody = pProp->GetBody(i);
		pBody->AddImpulse(VecToCoordSystem(pBody, vVec, asCoordSystem));
	}

}

void __stdcall cLuxScriptHandler::AddBodyForce(string& asName, float afX, float afY, float afZ, string& asCoordSystem)
{
	cLuxMap *pMap = gpBase->mpMapHandler->GetCurrentMap();
	iPhysicsBody *pBody = pMap->GetPhysicsWorld()->GetBody(asName);
	if(pBody==NULL){
		Error("Could not find body '%s'!\n", asName.c_str());
		return;
	}
	cVector3f vVec(afX, afY, afZ);

	pBody->AddForce(VecToCoordSystem(pBody, vVec, asCoordSystem));

}

void __stdcall cLuxScriptHandler::AddBodyImpulse(string& asName, float afX, float afY, float afZ, string& asCoordSystem)
{
	cLuxMap *pMap = gpBase->mpMapHandler->GetCurrentMap();
	iPhysicsBody *pBody = pMap->GetPhysicsWorld()->GetBody(asName);
	if(pBody==NULL){
		Error("Could not find body '%s'!\n", asName.c_str());
		return;
	}
	cVector3f vVec(afX, afY, afZ);
	
	pBody->AddImpulse(VecToCoordSystem(pBody, vVec, asCoordSystem));
}

void __stdcall cLuxScriptHandler::BreakJoint(string& asName)
{
	cLuxMap *pMap = gpBase->mpMapHandler->GetCurrentMap();
	iPhysicsJoint *pJoint = pMap->GetPhysicsWorld()->GetJoint(asName);
	if(pJoint == NULL)
	{
		Error("Could not find joint '%s'!\n", asName.c_str());
		return;
	}

	pJoint->Break();
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::SetBodyMass(string& asName, float afMass)
{
	cLuxMap *pMap = gpBase->mpMapHandler->GetCurrentMap();
	iPhysicsBody *pBody = pMap->GetPhysicsWorld()->GetBody(asName);
	if(pBody==NULL){
		Error("Could not find body '%s'!\n", asName.c_str());
		return;
	}

	pBody->SetMass(afMass);
}

//-----------------------------------------------------------------------

float __stdcall cLuxScriptHandler::GetBodyMass(string& asName)
{
	cLuxMap *pMap = gpBase->mpMapHandler->GetCurrentMap();
	iPhysicsBody *pBody = pMap->GetPhysicsWorld()->GetBody(asName);
	if(pBody==NULL){
		Error("Could not find body '%s'!\n", asName.c_str());
		return 0;
	}

	return pBody->GetMass();
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::InteractConnectPropWithRope(	string& asName, string& asPropName, string& asRopeName, bool abInteractOnly, 
																float afSpeedMul,float afMinSpeed, float afMaxSpeed,
																bool abInvert, int alStatesUsed)
{
	cLuxMap *pMap = gpBase->mpMapHandler->GetCurrentMap();
	iPhysicsWorld *pPhysicsWorld = pMap->GetPhysicsWorld();

	iLuxProp *pProp = ToProp(GetEntity(asPropName, eLuxEntityType_Prop, -1));
	if(pProp==NULL) return;
	
    iPhysicsRope *pRope = pPhysicsWorld->GetRope(asRopeName);
	if(pRope == NULL)
	{
		Error("Could not find rope '%s'!\n", asRopeName.c_str());
		return;
	}
	
	cLuxInteractConnection_Rope *pConnection = hplNew(cLuxInteractConnection_Rope, (asName, pProp,pRope, afSpeedMul,
														-cMath::Abs(afMinSpeed), cMath::Abs(afMaxSpeed),
														abInvert, alStatesUsed));
	pConnection->SetInteractionOnly(abInteractOnly);

	pProp->AddInteractConnection(pConnection);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::InteractConnectPropWithMoveObject(	string& asName, string& asPropName, string& asMoveObjectName, bool abInteractOnly,
																		bool abInvert, int alStatesUsed)
{
	iLuxProp *pProp = ToProp(GetEntity(asPropName, eLuxEntityType_Prop, -1));
	if(pProp==NULL) return;

	cLuxProp_MoveObject *pSwingDoor = ToMoveObject(GetEntity(asMoveObjectName, eLuxEntityType_Prop, eLuxPropType_MoveObject));
	if(pSwingDoor==NULL) return;

	cLuxInteractConnection_MoveObject *pConnection = hplNew(cLuxInteractConnection_MoveObject, (asName, pProp, pSwingDoor, abInvert, alStatesUsed));
	pConnection->SetInteractionOnly(abInteractOnly);

	pProp->AddInteractConnection(pConnection);
}

//-----------------------------------------------------------------------

void __stdcall cLuxScriptHandler::ConnectEntities(string& asName, string& asMainEntity, string& asConnectEntity, bool abInvertStateSent, int alStatesUsed, string& asCallbackFunc)
{
	iLuxEntity *pMainEntity = GetEntity(asMainEntity, eLuxEntityType_LastEnum, -1);
	if(pMainEntity==NULL) return;	

	iLuxEntity *pConnectEntity = GetEntity(asConnectEntity, eLuxEntityType_LastEnum, -1);
	if(pConnectEntity==NULL) return;

	pMainEntity->AddConnection(asName,pConnectEntity, abInvertStateSent, alStatesUsed, asCallbackFunc);
}

//-----------------------------------------------------------------------

/*void __stdcall cLuxScriptHandler::CreateRope(string& asName, 
											string& asStartArea, string& asEndArea, 
											string& asStartBody, string& asEndBody,
											float afMinTotalLength, float afMaxTotalLength,
											float afSegmentLength, float afDamping,
											float afStrength, float afStiffness,
											string& asMaterial, float afRadius,
											float afLengthTileAmount, float afLengthTileSize,
											string& asSound,
											float afSoundStartSpeed, float afSoundStopSpeed,
											bool abAutoMove, float afAutoMoveAcc, float afAutoMoveMaxSpeed)
{
	iLuxArea *pStartArea =	ToArea(GetEntity(asStartArea, eLuxEntityType_Area, -1));
	iLuxArea *pEndArea =	ToArea(GetEntity(asEndArea, eLuxEntityType_Area, -1));

	if(pStartArea==NULL || pEndArea==NULL) return;

	cLuxMap *pMap = gpBase->mpMapHandler->GetCurrentMap();
	iPhysicsWorld *pPhysicsWorld = pMap->GetPhysicsWorld();
	cResources *pResources = gpBase->mpEngine->GetResources();

	iPhysicsRope *pRope = pPhysicsWorld->CreateRope(asName, pStartArea->GetPosition(), pEndArea->GetPosition());

	pRope->SetMinTotalLength(afMinTotalLength);
	pRope->SetMaxTotalLength(afMaxTotalLength);
	pRope->SetSegmentLength(afSegmentLength);
	pRope->SetDamping(afDamping);
	pRope->SetStrength(afStrength);
	pRope->SetStiffness(afStiffness);
	pRope->SetMotorSound(asSound);
	pRope->SetMotorSoundStartSpeed(afSoundStartSpeed);
	pRope->SetMotorSoundStopSpeed(afSoundStopSpeed);
	pRope->SetAutoMoveActive(abAutoMove);
	pRope->SetAutoMoveAcc(afAutoMoveAcc);
	pRope->SetAutoMoveMaxSpeed(afAutoMoveMaxSpeed);

	pRope->SetUniqueID(mlRopeIdCount++);

	//Log("Total Length: %f\n", pRope->GetTotalLength());
	
	//////////////////////////
	//Create graphical entity

	int lMaxSegments = (int)(afMaxTotalLength / afSegmentLength)+4;
	cRopeEntity *pRopeGfx = pMap->GetWorld()->CreateRopeEntity(asName, pRope, lMaxSegments);

	pRopeGfx->SetMaterial(pResources->GetMaterialManager()->CreateMaterial(asMaterial));
	pRopeGfx->SetRadius(afRadius);
	pRopeGfx->SetLengthTileAmount(afLengthTileAmount);
	pRopeGfx->SetLengthTileSize(afLengthTileSize);

	pRopeGfx->SetUniqueID(mlRopeIdCount++);

	//////////////////////////
	//Start body attachment
	if(asStartBody != "")
	{
		iPhysicsBody *pBody = pPhysicsWorld->GetBody(asStartBody);
		if(pBody!=NULL)
		{
			pRope->SetAttachedStartBody(pBody);
			pRope->GetStartParticle()->SetInvMass(1.0f);
		}
		else
		{
			Warning("Could not find body '%s'", asStartBody.c_str());
		}
	}

	//////////////////////////
	//End body attachment
	if(asEndBody != "")
	{
		iPhysicsBody *pBody = pPhysicsWorld->GetBody(asEndBody);
		if(pBody!=NULL)
		{
			pRope->SetAttachedEndBody(pBody);
		}
		else
		{
			Warning("Could not find body '%s'", asEndBody.c_str());
		}
	}

	
}*/

//-----------------------------------------------------------------------

float __stdcall cLuxScriptHandler::ScriptSin(float afX)
{
	return sinf(afX);
}

float __stdcall cLuxScriptHandler::ScriptCos(float afX)
{
	return cosf(afX);
}

float __stdcall cLuxScriptHandler::ScriptTan(float afX)
{
	return tanf(afX);
}

float __stdcall cLuxScriptHandler::ScriptAsin(float afX)
{
	return asinf(afX);
}

float __stdcall cLuxScriptHandler::ScriptAcos(float afX)
{
	return acosf(afX);
}

float __stdcall cLuxScriptHandler::ScriptAtan(float afX)
{
	return atanf(afX);
}

float __stdcall cLuxScriptHandler::ScriptAtan2(float afX, float afY)
{
	return atan2f(afX, afY);
}

float __stdcall cLuxScriptHandler::ScriptSqrt(float afX)
{
	return sqrtf(afX);
}

float __stdcall cLuxScriptHandler::ScriptPow(float afBase, float afExp)
{
	return powf(afBase, afExp);
}

float __stdcall cLuxScriptHandler::ScriptMin(float afA, float afB)
{
	return cMath::Min(afA, afB);
}

float __stdcall cLuxScriptHandler::ScriptMax(float afA, float afB)
{
	return cMath::Max(afA, afB);
}

float __stdcall cLuxScriptHandler::ScriptClamp(float afX, float afMin, float afMax)
{
	return cMath::Clamp(afX, afMin, afMax);
}

float __stdcall cLuxScriptHandler::ScriptAbs(float afX)
{
	return cMath::Abs(afX);
}

//-----------------------------------------------------------------------

int __stdcall cLuxScriptHandler::ScriptStringToInt(string& asString)
{
	return cString::ToInt(asString.c_str(), 0);
}

float __stdcall cLuxScriptHandler::ScriptStringToFloat(string& asString)
{
	return cString::ToFloat(asString.c_str(), 0);
}

bool __stdcall cLuxScriptHandler::ScriptStringToBool(string& asString)
{
	return cString::ToBool(asString.c_str(), false);
}


//-----------------------------------------------------------------------


