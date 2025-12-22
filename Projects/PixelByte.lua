project "PixelByte"
  location "%{wks.location}/PixelByte"
  kind "StaticLib"

  targetdir (target_dir)
  objdir (obj_dir)
  
  files
  {
    "%{prj_src.PixelByte}/**.h",
    "%{prj_src.PixelByte}/**.c"
  }

  includedirs
  {
    "%{prj_src.PixelByte}/"
  }

  externalincludedirs
  {
    "%{include_dirs.fonts}/"
  }
