# Last Hope 3D Engine
In this repository you will be able to find a newly-created 3D game engine. 

It's been created for educational purposes, as we are 2 students from the Videogames development and design degree at CITM, Terrassa UPC.

## Example Scene:
![](https://i.gyazo.com/74d87d0a41695266943128662c33404b.png)

# FEATURES
- You can load a 3D model (FBX) + Texture dragging and dropping it to the engine's window.
- The Properties window displays all the relevant information from the 3D model and its texture:
  * Transfrom data
  * Geometry data
  * Texture data
- The Console window diplays all the relevant information comming from the inside of the engine. All kind of Logs, possible errors or usefull data. 
- The Application window displays all the relevant information from the engine's modules. It also allows to modify certain data to create a more personal engine. 
- You can freely orbit around your model with our 3D Camera. Controls: 
  * Right-Click free object orbit.
  * Right-Click + WASD moves the camera to the desider direction.
  * Mouse Wheel zooms in/out. 
  * ALT + Right-Click zoms in/out depending on the mouse motion. 
  * ALT + Left-Click focuses and orbits the 3D model on the X axis. 
  * F focuses the 3D model. 
- Some of the main engine's information is stored in a configuration (JSON) file, so it is not lost in further executions. 
- The engine has a Random Generator to help the user. 
- You can find 4 different visual styles for you to choose the one you like:
  * Classic style
  * Forest style (default)
  * Blue style
  * Dark style
- You can manage the entire UI, turn on/off every window or disable/enable all the UI so you can look at your 3D model perfectly. 
  
# Innovation: Antisotropic Filter 
- You can activate Antisotropic Filter in the Configuration Panel  
- There are up to 6 different modes: Nearest, Linear, Linear Mipmap Nearest, Linear Mipmap Linear, Low Antisotropic & Max Antisotropic. 

# Libraries Used:
- MathGeoLib -> http://clb.demon.fi/MathGeoLib/nightly/ 
- ImGui -> https://github.com/ocornut/imgui  
- SDL -> http://www.libsdl.org/
- Parson -> https://github.com/kgabis/parson
- ASSIMP -> http://assimp.sourceforge.net/
- DevIL -> http://openil.sourceforge.net/

# Authors: 
- Roger Homedes: https://github.com/rohomedesrius
- Samuel Navarro: https://github.com/GottaCodeHarder
