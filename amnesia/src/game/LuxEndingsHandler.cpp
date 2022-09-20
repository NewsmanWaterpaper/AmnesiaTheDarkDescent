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
	mbGotDefaultGoodEnding = gpBase->mpUserEndingConfig->GetBool("Endings", "GotDefaultGoodEnding", false);
	mbGotDefaultBadEnding = gpBase->mpUserEndingConfig->GetBool("Endings", "GotDefaultBadEnding", false);
	mbGotGoodJokeEnding = gpBase->mpUserEndingConfig->GetBool("Endings", "GotGoodJokeEnding", false);
	mbGotBadJokeEnding = gpBase->mpUserEndingConfig->GetBool("Endings", "GotBadJokeEnding", false);
	msPreviousEnding = gpBase->mpUserEndingConfig->GetString("Endings", "MostRecentEnding", "None");
	mbBeatenHardMode = gpBase->mpUserEndingConfig->GetBool("Endings", "BeatenHardMode", false);
	mbAllowHardmode = gpBase->mpUserEndingConfig->GetBool("Endings", "AllowHardMode", false);
	mbAllowBonusFeatures = gpBase->mpUserEndingConfig->GetBool("Endings", "AllowBonusFeatures", false);
	mlTotalGameClears = gpBase->mpUserEndingConfig->GetInt("Endings", "TotalGameClear", 0);
}

void cLuxEndingsHandler::SaveUserConfig()
{
	gpBase->mpUserEndingConfig->SetBool("Endings", "GotDefaultGoodEnding", mbGotDefaultGoodEnding);
	gpBase->mpUserEndingConfig->SetBool("Endings", "GotDefaultBadEnding", mbGotDefaultBadEnding);
	gpBase->mpUserEndingConfig->SetBool("Endings", "GotGoodJokeEnding", mbGotGoodJokeEnding);
	gpBase->mpUserEndingConfig->SetBool("Endings", "GotBadJokeEnding", mbGotBadJokeEnding);
	gpBase->mpUserEndingConfig->SetString("Endings", "MostRecentEnding", msPreviousEnding);
	gpBase->mpUserEndingConfig->SetBool("Endings", "BeatenHardMode", mbBeatenHardMode);
	gpBase->mpUserEndingConfig->SetBool("Endings", "AllowHardMode", mbAllowHardmode);
	gpBase->mpUserEndingConfig->SetBool("Endings", "AllowBonusFeatures", mbAllowBonusFeatures);
	gpBase->mpUserEndingConfig->SetInt("Endings", "TotalGameClear", mlTotalGameClears);
}
//--------------------------------------------------------------------------------------------
void cLuxEndingsHandler::AddEndingLog(eLuxEnding aEnding, bool mbCompletedOnHardMode)
{
	if(aEnding == eLuxEnding_DefaultGood)
	{
		mbGotDefaultGoodEnding = true;
		msPreviousEnding = "DefaultGood";
	}
	else if(aEnding == eLuxEnding_DefaultBad)
	{
		mbGotDefaultBadEnding = true;
		msPreviousEnding = "DefaultBad";
	}
	else if(aEnding == eLuxEnding_GoodJoke)
	{
		mbGotGoodJokeEnding = true;
		msPreviousEnding = "GoodJoke";
	}
	else if(aEnding == eLuxEnding_BadJoke)
	{
		mbGotBadJokeEnding = true;
		msPreviousEnding = "BadJoke";
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
	if(asEndingType == "DefaultGood")
	{
		return mbGotDefaultGoodEnding;
	}
	else if(asEndingType == "DefaultBad")
	{
		return mbGotDefaultBadEnding;
	}
	else if(asEndingType == "GoodJoke")
	{
		return mbGotGoodJokeEnding;
	}
	else if(asEndingType == "BadJoke")
	{
		return mbGotBadJokeEnding;
	}
	else if(asEndingType == "HardMode")
	{
		return mbBeatenHardMode; 
	}

	return false;
}
