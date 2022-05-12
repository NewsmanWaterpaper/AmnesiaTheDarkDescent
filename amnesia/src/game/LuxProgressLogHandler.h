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

#ifndef LUX_PROGRESS_LOG_HANDLER_H
#define LUX_PROGRESS_LOG_HANDLER_H

//----------------------------------------------

#include "LuxBase.h"

//----------------------------------------

class cLuxProgressLogHandler : public iLuxUpdateable
{
friend class cLuxProgressLogHandler_SaveData;
public:	
	cLuxProgressLogHandler();
	~cLuxProgressLogHandler();

	void LoadUserConfig();
	void SaveUserConfig();
	
	void OnStart();
	void Update(float afTimeStep);
	void DrawHud(float afTimeStep);
	void Reset();

	tString GetPlayTime(); 

	int GetProgLogCounter() {return mlCounter; }

	bool GetProgLogCounterActive() {return mbCounterActive; }
	void SetProgLogCounterActive(bool abX) { mbCounterActive = abX; }
	void ResetProgLogCounter(); 
	
	void DisplayCounterHUD(bool abX) { mbShowCounterHUD = abX; }
	void SetCounterHUDColor(cColor aColor) { mCounterTextColor = aColor; }
	void SetCounterHUDPosition(cVector3f aPos) { mvCounterPos = aPos; }
	void SetCounterHUDSize(cVector2f aSize) { mvCounterSize = aSize; }
	void SetCounterHUDAlign(eFontAlign aAlign) { mCounterTextAlign = aAlign; }

	void CreateAndResetLogFile();

	void AddLog(eLuxProgressLogLevel aLevel, const tString& asMessage);
	
private:
	tString LevelToString(eLuxProgressLogLevel aLevel);

	iFontData* mpCountFont;

	cVector3f mvCounterPos;
	cVector2f mvCounterSize;

	cColor mCounterTextColor;

	eFontAlign mCounterTextAlign;

	tWString msTimerDisplayText;
	int mlCounter;
	int mlFileNameCount;

	bool mbActive;
	bool mbCounterActive;
	bool mbShowCounterHUD; 

	FILE *mpFile;
};

//----------------------------------------------


#endif // LUX_PROGRESS_LOG_HANDLER_H
