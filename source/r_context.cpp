//
// The Eternity Engine
// Copyright(C) 2020 James Haley, Max Waine, et al.
// Copyright(C) 2020 Ethan Watson
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
// Purpose: Renderer context
// Some code is derived from Rum & Raisin Doom, Ethan Watson, used under
// terms of the GPLv3.
//
// Authors: Max Waine
//

#include <thread>

#include "c_io.h"
#include "c_runcmd.h"
#include "m_misc.h"
#include "i_video.h"
#include "r_context.h"
#include "v_misc.h"

using threadhandle_t = void (*)(); // FIXME: This elsewhere when threading real
using atomicval_t = ptrdiff_t; // FIXME: This elsewhere when threading real
struct renderdata_t
{
   rendercontext_t context;
   threadhandle_t	 thread;
   atomicval_t     running;
   atomicval_t     shouldquit;
   atomicval_t     framewaiting;
   atomicval_t     framefinished;
};

static renderdata_t *renderdatas = nullptr;

//
// Grab a given render context
//
rendercontext_t &R_GetContext(int index)
{
   return renderdatas[index].context;
}

//
// Initialises all the render contexts
//
void R_InitContexts(const int width)
{
   if(renderdatas)
      efree(renderdatas);

   renderdatas = estructalloc(renderdata_t, r_numcontexts);

   const float contextwidth = static_cast<float>(width) / static_cast<float>(r_numcontexts);

   for(int currentcontext = 0; currentcontext < r_numcontexts; currentcontext++)
   {
      rendercontext_t &context = renderdatas[currentcontext].context;

      context.bufferindex = currentcontext;

      context.startcolumn = static_cast<int>(roundf(static_cast<float>(currentcontext)     * contextwidth));
      context.endcolumn   = static_cast<int>(roundf(static_cast<float>(currentcontext + 1) * contextwidth));
   }
}


VARIABLE_INT(r_numcontexts, nullptr, 0, UL, nullptr);
CONSOLE_VARIABLE(r_numcontexts, r_numcontexts, 0)
{
   const int maxcontexts = std::thread::hardware_concurrency();

   if(r_numcontexts == 0)
      r_numcontexts = maxcontexts - 1; // allow scrolling left from 1 to maxcontexts
   else if(r_numcontexts == maxcontexts + 1)
      r_numcontexts = 1; // allow scrolling right from maxcontexts to 1
   else if(r_numcontexts > maxcontexts)
   {
      C_Printf(FC_ERROR "Warning: r_numcontexts's current maximum is %d, resetting to 1", maxcontexts);
      r_numcontexts = 1;
   }

   I_SetMode();
}

// EOF

