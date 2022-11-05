/*
 * Copyright © 2009-2021 Frictional Games
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

#include "LuxEndingsHandler.h"


 //-----------------------------------------------------------------------

 //////////////////////////////////////////////////////////////////////////
 // CONSTRUCTORS
 //////////////////////////////////////////////////////////////////////////

 //-----------------------------------------------------------------------
cLuxEndingsHandler::cLuxEndingsHandler() : iLuxUpdateable("LuxEndingsHandler")
{
	//mlTotalGameClears = 0; 
}

cLuxEndingsHandler::~cLuxEndingsHandler()
{
	
}
//--------------------------------------------------------------------------------------------
void cLuxEndingsHandler::LoadUserConfig()
{
	mlEndingAmount = gpBase->mpUserEndingConfig->GetInt("Endings", "AmountOfEndings", 1);

	for (int i = 0; i < mlEndingAmount; ++i)
	{
		vEndingNames.push_back(gpBase->mpUserEndingConfig->GetString("RankScreen", "Ending_"+ cString::ToString(i + 1)+"_Name", ""));
		vGotEnding.push_back(gpBase->mpUserEndingConfig->GetBool("Endings", "GotEnding_"+ cString::ToString(i+1), false));
	}

	msPreviousEnding = gpBase->mpUserEndingConfig->GetString("Endings", "MostRecentEnding", "None");
	mbBeatenHardMode = gpBase->mpUserEndingConfig->GetBool("Endings", "BeatenHardMode", false);
	mbAllowHardmode = gpBase->mpUserEndingConfig->GetBool("Endings", "AllowHardMode", false);
	mbAllowBonusFeatures = gpBase->mpUserEndingConfig->GetBool("Endings", "AllowBonusFeatures", false);
	mlTotalGameClears = gpBase->mpUserEndingConfig->GetInt("Endings", "TotalGameClear", 0);
}

void cLuxEndingsHandler::SaveUserConfig()
{

	gpBase->mpUserEndingConfig->SetInt("Endings", "AmountOfEndings", mlEndingAmount);

	for (int i = 0; i < mlEndingAmount; ++i)
	{
		//vGotEnding.push_back(gpBase->mpUserEndingConfig->GetBool("Endings", "GotEnding_" + cString::ToString(i + 1), false));

		gpBase->mpUserEndingConfig->SetString("Endings", "Ending_" + cString::ToString(i + 1) + "_Name", vEndingNames[i]);
		gpBase->mpUserEndingConfig->SetBool("Endings", "GotEnding_" + cString::ToString(i + 1), vGotEnding[i]);
	}
	gpBase->mpUserEndingConfig->SetString("Endings", "MostRecentEnding", msPreviousEnding);
	gpBase->mpUserEndingConfig->SetBool("Endings", "BeatenHardMode", mbBeatenHardMode);
	gpBase->mpUserEndingConfig->SetBool("Endings", "AllowHardMode", mbAllowHardmode);
	gpBase->mpUserEndingConfig->SetBool("Endings", "AllowBonusFeatures", mbAllowBonusFeatures);
	gpBase->mpUserEndingConfig->SetInt("Endings", "TotalGameClear", mlTotalGameClears);
}
//--------------------------------------------------------------------------------------------
void cLuxEndingsHandler::AddEndingLog(string& msEnding, bool mbCompletedOnHardMode)
{
	for (int i = 0; i < vEndingNames.size(); i++)
	{
		if (vEndingNames[i] == msEnding)
		{
			msPreviousEnding = vEndingNames[i];
			vGotEnding.push_back(true);
		}
	}

	if(mbCompletedOnHardMode)
	{
		mbBeatenHardMode = true;
	}

	mlTotalGameClears += 1;

	if (mlTotalGameClears == 1)
	{
		if (mbAllowHardmode == false)
		{
			mbAllowHardmode = true;
		}

		if (mbAllowBonusFeatures == false)
		{
			mbAllowBonusFeatures = true;
		}
	}

	gpBase->mpUserEndingConfig->Save();
}

bool cLuxEndingsHandler::GetEndingCompleted(string& asEndingType)
{
	for (int i = 0; i < vEndingNames.size(); i++)
	{
		if (vEndingNames[i] == asEndingType)
		{
			return vGotEnding[i];
		}
	}

	if(asEndingType == "HardMode")
	{
		return mbBeatenHardMode; 
	}

	return false;
}
