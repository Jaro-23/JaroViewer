workspace "JaroViewer Basic Example"
   location "Generated"
   language "C++"
   architecture "x86_64"

   configurations { "Debug", "Release" }
   
   filter "configurations:Debug"
      symbols "On"

   filter "configurations:Release"
      optimize "On"

   filter { }
      
   targetdir "Build/Bin/%{prj.name}/%{cfg.buildcfg}"
   objdir "Build/Obj/%{prj.name}/%{cfg.buildcfg}"

-- Custom functions
function includeJaroViewer()
	includedirs "Projects/JaroViewer/header"
	links "JaroViewer"

	libdirs "Libraries/Lib"
	links "glfw3"
end

-- Create the different projects
project "JaroViewer"
   kind "StaticLib"

   files "Projects/JaroViewer/**"
   includedirs "Libraries/Include"

project "App"
	kind "ConsoleApp"
	files "Projects/App/**"
   files { "Projects/App/src/glad.c" }

	includeJaroViewer()
   includedirs "Libraries/Include"

	filter { "system:windows" }
		links { "OpenGL32" }
		
	filter { "system:not windows" }
		links { "GL" }