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
!! You will either need XANs shaderloader (recommended for better usability) or Extended Customization 4.0.1 by Archie !!
!! when using EC4.0.1 you need to enable Custom shaders and compile FX files in the ini of EC !!

**Xan Shaderloader:** https://github.com/xan1242/xnfs-shaderloader-carbon/releases/tag/1.0

- create a folder named FX in game root directory
- copy files into that FX folder

**additional note:** The Shader is meant to be used with Project Palmont (https://nfsmods.xyz/mod/5165) but also works fine with the vanilla game

**How to customize the mod:**
-----------------------------------------------------------------------------------------------
- open "visualtreamtent.fx" with VisualStudio/Code, NotePad+ or any Editor you want
- Change the values to your preferations
- (! IMPORTANT !) To see the changes you Need to go to Video Settings and Exit out again, that will reload the shaders

 !! DO NOT TOUCH THE OTHER FILES OR YOU ELSE YOU CAN BRAKE THE MOD !!

**List of what added and changed:**
-----------------------------------------------------------------------------------------------
- better Motionblur: Utalizes motionvectors now
  - made it more nuanced
- added a simple DOF-shader
  - (new!) fixed (now actually applies DOF)
- added reimagined "HDR"
  - fixed, now behaves like actual FakeHDR
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
- (new!) added some more controls in "visualtreatment.fx"
- (new!) added standalone colorgrading
- (new!) added color temperature adjustment
- (new!) re-added pauseblur
- (new!) added brightness and contrast control
-----------------------------------------------------------------------------------------------
- balanced brightness-levels for Luma 709
- improved code Performance
  - (new!) now only enabled effects get calculated, that way, we improve Performance alot
- better particle shader for better Flares (bug seems to be fixed) and smoke
- improved log to linear colorspace decompression
- (BONUS) improved screenefect shader for better road reflections on Project Palmont
-----------------------------------------------------------------------------------------------
!! YOU DO NOT HAVE PERMISSION TO CUSTOMIZE THE MOD EXCEPT YOU ASK ME !!

!! YOU HAVE PERMISSION TO SHARE AND USE THE MOD IN MODPACKS AS LONG AS PROPS ARE GIVEN !!

![Thumbnail0](https://github.com/user-attachments/assets/da2f4d2a-6f8e-4c33-ab24-9546ba52b2e6)
![Screenshot0](https://github.com/user-attachments/assets/fd2df166-2f18-443b-9a31-24dad3d113c2)
![Screenshot1](https://github.com/user-attachments/assets/813bed66-8bca-42de-b997-4e7a9c1664ce)
![Screenshot2](https://github.com/user-attachments/assets/674f60a2-7aa9-479b-8503-dd7797587671)
![Screenshot3](https://github.com/user-attachments/assets/935f75f6-26cb-4455-82d8-c116ee0a2666)


