project "Glad"
    kind "StaticLib"
    language "C"
    staticruntime "off"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/gl.c",
        "include/glad/gl.h",

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
