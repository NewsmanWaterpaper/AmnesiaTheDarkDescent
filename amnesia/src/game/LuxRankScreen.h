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
 //----------------------------------------------

#include "LuxBase.h"

//----------------------------------------------
class cLuxRankScreen : public iLuxUpdateable
{
public:
	cLuxRankScreen();
	~cLuxRankScreen();

	void LoadData();

	void LoadFonts();

	void Reset();

	void Update(float afTimeStep);

	void Setup(const tString& asMusic, bool abLoopMusic, string& asImageName);

	void OnEnterContainer(const tString& asOldContainer);
	void OnLeaveContainer(const tString& asNewContainer);

	void OnDraw(float afFrameTime);

	void Exit();

	void AppLostInputFocus();
	void AppGotInputFocus();

	bool ShowOnAllExit() { return mbShowOnAllExit; }

	cGuiSet* GetSet() { return mpGuiSet; }

private:
	void SetUpButtonLabel(cWidgetLabel* apLabel, float* apFadeValue, tGuiCallbackFunc apCallback);

	bool ExitOnPressed(iWidget* apWidget, const cGuiMessageData& aData);
	kGuiCallbackDeclarationEnd(ExitOnPressed);
	bool UIExitOnPressed(iWidget* apWidget, const cGuiMessageData& aData);
	kGuiCallbackDeclarationEnd(UIExitOnPressed);

	bool ButtonLabelOnUpdate(iWidget* apWidget, const cGuiMessageData& aData);
	kGuiCallbackDeclarationEnd(ButtonLabelOnUpdate);
	bool ButtonLabelOnDraw(iWidget* apWidget, const cGuiMessageData& aData);
	kGuiCallbackDeclarationEnd(ButtonLabelOnDraw);
	bool ButtonLabelOnMouseEnter(iWidget* apWidget, const cGuiMessageData& aData);
	kGuiCallbackDeclarationEnd(ButtonLabelOnMouseEnter);
	bool ButtonLabelOnMouseLeave(iWidget* apWidget, const cGuiMessageData& aData);
	kGuiCallbackDeclarationEnd(ButtonLabelOnMouseLeave);

	/////////////
	//DATA
	cGraphics* mpGraphics;

	cGui* mpGui;
	cGuiSet* mpGuiSet;

	iFontData* mpFontMessage;
	iFontData* mpFontButton;

	cViewport* mpViewport;

	cGuiGfxElement* mpBlackGfx;

	cVector2f mvScreenSize;
	cVector2f mvGuiSetSize;
	cVector2f mvGuiSetCenterSize;
	cVector2f mvGuiSetOffset;
	cVector3f mvGuiSetStartPos;
	cVector3f mvButtonPos;

	cVector2f mvMessageFontSize;
	cVector2f mvButtonSize;
	cVector2f mvTitleFontSize;

	float mfTextWidth;
	float mfTextY;
	float mfExitButtonY;
	float mfFadeSpeed;

	cColor mMessageFontColor;
	cColor mButtonFontColor;
	cColor mTitleColor;
	cColor mGoldStatusColor;

	bool mbShowOnAllExit;
	/////////////
	//VARS
	bool mbActive;
	float mfYPos;
	int mlState;
	float mfTime;
	float mfFadeAlpha;
	int lSec;
	int lMin;
	int lHour;

	bool mbExiting;

	tString msMusic;
	bool mbLoopMusic;


	float mfExitFade;
	cWidgetLabel* mpLExit;

	cWidgetButton* mpBExit;

	tString msBackgroundFile;
	cGuiGfxElement* mpGfxBackground;

	tWStringVec mvTextRows;

	tWString msGameMode;
	tWString msTotalTime;
	tWString msEndingReceived;
	tWString msGameClear;
	tWString msTotalDamageTaken;
	tWString msTotalDeath;
	tWString msPlayerItemAmount;
	tWString msHealthItemsUsed;
	tWString msOilItemsUsed;
	tWString msStatuesFound;

	tString msStatuesVal;
	int mlStatuesVal;

	tWString msSec;
	tWString msMin;
	tWString msHour;
};

