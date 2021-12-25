﻿//
// The Eternity Engine
// Copyright(C) 2021 James Haley, Max Waine, et al.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/
//
//----------------------------------------------------------------------------
//
// Purpose: MBF21 action functions
// Authors: Max Waine
//

#include "a_args.h"

//=============================================================================
//
// Actor codepointers
//

//
// Generic actor spawn function.
//
// args[0] -- type (dehnum) of actor to spawn
// args[1] -- angle (degrees), relative to calling actor's angle
// args[2] -- x (forward/back) spawn position offset
// args[3] -- y (left/right) spawn position offset
// args[4] -- z (up/down) spawn position offset
// args[5] -- x (forward/back) velocity
// args[6] -- y (left/right) velocity
// args[7] -- z (up/down) velocity
//
void A_SpawnObject(actionargs_t *actionargs)
{
   // TODO
}

//
// Generic monster projectile attack.
//
// args[0] -- type (dehnum) of actor to spawn
// args[1] -- angle (degrees), relative to calling actor's angle
// args[2] -- pitch (degrees), relative to calling actor's pitch
// args[3] -- horizontal spawn offset, relative to calling actor's angle
// args[4] -- vertical spawn offset, relative to actor's default projectile fire height
//
void A_MonsterProjectile(actionargs_t *actionargs)
{
   // TODO
}

//
// Generic monster bullet attack.
//
// args[0] -- horizontal spread (degrees, in fixed point)
// args[1] -- bertical spread (degrees, in fixed point)
// args[2] -- number of bullets to fire; if not set, defaults to 1
// args[3] -- mase damage of attack; if not set, defaults to 3
// args[4] -- attack damage random multiplier; if not set, defaults to 5
//
void A_MonsterBulletAttack(actionargs_t *actionargs)
{
   // TODO
}


//
// Generic monster melee attack.
//
// args[0] -- base damage of attack; if not set, defaults to 3
// args[1] -- attack damage random multiplier; if not set, defaults to 8
// args[2] -- sound to play if attack hits
// args[3] -- attack range; if not set, defaults to calling actor's melee range property
//
void A_MonsterMeleeAttack(actionargs_t *actionargs)
{
   // TODO
}

//
// Generic explosion.
//
// args[0] -- max explosion damge
// args[1] -- explosion radius, in map units
//
void A_RadiusDamage(actionargs_t *actionargs)
{
   // TODO
}

//
// Alerts monsters within sound-travel distance of the calling actor's target.
//
void A_NoiseAlert(actionargs_t *actionargs)
{
   // TODO
}

//
// Generic A_VileChase.
// Chases the player and resurrects any corpses with a Raise state when bumping into them.
//
// args[0] -- state to jump to on the calling actor when resurrecting a corpse
// args[1] -- sound to play when resurrecting a corpse
//
void A_HealChase(actionargs_t *actionargs)
{
   // TODO
}

//
// Generic seeker missile function..
//
// args[0] -- if angle to target is lower than this, missile will 'snap' directly to face the target
// args[1] -- maximum angle a missile will turn towards the target if angle is above the threshold
//
void A_SeekTracer(actionargs_t *actionargs)
{
   // TODO
}

//
// Searches for a valid tracer (seek target), if the calling actor doesn't already have one.
// Particularly useful for player missiles.
//
// args[0] -- field-of-view, relative to calling actor's angle, to search for targets in.
//            if zero, the search will occur in all directions
// args[1] -- distance to search, in map blocks (128 units); if not set, defaults to 10
//
void A_FindTracer(actionargs_t *actionargs)
{
   // TODO
}

//
// Clears the calling actor's tracer (seek target) field.
//
void A_ClearTracer(actionargs_t *actionargs)
{
   // TODO
}

//
// Jumps to a state if caller's health is below the specified threshold.
//
// args[0] -- state to jump to
// args[1] -- health to check
//
void A_JumpIfHealthBelow(actionargs_t *actionargs)
{
   // TODO
}

//
// Jumps to a state if caller's target is in line-of-sight.
//
// args[0] -- state to jump to
// args[1] -- field-of-view, relative to calling actor's angle, to check for target in.
//            if zero, the check will occur in all directions
//
void A_JumpIfTargetInSight(actionargs_t *actionargs)
{
   // TODO
}

//
// Jumps to a state if caller's target is closer than the specified distance.
//
// args[0] -- state to jump to
// args[1] -- distance threshold, in map units
//
void A_JumpIfTargetCloser(actionargs_t *actionargs)
{
   // TODO
}

//
// Jumps to a state if caller's tracer (seek target) is in line-of-sight.
//
// args[0] -- state to jump to
// args[1] -- field-of-view, relative to calling actor's angle, to check for target in.
//            if zero, the check will occur in all directions
//
void A_JumpIfTracerInSight(actionargs_t *actionargs)
{
   // TODO
}

//
// Jumps to a state if caller's tracer (seek target) is closer than the specified distance.
//
// args[0] -- state to jump to
// args[1] -- distance threshold, in map units
//
void A_JumpIfTracerCloser(actionargs_t *actionargs)
{
   // TODO
}

//
// Jumps to a state if caller has the specified thing flags set.
//
// args[0] -- state to jump to
// args[1] -- standard actor flag(s) to check
// args[2] -- MBF21 actor flag(s) to check
//
void A_JumpIfFlagsSet(actionargs_t *actionargs)
{
   // TODO
}

//
// Adds the specified thing flags to the caller.
//
// args[0] -- standard actor flag(s) to add
// args[1] -- MBF21 actor flag(s) to add
//
void A_AddFlags(actionargs_t *actionargs)
{
   // TODO
}

//
// Removes the specified thing flags from the caller.
//
// args[0] -- standard actor flag(s) to remove
// args[1] -- MBF21 actor flag(s) to remove
//
void A_RemoveFlags(actionargs_t *actionargs)
{
   // TODO
}

//=============================================================================
//
// Weapon codepointers
//

//
// Generic weapon projectile attack.
//
// args[0] -- type (dehnum) of actor to spawn
// args[1] -- angle (degrees), relative to player's angle
// args[2] -- pitch (degrees), relative to player's pitch
// args[3] -- horizontal spawn offset, relative to player's angle
// args[4] -- vertical spawn offset, relative to player's default projectile fire height
//
void A_WeaponProjectile(actionargs_t *actionargs)
{
   // TODO
}

//
// Generic weapon bullet attack.
//
// args[0] -- horizontal spread (degrees, in fixed point)
// args[1] -- vertical spread (degrees, in fixed point)
// args[2] -- number of bullets to fire; if not set, defaults to 1
// args[3] -- base damage of attack; if not set, defaults to 5
// args[4] -- attack damage random multiplier; if not set, defaults to 3
//
void A_WeaponBulletAttack(actionargs_t *actionargs)
{
   // TODO
}

//
// Generic weapon melee attack.
//
// args[0] -- base damage of attack; if not set, defaults to 2
// args[1] -- attack damage random multiplier; if not set, defaults to 10
// args[2] -- berserk damage multiplier; if not set, defaults to 1.0
// args[3] -- sound index to play if attack hits
// args[4] -- attack range; if not set, defaults to player mobj's melee range property
//
void A_WeaponMeleeAttack(actionargs_t *actionargs)
{
   // TODO
}

//
// Generic playsound for weapons.
//
// args[0] -- sound index to play
// args[1] -- if nonzero, play this sound at full volume across the entire map
// args[2] -- description
// args[3] -- description
// args[4] -- description
// args[5] -- description
// args[6] -- description
// args[7] -- description
//
void A_WeaponSound(actionargs_t *actionargs)
{
   // TODO
}

//
// Random state jump for weapons.
//
// args[0] -- state index to jump to
// args[1] -- chance out of 256 to perform the jump. 0 (or below) never jumps, 256 (or higher) always jumps
//
void A_WeaponJump(actionargs_t *actionargs)
{
   // TODO
}

//
// Subtracts ammo from the currently-selected weapon's ammo pool.
//
// args[0] -- amount of ammo to subtract. if zero, will default to the current weapon's ammopershot value
//
void A_ConsumeAmmo(actionargs_t *actionargs)
{
   // TODO
}

//
// Jumps to state if ammo is below amount.
//
// args[0] -- state index to jump to.
// args[1] -- amount of ammo to check. if zero, will default to the current weapon's ammopershot value
//
void A_CheckAmmo(actionargs_t *actionargs)
{
   // TODO
}

//
// Jumps to state if the fire button is currently being pressed and the weapon has enough ammo to fire.
//
// args[0] -- state index to jump to
// args[1] -- if nonzero, skip the ammo check
//
void A_RefireTo(actionargs_t *actionargs)
{
   // TODO
}

//
// Generic weapon muzzle flash.
//
// args[0] -- state index to set the flash psprite to
// args[1] -- if nonzero, do not change the 3rd-person player sprite to the player muzzleflash state
//
void A_GunFlashTo(actionargs_t *actionargs)
{
   // TODO
}

//
// Alerts monsters within sound-travel distance of the player's presence.
// Useful for weapons with the WPF_SILENT flag set.
//
void A_WeaponAlert(actionargs_t *actionargs)
{
   // TODO
}

// EOF

