/*
 * Copyright © 2011-2021 Frictional Games
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

#include "LuxEnemy_Wraith.h" 

#include "LuxEnemyMover.h"
#include "LuxEnemyPathfinder.h"

#include "LuxMap.h"
#include "LuxMapHelper.h"
#include "LuxMapHandler.h"
#include "LuxMusicHandler.h"
#include "LuxDebugHandler.h"
#include "LuxGlobalDataHandler.h"

#include "LuxPlayer.h"
#include "LuxPlayerHelpers.h"
#include "LuxEffectHandler.h"
#include "LuxProp_Object.h"

 //////////////////////////////////////////////////////////////////////////
 // LOADER
 //////////////////////////////////////////////////////////////////////////

 //-----------------------------------------------------------------------
cLuxEnemyLoader_Wraith::cLuxEnemyLoader_Wraith(const tString& asName) : iLuxEnemyLoader(asName)
{
}

//-----------------------------------------------------------------------

iLuxEnemy* cLuxEnemyLoader_Wraith::CreateEnemy(const tString& asName, int alID, cLuxMap* apMap)
{
	return hplNew(cLuxEnemy_Wraith, (asName, alID, apMap));
}

//-----------------------------------------------------------------------

void cLuxEnemyLoader_Wraith::LoadVariables(iLuxEnemy* apEnemy, cXmlElement* apRootElem)
{
	cLuxEnemy_Wraith* pWraith = static_cast<cLuxEnemy_Wraith*>(apEnemy);

	pWraith->mfGroggyDamageCount = GetVarFloat("GroggyDamageCount", 0);
	pWraith->mfAlertToHuntDistance = GetVarFloat("AlertToHuntDistance", 0);
	pWraith->mfAlertToInstantHuntDistance = GetVarFloat("AlertToInstantHuntDistance", 0);
	pWraith->mfIncreaseAlertSpeedDistance = GetVarFloat("IncreaseAlertSpeedDistance", 0);
	pWraith->mfIncreasedAlertSpeedMul = GetVarFloat("IncreasedAlertSpeedMul", 0);
	pWraith->mfAlertRunTowardsToHuntLimit = GetVarFloat("AlertRunTowardsToHuntLimit", 0);
	pWraith->mfAlertRunTowardsCheckDistance = GetVarFloat("AlertRunTowardsCheckDistance", 0);

	pWraith->mfRangedAttackDistance = GetVarFloat("RangedAttackDistance", 12);
	pWraith->mvRangedAttackDamage = GetVarVector2f("RangedAttackDamage",0);
	pWraith->mvRangedAttackForce = GetVarVector2f("RangedAttackForce", 0);
	pWraith->mvRangedDamageSize = GetVarVector3f("RangedDamageSize", 0);
	pWraith->mlRangedAttackStrength = GetVarInt("RangedAttackStrength", 1);
	pWraith->msRangedAttackHitSound = GetVarString("RangedAttackHitSound", "");
	pWraith->msRangedAttackDisplayType = GetVarString("RangedDamageType","BloodSplat");

	pWraith->msHeatRaySpotlight = GetVarString("HeatRaySpotlight", "");
	pWraith->mHeatRaySpotlightColor = GetVarColor("HeatRayAttackLightColor", cColor(1, 0, 0, 1));
	pWraith->mHeatRaySpotlightColorIdle = GetVarColor("HeatRayAttackLightColorIdle", cColor(0.42, 0.557, 0.627));
	pWraith->mHeatRaySpotlightColorAlert = GetVarColor("HeatRayAttackLightColorAlert", cColor(0.7, 0.56, 0.42));
	pWraith->mHeatRaySpotlightColorHunt = GetVarColor("HeatRayAttackLightColorHunt", cColor(0.7, 0.175, 0.21));
	pWraith->mfHRSpotlightFOVAlert = GetVarFloat("HeatRaySpotlightFOVAlert", 120.0f);
	pWraith->mfHRSpotlightFOVPatrol = GetVarFloat("HeatRaySpotlightFOVPatrol", 60.f);

	pWraith->msFlameLightName = GetVarString("FlameLightName", "");
	pWraith->mAVFlameLightColor = GetVarColor("FlameLightColor", cColor(0.527, 0.235, 0, 1));
	pWraith->mfAVFlameLightBrightness = GetVarFloat("FlameLightBrightness", 2.5f);
	pWraith->mfAVFlameLightRadius = GetVarFloat("FlameLightRadius", 6.5f);
	pWraith->mfAVFlameLightBrightness = GetVarFloat("FlameLightBrightness", 2.5f);

	pWraith->mTeleportLightColor = GetVarColor("TeleportLightColor", cColor(0.4, 0.9, 1.0));
	pWraith->mfTeleportLightBrightness = GetVarFloat("TeleportLightBrightness", 17.0f);
	pWraith->mfTeleportLightRadius = GetVarFloat("TeleportLightRadius", 6.5f);

	pWraith->mAVFlameLightColorFlickerOffColor = GetVarColor("FlameLightOffColor", cColor(0.547, 0.203, 0.017, 1));
	pWraith->mfAVFlameLightFlickerOffRadius = GetVarFloat("FlameLightOffRadius", 7.5f);
	pWraith->mfAVFlameLightFlickerOnMinLength = GetVarFloat("FlameLightOnMinLength", 0.0f);
	pWraith->mfAVFlameLightFlickerOnMaxLength = GetVarFloat("FlameLightOnMaxLength", 0.2f);
	pWraith->mfAVFlameLightFlickerOffMinLength = GetVarFloat("FlameLightOffMinLength", 0.0f);
	pWraith->mfAVFlameLightFlickerOffMaxLength = GetVarFloat("FlameLightOffMaxLength", 0.05f);
	pWraith->mfAVFlameLightFlickerOnFadeMinLength = GetVarFloat("FlameLightOnFadeMinLength", 0.1f);
	pWraith->mfAVFlameLightFlickerOnFadeMaxLength = GetVarFloat("FlameLightOnFadeMaxLength", 0.45f);
	pWraith->mfAVFlameLightFlickerOffFadeMinLength = GetVarFloat("FlameLightOffFadeMinLength", 0.035f);
	pWraith->mfAVFlameLightFlickerOffFadeMaxLength = GetVarFloat("FlameLightOffFadeMaxLength", 0.5f);
	pWraith->mfFlameLightInitFadeTime = GetVarFloat("FlameLightFadeTime", 4.0f);
	
	pWraith->msBurnDamageSound = GetVarString("BurnDamageSound");

	pWraith->msAttackMeleePS = GetVarString("AttackMeleePS", "");

	pWraith->msTeleportDashPS_Teleport = GetVarString("TeleportDashPS_Teleport", "");
	pWraith->msTeleportDashPS_Trail = GetVarString("TeleportDashPS_Trail", "");

	pWraith->msArchvilePS_AttackStart = GetVarString("ArchvilePS_AttackStart", "");
	pWraith->mvArchvilePS_AttackStartOffset = GetVarVector3f("ArchvilePS_AttackStartOffset", cVector3f(0, 0.25, 0));
	pWraith->msArchvilePS_AttackEnd = GetVarString("ArchvilePS_AttackEnd", "");
	pWraith->mvArchvilePS_AttackEndOffset = GetVarVector3f("ArchvilePS_AttackEndOffset", cVector3f(0, 0.25, 0));
	pWraith->msProjectilePS_AttackStart = GetVarString("ProjectilePS_AttackStart", "");
	pWraith->msProjectilePS_AttackEnd = GetVarString("ProjectilePS_AttackEnd", "");

	pWraith->mfHuntPauseMinTime = GetVarFloat("HuntPauseMinTime", 0);
	pWraith->mfHuntPauseMaxTime = GetVarFloat("HuntPauseMaxTime", 0);

	//pWraith->msAttachmentBone = GetVarString("AttachmentBone");

	pWraith->msIdleStillLoop = GetVarString("IdleStillSoundLoop");
	pWraith->msIdleMoveLoop = GetVarString("IdleMoveSoundLoop");
	pWraith->msStealthLoop = GetVarString("StealthSoundLoop");
	pWraith->msSearchLoop = GetVarString("SearchSoundLoop");
	pWraith->msChaseLoop = GetVarString("ChaseSoundLoop");
	pWraith->msSpotlightLoop = GetVarString("SpotlightSoundLoop");
	pWraith->msTeleportingLoop = GetVarString("TeleportingSoundLoop");
	pWraith->msHeatRayBurnLoop = GetVarString("HeatRaySoundLoop");
	
	pWraith->msGiveUpNoticeSound = GetVarString("GiveUpNoticeSound");
	pWraith->msEnabledSound = GetVarString("EnabledSound");
	pWraith->msTeleportSound = GetVarString("TeleportSound");
	pWraith->msChaseSound = GetVarString("ChaseSound");

	pWraith->mfIdleExtraTimeMin = GetVarFloat("IdleExtraTimeMin", 2);
	pWraith->mfIdleExtraTimeMax = GetVarFloat("IdleExtraTimeMax", 6);
	pWraith->mlIdleExtraNum = GetVarInt("IdleExtraNum", 3);

	pWraith->mbIsActualWraith = GetVarBool("IsActualWraith", true);

	gpBase->PreloadSound(pWraith->msGiveUpNoticeSound);
	gpBase->PreloadSound(pWraith->msEnabledSound);
	gpBase->PreloadSound(pWraith->msTeleportSound);
	gpBase->PreloadSound(pWraith->msChaseSound);

	gpBase->PreloadSound(pWraith->msIdleStillLoop);
	gpBase->PreloadSound(pWraith->msIdleMoveLoop);
	gpBase->PreloadSound(pWraith->msStealthLoop);
	gpBase->PreloadSound(pWraith->msChaseLoop);
	gpBase->PreloadSound(pWraith->msSpotlightLoop);
	gpBase->PreloadSound(pWraith->msTeleportingLoop);
	gpBase->PreloadSound(pWraith->msHeatRayBurnLoop);

	gpBase->PreloadParticleSystem(pWraith->msArchvilePS_AttackStart);
	gpBase->PreloadParticleSystem(pWraith->msArchvilePS_AttackEnd);
	gpBase->PreloadParticleSystem(pWraith->msTeleportDashPS_Teleport);
	gpBase->PreloadParticleSystem(pWraith->msTeleportDashPS_Trail);
	gpBase->PreloadParticleSystem(pWraith->msAttackMeleePS);
	gpBase->PreloadParticleSystem(pWraith->msProjectilePS_AttackStart);
	gpBase->PreloadParticleSystem(pWraith->msProjectilePS_AttackEnd);

	//pWraith->mbIsActualWraith = GetVarBool("IsWraith", true);

	//LoadAttackDamageData("Ranged", &pWraith->mRangedAttackDamage);
}
//-----------------------------------------------------------------------

static eLuxAttackType ToAttackType(const tString& asStr)
{
	if (asStr == "Archvile") return eLuxAttackType_Archvile;
	if (asStr == "HeatRay") return eLuxAttackType_HeatRay;
	if (asStr == "Projectile") return eLuxAttackType_Projectile;
	if (asStr == "Meele") return eLuxAttackType_Melee;

	Error("eLuxAttackType '%s' does not exist, falling back to melee!\n", asStr.c_str());
	return eLuxAttackType_Melee;
}

static eLuxEnemyMoveSpeed ToMoveSpeed(const tString& asStr)
{
	if (asStr == "Run") return eLuxEnemyMoveSpeed_Run;
	if (asStr == "Walk") return eLuxEnemyMoveSpeed_Walk;

	Error("eLuxEnemyMoveSpeed '%s' does not exist, falling back to walk!\n", asStr.c_str());
	return eLuxEnemyMoveSpeed_Walk;
}



void cLuxEnemyLoader_Wraith::LoadInstanceVariables(iLuxEnemy* apEnemy, cResourceVarsObject* apInstanceVars)
{
	cLuxEnemy_Wraith* pWraith = static_cast<cLuxEnemy_Wraith*>(apEnemy);

	pWraith->mbThreatenOnAlert = apInstanceVars->GetVarBool("ThreatenOnAlert", false);
	//pWraith->mbFlyMode = apInstanceVars->GetVarBool("FlyMode", false);
	pWraith->mbStealthDashMode = apInstanceVars->GetVarBool("StealthDashMode", false);
	pWraith->mbAllowedToDashAtWill = apInstanceVars->GetVarBool("StealthDashAtWill", false);
	pWraith->mlStealthDashNodesLengthStart = apInstanceVars->GetVarInt("StealthDashNodesLength", 0);
	pWraith->mfEnterStealthDashNodeDistance = apInstanceVars->GetVarFloat("EnterStealthDashDistance", 15.5f);
	pWraith->mfExitStealthDashNodeDistance = apInstanceVars->GetVarFloat("ExitStealthDashDistance", 5.0f);
	pWraith->mPatrolMoveSpeed = ToMoveSpeed(apInstanceVars->GetVarString("PatrolMoveSpeed", "Walk"));
	pWraith->mbAllowZeroWaitTime = apInstanceVars->GetVarBool("AllowZeroNodeWaitTimes", false);
	pWraith->mfDamageMul = apInstanceVars->GetVarFloat("DamageMul", 1.0f);
	pWraith->mAttackType = ToAttackType(apInstanceVars->GetVarString("AttackType", "Melee"));
	pWraith->mfHeatRayDamageSpeed = apInstanceVars->GetVarFloat("HeatRayDamageSpeed", 5.0f);
	pWraith->mfLightLevelDamageStart = apInstanceVars->GetVarFloat("HeatRayLightDamageStart", 0.75f);
	pWraith->mfBurnDamageMinTime = apInstanceVars->GetVarFloat("BurnDamageMinTime", 2.5f);
	pWraith->mfBurnDamageMaxTime = apInstanceVars->GetVarFloat("BurnDamageMaxTime", 5.0f);
	pWraith->mfMinBurnDamage = apInstanceVars->GetVarFloat("MinBurnDamage", 5.0f);
	pWraith->mfMaxBurnDamage = apInstanceVars->GetVarFloat("MaxBurnDamage", 10.0f);
	pWraith->mbCanMeele = apInstanceVars->GetVarBool("CanMelee", false);
	pWraith->mbPlayActivateSound = apInstanceVars->GetVarBool("PlayActivateSound", true);
	pWraith->mfLanternSensitivity = apInstanceVars->GetVarFloat("LanternSensitivity", 1.0f);
	pWraith->mfHuntPauseTimeMul = apInstanceVars->GetVarFloat("HuntPauseTimeMul", 1.0f);
	//pWraith->msAttachmentBone = GetVarString("AttachmentBone");
	//pWraith->msAttachEntity = apInstanceVars->GetVarString("AttachmentEntity", "");
	//pWraith->mvAttachEntityPos = apInstanceVars->GetVarVector3f("AttachEntityPosition", 0);
	//pWraith->mvAttachEntityRot = apInstanceVars->GetVarVector3f("AttachEntityRotation", 0);
}

//-----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// CONSTRUCTORS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

cLuxEnemy_Wraith::cLuxEnemy_Wraith(const tString& asName, int alID, cLuxMap* apMap) : iLuxEnemy(asName, alID, apMap, eLuxEnemyType_Wraith)
{
	mCurrentMoveType = eLuxEnemyMoveType_Normal;

	mfWaitTime = 0;
	mfAlertRunTowardsCount = 0;

	mbIsActualWraith = false;

	mbAlignEntityWithGroundRay = true;

	mbPathReversed = false;
	mbThreatenOnAlert = false;
	mbFlyMode = false;
	mbFlyModeSetup = false;
	mbStealthDashMode = false;
	mbIsInStealthDashMode = false;
	mbAllowedToDashAtWill = false;

	mbHeatRayLightConnectionSetup = false;
	mbRangedAttackParticleSetup = false;
	mbStealthDashSetup = false;
	mbSpotlightSetup = false;
	mfSpotlightDefaultBrightness = 1;

	mbCanMeele = true;
	mAttackType = eLuxAttackType_Melee;

	mbHeatRaySoundIsPlaying = false;
	
	mbIsInArchvileAttack = false;
	msFlameLightName = "";
	mbFlameLightIsOn = false;
	mfFlameLightFadeTime = 0;
	mfFlameLightInitFadeTime = 4.0f;
	mbFlameLightFadeStarted = false;

	mfDamageMul = 1.0f;
	mfRunSpeedMul = 1.0f;

	mfDefaultMass = 50.0f;

	mlStealthDashNodesLengthStart = 0;
	mlStealthDashNodesLength = 0;
	mfEnterStealthDashNodeDistance = 15.5f;
	mfExitStealthDashNodeDistance = 5.0f;
	
	mpTeleportLight = NULL;
	mpFlameLight = NULL;
	mpHeatRayLight = NULL; 
	//mpHeatRayBurnSound = NULL;
	//mlHeatRayBurnSoundId = -1;
	mfHeatRayRadius = 0;
	mfHeatRaySpotlightFOV = 0;
	mfHeatRaySpotlightAspect = 0;
	mfStartHeatRayDamage = 0.0f;
	mfBurnDamageCountdown = cMath::RandRectf(2.5f,5.0f);
	mfHeatRayDamageSpeed = 5.0f;
	mfBurnDamageMinTime = 2.5f;
	mfBurnDamageMaxTime = 5.0f;
	mfMinBurnDamage = 5.0f;
	mfMaxBurnDamage = 10.0f;
	mfLightLevelDamageStart = 0.75f;

	mAVFlameLightColor = cColor(0.527, 0.235, 0, 1);
	mAVFlameLightColorFlickerOffColor = cColor(0.547, 0.203, 0.017, 1);

	mfAVFlameLightBrightness = 2.5f;

	mfAVFlameLightRadius = 6.5f;
	mfAVFlameLightFlickerOffRadius = 7.5f;
	mfAVFlameLightFlickerOnMinLength = 0;
	mfAVFlameLightFlickerOnMaxLength = 0.2f;
	mfAVFlameLightFlickerOffMinLength = 0;
	mfAVFlameLightFlickerOffMaxLength = 0.05f;

	mfAVFlameLightFlickerOnFadeMinLength = 0.1f;
	mfAVFlameLightFlickerOnFadeMaxLength = 0.45f;
	mfAVFlameLightFlickerOffFadeMinLength = 0.035f;
	mfAVFlameLightFlickerOffFadeMaxLength = 0.5f;

	mfHRSpotlightFOVAlert = 120.0f;
	mfHRSpotlightFOVPatrol = 60.0f;

	mTeleportLightColor = cColor(0.4, 0.9, 1.0);
	mfTeleportLightBrightness = 17.0f;
	mfTeleportLightRadius = 6.5f;

	mCurrentSpotColor = cColor(0, 0);
	mPrevSpotColor = cColor(0, 0);

	for (int i = 0; i < eLuxEnemyMoveType_LastEnum; ++i)
	{
		eLuxEnemyPoseType pose = eLuxEnemyPoseType_Biped;
		msIdleAnimationName[i][pose] = "IdleBiped";
		msWalkAnimationName[i][pose] = "WalkBiped";
		msRunAnimationName[i][pose] = "RunBiped";
	}

	pTeleportDashPS_Trail = NULL;
	pTeleportDashPS_Teleport = NULL;

	pArchvilePS_AttackStart = NULL;
	mvArchvilePS_AttackStartOffset = cVector3f(0, 0.25, 0);
	pArchvilePS_AttackEnd = NULL;
	mvArchvilePS_AttackEndOffset = cVector3f(0, 0.25, 0);
	pProjectilePS_AttackStart = NULL;
	pProjectilePS_AttackEnd = NULL;
	pAttackMeleePS = NULL;
}

cLuxEnemy_Wraith::~cLuxEnemy_Wraith()
{
	
}
//-----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

void cLuxEnemy_Wraith::OnSetupAfterLoad(cWorld* apWorld)
{
	////////////////////////////////////////
	// Wall avoidance
	mpMover->SetupWallAvoidance(0.9f, 8, 4);
	mpMover->SetWallAvoidanceActive(true);
	/////////////////////////////////////
	
	//SetUpAttachmentEntity();
}
//-----------------------------------------------------------------------

void cLuxEnemy_Wraith::UpdateEnemySpecific(float afTimeStep)
{
	cVector3f mvEnemyPos = mpCharBody->GetPosition();

	UpdateSoundLoops(afTimeStep);

	if (mbHeatRaySoundIsPlaying) UpdateHeatRaySound(afTimeStep);

	if (mbHeatRayLightConnectionSetup == false)
	{
		SetUpHeatRayConnection();
		///mbHeatRayLightConnectionSetup = true;
	}

	if (mpFlameLight == NULL)
	{
		CreateArchvileFlameLight();
	}


	UpdateAttack(afTimeStep);

	if (mbStealthDashSetup == false)
	{
		if (mbStealthDashMode)
		{
			SetToStealthMode(mbStealthDashMode); 
		}
		mbStealthDashSetup = true;
	}

	if (mbIsInStealthDashMode)
	{
		if(pTeleportDashPS_Trail && mpWorld->ParticleSystemExists(pTeleportDashPS_Trail)) pTeleportDashPS_Trail->SetPosition(mvEnemyPos);
	}

	
	if (mpTeleportLight == NULL)
	{
		TeleportLightCommand("CreateTeleportLight");
	}

}
//-----------------------------------------------------------------------

void cLuxEnemy_Wraith::OnAfterWorldLoad()
{
	mvDefaultLightColors.clear();
	for (size_t i = 0; i < mvLights.size(); ++i)
	{
		mvDefaultLightColors.push_back(mvLights[i]->GetDiffuseColor());
	}
}
//-----------------------------------------------------------------------
void cLuxEnemy_Wraith::SetToFlyMode(bool abX)
{
	if (abX == mbFlyMode) return;

	mbFlyMode = abX;

	if (mbFlyMode == true)
	{

		
		mpCharBody->SetGravityActive(false);
		mpCharBody->SetForceVelocity(cVector3f(0));
		mpCharBody->SetDeaccelerateMoveSpeedInAir(true);
		SetupFlyWallAvoidance();
	}
	else
	{
		
		mpCharBody->SetGravityActive(true);
		mpCharBody->SetDeaccelerateMoveSpeedInAir(false);
		SetupGroundWallAvoidance();
	}
}
//-----------------------------------------------------------------------
void cLuxEnemy_Wraith::SetToStealthMode(bool abX)
{
	if(abX)
	{
		StartTeleportDash();
	}
	else
	{
		StopTeleportDash();
	}
	
}
//-----------------------------------------------------------------------
void cLuxEnemy_Wraith::SetAttackType(eLuxAttackType aAttackType)
{
	mAttackType = aAttackType;

	if (aAttackType == eLuxAttackType_HeatRay)
	{
		SetUpHeatRayConnection();
	}
}

void cLuxEnemy_Wraith::SetPatrolSpeed(eLuxEnemyMoveSpeed aSpeedType)
{
	if (mPatrolMoveSpeed == aSpeedType) return;

	mPatrolMoveSpeed = aSpeedType;

	mbPatrolMoveSpeedChanged = true;

	mpMover->mMoveState = eLuxEnemyMoveState_LastEnum;
	mpMover->UpdateMoveAnimation(0.001f);
}


//-----------------------------------------------------------------------
bool cLuxEnemy_Wraith::StateEventImplement(int alState, eLuxEnemyStateEvent aEvent, cLuxStateMessage* apMessage)
{
	kLuxBeginStateMachine

		////////////////////////////////
		// Default
		////////////////////////////////

		//------------------------------

		kLuxOnMessage(eLuxEnemyMessage_Reset)

			gpBase->mpMusicHandler->RemoveEnemy(eLuxEnemyMusic_Attack, this);

			gpBase->mpPlayer->RemoveTerrorEnemy(this);

			ChangeState(eLuxEnemyState_Wait);

		//------------------------------
		kLuxOnMessage(eLuxEnemyMessage_PlayerDetected)
			ChangeState(eLuxEnemyState_Alert);
		//------------------------------
		kLuxOnMessage(eLuxEnemyMessage_TakeHit)
			mfDamageCount += apMessage->mfCustomValue;
			if (mfDamageCount > mfGroggyDamageCount)
			{
				ChangeState(eLuxEnemyState_Hurt);
				mfDamageCount = 0;
			}

	//------------------------------

		//If enemy is out of range (having been in, then turn him off)
		kLuxOnMessage(eLuxEnemyMessage_PlayerOutOfRange)
			SetActive(false);

	//------------------------------

		kLuxOnMessage(eLuxEnemyMessage_PlayerDead)
			ChangeState(eLuxEnemyState_Idle);

	//------------------------------

		kLuxOnMessage(eLuxEnemyMessage_HelpMe)
			ShowPlayerPosition();
			ChangeState(eLuxEnemyState_Hunt);

	//------------------------------

	////////////////////////////////
	// Idle
	////////////////////////////////

	//------------------------------
	kLuxState(eLuxEnemyState_Idle)
		kLuxOnEnter
			SetHeatRaySpotlightColor(mHeatRaySpotlightColorIdle);
			SetHeatRaySpotlightFOV(mfHRSpotlightFOVPatrol);
			mbPlayerInRange = false; //Reset so new checks are made in case player is near.
			SetMoveSpeed(eLuxEnemyMoveSpeed_Walk);

		kLuxOnMessage(eLuxEnemyMessage_PlayerInRange)
			gpBase->mpDebugHandler->AddMessage(_W("Enemy ") + cString::To16Char(msName) + _W(" enabled!"), false);

			if (gpBase->mpGlobalDataHandler->GetEnemyActivateSoundAllowed())
			{
				if (mbPlayActivateSound)
				{
					gpBase->mpGlobalDataHandler->SetEnemyActivateSoundMade();
					PlaySound(msEnabledSound);
				}
			}

			ChangeState(eLuxEnemyState_Patrol);

		kLuxOnMessage(eLuxEnemyMessage_PlayerDetected)
		//Nothing

//------------------------------

////////////////////////////////
// Go Home
////////////////////////////////

//------------------------------
	kLuxState(eLuxEnemyState_GoHome)
		kLuxOnEnter
			SetHeatRaySpotlightColor(mHeatRaySpotlightColorIdle);
			SetHeatRaySpotlightFOV(mfHRSpotlightFOVPatrol);
			mpPathfinder->MoveTo(mvStartPosition);
			SetMoveSpeed(eLuxEnemyMoveSpeed_Walk);

	kLuxOnMessage(eLuxEnemyMessage_EndOfPath)
		ChangeState(eLuxEnemyState_Idle);

	kLuxOnMessage(eLuxEnemyMessage_SoundHeard)
		if (apMessage->mfCustomValue > mfHearVolume)
		{
			ChangeState(eLuxEnemyState_Investigate);
			mvTempPos = apMessage->mvCustomValue;
			mfTempVal = apMessage->mfCustomValue;
		}
	//------------------------------

	////////////////////////////////
	// Wait
	////////////////////////////////

	//------------------------------
	kLuxState(eLuxEnemyState_Wait)
		kLuxOnEnter
			SetHeatRaySpotlightColor(mHeatRaySpotlightColorIdle);
			
			SetHeatRaySpotlightFOV(mfHRSpotlightFOVPatrol);
			if (mfWaitTime <= 0)
			{
				//gpBase->mpDebugHandler->AddMessage(_W("wait time=0"), false);
				if (mbAllowZeroWaitTime)
				{
					//gpBase->mpDebugHandler->AddMessage(_W("Zero wait time!!"), false);
					SendMessage(eLuxEnemyMessage_TimeOut, 0.01f, true);
				}
				else
				SendMessage(eLuxEnemyMessage_TimeOut, cMath::RandRectf(1, 3), true);
			}
			else
			{
			SendMessage(eLuxEnemyMessage_TimeOut, mfWaitTime, true);
			}
			mfWaitTime = 0;

			SendMessage(eLuxEnemyMessage_TimeOut_2, cMath::RandRectf(mfIdleExtraTimeMin, mfIdleExtraTimeMax), true);

			SetMoveSpeed(eLuxEnemyMoveSpeed_Walk);

		kLuxOnMessage(eLuxEnemyMessage_TimeOut)

			if (mPreviousState == eLuxEnemyState_Search)
			{
				FadeOutCurrentAnim(0.2f);
				ChangeState(eLuxEnemyState_Search);
			}
			else
			{
				if (GetPatrolNodeNum() > 0)
				{
					FadeOutCurrentAnim(0.2f);
					ChangeState(eLuxEnemyState_Patrol);
				}
			else
			{
				SendMessage(eLuxEnemyMessage_TimeOut, cMath::RandRectf(3, 5), true);
			}
			}

		kLuxOnMessage(eLuxEnemyMessage_TimeOut_2)
			PlayAnim("IdleExtra" + cString::ToString(cMath::RandRectl(1, mlIdleExtraNum)), false, 0.3f);

		kLuxOnMessage(eLuxEnemyMessage_AnimationOver)
			SendMessage(eLuxEnemyMessage_TimeOut_2, cMath::RandRectf(2, 6), true);

		kLuxOnMessage(eLuxEnemyMessage_SoundHeard)
			if (apMessage->mfCustomValue > mfHearVolume)
			{
				ChangeState(eLuxEnemyState_Investigate);
				mvTempPos = apMessage->mvCustomValue;
				mfTempVal = apMessage->mfCustomValue;
			}

	//------------------------------

	////////////////////////////////
	// Patrol
	////////////////////////////////

	//------------------------------
	kLuxState(eLuxEnemyState_Patrol)
		kLuxOnEnter
			
			SetHeatRaySpotlightColor(mHeatRaySpotlightColorIdle);
			SetHeatRaySpotlightFOV(mfHRSpotlightFOVPatrol);
			gpBase->mpMusicHandler->RemoveEnemy(eLuxEnemyMusic_Search, this);
			gpBase->mpMusicHandler->RemoveEnemy(eLuxEnemyMusic_Attack, this);

			gpBase->mpPlayer->RemoveTerrorEnemy(this);

			
			if(mbIsInStealthDashMode) ChangeSoundState(eLuxEnemySoundState_Silent);
			else ChangeSoundState(eLuxEnemySoundState_Idle);


			SetMoveSpeed(mPatrolMoveSpeed);
			if (mPatrolMoveSpeed == eLuxEnemyMoveSpeed_Run && !mbIsInStealthDashMode)
			{
				mfForwardSpeed *= mfRunSpeedMul;
			}
			else if (mbIsInStealthDashMode)
			{
				mfForwardSpeed *= mfRunSpeedMul*4;
				
			}
			PatrolUpdateGoal();


		kLuxOnUpdate
			if (mbPatrolMoveSpeedChanged)
			{
				SetMoveSpeed(mPatrolMoveSpeed);
				mbPatrolMoveSpeedChanged = false;
				if (mbIsInStealthDashMode) mfForwardSpeed *= mfRunSpeedMul * 4;
			}

			if (mbIsInStealthDashMode) ChangeSoundState(eLuxEnemySoundState_Silent);
			else ChangeSoundState(eLuxEnemySoundState_Idle);

			cLuxEnemyPatrolNode* pPatrolNode = GetCurrentPatrolNode();
			cVector3f pNode = pPatrolNode->mpNode->GetPosition();
			float fNodeDis = gpBase->mpMapHelper->CheckDistance(mpCharBody->GetFeetPosition(), pNode);

			if (fNodeDis >= mfEnterStealthDashNodeDistance && mbAllowedToDashAtWill && mbIsInStealthDashMode == false)
			{
				StartTeleportDash();
				//mfForwardSpeed *= mfRunSpeedMul * 4;
			}
			else if (fNodeDis <= mfExitStealthDashNodeDistance && mbIsInStealthDashMode == true)
			{
				StopTeleportDash();
			}

			if (mbStuckAtDoor)// && mpMap->DoorIsClosed(mlStuckDoorID))
			{
				iLuxEntity* pDoorEnt = mpMap->GetEntityByID(mlStuckDoorID);
				mvTempPos = pDoorEnt->GetAttachEntity()->GetWorldPosition();
				ChangeState(eLuxEnemyState_BreakDoor);
			}

		kLuxOnMessage(eLuxEnemyMessage_EndOfPath)

			/////////////////////////////////////
			//If at end, check if we want to remove
			if (IsAtLastPatrolNode() && CheckEnemyAutoRemoval(10)) // CheckEnemyAutoRemoval(10)
			{
				//Do nothing...
				//cLuxEnemyPatrolNode* pNode = GetCurrentPatrolNode();
				//DecCurrentPatrolNode(true);
			}
			////////////////////////////////////
			// Do stuff for node and go to next.
			else
			{
				cLuxEnemyPatrolNode* pNode = GetCurrentPatrolNode();
				if (pNode)
				{
					//////////////////////////////////
					// No animation is played
					if (pNode->msAnimation == "")
					{
						mfWaitTime = pNode->mfWaitTime;
						ChangeState(eLuxEnemyState_Wait);
					}
					//////////////////////////////////
					//Play animation
					else
					{
						PlayAnim(pNode->msAnimation, pNode->mbLoopAnimation, 0.2f);

						//////////////////////////////////
						// Animation is looping, check the end with timer
						if (pNode->mbLoopAnimation)
						{
							SendMessage(eLuxEnemyMessage_TimeOut, pNode->mfWaitTime, true);
							mfWaitTime = 0;
						}
						//////////////////////////////////
						// Animation is NOT looping, check end with AnimationOver message and wait after anim is done.
						else
						{
							mfWaitTime = pNode->mfWaitTime;
						}
					}
				}
				else
				{
					mfWaitTime = 0;
					ChangeState(eLuxEnemyState_Wait);
				}
				FinishPatrolEndOfPath(true);

				//if (!mbAutoReverseAtPathEnd || mbAutoReverseAtPathEnd && mbPathEndCallbackDelay == false)
				//{
					tString sCallback = msOverCallback;

					if (sCallback != "")
						gpBase->mpMapHandler->GetCurrentMap()->RunScript(sCallback + "()");

					if (PatrolRemoveCallback) msOverCallback = "";

					//if (mbAutoReverseAtPathEnd) mbPathEndCallbackDelay = true;
				//}
				//else
				//{
					//mbPathEndCallbackDelay = false;
				//}
			}

		//////////////////////////
		//Non looping animation ends
		kLuxOnMessage(eLuxEnemyMessage_AnimationOver)
			ChangeState(eLuxEnemyState_Wait);

		//////////////////////////
		//Looping animation shall end
		kLuxOnMessage(eLuxEnemyMessage_TimeOut)
			mpMover->UseMoveStateAnimations();
			ChangeState(eLuxEnemyState_Wait);

		//////////////////////////
		//Sound heard
		kLuxOnMessage(eLuxEnemyMessage_SoundHeard)
			if (apMessage->mfCustomValue > mfHearVolume)
			{
				ChangeState(eLuxEnemyState_Investigate);
				mvTempPos = apMessage->mvCustomValue;
				mfTempVal = apMessage->mfCustomValue;
			}
	//------------------------------

	////////////////////////////////
	// Investigate
	////////////////////////////////

	//------------------------------
	kLuxState(eLuxEnemyState_Investigate)
		kLuxOnEnter
			SetHeatRaySpotlightColor(mHeatRaySpotlightColorAlert);
			SetHeatRaySpotlightFOV(mfHRSpotlightFOVAlert);
			if (mbIsInStealthDashMode) StopTeleportDash();

			ChangeSoundState(eLuxEnemySoundState_Alert);
			if (mPatrolMoveSpeed == eLuxEnemyMoveSpeed_Run)
			{
				SetMoveSpeed(eLuxEnemyMoveSpeed_Run);
				mfForwardSpeed *= mfRunSpeedMul;
			}
			else
			{
				SetMoveSpeed(eLuxEnemyMoveSpeed_Walk);
			}

			mpPathfinder->Stop();
			PlayAnim("Notice", false, 0.3f);

			PlaySound(msNoticeSound);

			mfFOVMul = 4.0f; //When hearing a sound, enemy gets extra alert.

			gpBase->mpDebugHandler->AddMessage(_W("Sound Heard! Vol: ") + cString::ToStringW(mfTempVal), false);

		kLuxOnLeave
			mfFOVMul = 1.0f;
			FadeOutCurrentAnim(0.2f);
			SetMoveSpeed(eLuxEnemyMoveSpeed_Walk);

		//------------------------------
		kLuxOnMessage(eLuxEnemyMessage_AnimationOver)
			if (mPatrolMoveSpeed == eLuxEnemyMoveSpeed_Run)
			{
				SetMoveSpeed(eLuxEnemyMoveSpeed_Run);
				mfForwardSpeed *= mfRunSpeedMul;
			}
			else
			{
				SetMoveSpeed(eLuxEnemyMoveSpeed_Walk);
			}

			cAINode* pNode = mpPathfinder->GetNodeAtPos(mvTempPos, 0, 10, true, true, true, NULL, 1);
			if (pNode)
			{
				mpPathfinder->MoveTo(pNode->GetPosition());
			}
			else
			{
				gpBase->mpDebugHandler->AddMessage(_W("Could not find node near sound!"), false);
				mpPathfinder->MoveTo(mvTempPos);
			}
	//------------------------------
		kLuxOnMessage(eLuxEnemyMessage_EndOfPath)

			PlaySound(msGiveUpNoticeSound);

			if (mPreviousState == eLuxEnemyState_Search)
				ChangeState(eLuxEnemyState_Search);
			else
				ChangeState(eLuxEnemyState_Patrol);

	//------------------------------

		kLuxOnUpdate
			if (mbStuckAtDoor)// && mpMap->DoorIsClosed(mlStuckDoorID))
			{
				iLuxEntity* pDoorEnt = mpMap->GetEntityByID(mlStuckDoorID);
				mvTempPos = pDoorEnt->GetAttachEntity()->GetWorldPosition();
				ChangeState(eLuxEnemyState_BreakDoor);
			}

	//------------------------------


		kLuxOnMessage(eLuxEnemyMessage_SoundHeard)
			//If a new sound is loader than the previous go for that instead!
			if (mpPathfinder->IsMoving() && apMessage->mfCustomValue > mfTempVal)
			{
				cAINode* pNode = mpPathfinder->GetNodeAtPos(mvTempPos, 0, 10, true, true, true, NULL, 1);
				if (pNode)
					mpPathfinder->MoveTo(pNode->GetPosition());

				mfTempVal = apMessage->mfCustomValue;
			}

	//------------------------------

	////////////////////////////////
	// Alert
	////////////////////////////////

	//------------------------------
	kLuxState(eLuxEnemyState_Alert)
		/////////////
		//Enter
		kLuxOnEnter
			/////////////////////////
			// Go into alert!
			SetHeatRaySpotlightColor(mHeatRaySpotlightColorAlert);
			SetHeatRaySpotlightFOV(mfHRSpotlightFOVAlert);

			if (mbIsInStealthDashMode) StopTeleportDash();
			/////////////////////////
			// Set up sound
			ChangeSoundState(eLuxEnemySoundState_Alert);

			gpBase->mpPlayer->AddTerrorEnemy(this);
			//gpBase->mpMusicHandler->AddEnemy(eLuxEnemyMusic_Search,this);

			/////////////////////////
			// Stay and threaten
			if (mbThreatenOnAlert)
			{
				float fThreatLength = 9.0f; //float fThreatLength = mbFleeFromPlayer ? 3.0f : 6.0f;
				float fThreatFristStopCheck = 3.5f; //float fThreatFristStopCheck = mbFleeFromPlayer ? 1.0f : 3.5f;

				mpPathfinder->Stop();

				PlayAnim("ThreatLoop", true, 0.3f);

				SendMessage(eLuxEnemyMessage_TimeOut_2, fThreatLength, true); //Attack!!
				SendMessage(eLuxEnemyMessage_TimeOut_3, fThreatFristStopCheck, true);//Check if stop the threat
				mbSkipVisibilityRangeHandicaps = true;
			}
			/////////////////////////
			// Move towards player
			else
			{
				mpPathfinder->MoveTo(mvLastKnownPlayerPos);
				SetMoveSpeed(eLuxEnemyMoveSpeed_Walk);
				//mfForwardSpeed *= 1.2f;
				SendMessage(eLuxEnemyMessage_TimeOut, 0.3f, true); //To update path
			}

			/////////////////////////
			// Set up properties
			mfFOVMul = 4.0f;

			mfAlertRunTowardsCount = 0;

			//////////////
			//Leave
		kLuxOnLeave

				mbSkipVisibilityRangeHandicaps = false;

				if (mbThreatenOnAlert)
				{
					mpCharBody->SetMoveDelay(0.6f);
					mpMover->SetOverideMoveState(false);
				}

				mfFOVMul = 1.0f;
				if (mNextState != eLuxEnemyState_BreakDoor)
				{
					SetMoveSpeed(eLuxEnemyMoveSpeed_Walk);

					gpBase->mpPlayer->RemoveTerrorEnemy(this);
				}
		//------------------------------

		//////////////
		//Enter
		kLuxOnUpdate
			float fDistToPlayer = DistToPlayer();

			//////////////////////////////
			//Turn towards player
			if (mbThreatenOnAlert && PlayerIsDetected())
			{
				float fAngleDist = cMath::GetAngleDistanceRad(mpCharBody->GetYaw() + kPif,
					gpBase->mpPlayer->GetCharacterBody()->GetYaw());
				if (fabs(fAngleDist) > cMath::ToRad(30))
					mpMover->TurnToPos(GetPlayerFeetPos());
			}

			//////////////////////////////
			//Check if player is moving towards enemy
			if (fDistToPlayer < mfAlertRunTowardsCheckDistance)
			{
				float fPlayerDirAmount = GetPlayerMovementTowardEnemyAmount();
				mfAlertRunTowardsCount += fPlayerDirAmount;
			}
			else
			{
				mfAlertRunTowardsCount -= 1;
			}
			if (mfAlertRunTowardsCount < 0) mfAlertRunTowardsCount = 0;

			//////////////////////////////
			//Update the speed of movement.
			if (fDistToPlayer > mfIncreaseAlertSpeedDistance)
			{
				SetMoveSpeed(eLuxEnemyMoveSpeed_Walk);
				//mfForwardSpeed *= 1.2f * mfIncreasedAlertSpeedMul;
				mfForwardSpeed *= mfIncreasedAlertSpeedMul;
			}
			else
			{
				SetMoveSpeed(eLuxEnemyMoveSpeed_Walk);
				//mfForwardSpeed *= 1.2f;
			}

			//////////////////////////////
			//Stuck at door, break it
			if (gpBase->mpPlayer->GetTerror() >= 1 && mbStuckAtDoor)// && mpMap->DoorIsClosed(mlStuckDoorID))
			{
				iLuxEntity* pDoorEnt = mpMap->GetEntityByID(mlStuckDoorID);
				mvTempPos = pDoorEnt->GetAttachEntity()->GetWorldPosition();
				ChangeState(eLuxEnemyState_BreakDoor);
			}

			//////////////////////////////
			//Player is no longer seen, see if time to search or wait
			else if (PlayerIsDetected() == false)
			{
				float fTerror = gpBase->mpPlayer->GetTerror();
				if (fTerror < 0.45)
				{
					ChangeState(eLuxEnemyState_Wait);
				}
				else if (fTerror < 1)
				{
					ChangeState(eLuxEnemyState_Search);
				}
			}
			//////////////////////////////
			//Player is seen, see if close enough for hunt
			else
			{
				if (mbThreatenOnAlert)
				{

					//////////////
					//Attack

					//If terror is topped and distance to player is under a value or player is running towards enemy
					//Or if distance to player is less than a value
					if ((gpBase->mpPlayer->GetTerror() >= 1 && (mfAlertRunTowardsCount > mfAlertRunTowardsToHuntLimit || fDistToPlayer < mfAlertToInstantHuntDistance * 2)) ||
						fDistToPlayer < mfAlertToInstantHuntDistance)
					{
						gpBase->mpPlayer->SetTerror(1.0f);

						ChangeState(eLuxEnemyState_Hunt);
					}

				}
				else
				{
					//If terror is topped and distance to player is over a value or player is running towards enemy
					//Or if distance to player is less than a value
					float fTerror = gpBase->mpPlayer->GetTerror();
					if ((fTerror >= 1 && (fDistToPlayer > mfAlertToHuntDistance || mfAlertRunTowardsCount > mfAlertRunTowardsToHuntLimit)) ||
						fDistToPlayer < mfAlertToInstantHuntDistance)
					{
						gpBase->mpPlayer->SetTerror(1.0f);

						ChangeState(eLuxEnemyState_Hunt);
					}
				}
			}
	//------------------------------
		//////////////
		//Reach end of path
		kLuxOnMessage(eLuxEnemyMessage_EndOfPath)
			if (mbThreatenOnAlert == false)
			{
				float fDistToPlayer = DistToPlayer();

				//Path ended and player is not seen or enemy is stuck (this should only happen when at a distance!
				if (PlayerIsDetected() == false || (apMessage->mlCustomValue == 1 && fDistToPlayer > 5))
				{
					ChangeState(eLuxEnemyState_Search);
				}
				else if (apMessage->mlCustomValue == 1 && PlayerIsDetected())
				{
					//This is when the enemy should just stnad still but I think nothing is really needed
				}
			}
	//------------------------------		

		//////////////
		//Update move to
		kLuxOnMessage(eLuxEnemyMessage_TimeOut)
			if (PlayerIsDetected())
			mpPathfinder->MoveTo(mvLastKnownPlayerPos);

			SendMessage(eLuxEnemyMessage_TimeOut, 0.3f, true);

	//------------------------------
		//////////////
		//Start Hunt
		kLuxOnMessage(eLuxEnemyMessage_TimeOut_2)

			gpBase->mpPlayer->SetTerror(1.0f);
			ChangeState(eLuxEnemyState_Hunt);


		/////////////
		//Check if threat should end
		kLuxOnMessage(eLuxEnemyMessage_TimeOut_3)
			if (PlayerIsDetected() == false)
				ChangeState(eLuxEnemyState_Patrol);
			else
				SendMessage(eLuxEnemyMessage_TimeOut_3, 0.75f, true);


	//------------------------------

		//////////////
		//Takes a hit
		kLuxOnMessage(eLuxEnemyMessage_TakeHit)
			ChangeState(eLuxEnemyState_Hurt);
			gpBase->mpMusicHandler->RemoveEnemy(eLuxEnemyMusic_Search, this);

	//------------------------------

		///////////
		//Overload
		kLuxOnMessage(eLuxEnemyMessage_PlayerDetected)

	//------------------------------

	////////////////////////////////
	// Search
	////////////////////////////////

	//------------------------------
	kLuxState(eLuxEnemyState_Search)
		kLuxOnEnter
			ChangeSoundState(eLuxEnemySoundState_Alert);

			SetHeatRaySpotlightColor(mHeatRaySpotlightColorAlert);
			SetHeatRaySpotlightFOV(mfHRSpotlightFOVAlert);

			if (mbIsInStealthDashMode) StopTeleportDash();

			SendMessage(eLuxEnemyMessage_TimeOut, mfPlayerSearchTime, true);

			SendMessage(eLuxEnemyMessage_TimeOut_2, cMath::RandRectf(0, 1), true);

			gpBase->mpMusicHandler->RemoveEnemy(eLuxEnemyMusic_Attack, this);
			gpBase->mpMusicHandler->AddEnemy(eLuxEnemyMusic_Search, this);

			if (mPreviousState == eLuxEnemyState_Hunt ||
				mPreviousState == eLuxEnemyState_HuntWander ||
				mPreviousState == eLuxEnemyState_HuntPause ||
				mPreviousState == eLuxEnemyState_BreakDoor|| mPreviousState == eLuxEnemyState_AttackRange)
			{
				SetMoveSpeed(eLuxEnemyMoveSpeed_Run);
				mfForwardSpeed *= mfRunSpeedMul;
			}
			else
			{
				SetMoveSpeed(eLuxEnemyMoveSpeed_Walk);
				mfForwardSpeed *= 1.0f;
			}

		kLuxOnLeave
			SetMoveSpeed(eLuxEnemyMoveSpeed_Walk);
	//------------------------------

		kLuxOnUpdate
			if (mbStuckAtDoor)// && mpMap->DoorIsClosed(mlStuckDoorID))
			{
				iLuxEntity* pDoorEnt = mpMap->GetEntityByID(mlStuckDoorID);
				mvTempPos = pDoorEnt->GetAttachEntity()->GetWorldPosition();
				ChangeState(eLuxEnemyState_BreakDoor);
			}

		//At node
		kLuxOnMessage(eLuxEnemyMessage_EndOfPath)
			if (GetSpecialSearchNodeActive() == true)
			{
				SetSpecialSearchNodeActive(false);
			}
			mpPathfinder->Stop();
			SendMessage(eLuxEnemyMessage_TimeOut_2, cMath::RandRectf(1, 3), true);

	//Wait a few secs
		kLuxOnMessage(eLuxEnemyMessage_TimeOut_2)
			//cAINode * pNode = GetSearchForPlayerNode();
			if (GetSpecialSearchNodeActive() == true)
			{
				cAINode* pNode = mpPathfinder->GetNodeAtPos(GetSpecialSearchNode(), 4, 12, false, false, true, NULL);
				if (pNode)
					mpPathfinder->MoveTo(pNode->GetPosition());
				else
					ChangeState(eLuxEnemyState_Patrol);
			}
			else
			{
				cAINode* pNode = mpPathfinder->GetNodeAtPos(gpBase->mpPlayer->GetCharacterBody()->GetFeetPosition(), 4, 12, false, false, true, NULL); //GetFeetPosition(), 4, 12,false, false, true, NULL);
				if (pNode)
					mpPathfinder->MoveTo(pNode->GetPosition());
				else
					ChangeState(eLuxEnemyState_Patrol);
			}

		//End of searching
		kLuxOnMessage(eLuxEnemyMessage_TimeOut)
			ChangeState(eLuxEnemyState_Patrol);
			gpBase->mpMusicHandler->RemoveEnemy(eLuxEnemyMusic_Search, this);

		//Hear sound
		kLuxOnMessage(eLuxEnemyMessage_SoundHeard)
			if (apMessage->mfCustomValue > mfHearVolume)
			{
				ChangeState(eLuxEnemyState_Investigate);
				mvTempPos = apMessage->mvCustomValue;
				mfTempVal = apMessage->mfCustomValue;
			}

	//------------------------------

	////////////////////////////////
	// Hunt
	////////////////////////////////

	//------------------------------
	kLuxState(eLuxEnemyState_Hunt)
		///////////////////////
		// Enter
		kLuxOnEnter
			gpBase->mpMusicHandler->RemoveEnemy(eLuxEnemyMusic_Search, this);
			if (mAttackType == eLuxAttackType_HeatRay)
			{
				SetHeatRaySpotlightColor(mHeatRaySpotlightColor);
			}
			else
			{
				SetHeatRaySpotlightColor(mHeatRaySpotlightColorHunt);
			}
			SetHeatRaySpotlightFOV(mfHRSpotlightFOVAlert);

			float fMaxHits = cMath::RandRectl(1, 3);
			if (mlAttackHitCounter >= fMaxHits || mbLastAttackWasMiss || mPreviousState == eLuxEnemyState_AttackMeleeLong || mPreviousState == eLuxEnemyState_AttackMeleeShort)
			{
				mlAttackHitCounter = 0;

				ChangeState(eLuxEnemyState_HuntPause);
				mbLastAttackWasMiss = false;
			}
			else
			{
				ChangeSoundState(eLuxEnemySoundState_Hunt);
				SetMoveSpeed(eLuxEnemyMoveSpeed_Run);

				if (mbIsInStealthDashMode) mfForwardSpeed *= mfRunSpeedMul * 2;
				else mfForwardSpeed *= mfRunSpeedMul;

				SendMessage(eLuxEnemyMessage_TimeOut, 0.1f, true);
				mfFOVMul = 4.0f;

				

				gpBase->mpMusicHandler->AddEnemy(eLuxEnemyMusic_Attack, this);

				gpBase->mpPlayer->AddTerrorEnemy(this);

				mpPathfinder->MoveTo(mvLastKnownPlayerPos);

				float fPodDis = gpBase->mpMapHelper->CheckDistance(mpCharBody->GetFeetPosition(), mvLastKnownPlayerPos);

				SendMessage(eLuxEnemyMessage_TimeOut_2, 0.5f, true);
				SendMessage(eLuxEnemyMessage_TimeOut_3, cMath::RandRectf(0.25f, 1.0f), true);
			}


	///////////////////////
	// Leave
		kLuxOnLeave
			SetMoveSpeed(eLuxEnemyMoveSpeed_Walk);
			mfFOVMul = 1.0f;

	//------------------------------

		///////////////////////
		// Update
		kLuxOnUpdate
			//////////////////////////
			//Short attack
			float fDistToPlayer = DistToPlayer();

			if (mbIsInStealthDashMode)
			{
				mfForwardSpeed = mfForwardSpeed * 2;
				ChangeSoundState(eLuxEnemySoundState_Silent);
			}
			else
			{
				ChangeSoundState(eLuxEnemySoundState_Hunt);
				//mfForwardSpeed *= mfRunSpeedMul;
			}
			if (CanUseRangedAttacks() && fDistToPlayer >= mfRangedAttackDistance && !mbIsInStealthDashMode)
			{
						ChangeState(eLuxEnemyState_AttackRange);
			}
			else if (mbCanMeele && fDistToPlayer < mfNormalAttackDistance && !mbIsInStealthDashMode)
			{
				if (mbForceChargeAttack)
				{
					ChangeState(eLuxEnemyState_AttackMeleeLong);
					mbForceChargeAttack = false;
				}
				else
				{
					ChangeState(eLuxEnemyState_AttackMeleeShort);
				}
			}
			else if (fDistToPlayer >= mfEnterStealthDashNodeDistance && mbAllowedToDashAtWill && mbIsInStealthDashMode == false)
			{
				StartTeleportDash();
			}
			else if (fDistToPlayer <= mfExitStealthDashNodeDistance && mbIsInStealthDashMode == true)
			{
				StopTeleportDash();
			}
			else if (mbStuckAtDoor)// && mpMap->DoorIsClosed(mlStuckDoorID))
			{
				iLuxEntity* pDoorEnt = mpMap->GetEntityByID(mlStuckDoorID);
				mvTempPos = pDoorEnt->GetAttachEntity()->GetWorldPosition();
				ChangeState(eLuxEnemyState_BreakDoor);
			}
			else if (mfHealth <= mfFleeHealth)
			{
				ChangeState(eLuxEnemyState_Alert);
			}
	//------------------------------

	/////////////////////////////
	// Check if close enough for launch attack
		kLuxOnMessage(eLuxEnemyMessage_TimeOut_2)
			//////////////////////////
			//Launch attack (Long Ranged)
			float fDist = DistToPlayer();
			if (fDist > mfRangedAttackDistance && fDist < mfRangedAttackDistance * 2.0f && mpMover->GetStuckCounter() < 0.5f && CanUseRangedAttacks())
			{
				ChangeState(eLuxEnemyState_AttackRange);
				
			}
			else if (fDist > mfNormalAttackDistance && fDist < mfNormalAttackDistance * 4.0f && mpMover->GetStuckCounter() < 0.5f && mbCanMeele)
			{
				ChangeState(eLuxEnemyState_AttackMeleeLong);	
			}


			SendMessage(eLuxEnemyMessage_TimeOut_2, 1.5f, true);

		/////////////////////////////
		// Sound
		kLuxOnMessage(eLuxEnemyMessage_TimeOut_3)

			PlayHuntSound();
			SendMessage(eLuxEnemyMessage_TimeOut_3, cMath::RandRectf(0.5f, 2.0f), true);
	//------------------------------

		////////////////////////
		// End of current path
		kLuxOnMessage(eLuxEnemyMessage_EndOfPath)
			float fDistToPlayer = DistToPlayer();

			//Check if was end of path because of stuck.,
			if (apMessage->mlCustomValue == 1 && fDistToPlayer >= mfNormalAttackDistance && fDistToPlayer <= mfRangedAttackDistance)
			{
				if (PlayerIsDetected() == false)
				{
					//Give some extra time to find player!
					SendMessage(eLuxEnemyMessage_TimeOut_4, 1.5f, true);
				}
				else
				{
					ChangeState(eLuxEnemyState_HuntWander);
				}
			}
			//Get new path
			else
			{
				mpPathfinder->MoveTo(mvLastKnownPlayerPos);

				if (PlayerIsDetected() == false)
				{
					//When lost player give some extra time to catch up
					SendMessage(eLuxEnemyMessage_TimeOut_4, 1.5f, true);
				}
			}
	//------------------------------

		//////////////////
		// Update path and call for help!
		kLuxOnMessage(eLuxEnemyMessage_TimeOut)

			mpMap->BroadcastEnemyMessage(eLuxEnemyMessage_HelpMe, true, mpCharBody->GetPosition(), mfActivationDistance * 0.5f,
				0, false, mpCharBody->GetFeetPosition());


			mpPathfinder->MoveTo(mvLastKnownPlayerPos);

			SendMessage(eLuxEnemyMessage_TimeOut, 0.2f, true);
	//------------------------------

		//////////////////
		//When lost player give some extra time to catch up
		kLuxOnMessage(eLuxEnemyMessage_TimeOut_4)
			if (PlayerIsDetected() == false)
			{
				gpBase->mpPlayer->RemoveTerrorEnemy(this);

				gpBase->mpMusicHandler->RemoveEnemy(eLuxEnemyMusic_Attack, this);

				if (mbThreatenOnAlert)
					ChangeState(eLuxEnemyState_Patrol);
				else
					ChangeState(eLuxEnemyState_Search);
			}
			else
			{
				SendMessage(eLuxEnemyMessage_TimeOut, 0.2f, true);
			}

		////////////////////////
		// Overload global
		kLuxOnMessage(eLuxEnemyMessage_PlayerDetected)
		kLuxOnMessage(eLuxEnemyMessage_HelpMe)
	//------------------------------

	///////////////////////////////
	// Hunt Wander (Remain in hunt mode but go to nearby reachable node)
	////////////////////////////////

	//------------------------------

	kLuxState(eLuxEnemyState_HuntWander)
		///////////////////////
		// Enter
		kLuxOnEnter

			mpMover->ResetStuckCounter();

			SetMoveSpeed(eLuxEnemyMoveSpeed_Walk);
			mfForwardSpeed *= 1.2f;

			mfFOVMul = 4.0f;

			cAINode* pNode = mpPathfinder->GetNodeAtPos(mpCharBody->GetFeetPosition(), 2, 5, false, true, true, NULL);
			if (pNode)
				mpPathfinder->MoveTo(pNode->GetPosition());
			else
				ChangeState(eLuxEnemyState_Hunt);

			SendMessage(eLuxEnemyMessage_TimeOut_3, cMath::RandRectf(0.5f, 2.0f), true);

		///////////////////////
		// Leave
		kLuxOnLeave
			mfFOVMul = 1.0f;

	//------------------------------

		/////////////////////////////
		// Sound
		kLuxOnMessage(eLuxEnemyMessage_TimeOut_3)

			PlayHuntSound();
			SendMessage(eLuxEnemyMessage_TimeOut_3, cMath::RandRectf(0.5f, 3.0f), true);

	//------------------------------

		kLuxOnMessage(eLuxEnemyMessage_EndOfPath)
			ChangeState(eLuxEnemyState_Hunt);

		////////////////////////
		// Overload global
		kLuxOnMessage(eLuxEnemyMessage_PlayerDetected)
		kLuxOnMessage(eLuxEnemyMessage_HelpMe)

		//------------------------------

		////////////////////////////////
		// Hunt Pause (Walk slowly towards the player)
		////////////////////////////////

		//------------------------------

		kLuxState(eLuxEnemyState_HuntPause)
			///////////////////////
			// Enter
			kLuxOnEnter

			if (mPreviousState == eLuxEnemyState_AttackRange && mAttackType == eLuxAttackType_Archvile)
			{
				PlayAnim("Cooldown", false, 0.3f);
				SendMessage(eLuxEnemyMessage_TimeOut, 0.2f, true);
			}
			else
			{
				SendMessage(eLuxEnemyMessage_TimeOut_2, cMath::RandRectf(mfHuntPauseMinTime, mfHuntPauseMaxTime) * mfHuntPauseTimeMul, true);
				SendMessage(eLuxEnemyMessage_TimeOut, 0.2f, true);
				SendMessage(eLuxEnemyMessage_TimeOut_3, cMath::RandRectf(0.5f, 3.0f), true);
			}
				
			mpPathfinder->Stop();

			SetMoveSpeed(eLuxEnemyMoveSpeed_Run);
			mfForwardSpeed *= 0.6f;

			mfFOVMul = 4.0f;



		///////////////////////
		// Leave
		kLuxOnLeave
			mfFOVMul = 1.0f;

	//------------------------------

		///////////////////////
		// Update
		kLuxOnMessage(eLuxEnemyMessage_TimeOut)

			if (DistToPlayer2D() > 2.0f)
				mpPathfinder->MoveTo(gpBase->mpPlayer->GetCharacterBody()->GetFeetPosition());
			else
				mpMover->TurnToPos(gpBase->mpPlayer->GetCharacterBody()->GetFeetPosition());

			if (CanSeePlayer())
			{
				SendMessage(eLuxEnemyMessage_TimeOut, 0.2f, true);
			}

		///////////////////////
		// Time out
		kLuxOnMessage(eLuxEnemyMessage_TimeOut_2)
			ChangeState(eLuxEnemyState_Alert);

		/////////////////////////////
		// Sound
		kLuxOnMessage(eLuxEnemyMessage_TimeOut_3)

			PlayHuntSound();
			SendMessage(eLuxEnemyMessage_TimeOut_3, cMath::RandRectf(0.5f, 3.0f), true);

		////////////////////////////
		// Cooldown Anim
		kLuxOnMessage(eLuxEnemyMessage_AnimationOver)
			//SendMessage(eLuxEnemyMessage_TimeOut, 0.2f, true);
			SendMessage(eLuxEnemyMessage_TimeOut_2, cMath::RandRectf(mfHuntPauseMinTime, mfHuntPauseMaxTime) * mfHuntPauseTimeMul, true);

		
		kLuxOnMessage(eLuxEnemyMessage_SoundHeard)
			if (apMessage->mfCustomValue > mfHearVolume)
			{
				ChangeState(eLuxEnemyState_Search);
				mvTempPos = apMessage->mvCustomValue;
				mfTempVal = apMessage->mfCustomValue;
			}

		////////////////////////
		// Overload global
		kLuxOnMessage(eLuxEnemyMessage_PlayerDetected)
		kLuxOnMessage(eLuxEnemyMessage_HelpMe)

		//------------------------------

		////////////////////////////////
		// Break door
		////////////////////////////////

		//------------------------------

	kLuxState(eLuxEnemyState_BreakDoor)
		kLuxOnEnter
			mpPathfinder->Stop();
			PlayAnim("BreakDoor", false, 0.3f);
			mfFOVMul = 4.0f;

		kLuxOnLeave
			mlAttackHitCounter = 0; //When returning from door breakage there should be no pause!
			mfFOVMul = 1.0f;

	//------------------------------

		kLuxOnUpdate
			//Turn towards the door!
			mpMover->TurnToPos(mvTempPos);

	//------------------------------

		kLuxOnMessage(eLuxEnemyMessage_AnimationOver)
			if (PlayerIsDetected())
			{
				ChangeState(eLuxEnemyState_Alert);
			}
			else if (mpMap->DoorIsBroken(mlStuckDoorID))
			{
				if (mPreviousState == eLuxEnemyState_Hurt)
					ChangeState(eLuxEnemyState_Hunt);
				else
					ChangeState(mPreviousState);
			}
			else
			{
				PlayAnim("BreakDoor", false, 0.3f);
			}

	//------------------------------

		kLuxOnMessage(eLuxEnemyMessage_AnimationSpecialEvent)
			Attack(mNormalAttackSize, mBreakDoorAttackDamage);


		////////////////////////
		// Overload global
		kLuxOnMessage(eLuxEnemyMessage_PlayerDetected)
		kLuxOnMessage(eLuxEnemyMessage_HelpMe)

	//------------------------------

	////////////////////////////////
	// Long Range Attack
	////////////////////////////////

	//------------------------------
	kLuxState(eLuxEnemyState_AttackRange)
		kLuxOnEnter

			mpPathfinder->Stop();
			//SetMoveSpeed(eLuxEnemyMoveSpeed_Walk);
			SendMessage(eLuxEnemyMessage_TimeOut, 1.0f, true);
			mlTempVal = 0;
			mfFOVMul = 4.0f;
			

			if (mAttackType == eLuxAttackType_HeatRay)
			{
				SetMoveSpeed(eLuxEnemyMoveSpeed_Run);
			}
			else
			{
				SetMoveSpeed(eLuxEnemyMoveSpeed_Walk);
				mfForwardSpeed *= 0.6f;
			}


		kLuxOnLeave
			mfFOVMul = 1.0f;
			SetMoveSpeed(eLuxEnemyMoveSpeed_Run);

	//------------------------------

		kLuxOnUpdate
			//If close enough, attack early.
			float fDist = DistToPlayer();
			

			if (PlayerIsDetected())
			{
				float fAngleDist = cMath::GetAngleDistanceRad(mpCharBody->GetYaw() + kPif,
					gpBase->mpPlayer->GetCharacterBody()->GetYaw());
				if (fabs(fAngleDist) > cMath::ToRad(30))
					mpMover->TurnToPos(GetPlayerFeetPos());
			}

			if (mlTempVal == 0 && DistToPlayer2D() <= mfRangedAttackDistance && DistToPlayer2D() > mfRangedAttackDistance*4.0f && PlayerIsDetected())
			{
				mlTempVal = 1;
				if (mAttackType == eLuxAttackType_Archvile)
				{
					PlayAnim("OpenAggro", false, 0.3f, false);
					EnableArchvileAttackParticle(true);
					PlayHeatRaySounds(true);
					mfFlameLightFadeTime = mfFlameLightInitFadeTime;
					mbIsInArchvileAttack = true;
					mfFlameLightFadeTime = mfFlameLightInitFadeTime;
					gpBase->mpDebugHandler->AddMessage(_W("Archvile Attack Started throu Update"), false);
				}
				else if (mAttackType == eLuxAttackType_Projectile)
				{
					PlayAnim("ShootProjectile", false, 0.3f, false);
				}
			}

			if (mlTempVal == 1)
			{
				if (mAttackType == eLuxAttackType_Archvile)
				{
					if (PlayerIsDetected() == false || fDist > mfRangedAttackDistance * 4.0f)
					{
						
						EnableArchvileAttackParticle(false);
						PlayHeatRaySounds(false);
						mbIsInArchvileAttack = false;
						mfFlameLightFadeTime = mfFlameLightInitFadeTime;
						ChangeState(eLuxEnemyState_HuntPause);
						gpBase->mpDebugHandler->AddMessage(_W("Archvile Attack Stopped throu Update"), false);
					}
					else if (PlayerIsDetected() == true && fDist >= mfRangedAttackDistance && fDist < mfRangedAttackDistance * 4.0f)
					{
						if (pArchvilePS_AttackStart) pArchvilePS_AttackStart->SetPosition(GetPlayerFeetPosition() + mvArchvilePS_AttackStartOffset);
					}
					else if (PlayerIsDetected() == true && mbCanMeele && fDist < mfNormalAttackDistance)
					{
						EnableArchvileAttackParticle(false);
						PlayHeatRaySounds(false);
						mbIsInArchvileAttack = false;
						mfFlameLightFadeTime = mfFlameLightInitFadeTime;
						mlTempVal = 0;
						gpBase->mpDebugHandler->AddMessage(_W("Archvile Attack Stopped for Melee Attack Update"), false);
						if (mbForceChargeAttack)
						{
							ChangeState(eLuxEnemyState_AttackMeleeLong);
							mbForceChargeAttack = false;
						}
						else
						{
							ChangeState(eLuxEnemyState_AttackMeleeShort);
						}
					}
				} 
				else if (mAttackType == eLuxAttackType_HeatRay)
				{
					if (PlayerIsDetected() == false || fDist > mfRangedAttackDistance * 4.0f)
					{
						ChangeState(eLuxEnemyState_HuntPause);
					}
					else if (PlayerIsDetected() == true && fDist >= mfRangedAttackDistance && fDist < mfRangedAttackDistance * 4.0f)
					{
						
					}
					else if (PlayerIsDetected() == true && mbCanMeele && fDist < mfNormalAttackDistance)
					{
						mlTempVal = 0;
						if (mbForceChargeAttack)
						{
							ChangeState(eLuxEnemyState_AttackMeleeLong);
							mbForceChargeAttack = false;
						}
						else
						{
							ChangeState(eLuxEnemyState_AttackMeleeShort);
						}
					}
					else if (PlayerIsDetected() == true && !mbCanMeele && fDist < mfNormalAttackDistance)
					{
						ChangeState(eLuxEnemyState_HuntPause);
					}
				}

			}

			//Move towards player
			mpMover->MoveToPos(gpBase->mpPlayer->GetCharacterBody()->GetFeetPosition());


	//------------------------------

		kLuxOnMessage(eLuxEnemyMessage_TimeOut)
			cVector3f mvPlayerPos = gpBase->mpPlayer->GetCharacterBody()->GetFeetPosition();
			if (mlTempVal == 0 && PlayerIsDetected())
			{
				mlTempVal = 1;
				if (mAttackType == eLuxAttackType_Archvile)
				{
					PlayAnim("OpenAggro", false, 0.3f, false);
					EnableArchvileAttackParticle(true);
					PlayHeatRaySounds(true);
					mbIsInArchvileAttack = true;
					mfFlameLightFadeTime = mfFlameLightInitFadeTime;
					gpBase->mpDebugHandler->AddMessage(_W("Archvile Attack Started throu Time Out"), false);
				}
				if (mAttackType == eLuxAttackType_Projectile) PlayAnim("OpenAggro", false, 0.3f, false);
			}
	//------------------------------
		kLuxOnMessage(eLuxEnemyMessage_TimeOut_2)
			mlTempVal = 0;
			ChangeState(eLuxEnemyState_HuntPause);

	//------------------------------
		kLuxOnMessage(eLuxEnemyMessage_AnimationSpecialEvent)
			float fDist = DistToPlayer();
			cVector3f mvPlayerPos = gpBase->mpPlayer->GetCharacterBody()->GetFeetPosition();
			if (PlayerIsDetected() == true && fDist < mfRangedAttackDistance * 4.0f)
			{
				if (mlTempVal == 1)
				{
					if (mAttackType == eLuxAttackType_Archvile)
					{
						if (PlayerIsDetected() == true && fDist < mfRangedAttackDistance * 4.0f)
						{
							float fDamage = cMath::RandRectf(mvRangedAttackDamage.x, mvRangedAttackDamage.y);
							eLuxDamageType mDamageType;
							tString sLowType = cString::ToLowerCase(msRangedAttackDisplayType);
							if (sLowType == "claws") mDamageType = eLuxDamageType_Claws;
							if (sLowType == "slash") mDamageType = eLuxDamageType_Slash;

							EnableArchvileAttackParticle(false);
							PlayHeatRaySounds(false);

							pArchvilePS_AttackEnd = mpMap->GetWorld()->CreateParticleSystem(msName+"_ArchvileAttackEnd", msArchvilePS_AttackEnd, 1);
							if (pArchvilePS_AttackEnd) pArchvilePS_AttackEnd->SetPosition(GetPlayerFeetPosition() + mvArchvilePS_AttackEndOffset);

							
							mbIsInArchvileAttack = false;
							mfFlameLightFadeTime = mfFlameLightInitFadeTime;
				
							gpBase->mpPlayer->GiveDamage(fDamage, mlRangedAttackStrength, mDamageType, true, true);
							gpBase->mpPlayer->GetCharacterBody()->AddForceVelocity(mvRangedAttackForce * 0.1f);
							
							cSoundHandler* pSoundHandler = gpBase->mpEngine->GetSound()->GetSoundHandler();
							pSoundHandler->PlayGui(msRangedAttackHitSound, false, 2);

							gpBase->mpDebugHandler->AddMessage(_W("Archvile Attack completed"), false);

							//SendMessage(eLuxEnemyMessage_TimeOut_2, 0.5f, true);
							ChangeState(eLuxEnemyState_HuntPause);
						}
					}
					//Attack(mRangedAttackSize, mRangedAttackDamage, mfDamageMul);
				}
			}
			else
			{
				ChangeState(mPreviousState);
			}
	//------------------------------	

		kLuxOnMessage(eLuxEnemyMessage_AnimationOver)
			ChangeState(mPreviousState);


	//------------------------------

		////////////////////////
		// Overload global
		kLuxOnMessage(eLuxEnemyMessage_PlayerDetected)
		kLuxOnMessage(eLuxEnemyMessage_HelpMe)

	//------------------------------

	////////////////////////////////
	// Attack Short
	////////////////////////////////

	//------------------------------

	kLuxState(eLuxEnemyState_AttackMeleeShort)
		kLuxOnEnter

			if (mAttackType == eLuxAttackType_HeatRay)
			{
				SetHeatRaySpotlightColor(mHeatRaySpotlightColor);
			}

			mpPathfinder->Stop();
			mlTempVal = 0;

			if (mbIsActualWraith)
			{
				PlayAnim("AttackMelee", false, 0.3f);
			}
			else
			{
				PlayAnim("Attack"+cString::ToString(cMath::RandRectl(1, 3)), false, 0.3f);
			}
			mfFOVMul = 4.0f;

		kLuxOnLeave
			mfFOVMul = 1.0f;

		kLuxOnUpdate
			/*if (mlTempVal == 1 && mbIsActualWraith)
			{
				if (pAttackMeleePS) pAttackMeleePS->SetPosition(mpCharBody->GetFeetPosition());
			}*/

		//------------------------------
		kLuxOnMessage(eLuxEnemyMessage_TimeOut)
			ChangeState(eLuxEnemyState_Flee);

		kLuxOnMessage(eLuxEnemyMessage_AnimationOver)

			if (mPreviousState == eLuxEnemyState_Hurt)
				ChangeState(eLuxEnemyState_Hunt);
			else
				ChangeState(mPreviousState);

		//------------------------------

		kLuxOnMessage(eLuxEnemyMessage_AnimationSpecialEvent)

			if (mbIsActualWraith && msAttackMeleePS != "")
			{
				pAttackMeleePS = mpMap->GetWorld()->CreateParticleSystem("WraithMeleePS", msAttackMeleePS, 1);
				if (pAttackMeleePS) pAttackMeleePS->SetPosition(mpCharBody->GetFeetPosition());
				mlTempVal = 1;
			}
			if (!mbIsActualWraith)
			{
				SendMessage(eLuxEnemyMessage_TimeOut, 1.0f, true);
			}
			if (Attack(mNormalAttackSize, mNormalAttackDamage, mfDamageMul) == false)
			{
				mbLastAttackWasMiss = true;
				mbForceChargeAttack = cMath::RandRectl(0, 1) == 0;
			}

		//------------------------------

		////////////////////////
		// Overload global
		kLuxOnMessage(eLuxEnemyMessage_PlayerDetected)
		kLuxOnMessage(eLuxEnemyMessage_HelpMe)
	
	//------------------------------

	////////////////////////////////
	// Attack Launch
	////////////////////////////////

	//------------------------------
	kLuxState(eLuxEnemyState_AttackMeleeLong)
		kLuxOnEnter

		mpPathfinder->Stop();
		SetMoveSpeed(eLuxEnemyMoveSpeed_Run);
		mfForwardSpeed *= 2.0f;
		mlTempVal = 0;
		mfFOVMul = 4.0f;
		SendMessage(eLuxEnemyMessage_TimeOut, 1.0f, true); //Run a bit at full speed before charge.


		kLuxOnLeave
			mfFOVMul = 1.0f;
		SetMoveSpeed(eLuxEnemyMoveSpeed_Run);

		//------------------------------

		kLuxOnUpdate
			//If close enough, charge early.
			if (mlTempVal == 0 && DistToPlayer2D() < 1.3f && mbIsActualWraith)
			{
				mlTempVal = 1;
				PlayAnim("AttackMelee", false, 0.3f, false);
			}
			else if (mlTempVal == 0 && DistToPlayer2D() < 1.3f && !mbIsActualWraith)
			{
				mlTempVal = 1;
				PlayAnim("ChargeAttack", false, 0.3f, false);
			}

			/*if (mlTempVal == 1 && mbIsActualWraith)
			{
				if (pAttackMeleePS) pAttackMeleePS->SetPosition(mpCharBody->GetFeetPosition());
			}*/

		//Move towards player
		mpMover->MoveToPos(gpBase->mpPlayer->GetCharacterBody()->GetFeetPosition());

		//------------------------------

		kLuxOnMessage(eLuxEnemyMessage_TimeOut)
			if (mlTempVal == 0 && mbIsActualWraith)
			{
				mlTempVal = 1;
				PlayAnim("AttackMelee", false, 0.3f, false);
			}
			else if (mlTempVal == 0 && !mbIsActualWraith)
			{
				mlTempVal = 1;
				PlayAnim("ChargeAttack", false, 0.3f, false);
			}

		kLuxOnMessage(eLuxEnemyMessage_AnimationSpecialEvent)
			if (mlTempVal == 1)
			{
				if (msAttackMeleePS != "" && mbIsActualWraith)
				{
					 pAttackMeleePS = mpMap->GetWorld()->CreateParticleSystem("WraithMeleePS", msAttackMeleePS, 1);
					if (pAttackMeleePS) pAttackMeleePS->SetPosition(mpCharBody->GetFeetPosition());
				}
				Attack(mNormalAttackSize, mNormalAttackDamage, mfDamageMul);
			}

		kLuxOnMessage(eLuxEnemyMessage_AnimationOver)
			ChangeState(mPreviousState);


		//------------------------------

		////////////////////////
		// Overload global
		kLuxOnMessage(eLuxEnemyMessage_PlayerDetected)
		kLuxOnMessage(eLuxEnemyMessage_HelpMe)
	//------------------------------

	////////////////////////////////
	// Hurt
	////////////////////////////////

	//------------------------------

	kLuxState(eLuxEnemyState_Hurt)
		kLuxOnEnter
			mpPathfinder->Stop();
			PlayAnim("Flinch", false, 0.5f);
			SendMessage(eLuxEnemyMessage_TimeOut, 0.2f, true);
			mfFOVMul = 4.0f;

		kLuxOnLeave
			mfFOVMul = 1.0f;

	//------------------------------

		kLuxOnMessage(eLuxEnemyMessage_AnimationOver)
			if (PlayerIsDetected())
			{
				ChangeState(eLuxEnemyState_Hunt);
			}
			else
			{
				ChangeState(eLuxEnemyState_Alert);
			}

		kLuxOnMessage(eLuxEnemyMessage_TimeOut)
			if (cMath::RandRectl(0, 1) == 0 && DistToPlayer() < mfNormalAttackDistance * 1.3f)
			{
				ChangeState(eLuxEnemyState_AttackRange);
			}

		////////////////////////
		// Overload global
		kLuxOnMessage(eLuxEnemyMessage_PlayerDetected)
		kLuxOnMessage(eLuxEnemyMessage_HelpMe)
			
	//------------------------------

	////////////////////////////////
	// Flee
	////////////////////////////////

	//------------------------------
	kLuxState(eLuxEnemyState_Flee)
		///////////////////////
		// Enter
		kLuxOnEnter
			IncCurrentPatrolNode(true);

			if (FleeTryToFindSafeNode())
			{
				mfFleeCheckIfInvisbleCount = 0;
				mfFOVMul = 5;

				SetMoveSpeed(eLuxEnemyMoveSpeed_Run);
				mfForwardSpeed *= mfRunSpeedMul;
				
				if (!mbIsActualWraith && mPreviousState == eLuxEnemyState_AttackMeleeLong || !mbIsActualWraith && mPreviousState == eLuxEnemyState_AttackMeleeShort)
				{
					gpBase->mpMusicHandler->AddEnemy(eLuxEnemyMusic_Search, this);
					gpBase->mpMusicHandler->RemoveEnemy(eLuxEnemyMusic_Attack, this);
				}

				gpBase->mpPlayer->RemoveTerrorEnemy(this);

				SendMessage(eLuxEnemyMessage_TimeOut, 2.0f, true); //Check if player is too close!
				SendMessage(eLuxEnemyMessage_TimeOut_2, 0.2f, true); //Check if not seen by player

				//ChangeMoveType(eLuxEnemyMoveType_Flee);
			}
			else
			{
				ChangeState(eLuxEnemyState_Hunt);
			}

		///////////////////////
		// Leave
		kLuxOnLeave
			//ChangeMoveType(eLuxEnemyMoveType_Normal);
			mfFOVMul = 1;

		//------------------------------

		//////////////////
		// Check if player is too close!
		kLuxOnMessage(eLuxEnemyMessage_TimeOut)

			if (DistToPlayer2D() < 2.5f)
			{
				ChangeState(eLuxEnemyState_Hunt);
			}

			SendMessage(eLuxEnemyMessage_TimeOut, 1.0f, true);

		//------------------------------

		//////////////////
		// Check if no longer visible.
		kLuxOnMessage(eLuxEnemyMessage_TimeOut_2)

			if (PlayerIsDetected() == false)	mfFleeCheckIfInvisbleCount += 0.2f;
			else							mfFleeCheckIfInvisbleCount = 0;

			if (mfFleeCheckIfInvisbleCount >= 1.6)
			{
				if (!mbIsActualWraith && mPreviousState == eLuxEnemyState_AttackMeleeLong || !mbIsActualWraith && mPreviousState == eLuxEnemyState_AttackMeleeShort)
				{
					ChangeState(eLuxEnemyState_Search);
				}
				else 
				ChangeState(eLuxEnemyState_Wait);
			}
			else
			{
				SendMessage(eLuxEnemyMessage_TimeOut_2, 0.2f, true);
			}


		//------------------------------

		////////////////////////
		// End of current path
		kLuxOnMessage(eLuxEnemyMessage_EndOfPath)

			//Check if enemy can be auto removed, else continue fleeing or, go on patrol
			
			
			if (PlayerIsDetected())
			{
				if (FleeTryToFindSafeNode() == false)
						ChangeState(eLuxEnemyState_Alert);
				else if (FleeTryToFindSafeNode() == true)
						ChangeState(eLuxEnemyState_Search);
			}
			else
			{
				if (!mbIsActualWraith && mPreviousState == eLuxEnemyState_AttackMeleeLong || !mbIsActualWraith && mPreviousState == eLuxEnemyState_AttackMeleeShort)
				{
					ChangeState(eLuxEnemyState_Search);
				}
				else
					ChangeState(eLuxEnemyState_Wait);
			}
			

		////////////////////////
		// Overload global
		kLuxOnMessage(eLuxEnemyMessage_PlayerDetected)
		kLuxOnMessage(eLuxEnemyMessage_HelpMe)

	kLuxEndStateMachine

}
//-----------------------------------------------------------------------
void cLuxEnemy_Wraith::OnRenderSolidImplemented(cRendererCallbackFunctions* apFunctions)
{
	iPhysicsWorld* pPhysicsWorld = mpMap->GetPhysicsWorld();

	if (mCurrentState == eLuxEnemyState_AttackMeleeShort || mCurrentState == eLuxEnemyState_AttackMeleeLong)
	{
		pPhysicsWorld->RenderShapeDebugGeometry(GetAttackShape(0), GetDamageShapeMatrix(cVector3f(0, 0, 1)), apFunctions->GetLowLevelGfx(),
			cColor(1, 0, 0, 1));
	}

}

//-----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// PRIVATE METHODS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

bool cLuxEnemy_Wraith::PlayerIsDetected()
{
	if (CanSeePlayer())
	{
		return true;
	}
	else
	{
		return (DistToPlayer() < mpCharBody->GetSize().x && PlayerInFOV()) || (mfInLanternLightCount >= 1 && mbBlind == false);
	}
}

//-----------------------------------------------------------------------

void cLuxEnemy_Wraith::OnDisableTriggers()
{
	if (mCurrentState == eLuxEnemyState_Idle) return;

	ChangeState(eLuxEnemyState_Patrol);
}

//-----------------------------------------------------------------------

float cLuxEnemy_Wraith::GetDamageMul(float afAmount, int alStrength)
{
	if (mCurrentState == eLuxEnemyState_Idle ||
		mCurrentState == eLuxEnemyState_Wait ||
		mCurrentState == eLuxEnemyState_Patrol ||
		mCurrentState == eLuxEnemyState_Investigate)
	{
		return 1.0f;
	}
	if (mCurrentState == eLuxEnemyState_Hurt) return 0.5f;

	return 0.2f;
}

//-----------------------------------------------------------------------

void cLuxEnemy_Wraith::OnSetActiveEnemySpecific(bool abX)
{
		for (size_t i = 0; i < mvLights.size(); ++i)
		{
			mvLights[i]->SetVisible(abX);
		}	

		if(mpAttachEntity)
		{
			mpAttachEntity->SetActive(abX);
		}

		if (abX == true)
		{
			StartSoundLoops();
		}
		else
		{
			//StopSoundLoops();
			//mpMap->GetWorld()->SoundEntityExists(mpTeleportDashLoopSound, mTeleportDashLoopSoundId)
			//if (mpTeleportDashLoopSound != NULL)
			//{
				//mpTeleportDashLoopSound->FadeOut(0.5);
			//}
		}
}

//-----------------------------------------------------------------------

bool cLuxEnemy_Wraith::CheckEnemyAutoRemoval(float afDistance)
{
	if (mbIsSeenByPlayer == false && DistToPlayer() > afDistance && mbAutoRemoveAtPathEnd)
	{
		SetActive(false);

		RunCallbackFunc("OnAutoDisabled");

		return true;
	}
	return false;
}

//-----------------------------------------------------------------------

void cLuxEnemy_Wraith::PatrolUpdateGoal()
{
	if (mvPatrolNodes.empty())
	{
		ChangeState(eLuxEnemyState_Wait);	
		return;
	}
	
	cLuxEnemyPatrolNode* pPatrolNode = GetCurrentPatrolNode();
	//cVector3f pNode = pPatrolNode->mpNode->GetPosition(); 

	//float fNodeDis = gpBase->mpMapHelper->CheckDistance(mpCharBody->GetFeetPosition(), pNode);

	mpPathfinder->MoveTo(pPatrolNode->mpNode->GetPosition());
	
	if (mlStealthDashNodesLength > 0 && mbIsInStealthDashMode)
	{
		mlStealthDashNodesLength--;
	}
	else if (mlStealthDashNodesLength <= 0 && mbIsInStealthDashMode)
	{
		mlStealthDashNodesLength = mlStealthDashNodesLengthStart;
	}

	/*if (fNodeDis > mlEnterStealthDashNodeDistance && mbAllowedToDashAtWill && mbIsInStealthDashMode == false)
	{
		StartTeleportDash();
	}*/
}

//-----------------------------------------------------------------------

void cLuxEnemy_Wraith::StartTeleportDash()
{
	//////Vars
	mpMeshEntity->SetVisible(false);
	mpMover->SetWallAvoidanceActive(false);
	mpCharBody->SetCollideCharacter(false);
	//mpHeatRaySpotLight->SetVisible(false);
	mbPrevTriggers = GetDisableTriggers(); 
	mPrevPatrolSpeed = mPatrolMoveSpeed; 
	mPatrolMoveSpeed = eLuxEnemyMoveSpeed_Run; 
	mbPatrolMoveSpeedChanged = true;
	mbIsInStealthDashMode = true;
	mpCharBody->StopMovement();

	/////Lights
	for (size_t i = 0; i < mvLights.size(); ++i)
	{
		mvLights[i]->SetVisible(false);
	}
	TeleportLightCommand("InitalTeleportFlash");

	/////Actions
	PlayTeleportSound();
	StopSoundLoops();

	if (mCurrentState != eLuxEnemyState_Hunt && mCurrentState != eLuxEnemyState_HuntWander && 
		mCurrentState != eLuxEnemyState_HuntPause && mCurrentState != eLuxEnemyState_BreakDoor)
	{ 
		SetDisableTriggers(true);
	}
	
	////Attach
	//if (mpAttachEntity) mpMeshEntity->SetActive(false);

	////Particles 
	 pTeleportDashPS_Teleport = mpMap->GetWorld()->CreateParticleSystem(msName + "_TeleportDashTeleport", msTeleportDashPS_Teleport, 1);
	 pTeleportDashPS_Trail = mpMap->GetWorld()->CreateParticleSystem(msName + "_TeleportDashTrail", msTeleportDashPS_Trail, 1);
	
	 cVector3f mvEnemyPos = mpCharBody->GetPosition();
	if (pTeleportDashPS_Trail) pTeleportDashPS_Trail->SetPosition(mvEnemyPos);
	if (pTeleportDashPS_Teleport) pTeleportDashPS_Teleport->SetPosition(mvEnemyPos);

	////Sounds
	if (msTeleportingLoop != "") mpTeleportDashLoopSound = mpMap->GetWorld()->CreateSoundEntity("TeleportingDashLoop", msTeleportingLoop, false);
	if (mpMeshEntity) mpMeshEntity->AddChild(mpTeleportDashLoopSound);
	mTeleportDashLoopSoundId = mpTeleportDashLoopSound->GetCreationID();

	if (mpTeleportDashLoopSound)
	{
		mpTeleportDashLoopSound->FadeIn(1);
	}

}
//-----------------------------------------------------------------------
void cLuxEnemy_Wraith::StopTeleportDash()
{
	/////Vars
	mpMeshEntity->SetVisible(true);
	mpMover->SetWallAvoidanceActive(true);
	mpCharBody->SetCollideCharacter(true);
	//mpHeatRaySpotLight->SetVisible(true);
	mPatrolMoveSpeed = mPrevPatrolSpeed;
	mbIsInStealthDashMode = false;
	mbStealthDashMode = false;
	mbPatrolMoveSpeedChanged = true;
	mpCharBody->StopMovement();
	mpCharBody->SetMoveDelay(0.5);
	//mpPathfinder->Stop();

	///States///
	if (mCurrentState == eLuxEnemyState_Hunt)
	{
		ChangeState(eLuxEnemyState_HuntPause);
	}

	/////Lights
	for (size_t i = 0; i < mvLights.size(); ++i)
	{
		mvLights[i]->SetVisible(true);
	}
	
	//if(mbHeatRayLightConnectionSetup)mpHeatRaySpotLight->SetCastShadows(true);
	
	TeleportLightCommand("ExitTeleportFlash");

	////Actions
	PlayTeleportSound();
	SetDisableTriggers(mbPrevTriggers);
	StartSoundLoops();

	////Particles 
	if (pTeleportDashPS_Trail && mpWorld->ParticleSystemExists(pTeleportDashPS_Trail)) pTeleportDashPS_Trail->Kill();

	cVector3f mvEnemyPos = mpCharBody->GetPosition();
	pTeleportDashPS_Teleport = mpMap->GetWorld()->CreateParticleSystem(msName+"_TeleportDashTeleport", msTeleportDashPS_Teleport, 1);
	if (pTeleportDashPS_Teleport) pTeleportDashPS_Teleport->SetPosition(mvEnemyPos);

	////Attach 
	//if (mpAttachEntity) mpAttachEntity->SetActive(true);

	////Sounds
	if (mpTeleportDashLoopSound && mpMap->GetWorld()->SoundEntityExists(mpTeleportDashLoopSound, mTeleportDashLoopSoundId))
	{
		mpTeleportDashLoopSound->FadeOut(0.5);
	}
}
//-----------------------------------------------------------------------

void cLuxEnemy_Wraith::TeleportLightCommand(string asCommand)
{
	
	if (asCommand == "CreateTeleportLight")
	{
		mpTeleportLight = mpWorld->CreateLightPoint(msName+"_WraithTeleportLight", "", false);

		mpTeleportLight->SetDiffuseColor(mTeleportLightColor);
		mpTeleportLight->SetBrightness(0);
		mpTeleportLight->SetRadius(mfTeleportLightRadius);

		mpTeleportLight->SetCastShadows(false);
		mpTeleportLight->SetIsSaved(false);

		mpMeshEntity->AddChild(mpTeleportLight);
	}
	else if (asCommand == "InitalTeleportFlash")
	{
		if (!mpTeleportLight)
		{
			TeleportLightCommand("CreateTeleportLight");
		}

		//mpTeleportLight->SetDiffuseColor(cColor(0.4, 0.9, 1.0));
		mpTeleportLight->SetBrightness(mfTeleportLightBrightness);
		mpTeleportLight->FadeBrightnessTo(0, 0.15f);
	}
	else if (asCommand == "ExitTeleportFlash")
	{
		//mpTeleportLight->SetDiffuseColor(cColor(0.4, 0.9, 1.0));
		mpTeleportLight->SetBrightness(mfTeleportLightBrightness);
		mpTeleportLight->FadeBrightnessTo(0, 0.3f);
	}
}

//-----------------------------------------------------------------------

void cLuxEnemy_Wraith::FinishPatrolEndOfPath(bool callPatrolUpdateNow)
{
	cLuxEnemyPatrolNode* pNode = GetCurrentPatrolNode();

	if (!mbPathReversed && IsAtLastPatrolNode()
		|| mbPathReversed && IsAtFirstPatrolNode())
	{
		if (mbAutoReverseAtPathEnd)
		{
			mbPathReversed = !mbPathReversed;
		}
	}

	if (pNode)
		mfWaitTime = pNode->mfWaitTime;
	else
		mfWaitTime = 0;

	if (mbPathReversed)
	{
		DecCurrentPatrolNode(true);
	}
	else
	{
		IncCurrentPatrolNode(true);
	}

	if (mfWaitTime == 0 && mbAllowZeroWaitTime)
	{
		if (callPatrolUpdateNow)
		{
			PatrolUpdateGoal();
		}
	}
	else
	{

		ChangeState(eLuxEnemyState_Wait);
	}
}

//-----------------------------------------------------------------------

bool cLuxEnemy_Wraith::FleeTryToFindSafeNode()
{
	float fLength = 20;
	cVector3f vDirToPlayer = GetDirection2DToPlayer();

	//Try a couple of times to find a good node!
	for (int i = 0; i < 10; ++i)
	{
		/////////////////////////////////////
		//Find a node to run to
		cAINode* pNode = mpPathfinder->GetNodeAtPos(mpCharBody->GetPosition() + vDirToPlayer * fLength * 0.2f,
			0, fLength, false, false, true, NULL);
		if (pNode == NULL && DistToPlayer2D(pNode->GetPosition()) < 4.0f)
		{
			continue;
		}

		/////////////////////////////////////
		//See if first node takes you away from player
		mpPathfinder->MoveTo(pNode->GetPosition());
		cVector3f vDirToNode = GetDirection2D(mpPathfinder->GetNextGoalPos());
		if (cMath::Vector3Angle(vDirToPlayer, vDirToNode) < cMath::ToRad(45))
		{
			mpPathfinder->Stop();
			continue;
		}

		//Path was found!
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------

void cLuxEnemy_Wraith::PlayHuntSound()
{
	if (mpHuntSound && mpWorld->SoundEntityExists(mpHuntSound, mlHuntSoundId)) return;

	mpHuntSound = PlaySound(msChaseSound);
	if (mpHuntSound) mlHuntSoundId = mpHuntSound->GetCreationID();
}

void cLuxEnemy_Wraith::PlayTeleportSound()
{
	if (mpTeleportSound && mpWorld->SoundEntityExists(mpTeleportSound, mlTeleportSoundId)) return;

	mpTeleportSound = PlaySound(msTeleportSound);
	if (mpTeleportSound) mlTeleportSoundId = mpTeleportSound->GetCreationID();
}

//-----------------------------------------------------------------------
bool cLuxEnemy_Wraith::InsidePlayerView()
{
	cFrustum* pFrust = gpBase->mpPlayer->GetCamera()->GetFrustum();

	//Log("Check inside ray:\n");

	/////////////////////////
	//If near enough, just check if in frustum
	if (cMath::Vector3Dist(mpCharBody->GetPosition(), gpBase->mpPlayer->GetCharacterBody()->GetPosition()) < 2.25f)
	{
		//Log(" true: close!\n");
		return pFrust->CollideBoundingVolume(mpCharBody->GetCurrentBody()->GetBoundingVolume()) != eCollision_Outside;
	}

	/////////////////////////
	//Check inside frustum
	if (pFrust->CollideBoundingVolume(mpCharBody->GetCurrentBody()->GetBoundingVolume()) != eCollision_Inside)
	{
		//Log(" false: outside frustum!\n");
		return false;
	}

	/////////////////////////
	//Check line of sight
	cVector3f vStart = pFrust->GetOrigin();
	cVector3f vPositions[3];
	vPositions[0] = mpCharBody->GetPosition();

	cVector3f vSideAdd = mpCharBody->GetRight() * mpCharBody->GetSize().x * 0.4f;
	vPositions[1] = mpCharBody->GetPosition() + vSideAdd;
	vPositions[2] = mpCharBody->GetPosition() - vSideAdd;

	for (int i = 0; i < 3; ++i)
	{
		if (gpBase->mpMapHelper->CheckLineOfSight(vStart, vPositions[i], true))
		{
			//Log(" true: ray %d!\n", i);
			return true;
		}
	}

	//Log(" false: no rays!\n");
	return false;
}
//-----------------------------------------------------------------------
bool cLuxEnemy_Wraith::CanUseRangedAttacks()
{
	if (mAttackType == eLuxAttackType_Archvile || mAttackType == eLuxAttackType_HeatRay || mAttackType == eLuxAttackType_Projectile)
	{
		return true;
	}
	else
	{
		return false;
	}
}
//-----------------------------------------------------------------------
bool cLuxEnemy_Wraith::PlayerInLOS()
{
	iCharacterBody* pPlayerBody = gpBase->mpPlayer->GetCharacterBody();

	return PlayerInFOV();

}
//-----------------------------------------------------------------------
void cLuxEnemy_Wraith::CreateArchvileFlameLight()
{
	if (!mpFlameLight)
	{

		if (msFlameLightName != "")
		{
			for (size_t i = 0; i < mvLights.size(); ++i)
			{
				if (mvLights[i]->GetName() == msFlameLightName && mvLights[i]->GetLightType() == eLightType_Point)
				{
					mpFlameLight = static_cast<cLightPoint*>(mvLights[i]);
				}
			}
		}
		else
		{

			cColor offColor = mAVFlameLightColorFlickerOffColor;
			float offRadius = mfAVFlameLightFlickerOffRadius;
			float OnMin = mfAVFlameLightFlickerOnMinLength;
			float OnMax = mfAVFlameLightFlickerOnMaxLength;
			float OffMin = mfAVFlameLightFlickerOffMinLength;
			float OffMax = mfAVFlameLightFlickerOffMaxLength;
			float FadeOnMin = mfAVFlameLightFlickerOnFadeMinLength;
			float FadeOnMax = mfAVFlameLightFlickerOnFadeMaxLength;
			float FadeOffMin = mfAVFlameLightFlickerOffFadeMinLength;
			float FadeOffMax = mfAVFlameLightFlickerOffFadeMaxLength;

			mpFlameLight = mpWorld->CreateLightPoint(msName + "_ArchVilePlayerFlameLight", "", false);

			mpFlameLight->SetDiffuseColor(mAVFlameLightColor);
			mpFlameLight->SetRadius(0);
			mpFlameLight->FadeTo(mpFlameLight->GetDiffuseColor(), mfAVFlameLightRadius, 1);
			mpFlameLight->SetFlickerActive(false);
			mpFlameLight->SetFlicker(offColor, offRadius, OnMin, OnMax, "", "", OffMin, OffMax, "", "", true, FadeOnMin, FadeOnMax, FadeOffMin, FadeOffMax);
			mpFlameLight->SetBrightness(mfAVFlameLightBrightness);

			mpFlameLight->SetCastShadows(false);
			mpFlameLight->SetIsSaved(false);
		}
	}
}
//-----------------------------------------------------------------------
void cLuxEnemy_Wraith::UpdateAttack(float afTimeStep)
{
	if (mAttackType == eLuxAttackType_Archvile)
	{
		if (!mbIsInArchvileAttack && mbFlameLightIsOn)
		{
			if (!mbFlameLightFadeStarted)
			{
				mpFlameLight->FadeTo(cColor(0, 0, 0), mfAVFlameLightRadius, 1);
				gpBase->mpDebugHandler->AddMessage(_W("Flame Light Fade to Black!"), true);
			}
			mpFlameLight->SetFlickerActive(false);
			mfFlameLightFadeTime -= afTimeStep;
			if (mfFlameLightFadeTime <= 0)
			{
				ResetAttack();
			}
		}
		else
		{
			if (!mbFlameLightIsOn)
			{
				if (!mbFlameLightFadeStarted)
				{
					mpFlameLight->FadeTo(mAVFlameLightColor, mfAVFlameLightRadius, 1);
					mbFlameLightFadeStarted = true;
					gpBase->mpDebugHandler->AddMessage(_W("Flame Light Fade to Color!"), true);
				}
				mfFlameLightFadeTime -= afTimeStep;
				if (mfFlameLightFadeTime <= 0)
				{
					mpFlameLight->SetFlickerActive(true);
					mfFlameLightFadeTime = 0;
					mbFlameLightIsOn = true;
					mbFlameLightFadeStarted = false;
				}
			}
			else
			{
				///////////////////////
				// Update ambient light position.
				cVector3f vCamPos = gpBase->mpPlayer->GetCamera()->GetPosition();
				mpFlameLight->SetPosition(vCamPos - cVector3f(0, -0.3f, 0));
				if (pArchvilePS_AttackStart) pArchvilePS_AttackStart->SetPosition(GetPlayerFeetPosition()+mvArchvilePS_AttackStartOffset);
				
			}
		}
	}
	//BREAK
	if (mAttackType == eLuxAttackType_HeatRay)
	{
		float mfHeatRayLightLevel = GetPlayerInHeatRayLightLevel();
		float mfInitalDamage = mfHeatRayLightLevel;

		if (mfHeatRayLightLevel >= mfLightLevelDamageStart && mbHeatRayLightConnectionSetup == true)
		{
			if (mfStartHeatRayDamage < 1)
			{
				mfStartHeatRayDamage += afTimeStep * 0.3f;

				if (mfStartHeatRayDamage > 1)
				{
					mfStartHeatRayDamage = 1;
				}
			}
			
			gpBase->mpPlayer->GiveDamageAlt(mfInitalDamage *= mfHeatRayDamageSpeed * mfDamageMul * afTimeStep, mlRangedAttackStrength, eLuxDamageType_Blank, false, true);

			float fPulse = 0.5f + (sin(mfHeatRayLightLevel * 2.5f) * 0.5f + 0.5f) * 0.5f;

			gpBase->mpEffectHandler->GetRadialBlur()->SetBlurStartDist(0.2f);
			gpBase->mpEffectHandler->GetRadialBlur()->FadeTo(0.12f * mfStartHeatRayDamage * fPulse, 10.0f);
			PlayHeatRaySounds(true);
		}
		else
		{
			if (mfStartHeatRayDamage > 0)
			{
				mfStartHeatRayDamage -= afTimeStep * 0.15f;
				if (mfStartHeatRayDamage < 0)mfStartHeatRayDamage = 0;
			}

			ResetAttack();
		}
	}
	//BREAK
	if (mAttackType == eLuxAttackType_HeatRay && mbHeatRayLightConnectionSetup == true && GetPlayerInHeatRayLightLevel() >= mfLightLevelDamageStart 
		||mAttackType == eLuxAttackType_Archvile && mbHeatRayLightConnectionSetup == true && pArchvilePS_AttackStart != NULL && mbIsInArchvileAttack)
	{
		mfBurnDamageCountdown -= afTimeStep;
		if (mfBurnDamageCountdown > 0)	return;
		
		mfBurnDamageCountdown = cMath::RandRectf(mfBurnDamageMinTime, mfBurnDamageMaxTime);

		gpBase->mpPlayer->GiveDamage(cMath::RandRectf(mfMinBurnDamage, mfMaxBurnDamage)* mfDamageMul, 1, eLuxDamageType_BloodSplat, false, true);
		PlaySound(msBurnDamageSound);
		
	}
}
//-----------------------------------------------------------------------
void cLuxEnemy_Wraith::ResetAttack()
{
	//mfBurnDamageCountdown = 0;

	if (mAttackType == eLuxAttackType_Archvile)
	{
		mbFlameLightIsOn = false;
		mbFlameLightFadeStarted = false;
		mfFlameLightFadeTime = 0;

		if (mpFlameLight)
		{
			mpFlameLight->SetRadius(0);
			gpBase->mpDebugHandler->AddMessage(_W("Flame Light Disable!"), true);
		}
	}
	//BREAK
	if (mAttackType == eLuxAttackType_HeatRay)
	{
		//mfStartHeatRayDamage = 3.0f;
		PlayHeatRaySounds(false);
		//gpBase->mpEffectHandler->GetRadialBlur()->FadeTo(0, 10.0f);
		gpBase->mpEffectHandler->GetRadialBlur()->FadeTo(0, 0.12f / 2.0f);
	}
}
//-----------------------------------------------------------------------
void cLuxEnemy_Wraith::EnableArchvileAttackParticle(bool abX)
{
	if (abX == true)
	{
		if (pArchvilePS_AttackStart)
		{
			pArchvilePS_AttackStart->SetPosition(GetPlayerFeetPosition()+mvArchvilePS_AttackStartOffset);
		}
		else
		{
			pArchvilePS_AttackStart = mpMap->GetWorld()->CreateParticleSystem(msName+"_ArchvileAttack", msArchvilePS_AttackStart, 1);
		}
		
	}
	else if (abX == false)
	{
		if (pArchvilePS_AttackStart && mpWorld->ParticleSystemExists(pArchvilePS_AttackStart))
		{
			pArchvilePS_AttackStart->Kill();
			pArchvilePS_AttackStart = NULL;
		}
	}
}
//-----------------------------------------------------------------------
void cLuxEnemy_Wraith::SetUpHeatRayConnection()
{
	if (msHeatRaySpotlight != "")
	{
		iLight* pHeatLight = NULL;
		//Iterate all lights and get light with name and with NO parent (otherwise a light connected to an entity is gotten).
		cLightListIterator lightIt = mpWorld->GetLightIterator();
		for (size_t i = 0; i < mvLights.size(); ++i)
		{
			if (mvLights[i]->GetName() == msName + cString::To8Char(_W("_")) + msHeatRaySpotlight)
			{
				pHeatLight = mvLights[i];
				break;
			}
		}

		if (pHeatLight)
		{
			mpHeatRayLight = pHeatLight;
			mfHeatRayRadius = pHeatLight->GetRadius();
			if (pHeatLight->GetLightType() == eLightType_Spot)
			{
				cLightSpot* pSpotLight = static_cast<cLightSpot*>(pHeatLight);
				mpHeatRaySpotLight = pSpotLight;
				mfSpotlightDefaultBrightness = mpHeatRaySpotLight->GetBrightness();
					
				SetHeatRaySpotlightColor(mHeatRaySpotlightColorIdle);
			}
		}
		mbHeatRayLightConnectionSetup = true; 
	}
	mfBurnDamageCountdown = cMath::RandRectf(mfBurnDamageMinTime, mfBurnDamageMaxTime);
}
//-----------------------------------------------------------------------
float cLuxEnemy_Wraith::GetPlayerInHeatRayLightLevel()
{
		////////////////////////////////
		//Get character body properties
		iCharacterBody* pCharBody = gpBase->mpPlayer->GetCharacterBody();
		cVector3f vPos = pCharBody->GetPosition();
		cVector3f vSize = pCharBody->GetSize();
		cVector3f vForward = pCharBody->GetForward();

		////////////////////////////////
		//Set up positions to test light level at.
		const int lTestPos = 5;
		cVector3f vTestPos[lTestPos] =
		{
			vPos,	 //Center
			vPos + cVector3f(0,vSize.y - 0.1f, 0),	//Above feet
			vPos - cVector3f(0,vSize.y - 0.1f, 0),	//Head
			vPos + vForward * vSize.z * 0.8f,			//In front of center
			vPos - cVector3f(0,vSize.y - 0.1f, 0) + vForward * vSize.z * 0.8f //In front of feet.
		};

		////////////////////////////////
		//Get lights to skip
		tLightList lstWorldLights;
		std::vector<iLight*> vSkipLights;
		vSkipLights.push_back(gpBase->mpPlayer->GetHelperInDarkness()->GetAmbientLight());
		if(gpBase->mpPlayer->GetHelperLantern()->IsActive()) vSkipLights.push_back(gpBase->mpPlayer->GetHelperLantern()->GetLight());

		tLightListIt it = lstWorldLights.begin();
		for (; it != lstWorldLights.end(); ++it)
		{
			iLight* pLight = *it;

			if (pLight->GetParentEntity()->GetName() == msName) continue;

			if (pLight == mpHeatRayLight) continue;

			for (size_t i = 0; i < mvLights.size(); ++i)
			{
				if (pLight == mvLights[i]) continue; 
			}

			vSkipLights.push_back(pLight);
		}
		////////////////////////////////

		for (int i = 0; i < lTestPos; ++i)
		{
			//fTotalLight += gpBase->mpMapHelper->GetLightLevelAtPos(vTestPos[i], &vSkipLights);
			mfHRSpotlightLightLevel = gpBase->mpMapHelper->GetLightLevelAtPos(vTestPos[i], &vSkipLights, 0);
		}

		return mfHRSpotlightLightLevel;

}
//-----------------------------------------------------------------------
void cLuxEnemy_Wraith::PlayHeatRaySounds(bool abX)
{
	cSoundHandler* pSoundHandler = gpBase->mpEngine->GetSound()->GetSoundHandler();
	float mfHeatRaySoundMul = GetPlayerInHeatRayLightLevel();
	
	if (abX == true)
	{
		if (mpHeatRayBurnSound == NULL)
		{
			if (msHeatRayBurnLoop != "")
			{
				mpHeatRayBurnSound = mpMap->GetWorld()->CreateSoundEntity(msName+"_HeatRay", msHeatRayBurnLoop, false);
				if (mpHeatRayBurnSound)
				{
					mlHeatRayBurnSoundId = mpHeatRayBurnSound->GetCreationID();
					mpHeatRayBurnSound->FadeIn(1.0f);
				}
			}
		}

		mbHeatRaySoundIsPlaying = true;
	}
	else
	{
		if (mpHeatRayBurnSound)
		{
			
			if (mpMap->GetWorld()->SoundEntityExists(mpHeatRayBurnSound, mlHeatRayBurnSoundId))
			{
				mpHeatRayBurnSound->FadeOut(1);
			}
			mpHeatRayBurnSound = NULL;
		}
		mbHeatRaySoundIsPlaying = false;
	}
}
//-----------------------------------------------------------------------

void cLuxEnemy_Wraith::SetHeatRaySpotlightColor(cColor aColor)
{
	if (!mbHeatRayLightConnectionSetup) return; 


		for (size_t i = 0; i < mvLights.size(); ++i)
		{
			if (mvLights[i]->GetLightType() == eLightType_Spot)
			{
				mvLights[i]->FadeTo(aColor, mvLights[i]->GetRadius(), 1);

				if (mvLights[i]->GetColor() == mHeatRaySpotlightColor)
				{
					if (mvLights[i] == mpHeatRayLight)
					{
						mvLights[i]->FadeBrightnessTo(30, 1);
						gpBase->mpDebugHandler->AddMessage(_W("Fade Brightnes to 30!"), false);
					}
					else
					{
						mvLights[i]->FadeBrightnessTo(mfSpotlightDefaultBrightness, 1);
						gpBase->mpDebugHandler->AddMessage(_W("Fade Brightnes to Default!"), false);
					}
				}
			}
			else
			{
				mvLights[i]->FadeTo(mvDefaultLightColors[i] * aColor * 2, mvLights[i]->GetRadius(), 1);
			}
		}
	
	
}

void cLuxEnemy_Wraith::SetHeatRaySpotlightFOV(float afFOV)
{
	if (!mbHeatRayLightConnectionSetup ||!mpHeatRaySpotLight) return;
	
	
	float fFOV = cMath::ToRad(afFOV);
	//if (mpHeatRayLight->IsVisible() == false) mpHeatRaySpotLight->SetVisible(true);

	//mpHeatRaySpotLight->SetFOV(fFOV);
}

//-----------------------------------------------------------------------
void cLuxEnemy_Wraith::StartSoundLoops()
{
	if (msSpotlightLoop != "")
	{
		cLuxMap* pMap = gpBase->mpMapHandler->GetCurrentMap();
		mpSpotlightLoopSound = pMap->GetWorld()->CreateSoundEntity(msName+"_SpotlightLoop", msSpotlightLoop, false);
		if(mpMeshEntity) mpMeshEntity->AddChild(mpSpotlightLoopSound);
		mlSpotlightLoopSoundID = mpSpotlightLoopSound->GetCreationID();
	}

	if (mpCurrentLoopSound)
	{
		mpCurrentLoopSound->FadeIn(1);
	}
}

void cLuxEnemy_Wraith::StopSoundLoops()
{
	if (mpSpotlightLoopSound && mpMap->GetWorld()->SoundEntityExists(mpSpotlightLoopSound, mlSpotlightLoopSoundID))
	{
		mpSpotlightLoopSound->FadeOut(1);
	}
	//&& mpMap->GetWorld()->SoundEntityExists(mpSpotlightLoopSound, mlSpotlightLoopSoundID)

	if (mpCurrentLoopSound && mpMap->GetWorld()->SoundEntityExists(mpCurrentLoopSound, mlCurrentLoopSoundID))
	{
		mpCurrentLoopSound->FadeOut(1);
	}

}
//-----------------------------------------------------------------------

void cLuxEnemy_Wraith::UpdateSoundLoops(float afTimeStep)
{
	
	if (gpBase->mpPlayer->IsDead() || mbIsInStealthDashMode || IsActive() == false)
	{
		mpCurrentLoopSound->FadeOut(1);
		//mpCurrentLoopSound = NULL;
		
	}

	eLuxEnemyState pCurrentState = GetCurrentEnemyState();
	if (pCurrentState != mPrevLoopState && mbIsInStealthDashMode == false || mpCurrentLoopSound == NULL && mbIsInStealthDashMode == false)
	{
		
		float fCrossFadeTime = pCurrentState != mPrevLoopState ? 0.5f : 2.0f;

		if (mpCurrentLoopSound && mpMap->GetWorld()->SoundEntityExists(mpCurrentLoopSound, mlCurrentLoopSoundID))
		{
			mpCurrentLoopSound->FadeOut(fCrossFadeTime);
		}
		//Idle Still Sound///
		if (pCurrentState == eLuxEnemyState_Idle || pCurrentState == eLuxEnemyState_Wait)
		{
			mpCurrentLoopSound = mpMap->GetWorld()->CreateSoundEntity(msName+"_idle_still", msIdleStillLoop, false);
		}
		//Idle Moving Sound///
		else if (pCurrentState == eLuxEnemyState_Patrol)
		{
			mpCurrentLoopSound = mpMap->GetWorld()->CreateSoundEntity(msName+"_idle_move", msIdleMoveLoop, false);
		}
		//Investigate & Search Sound//
		else if (pCurrentState == eLuxEnemyState_Investigate)
		{
			mpCurrentLoopSound = mpMap->GetWorld()->CreateSoundEntity(msName+"_search_low", msSearchLoop, false);
		}
		//Hunt Sound//
		else if (pCurrentState == eLuxEnemyState_Search || pCurrentState == eLuxEnemyState_Hunt)
		{
			 mpCurrentLoopSound = mpMap->GetWorld()->CreateSoundEntity(msName+"_hunt", msChaseLoop, false);
		}

		if (mpCurrentLoopSound)
		{
			mpCurrentLoopSound->FadeIn(1.0f);
			mlCurrentLoopSoundID = mpCurrentLoopSound->GetCreationID();
		}

		if (mpMeshEntity)
		{
			mpMeshEntity->AddChild(mpCurrentLoopSound);
			//mpCurrentLoopSound->SetPosition(mpMeshEntity->GetWorldPosition());

		}
	}

	if (mbIsInStealthDashMode && msTeleportingLoop != "" && !mpTeleportDashLoopSound)
	{
		mpTeleportDashLoopSound = mpMap->GetWorld()->CreateSoundEntity(msName+"_TeleportingDashLoop", msTeleportingLoop, false);
		if (mpMeshEntity) mpMeshEntity->AddChild(mpTeleportDashLoopSound);
		mTeleportDashLoopSoundId = mpTeleportDashLoopSound->GetCreationID();
	}

	if (msSpotlightLoop != "" && !mbSpotlightSetup)
	{
		cLuxMap* pMap = gpBase->mpMapHandler->GetCurrentMap();
		mpSpotlightLoopSound = pMap->GetWorld()->CreateSoundEntity(msName+"_SpotlightLoop", msSpotlightLoop, false);
		if (mpMeshEntity) mpMeshEntity->AddChild(mpSpotlightLoopSound);
		mlSpotlightLoopSoundID = mpSpotlightLoopSound->GetCreationID();
		mbSpotlightSetup = true;
	}
	mPrevLoopState = pCurrentState;
}

void cLuxEnemy_Wraith::UpdateHeatRaySound(float afTimeStep)
{
	if (!mbHeatRaySoundIsPlaying) return;

	/*float mfHeatRaySoundMul = GetPlayerInHeatRayLightLevel();

	if (mpHeatRayBurnSound)
	{
		if (mAttackType = eLuxAttackType_HeatRay)
		{
			mpHeatRayBurnSound->FadeVolumeMulTo(0.1f + afTimeStep * 0.9+mfHeatRaySoundMul, 0.25f);
		}
	}*/
}

cVector3f cLuxEnemy_Wraith::GetPlayerFeetPosition()
{
	cVector3f mvPlayerPos = gpBase->mpPlayer->GetCharacterBody()->GetFeetPosition();

	return mvPlayerPos; 
}
//-----------------------------------------------------------------------
void cLuxEnemy_Wraith::SetUpAttachedEntity()
{
	if (msAttachEntity != "" && msAttachmentBone != "")
	{

	}
}
//-----------------------------------------------------------------------
void cLuxEnemy_Wraith::SetupGroundWallAvoidance()
{
	mpMover->SetupWallAvoidance(2.0f, 2.0f, 4);
}

void cLuxEnemy_Wraith::SetupFlyWallAvoidance()
{
	mpMover->SetupWallAvoidance(4.0f, 10.0f, 4);
}
//-----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// SAVE DATA STUFF
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------
kBeginSerialize(cLuxEnemy_Wraith_SaveData, iLuxEnemy_SaveData)

kSerializeVar(mbThreatenOnAlert, eSerializeType_Bool)
kSerializeVar(mbPlayActivateSound, eSerializeType_Bool)
kSerializeVar(mfLanternSensitivity, eSerializeType_Float32)
kSerializeVar(mPatrolMoveSpeed, eSerializeType_Int32)
kSerializeVar(mPrevPatrolSpeed, eSerializeType_Int32)
kSerializeVar(mlStealthDashNodesLength, eSerializeType_Int32)
kSerializeVar(mlStealthDashNodesLengthStart, eSerializeType_Int32)
kSerializeVar(mfEnterStealthDashNodeDistance, eSerializeType_Float32)
kSerializeVar(mfExitStealthDashNodeDistance, eSerializeType_Float32)
kSerializeVar(mfDamageMul, eSerializeType_Float32)
kSerializeVar(mfRunSpeedMul, eSerializeType_Float32)
kSerializeVar(mfInLanternLightCount, eSerializeType_Float32)
kSerializeVar(mbAllowZeroWaitTime, eSerializeType_Bool)
kSerializeVar(mfHuntPauseTimeMul, eSerializeType_Float32)
kSerializeVar(mbFlyMode, eSerializeType_Bool)
kSerializeVar(mbStealthDashMode, eSerializeType_Bool)
kSerializeVar(mbPathReversed, eSerializeType_Bool)
kSerializeVar(mbCanMeele, eSerializeType_Bool)
kSerializeVar(mlAttackType, eSerializeType_Int32)
kSerializeVar(mfHRSpotlightLightLevel, eSerializeType_Float32)
kSerializeVar(mfStartHeatRayDamage, eSerializeType_Float32)
kSerializeVar(mfLightLevelDamageStart, eSerializeType_Float32)
kSerializeVar(mfHeatRayDamageSpeed, eSerializeType_Float32)
kSerializeVar(mfBurnDamageMinTime, eSerializeType_Float32)
kSerializeVar(mfBurnDamageMaxTime, eSerializeType_Float32)
kSerializeVar(mfMinBurnDamage, eSerializeType_Float32)
kSerializeVar(mfMaxBurnDamage, eSerializeType_Float32)
kSerializeVar(mfBurnDamageCountdown, eSerializeType_Float32)
kSerializeVar(mbIsInStealthDashMode, eSerializeType_Bool) 
kSerializeVar(mbAllowedToDashAtWill, eSerializeType_Bool)
kSerializeVar(mbPrevTriggers, eSerializeType_Bool)
kSerializeVar(mbStealthDashSetup, eSerializeType_Bool)
kSerializeVar(mbHeatRayLightConnectionSetup, eSerializeType_Bool)
kSerializeVar(mfSpotlightDefaultBrightness, eSerializeType_Float32)
kSerializeVar(mbIsInArchvileAttack, eSerializeType_Bool)

kEndSerialize()
//-----------------------------------------------------------------------
iLuxEntity_SaveData* cLuxEnemy_Wraith::CreateSaveData()
{
	return hplNew(cLuxEnemy_Wraith_SaveData, ());
}
//-----------------------------------------------------------------------
void cLuxEnemy_Wraith::SaveToSaveData(iLuxEntity_SaveData* apSaveData)
{
	//////////////////
	//Init
	super_class::SaveToSaveData(apSaveData);
	cLuxEnemy_Wraith_SaveData* pData = static_cast<cLuxEnemy_Wraith_SaveData*>(apSaveData);

	//////////////////
	//Set variables
	kCopyToVar(pData, mbThreatenOnAlert);
	kCopyToVar(pData, mfLanternSensitivity);
	kCopyToVar(pData, mPatrolMoveSpeed);
	kCopyToVar(pData, mPrevPatrolSpeed);
	kCopyToVar(pData, mlStealthDashNodesLength);
	kCopyToVar(pData, mlStealthDashNodesLengthStart);
	kCopyToVar(pData, mfEnterStealthDashNodeDistance);
	kCopyToVar(pData, mfExitStealthDashNodeDistance);
	kCopyToVar(pData, mfRunSpeedMul);
	kCopyToVar(pData, mfDamageMul);
	kCopyToVar(pData, mfHeatRayDamageSpeed);
	kCopyToVar(pData, mfBurnDamageMinTime);
	kCopyToVar(pData, mfBurnDamageMaxTime);
	kCopyToVar(pData, mfMinBurnDamage);
	kCopyToVar(pData, mfMaxBurnDamage);
	kCopyToVar(pData, mfBurnDamageCountdown);
	kCopyToVar(pData, mfInLanternLightCount);
	kCopyToVar(pData, mbAllowZeroWaitTime);
	kCopyToVar(pData, mfHuntPauseTimeMul);
	kCopyToVar(pData, mbPlayActivateSound);
	kCopyToVar(pData, mbFlyMode);
	kCopyToVar(pData, mbStealthDashMode);
	kCopyToVar(pData, mbPathReversed);
	kCopyToVar(pData, mbCanMeele);
	kCopyToVar(pData, mbPrevTriggers);
	kCopyToVar(pData, mbAllowedToDashAtWill);
	kCopyToVar(pData, mbIsInStealthDashMode);
	kCopyToVar(pData, mfHRSpotlightLightLevel);
	kCopyToVar(pData, mfStartHeatRayDamage);
	kCopyToVar(pData, mfLightLevelDamageStart);
	kCopyToVar(pData, mbIsInArchvileAttack);
	kCopyToVar(pData, mbStealthDashSetup);
	kCopyToVar(pData, mbHeatRayLightConnectionSetup);
	kCopyToVar(pData, mfSpotlightDefaultBrightness);

	pData->mlAttackType = mAttackType;

}
//-----------------------------------------------------------------------

void cLuxEnemy_Wraith::LoadFromSaveData(iLuxEntity_SaveData* apSaveData)
{
	//////////////////
	//Init
	super_class::LoadFromSaveData(apSaveData);
	cLuxEnemy_Wraith_SaveData* pData = static_cast<cLuxEnemy_Wraith_SaveData*>(apSaveData);

	//////////////////
	//Set variables
	kCopyFromVar(pData, mbThreatenOnAlert);
	kCopyFromVar(pData, mbPlayActivateSound);
	kCopyFromVar(pData, mfLanternSensitivity);
	mPatrolMoveSpeed = (eLuxEnemyMoveSpeed)pData->mPatrolMoveSpeed;
	mPrevPatrolSpeed = (eLuxEnemyMoveSpeed)pData->mPrevPatrolSpeed;
	kCopyFromVar(pData, mlStealthDashNodesLength);
	kCopyFromVar(pData, mlStealthDashNodesLengthStart);
	kCopyFromVar(pData, mfEnterStealthDashNodeDistance);
	kCopyFromVar(pData, mfExitStealthDashNodeDistance);
	kCopyFromVar(pData, mfRunSpeedMul);
	kCopyFromVar(pData, mfDamageMul);
	kCopyFromVar(pData, mfHeatRayDamageSpeed);
	kCopyFromVar(pData, mfBurnDamageMinTime);
	kCopyFromVar(pData, mfBurnDamageMaxTime);
	kCopyFromVar(pData, mfMinBurnDamage);
	kCopyFromVar(pData, mfMaxBurnDamage);
	kCopyFromVar(pData, mfBurnDamageCountdown);
	kCopyFromVar(pData, mfInLanternLightCount);
	kCopyFromVar(pData, mbAllowZeroWaitTime);
	kCopyFromVar(pData, mfHuntPauseTimeMul);
	kCopyFromVar(pData, mbFlyMode);
	kCopyFromVar(pData, mbStealthDashMode);
	kCopyFromVar(pData, mbPathReversed);
	kCopyFromVar(pData, mbCanMeele);
	kCopyFromVar(pData, mbPrevTriggers);
	kCopyFromVar(pData, mbAllowedToDashAtWill);
	kCopyFromVar(pData, mbIsInStealthDashMode);
	kCopyFromVar(pData, mfHRSpotlightLightLevel);
	kCopyFromVar(pData, mfStartHeatRayDamage); 
	kCopyFromVar(pData, mfLightLevelDamageStart);
	kCopyFromVar(pData, mbIsInArchvileAttack);
	kCopyFromVar(pData, mbStealthDashSetup);
	kCopyFromVar(pData, mbHeatRayLightConnectionSetup);
	kCopyFromVar(pData, mfSpotlightDefaultBrightness);

	mAttackType = (eLuxAttackType)pData->mlAttackType;

	////////////////////////
	// Handle changed enums
	

}
//-----------------------------------------------------------------------
void cLuxEnemy_Wraith::SetupSaveData(iLuxEntity_SaveData* apSaveData)
{
	super_class::SetupSaveData(apSaveData);
}

//-----------------------------------------------------------------------