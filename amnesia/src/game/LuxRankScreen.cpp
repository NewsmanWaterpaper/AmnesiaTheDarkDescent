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

#include <sstream>

 //-----------------------------------------------------------------------

const int glTotalItems_Num = 98;

#if MAC_OS || LINUX
std::wstring LongToWString(const long long& l)
{
	std::string s;
	std::stringstream strstream;
	strstream << l;
	strstream >> s;
	std::wstring temp(s.length(), L' ');
	std::copy(s.begin(), s.end(), temp.begin());
	return temp;
}

const tWString gsTotalItems_String = tWString(LongToWString((static_cast<long long>(glTotalItems_Num))));
#else
const tWString gsTotalItems_String = tWString(to_wstring(static_cast<long long>(glTotalItems_Num)));
#endif

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
	mGoldStatusColor = cColor(232.0f/255.0f, 201.0f/255.0f, 28.0f/255.0f);

	///////////////////////////////
	//Load Data
	tString sSkinFile = gpBase->mpRankCfg->GetString("RankScreen", "Skin", "gui_default.skin");
	mpGuiSet->SetSkin(mpGui->CreateSkin(sSkinFile));

	msBackgroundFile = gpBase->mpRankCfg->GetString("RankScreen", "BackgroundImage", "");

	mvMessageFontSize = gpBase->mpRankCfg->GetVector2f("RankScreen", "MessageFontSize", 0);
	mMessageFontColor = gpBase->mpRankCfg->GetColor("RankScreen", "MessageFontColor", cColor(1, 1));

	mfTextWidth = gpBase->mpRankCfg->GetFloat("RankScreen", "TextWidth", 0);
	mfTextY = gpBase->mpRankCfg->GetFloat("RankScreen", "TextY", 0);

	mfExitButtonY = gpBase->mpRankCfg->GetFloat("RankScreen", "ExitButtonY", 0);

	mpBlackGfx = mpGui->CreateGfxFilledRect(cColor(0, 1), eGuiMaterial_Alpha);

	mfFadeSpeed = gpBase->mpRankCfg->GetFloat("RankScreen", "FadeSpeed", 0);

	mbShowOnAllExit = gpBase->mpRankCfg->GetBool("RankScreen", "ShowOnAllExit", false);

	///////////////////////////////////////

	tWString sButtonLabel = kTranslate("RankScreen", "Exit");
	
	float fButtonWidth = 20.0f;
	float fButtonHeight = 30.0f;

	mvButtonSize = cVector2f(fButtonWidth+60, fButtonHeight+5) + 10;
	mvButtonPos = cVector3f(355,550,10);

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
	mbLoopMusic = false;
}

//-----------------------------------------------------------------------

void cLuxRankScreen::LoadData()
{
	cLuxHelpFuncs *pHelpFuncs = gpBase->mpHelpFuncs;

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
#else
	 ////////////////////////////////////////////////////////
	 // Data
	 msGameMode = gpBase->mbHardMode == true ? kTranslate("MainMenu", "HardMode") : kTranslate("MainMenu", "NormalMode");

	 int mlCounter = gpBase->mpProgressLogHandler->GetProgLogCounter();
	 lSec = (mlCounter / 60) % 60;
	 lMin = (mlCounter / (60 * 60)) % 60;
	 lHour = mlCounter / (60 * 60 * 60);
	 char sTemp[1024];
	 sprintf(sTemp, "%02d:%02d:%02d", lHour, lMin, lSec);
	 tWString sTotalTimeTemp = cString::To16Char(sTemp);
	 msTotalTime = sTotalTimeTemp;

	 string sEndingTemp = gpBase->mpEndingsHandler->GetPreviousEnding();
	 msEndingReceived = kTranslate("RankScreen", sEndingTemp);

	 tWString sClearTemp = tWString(std::to_wstring(static_cast<long long>(gpBase->mpEndingsHandler->GetTotalGameClears())));
	 msGameClear = sClearTemp;

	 tWString sDamageTemp = tWString(std::to_wstring(static_cast<long long>(gpBase->mpPlayer->GetTotalDamageTaken())));
	 msTotalDamageTaken = sDamageTemp;

	 tWString sDeathTemp = tWString(std::to_wstring(static_cast<long long>(gpBase->mpPlayer->GetTotalDeaths())));
	 msTotalDeath = sDeathTemp;

	 tWString sTemp2 = tWString(std::to_wstring(static_cast<long long>(gpBase->mpPlayer->GetTotalItemCount())));  //msPlayerItemAmount

	 tWString sHealthTemp = tWString(std::to_wstring(static_cast<long long>(gpBase->mpPlayer->GetHealthItemsUsed())));
	 msHealthItemsUsed = sHealthTemp;

	 tWString sOilTemp = tWString(std::to_wstring(static_cast<long long>(gpBase->mpPlayer->GetOilItemsUsed())));
	 msOilItemsUsed = sOilTemp;

#endif
	
	 tWString sItemText = kTranslate("RankScreen", "TotalItems");
	 size_t lNumIndexAlt = sItemText.find(L"@");
	 size_t lNumIndex = sItemText.find(L"#");

	 sItemText.replace(lNumIndexAlt, sTemp2.size(), sTemp2);
	 sItemText.replace(lNumIndex, gsTotalItems_String.size(), gsTotalItems_String);
	 

	 msPlayerItemAmount = sItemText;

	 cLuxScriptVar* pVar = gpBase->mpGlobalDataHandler->GetVar("AlienEnding");
	 msStatuesVal = pVar->msVal;
	 mlStatuesVal = 0;
	 if (msStatuesVal != "0")
	 {
		 mlStatuesVal = cString::ToInt(msStatuesVal.c_str(), 0);
		 //mlStatuesVal += 1;
	 }

	 if (mlStatuesVal >= 1)
	 {
		 tWString sStatueTemp = tWString(std::to_wstring(static_cast<long long>(mlStatuesVal)));
		 msStatuesFound = sStatueTemp;
	 }
}

void cLuxRankScreen::LoadFonts()
{
	tString sFontMessage = gpBase->mpRankCfg->GetString("RankScreen", "MessageFont", "");
	//tString sFontButton = gpBase->mpDemoCfg->GetString("DemoEnd", "ButtonFont", "");
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

	// Load background
	iTexture* pTex = gpBase->mpEngine->GetResources()->GetTextureManager()->Create2D(msBackgroundFile, false, eTextureType_Rect);
	if (pTex) mpGfxBackground = mpGui->CreateGfxTexture(pTex, true, eGuiMaterial_Alpha);

	///////////////////////////////
	//Play music
	if (msMusic != "")
	{
		cMusicHandler* pMusHandler = gpBase->mpEngine->GetSound()->GetMusicHandler();
		pMusHandler->Play(msMusic, 1.0f, 1.0f, mbLoopMusic, false);
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
			gpBase->mpEngine->GetUpdater()->BroadcastMessageToAll(eUpdateableMessage_Reset);

			gpBase->mpLoadScreenHandler->DrawMenuScreen();
			gpBase->mpEngine->GetUpdater()->SetContainer("MainMenu");
			
				//cPlatform::OpenBrowserWindow(cString::To16Char(msDestinationURL));
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

void cLuxRankScreen::Setup(const tString& asMusic, bool abLoopMusic, string& asImageName)
{
	msMusic = asMusic;
	mbLoopMusic = abLoopMusic;
	
	msBackgroundFile = asImageName; 
}
//-----------------------------------------------------------------------

void cLuxRankScreen::OnDraw(float afFrameTime)
{
	//////////////////////////////////////////////
	// Background
	mpGuiSet->DrawGfx(mpBlackGfx, mvGuiSetStartPos, mvGuiSetSize);

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
	
	LoadData();


	//////////////////////////////////////////////
	// Text
	if (mpFontMessage)
	{
		float fY = 50.0f;
		float fX = 200.0f;
		float fX2 = 600.0f;

		cColor mGameModeColor = gpBase->mbHardMode == true ? mGoldStatusColor : mMessageFontColor;
		cColor mTotalTimeColor;
		if (lHour <= 1 && lMin <= 30)
		{
			mTotalTimeColor = mGoldStatusColor;
		}
		else
		{
			mTotalTimeColor = mMessageFontColor;
		}
		cColor mEndingColor;
		string sEnding = gpBase->mpEndingsHandler->GetPreviousEnding(); 
		if (sEnding == "Default" || sEnding == "GoodJoke")
		{
			mEndingColor = mGoldStatusColor;
		}
		else
		{
			mEndingColor = mMessageFontColor;
		}
	
		cColor mTotalDeathsColor = gpBase->mpPlayer->GetTotalDeaths() <= 0 ? mGoldStatusColor : mMessageFontColor;
		cColor mTotalDamageColor = gpBase->mpPlayer->GetTotalDamageTaken() <= 250 ? mGoldStatusColor : mMessageFontColor;
		cColor mTotalItemsColor = gpBase->mpPlayer->GetTotalItemCount() >= 69 ? mGoldStatusColor : mMessageFontColor;
		cColor mHealthItemsColor = gpBase->mpPlayer->GetHealthItemsUsed() <= 5 ? mGoldStatusColor : mMessageFontColor;
		cColor mOilItemsColor = gpBase->mpPlayer->GetOilItemsUsed() <= 5 ? mGoldStatusColor : mMessageFontColor;
		cColor mStatuesColor = mlStatuesVal == 12 ? mGoldStatusColor : mMessageFontColor;
		
		//Heading
		mpGuiSet->DrawFont(kTranslate("RankScreen", "RankScreenTitle"), mpFontMessage, cVector3f(400.0f, fY, 10), mvTitleFontSize, mTitleColor, eFontAlign_Center);

		fY += 75.0f;

		//Game Mode
		mpGuiSet->DrawFont(kTranslate("RankScreen", "GameMode"), mpFontMessage, cVector3f(fX, fY, 10), mvMessageFontSize, mMessageFontColor, eFontAlign_Left);
		mpGuiSet->DrawFont(msGameMode, mpFontMessage, cVector3f(fX2, fY, 10), mvMessageFontSize, mGameModeColor, eFontAlign_Right);

		fY += 35;
		//Game Clear
		mpGuiSet->DrawFont(kTranslate("RankScreen", "TotalGameClear"), mpFontMessage, cVector3f(fX, fY, 10), mvMessageFontSize, mMessageFontColor, eFontAlign_Left);
		mpGuiSet->DrawFont(msGameClear, mpFontMessage, cVector3f(fX2, fY, 10), mvMessageFontSize, mMessageFontColor, eFontAlign_Right);

		fY += 35;
		//Timer
		mpGuiSet->DrawFont(kTranslate("RankScreen", "TotalTime"), mpFontMessage, cVector3f(fX, fY, 10), mvMessageFontSize, mMessageFontColor, eFontAlign_Left);
		mpGuiSet->DrawFont(msTotalTime, mpFontMessage, cVector3f(fX2, fY, 10), mvMessageFontSize, mTotalTimeColor, eFontAlign_Right);

		fY += 35;
		//Ending
		mpGuiSet->DrawFont(kTranslate("RankScreen", "Ending"), mpFontMessage, cVector3f(fX, fY, 10), mvMessageFontSize, mMessageFontColor, eFontAlign_Left);
		mpGuiSet->DrawFont(msEndingReceived, mpFontMessage, cVector3f(fX2, fY, 10), mvMessageFontSize, mEndingColor, eFontAlign_Right);

		fY += 35;
		//Death & Damage
		mpGuiSet->DrawFont(kTranslate("RankScreen", "TotalDamage"), mpFontMessage, cVector3f(fX, fY, 10), mvMessageFontSize, mMessageFontColor, eFontAlign_Left);
		mpGuiSet->DrawFont(msTotalDamageTaken, mpFontMessage, cVector3f(fX2, fY, 10), mvMessageFontSize, mTotalDamageColor, eFontAlign_Right);
		fY += 35;

		mpGuiSet->DrawFont(kTranslate("RankScreen", "TotalDeaths"), mpFontMessage, cVector3f(fX, fY, 10), mvMessageFontSize, mMessageFontColor, eFontAlign_Left);
		mpGuiSet->DrawFont(msTotalDeath, mpFontMessage, cVector3f(fX2, fY, 10), mvMessageFontSize, mTotalDeathsColor, eFontAlign_Right);
		fY += 35;

		//Items
		mpGuiSet->DrawFont(kTranslate("RankScreen", "ItemsFoundByPlayer"), mpFontMessage, cVector3f(fX, fY, 10), mvMessageFontSize, mMessageFontColor, eFontAlign_Left);
		mpGuiSet->DrawFont(msPlayerItemAmount, mpFontMessage, cVector3f(fX2, fY, 10), mvMessageFontSize, mTotalItemsColor, eFontAlign_Right);

		fY += 35;

		mpGuiSet->DrawFont(kTranslate("RankScreen", "HealthUsed"), mpFontMessage, cVector3f(fX, fY, 10), mvMessageFontSize, mMessageFontColor, eFontAlign_Left);
		mpGuiSet->DrawFont(msHealthItemsUsed, mpFontMessage, cVector3f(fX2, fY, 10), mvMessageFontSize, mHealthItemsColor, eFontAlign_Right);
		fY += 35;

		mpGuiSet->DrawFont(kTranslate("RankScreen", "OilUsed"), mpFontMessage, cVector3f(fX, fY, 10), mvMessageFontSize, mMessageFontColor, eFontAlign_Left);
		mpGuiSet->DrawFont(msOilItemsUsed, mpFontMessage, cVector3f(fX2, fY, 10), mvMessageFontSize, mOilItemsColor, eFontAlign_Right);
		fY += 35;

		if (mlStatuesVal >= 1)
		{
			mpGuiSet->DrawFont(kTranslate("RankScreen", "StatuesFound"), mpFontMessage, cVector3f(fX, fY, 10), mvMessageFontSize, mMessageFontColor, eFontAlign_Left);
			mpGuiSet->DrawFont(msStatuesFound, mpFontMessage, cVector3f(fX2, fY, 10), mvMessageFontSize, mStatuesColor, eFontAlign_Right);
		}
	}

	//////////////////////////////////////////////
	// Fade
	if (mfFadeAlpha > 0)
	{
		mpGuiSet->DrawGfx(mpBlackGfx, mvGuiSetStartPos + cVector3f(0, 0, 20), mvGuiSetSize, cColor(1, mfFadeAlpha));
	}

	//gpBase->mpDebugHandler->AddMessage(cString::To16Char(msEndingReceived).c_str, false);
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
	Exit();
	return true;
}
kGuiCallbackDeclaredFuncEnd(cLuxRankScreen, ExitOnPressed);

bool cLuxRankScreen::UIExitOnPressed(iWidget* apWidget, const cGuiMessageData& aData)
{
	if (aData.mlVal == eUIButton_Primary)
	{
		
		Exit();
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
