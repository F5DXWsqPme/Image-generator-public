# Image-generator  ![workflow-badge](https://github.com/F5DXWsqPme/CubeGame-public/actions/workflows/cmake.yml/badge.svg) #  
Simple image generator

![image](/images/frame_one_seed_5000.png "5000 samples per pixel")
*5000 samples per pixel*

### How install ###
- Install Vulkan (https://vulkan.lunarg.com/)
- Install Boost (https://www.boost.org/)
- Install GNU GCC compiler
- Install CMake
- Install git
- git clone --recursive https://github.com/F5DXWsqPme/Image-generator-public
- cd Image-generator-public
- mkdir build
- cd build
- cmake ..
- cmake --build . (macOS: brew install gmake; gmake)

### How run ###
Run with parameter-name of scene file (xml file; description of tags [here](InputFormat.md))

### Features ###
- Lighting
	- Rough surfaces
	- Metall materials
	- Dielectric materials
	- Volumetric light sources
- HDR rendering
- Bloom
- Tone mapping
- Absorption
- Fog
- XML scene description