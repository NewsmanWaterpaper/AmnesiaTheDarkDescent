Amnesia: The Dark Descent Source Code
=======================

Currently the engine uses fbx sdk 2012 which isn't avalable anymore which means the engine wont compile. If you want to give a shot anyway you can find the sdk here:
https://www.autodesk.com/fbx


Other than that, here is almost everything you need to build Amnesia: The Dark Descent. Included are project files for Visual Studio 2010 and CMake for Linux & macOS. 

Contributing Code
-----------------
We encourage everyone to contribute code to this project, so just sign up for a github account, create a fork and hack away at the codebase.

License Information
-------------------
All code is under the GPL Version 3 license. Read the LICENSE file for terms of use of the license.

Script Function Documentation:
-------------------
## Global & Local Vars:
Local variables can be used throughout the same script file.
```as
void SetLocalVarBool(string &in asName, bool abVal);
bool GetLocalVarBool(string& asName)
```

Global variables can be used throughout several maps and can be accessed by several script files.
```as
void SetGlobalVarBool(string &in asName, bool abVal);
bool GetGlobalVarBool(string& asName)
```

## Game:
```as
void SetProgLogTimerActive(bool abTimerActive);
```
Sets the ending to be in-game timer to be active.
1. *abTimerActive* -  Sets the in-game timer to be either on or off
---------------------------------------
```as
void DisplayCounterTimer(bool abDisplayTimer);
```
Should the in-game timer be displayed on screen.
1. *abDisplayTimer* -  Enables/disables in-game timer display
---------------------------------------
```as
void DisplayCounterSettings(float afX, float afY, float afZ, float afR, float afG, float afB, float afA, float afTimerSize, string& asTimerAlign);
```
Additional settings for the in-game timer display.
1. *afX* -  The X position of the timer
2. *afY* -  The Y position of the timer
3. *afZ* -  The Z position of the timer
4. *afR* -  The Red color value of the timer text
5. *afG* -  The Green color value of the timer text
6. *afB* -  The Blue color value of the timer text
7. *afTimerSize* -  How big should the timer text should be
8. *asTimerAlign* -  Which side of the screen should the timer should be displayed at; can  be either "Left","Right" or "Center".
---------------------------------------

```as
int GetCurrentInGameHour();
int GetCurrentInGameMin();
int GetCurrentInGameSec();
```
Returns either the in-game hour, minute or second.
```as
StartCreditsAndRankScreen(string& asMusicCredits, bool abLoopMusicCredits, string& asTextCat, string& asTextEntry, bool abCreditsBackground, string& asMusicRank, bool abLoopMusicRank, float afRankMusicVol, float afRankMusicFade, string& asImageName);
```
Starts the credits as well as the rank screen at the end of it.
1. *asMusicCredits* -  The music to play during the credits
2. *abLoopMusicCredits* -  Whether the credits music should loop
3. *asTextCat* -  The category to be used in the .lang file for the credits
4. *asTextEntry* -  The entry in the .lang file for the credits
5. *abCreditsBackground* -  Should the credits background should be displayed
6. *asMusicRank* -  The music to play during the rank screen
7. *abLoopMusicRank* -  Whether the rank screen music should loop
8. *afRankMusicVol* -  Sets the volume for rank screen music
9. *afRankMusicFade* -  Sets the time for rank screen music to fade in
10. *asImageName* -  What image to display during the rank screen.
---------------------------------------
```as
void StartRankScreen(string& asMusic, bool abLoopMusic, float afRankMusicVol, float afRankMusicFade, string& asImageName);
```
Starts the rank screen. 
1. *asMusic* -  The music to play during the rank scrren
2. *abLoopMusic* -  Whether the music should loop
3. *afRankMusicVol* -  Sets the volume for rank screen music
4. *afRankMusicFade* -  Sets the time for rank screen music to fade in
5. *asImageName* -  What image to display during the rank screen.
---------------------------------------
```as
SetEnding(string& asEnding, bool abCompletedOnHardMode);
```
Sets the ending to be written as completed in the user_endings.cfg in the player's save folder and if they completed it on Hard Mode or not.
1. *asEnding* - Which Ending type to use, can be either "DefaultGood", "DefaulBad", "GoodJoke" or "BadJoke".
2. *abCompletedOnHardMode* - If the game was completed on Hard Mode or not
---------------------------------------
```as
bool GetEndingCompletion(string& asEndingType);
```
Returns if the player got a certain ending.
```as
string& GetPreviousEnding();
```
Returns the most recent ending obtained. 
```as
int GetGameClears();
```
Returns the amount of times the player has beaten the game.
```as
DoHardModeSave();
```
Saves the game in Hard Mode. 
```as
bool HardModeEnabled();
```
Returns if the game is being played in Hard Mode or not. 
```as
void ForceExitDirectlyToMenu();
```
Forces the game to exit to the main menu. 
## Player:
```as
bool GetPlayerCrouching();
```
Returns if the player is crouching or not.
```as
void SetCustomTextMessage(string &in asTextCategory, string &in asTextEntry, float afX, float afY, float afZ, float afR, float afG, float afB, float afTime);
```
Extended version of the original SetMessage function, displays a message on the screen with custom positions and coloring.
1. *asTextCategory* - the category in the .lang file
2. *asTextEntry* - the entry in the .lang file
3. *afX* - X-axis position of the text on the screen
4. *afY* - Y-axis position of the text on the screen
5. *afZ* -  Z-axis position of the text on the screen
6. *afR* - Red color value of the text on the screen
7. *afG* - Green color value of the text on the screen
8. *afB* - Blue color value of the text on the screen
9. *afTime* - determines how long the message is displayed. If time is < =0 then the life time is calculated based on string length.
---------------------------------------
```as
void StartCustomPauseMessage(string &in asMainTextCategory, string &in asMainTextEntry, bool abMultipleButtons, string &in asButton1TextCat, string &in asButton1TextEnt, string &in asButton2TextCat, string &in asButton2TextEnt, string &in asCallback);
```
Starts a custom message prompt on the screen that pauses the game until a button(s) is pressed/selected.
1. *asMainTextCategory* - The category in the .lang file for the main text of the prompt
2. *asMainTextEntry* - The entry in the .lang file for the main text of the prompt
3. *abMultipleButtons* - If set to True - allows up to two buttons to be selected, handy for Yes/No-style prompts. If False - only one button displayed.
4. *asButton1TextCat* - The category in the .lang file for the text on Button 1 - REQUIRED if abMultipleButtons is set to either True or False.
5. *asButton1TextEnt* -  The entry in the .lang file for the text on Button 1 - REQUIRED if abMultipleButtons is set to either True or False.
6. *asButton2TextCat* - The category in the .lang file for the text on Button 2 - REQUIRED if abMultipleButtons is set to True.
7. *asButton2TextEnt* - The entry in the .lang file for the text on Button 2 - REQUIRED if abMultipleButtons is set to True.
8. *asCallback* - The function to call when one of the Buttons is pressed.
---------------------------------------

## Screen Effects:
```as
void StartCustomColorEffectFlash(float afR, float afG, float afB, float afA, float afFadeIn, float afFadeOut);
```
Fades the screen to a custom color.
1. *afR* - Red color value of the screen flash
2. *afG* - Green color value of the screen flash
3. *afB* - Blue color value of the screen flash
4. *afA* - Alpha value of the screen flash
5. *afFadeIn* -  time in seconds until the effect is started
6. *afFadeOut* - time in seconds until screen is back to normal again
---------------------------------------
```as
void ShowScreenImage(string &in asImageName, float afX, float afY, float afScale, bool abUseRelativeCoordinates, float afDuration, float afFadeIn, float afFadeOut);
```
Shows a 2D image on the screen
1. *asImageFile* - The image file to show. Does not have to be pow2 (although it gives a warning if not)
2. *afX* - The X position of the top left corner, starting from the center of the screen. Use negative half of the width of the image to center it. 
3. *afY* - The Y position of the top left corner, starting from the center of the screen.
4. *afScale* - The scale of the image file, use -1 to display the image with it's original size
5. *abUseRelativeCoordinates* - If the relative coordinates of the screen size should be use?
6. *afDuration* - How long, in seconds, the image displays. This does not include fade times
7. *afFadeIn* - How long, in seconds, the fade in spends
8. *afFadeOut* - How long, in seconds, the fade out spends
---------------------------------------
```as
void HideScreenImageImmediately();
```
Instantly hides an image being shown from ShowScreenImage.

```as
void HideScreenImageWithFade(float afFadeOut);
```
Simliar to HideScreenImageImmediately but allows you to fade out the image instead.
1. *afFadeOut* - How long, in seconds that it takes the image to fade out
---------------------------------------

## Entities:
```as
void  AttachAreaToProp(string& asAreaName, string& asProp, int alBody);
```
Attaches an area to a prop.
1. *asAreaName* - The area to attach
2. *asProp* - The prop to attach area to
3. *alBody* - Body of the prop to attach the area to
---------------------------------------
```as
float GetEntitiesDistance(string& asEntityA, string& asEntityB);
```
Returns the distance between two entities.
1. *asEntityA* - The name of the first entity
2. *asEntityB* - The name of the second entity
---------------------------------------
```as
float GetEntityRotationX(string &in asName, int body);
float GetEntityRotationY(string &in asName, int body);
float GetEntityRotationZ(string &in asName, int body);
```
Returns the distance between two entities. Function by NB-Nutboi.
1. *asName* - Name of the entity to get the rotation from
2. *body* - The body of the entity to get the rotation from (if you're not sure, go with 0)
---------------------------------------
```as
void SetEntityRotation(string &in asName, float afrX, float afrY, float afrZ, int body);
```
It will set the world rotation for the entity body selected. Function by NB-Nutboi.
1. *asName* - name of the entity
2. *afrX* -  rotation in the X axis that should be set
3. *afrY* -  rotation in the Y axis that should be set
4. *afrZ* -  rotation in the Z axis that should be set
5. *afpX* - position in the X axis that should be set
6. *afpY* - position in the Y axis that should be set
7. *afpZ* - position in the Z axis that should be set
8. *body* - body of the entity that the rotation should be applied to (if you're not sure, go with 0)
---------------------------------------
```as
float GetBonePosX(string &in asEntity, string &in asBone);
float GetBonePosY(string &in asEntity, string &in asBone);
float GetBonePosZ(string &in asEntity, string &in asBone);
```
You can get the specific location of bones. Function by NB-Nutboi.
1. *asEntity* -  Name of the entity (must have a skeleton)
2. *asBone* - name of the bone in the entity (get with the ModelEditor)
---------------------------------------
```as
void  SetSwingDoorOpenAmount(string& asName, float afOpenAmount, float afDuration, bool abOpenTowardsMaxAngle);
```
Sets the open amount for a swing door.
1. *asName* - The name of the he SwingDoor entity
2. *afOpenAmount* - The new amount state to set. Range: 0 - 1
3. *afDuration* - The time in seconds until the door has changed state
4. *abOpenTowardsMaxAngle* - If the door should open turns the max angle
---------------------------------------
```as
void  StopPropAnimation(string& asProp);
```
Stops the animation currently playing on a prop. Animations can be started with PlayPropAnimation.
1. *asProp* - The name of the entity/prop
---------------------------------------
```as
void  SetPropAnimationSpeed(string& asProp, float afSpeed);
```
Sets the speed of an animation currently playing on a prop or entity.
1. *asProp* - The name of the entity/prop
2. *afSpeed* - The speed that the animation should go at, default is 1
---------------------------------------
```as
void  SetPropAnimationPosition(string& asProp, float afPos);
```
Jumps to a specifc point in an animation. Generally used in conjunction with PlayPropAnimation.
1. *asProp* - The entity that is being animated
2. *afPos* - The time within the animation, in seconds, to jump to
---------------------------------------
```as
void  PlayCurrentAnimation(string& asProp, float afFadeTime, bool abLoop);
```
Simliar to PlayPropAnimation but can be used to "unpause" an animation on a entity if StopPropAnimation or PauseCurrentAnimation has been called beforehand.
1. *asProp* - The entity that is being animated
2. *afFadeTime* - The transition time until the animation starts playing normally again
3. *abLoop* - If the animation should loop or not
---------------------------------------
```as
void  PauseCurrentAnimation(string& asProp, float afFadeTime);
```
Simliar to StopPropAnimation but allows for a "transition" effect before the animation fully stops.
1. *asProp* - The entity that is being animated
2. *afFadeTime* - The transition time until the animation is fully paused. 
---------------------------------------

## Enemies:

```as
void  SetEnemyBlind(string& asName, bool abX);
```
Sets whether the enemy can see the player if they are within visible range.
1. *asName* - Internal name of the enemy
2. *abX* - Whether enemy is blind
---------------------------------------
```as
void  SetEnemyDeaf(string& asName, bool abX);
```
Sets whether the enemy can hear the player make sound if they are within audible range.
1. *asName* - Internal name of the enemy
2. *abX* - Whether enemy is deaf
---------------------------------------
```as
void  SetEnemyEndOfPatrolCallback(string& asName, string& asFunc, bool abRemoveWhenCalled);
```
Sets the function to be called when an enemy reaches the end of it's patrol.
1. *asName* - Internal name of the enemy
2. *asFunc* - Function to be called, Callback syntax: void MyFunc()
3. *abRemoveWhenCalled*-  determines whether the callback should be removed when the enemy reaches the end of it's path. 
---------------------------------------
```as
void MakeEnemySearchPosition(string& asName, float afX, float afY, float afZ);
```
Gets an enemy to search a position without searching where the player exactly is. Make
sure that nodes are placed in the level!
1. *asName* - Internal name of the enemy
2. *afX* - X axis of the position
3. *afY*-  Y axis of the position
3. *afZ*-  Z axis of the position.
---------------------------------------
```as
void  ChangeEnemyState(string& asName, string& asState);
```
Changes the AI state of a specified enemy.
1. *asName* - Internal name of the enemy
2. *asState* - desired state, can be: Patrol, Investigate Idle, Wait, Alert, Hunt, HuntPause, HuntWander and Hurt
---------------------------------------
```as
void  GetEnemyPreviousState(string& asName);
```
Gets the last state that the enemy was in
1. *asName* - Internal name of the enemy
---------------------------------------
```as
void ForceEnemyWaitState(string& asName);
```
Forces the enemy's AI to change the state to "Wait" which makes the enemy wait for a short while before continuing its' normal actions. An enemy without patrol nodes defaults to the "Wait" state. Otherwise, if patrol nodes are added, the enemy will continue the path after waiting is done.
1. *asName* - Internal name of the enemy
---------------------------------------
```as
float GetEnemyPlayerDistance(string& asEnemyName);
```
Returns the distance (in HPL units) between the enemy and the player.
1. *asEnemyName* - Internal name of the enemy
---------------------------------------
```as
bool GetPlayerCanSeeEnemy(string& asEnemyName);
```
Returns whether the enemy is within visible range of the player.
1. *asEnemyName* - Internal name of the enemy
---------------------------------------
```as
void  PlayEnemyAnimation(string& asEnemyName, string& asAnimName, bool abLoop, float afFadeTime);
```
Plays a specific animation for an enemy.
1. *asEnemyName* - Internal name of the enemy  (asterisk is allowed)
2. *asAnimName* - The name of an animation registered to the enemy
3. *abLoop* - Whether the animation loops
4. *afFadeTime* - The transition time until the animation starts fully playing
---------------------------------------
```as
void  ChangeEnemyPatrolSpeed(string& asName, string& asSpeedType);
```
Changes the patrol speed of a specified enemy.
1. *asName* - Internal name of the enemy
2. *asSpeedType* - Name of the Patrol Speed to use. Can be "Run" or "Walk"
---------------------------------------
```as
void  SetEnemyRunSpeedMul(string& asName, float afRunSpeedMul);
float  GetEnemyRunSpeedMul(string& asName);
```
Modifies/returns how much an enemy's run speed is multiplied by.
1. *asName* - Internal name of the enemy
2. *afRunSpeedMul* - The amount to multiply the run speed with
---------------------------------------
```as
void  SetEnemySearchTime(string& asName, float afSearchTime);
float  GetEnemySearchTime(string& asName);
```
Modifies/returns  how much time an enemy spends in it's Search state.
1. *asName* - Internal name of the enemy
2. *afSearchTime* - The amount of time an enemy spends in it's Search state
---------------------------------------
```as
void  SetEnemyHealth(string& asName, float afHealth);
float  GetEnemyHealth(string& asName);
```
Modifies/returns the amount of health for a specific enemy.
1. *asName* - Internal name of the enemy
2. *afHealth* - The amount of health to change to
---------------------------------------
```as
void  SetEnemyHearVolume(string& asName, float afHearVolume);
float  GetEnemyHearVolume(string& asName);
```
Modifies/returns the amount of volume needed for an enemy to hear sounds.
1. *asName* - Internal name of the enemy
2. *afHearVolume* - The hearing volume to change to
---------------------------------------
```as
void   SetEnemyRegenHealthSpeed(string& asName, float afRegenHealthSpeed);
float  GetEnemyRegenHealthSpeed(string& asName);
```
Modifies/returns how quickly a specified enemy can regenerate health back.
1. *asName* - Internal name of the enemy
2. *afRegenHealthSpeed* - Speed multiplier of how quickly health is regenerated 
---------------------------------------
```as
void  SetManPigIdleBehavior(string& asName, string& asBehaviorType);
```
Changes the Idle Behaviour type of a specified Manpig enemy.
1. *asName* - Internal name of the enemy
2. *asBehaviorType* - Name of the Behavior Type to use. Can be "None", "Stalk" or "Track"
---------------------------------------
```as
void  SetManPigToFlee(string& asName, bool abX);
```
Sets if a specific Manpig should flee from the player if they get close.
1. *asName* - Internal name of the enemy
2. *abX* - Enabled/disabled
---------------------------------------
```as
void  SetManPigToThreatenOnAlert(string& asName, bool abX);
```
Sets if a specific Manpig should perform it's screaming animation upon spotting the player and entering it's Alert state.
1. *asName* - Internal name of the enemy
2. *abX* - Enabled/disabled
---------------------------------------
```as
void  SetManPigCanTrackTeleport(string& asName, bool abX);
```
Sets if a specific Manpig is allowed to teleport around whilst in it's "Track" behavior type/state.
1. *asName* - Internal name of the enemy
2. *abX* - Enabled/disabled
---------------------------------------
```as
void  SetManPigFleeHealth(string& asName, float afFleeHealth);
float  GetManPigFleeHealth(string& asName);
```
Modifies/returns the amount of health a specific Manpig has to be under to enter it's "Flee" state.
1. *asName* - Internal name of the enemy
2. *afFleeHealth* - The amount of health where the enemy should start fleeing from the player
---------------------------------------
```as
void  SetWraithStealthDashMode (string& asName, bool abX);
```
Sets if a specific Wraith is to be in Stealth Dash mode/state.
1. *asName* - Internal name of the enemy
2. *abX* - Enabled/disabled
---------------------------------------
```as
void  SetWraithStealthDashAtWill(string&in asName, bool abX);
```
Enables/disables if a Wraith is allowed to enter it's Stealth Dash mode/state automatically.
1. *asName* - Internal name of the enemy
2. *abX* -  Enabled/disabled
---------------------------------------
```as
void  SetWraithStealthDashModeLength(string& asName, int alNodes);
```
Sets how many nodes will a specific Wraith stay in it's Stealth Dash mode/state before exiting.
1. *asName* - Internal name of the enemy
2. *alNodes* - Amount of nodes (Default: 0)
---------------------------------------
```as
void  SetWraithStealthDashEnterDistance(string& asName, float afNodeDistance);
```
How far away/close does a path node have to be for a Wraith to enter it's Stealth Dash mode/state.
1. *asName* - Internal name of the enemy
2. *afNodeDistance* - The distance from the node (Default: 15.5)
---------------------------------------
```as
void  SetWraithStealthDashExitDistance(string& asName, float afNodeDistance);
```
How far away/close does a path node have to be for a Wraith to exit it's Stealth Dash mode/state.
1. *asName* - Internal name of the enemy
2. *afNodeDistance* - The distance from the node (Default: 5.0)
---------------------------------------
```as
void   SetWraithAttackType(string& asName, string& asAttackType);
```
Sets the Attack Type for a Wraith to use.
1. *asName* - Internal name of the enemy
2. *asAttackType* - Name of the Attack Type to use. Can be "Melee", "Archvile" or "HeatRay"
---------------------------------------
```as
void   SetWraithHeatRayDamageSpeed(string& asName, float afDamageSpeed);
```
Sets how faster should the Wraith's Heat-Ray attack type should drain the player's health.
1. *asName* - Internal name of the enemy
2. *afDamageSpeed* - The  speed of the damage drain (Default: 5.0). 
---------------------------------------
```as
void   SetWraithHeatRayDamageLightAmount(string& asName, float afLightAmount);
```
Sets the amount of Light from the Wraith's Heat-Ray spotlight should the player be in before recieving damage.
1. *asName* - Internal name of the enemy
2. *afLightAmount* - The amount of Light from (Default: 0.75)
---------------------------------------
```as
void    SetWraithBurnDamageMinTime(string& asName, float afMinTime);
void    SetWraithBurnDamageMaxTime(string& asName, float afMaxTime);
void    SetWraithBurnDamageTime(string& asName, float afMinTime, float afMaxTime)l
```
Sets the Min and Max Times of the player recieving Burn Damage from the Wraith's attack.
1. *asName* - Internal name of the enemy
2. *afMinTime* -  The minimum amount of time before Burn Damage can occur/be applied 
2. *afMaxTime* -  The maximum amount of time before Burn Damage can occur/be applied 
---------------------------------------
```as
void    SetWraithBurnMinDamage(string& asName, float afMinDamage);
void    SetWraithBurnMaxDamage(string& asName, float afMaxDamage);
void    SetWraithBurnDamage(string& asName, float afMinDamage, float afMaxDamage)
```
Sets the Min and Max amount of Burn Damage the player recieves from the Wraith's attack.
1. *asName* - Internal name of the enemy
2. *afMinDamage* -  The minimum amount of Burn Damage that the Player recieve
2. *afMaxDamage* -  The maximum amount of Burn Damage that the Player recieve
---------------------------------------
```as
void  SetWraithCanMeele (string& asName, bool abX);
```
Allows a specific Wraith to meele attack the Player if close enough.
1. *asName* - Internal name of the enemy
2. *abX* - Enabled/disabled
---------------------------------------
## Particles:
```as
void   SetParticleSystemActive(string& asName, bool abActive);
```
Sets a particle system visible or active. The paused particle system remains frozen at this frame until reactivated or destroyed.
1. *asParticleSystem* - The name of the particle system
2. *abActive* - False to invisible/inactive, true to visible/active
---------------------------------------
```as
void    DestroyParticleSystemInstantly(string& asName);
```
Destroys a particle system and any existing particles already emitted from it. Similar to DestroyParticleSystem, except that one will not destroy the existing particles and rather let them live out their lives. This function will cut all particles' lives short.
1. *asParticleSystem* - The name of the particle system
---------------------------------------
## Lights:
```as
void   FadeLightToExt(string& asLightName, float afR, float afG, float afB, float afA, float afRadius, float afBrightness, float afTime);
```
Changes the properties of a light. Includes a brightness option.
1. *asLightName* - Interal name
2. *afR* - red value
3. *afG* - green value
4. *afB* - blue value
5. *afA* - alpha value
6. *afRadius* - radius of the light. -1 means keeping the radius
7. *afBrightness* - brightness of the light
8. *afTime* time in seconds until change is done
---------------------------------------
```as
void   FadeLightBrightnessTo(string& asLightName, float afBrightness, float afTime);
```
Changes the brightness of a light.
1. *asLightName* - Interal name
2. *afBrightness* - brightness of the light
3. *afTime*  time in seconds until change is done
---------------------------------------
```as
float   GetLightColorR(string& asLightName);
float   GetLightColorG(string& asLightName);
float   GetLightColorB(string& asLightName);
float GetLightAlpha(string& asLightName);
float GetLightBrightness(string& asLightName);
```
Returns the Red, Green, Blue, Alpha and Brightness value of a Light.

## Fog Area:
```as
void   SetFogAreaVisible(string& asFogAreaName, bool abVisible);
```
Sets a Fog Area visible or not.
1. *asFogAreaName* - The name of the Fog Area
2. *abVisible* - False to invisible, true to visible
---------------------------------------
