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

#include "LuxPauseMessageButtonPrompt.h"

#include "LuxConfigHandler.h"
#include "LuxMapHandler.h"
#include "LuxInputHandler.h"
#include "LuxHelpFuncs.h"
#include "LuxMap.h"
#include "LuxMapHelper.h"
#include "LuxMapHandler.h"
#include "LuxEffectHandler.h"
#include "LuxGlobalDataHandler.h"
#include "LuxDebugHandler.h"

 //-----------------------------------------------------------------------

 //////////////////////////////////////////////////////////////////////////
 // CONSTRUCTORS
 //////////////////////////////////////////////////////////////////////////

 //-----------------------------------------------------------------------
cLuxPauseMessageButtonPrompt::cLuxPauseMessageButtonPrompt() : iLuxUpdateable("LuxPauseMessageButtonPrompt")
{
	mpGui = gpBase->mpEngine->GetGui();
	mpBlackGfx = mpGui->CreateGfxFilledRect(cColor(0, 1), eGuiMaterial_Alpha);

	mpFont = NULL;

	mvFontSize = gpBase->mpMenuCfg->GetVector2f("Messages", "GameMessageFontSize", 0);

	/// <summary>
	/// Pause Message
	/// </summary>

	mpGraphics = gpBase->mpEngine->GetGraphics();

	cGuiSkin* mpSkin = mpGui->CreateSkin("gui_main_menu.skin");
	mpGuiSet = mpGui->CreateSet("PauseScreen", mpSkin);

	mvScreenSize = gpBase->mpEngine->GetGraphics()->GetLowLevel()->GetScreenSizeFloat();

	mvGuiSetCenterSize = cVector2f(800, 600);
	LuxCalcGuiSetScreenOffset(mvGuiSetCenterSize, mvGuiSetSize, mvGuiSetOffset);
	mvGuiSetStartPos = cVector3f(-mvGuiSetOffset.x, -mvGuiSetOffset.y, 0);


	mpGuiSet->SetVirtualSize(mvGuiSetSize, -1000, 1000, mvGuiSetOffset);
	mpGuiSet->SetSkin(mpGui->CreateSkin("gui_main_menu.skin"));

	mpGuiSet->SetDrawMouse(false);
	mpGuiSet->SetActive(false);
  
	mpViewport = gpBase->mpEngine->GetScene()->CreateViewport();
	mpViewport->SetActive(false);
	mpViewport->SetVisible(false);

	mpViewport->AddGuiSet(mpGuiSet);
	

	float fButtonWidth = 20.0f;
	float fButtonHeight = 30.0f;

	cVector2f mvButtonSize = cVector2f(fButtonWidth + 60, fButtonHeight + 5) + 10;

	mpButton1 = mpGuiSet->CreateWidgetButton(0, mvButtonSize, _W(""), NULL);
	mpButton2 = mpGuiSet->CreateWidgetButton(0, mvButtonSize, _W(""), NULL);

	mpButton1->AddCallback(eGuiMessage_UIButtonPress, this, kGuiCallback(UIExitOnPressed));
	mpButton1->AddCallback(eGuiMessage_ButtonPressed, this, kGuiCallback(ExitOnPressed));
	mpButton1->SetGlobalUIInputListener(true);
	mpButton1->SetVisible(false);

	mpButton2->AddCallback(eGuiMessage_UIButtonPress, this, kGuiCallback(UIExitOnPressed));
	mpButton2->AddCallback(eGuiMessage_ButtonPressed, this, kGuiCallback(ExitOnPressed));
	//mpButton2->SetGlobalUIInputListener(true);
	mpButton2->SetVisible(false);

	//mpGuiSet->SetFocusedWidget(mpButton1);

	mpScreenGfx = NULL;
	mpScreenTexture = NULL;
	mpScreenBgTexture = NULL;
	mpScreenBgGfx = NULL;

	//mfPauseMessageAlpha = 0;
	mbExitingPauseFromPress = false;

	cParserVarContainer programVars;
	mpEffectProgram = mpGraphics->CreateGpuProgramFromShaders("InventoryEffect", "inventory_screen_effect_vtx.glsl", "inventory_screen_effect_frag.glsl", &programVars);
}

//-----------------------------------------------------------------------

cLuxPauseMessageButtonPrompt::~cLuxPauseMessageButtonPrompt()
{
}

//-----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

void cLuxPauseMessageButtonPrompt::LoadFonts()
{
	tString sFontFile = "game_default.fnt";
	mpFont = LoadFont(sFontFile);
}

void cLuxPauseMessageButtonPrompt::OnStart()
{

}

//-----------------------------------------------------------------------

void cLuxPauseMessageButtonPrompt::Reset()
{
	mbPauseMessageActive = false;
	mfPauseMessageAlpha = 0;

	mvLines.clear();
	msCallback = "";
	mbPressedYes = false;
	mlButtonPressed = 0;
	mbMessageYesNo = false;
	mbExitingPauseFromPress = false;
}

//-----------------------------------------------------------------------

void cLuxPauseMessageButtonPrompt::OnMapEnter(cLuxMap* apMap)
{
	Reset();
}

void cLuxPauseMessageButtonPrompt::OnMapLeave(cLuxMap* apMap)
{

}

//-----------------------------------------------------------------------

void cLuxPauseMessageButtonPrompt::StartPauseMessage(const tWString& asText, bool abYesNo, const tWString& asButton1Text, const tWString& asButton2Text, string asCallback)
{
	gpBase->mpDebugHandler->AddMessage(_W("Start Pause Message!"), true);

	mbMessageYesNo = abYesNo;
	msCallback = asCallback;

	mvLines.clear();

	mpFont->GetWordWrapRows(500, mvFontSize.y + 2, mvFontSize, gpBase->mpHelpFuncs->ParseString(asText), &mvLines);

	tWStringVec vLabels;
	vLabels.push_back(asButton1Text);
	vLabels.push_back(asButton2Text);

	float fButtonWidth = 0;
	cWidgetButton* pButton = mpGuiSet->CreateWidgetButton(0, cVector2f(fButtonWidth, 30), _W(""), NULL);


	//////////////////////////////////////////////////////////////
	// Get longest label string and set button width accordingly
	for (int i = 0; i < (int)vLabels.size(); ++i)
	{
		const tWString& sLabel = vLabels[i];
		float fLabelLength = pButton->GetDefaultFontType()->GetLength(pButton->GetDefaultFontSize(),
			sLabel.c_str());

		if (fButtonWidth < fLabelLength)
			fButtonWidth = fLabelLength;
	}

	pButton->SetVisible(false);
	pButton->SetEnabled(false);

	fButtonWidth += 20.0f;
	float fButtonHeight = 30.0f;

	mpButton1->SetSize(cVector2f(fButtonWidth + 60, fButtonHeight) + 10);
	mpButton2->SetSize(cVector2f(fButtonWidth + 60, fButtonHeight) + 10);
	mpButton1->SetText(vLabels[0]);
	mpButton2->SetText(vLabels[1]);


	//gpBase->mpEffectHandler->GetSepiaColor()->FadeTo(1, 1);


	SetPauseMessageActive(true);

}
//-----------------------------------------------------------------------

void cLuxPauseMessageButtonPrompt::Update(float afTimeStep)
{
	////////////////////////////////
	// Pause message
	if (mbPauseMessageActive)
	{
		mfPauseMessageAlpha += afTimeStep * 2.0f;
		if (mfPauseMessageAlpha > 1)
		{
			mfPauseMessageAlpha = 1;
			gpBase->mpDebugHandler->AddMessage(_W("Enter Container Should Start Now!"), true);
			//gpBase->mpEngine->GetUpdater()->SetContainer("PauseScreen");

		}
	}
	else
	{
		mfPauseMessageAlpha -= afTimeStep * 2.0f;
		if (mfPauseMessageAlpha < 0)
		{
			mfPauseMessageAlpha = 0;

			if (mbExitingPauseFromPress && msCallback != "")
			{
				gpBase->mpMapHandler->GetCurrentMap()->RunScript(msCallback + "(" +cString::ToString(mlButtonPressed)+ ")");
				gpBase->mpDebugHandler->AddMessage(_W("Script should be called!"), true);
				mbExitingPauseFromPress = false;
			}

			gpBase->mpInputHandler->ChangeState(eLuxInputState_Game);
			gpBase->mpEngine->GetUpdater()->SetContainer("Default");

		}
	}
}

void cLuxPauseMessageButtonPrompt::OnDraw(float afFrameTime)
{
	DrawPauseMessage();
}

//-----------------------------------------------------------------------

void cLuxPauseMessageButtonPrompt::OnPress(bool abYes)
{
	mbPressedYes = abYes;
	mbExitingPauseFromPress = true;
	mlButtonPressed; 

}

void cLuxPauseMessageButtonPrompt::OnPress(int alButton)
{
	mbExitingPauseFromPress = true;
	mlButtonPressed = alButton;

}

//-----------------------------------------------------------------------
void cLuxPauseMessageButtonPrompt::OnEnterContainer(const tString& asOldContainer)
{
	CreateBackground();

	//gpBase->mpDebugHandler->AddMessage(_W("Enter Container!"), false);

	////////////////////////////
	//Set up states and viewport
	gpBase->mpInputHandler->ChangeState(eLuxInputState_PauseMessage);

	mpViewport->SetActive(true);
	mpViewport->SetVisible(true);

#ifdef USE_GAMEPAD
	if (gpBase->mpInputHandler->IsGamepadPresent() == false)
	{
		mpGuiSet->SetDrawMouse(true);
		mpGuiSet->SetMouseMovementEnabled(true);
	}
	else if (gpBase->mpInputHandler->IsGamepadPresent())
	{
		mpGuiSet->SetDrawMouse(false);
		mpGuiSet->SetMouseMovementEnabled(false);
	}
#else
	mpGuiSet->SetDrawMouse(true);
#endif

	mpGuiSet->ResetMouseOver();
	mpGuiSet->SetActive(true);
	mpGui->SetFocus(mpGuiSet);

	////////////////////////
	// Text
	cVector2f vInitTextPos = cVector2f(0.8, 0.325);
	vInitTextPos *= mvScreenSize;

	cVector3f vTextPos = cVector3f(400, 265, 10);

	vTextPos.y += 50.0f;
	////////////////////////
	// Yes / No
	if (mbMessageYesNo)
	{

		vTextPos.x = 200;

		mpButton1->SetVisible(true);
		mpButton1->SetEnabled(true);
		mpButton1->SetPosition(vTextPos);

		vTextPos.x += 300;

		mpButton2->SetVisible(true);
		mpButton2->SetEnabled(true);
		mpButton2->SetPosition(vTextPos);

	}
	else
	{
		mpButton1->SetVisible(true);
		mpButton1->SetEnabled(true);
		mpButton1->SetPosition(cVector3f(325, vTextPos.y, 10));
	}

	//mfPauseMessageAlpha = 1;

	cSoundHandler* pSoundHandler = gpBase->mpEngine->GetSound()->GetSoundHandler();
	pSoundHandler->FadeGlobalVolume(0.5f, 0.5f, eSoundEntryType_World, eLuxGlobalVolumeType_GameMenu, false);
}

void cLuxPauseMessageButtonPrompt::OnLeaveContainer(const tString& asNewContainer)
{
	cSoundHandler* pSoundHandler = gpBase->mpEngine->GetSound()->GetSoundHandler();
	pSoundHandler->FadeGlobalVolume(1.0f, 0.5f, eSoundEntryType_World, eLuxGlobalVolumeType_GameMenu, false);

	mpViewport->SetActive(false);
	mpViewport->SetVisible(false);

	mpGuiSet->SetActive(false);

	//gpBase->mpDebugHandler->AddMessage(_W("Leave Container!"), false);

	mbExitingPauseFromPress = false;

	DestroyBackground();
}
//-----------------------------------------------------------------------

void cLuxPauseMessageButtonPrompt::SetPauseMessageActive(bool abX)
{
	if (mbPauseMessageActive == abX) return;

	mbPauseMessageActive = abX;

	if (abX == true)
	{
		gpBase->mpEngine->GetUpdater()->SetContainer("PauseScreen");
	}


	//gpBase->mpMapHandler->SetUpdateActive(mbPauseMessageActive? false : true);
}

//-----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// PRIVATE METHODS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

void cLuxPauseMessageButtonPrompt::DrawPauseMessage()
{
	if (mfPauseMessageAlpha <= 0) return;

	////////////////////////
	//Draw background
	if (mpScreenGfx && mfPauseMessageAlpha < 1)
		mpGuiSet->DrawGfx(mpScreenGfx, mvGuiSetStartPos + cVector3f(0, 0, 0), mvGuiSetSize);

	if (mpScreenBgGfx)
		mpGuiSet->DrawGfx(mpScreenBgGfx, mvGuiSetStartPos + cVector3f(0, 0, 0.2f), mvGuiSetSize, cColor(1, mfPauseMessageAlpha));

	////////////////////////
	// Black background
	//mpPauseGuiSet->DrawGfx(mpBlackGfx,cVector3f(0,0,3),mvScreenSize,cColor(1, mfPauseMessageAlpha*0.5f));

	////////////////////////
	// Text
	cVector2f vInitTextPos = cVector2f(0.8, 0.325);
	vInitTextPos *= mvScreenSize;

	cVector3f vTextPos = cVector3f(400, 200, 10); //cVector3f(vInitTextPos.x, vInitTextPos.y, 2); cVector3f(400, 345, 4);


	for (size_t i = 0; i < mvLines.size(); ++i)
	{
		mpGuiSet->DrawFont(mvLines[i], mpFont, vTextPos, mvFontSize, cColor(1, mfPauseMessageAlpha), eFontAlign_Center);

		vTextPos.y += mvFontSize.y + 2;
	}
}
//-----------------------------------------------------------------------
void cLuxPauseMessageButtonPrompt::ExitPause()
{
	//mbExiting = true;
	mpGui->SetFocus(NULL);
	gpBase->mpInputHandler->ChangeState(eLuxInputState_Null);
	SetPauseMessageActive(false);

	mpGuiSet->SetDrawMouse(false);

	mpButton1->SetVisible(false);
	mpButton1->SetEnabled(false);

	mpButton2->SetVisible(false);
	mpButton2->SetEnabled(false);

}
//-----------------------------------------------------------------------

void cLuxPauseMessageButtonPrompt::CreateBackground()
{
	CreateScreenTextures();
	RenderBackgroundImage();
}

//-----------------------------------------------------------------------

void cLuxPauseMessageButtonPrompt::DestroyBackground()
{
	if (mpScreenGfx) mpGui->DestroyGfx(mpScreenGfx);
	if (mpScreenTexture) mpGraphics->DestroyTexture(mpScreenTexture);
	if (mpScreenBgGfx) mpGui->DestroyGfx(mpScreenBgGfx);
	if (mpScreenBgTexture) mpGraphics->DestroyTexture(mpScreenBgTexture);

	mpScreenGfx = NULL;
	mpScreenTexture = NULL;
	mpScreenBgTexture = NULL;
	mpScreenBgGfx = NULL;
}

//-----------------------------------------------------------------------

void cLuxPauseMessageButtonPrompt::CreateScreenTextures()
{
	iLowLevelGraphics* pLowGfx = mpGraphics->GetLowLevel();
	cVector3l vTexSize = pLowGfx->GetScreenSizeInt();
	vTexSize.z = 0;

	mpScreenTexture = mpGraphics->CreateTexture("Screen", eTextureType_Rect, eTextureUsage_RenderTarget);
	mpScreenTexture->CreateFromRawData(vTexSize, ePixelFormat_RGBA, NULL);
	mpScreenTexture->SetWrapSTR(eTextureWrap_ClampToEdge);

	mpScreenBgTexture = mpGraphics->CreateTexture("ScreenBlur", eTextureType_Rect, eTextureUsage_RenderTarget);
	mpScreenBgTexture->CreateFromRawData(vTexSize, ePixelFormat_RGBA, NULL);

	mpScreenGfx = mpGui->CreateGfxTexture(mpScreenTexture, false, eGuiMaterial_Diffuse);
	mpScreenBgGfx = mpGui->CreateGfxTexture(mpScreenBgTexture, false, eGuiMaterial_Alpha);
}

//-----------------------------------------------------------------------

void cLuxPauseMessageButtonPrompt::RenderBackgroundImage()
{
	iLowLevelGraphics* pLowGfx = mpGraphics->GetLowLevel();

	//////////////////////////////
	// Create frame buffers
	iFrameBuffer* pEffectBuffer = mpGraphics->CreateFrameBuffer("InventoryEffectbuffer");
	pEffectBuffer->SetTexture2D(0, mpScreenBgTexture);
	pEffectBuffer->CompileAndValidate();

	//////////////////////////////
	// Render

	//Render scene again without gui.
	gpBase->mpHelpFuncs->RenderBackgroundScreen(false);

	//Set up main states
	pLowGfx->SetBlendActive(false);
	pLowGfx->SetDepthTestActive(false);
	pLowGfx->SetDepthWriteActive(false);

	pLowGfx->SetOrthoProjection(mvScreenSize, -1000, 1000);
	pLowGfx->SetIdentityMatrix(eMatrix_ModelView);

	//Copy screen to screen texture
	pLowGfx->CopyFrameBufferToTexure(mpScreenTexture, 0, pLowGfx->GetScreenSizeInt(), 0);

	//Bind shader and draw
	mpEffectProgram->Bind();
	pLowGfx->SetCurrentFrameBuffer(pEffectBuffer);

	pLowGfx->SetTexture(0, mpScreenTexture);

	pLowGfx->DrawQuad(0, mvScreenSize, cVector2f(0, mvScreenSize.y), cVector2f(mvScreenSize.x, 0), cColor(1, 1));
	mpEffectProgram->UnBind();

	//Copy a copy of the full gui with all HUD!
	pLowGfx->SetCurrentFrameBuffer(NULL);
	pLowGfx->SetTexture(0, NULL);

	gpBase->mpHelpFuncs->RenderBackgroundScreen(true);
	pLowGfx->CopyFrameBufferToTexure(mpScreenTexture, 0, pLowGfx->GetScreenSizeInt(), 0);


	///////////////////////
	// Exit

	//Render states
	pLowGfx->SetTexture(0, NULL);
	pLowGfx->SetCurrentFrameBuffer(NULL);
	pLowGfx->SetDepthTestActive(true);

	//Flush the rendering
	pLowGfx->FlushRendering();
	pLowGfx->WaitAndFinishRendering();

	//Destroy data
	mpGraphics->DestroyFrameBuffer(pEffectBuffer);
}
//-----------------------------------------------------------------------

bool cLuxPauseMessageButtonPrompt::ExitOnPressed(iWidget* apWidget, const cGuiMessageData& aData)
{
	if (mbMessageYesNo)
	{
		if (apWidget == mpButton1)
		{
			//OnPress(true);
			OnPress(1);
		}
		else
		{
			//OnPress(false);
			OnPress(2);
		}
	}
	ExitPause();
	return true;
}
kGuiCallbackDeclaredFuncEnd(cLuxPauseMessageButtonPrompt, ExitOnPressed);

bool cLuxPauseMessageButtonPrompt::UIExitOnPressed(iWidget* apWidget, const cGuiMessageData& aData)
{
	if (aData.mlVal == eUIButton_Primary)
	{
		if (mbMessageYesNo)
		{
			if (apWidget == mpButton1)
			{
				//OnPress(true);
				OnPress(1);
			}
			else
			{
				//OnPress(false);
				OnPress(2);
			}
		}
		ExitPause();
		return true;

	}
	return false;
}
kGuiCallbackDeclaredFuncEnd(cLuxPauseMessageButtonPrompt, UIExitOnPressed);

//-----------------------------------------------------------------------

bool cLuxPauseMessageButtonPrompt::ButtonLabelOnUpdate(iWidget* apWidget, const cGuiMessageData& aData)
{
	bool bMouseOver = apWidget->GetUserValue() == 1;
	cColor mButtonFontColor = cColor(1, 1);
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
kGuiCallbackDeclaredFuncEnd(cLuxPauseMessageButtonPrompt, ButtonLabelOnUpdate);

//-----------------------------------------------------------------------

bool cLuxPauseMessageButtonPrompt::ButtonLabelOnDraw(iWidget* apWidget, const cGuiMessageData& aData)
{
	return true;
}
kGuiCallbackDeclaredFuncEnd(cLuxPauseMessageButtonPrompt, ButtonLabelOnDraw);

//-----------------------------------------------------------------------

bool cLuxPauseMessageButtonPrompt::ButtonLabelOnMouseEnter(iWidget* apWidget, const cGuiMessageData& aData)
{
	apWidget->SetUserValue(1);
	return true;
}
kGuiCallbackDeclaredFuncEnd(cLuxPauseMessageButtonPrompt, ButtonLabelOnMouseEnter);

//-----------------------------------------------------------------------

bool cLuxPauseMessageButtonPrompt::ButtonLabelOnMouseLeave(iWidget* apWidget, const cGuiMessageData& aData)
{
	apWidget->SetUserValue(0);
	return true;
}
kGuiCallbackDeclaredFuncEnd(cLuxPauseMessageButtonPrompt, ButtonLabelOnMouseLeave);

//-----------------------------------------------------------------------
