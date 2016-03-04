#pragma once

#include <stdint.h>
#include <string>
#include <vector>


// CORE LOADING
//--------------------------------------------------------------------------------------------------

void coreInit(const std::string & corePath);
void coreClose();


// ROM LOADING
//--------------------------------------------------------------------------------------------------

void coreLoadGame(const std::string & romPath);


// CORE EMULATION
//--------------------------------------------------------------------------------------------------

void coreUpdate();


// VIDEO
//--------------------------------------------------------------------------------------------------

// Bits
// 31 . . . . . . . . . . . . . . . . . . 0
// AAAA AAAA RRRR RRRR GGGG GGGG BBBB BBBB
const std::vector<uint32_t> & coreVideoData(size_t & width, size_t & height);


// AUDIO
//--------------------------------------------------------------------------------------------------

std::vector<int16_t> coreAudioData();


// TIMINGS (AUDIO AND VIDEO)
//--------------------------------------------------------------------------------------------------

void coreTimings(double & fps, double & audioSampleRate);


// CORE SETTINGS
//--------------------------------------------------------------------------------------------------

struct SettingsEntryDesc
{
  std::string key;
  std::string name; // Human readable
  std::vector<std::string> choices;
};

typedef std::vector<SettingsEntryDesc> SettingsDesc;

SettingsDesc coreSettingsDesc();
void coreSettingsSet(const std::string & key, const std::string & value);


// JOYPAD
//--------------------------------------------------------------------------------------------------

// @note The same joypad for all players
std::vector<std::string> coreJoypadDesc();
void coreJoypadPress(const std::string & name);
void coreJoypadRelease(const std::string & name);

// SAVE STATE
//--------------------------------------------------------------------------------------------------

std::vector<uint8_t> coreSaveState();
bool coreRestoreState(const char * data, size_t sz);
