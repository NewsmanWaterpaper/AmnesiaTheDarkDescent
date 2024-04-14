/*
 * Copyright © 2011-2020 Frictional Games
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

#include "LuxEnemy_Child.h"

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

//////////////////////////////////////////////////////////////////////////
// LOADER
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

cLuxEnemyLoader_Child::cLuxEnemyLoader_Child(const tString& asName) : iLuxEnemyLoader(asName)
{
}

//-----------------------------------------------------------------------

iLuxEnemy *cLuxEnemyLoader_Child::CreateEnemy(const tString& asName, int alID, cLuxMap *apMap)
{
	return hplNew(cLuxEnemy_Child, (asName, alID,apMap) );
}

//-----------------------------------------------------------------------

void cLuxEnemyLoader_Child::LoadVariables(iLuxEnemy *apEnemy, cXmlElement *apRootElem)
{
	cLuxEnemy_Child *pChild = static_cast<cLuxEnemy_Child*>(apEnemy);
}

//-----------------------------------------------------------------------

static eLuxEnemyMoveSpeed ToMoveSpeed(const tString& asStr)
{
	if(asStr == "Run") return eLuxEnemyMoveSpeed_Run;
	if(asStr == "Walk") return eLuxEnemyMoveSpeed_Walk;

	Error("eLuxEnemyMoveSpeed '%s' does not exist, falling back to walk!\n", asStr.c_str());
	return eLuxEnemyMoveSpeed_Walk;
}

void cLuxEnemyLoader_Child::LoadInstanceVariables(iLuxEnemy *apEnemy, cResourceVarsObject *apInstanceVars)
{
	cLuxEnemy_Child *pChild = static_cast<cLuxEnemy_Child*>(apEnemy);

	pChild->mPatrolMoveSpeed = ToMoveSpeed(apInstanceVars->GetVarString("PatrolMoveSpeed", "Walk"));
	pChild->mbAllowZeroWaitTime = apInstanceVars->GetVarBool("AllowZeroNodeWaitTimes", false);

}

//-----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// CONSTRUCTORS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

cLuxEnemy_Child::cLuxEnemy_Child(const tString &asName, int alID, cLuxMap *apMap) : iLuxEnemy(asName,alID,apMap, eLuxEnemyType_Child)
{
	mCurrentMoveType = eLuxEnemyMoveType_Normal;
	mfRunSpeedMul = 1.0f;
	mfWaitTime = 0;
	mbPathReversed = false;

	mfDamageMul = 1.0f;

	for(int i=0; i<eLuxEnemyMoveType_LastEnum; ++i)
	{
		eLuxEnemyPoseType pose = eLuxEnemyPoseType_Biped;
		msIdleAnimationName[i][pose] = "IdleBiped";
		msWalkAnimationName[i][pose] = "WalkBiped";
		msRunAnimationName[i][pose] =  "RunBiped";
	}
}

//-----------------------------------------------------------------------

cLuxEnemy_Child::~cLuxEnemy_Child()
{
	
}

//-----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

void cLuxEnemy_Child::OnSetupAfterLoad(cWorld *apWorld)
{
	mpMover->SetupWallAvoidance(0.9f, 8, 4);
	mpMover->SetWallAvoidanceActive(true);
}


//-----------------------------------------------------------------------

void cLuxEnemy_Child::OnAfterWorldLoad()
{

}

void cLuxEnemy_Child::SetPatrolSpeed(eLuxEnemyMoveSpeed aSpeedType)
{
	if (mPatrolMoveSpeed == aSpeedType) return;

	mPatrolMoveSpeed = aSpeedType;
	mbPatrolMoveSpeedChanged = true;

	mpMover->mMoveState = eLuxEnemyMoveState_LastEnum;
	mpMover->UpdateMoveAnimation(0.001f);
}

//-----------------------------------------------------------------------

void cLuxEnemy_Child::UpdateEnemySpecific(float afTimeStep)
{
}

//-----------------------------------------------------------------------

bool cLuxEnemy_Child::StateEventImplement(int alState, eLuxEnemyStateEvent aEvent, cLuxStateMessage *apMessage)
{
	kLuxBeginStateMachine
	
	////////////////////////////////
	// Default
	////////////////////////////////

	//------------------------------

	kLuxOnMessage(eLuxEnemyMessage_Reset)
		ChangeState(eLuxEnemyState_Wait);

	//------------------------------

	kLuxOnMessage(eLuxEnemyMessage_PlayerDetected)
		
	
	//------------------------------

	kLuxOnMessage(eLuxEnemyMessage_TakeHit)
		
	//------------------------------

	//If enemy is out of range (having been in, then turn him off)
	kLuxOnMessage(eLuxEnemyMessage_PlayerOutOfRange)
	
	//------------------------------

	kLuxOnMessage(eLuxEnemyMessage_PlayerDead)
	
	//------------------------------

	kLuxOnMessage(eLuxEnemyMessage_HelpMe)
	
	//------------------------------

	kLuxOnMessage(eLuxEnemyMessage_ChangePose)
		
		

	//------------------------------

	////////////////////////////////
	// Idle
	////////////////////////////////

	//------------------------------

	kLuxState(eLuxEnemyState_Idle)
		kLuxOnEnter
			SetMoveSpeed(eLuxEnemyMoveSpeed_Walk);
			ChangeState(eLuxEnemyState_Patrol);

		kLuxOnMessage(eLuxEnemyMessage_PlayerInRange)
		
		kLuxOnMessage(eLuxEnemyMessage_PlayerDetected)
			//Nothing

	//------------------------------
	
	////////////////////////////////
	// Go Home
	////////////////////////////////

	//------------------------------

	kLuxState(eLuxEnemyState_GoHome)
		kLuxOnEnter
			mpPathfinder->MoveTo(mvStartPosition);
			SetMoveSpeed(eLuxEnemyMoveSpeed_Walk);
		
		kLuxOnMessage(eLuxEnemyMessage_EndOfPath)
			ChangeState(eLuxEnemyState_Idle);

	//------------------------------

	////////////////////////////////
	// Wait
	////////////////////////////////

	//------------------------------

	kLuxState(eLuxEnemyState_Wait)
		kLuxOnEnter
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

			SendMessage(eLuxEnemyMessage_TimeOut_2, cMath::RandRectf(2, 5), true);

			SetMoveSpeed(eLuxEnemyMoveSpeed_Walk);
			mpPathfinder->Stop();

	kLuxOnLeave
		mpMover->SetOverideMoveState(false);

		//------------------------------
		
		kLuxOnMessage(eLuxEnemyMessage_TimeOut)
			
			if (mPreviousState == eLuxEnemyState_Patrol)
			{
				FadeOutCurrentAnim(0.2f);
				ChangeState(eLuxEnemyState_Patrol);
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
					SendMessage(eLuxEnemyMessage_TimeOut, cMath::RandRectf(0.1f, 0.3f), true);
				}
			}

		kLuxOnMessage(eLuxEnemyMessage_TimeOut_2)
			PlayAnim("IdleBiped", false, 0.3f);
		//------------------------------

		kLuxOnMessage(eLuxEnemyMessage_AnimationOver)
			SendMessage(eLuxEnemyMessage_TimeOut_2, cMath::RandRectf(0.1f, 0.3f), true);
	
		
	//------------------------------

	////////////////////////////////
	// Patrol
	////////////////////////////////

	//------------------------------
	kLuxState(eLuxEnemyState_Patrol)
		kLuxOnEnter
			ChangeSoundState(eLuxEnemySoundState_Idle);
			SetMoveSpeed(mPatrolMoveSpeed);
			if (mPatrolMoveSpeed == eLuxEnemyMoveSpeed_Run) mfForwardSpeed *= mfRunSpeedMul;
			
			PatrolUpdateGoal();

		kLuxOnUpdate
			if (mbPatrolMoveSpeedChanged)
			{
				SetMoveSpeed(mPatrolMoveSpeed);
				mbPatrolMoveSpeedChanged = false;
			}

		kLuxOnMessage(eLuxEnemyMessage_EndOfPath)
			
			/////////////////////////////////////
			//If at end, check if we want to remove
			if (IsAtLastPatrolNode() && CheckEnemyAutoRemoval(10))
			{
				//Do nothing...
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

				tString sCallback = msOverCallback;
				if (PatrolRemoveCallback) msOverCallback = "";

				if (sCallback != "")
					gpBase->mpMapHandler->GetCurrentMap()->RunScript(sCallback + "()");
			}


		kLuxOnMessage(eLuxEnemyMessage_AnimationOver)
			ChangeState(eLuxEnemyState_Wait);

		kLuxOnMessage(eLuxEnemyMessage_TimeOut)
			mpMover->UseMoveStateAnimations();
			ChangeState(eLuxEnemyState_Wait);
			
	//------------------------------
	
	

	
	kLuxEndStateMachine
}


//-----------------------------------------------------------------------

void cLuxEnemy_Child::OnRenderSolidImplemented(cRendererCallbackFunctions* apFunctions)
{
	iPhysicsWorld *pPhysicsWorld = mpMap->GetPhysicsWorld();

	if(mCurrentState == eLuxEnemyState_AttackMeleeShort)
	{
		pPhysicsWorld->RenderShapeDebugGeometry(GetAttackShape(0), GetDamageShapeMatrix(cVector3f(0,0,1)), apFunctions->GetLowLevelGfx(),
												cColor(1,0,0,1));
	}
}

//-----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// PRIVATE METHODS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

bool cLuxEnemy_Child::PlayerIsDetected()
{
	return false;
}

//-----------------------------------------------------------------------

void cLuxEnemy_Child::OnDisableTriggers()
{
	if (mCurrentState == eLuxEnemyState_Idle) return;

	ChangeState(eLuxEnemyState_Patrol);
}

//-----------------------------------------------------------------------

void cLuxEnemy_Child::PatrolUpdateGoal()
{
	if (mvPatrolNodes.empty())
	{

		return;
	}

	cLuxEnemyPatrolNode* pPatrolNode = GetCurrentPatrolNode();

	mpPathfinder->MoveTo(pPatrolNode->mpNode->GetPosition());
}
//-----------------------------------------------------------------------

void cLuxEnemy_Child::FinishPatrolEndOfPath(bool callPatrolUpdateNow)
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
bool cLuxEnemy_Child::CheckEnemyAutoRemoval(float afDistance)
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
float cLuxEnemy_Child::GetDamageMul(float afAmount, int alStrength)
{
	return 1.0f;
}


//-----------------------------------------------------------------------


//-----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// SAVE DATA STUFF
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

kBeginSerialize(cLuxEnemy_Child_SaveData, iLuxEnemy_SaveData)

kSerializeVar(mPatrolMoveSpeed, eSerializeType_Int32)
kSerializeVar(mfWaitTime, eSerializeType_Float32)
kSerializeVar(mbAllowZeroWaitTime, eSerializeType_Bool)
kSerializeVar(mfRunSpeedMul, eSerializeType_Float32)
kSerializeVar(mfDamageMul, eSerializeType_Float32)


kEndSerialize()

//-----------------------------------------------------------------------

iLuxEntity_SaveData* cLuxEnemy_Child::CreateSaveData()
{
	return hplNew(cLuxEnemy_Child_SaveData, ());
}

//-----------------------------------------------------------------------

void cLuxEnemy_Child::SaveToSaveData(iLuxEntity_SaveData* apSaveData)
{
	//////////////////
	//Init
	super_class::SaveToSaveData(apSaveData);
	cLuxEnemy_Child_SaveData *pData = static_cast<cLuxEnemy_Child_SaveData*>(apSaveData);

	//////////////////
	//Set variables
	kCopyToVar(pData,mPatrolMoveSpeed);
	kCopyToVar(pData,mfWaitTime);
	kCopyToVar(pData, mbAllowZeroWaitTime);
	kCopyToVar(pData, mfRunSpeedMul);
	kCopyToVar(pData, mfDamageMul);
}

//-----------------------------------------------------------------------

void cLuxEnemy_Child::LoadFromSaveData(iLuxEntity_SaveData* apSaveData)
{
	//////////////////
	//Init
	super_class::LoadFromSaveData(apSaveData);
	cLuxEnemy_Child_SaveData *pData = static_cast<cLuxEnemy_Child_SaveData*>(apSaveData);

	//////////////////
	//Set variables
	mPatrolMoveSpeed  = (eLuxEnemyMoveSpeed)pData->mPatrolMoveSpeed;
	kCopyFromVar(pData, mfWaitTime);
	kCopyFromVar(pData, mbAllowZeroWaitTime);
	kCopyFromVar(pData, mfRunSpeedMul);
	kCopyFromVar(pData, mfDamageMul);
}

//-----------------------------------------------------------------------

void cLuxEnemy_Child::SetupSaveData(iLuxEntity_SaveData *apSaveData)
{
	super_class::SetupSaveData(apSaveData);
}


//-----------------------------------------------------------------------

