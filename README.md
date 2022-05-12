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
void StartCreditsAndRankScreen(string& asMusicCredits, bool abLoopMusicCredits, string& asTextCat, string& asTextEntry, bool abCreditsBackground, string& asMusicRank, bool abLoopMusicRank, string& asImageName);
```
Starts the credits as well as the rank screen at the end of it.
1. *asMusicCredits* -  The music to play during the credits
2. *abLoopMusicCredits* -  Whether the credits music should loop
3. *asTextCat* -  The category to be used in the .lang file for the credits
4. *asTextEntry* -  The entry in the .lang file for the credits
5. *abCreditsBackground* -  Should the credits background should be displayed
6. *asMusicRank* -  The music to play during the rank screen
7. *abLoopMusicRank* -  Whether the rank screen music should loop
7. *asImageName* -  What image to display during the rank screen.
---------------------------------------
```as
void StartRankScreen();
```
Starts the rank screen. 
```as
SetEnding(string& asEnding, bool abCompletedOnHardMode);
```
Sets the ending to be writtem as completed in the user_endings.cfg in the player's save folder and if they completed it on Hard Mode or not.
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

## Screen Effects:
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
## Particles:
```as
void   SetParticleSystemActive(string& asName, bool abActive);
```
Pauses a particle system in its current frame. The paused particle system remains frozen at this frame until reactivated or destroyed.
1. *asParticleSystem* - The name of the particle system
2. *abActive* - False to pause, true to unpause
---------------------------------------
```as
void    DestroyParticleSystemInstantly(string& asName);
```
Destroys a particle system and any existing particles already emitted from it. Similar to DestroyParticleSystem, except that one will not destroy the existing particles and rather let them live out their lives. This function will cut all particles' lives short.
1. *asParticleSystem* - The name of the particle system
---------------------------------------
