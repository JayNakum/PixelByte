-- DEFINED PATHS --
action = _ACTION or "null"
target_dir = "%{wks.location}/../bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
obj_dir = "%{wks.location}/../obj"

code_path = "%{wks.location}/../../code"
vendors_path = "%{wks.location}/../../vendors"

prj_src = {}
include_dirs = {}
lib_dirs = {}

-- CORE --
prj_src["PixelByte"] = "%{code_path}/PixelByte"
prj_src["Sandbox"] = "%{code_path}/Sandbox"

-- LIBRARIES --
include_dirs["fonts"] = "%{vendors_path}/font8x8/font8x8-master/"
include_dirs["RGFW"] = "%{vendors_path}/RGFW/RGFW-1.8.0_win64_msvc16/include/"
