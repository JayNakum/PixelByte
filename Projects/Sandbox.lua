project "Sandbox"
  location "%{wks.location}/Sandbox"
  kind "ConsoleApp"

  targetdir (target_dir)
  objdir (obj_dir)
  
  files
  {
    "%{prj_src.Sandbox}/**.h",
    "%{prj_src.Sandbox}/**.c"
  }

  includedirs
  {
    "%{prj_src.Sandbox}/",
    "%{prj_src.PixelByte}/"
  }
  
  externalincludedirs
  {
    "%{include_dirs.SDL}/"
  }

  libdirs
  {
    "%{lib_dirs.SDL}/"
  }

  filter "system:Windows"
    defines { "SDL_MAIN_HANDLED" }
    postbuildcommands
    {
      '{COPY} "%{lib_dirs.SDL}/Release/SDL3.dll" "%{cfg.targetdir}/"'
    }
    libdirs
    {
      "%{lib_dirs.SDL}/Release/"
    }
  
  filter {}

  links
  {
    "PixelByte",
    "SDL3"
  }
