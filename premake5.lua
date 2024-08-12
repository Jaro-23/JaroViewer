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

	flags {
		"MultiProcessorCompile"
	}

   targetdir "Build/Bin/%{prj.name}/%{cfg.buildcfg}"
   objdir "Build/Obj/%{prj.name}/%{cfg.buildcfg}"

-- Custom functions
function includeJaroViewer()
	includedirs "Projects/JaroViewer/header"
	links "JaroViewer"

	libdirs "Libraries/Lib"
	filter { "kind:not StaticLib" }
		links "glfw3"

	filter {}
end

-- Create the different projects
project "JaroViewer"
   kind "StaticLib"

   files "Projects/JaroViewer/**"
   files { "Projects/JaroViewer/src/glad.c" }
   includedirs "Libraries/Include"

project "App"
	kind "ConsoleApp"
	files "Projects/App/**"

	includedirs "Libraries/Include"
	includeJaroViewer()

	filter { "system:windows" }
		links { "OpenGL" }

	filter { "system:not windows" }
		links { "GL" }
