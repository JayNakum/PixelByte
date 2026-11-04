import os

from Scripts.terminal import run_command, run_commands
from Scripts.cleanup import clean_files
from Scripts.code_changes import sync, submit
from Scripts.file import download, extract, delete_folders

import platform
IS_WINDOWS = platform.system() == 'Windows'
IS_LINUX = platform.system() == 'Linux'

VENDORS_DIR = "vendors"
TEMP_DIR = "tmp"

DEPENDENCIES = {
    'premake5': "https://github.com/premake/premake-core/releases/download/v5.0.0-beta8/premake-5.0.0-beta8-windows.zip",
    'RGFW': "https://github.com/ColleagueRiley/RGFW/releases/download/1.8.1/RGFW-1.8.0_win64_msvc16.zip",
    'font8x8': "https://github.com/dhepper/font8x8/archive/refs/heads/master.zip"
}

def fetch_dependencies(working_dir:str) -> bool:
    okay = True

    for folder_name, url in DEPENDENCIES.items():
        extract_dir = os.path.join(VENDORS_DIR, folder_name)
        delete_folders((extract_dir,), working_dir)

        raw_archive_name = url.split('/')[-1]
        archive_name = f"dependencies/{folder_name}_{raw_archive_name}"
        temp_archive_path = os.path.join(TEMP_DIR, archive_name)

        full_temp_path = os.path.join(working_dir, temp_archive_path)
        if not os.path.exists(full_temp_path):
            print(f"[INFO] Dependency '{folder_name}' not cached.")
            okay &= download(url, temp_archive_path, working_dir)

        if okay:
            okay &= extract(temp_archive_path, extract_dir, working_dir)

    return okay

MSBUILD = 'C:\\Program Files\\Microsoft Visual Studio\\2022\\Professional\\MSBuild\\Current\\Bin\\MSBuild.exe'
SOLUTION_DIR = os.path.join("build", "vs2022", "PixelByte.sln")

def generate_projects() -> str:
    PREMAKE_FILE = os.path.join("projects", "premake5.lua")
    if IS_WINDOWS:
        return f'{ os.path.join("vendors", "premake5", "premake5.exe") } --file={PREMAKE_FILE} vs2022'

def clean_build() -> str:
    if IS_WINDOWS:
        return f'"{MSBUILD}" { SOLUTION_DIR } /t:Clean'

def build_project(config:str) -> str:
    if IS_WINDOWS:
        return f'"{MSBUILD}" { SOLUTION_DIR } /p:Configuration={config}'

TASKS = {
    'clean': {
        'func': clean_files,
        'kwargs': {
            'file_extensions': ('.h', '.cpp', '.c', '.hlsl'),
            'ignore_dir': ('.git', '.vscode', '__pycache__', 'build', 'vendors'),
            'working_dir': '.',
        }
    },
    'cleanup': {
        'func': delete_folders,
        'kwargs': {
            'folder_paths': ('build', 'tmp'),
            'working_dir': '.',
        }
    },
    'sync': {
        'func': sync,
        'kwargs': {
            'commit': None,
            'working_dir': '.',
        }
    },

    'fetch dependencies': {
        'func': fetch_dependencies,
        'kwargs' : {
            'working_dir': '.',
        }
    },

    'generate projects': {
        'func': run_command,
        'kwargs': {
            'command': generate_projects(),
            'working_dir': '.',
        }
    },

    'clean build': {
        'func': run_commands,
        'kwargs': {
            'commands' : [ 
                clean_build(), 
                build_project('Debug'),
                build_project('Profile'),
                build_project('Release')
            ],
            'working_dir': '.'
        }
    },

    'submit': {
        'func': submit,
        'kwargs': {
            'working_dir': '.',
        }
    }
}
