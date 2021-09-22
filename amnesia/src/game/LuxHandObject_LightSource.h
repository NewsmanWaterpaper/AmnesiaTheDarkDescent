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

#ifndef LUX_HAND_OBJECT_LIGHT_SOURCE_H
#define LUX_HAND_OBJECT_LIGHT_SOURCE_H

//----------------------------------------------

#include "LuxHandObject.h"


//----------------------------------------------

class cLuxHandObject_LightSource : public iLuxHandObject
{
public:	
	cLuxHandObject_LightSource(const tString& asName, cLuxPlayerHands *apHands);
	~cLuxHandObject_LightSource();
	
	void LoadImplementedVars(cXmlElement *apVarsElem);

	void ImplementedCreateEntity(cLuxMap *apMap);
	void ImplementedDestroyEntity(cLuxMap *apMap);

	void ImplementedReset();

	void Update(float afTimeStep);

	bool DoAction(eLuxPlayerAction aAction, bool abPressed);
	bool AnimationIsOver();

	void SetFlickering(bool abX);
	bool GetFlickering() { return mbFlickering; }

	void SetFlickeringSpeed(float afX) { mfFlickeringSpeed = afX; }
	float GetFlickeringSpeed() { return mfFlickeringSpeed; }

private:
	void UpdateSwayPhysics(float afTimeStep);
	float UpdateFlickering(float afTimeStep);

	bool mbFlickering;
	float mfFlickeringSpeed;
	int mlFlickeringState;

	float mfFlickerAmount;
	float mfFlickerTime;
	float mfFlickerPauseTime;

	float mfFadeInSpeed;
	float mfFadeOutSpeed;

	float mfStrobeDroneTime;
	cSoundEntry* mpDroneSound;
	int mlDroneSoundId;
	
	bool mbHasSwayPhysics;
	float mfMaxSwayVel;
	cVector2f mvSwayAngleLimits;
	cVector2f mvSwayDownAngleLimits;
	cVector3f mvSwayPinDir;
	float mfSwayGravity;
	float mfSwayFriction;
	float mfSwayPlayerSpeedMul;
	float mfSwayCameraRollMul;
	tString msSkipSwaySubMesh;
	
	float mfSwayAngle;
	float mfSwayVel;

	bool mbUseFlicker;

	std::vector<cColor> mvLightFadeOutColor;
	std::vector<cColor> mvDefaultLightColors;
	std::vector<bool> mvDefaultLightFlicker;
	std::vector<cMatrixf> mvDefaultSubMeshMatrix;
	std::vector<cMatrixf> mvDefaultLightMatrix;
	std::vector<cMatrixf> mvDefaultBillboardMatrix;
};

//----------------------------------------------



#endif // LUX_HAND_OBJECT_LIGHT_SOURCE_H
