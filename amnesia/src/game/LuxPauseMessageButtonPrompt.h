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


 //----------------------------------------------

#include "LuxBase.h"

 //----------------------------------------------
class cLuxPauseMessageButtonPrompt : public iLuxUpdateable
{
friend class cLuxMusicHandler_SaveData;
public:
	cLuxPauseMessageButtonPrompt();
	~cLuxPauseMessageButtonPrompt();

	void LoadFonts();
	void OnStart();
	void Update(float afTimeStep);
	void Reset();


	void OnEnterContainer(const tString& asOldContainer);
	void OnLeaveContainer(const tString& asNewContainer);

	void OnMapEnter(cLuxMap* apMap);
	void OnMapLeave(cLuxMap* apMap);

	void StartPauseMessage(const tWString& asText, bool abYesNo, const tWString& asButton1Text, const tWString& asButton2Text, string asCallback);

	void OnDraw(float afFrameTime);

	bool IsPauseMessageActive() { return mbPauseMessageActive; }
	void SetPauseMessageActive(bool abX);
	void OnPress(bool abYes);

	cGuiSet* GetSet() { return mpGuiSet; }
	
	void ExitPause();

private:
	void CreateBackground();
	void RenderBackgroundImage();
	void CreateScreenTextures();
	void DestroyBackground();
	void DrawPauseMessage();

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

	//////////////////
	// Data
	cGuiGfxElement* mpBlackGfx; 
	iFontData* mpFont;

	cGuiGfxElement* mpQuestAddedIcon;
	tString msQuestAddedSound;

	cVector2f mvFontSize;

	//////////////////
	// Variables
	bool mbPauseMessageActive;
	float mfPauseMessageAlpha;
	bool mbPressedYes;
	bool mbExitingPauseFromPress;

	string msCallback;

	tWStringVec mvMessageRows;

	tWStringVec mvLines;
	bool mbMessageYesNo;

	cVector2f mvScreenSize;

	cVector2f mvGuiSetSize;
	cVector2f mvGuiSetCenterSize;
	cVector2f mvGuiSetOffset;
	cVector3f mvGuiSetStartPos;

	iTexture* mpScreenTexture;
	cGuiGfxElement* mpScreenGfx;
	iTexture* mpScreenBgTexture;
	cGuiGfxElement* mpScreenBgGfx;
	iGpuProgram* mpEffectProgram;

	cGraphics* mpGraphics;
	cGui* mpGui;
	cGuiSet* mpGuiSet;
	cViewport* mpViewport;

	cWidgetButton* mpButton1;
	cWidgetButton* mpButton2;
};


