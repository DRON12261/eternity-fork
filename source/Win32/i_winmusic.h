//
// Copyright(C) 2021 Roman Fomin
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// DESCRIPTION:
//      Windows native MIDI

#ifdef _WIN32

#ifndef __I_WINMUSIC__
#define __I_WINMUSIC__

bool I_WIN_InitMusic(int device);
void I_WIN_PlaySong(bool looping);
void I_WIN_PauseSong();
void I_WIN_ResumeSong();
void I_WIN_StopSong();
void I_WIN_SetMusicVolume(int volume);
bool I_WIN_RegisterSong(void *data, int size);
void I_WIN_UnRegisterSong();
void I_WIN_ShutdownMusic();

#endif

#endif
