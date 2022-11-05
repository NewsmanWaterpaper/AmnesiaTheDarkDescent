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

 //----------------------------------------------
#include "LuxBase.h"
//----------------------------------------------

class cLuxEndingsHandler : public iLuxUpdateable
{
public:
	cLuxEndingsHandler();
	~cLuxEndingsHandler();
	void LoadUserConfig();
	void SaveUserConfig();

	void AddEndingLog(string& msEnding, bool mbCompletedOnHardMode);

	//const string& GetPreviousEnding() { return msPreviousEnding; }
	string& GetPreviousEnding(){return msPreviousEnding;}
	string msPreviousEnding;

	bool GetEndingCompleted(string& asEndingType);
	int GetTotalGameClears() {return mlTotalGameClears;}

	bool mbAllowHardmode;
	bool mbAllowBonusFeatures;

private:

	int mlEndingAmount;
	std::vector<int> vEndingNumbers;
	tStringVec vEndingNames;
	tBoolVec vGotEnding;

	int mlTotalGameClears;
	bool mbGotDefaultGoodEnding;
	bool mbGotDefaultBadEnding;
	bool mbGotGoodJokeEnding;
	bool mbGotBadJokeEnding;
	bool mbBeatenHardMode;

	tWString msNewInitConfigFile;

	
};
//----------------------------------------------
