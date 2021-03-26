//
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
//
//----------------------------------------------------------------------------
//
// Purpose: Aeon system
// Authors: Max Waine, Andreas Jönsson
//
// ScriptManager::ProcessAeonFile, ScriptManager::SkipStatement, and
// OverwriteCode are based on code from AngelScript's Script Builder addon,
// licenced under the zlib and written by Andreas Jönsson
//

#include "aeon_common.h"
#include "aeon_system.h"
#include "d_dwfile.h"
#include "doomstat.h"
#include "e_actions.h"
#include "i_system.h"
#include "m_utils.h"
#include "m_qstr.h"
#include "p_map.h"
#include "sounds.h"
#include "w_wad.h"

namespace Aeon
{
   static void OverwriteCode(qstring &fileStr, size_t start, size_t len)
   {
      char *code = fileStr.bufferAt(start);
      for(size_t n = 0; n < len; n++)
      {
         if(*code != '\n')
            *code = ' ';
         code++;
      }
   }

   size_t ScriptManager::SkipStatement(qstring &fileStr, size_t pos)
   {
      asUINT len = 0;

      // Skip until ; or { whichever comes first
      while(pos < fileStr.length() && fileStr[pos] != ';' && fileStr[pos] != '{')
      {
         engine->ParseToken(fileStr.bufferAt(pos), fileStr.getSize() - pos, &len);
         pos += len;
      }

      // Skip entire statement block
      if(pos < fileStr.length() && fileStr[pos] == '{')
      {
         pos += 1;

         // Find the end of the statement block
         int level = 1;
         while(level > 0 && pos < fileStr.getSize())
         {
            asETokenClass t = engine->ParseToken(fileStr.bufferAt(pos), fileStr.getSize() - pos, &len);
            if(t == asTC_KEYWORD)
            {
               if(fileStr[pos] == '{')
                  level++;
               else if(fileStr[pos] == '}')
                  level--;
            }

            pos += len;
         }
      }
      else
         pos += 1;

      return pos;
   }

   void ScriptManager::ProcessAeonFile(qstring &fileStr)
   {
      // TODO: C preprocessing here
      Collection<qstring> actions;

      size_t pos = 0;
      while(pos < fileStr.getSize())
      {
         asUINT len = 0;
         asETokenClass t = engine->ParseToken(fileStr.bufferAt(pos), fileStr.getSize() - pos, &len);
         if(t == asTC_COMMENT || t == asTC_WHITESPACE)
         {
            pos += len;
            continue;
         }

         // Is this a preprocessor directive?
         if(fileStr[pos] == '$' && (pos + 1 < fileStr.getSize()))
         {
            const size_t directiveStart = pos++;

            t = engine->ParseToken(fileStr.bufferAt(pos), fileStr.getSize() - pos, &len);
            if(t == asTC_IDENTIFIER)
            {
               qstring token;
               token.copy(fileStr.bufferAt(pos), len);
               if(token == "action")
               {

                  pos += len;
                  t = engine->ParseToken(fileStr.bufferAt(pos), fileStr.getSize() - pos, &len);
                  if(t == asTC_WHITESPACE)
                  {
                     pos += len;
                     t = engine->ParseToken(fileStr.bufferAt(pos), fileStr.getSize() - pos, &len);
                  }

                  if(t == asTC_IDENTIFIER)
                  {
                     // Get the include file
                     qstring actionFunction;
                     actionFunction.copy(fileStr.bufferAt(pos), len);
                     pos += len;

                     // Store it for later processing
                     actions.add(actionFunction);

                     // Overwrite the action directive with space characters and 'void' to avoid compiler error
                     OverwriteCode(fileStr, directiveStart, strlen("$action"));
                     strncpy(fileStr.bufferAt(directiveStart), "void", strlen("void"));
                  }
               }
            }
         }
         // Don't search for metadata/actions within statement blocks or between tokens in statements
         else
            pos = SkipStatement(fileStr, pos);
      }

      // FIXME: Proper script section name. Maybe getLumpFileName and use that?
      module->AddScriptSection("AEONROOT", fileStr.constPtr());

      for(const qstring &action : actions)
         E_DefineAction(action.constPtr());
   }
};

// EOF

