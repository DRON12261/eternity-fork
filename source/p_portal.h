// Emacs style mode select   -*- C++ -*- 
//-----------------------------------------------------------------------------
//
// Copyright(C) 2013 Stephen McGranahan et al.
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
//--------------------------------------------------------------------------
//
// DESCRIPTION:
//      Linked portals
//      SoM created 02/13/06
//
//-----------------------------------------------------------------------------

#ifndef P_PORTAL_H__
#define P_PORTAL_H__

#include "m_vector.h"
#include "r_defs.h"

struct polyobj_s;

extern bool useportalgroups;

// ioanch 20160109: true if sector portals are in map
extern bool gMapHasSectorPortals;
extern bool gMapHasLinePortals;  // ioanch 20160131: also check line portals
extern bool *gGroupVisit;  // ioanch 20160121: a global helper array
extern const polyobj_s **gGroupPolyobject; // ioanch 20160227

#ifndef R_NOGROUP
// No link group. I know this means there is a signed limit on portal groups but
// do you think anyone is going to make a level with 2147483647 groups that 
// doesn't have NUTS in the wad name? I didn't think so either.
#define R_NOGROUP -1
#endif

struct line_t;
struct linkdata_t;
struct linkoffset_t;
struct portal_t;
struct sector_t;

//
// Sector_SetPortal arguments
//
enum
{
   paramPortal_argType = 1,
   paramPortal_argPlane = 2,
   paramPortal_argMisc = 3,

   paramPortal_planeFloor = 0,
   paramPortal_planeCeiling = 1,
   paramPortal_planeBoth = 2,

   paramPortal_normal = 0,
   paramPortal_copied = 1,
   paramPortal_skybox = 2,
   paramPortal_pplane = 3,
   paramPortal_horizon = 4,
   paramPortal_copyline = 5,
   paramPortal_linked = 6,
};


//
// P_PortalGroupCount
//
int P_PortalGroupCount();

//
// P_CreatePortalGroup
// This creates an portal group, and returns the id of that group
//
int P_CreatePortalGroup(sector_t *from);

//
// P_GatherSectors
// This function gathers adjacent sectors, starting with the supplied sector,
// and adds them to the specified group.
//
void P_GatherSectors(sector_t *from, int groupid);

//
// R_BuildLinkTable
// Builds the link table. This should only be called after all the portals for
// the level have been created.
//
bool P_BuildLinkTable();

//
// P_LinkRejectTable
// Currently just clears each group for every other group.
//
void P_LinkRejectTable();

void P_InitPortals();

bool EV_PortalTeleport(Mobj *mo, fixed_t dx, fixed_t dy, fixed_t dz,
                       int fromid, int toid);
void P_LinePortalDidTeleport(Mobj *mo, fixed_t dx, fixed_t dy, fixed_t dz,
                             int fromid, int toid);

void R_SetSectorGroupID(sector_t *sector, int groupid);

//
// P_CheckCPortalState
// 
// Checks the state of the ceiling portal in the given sector and updates
// the state flags accordingly.
//
void P_CheckCPortalState(sector_t *sec);


// P_CheckFPortalState
// 
// Checks the state of the floor portal in the given sector and updates
// the state flags accordingly.
//
void P_CheckFPortalState(sector_t *sec);

//
// P_CheckLPortalState
// 
// Checks the state of the portal in the given line and updates
// the state flags accordingly.
//
void P_CheckLPortalState(line_t *line);

//
// P_SetFloorHeight
// This function will set the floor height, and update
// the float version of the floor height as well.
//
void P_SetFloorHeight(sector_t *sec, fixed_t h);

//
// P_SetCeilingHeight
// This function will set the ceiling height, and update
// the float version of the ceiling height as well.
//
void P_SetCeilingHeight(sector_t *sec, fixed_t h);

//
// P_SetPortalBehavior
//
// Sets the behavior flags for a portal. This function then iterates through all
// the sectors and lines in the currently loaded map and updates the state flags
// as such, this function shouldn't really be used a lot.
//
void P_SetPortalBehavior(portal_t *portal, int newbehavior);

//
// P_SetFPortalBehavior
//
// This function sets the behavior flags for the floor portal of a given sector
// and updates the state flags for the surface.
//
void P_SetFPortalBehavior(sector_t *sec, int newbehavior);

//
// P_SetCPortalBehavior
//
// This function sets the behavior flags for the ceiling portal of a given 
// sector and updates the state flags for the surface.
//
void P_SetCPortalBehavior(sector_t *sec, int newbehavior);

//
// P_SetLPortalBehavior
//
// This function sets the behavior flags for the portal of a given line
// and updates the state flags for the surface.
//
void P_SetLPortalBehavior(line_t *line, int newbehavior);

//
// P_LinePortalCrossing
//
// ioanch 20160106: function to trace one or more paths through potential line
// portals. Needed because some objects are spawned at given offsets from
// others, and there's no other way to detect line portal change.
//
v2fixed_t P_LinePortalCrossing(fixed_t x, fixed_t y, fixed_t dx, fixed_t dy,
                               int *group = nullptr, bool *passed = nullptr);

template <typename T> 
inline static v2fixed_t P_LinePortalCrossing(T &&u, fixed_t dx, fixed_t dy, 
                                             int *group = nullptr)
{
   return P_LinePortalCrossing(u.x, u.y, dx, dy, group);
}

template <typename T, typename U> 
inline static v2fixed_t P_LinePortalCrossing(T &&u, U &&dv, int *group = nullptr)
{
   return P_LinePortalCrossing(u.x, u.y, dv.x, dv.y, group);
}

//
// P_ExtremeSectorAtPoint
// ioanch 20160107
//
sector_t *P_ExtremeSectorAtPoint(fixed_t x, fixed_t y, bool ceiling, 
                                 sector_t *preCalcSector = nullptr);

inline static sector_t *P_ExtremeSectorAtPoint(const Mobj *mo, bool ceiling)
{
   return P_ExtremeSectorAtPoint(mo->x, mo->y, ceiling, mo->subsector->sector);
}

//
// P_TransPortalBlockWalker
// ioanch 20160107
//
bool P_TransPortalBlockWalker(const fixed_t bbox[4], int groupid, bool xfirst,
                              void *data, 
                              bool (*func)(int x, int y, int groupid, void *data));

// variant with generic callable
template <typename C> inline static
bool P_TransPortalBlockWalker(const fixed_t bbox[4], int groupid, bool xfirst,
                              C &&callable)
{
   return P_TransPortalBlockWalker(bbox, groupid, xfirst, &callable, 
      [](int x, int y, int groupid, void *data)
   {
      auto c = static_cast<C *>(data);
      return (*c)(x, y, groupid);
   });
}

//
//P_SectorTouchesThing
// ioanch 20160115
//
bool P_SectorTouchesThingVertically(const sector_t *sector, const Mobj *mobj);

// ioanch 20160222
sector_t *P_PointReachesGroupVertically(fixed_t cx, fixed_t cy, fixed_t cmidz,
                                        int cgroupid, int tgroupid,
                                        sector_t *csector, fixed_t midzhint);
inline static sector_t *P_ThingReachesGroupVertically(const Mobj *mo,
                                                      int groupid,
                                                      fixed_t midzhint)
{
   return P_PointReachesGroupVertically(mo->x, mo->y, mo->z + mo->height / 2,
      mo->groupid, groupid, mo->subsector->sector, midzhint);
}

void P_MoveLinkedPortal(portal_t *portal, fixed_t dx, fixed_t dy,
                        bool movebehind);

bool P_BlockHasLinkedPortalLines(int index);

#endif

// EOF

