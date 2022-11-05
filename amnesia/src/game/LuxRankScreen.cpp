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

#include "LuxRankScreen.h"

#include "LuxHelpFuncs.h"
#include "LuxConfigHandler.h"
#include "LuxDebugHandler.h"
#include "LuxLoadScreenHandler.h"
#include "LuxMusicHandler.h"
#include "LuxInputHandler.h"
#include "LuxProgressLogHandler.h"
#include "LuxEndingsHandler.h"
#include "LuxPlayer.h"
#include "LuxGlobalDataHandler.h"
#include "LuxMainMenu_CustomStory.h"

//#include <sstream>
//#include <vector>

 //-----------------------------------------------------------------------

//int glTotalItems_Num = 100;
//
//#if MAC_OS || LINUX
//std::wstring LongToWString(const long long& l)
//{
//	std::string s;
//	std::stringstream strstream;
//	strstream << l;
//	strstream >> s;
//	std::wstring temp(s.length(), L' ');
//	std::copy(s.begin(), s.end(), temp.begin());
//	return temp;
//}
//
//tWString gsTotalItems_String = tWString(LongToWString((static_cast<long long>(glTotalItems_Num))));
//#else
//tWString gsTotalItems_String = tWString(to_wstring(static_cast<long long>(glTotalItems_Num)));
//#endif

static eFontAlign ToFontAlign(const tString& asX)
{
	tString sLowerCase = cString::ToLowerCase(asX);
	if (sLowerCase == "center")
		return eFontAlign_Center;
	else if (sLowerCase == "right")
		return eFontAlign_Right;
	else if (sLowerCase == "left")
		return eFontAlign_Left;

	Error("The font alignment '%s' does not exist!\n", asX.c_str());
	return eFontAlign_Left;
}

//-----------------------------------------------------------------------

 //////////////////////////////////////////////////////////////////////////
 // CONSTRUCTORS
 //////////////////////////////////////////////////////////////////////////

 //-----------------------------------------------------------------------
cLuxRankScreen::cLuxRankScreen() : iLuxUpdateable("LuxRankScreen")
{
	///////////////////////////////
	//Get Engine
	mpGraphics = gpBase->mpEngine->GetGraphics();
	mpGui = gpBase->mpEngine->GetGui();
	mpGuiSet = mpGui->CreateSet("RankScreen", NULL);

	///////////////////////
	// Load settings
	mvScreenSize = gpBase->mpEngine->GetGraphics()->GetLowLevel()->GetScreenSizeFloat();

	mvGuiSetCenterSize = cVector2f(800, 600);

	LuxCalcGuiSetScreenOffset(mvGuiSetCenterSize, mvGuiSetSize, mvGuiSetOffset);
	mvGuiSetStartPos = cVector3f(-mvGuiSetOffset.x, -mvGuiSetOffset.y, 0);

	mpGuiSet->SetVirtualSize(mvGuiSetSize, -1000, 1000, mvGuiSetOffset);

	///////////////////////////////
	//Create Viewport
	mpViewport = gpBase->mpEngine->GetScene()->CreateViewport();
	mpViewport->SetActive(false);
	mpViewport->SetVisible(false);
	mpGuiSet->SetActive(false);
	mpGuiSet->SetDrawMouse(false);

	mpViewport->AddGuiSet(mpGuiSet);

	mpFontMessage = NULL;
	mpFontButton = NULL;

	mpGfxBackground = NULL;

	mfFadeAlpha = 1;
	lSec = 0;
	lMin = 0;
	lHour = 0;

	mvTitleFontSize = cVector2f(50, 50);
	mTitleColor = cColor(0.7, 0.7, 0.9, 0.9);
	//mGoldStatusColor = cColor(232.0f/255.0f, 201.0f/255.0f, 28.0f/255.0f);

	///////////////////////////////
	//Load GUI Data
	tString sSkinFile = gpBase->mpRankCfg->GetString("RankScreen", "Skin", "gui_main_menu.skin");
	mpGuiSet->SetSkin(mpGui->CreateSkin(sSkinFile));

	msBackgroundFile = gpBase->mpRankCfg->GetString("RankScreen", "BackgroundImage", "demo_end_image.jpg");

	mvMessageFontSize = gpBase->mpRankCfg->GetVector2f("RankScreen", "MessageFontSize", cVector2f(25,25));
	mMessageFontColor = gpBase->mpRankCfg->GetColor("RankScreen", "MessageFontColor", cColor(1, 1));
	mGoldStatusColor = gpBase->mpRankCfg->GetColor("RankScreen", "RankedFontColor", cColor(232.0f / 255.0f, 201.0f / 255.0f, 28.0f / 255.0f));

	mfTextWidth = gpBase->mpRankCfg->GetFloat("RankScreen", "TextWidth", 500);
	mfTextY = gpBase->mpRankCfg->GetFloat("RankScreen", "TextY", 120);


	mvButtonPos = gpBase->mpRankCfg->GetVector3f("RankScreen", "ExitButtonPos", cVector3f(355, 550, 10));
	mvButtonSize = gpBase->mpRankCfg->GetVector2f("RankScreen", "MessageFontSize", cVector2f(90, 45));
	mButtonFontColor = gpBase->mpRankCfg->GetColor("RankScreen", "ButtonFontColor", cColor(1, 0.9,0.9,1));

	mpBlackGfx = mpGui->CreateGfxFilledRect(cColor(0, 1), eGuiMaterial_Alpha);

	mfFadeSpeed = gpBase->mpRankCfg->GetFloat("RankScreen", "FadeSpeed", 0);


	///////////////////////////////
	//Load Rank Data

	mlTimeHourRank = gpBase->mpRankCfg->GetInt("RankScreen", "TimeHourRank", 1);
	mlTimeMinRank = gpBase->mpRankCfg->GetInt("RankScreen", "TimeMinRank", 30);
	mlTimeSecRank = gpBase->mpRankCfg->GetInt("RankScreen", "TimeSecRank", 15);
	mbDisplayTimeRank = gpBase->mpRankCfg->GetBool("RankScreen", "DisplayTimeRank", true);
	msTimeRankOperator = gpBase->mpRankCfg->GetString("RankScreen", "TimeRank_Operator", "<=");


	mbDisplayEndingRank = gpBase->mpRankCfg->GetBool("RankScreen", "DisplayEndingRank", true);
	msEndingRank = gpBase->mpRankCfg->GetString("RankScreen", "EndingRank", "Good");

	mbDisplayGameClearRank = gpBase->mpRankCfg->GetBool("RankScreen", "DisplayGameClearRank", true);
	mbDisplayGameModeRank = gpBase->mpRankCfg->GetBool("RankScreen", "DisplayGameModeRank", true);

	mfMinDeathCountRank = gpBase->mpRankCfg->GetInt("RankScreen", "MinDeathRank", 0);
	mbDisplayDeathRank = gpBase->mpRankCfg->GetBool("RankScreen", "DisplayDeathRank", true);

	mfMinDamageRank = gpBase->mpRankCfg->GetInt("RankScreen", "MinDamageRank", 250);
	mbDisplayDamageRank = gpBase->mpRankCfg->GetBool("RankScreen", "DisplayDamageRank", true);

	mlItemAmount = gpBase->mpRankCfg->GetInt("RankScreen", "TotalItemAmount", 100);
	mbDisplayItemRank = gpBase->mpRankCfg->GetBool("RankScreen", "DisplayItemRank", true);

	mfMinHealthItemUsedRank = gpBase->mpRankCfg->GetInt("RankScreen", "HealthItemUsedRank", 5);
	mbDisplayHealthRank = gpBase->mpRankCfg->GetBool("RankScreen", "DisplayHealthRank", true);

	mfMinOilItemUsedRank = gpBase->mpRankCfg->GetInt("RankScreen", "OilItemUsedRank", 5);
	mbDisplayOilRank = gpBase->mpRankCfg->GetBool("RankScreen", "DisplayOilRank", true);

	mlCustomGlobalVarRankAmount = gpBase->mpRankCfg->GetInt("RankScreen", "CustomGlobalVarRankAmount", 0);
	mlRewardSlideAmount = gpBase->mpRankCfg->GetInt("RankScreen", "RewardSlideAmount", 0);

	///////////////////////////////////////

	tWString sButtonLabel = kTranslate("RankScreen", "Exit");
	
	float fButtonWidth = 20.0f;
	float fButtonHeight = 30.0f;

	//mvButtonSize = cVector2f(fButtonWidth+60, fButtonHeight+5) + 10;
	//mvButtonPos = cVector3f(355,550,10);

	mpBExit = mpGuiSet->CreateWidgetButton(mvButtonPos, mvButtonSize, sButtonLabel, NULL);

	//mpBExit->SetPosition(vPos);
   // mpBExit->SetSize(vSize);

	mpBExit->AddCallback(eGuiMessage_ButtonPressed, this, kGuiCallback(ExitOnPressed));
	mpBExit->AddCallback(eGuiMessage_UIButtonPress, this, kGuiCallback(UIExitOnPressed));
	mpBExit->SetGlobalUIInputListener(true);
	mpBExit->SetVisible(false);
	

	//mpGuiSet->SetDefaultFocusNavWidget(mpBExit);
	mpGuiSet->SetFocusedWidget(mpBExit);

	Reset();
}

//-----------------------------------------------------------------------
cLuxRankScreen::~cLuxRankScreen()
{

}

//-----------------------------------------------------------------------

void cLuxRankScreen::Reset()
{
	mbExiting = false;
	msMusic = "";
	mfMusicVolume = 1.0f;
	mfMusicFade = 1.0f;
	mbLoopMusic = false;
	mbShowRankScreen = true;
	mlCurrentRewardSlide = -1;
	mbRewardSlideSoundPlayed = false;
	

	vCustomGlobalVarRankNames.clear();
	vCustomGlobalVarRankType.clear();
	vCustomGlobalVarRankOper.clear();
	vCustomGlobalVarRankCurrentValueString.clear();
	vCustomGlobalVarRankFinalTextData.clear();

	vCustomGlobalVarRankBool.clear();
	vCustomGlobalVarCurrentBool.clear();

	vCustomGlobalVarRankNum.clear();
	vCustomGlobalVarRankCurrentValueInt.clear();
	
	vCustomGlobalVarRankNumFloat.clear();
	vCustomGlobalVarRankCurrentValueFloat.clear();

	vRewardSlideConditions.clear();
	vRewardSlideSounds.clear();
	vRewardSlideBackgroundImage.clear();
	//vRewardSlideTextCategories.clear();
	vRewardSlideTextEntries.clear();
	vRewardSlideTextAlign.clear();
	vRewardSlideTitleSize.clear();
	vRewardSlideTitlePos.clear();
	vRewardSlideDescSize.clear();
	vRewardSlideDescPos.clear();

	vRewardSlideGameClearAmount.clear();
	vRewardSlideTextColors.clear();
	vRewardSlideTextPos.clear();
	vRewardSlideTextSize.clear();
}

//-----------------------------------------------------------------------

void cLuxRankScreen::LoadData()
{
	cLuxHelpFuncs *pHelpFuncs = gpBase->mpHelpFuncs;

	//glTotalItems_Num = mlItemAmount;

	////////////////////////////////////////////
	//Get Custom Story Rank Data///////////////
	//////////////////////////////////////////

	tWString sPath = cString::AddSlashAtEndW(cString::To16Char(gpBase->msCustomStoryPath));

	tWStringList lstStoryDirs;
	cPlatform::FindFoldersInDir(lstStoryDirs, sPath, false);

	cConfigFile* pCustomStoryCfg;
	tWString sFile;

	if (gpBase->GetIsInCustomStory() == true)
	{
		tString sErrorMsg;
		tWString sStoryName = gpBase->GetCurrentCustomStoryName();

		tWStringListIt it = lstStoryDirs.begin();

		for (; it != lstStoryDirs.end(); ++it)
		{
			const tWString& sStoryPath = sPath + *it;
			cLuxCustomStorySettings* pStory = hplNew(cLuxCustomStorySettings, ());

			sFile = cString::AddSlashAtEndW(sStoryPath) + _W("custom_story_settings.cfg");

			pCustomStoryCfg = hplNew(cConfigFile, (sFile));
			bool bValid = pCustomStoryCfg->Load();
			if (bValid)
			{
				tWString sTempName = cString::To16Char(pCustomStoryCfg->GetString("Main", "Name", ""));
				if (sTempName == _W(""))
				{
					sErrorMsg = "custom story has no name!";
				}

				if (sTempName == sStoryName)
				{
					//Custom GUI
					msBackgroundFile = pCustomStoryCfg->GetString("RankScreen", "BackgroundImage", "demo_end_image.jpg");

					mvMessageFontSize = pCustomStoryCfg->GetVector2f("RankScreen", "MessageFontSize", cVector2f(25, 25));
					mMessageFontColor = pCustomStoryCfg->GetColor("RankScreen", "MessageFontColor", cColor(1, 1));
					mGoldStatusColor = pCustomStoryCfg->GetColor("RankScreen", "RankedFontColor", cColor(232.0f / 255.0f, 201.0f / 255.0f, 28.0f / 255.0f));

					mfTextWidth = pCustomStoryCfg->GetFloat("RankScreen", "TextWidth", 500);
					mfTextY = pCustomStoryCfg->GetFloat("RankScreen", "TextY", 120);

					mvButtonPos = pCustomStoryCfg->GetVector3f("RankScreen", "ExitButtonPos", cVector3f(355, 550, 10));
					mvButtonSize = pCustomStoryCfg->GetVector2f("RankScreen", "ButtonFontSize", cVector2f(90, 45));
					mButtonFontColor = pCustomStoryCfg->GetColor("RankScreen", "ButtonFontColor", cColor(1, 0.9, 0.9, 1));


					//Custom Rank
					mlTimeHourRank = pCustomStoryCfg->GetInt("RankScreen", "TimeHourRank", 1);
					mlTimeMinRank = pCustomStoryCfg->GetInt("RankScreen", "TimeMinRank", 30);
					mlTimeSecRank = pCustomStoryCfg->GetInt("RankScreen", "TimeSecRank", 15);
					msTimeRankOperator = pCustomStoryCfg->GetString("RankScreen", "TimeRank_Operator", "<=");
					mbDisplayTimeRank = pCustomStoryCfg->GetBool("RankScreen", "DisplayTimeRank", true);


					mbDisplayEndingRank = pCustomStoryCfg->GetBool("RankScreen", "DisplayEndingRank", false);
					msEndingRank = pCustomStoryCfg->GetString("RankScreen", "EndingRank", "Good");

					mbDisplayGameClearRank = pCustomStoryCfg->GetBool("RankScreen", "DisplayGameClearRank", false);
					mbDisplayGameModeRank = pCustomStoryCfg->GetBool("RankScreen", "DisplayGameModeRank", false);

					mfMinDeathCountRank = pCustomStoryCfg->GetInt("RankScreen", "MinDeathRank", 0);
					mbDisplayDeathRank = pCustomStoryCfg->GetBool("RankScreen", "DisplayDeathRank", true);

					mfMinDamageRank = pCustomStoryCfg->GetInt("RankScreen", "MinDamageRank", 250);
					mbDisplayDamageRank = pCustomStoryCfg->GetBool("RankScreen", "DisplayDamageRank", true);

					mlItemAmount = pCustomStoryCfg->GetInt("RankScreen", "TotalItemAmount", 100);
					mbDisplayItemRank = gpBase->mpRankCfg->GetBool("RankScreen", "DisplayItemRank", true);

					mfMinHealthItemUsedRank = pCustomStoryCfg->GetInt("RankScreen", "HealthItemUsedRank", 5);
					mbDisplayHealthRank = pCustomStoryCfg->GetBool("RankScreen", "DisplayHealthRank", true);

					mfMinOilItemUsedRank = pCustomStoryCfg->GetInt("RankScreen", "OilItemUsedRank", 5);
					mbDisplayOilRank = pCustomStoryCfg->GetBool("RankScreen", "DisplayOilRank", true);

					mlCustomGlobalVarRankAmount = pCustomStoryCfg->GetInt("RankScreen", "CustomGlobalVarRankAmount", 0);

					//Reward Slides


					sErrorMsg = "smh";
					Log("Found current custom story from path \"%ls\" : %s.\n", sStoryPath.c_str(), sErrorMsg.c_str());

					break;
					//return;
				}
				else
				{
					sErrorMsg = "smh";
					Log("Error finding current custom story from path \"%ls\" : %s.\n", sStoryPath.c_str(), sErrorMsg.c_str());
				}
			}
			else
			{
				sErrorMsg = "could not find custom_story_settings.cfg file or it was invalid";
			}

			if (bValid == false)
				Log("Error creating custom story from path \"%ls\" : %s.\n", sStoryPath.c_str(), sErrorMsg.c_str());
		}
	}
	
	////////////////////////////////////////////
#if MAC_OS || LINUX
	msGameMode = cString::To16Char(gpBase->mbHardMode == true ? "Hard" : "Default");
	
	char sTemp[1024];
	sprintf(sTemp, "%02d:%02d:%02d", lHour, lMin, lSec);

	msTotalTime = cString::To16Char(sTemp);

	msEndingReceived = cString::To16Char(gpBase->mpEndingsHandler->GetPreviousEnding());
	msTotalDamageTaken = tWString(LongToWString(static_cast<long long>((gpBase->mpPlayer->GetTotalDamageTaken()))));
	msTotalDeath = tWString(LongToWString(static_cast<long long>((gpBase->mpPlayer->GetTotalDeaths()))));
	tWString sTemp2 = tWString(LongToWString(static_cast<long long>((gpBase->mpPlayer->GetTotalItemCount())))); //msPlayerItemAmount
	msHealthItemsUsed = tWString(LongToWString(static_cast<long long>((gpBase->mpPlayer->GetHealthItemsUsed()))));
	msOilItemsUsed = tWString(LongToWString(static_cast<long long>((gpBase->mpPlayer->GetOilItemsUsed()))));

	gsTotalItems_String = tWString(LongToWString((static_cast<long long>(mlItemAmount))));
#else
	 ////////////////////////////////////////////////////////
	 // Data
	 msGameMode = gpBase->mbHardMode == true ? kTranslate("MainMenu", "HardMode") : kTranslate("MainMenu", "NormalMode");

	 tWString gsTotalItems_String = tWString(to_wstring(static_cast<long long>(mlItemAmount)));

	 /////////////////////////////
	 /// Game Time
	 int mlCounter = gpBase->mpProgressLogHandler->GetProgLogCounter();
	 lSec = (mlCounter / 60) % 60;
	 lMin = (mlCounter / (60 * 60)) % 60;
	 lHour = mlCounter / (60 * 60 * 60);
	 char sTemp[1024];
	 sprintf(sTemp, "%02d:%02d:%02d", lHour, lMin, lSec);
	 tWString sTotalTimeTemp = cString::To16Char(sTemp);
	 msTotalTime = sTotalTimeTemp;
	 //vRankingVariableStrings.push_back(msTotalTime);

	 //Ending
	 string sEndingTemp = gpBase->mpEndingsHandler->GetPreviousEnding();
	 msEndingReceived = kTranslate("RankScreen", sEndingTemp);
	 //vRankingVariableStrings.push_back(msEndingReceived);

	 //Game Clear
	 tWString sClearTemp = tWString(std::to_wstring(static_cast<long long>(gpBase->mpEndingsHandler->GetTotalGameClears())));
	 msGameClear = sClearTemp;
	 //vRankingVariableStrings.push_back(msGameClear);

	 //Damage Taken
	 tWString sDamageTemp = tWString(std::to_wstring(static_cast<long long>(gpBase->mpPlayer->GetTotalDamageTaken())));
	 msTotalDamageTaken = sDamageTemp;
	 //vRankingVariableStrings.push_back(msTotalDamageTaken);

	 //Death Amount 
	 tWString sDeathTemp = tWString(std::to_wstring(static_cast<long long>(gpBase->mpPlayer->GetTotalDeaths())));
	 msTotalDeath = sDeathTemp;
	 //vRankingVariableStrings.push_back(msTotalDeath);

	 //Items Collected
	 tWString sTemp2 = tWString(std::to_wstring(static_cast<long long>(gpBase->mpPlayer->GetTotalItemCount())));  //msPlayerItemAmount

	 //Health Items Used
	 tWString sHealthTemp = tWString(std::to_wstring(static_cast<long long>(gpBase->mpPlayer->GetHealthItemsUsed())));
	 msHealthItemsUsed = sHealthTemp;
	 //vRankingVariableStrings.push_back(msHealthItemsUsed);

	 //Oil Items Used
	 tWString sOilTemp = tWString(std::to_wstring(static_cast<long long>(gpBase->mpPlayer->GetOilItemsUsed())));
	 msOilItemsUsed = sOilTemp;
	 //vRankingVariableStrings.push_back(msOilItemsUsed);

#endif
	
	 tWString sItemText = kTranslate("RankScreen", "TotalItems");
	 size_t lNumIndexAlt = sItemText.find(L"@");
	 size_t lNumIndex = sItemText.find(L"#");

	 sItemText.replace(lNumIndexAlt, sTemp2.size(), sTemp2);


	 sItemText.replace(lNumIndex, gsTotalItems_String.size(), gsTotalItems_String);
	 

	 msPlayerItemAmount = sItemText;
	 //vRankingVariableStrings.push_back(msPlayerItemAmount);

	 /////////////////////
	 /// Custom Vars

	 cConfigFile* pCfg;

	 if (gpBase->GetIsInCustomStory() == true)
	 {
		 pCfg = pCustomStoryCfg;

	 }
	 else
	 {
		 pCfg = gpBase->mpRankCfg;
	 }

	 if (mlCustomGlobalVarRankAmount != 0)
	 {

		 for (int i = 0; i <= mlCustomGlobalVarRankAmount; ++i)
		 {
			 string tempName1 = "CustomGlobalVarRank_" + cString::ToString(i+1) + "_Name";
			 string tempType1 = "CustomGlobalVarRank_" + cString::ToString(i+1) + "_Type";

			 const tString tempName2 = pCfg->GetString("RankScreen", tempName1, "");
			 const tString tempType2 = pCfg->GetString("RankScreen", tempType1, "");
			 
			 vCustomGlobalVarRankNames.push_back(tempName2);
			 vCustomGlobalVarRankType.push_back(tempType2);

			 const tString &tempVar = vCustomGlobalVarRankNames[i];

			 cLuxScriptVar* pVar = gpBase->mpGlobalDataHandler->GetVar(tempVar);

			 if (vCustomGlobalVarRankType[i] == "Int" || vCustomGlobalVarRankType[i] == "Float")
			 {
				 tString tempOper = "CustomGlobalVarRank_" + cString::ToString(i + 1) + "_Operator";

				 vCustomGlobalVarRankOper.push_back(pCfg->GetString("RankScreen", tempOper, "=="));

				 vCustomGlobalVarRankCurrentValueString.push_back(pVar->msVal);

				 string sNumValue = pVar->msVal;

				 if (vCustomGlobalVarRankType[i] == "Int")
				 {
					 tString tempInt = "CustomGlobalVarRank_" + cString::ToString(i + 1) + "_Num";
					 vCustomGlobalVarRankNum.push_back(pCfg->GetInt("RankScreen", tempInt, 0));

					 int tempCurrentInt = cString::ToInt(sNumValue.c_str(), 0);
					 vCustomGlobalVarRankCurrentValueInt.push_back(tempCurrentInt);

					#if MAC_OS || LINUX
					 tWString sDataTemp = tWString(LongToWString((static_cast<long long>(tempCurrentInt))));
					#else
					 tWString sDataTemp = tWString(std::to_wstring(static_cast<long long>(tempCurrentInt)));
					#endif
					 vCustomGlobalVarRankFinalTextData.push_back(sDataTemp);

					 //vRankingVariableStrings.push_back(vCustomGlobalVarRankFinalTextData[i]);
				 }
				 else if (vCustomGlobalVarRankType[i] == "Float")
				 {
					 tString tempFloat = "CustomGlobalVarRank_" + cString::ToString(i + 1) + "_Num";
					 vCustomGlobalVarRankNumFloat.push_back(pCfg->GetFloat("RankScreen", tempFloat, 0));

					 float tempCurrentFloat = cString::ToFloat(sNumValue.c_str(), 0);
					 vCustomGlobalVarRankCurrentValueFloat.push_back(tempCurrentFloat);

					#if MAC_OS || LINUX
					 tWString sDataTemp = tWString(LongToWString((static_cast<long long>(tempCurrentFloat))));
					#else
					 tWString sDataTemp = tWString(std::to_wstring(static_cast<long long>(tempCurrentFloat)));
					#endif
					 vCustomGlobalVarRankFinalTextData.push_back(sDataTemp);

					 //vRankingVariableStrings.push_back(vCustomGlobalVarRankFinalTextData[i]);
				 }
				 
			 }
			 else if (vCustomGlobalVarRankType[i] == "Bool")
			 {
				 tString tempBool = "CustomGlobalVarRank_" + cString::ToString(i+1) + "_RankBool";
				 //tempBoolVec[i] = tempBool;

				 vCustomGlobalVarRankBool.push_back(pCfg->GetBool("RankScreen", tempBool, true));

				 tString stempBool = cString::ToLowerCase(pVar->msVal);
				 vCustomGlobalVarRankCurrentValueString.push_back(stempBool);

				 if (vCustomGlobalVarRankCurrentValueString[i] == "true")
				 {
					 vCustomGlobalVarCurrentBool.push_back(true);
				 }
				 else
				 {
					 vCustomGlobalVarCurrentBool.push_back(false);
				 }

				 if (vCustomGlobalVarCurrentBool[i] == true) vCustomGlobalVarRankFinalTextData.push_back(kTranslate("RankScreen", "CustomGlobalVarRank_" + cString::ToString(i+1) + "_True"));
				 else if (vCustomGlobalVarCurrentBool[i] == false)  vCustomGlobalVarRankFinalTextData.push_back(kTranslate("RankScreen", "CustomGlobalVarRank_" + cString::ToString(i+1) + "_False"));

				 //vRankingVariableStrings.push_back(vCustomGlobalVarRankFinalTextData[i]);
			 }

			 
		 }
	 }

	 /////////////////////
	 /// Reward Slides
	 //if (mlRewardSlideAmount != 0)
	 //{
		// int lGameClears = gpBase->mpEndingsHandler->GetTotalGameClears();
		// string sMode = "HardMode";
		// bool bHardMode = gpBase->mpEndingsHandler->GetEndingCompleted(sMode);

		// for (int i = 0; i <= mlRewardSlideAmount; ++i)
		// {
		//	 tStringVec tempConditon;
		//	 tString tGCLR = "GameClear";
		//	 tString tHRDM = "HardMode";
		//	 std::vector<int>  tempGameClear;
		//	 tempConditon.push_back(pCfg->GetString("RankScreen", "RewardSlide_" + cString::ToString(i + 1) + "_RewardCondition", "GameClear"));

		//	 if (tempConditon[i] == tGCLR)
		//	 {
		//		 tempGameClear.push_back(pCfg->GetInt("RankScreen", "RewardSlide_" + cString::ToString(i + 1) + "_GameClearAmount", 1));
		//	 }

		//	 if (tempConditon[i] == tGCLR && tempGameClear[i] == lGameClears || tempConditon[i] == tHRDM && bHardMode == true)
		//	 {
		//		 vRewardSlideConditions.push_back(pCfg->GetString("RankScreen", "RewardSlide_" + cString::ToString(i + 1) + "_RewardCondition", "GameClear"));

		//		 vRewardSlideGameClearAmount.push_back(pCfg->GetInt("RankScreen", "RewardSlide_" + cString::ToString(i + 1) + "_GameClearAmount", 1));

		//		 vRewardSlideSounds.push_back(pCfg->GetString("RankScreen", "RewardSlide_" + cString::ToString(i + 1) + "_Sound", ""));
		//		 vRewardSlideBackgroundImage.push_back(pCfg->GetString("RankScreen", "RewardSlide_" + cString::ToString(i + 1) + "_BackgroundImage", ""));

		//		 //vRewardSlideTextCategories.push_back(pCfg->GetString("RankScreen", "RewardSlide_" + cString::ToString(i + 1) + "_TextCat", ""));
		//		 vRewardSlideTextEntries.push_back(pCfg->GetString("RankScreen", "RewardSlide_" + cString::ToString(i + 1) + "_TextEntry", ""));
		//		 vRewardSlideTitleSize.push_back(pCfg->GetVector2f("RankScreen", "RewardSlide_" + cString::ToString(i + 1) + "_TitleSize", cVector2f(24, 24)));
		//		 vRewardSlideTitlePos.push_back(pCfg->GetVector3f("RankScreen", "RewardSlide_" + cString::ToString(i + 1) + "_TitlePos", cVector3f(80, 210, 0)));
		//		 vRewardSlideDescSize.push_back(pCfg->GetVector2f("RankScreen", "RewardSlide_" + cString::ToString(i + 1) + "_TitleSize", cVector2f(18, 18)));
		//		 vRewardSlideDescPos.push_back(pCfg->GetVector3f("RankScreen", "RewardSlide_" + cString::ToString(i + 1) + "_TitlePos", cVector3f(80, 450, 0)));
		//		 vRewardSlideTextColors.push_back(pCfg->GetColor("RankScreen", "RewardSlide_" + cString::ToString(i + 1) + "_TextColor", cColor(1, 1, 1)));
		//		 vRewardSlideTextAlign.push_back(pCfg->GetString("RankScreen", "RewardSlide_" + cString::ToString(i + 1) + "_TextAlign", "Center"));
		//	 }
		// }

		// mlValidSlideAmount = vRewardSlideConditions.size(); 
	 //}
 
}

void cLuxRankScreen::LoadFonts()
{
	tString sFontMessage = gpBase->mpRankCfg->GetString("RankScreen", "MessageFont", "");
	tString sFontButton = gpBase->mpRankCfg->GetString("RankScreen", "ButtonFont", "");
	mpFontMessage = LoadFont(sFontMessage);
	//mpFontButton = LoadFont(sFontButton);

	if (mpFontButton)
	{
		
		//mpBExit->SetDefaultFontType(mpFontButton);
	}
}
//-----------------------------------------------------------------------

void cLuxRankScreen::OnEnterContainer(const tString& asOldContainer)
{
	LoadData();
	//Unlock input if not in window
	if (gpBase->mpConfigHandler->mbFullscreen == false) {
		gpBase->mpEngine->GetInput()->GetLowLevel()->LockInput(false);
	}
	gpBase->mpEngine->GetInput()->GetLowLevel()->RelativeMouse(false);

	////////////////////////////
	//Set up states and viewport
	gpBase->mpInputHandler->ChangeState(eLuxInputState_RankScreen);

	mpViewport->SetActive(true);
	mpViewport->SetVisible(true);

	mpGuiSet->SetActive(true);
	mpGuiSet->SetDrawMouse(true);
	mpGui->SetFocus(mpGuiSet);

	mpBExit->SetPosition(mvButtonPos);
	mpBExit->SetSize(mvButtonSize);
	mpBExit->SetVisible(true);
	mpBExit->SetEnabled(true);

	gpBase->mpProgressLogHandler->SetProgLogCounterActive(false);

	mbShowRankScreen = true;
	mlCurrentRewardSlide = -1;
	mbRewardSlideSoundPlayed = false;

	// Load background
	iTexture* pTex = gpBase->mpEngine->GetResources()->GetTextureManager()->Create2D(msBackgroundFile, false, eTextureType_Rect);
	if (pTex) mpGfxBackground = mpGui->CreateGfxTexture(pTex, true, eGuiMaterial_Alpha);

	///////////////////////////////
	//Play music
	if (msMusic != "")
	{
		cMusicHandler* pMusHandler = gpBase->mpEngine->GetSound()->GetMusicHandler();
		pMusHandler->Play(msMusic, mfMusicVolume, mfMusicFade, mbLoopMusic, false);
	}
}
//-----------------------------------------------------------------------

void cLuxRankScreen::OnLeaveContainer(const tString& asNewContainer)
{
	//Turn off viewport and sets
	mpViewport->SetActive(false);
	mpViewport->SetVisible(false);

	mpGuiSet->SetActive(false);

	mpBExit->SetVisible(false);
	mpBExit->SetEnabled(false);

	cMusicHandler* pMusHandler = gpBase->mpEngine->GetSound()->GetMusicHandler();
	pMusHandler->Stop(0.3f);

	//Destroy background graphics
	if (mpGfxBackground)
	{
		mpGui->DestroyGfx(mpGfxBackground);
		mpGfxBackground = NULL;
	}
}

//-----------------------------------------------------------------------

void cLuxRankScreen::Update(float afTimeStep)
{
	mfTime += afTimeStep;

	if (mbExiting)
	{
		mfFadeAlpha += afTimeStep * 0.7f;
		if (mfFadeAlpha > 1)
		{
			mfFadeAlpha = 1;

			//if (mbExiting)
			//
				gpBase->mpEngine->GetUpdater()->BroadcastMessageToAll(eUpdateableMessage_Reset);
				gpBase->mpLoadScreenHandler->DrawMenuScreen();
				gpBase->mpEngine->GetUpdater()->SetContainer("MainMenu");

				//cPlatform::OpenBrowserWindow(cString::To16Char(msDestinationURL));
			//}
			/*else
			{
				mbStartFading = false;
				gpBase->mpInputHandler->ChangeState(eLuxInputState_RankScreen);
				mpGuiSet->SetDrawMouse(true);

				if (mlValidSlideAmount != 0 && mlCurrentRewardSlide <= mlValidSlideAmount)
				{
					mlCurrentRewardSlide += 1;
					if (mbRewardSlideSoundPlayed == false && vRewardSlideSounds[mlCurrentRewardSlide] != "")
					{
						cSoundHandler* pSoundHandler = gpBase->mpEngine->GetSound()->GetSoundHandler();
						pSoundHandler->PlayGui(vRewardSlideSounds[mlCurrentRewardSlide], false, 1);
						mbRewardSlideSoundPlayed = true;
					}
				}
			}*/
		}
	}
	else
	{
		if (mfFadeAlpha > 0.0f)
		{
			mfFadeAlpha -= afTimeStep * 0.7f;
			if (mfFadeAlpha < 0) mfFadeAlpha = 0;
		}
	}
}

//-----------------------------------------------------------------------

void cLuxRankScreen::Exit()
{
	mbExiting = true;
	gpBase->mpInputHandler->ChangeState(eLuxInputState_Null);
	mpGuiSet->SetDrawMouse(false);
}
//-----------------------------------------------------------------------

void cLuxRankScreen::Setup(const tString& asMusic, bool abLoopMusic, float afMusicVol, float afMusicFade, string& asImageName)
{
	msMusic = asMusic;
	mbLoopMusic = abLoopMusic;
	mfMusicVolume = afMusicVol;
	mfMusicFade = afMusicFade;
	
	msBackgroundFile = asImageName; 

}
//-----------------------------------------------------------------------

void cLuxRankScreen::OnDraw(float afFrameTime)
{
	//////////////////////////////////////////////
	// Background
	mpGuiSet->DrawGfx(mpBlackGfx, mvGuiSetStartPos, mvGuiSetSize);
	LoadData();

	if (mpGfxBackground)
	{
		//Draw the image with correct ratio, always filling out height-wise.
		cVector2f vSize = mpGfxBackground->GetImageSize();
		float fRatio = vSize.x / vSize.y;
		float fRatioMul = fRatio / (4.0f / 3.0f);

		mpGuiSet->DrawGfx(mpGfxBackground, cVector3f(400 * (1 - fRatioMul), 0, 5), cVector2f(800 * fRatioMul, 600), cColor(1, 1));
	}
	//Set button visible 
	mpBExit->SetPosition(mvButtonPos);
	mpBExit->SetSize(mvButtonSize);
	mpBExit->SetVisible(true);
	mpBExit->SetEnabled(true);
	
	//LoadData();


	//////////////////////////////////////////////
	// Text
	if (mpFontMessage)
	{
		float fY = 50.0f;
		float fX = 200.0f;
		float fX2 = 600.0f;

		cColor mGameModeColor = gpBase->mbHardMode == true ? mGoldStatusColor : mMessageFontColor;
		cColor mTotalTimeColor;

		///Time
		if (msTimeRankOperator == "<=")
		{
			if (lHour <= mlTimeHourRank && lMin <= mlTimeMinRank) mTotalTimeColor = mGoldStatusColor;
			else mTotalTimeColor = mMessageFontColor;
		}
		else if (msTimeRankOperator == ">=")
		{
			if (lHour >= mlTimeHourRank && lMin >= mlTimeMinRank) mTotalTimeColor = mGoldStatusColor;
			else mTotalTimeColor = mMessageFontColor;
		}
		else if (msTimeRankOperator == ">")
		{
			if (lHour > mlTimeHourRank && lMin > mlTimeMinRank) mTotalTimeColor = mGoldStatusColor;
			else mTotalTimeColor = mMessageFontColor;
		}
		else if (msTimeRankOperator == "<")
		{
			if (lHour < mlTimeHourRank && lMin < mlTimeMinRank) mTotalTimeColor = mGoldStatusColor;
			else mTotalTimeColor = mMessageFontColor;
		}
		else if (msTimeRankOperator == "==")
		{
			if (lHour == mlTimeHourRank && lMin == mlTimeMinRank) mTotalTimeColor = mGoldStatusColor;
			else mTotalTimeColor = mMessageFontColor;
		}
		else if (msTimeRankOperator == "!=")
		{
			if (lHour != mlTimeHourRank && lMin != mlTimeMinRank) mTotalTimeColor = mGoldStatusColor;
			else mTotalTimeColor = mMessageFontColor;
		}

		//Ending
		cColor mEndingColor;
		string sEnding = gpBase->mpEndingsHandler->GetPreviousEnding(); 
		
		if (sEnding == msEndingRank)
		{
			mEndingColor = mGoldStatusColor;
		}
		else
		{
			mEndingColor = mMessageFontColor;
		}
	
		cColor mTotalDeathsColor = gpBase->mpPlayer->GetTotalDeaths() <= mfMinDeathCountRank ? mGoldStatusColor : mMessageFontColor;
		cColor mTotalDamageColor = gpBase->mpPlayer->GetTotalDamageTaken() <= mfMinDamageRank ? mGoldStatusColor : mMessageFontColor;
		cColor mTotalItemsColor = gpBase->mpPlayer->GetTotalItemCount() >= 69 ? mGoldStatusColor : mMessageFontColor;
		cColor mHealthItemsColor = gpBase->mpPlayer->GetHealthItemsUsed() <= mfMinHealthItemUsedRank ? mGoldStatusColor : mMessageFontColor;
		cColor mOilItemsColor = gpBase->mpPlayer->GetOilItemsUsed() <= mfMinOilItemUsedRank ? mGoldStatusColor : mMessageFontColor;
		cColor mStatuesColor = mlStatuesVal == 12 ? mGoldStatusColor : mMessageFontColor;
		
		//Heading
		mpGuiSet->DrawFont(kTranslate("RankScreen", "RankScreenTitle"), mpFontMessage, cVector3f(400.0f, fY, 10), mvTitleFontSize, mTitleColor, eFontAlign_Center);

		fY += 75.0f;

		//Game Mode
		if (mbDisplayGameModeRank)
		{
			mpGuiSet->DrawFont(kTranslate("RankScreen", "GameMode"), mpFontMessage, cVector3f(fX, fY, 10), mvMessageFontSize, mMessageFontColor, eFontAlign_Left);
			mpGuiSet->DrawFont(msGameMode, mpFontMessage, cVector3f(fX2, fY, 10), mvMessageFontSize, mGameModeColor, eFontAlign_Right);
			fY += 35;
		}

		
		//Game Clear
		if (mbDisplayGameClearRank)
		{
			mpGuiSet->DrawFont(kTranslate("RankScreen", "TotalGameClear"), mpFontMessage, cVector3f(fX, fY, 10), mvMessageFontSize, mMessageFontColor, eFontAlign_Left);
			mpGuiSet->DrawFont(msGameClear, mpFontMessage, cVector3f(fX2, fY, 10), mvMessageFontSize, mMessageFontColor, eFontAlign_Right);
			fY += 35;
		}

		
		//Timer
		if (mbDisplayTimeRank)
		{
			mpGuiSet->DrawFont(kTranslate("RankScreen", "TotalTime"), mpFontMessage, cVector3f(fX, fY, 10), mvMessageFontSize, mMessageFontColor, eFontAlign_Left);
			mpGuiSet->DrawFont(msTotalTime, mpFontMessage, cVector3f(fX2, fY, 10), mvMessageFontSize, mTotalTimeColor, eFontAlign_Right);
			fY += 35;
		}

		
		//Ending
		if (mbDisplayEndingRank)
		{
			mpGuiSet->DrawFont(kTranslate("RankScreen", "Ending"), mpFontMessage, cVector3f(fX, fY, 10), mvMessageFontSize, mMessageFontColor, eFontAlign_Left);
			mpGuiSet->DrawFont(msEndingReceived, mpFontMessage, cVector3f(fX2, fY, 10), mvMessageFontSize, mEndingColor, eFontAlign_Right);
			fY += 35;
		}

		//Death & Damage
		if (mbDisplayDamageRank)
		{
			mpGuiSet->DrawFont(kTranslate("RankScreen", "TotalDamage"), mpFontMessage, cVector3f(fX, fY, 10), mvMessageFontSize, mMessageFontColor, eFontAlign_Left);
			mpGuiSet->DrawFont(msTotalDamageTaken, mpFontMessage, cVector3f(fX2, fY, 10), mvMessageFontSize, mTotalDamageColor, eFontAlign_Right);
			fY += 35;
		}

		if (mbDisplayDeathRank)
		{
			mpGuiSet->DrawFont(kTranslate("RankScreen", "TotalDeaths"), mpFontMessage, cVector3f(fX, fY, 10), mvMessageFontSize, mMessageFontColor, eFontAlign_Left);
			mpGuiSet->DrawFont(msTotalDeath, mpFontMessage, cVector3f(fX2, fY, 10), mvMessageFontSize, mTotalDeathsColor, eFontAlign_Right);
			fY += 35;
		}

		//Items
		if (mbDisplayItemRank)
		{
			mpGuiSet->DrawFont(kTranslate("RankScreen", "ItemsFoundByPlayer"), mpFontMessage, cVector3f(fX, fY, 10), mvMessageFontSize, mMessageFontColor, eFontAlign_Left);
			mpGuiSet->DrawFont(msPlayerItemAmount, mpFontMessage, cVector3f(fX2, fY, 10), mvMessageFontSize, mTotalItemsColor, eFontAlign_Right);
			fY += 35;
		}

		if (mbDisplayHealthRank)
		{
			mpGuiSet->DrawFont(kTranslate("RankScreen", "HealthUsed"), mpFontMessage, cVector3f(fX, fY, 10), mvMessageFontSize, mMessageFontColor, eFontAlign_Left);
			mpGuiSet->DrawFont(msHealthItemsUsed, mpFontMessage, cVector3f(fX2, fY, 10), mvMessageFontSize, mHealthItemsColor, eFontAlign_Right);
			fY += 35;
		}

		if (mbDisplayOilRank)
		{
			mpGuiSet->DrawFont(kTranslate("RankScreen", "OilUsed"), mpFontMessage, cVector3f(fX, fY, 10), mvMessageFontSize, mMessageFontColor, eFontAlign_Left);
			mpGuiSet->DrawFont(msOilItemsUsed, mpFontMessage, cVector3f(fX2, fY, 10), mvMessageFontSize, mOilItemsColor, eFontAlign_Right);
			fY += 35;
		}

		////////////////
		//Custom Rank Vars

		if (mlCustomGlobalVarRankAmount != 0)
		{
			std::vector<cColor> vColorVec; //= mlStatuesVal == 12 ? mGoldStatusColor : mMessageFontColor;

			for (int i = 0; i <= mlCustomGlobalVarRankAmount; ++i)
			{
				if (vCustomGlobalVarRankType[i] == "Int")
				{
					if(vCustomGlobalVarRankOper[i] == "==") vColorVec.push_back(vCustomGlobalVarRankCurrentValueInt[i] == vCustomGlobalVarRankNum[i] ? mGoldStatusColor : mMessageFontColor);
					else if (vCustomGlobalVarRankOper[i] == ">=") vColorVec.push_back(vCustomGlobalVarRankCurrentValueInt[i] >= vCustomGlobalVarRankNum[i] ? mGoldStatusColor : mMessageFontColor);
					else if (vCustomGlobalVarRankOper[i] == "<=") vColorVec.push_back(vCustomGlobalVarRankCurrentValueInt[i] <= vCustomGlobalVarRankNum[i] ? mGoldStatusColor : mMessageFontColor);
					else if (vCustomGlobalVarRankOper[i] == "<")  vColorVec.push_back(vCustomGlobalVarRankCurrentValueInt[i] < vCustomGlobalVarRankNum[i] ? mGoldStatusColor : mMessageFontColor);
					else if (vCustomGlobalVarRankOper[i] == ">")  vColorVec.push_back(vCustomGlobalVarRankCurrentValueInt[i] > vCustomGlobalVarRankNum[i] ? mGoldStatusColor : mMessageFontColor);
					else if (vCustomGlobalVarRankOper[i] == "!=")  vColorVec.push_back(vCustomGlobalVarRankCurrentValueInt[i] != vCustomGlobalVarRankNum[i] ? mGoldStatusColor : mMessageFontColor);
				}
				else if (vCustomGlobalVarRankType[i] == "Float")
				{
					if (vCustomGlobalVarRankOper[i] == "==") vColorVec.push_back(vCustomGlobalVarRankCurrentValueFloat[i] == vCustomGlobalVarRankNumFloat[i] ? mGoldStatusColor : mMessageFontColor);
					else if (vCustomGlobalVarRankOper[i] == ">=") vColorVec.push_back(vCustomGlobalVarRankCurrentValueFloat[i] >= vCustomGlobalVarRankNumFloat[i] ? mGoldStatusColor : mMessageFontColor);
					else if (vCustomGlobalVarRankOper[i] == "<=") vColorVec.push_back(vCustomGlobalVarRankCurrentValueFloat[i] <= vCustomGlobalVarRankNumFloat[i] ? mGoldStatusColor : mMessageFontColor);
					else if (vCustomGlobalVarRankOper[i] == "<")  vColorVec.push_back(vCustomGlobalVarRankCurrentValueFloat[i] < vCustomGlobalVarRankNumFloat[i] ? mGoldStatusColor : mMessageFontColor);
					else if (vCustomGlobalVarRankOper[i] == ">")  vColorVec.push_back(vCustomGlobalVarRankCurrentValueFloat[i] > vCustomGlobalVarRankNumFloat[i] ? mGoldStatusColor : mMessageFontColor);
					else if (vCustomGlobalVarRankOper[i] == "!=")  vColorVec.push_back(vCustomGlobalVarRankCurrentValueFloat[i] != vCustomGlobalVarRankNumFloat[i] ? mGoldStatusColor : mMessageFontColor);
				}
				else if (vCustomGlobalVarRankType[i] == "Bool")
				{
					vColorVec.push_back(vCustomGlobalVarCurrentBool[i] == vCustomGlobalVarRankBool[i] ? mGoldStatusColor : mMessageFontColor);
				}

				mpGuiSet->DrawFont(kTranslate("RankScreen", "CustomGlobalVarRank_"+cString::ToString(i+1)), mpFontMessage, cVector3f(fX, fY, 10), mvMessageFontSize, mMessageFontColor, eFontAlign_Left);
				mpGuiSet->DrawFont(vCustomGlobalVarRankFinalTextData[i], mpFontMessage, cVector3f(fX2, fY, 10), mvMessageFontSize, vColorVec[i], eFontAlign_Right);
				fY += 35;
			}
			
		}

	}
	/*else if (mbShowRankScreen == false && mlValidSlideAmount != 0)
	{
		int i = mlCurrentRewardSlide;
		{
			eFontAlign eAlign = ToFontAlign(vRewardSlideTextAlign[i]);
			int lGameClears = gpBase->mpEndingsHandler->GetTotalGameClears();
			string sMode = "HardMode";
			bool bHardMode = gpBase->mpEndingsHandler->GetEndingCompleted(sMode);
			
			if (vRewardSlideConditions[i] == "GameClear" && vRewardSlideGameClearAmount[i] == lGameClears || vRewardSlideConditions[i] == "HardMode" && bHardMode == true)
			{
				iTexture* pTex = gpBase->mpEngine->GetResources()->GetTextureManager()->Create2D(vRewardSlideBackgroundImage[i], false, eTextureType_Rect);
				if (pTex) mpGfxBackground = mpGui->CreateGfxTexture(pTex, true, eGuiMaterial_Alpha);

				mpGuiSet->DrawFont(kTranslate("RankScreen", vRewardSlideTextEntries[i]+"_Title"), mpFontMessage, vRewardSlideTitlePos[i], vRewardSlideTitleSize[i], vRewardSlideTextColors[i], eAlign);
				mpGuiSet->DrawFont(kTranslate("RankScreen", vRewardSlideTextEntries[i] + "_Text"), mpFontMessage, vRewardSlideDescPos[i], vRewardSlideDescSize[i], vRewardSlideTextColors[i], eAlign);
			}
		}
	}*/

	//////////////////////////////////////////////
	// Fade
	if (mfFadeAlpha > 0)
	{
		mpGuiSet->DrawGfx(mpBlackGfx, mvGuiSetStartPos + cVector3f(0, 0, 20), mvGuiSetSize, cColor(1, mfFadeAlpha));
	}

}

//-----------------------------------------------------------------------

void cLuxRankScreen::AppLostInputFocus()
{
	cMusicHandler* pMusHdlr = gpBase->mpEngine->GetSound()->GetMusicHandler();
	pMusHdlr->Pause();
}

//-----------------------------------------------------------------------

void cLuxRankScreen::AppGotInputFocus()
{
	cMusicHandler* pMusHdlr = gpBase->mpEngine->GetSound()->GetMusicHandler();
	pMusHdlr->Resume();
}

//-----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// PRIVATE METHODS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

bool cLuxRankScreen::ExitOnPressed(iWidget* apWidget, const cGuiMessageData& aData)
{
	/*if (mbShowRankScreen == false && mlCurrentRewardSlide == mlValidSlideAmount || mbShowRankScreen && mlRewardSlideAmount <= 0)
	{
		mbStartFading = true;*/
		Exit();
	//}
	//else if (mbShowRankScreen && mlValidSlideAmount != 0)
	//{
	//	mbStartFading = true;
	//	mbShowRankScreen = false;
	//	//mlCurrentRewardSlide += 1;
	//	gpBase->mpInputHandler->ChangeState(eLuxInputState_Null);
	//	mpGuiSet->SetDrawMouse(false);
	//	mbRewardSlideSoundPlayed = false;
	//}
	//else if (mbShowRankScreen == false && mlValidSlideAmount != 0 && mlCurrentRewardSlide <= mlValidSlideAmount)
	//{
	//	mbStartFading = true;
	//	//mlCurrentRewardSlide += 1;
	//	gpBase->mpInputHandler->ChangeState(eLuxInputState_Null);
	//	mpGuiSet->SetDrawMouse(false);
	//	mbRewardSlideSoundPlayed = false;
	//}
	return true;
}
kGuiCallbackDeclaredFuncEnd(cLuxRankScreen, ExitOnPressed);

bool cLuxRankScreen::UIExitOnPressed(iWidget* apWidget, const cGuiMessageData& aData)
{
	if (aData.mlVal == eUIButton_Primary)
	{
		//if (mbShowRankScreen == false && mlCurrentRewardSlide == mlValidSlideAmount || mbShowRankScreen && mlRewardSlideAmount <= 0)
		//{
			Exit();
		//}
		return true;
		
	}
	return false;
}
kGuiCallbackDeclaredFuncEnd(cLuxRankScreen, UIExitOnPressed);

//-----------------------------------------------------------------------

bool cLuxRankScreen::ButtonLabelOnUpdate(iWidget* apWidget, const cGuiMessageData& aData)
{
	bool bMouseOver = apWidget->GetUserValue() == 1;
	cColor col = mButtonFontColor;
	float* pFade = (float*)apWidget->GetUserData();

	if (bMouseOver)
	{
		if (*pFade < 1.0f)
		{
			*pFade += aData.mfVal * 3.0f;
			if (*pFade > 1)*pFade = 1;
		}
	}
	else
	{
		if (*pFade > 0)
		{
			*pFade -= aData.mfVal;
			if (*pFade < 0)*pFade = 0;
		}
	}

	float fT = *pFade;

	col = col * (1.0f - fT) + cColor(1, 0, 0) * fT;

	apWidget->SetDefaultFontColor(col);

	return true;
}
kGuiCallbackDeclaredFuncEnd(cLuxRankScreen, ButtonLabelOnUpdate);

//-----------------------------------------------------------------------

bool cLuxRankScreen::ButtonLabelOnDraw(iWidget* apWidget, const cGuiMessageData& aData)
{
	return true;
}
kGuiCallbackDeclaredFuncEnd(cLuxRankScreen, ButtonLabelOnDraw);

//-----------------------------------------------------------------------

bool cLuxRankScreen::ButtonLabelOnMouseEnter(iWidget* apWidget, const cGuiMessageData& aData)
{
	apWidget->SetUserValue(1);
	return true;
}
kGuiCallbackDeclaredFuncEnd(cLuxRankScreen, ButtonLabelOnMouseEnter);

//-----------------------------------------------------------------------

bool cLuxRankScreen::ButtonLabelOnMouseLeave(iWidget* apWidget, const cGuiMessageData& aData)
{
	apWidget->SetUserValue(0);
	return true;
}
kGuiCallbackDeclaredFuncEnd(cLuxRankScreen, ButtonLabelOnMouseLeave);

//-----------------------------------------------------------------------
