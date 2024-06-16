workspace "HelloWorld"
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

project "HelloWorld"
   kind "ConsoleApp"

   links { "OpenGL" }
   files "Projects/HelloWorld/**"
   includedirs "Libraries/HelloWorld/Include/*"
