# [NFSC] NextGenFX
Shader-Mod for NFS Carbon

Hi Folks,
This is a shader mod that completely changes how the game Looks and feel.

**First of all, i want to give a Special thanks to everyone who helped me and set the base:**
-----------------------------------------------------------------------------------------------
- Xan/Tenjoin (decompiled shaders)
- BlueSkyWest (explaining some code)
- PAIN (some ideas)
- Good Shit (idea for Fixing flare-rotation direction)
- UNLOCALIZED (some ideas of how to fix certain szenarios)

**How to set-up the mod:**
-----------------------------------------------------------------------------------------------
- delete FX folder (in game root Folder) if existing

Vanilla install:
- install widescreenfix and hdreflections first!
- copy FX Folder, SCRIPTS Folder and dinput8.dll in game root Folder, replace files if asked for
- install new Filters with Binary from "Filter" Folder (uninstaller also located there)
- (Bonus) install new Flares from "Flares" Folder (uninstaller also located there), install VANILLA_INSTALL(or _UNSINSTALL) script with OGVI

SunRise/REDUX Install:
- copy FX and SCRIPTS Folder to game root Folder
- replace files if any
- (Bonus) Install new Flares from "Flares" Folder (uninstaller also located there)

Emitter Overhaul:
- make a backup of your global folder first
- install each part by number with OGVI (ignore all errors if there are any, all is fine):
  1.  emitterdata
  2.  emittergroup
  3.  simsurface
  3.1 simsurface(W2C) (if Wolrd 2 Carbon mod is used)

**How to customize the mod:**
-----------------------------------------------------------------------------------------------
- open "visualtreamtent.fx" from FX folder with VisualStudio/Code, NotePad+ or any Editor you want
- Change the values to your preferations
- (! IMPORTANT !) To see the changes you Need to go to Video Settings and Exit out again, that will reload the shaders
	 - if u use SunRise: in SunRise.ini (scripts folder) set Shaderreload key to 35 (END Key), than you can reload with a keypress

 !! DO NOT TOUCH THE OTHER FILES OR YOU ELSE YOU CAN BRAKE THE MOD !!

**List of what added and changed:**
-----------------------------------------------------------------------------------------------
- better Motionblur
- reimagined "HDR"
- Vignette
- Tonemaps
- Chromatic Aberration
- Sharpen shader
- Film-grain shader
- Bloom shader
- Color desaturation shader
- Adjustable Cliff-Fall, Cop-Engangement and Speedbreaker Effects: for SunRise we use vanilla cop-intro
- more Controls in "visualtreatment.fx"
- Standalone Color Grading
- Color-Temperature Adjustment
- Brightness and Contrast Control
-----------------------------------------------------------------------------------------------
!! YOU DO NOT HAVE PERMISSION TO CUSTOMIZE THE MOD EXCEPT YOU ASK ME !!
!! YOU ONLY HAVE PERMISSION TO USE THE MOD IN MODPACKS AS LONG AS PROPS ARE GIVEN !!
!! YOU ONLY HAVE PERMISSION TO SHARE YOUR OWN CONFIG-FILE AND COLORGRADINGS !!

![Thumbnail0](https://github.com/user-attachments/assets/da2f4d2a-6f8e-4c33-ab24-9546ba52b2e6)
![Screenshot0](https://github.com/user-attachments/assets/fd2df166-2f18-443b-9a31-24dad3d113c2)
![Screenshot1](https://github.com/user-attachments/assets/813bed66-8bca-42de-b997-4e7a9c1664ce)
![Screenshot2](https://github.com/user-attachments/assets/674f60a2-7aa9-479b-8503-dd7797587671)
![Screenshot3](https://github.com/user-attachments/assets/935f75f6-26cb-4455-82d8-c116ee0a2666)


