#include "../../addons/include/xbmc_vis_dll.h"

#include "nastyfft.h"

#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
using namespace std;

NastyFFT plugin;


char **g_presets=0;


//-- Create -------------------------------------------------------------------
// Called on load. Addon should fully initalize or return error status
//-----------------------------------------------------------------------------
ADDON_STATUS Create(void* hdl, void* props)
{
  
  printf("Create\n");
	
  if (!props)
    return STATUS_UNKNOWN;

  VIS_PROPS* visProps = (VIS_PROPS*)props;

  plugin.setSize(visProps->width, visProps->height);
 

  return STATUS_NEED_SETTINGS;
  
}


//-- Stop ---------------------------------------------------------------------
// This dll must cease all runtime activities
// !!! Add-on master function !!!
//-----------------------------------------------------------------------------
extern "C" 
void Stop()
{
   printf("Stop\n");
}

//-- Destroy ------------------------------------------------------------------
// Do everything before unload of this add-on
// !!! Add-on master function !!!
//-----------------------------------------------------------------------------
extern "C" 
void Destroy()
{
  printf("Destroy\n");
  if (g_presets) {
	  free(g_presets);
	  g_presets = 0;
	}
}



//-- Render -------------------------------------------------------------------
// Called once per frame. Do all rendering here.
//-----------------------------------------------------------------------------

extern "C" 
void Render()
{
  plugin.render();
}

extern "C" 
void Start(int iChannels, int iSamplesPerSec, int iBitsPerSample, const char* szSongName)
{

}


extern "C" 
void AudioData(const short* pAudioData, int iAudioDataLength, float *pFreqData, int iFreqDataLength)
{
  plugin.scopeEvent(pFreqData, iFreqDataLength);
}


//-- GetInfo ------------------------------------------------------------------
// Tell XBMC our requirements
//-----------------------------------------------------------------------------
extern "C" 
void GetInfo(VIS_INFO* pInfo)
{
  pInfo->bWantsFreq = true;
//   pInfo->iSyncDelay = 0;
}


//-- GetSubModules ------------------------------------------------------------
// Return any sub modules supported by this vis
//-----------------------------------------------------------------------------
extern "C" unsigned int GetSubModules(char ***names)
{
  return 0; // this vis supports 0 sub modules
}

//-- OnAction -----------------------------------------------------------------
// Handle XBMC actions such as next preset, lock preset, album art changed etc
//-----------------------------------------------------------------------------
extern "C" 
bool OnAction(long flags, const void *param)
{
  printf("OnAction\n");
  
  bool ret = false;
  
  if (flags == VIS_ACTION_LOAD_PRESET && param)
  {
	printf("VIS_ACTION_LOAD_PRESET\n");
    int pindex = *((int *)param);
	plugin.loadPreset(pindex);
	
    ret = true;
  }
//   else if (flags == VIS_ACTION_NEXT_PRESET)
//   {
// //    switchPreset(ALPHA_NEXT, SOFT_CUT);
//     if (!globalPM->isShuffleEnabled())
//       globalPM->key_handler(PROJECTM_KEYDOWN, PROJECTM_K_n, PROJECTM_KMOD_CAPS); //ignore PROJECTM_KMOD_CAPS
//     else
//       globalPM->key_handler(PROJECTM_KEYDOWN, PROJECTM_K_r, PROJECTM_KMOD_CAPS); //ignore PROJECTM_KMOD_CAPS
//     ret = true;
//   }
//   else if (flags == VIS_ACTION_PREV_PRESET)
//   {
// //    switchPreset(ALPHA_PREVIOUS, SOFT_CUT);
//     if (!globalPM->isShuffleEnabled())
//       globalPM->key_handler(PROJECTM_KEYDOWN, PROJECTM_K_p, PROJECTM_KMOD_CAPS); //ignore PROJECTM_KMOD_CAPS
//     else
//       globalPM->key_handler(PROJECTM_KEYDOWN, PROJECTM_K_r, PROJECTM_KMOD_CAPS); //ignore PROJECTM_KMOD_CAPS
// 
//     ret = true;
//   }
//   else if (flags == VIS_ACTION_RANDOM_PRESET)
//   {
//     globalPM->setShuffleEnabled(g_configPM.shuffleEnabled);
//     ret = true;
//   }
//   else if (flags == VIS_ACTION_LOCK_PRESET)
//   {
//     globalPM->setPresetLock(!globalPM->isPresetLocked());
//     unsigned preset;
//     globalPM->selectedPresetIndex(preset);
//     globalPM->selectPreset(preset);
//     ret = true;
//   }
  
  return ret;
}

//-- GetPresets ---------------------------------------------------------------
// Return a list of presets to XBMC for display
//-----------------------------------------------------------------------------
extern "C" 
unsigned int GetPresets(char ***preset_names)
{
  vector<string> names;
  
  names.push_back("Standard");
  names.push_back("Hero");
  names.push_back("Flames");
  
  int g_numPresets = names.size();
  
  if (g_numPresets > 0)
  {
    g_presets = (char**) malloc(sizeof(char*)*g_numPresets);
    for (unsigned i = 0; i < g_numPresets; i++)
    {
      g_presets[i] = (char*) malloc(strlen(names.at(i).c_str())+2);
      if (g_presets[i])
        strcpy(g_presets[i], names.at(i).c_str());
    }
    *preset_names = g_presets;
  }
  return g_numPresets;

}

//-- GetPreset ----------------------------------------------------------------
// Return the index of the current playing preset
//-----------------------------------------------------------------------------
extern "C" 
unsigned GetPreset()
{
  printf("GetPreset\n");
  return plugin.presetIndex();
}

//-- IsLocked -----------------------------------------------------------------
// Returns true if this add-on use settings
//-----------------------------------------------------------------------------
extern "C" 
bool IsLocked()
{
  printf("Is Locked\n");
  return false;
}


//-- HasSettings --------------------------------------------------------------
// Returns true if this add-on use settings
// !!! Add-on master function !!!
//-----------------------------------------------------------------------------
extern "C" 
bool HasSettings()
{
  printf("Has Settings\n");
  return true;
}

//-- GetStatus ---------------------------------------------------------------
// Returns the current Status of this visualisation
// !!! Add-on master function !!!
//-----------------------------------------------------------------------------
extern "C" 
ADDON_STATUS GetStatus()
{
  printf("GetStatus\n");
  return STATUS_OK;
}

//-- GetSettings --------------------------------------------------------------
// Return the settings for XBMC to display
// !!! Add-on master function !!!
//-----------------------------------------------------------------------------
extern "C" unsigned int GetSettings(StructSetting ***sSet)
{
  printf("GetSettings\n");
   
  return 0;
}

//-- FreeSettings --------------------------------------------------------------
// Free the settings struct passed from XBMC
// !!! Add-on master function !!!
//-----------------------------------------------------------------------------

extern "C" void FreeSettings()
{
  printf("FreeSettings\n");
}

//-- SetSetting ---------------------------------------------------------------
// Set a specific Setting value (called from XBMC)
// !!! Add-on master function !!!
//-----------------------------------------------------------------------------
extern "C" ADDON_STATUS SetSetting(const char *strSetting, const void* value)
{
  printf("SetSettings\n");
	
  if (!strSetting || !value) return STATUS_UNKNOWN;

  printf("Setting: %s\n", strSetting);

  ScenePreset *priv = plugin.scenePreset();
   
  if (strcmp(strSetting, "scale")==0) {
	  int val = *(int*)value;
	  printf("Scale: %d\n", val);
	  priv->scale = 1+val ;
  }
  else if (strcmp(strSetting, "invert")==0) {
	  priv->cinvert = *(bool*)value;
  }
  else if (strcmp(strSetting, "transient")==0) {
	  priv->tranzient = *(bool*)value;
  }
  else if (strcmp(strSetting, "cammove")==0) {
	  priv->cammove = *(bool*)value;
  }
  else if (strcmp(strSetting, "flatten")==0) {
	  priv->flatten = *(bool*)value;
  }
  else if (strcmp(strSetting, "eye_y")==0) {
	  int val = *(int*)value;
	  printf("Cam-Y: %d\n", val);
	  priv->cam_coords.y =  val;
	  
  }
  else if (strcmp(strSetting, "eye_z")==0) {
	  int val = *(int*)value;
	  printf("Cam-Z: %d\n", val);
	  priv->cam_coords.z = val ;
	  
  }
  else if (strcmp(strSetting, "brick_space_z")==0) {
	  int val = *(int*)value;
	  printf("brick_space_z: %d\n", val);
	  priv->step_z = val / 10.0f;
  }
  else if (strcmp(strSetting, "brick_shiness")==0) {
	  int val = *(int*)value;
	  printf("brick_shiness: %d\n", val);
	  priv->shiness = val / 100.0f;
  }
  else if (strcmp(strSetting, "floor_rotate_x")==0) {
	
	  int val = *(int*)value;
	  printf("floor_rotate_x: %d\n", val);
	  priv->rot_x = 0 + (val*36);
  }
  else {
	return STATUS_UNKNOWN;
	
  }
  return STATUS_OK;
}
