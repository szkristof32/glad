project "Glad"
    kind "StaticLib"
    language "C"
    staticruntime "off"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/gl.c",
        "src/egl.c",
        "src/gles1.c",
        "src/gles2.c",

        "include/glad/gl.h",
        "include/glad/egl.h",
        "include/glad/gles1.h",
        "include/glad/gles2.h",

        "include/EGL/*.h",
        "include/KHR/*.h"
    }

    includedirs
    {
        "include"
    }
    
    filter "system:windows"
        systemversion "latest"

        files
        {
            "src/wgl.c",
            "include/glad/wgl.h"
        }

    filter "system:linux"
        files
        {
            "src/glx.c",
            "include/glad/glx.h"
        }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
