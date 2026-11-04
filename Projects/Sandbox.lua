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
    "%{include_dirs.RGFW}/"
  }

  links
  {
    "PixelByte"
  }

  filter "system:Windows"
    links { "gdi32", "user32", "shell32" }

  filter "system:Linux"
    links { "X11", "Xrandr", "m", "dl", "pthread" }

  filter {}
