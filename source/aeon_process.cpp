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
// Authors: Max Waine, James Haley, Samuel Villarreal, Andreas Jönsson
//
// ScriptManager::ProcessAeonFile, ScriptManager::SkipStatement, and
// OverwriteCode are based on code from AngelScript's Script Builder addon,
// licenced under the zlib and written by Andreas Jönsson
//
// mcpp-related code is cribbed from Kex which, whilst people can't pulicly
// see (at the time of typing), I'd feel guilty for not crediting
//

#include "mcpp_lib.h"
#include "rwops.h"

#include "aeon_common.h"
#include "aeon_system.h"
#include "d_dwfile.h"
#include "doomstat.h"
#include "e_actions.h"
#include "e_edf.h"
#include "i_system.h"
#include "m_utils.h"
#include "m_qstr.h"
#include "p_map.h"
#include "sounds.h"
#include "w_wad.h"

namespace Aeon
{
   static int ppSource; // As in lumpinfo_t source
   static constexpr size_t MCPP_NUM_ARGS = 6;

   void ScriptManager::InitMCPP()
   {
      mcpp_setopencallback([](const char *fileName, const char *mode) -> void *
      {
         DWFILE dwfile;
         size_t buffsize;
         byte*  buffer;
         lumpinfo_t **lumpinfo = wGlobalDir.getLumpInfo();
         int          lumpnum  = wGlobalDir.getLumpNameChain(fileName)->index;

         while(lumpnum >= 0 &&
              (strncasecmp(lumpinfo[lumpnum]->name, fileName, 8) || lumpinfo[lumpnum]->li_namespace != lumpinfo_t::ns_global)
               && lumpinfo[lumpnum]->source != ppSource)
            lumpnum = lumpinfo[lumpnum]->next;

         if(lumpnum < 0)
         {
            E_EDFLoggedWarning(2, "mcpp_setopencallback: %s not found\n", fileName);
            return nullptr;
         }

         dwfile.openLump(lumpnum);

         if(!dwfile.isOpen())
         {
            E_EDFLoggedWarning(2, "mcpp_setopencallback: %s not found\n", fileName);
            return nullptr;
         }

         const size_t fileLen = dwfile.fileLength() + 1;
         qstring fileStr(fileLen);
         dwfile.read(fileStr.getBuffer(), 1, fileLen - 1);
         fileStr[fileLen - 1] = '\0';

         buffer = emalloc(byte *, fileLen);
         fileStr.copyInto(reinterpret_cast<char *>(buffer), fileLen);

         return mcpp_openmemory(buffer, fileLen);
      });

      mcpp_setclosecallback([](void* pData)
      {
         if(pData)
            efree(pData);
      });
   }

   static void GetMCPPOutput(const qstring &fileStr, lumpinfo_t *lumpinfo)
   {
      ppSource = lumpinfo->source;

      char** argv;
      uint32_t argc = MCPP_NUM_ARGS;

      argv = new char*[MCPP_NUM_ARGS];

      argv[0] = estrdup("mcpp.exe");
      argv[1] = estrdup("-P");
      argv[2] = estrdup(lumpinfo->name); // FIXME: lfn
      argv[3] = estrdup("-Y");
      argv[4] = estrdup("-e");
      argv[5] = estrdup("utf8");

      mcpp_use_mem_buffers(1);
      mcpp_lib_main((int)argc, argv);

      const char *output = mcpp_get_mem_buffer(OUTDEST::OUT);
      const char *error  = mcpp_get_mem_buffer(OUTDEST::ERR);
      const char *debug  = mcpp_get_mem_buffer(OUTDEST::DBG);

      for(uint32_t i = 0; i < argc; ++i)
         efree(argv[i]);

      delete[] argv;
   }

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

   void ScriptManager::ProcessAeonFile(qstring &fileStr, lumpinfo_t *lumpinfo)
   {
      // TODO: C preprocessing here
      Collection<qstring> actions;
      Collection<qstring> currNamespaces;

      GetMCPPOutput(fileStr, lumpinfo);

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

         // Check if namespace
         qstring tokStr(len);
         strncpy(tokStr.getBuffer(), fileStr.bufferAt(pos), len);
         if(tokStr == "namespace")
         {
            // Get the identifier after "namespace"
            do
            {
               pos += len;
               t = engine->ParseToken(fileStr.bufferAt(pos), fileStr.getSize() - pos, &len);
            }
            while(t == asTC_COMMENT || t == asTC_WHITESPACE);

            qstring newNamespace(len);
            strncpy(newNamespace.getBuffer(), fileStr.bufferAt(pos), len);
            currNamespaces.add(std::move(newNamespace));

            // Search until first { is encountered
            while(pos < fileStr.length())
            {
               engine->ParseToken(fileStr.bufferAt(pos), fileStr.getSize() - pos, &len);

               // If start of namespace section encountered stop
               if(fileStr[pos] == '{')
               {
                  pos += len;
                  break;
               }

               // Check next symbol
               pos += len;
            }

            continue;
         }

         // Check if end of namespace
         if(!currNamespaces.isEmpty() && fileStr[pos] == '}')
         {
            currNamespaces.pop();
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
                     // Get the action function
                     qstring actionFunction;
                     qstring fullyQualifiedAction;

                     actionFunction.copy(fileStr.bufferAt(pos), len);

                     for(const qstring &currNS : currNamespaces)
                        fullyQualifiedAction += currNS + "::";
                     fullyQualifiedAction += actionFunction;

                     pos += len;

                     // Store it for later processing
                     actions.add(std::move(fullyQualifiedAction));

                     // Overwrite the action directive with space characters and 'void' to avoid compiler error
                     OverwriteCode(fileStr, directiveStart, strlen("$action"));
                     strncpy(fileStr.bufferAt(directiveStart), "void", strlen("void"));
                  }
               }
            }
         }
         else
         {
            // Don't search for metadata/actions within statement blocks or between tokens in statements
            pos = SkipStatement(fileStr, pos);
         }
      }

      // FIXME: Proper script section name. Maybe getLumpFileName and use that?
      module->AddScriptSection("AEONROOT", fileStr.constPtr());

      for(const qstring &action : actions)
         E_DefineAction(action.constPtr());
   }
};

// EOF

