# JaroViewer
JaroViewer is a simple 3D graphics library that allows the user to still work with glfw3 directly but abstracts the basic handling and managing of the component. It is also include some abstracted features like framebuffers, post processing, cubemaps, ...

# Installatation
## Prerequisites
The library is compiled and build using generated files from premake. So you going to need premake5 (other versions are untested). You also going to need a few libraries. Here you have list with links to the respective libraries:
### Libraries
 * GLFW (https://github.com/glfw/glfw?tab=readme-ov-file)
 * Assimp (https://github.com/assimp/assimp)
 * [Possibly needed for assimp] Zlib (https://github.com/madler/zlib)
## Generating files
To run the program or compile the library, first you need to run premake5 {format}. After this command the project files will be generated in the folder.
## Run the generated files
The last step for generating the library is to execute these generated files. Beneath you will see the command or step to produce the library based of the format:
 * gmake(2) &#8594 make config={release|debug} -C Generated/
## Copy library or execute test program
The library will now be found in "Build/Bin/JaroViewer/{Debug|Release}/". If you want to test the library you can run the program at "Build/Bin/App/{Debug|Release}/" for a simple demo.
