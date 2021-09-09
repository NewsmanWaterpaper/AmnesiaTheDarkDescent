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

Modding Documentation:
-------------------

## Enemies:

```as
void  SetEnemyBlind(string& asName, bool abX){}
```
Sets whether the enemy can see the player if they are within visible range.
1. * asName* - Internal name of the enemy
2. *abX* - Whether enemy is blind
---------------------------------------
```as
void  SetEnemyDeaf(string& asName, bool abX){}
```
Sets whether the enemy can hear the player make sound if they are within audible range.
1. * asName* - Internal name of the enemy
2. *abX* - Whether enemy is deaf
---------------------------------------
```as
void  ChangeEnemyState(string& asName, string& asState){}
```
Changes the AI state of a specified enemy.
1. * asName* - Internal name of the enemy
2. *asState* - desired state, can be: Patrol, Investigate Idle, Wait, Alert, Hunt, HuntPause, HuntWander and Hurt
---------------------------------------
```as
void ForceEnemyWaitState(string& asName){}
```
Forces the enemy's AI to change the state to "Wait" which makes the enemy wait for a short while before continuing its' normal actions. An enemy without patrol nodes defaults to the "Wait" state. Otherwise, if patrol nodes are added, the enemy will continue the path after waiting is done.
1. * asName* - Internal name of the enemy

---------------------------------------
```as
float GetEnemyPlayerDistance(string& asEnemyName){}
```
Returns the distance (in HPL units) between the enemy and the player.
1. * asEnemyName* - Internal name of the enemy

---------------------------------------
```as
bool GetEnemyPlayerDistance(string& asEnemyName){}
```
Returns whether the enemy is within visible range of the player.
1. * asEnemyName* - Internal name of the enemy

---------------------------------------
```as
void  ChangeEnemyPatrolSpeed(string& asName, string& asSpeedType){}
```
Changes the patrol speed of a specified enemy.
1. * asName* - Internal name of the enemy
2. *asSpeedType* - Name of the Patrol Speed to use. Can be "Run" or "Walk"
---------------------------------------
```as
void  SetEnemyRunSpeedMul(string& asName, float afRunSpeedMul){}
float  GetEnemyRunSpeedMul(string& asName){}
```
Modifies/returns how much an enemy's run speed is multiplied by.
1. * asName* - Internal name of the enemy
2. *afRunSpeedMul* - The amount to multiply the run speed with
---------------------------------------
```as
void  SetEnemyHealth(string& asName, float afHealth){}
float  GetEnemyHealth(string& asName){}
```
Modifies/returns the amount of health for a specific enemy.
1. * asName* - Internal name of the enemy
2. *afHealth* - The amount of health to change to
---------------------------------------
```as
void   SetEnemyRegenHealthSpeed(string& asName, float afRegenHealthSpeed){}
float  GetEnemyRegenHealthSpeed(string& asName){}
```
Modifies/returns how quickly a specified enemy can regenerate health back.
1. * asName* - Internal name of the enemy
2. *afRegenHealthSpeed* - Speed multiplier of how quickly health is regenerated 
---------------------------------------
```as
void  SetManPigIdleBehavior(string& asName, string& asBehaviorType){}
```
Changes the Idle Behaviour type of a specified Manpig enemy.
1. * asName* - Internal name of the enemy
2. *asBehaviorType* - Name of the Behavior Type to use. Can be "None", "Stalk" or "Track"
---------------------------------------
```as
void  SetManPigToFlee(string& asName, bool abX){}
```
Sets if a specific Manpig should flee from the player if they get close.
1. * asName* - Internal name of the enemy
2. *abX* - Enabled/disabled
---------------------------------------
```as
void  SetManPigToThreatenOnAlert(string& asName, bool abX){}
```
Sets if a specific Manpig should perform it's screaming animation upon spotting the player and entering it's Alert state.
1. * asName* - Internal name of the enemy
2. *abX* - Enabled/disabled
---------------------------------------
```as
void  SetManPigCanTrackTeleport(string& asName, bool abX){}
```
Sets if a specific Manpig is allowed to teleport around whilst in it's "Track" behavior type/state.
1. * asName* - Internal name of the enemy
2. *abX* - Enabled/disabled
---------------------------------------
```as
void  SetManPigFleeHealth(string& asName, float afFleeHealth){}
float  GetManPigFleeHealth(string& asName){}
```
Modifies/returns the amount of health a specific Manpig has to be under to enter it's "Flee" state.
1. * asName* - Internal name of the enemy
2. *afFleeHealth* - The amount of health where the enemy should start fleeing from the player
---------------------------------------