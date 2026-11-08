include "directories.lua"

workspace "PixelByte"
  location "../build/%{action}"
  startproject "Sandbox"
  multiprocessorcompile "On"

  language "C"
  cdialect "C17"
  toolset "clang"

  warnings "Extra"
  fatalwarnings { "all" }
  enablewarnings {
    "all",
    "extra",
    "pedantic"
  }
  disablewarnings {
    "declaration-after-statement",
    "covered-switch-default",
    "unsafe-buffer-usage"
  }

  configurations { "Debug", "Profile", "Release" }

  filter "system:Windows"
    architecture "x86_64"
    defines { "PXB_WINDOWS" }
    
  filter "system:Linux"
    architecture "x86_64"
    defines { "PXB_LINUX" }

  filter "configurations:Debug"
    runtime "Debug"
    symbols "On"
    defines { "PXB_DEBUG" }

  filter "configurations:Profile"
    runtime "Release"
    optimize "On"
    symbols "On"
    defines { "PXB_PROFILE" }

  filter "configurations:Release"
    runtime "Release"
    optimize "Speed"
    defines { "PXB_RELEASE" }
  
  filter {}

group "PixelByte"
  include "PixelByte"

group ""
  include "Sandbox"

