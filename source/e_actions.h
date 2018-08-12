//
// Copyright (C) 2018 James Haley, Max Waine, et al.
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
// Additional terms and conditions compatible with the GPLv3 apply. See the
// file COPYING-EE for details.
//
//----------------------------------------------------------------------------
//
// Purpose: EDF Aeon Actions
// Authors: Max Waine
//

#ifndef E_ACTIONS_H__
#define E_ACTIONS_H__

struct cfg_t;

#ifdef NEED_EDF_DEFINITIONS

#define EDF_SEC_ACTION "action"

extern cfg_opt_t edf_action_opts[];

void E_CollectActions(cfg_t *cfg);
void E_ProcessActions(cfg_t *cfg);

#endif

#endif

// EOF

