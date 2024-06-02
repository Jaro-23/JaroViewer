workspace "JaroViewerBasicExample"
    location "Generated"
    language "C++"
    architecture "x86_64"

    configurations { "Debug", "Release" }
    filter { "configurations:Debug" }
        symbols "On"

    filter { "configurations:Release" }
        optimize "On"

    filter {}

    targetdir ("Build/Bin/%{prj.name}/%{cfg.longname}")
	objdir ("Build/Obj/%{prj.name}/%{cfg.longname}")

-- Custom functions for easy use
function includeGLFW()
    includedirs "Libraries/GLFW/Include"
end

function linkGLFW()
    libdirs "Libraries/GLFW/Lib"

    filter "kind:not StaticLib"
        links "glfw3"
    filter {}
end

function includeGLAD()
    includedirs "Libraries/GLAD/Include"
end

function useJaroViewer()
    includedirs "Projects/JaroViewer/header"
    links "JaroViewer"
    linkGLFW()
end

-- Projects
project "JaroViewer"
    kind "StaticLib"
    files "Projects/JaroViewer/**"
    includeGLFW()

project "App"
    kind "WindowedApp"
    files "Projects/App/**"

    useJaroViewer()

    filter { "system:windows" }
        links { "OpenGL32" }

    filter { "system:not windows" }
        links { "GL" }