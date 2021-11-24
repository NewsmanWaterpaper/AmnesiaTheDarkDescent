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
#ifndef LUX_ENEMY_WRAITH_H
#define LUX_ENEMY_WRAITH_H
 //----------------------------------------------
#include "LuxEnemy.h"
//----------------------------------------------
enum eLuxAttackType
{
	eLuxAttackType_Archvile,
	eLuxAttackType_HeatRay,
	eLuxAttackType_Projectile,
	eLuxAttackType_Melee,

	eLuxAttackType_LastEnum
};
//----------------------------------------------
class cLuxEnemy_Wraith_SaveData : public iLuxEnemy_SaveData
{
	kSerializableClassInit(cLuxEnemy_Wraith_SaveData)
public:
	////////////////
	//Properties
	bool mbThreatenOnAlert;
	
	bool mbPlayActivateSound;
	float mfLanternSensitivity;

	bool mbAllowZeroWaitTime;

	bool mbFlyMode;
	bool mbStealthDashMode;
	bool mbPathReversed;
	bool mbCanMeele;
	bool mbIsInStealthDashMode;
	bool mbAllowedToDashAtWill;
	bool mbPrevTriggers;
	
	int mlStealthDashNodesLength;
	int mlStealthDashNodesLengthStart;
	float mlEnterStealthDashNodeDistance;
	float mlExitStealthDashNodeDistance;

	float mfInLanternLightCount;
	float mfHuntPauseTimeMul;
	float mfHRSpotlightLightLevel;
	float mfStartHeatRayDamage;

	bool mbIsInArchvileAttack;
	bool mbHeatRaySoundIsPlaying;
	bool mbFlameLightIsOn;
	float mfFlameLightFadeTime;
	bool mbFlameLightFadeStarted;

	int mPrevPatrolSpeed;
	//float  mfFleeHealth;
	float mfDamageMul;
	int mlAttackType;

};
//----------------------------------------------

class cLuxEnemy_Wraith : public iLuxEnemy
{
friend class cLuxEnemyLoader_Wraith;
typedef iLuxEnemy super_class;
public:
	cLuxEnemy_Wraith(const tString& asName, int alID, cLuxMap* apMap);
	virtual ~cLuxEnemy_Wraith();

	//////////////////////
	//General
	void OnSetupAfterLoad(cWorld* apWorld);
	void OnAfterWorldLoad();
	void UpdateEnemySpecific(float afTimeStep);

	//////////////////////
	//Actions
	void SetFlyMode(bool abX) { mbFlyMode = abX; }
	void SetToFlyMode(bool abX);
	bool GetIsInFlyMode() { return mbFlyMode; }
	void SetToStealthMode(bool abX);
	bool GetIsInStealthMode() { return mbIsInStealthDashMode; }
	void SetStealthModeNodesLength(int alNodes) {mlStealthDashNodesLengthStart = alNodes;}
	int  GetStealthModeNodesLeft() { return mlStealthDashNodesLength; }
	void SetCanMeele(bool abX) { mbCanMeele = abX; }
	bool StateEventImplement(int alState, eLuxEnemyStateEvent aEvent, cLuxStateMessage* apMessage);
	void SetAttackType(eLuxAttackType aAttackType);
	void SetPatrolSpeed(eLuxEnemyMoveSpeed aSpeedType);

	//////////////////////
	//Debug
	void OnRenderSolidImplemented(cRendererCallbackFunctions* apFunctions);

	//////////////////////
	//Save data stuff
	iLuxEntity_SaveData* CreateSaveData();
	void SaveToSaveData(iLuxEntity_SaveData* apSaveData);
	void LoadFromSaveData(iLuxEntity_SaveData* apSaveData);
	void SetupSaveData(iLuxEntity_SaveData* apSaveData);

private:
	//////////////////////
	// Callbacks
	bool PlayerIsDetected();
	void OnDisableTriggers();
	float GetDamageMul(float afAmount, int alStrength);
	void OnSetActiveEnemySpecific(bool abX);

	//////////////////////
	// State stuff
	bool CheckEnemyAutoRemoval(float afDistance);
	void FinishPatrolEndOfPath(bool callPatrolUpdateNow);
	void PatrolUpdateGoal();
	bool FleeTryToFindSafeNode();
	void StartTeleportDash();
	void StopTeleportDash();
	//////////////////////
	// Helper
	void PlayHuntSound();
	void PlayTeleportSound();
	
	bool CanUseRangedAttacks();
	
	bool InsidePlayerView();
	bool PlayerInLOS();
	
	void StartSoundLoops();
	void StopSoundLoops();

	void SetUpHeatRayConnection();
	void PlayHeatRaySounds(bool abX); 
	float GetPlayerInHeatRayLightLevel();
	void ResetPlayerHeatRay();
	void SetHeatRaySpotlightColor(cColor aColor);
	void SetHeatRaySpotlightFOV(float afFOV);
	void UpdatePlayerHeatRayDamage(float afTimeStep);

	void UpdateArchvileAttack(float afTimeStep);
	void CreateArchvileFlameLight();
	void ResetArchvileAttack();
	void EnableArchvileAttackParticle(bool abX);

	void TeleportLightCommand(string asCommand);

	void SetUpAttachedEntity();

	void UpdateSoundLoops(float afTimeStep);
	void UpdateAttachedEntity(float afTimeStep);
	void UpdateHeatRaySound(float afTimeStep);
	string GetSoundStateLoops();

	void SetupGroundWallAvoidance();
	void SetupFlyWallAvoidance();

	//void SetUpAttachmentEntity();

	void FadeIdleLoopVolume(float afVolTarget, float afSpeed);
	void FadeSearchLoopVolume(float afVolTarget, float afSpeed);
	void FadeChaseLoopVolume(float afVolTarget, float afSpeed);
	
	//////////////
	//Data
	tString msIdleStillLoop;
	tString msIdleMoveLoop;
	tString msSearchLoop;
	tString msStealthLoop;
	tString msChaseLoop;
	tString msSpotlightLoop;
	tString msHeatRayBurnLoop;
	tString msTeleportingLoop;

	tString msTeleportSound;
	tString msChaseSound;
	tString msEnabledSound;
	tString msNoticeSound;
	tString msGiveUpNoticeSound;

	tString msAttackMeleePS;
	cParticleSystem* pAttackMeleePS;

	tString msTeleportDashPS_Teleport;
	tString msTeleportDashPS_Trail;
	
	cParticleSystem* pTeleportDashPS_Teleport;
	cParticleSystem* pTeleportDashPS_Trail;

	tString msArchvilePS_AttackStart;
	tString msArchvilePS_AttackEnd;
	tString msProjectilePS_AttackStart;
	tString msProjectilePS_AttackEnd;

	cParticleSystem* pArchvilePS_AttackStart;
	cParticleSystem* pArchvilePS_AttackEnd;
	cParticleSystem* pProjectilePS_AttackStart;
	cParticleSystem* pProjectilePS_AttackEnd;
	bool mbRangedAttackParticleSetup;

	tString msHeatRaySpotlight;

	float mfRangedAttackDistance;
	cEnemyAttackDamageData mRangedAttackDamage;
	cEnemyAttackSizeData mRangedAttackSize;

	cVector2f mvRangedAttackDamage;
	cVector2f mvRangedAttackForce;
	cVector3f mvRangedDamageSize;
	cVector3f mvRangedAttackOffset;
	int mlRangedAttackStrength;

	tString msRangedAttackHitSound;
	tString msRangedAttackDisplayType;

	float mfIdleExtraTimeMin;
	float mfIdleExtraTimeMax;
	int mlIdleExtraNum;

	float mfGroggyDamageCount;

	bool mbIsActualWraith;

	tString msAttachEntity;
	tString msAttachmentBone;
	cVector3f mvAttachEntityPos;
	cVector3f mvAttachEntityRot;

	cVector3f mvFlyModeCharOffset;
	cVector3f mvDefaultCharOffset;

	bool mbHeatRayLightConnectionSetup;
	iLight* mpHeatRayLight;
	cLightSpot* mpHeatRaySpotLight; 
	float mfHeatRayRadius;
	float mfHeatRaySpotlightFOV;
	cColor mHeatRaySpotlightColor;
	cColor mHeatRaySpotlightColorIdle;
	cColor mHeatRaySpotlightColorAlert;
	cColor mHeatRaySpotlightColorHunt;
	float mfHRSpotlightFOVAlert;
	float mfHRSpotlightFOVPatrol;
	float mfHeatRaySpotlightAspect;

	std::vector<cColor> mvDefaultLightColors;

	//////////////
	//Variables
	float mfWaitTime;
	float mfDamageMul;
	
	float mfHuntPauseMinTime;
	float mfHuntPauseMaxTime;
	float mfHuntPauseTimeMul;

	float mfAlertRunTowardsCount;
	float mfAlertToHuntDistance;
	float mfAlertToInstantHuntDistance;
	float mfIncreaseAlertSpeedDistance;
	float mfIncreasedAlertSpeedMul;
	float mfAlertRunTowardsToHuntLimit;
	float mfAlertRunTowardsCheckDistance;

	float mfHRSpotlightLightLevel;
	
	float mfLanternSensitivity;
	float mfCheckFlashLightShining;

	float mfPlayerHeatRayLightLevel;
	float mfStartHeatRayDamage;
	float mfUpdateCount;

	bool mbFlyMode;
	bool mbFlyModeSetup;
	bool mbStealthDashMode;
	bool mbIsInStealthDashMode;
	bool mbAllowedToDashAtWill;
	bool mbStealthDashSetup;
	bool mbSpotlightSetup;

	int mlStealthDashNodesLength;
	int mlStealthDashNodesLengthStart;
	float mlEnterStealthDashNodeDistance;
	float mlExitStealthDashNodeDistance;

	bool mbPathReversed;
	bool mbAllowZeroWaitTime;
	bool mbPlayActivateSound;
	bool mbThreatenOnAlert;
	bool mbLastAttackWasMiss;
	bool mbForceLongRangeAttack;
	bool mbCanMeele;

	float mfFleeCheckIfInvisbleCount;

	bool mbLastShortAttackWasMiss;
	bool mbForceChargeAttack;
	bool mbIsInArchvileAttack;
	bool mbHeatRaySoundIsPlaying;

	bool mbPrevTriggers;

	cLightPoint* mpTeleportLight;

	cLightPoint* mpFlameLight;
	bool mbFlameLightIsOn;
	float mfFlameLightFadeTime;
	bool mbFlameLightFadeStarted;

	eLuxAttackType mAttackType;

	cSoundEntity* mpHuntSound;
	cSoundEntity* mpTeleportSound;
	cSoundEntity* mpCurrentLoopSound;
	cSoundEntity* mpSpotlightLoopSound;
	cSoundEntry* mpHeatRayBurnSound;
	cSoundEntity* mpTeleportDashLoopSound;
	int mlTeleportSoundId;
	int mTeleportDashLoopSoundId;
	int mlHeatRayBurnSoundId;
	int mlCurrentLoopSoundID;
	int mlSpotlightLoopSoundID;
	int mlHuntSoundId;
	tString msCurrentLoopState;
	tString msPrevLoopState;

	cColor mPrevSpotColor;
	cColor mCurrentSpotColor;

	eLuxEnemyState mPrevLoopState;

	eLuxEnemyMoveSpeed mPrevPatrolSpeed;
	eLuxEnemyMoveType mCurrentMoveType;


};
//----------------------------------------------
class cLuxEnemyLoader_Wraith : public iLuxEnemyLoader
{
public:
	cLuxEnemyLoader_Wraith(const tString& asName);
	virtual ~cLuxEnemyLoader_Wraith() {}

	iLuxEnemy* CreateEnemy(const tString& asName, int alID, cLuxMap* apMap);
	void LoadVariables(iLuxEnemy* apEnemy, cXmlElement* apRootElem);
	void LoadInstanceVariables(iLuxEnemy* apEnemy, cResourceVarsObject* apInstanceVars);
};

//----------------------------------------------

#endif // LUX_ENEMY_WRAITH_H

