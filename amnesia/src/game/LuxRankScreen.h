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
class cLuxRankScreenTextElement
{
public:
	bool Load(cXmlElement* apElement, const cVector2f& avGuiSetSize);

	cWidgetLabel* CreateLabel(cGuiSet* apSet);

private:
	tWString	msText;
	cVector3f	mvPos;
	cVector2f	mvFrameSize;
	cVector2f	mvFontSize;
	cColor		mColor;
	tString		msFontFile;
	eFontAlign	mAlign;

	float		mfTime;
};
//----------------------------------------------
class cLuxRankScreen : public iLuxUpdateable
{
public:
	cLuxRankScreen();
	~cLuxRankScreen();

	void LoadData();

	void LoadRankScreenTextElements(cConfigFile cFile);

	void LoadFonts();

	void Reset();

	void Update(float afTimeStep);

	void Setup(const tString& asMusic, bool abLoopMusic,float afMusicVol,float afMusicFade, string& asImageName);

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

	void PassCustomVarStringToVector(tString asVarString, tString asVarVec);

	bool ExitOnPressed(iWidget* apWidget, const cGuiMessageData& aData);
	kGuiCallbackDeclarationEnd(ExitOnPressed);
	bool UIExitOnPressed(iWidget* apWidget, const cGuiMessageData& aData);
	kGuiCallbackDeclarationEnd(UIExitOnPressed);

	bool Continue_Pressed(iWidget* apWidget, const cGuiMessageData& aData);
	kGuiCallbackDeclarationEnd(Continue_Pressed);

	bool Continue_UIPressed(iWidget* apWidget, const cGuiMessageData& aData);
	kGuiCallbackDeclarationEnd(Continue_UIPressed);

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

	bool mbShowRankScreen;

	float mfTextWidth;
	float mfTextY;
	float mfExitButtonY;
	float mfFadeSpeed;

	int mlTimeHourRank;
	int mlTimeMinRank;
	int mlTimeSecRank;
	bool mbDisplayTimeRank;
	tString msTimeRankOperator;


	bool mbDisplayEndingRank;
	tString msEndingRank;

	bool mbDisplayGameClearRank;
	bool mbDisplayGameModeRank;

	float mfMinDamageRank;
	bool mbDisplayDamageRank;
	
	int mfMinDeathCountRank;
	bool mbDisplayDeathRank;

	int mlItemAmount;
	int mfMinItemCountRank;
	int mfMinItemPercentRank;
	bool mbDisplayItemRank;

	int mfMinHealthItemUsedRank;
	bool mbDisplayHealthRank;

	int mfMinOilItemUsedRank;
	bool mbDisplayOilRank;

	int mlCustomGlobalVarRankAmount;

	tStringVec vCustomGlobalVarRankNames;
	tStringVec vCustomGlobalVarRankType;
	tStringVec vCustomGlobalVarRankOper;
	tStringVec vCustomGlobalVarRankCurrentValueString;
	tWStringVec vCustomGlobalVarRankFinalTextData;

	tStringVec vRankingVariableStrings;
	tStringVec vRankingTitleStrings;

	tBoolVec vCustomGlobalVarRankBool;
	tBoolVec vCustomGlobalVarCurrentBool;

	std::vector<int> vCustomGlobalVarRankNum;
	std::vector<int> vCustomGlobalVarRankCurrentValueInt;
	std::vector<float> vCustomGlobalVarRankCurrentValueFloat;
	std::vector<float> vCustomGlobalVarRankNumFloat;


	int mlRewardSlideAmount;
	int mlValidSlideAmount;
	int mlCurrentRewardSlide;
	tStringVec vRewardSlideConditions;
	tStringVec vRewardSlideSounds;
	tStringVec vRewardSlideBackgroundImage;
	tStringVec vRewardSlideTextCategories;
	tStringVec vRewardSlideTextEntries;
	tStringVec vRewardSlideTextAlign;

	std::vector<int> vRewardSlideGameClearAmount;
	std::vector<cColor>vRewardSlideTextColors;
	std::vector<cVector3f> vRewardSlideTextPos;
	std::vector<cVector2f> vRewardSlideTextSize;
	std::vector<cVector3f> vRewardSlideTitlePos;
	std::vector<cVector2f> vRewardSlideTitleSize;
	std::vector<cVector3f> vRewardSlideDescPos;
	std::vector<cVector2f> vRewardSlideDescSize;
	std::vector<cVector2f> vRewardSlideFrameSizes;

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
	bool mbStartFading;
	bool mbRewardSlideSoundPlayed;

	tString msMusic;
	bool mbLoopMusic;
	float mfMusicVolume;
	float mfMusicFade;


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

