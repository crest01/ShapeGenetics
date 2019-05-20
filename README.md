# ShapeGenetics

Contains the code for the 2017 paper [ShapeGenetics: Using Genetic Algorithms for Procedural Modeling](http://onlinelibrary.wiley.com/doi/10.1111/cgf.13120/abstract)

## Structure
- *dependencies*: OpenGL and CUDA wrappers and tools, zlib, libpng
- *genetics*: Optimization algorithms (GA, SMC/SOSMC, RJMCMC) for the derivation tree
- *proccore*: Shape Grammar system
- *procrender*: UI and rendering stuff, main executable
- *run*: scripts and config files to run the executable

## Requirements
The code has been tested on Arch Linux (with clang 3.5.2 or gcc 5.4) and Windows 10 with VS2015

- C++11
- CUDA >= 8.0 (for volumetric evaluation)
- OpenGL >= 4.3
- libpng
- zlib

## Build

execute cmake and make in procrender/build/cmake

set the path in genetics/eval/gpuvolume/GpuVolumeEvaluator.cpp:234 so that it points to the nvrtc_files - folder in the same directory. I never got around to implementing the nvrtc setup properly...

Adapt some paths in the run/run.py - script: the script starts the program for each of the 4 config files in the experiment folders (ga.cfg, mh.cfg, smc.cfg, sosmc.cfg). The structure of the config file is pretty self-explainatory.
Alternatively just provide a config file as argument for the program.

## Notes
The gcc-version shipped with Arch Linux wasn't compatible with CUDA 8.0, and CUDA/OpenGL Interop was impossible due to my hardware setup, so i switched to the gcc deliverd with the CUDA package, and used ComputeShaders for some parts.

On Windows i had to use the nvrtc-package, since CUDA didn't support VS2015, and VS2014 didn't support C++11.

Futhermore on some versions of MSVC compile-time floating point operations sometimes fail: setting the floating point model to /fp:strict for at least the genetics_grammar - project solved the issue for me.

