-- DEFINED PATHS --
action = _ACTION or "null"
target_dir = "%{wks.location}/../Bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
obj_dir = "%{wks.location}/../Obj"

code_path = "%{wks.location}/../../Code"
vendors_path = "%{wks.location}/../../Vendors"

prj_src = {}
include_dirs = {}
lib_dirs = {}

-- CORE --
prj_src["PixelByte"] = "%{code_path}/PixelByte"
prj_src["Sandbox"] = "%{code_path}/Sandbox"

-- LIBRARIES --
include_dirs["fonts"] = "%{vendors_path}/fonts/"
include_dirs["SDL"] = "%{vendors_path}/SDL/include"
lib_dirs["SDL"] = "%{vendors_path}/SDL/build"
