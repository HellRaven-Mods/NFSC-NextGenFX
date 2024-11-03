# OPUS-VANILLA-CustomVT
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
!! You will either need XANs shaderloader (recommended for better usability) or Extended Customization 4.0.1 by Archie !!
!! when using EC4.0.1 you need to enable Custom shaders and compile FX files in the ini of EC !!

**Xan Shaderloader:** https://github.com/xan1242/xnfs-shaderloader-carbon/releases/tag/1.0

- create a folder named FX in game root directory
- copy files into that FX folder

**How to customize the mod:**
-----------------------------------------------------------------------------------------------
- open "visualtreamtent.fx" with VisualStudio/Code, NotePad+ or any Editor you want
- Change the values to your preferations
- (! IMPORTANT !) To see the changes you Need to go to Video Settings and Exit out again, that will reload the shaders

 !! DO NOT TOUCH THE OTHER FILES OR YOU ELSE YOU CAN BRAKE THE MOD !!

**List of what added and changed:**
-----------------------------------------------------------------------------------------------
- better Motionblur: Utalizes motionvectors now
- added a simple DOF-shader
  - (new!) fixed (now actually applies DOF)
- added reimagined "HDR" with colorgrading
- added ambient-lighting
- added Vignette
- added ACES2 and FILMIC tonemaps (ACES2 activated at beginning, you can Change it)
- added chromatic abberation shader
- added sharpen shader
- added Film-grain shader
- (new!) added Bloom shader
- (new!) added Camera Distortion shader
- (new!) added Color desaturation shader
- (new!) made cliff fall, cop intro and speedbreaker effects editable
- (new!) added Fog Shader (currently nor recommended to use, use FogOptions instead: https://nfsmods.xyz/mod/729)


- balanced brightness-levels for Luma 709
- improved code Performance
  - (new!) now only enabled effects get calculated, that way, we improve Performance alot
- better particle shader for better Flares (Buggy when you fall from cliff in Canyon races) and smoke
- improved log to linear colorspace decompression
- improved screenefect shader for better road reflections and blur/Bloom calculations

!! YOU HAVE PERMISSION TO CUSTOMIZE THE MOD AS LONG AS PROPS ARE GIVEN !!

!! YOU HAVE PERMISSION TO SHARE AND USE THE MOD IN MODPACKS AS LONG AS PROPS ARE GIVEN !!



![Thumbnail0](https://github.com/user-attachments/assets/fd2df166-2f18-443b-9a31-24dad3d113c2)
![Need for Speed™ Carbon 22 10 2024 01_35_57](https://github.com/user-attachments/assets/674f60a2-7aa9-479b-8503-dd7797587671)
![Screenshot 2024-10-22 013427](https://github.com/user-attachments/assets/910a8d9d-4e6b-4635-b322-08b1d677e1de)
![Screenshot 2024-10-18 023836](https://github.com/user-attachments/assets/935f75f6-26cb-4455-82d8-c116ee0a2666)

