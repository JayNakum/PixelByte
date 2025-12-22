![PixelByte](https://0x1deadb0b.github.io/assets/0xpixelbyte_2048x1024.png)

## TaskMan Config

```python
import os

from Scripts.terminal import run_command, run_commands
from Scripts.clean_files import clean_files, delete_folder
from Scripts.code_changes import sync, submit

import platform
IS_WINDOWS = platform.system() == 'Windows'
IS_LINUX = platform.system() == 'Linux'

def generate_projects() -> str:
    PREMAKE_FILE = os.path.join("Projects", "premake5.lua")
    if IS_WINDOWS:
        return f'{ os.path.join("Vendors", "premake5", "premake5.exe") } --file={PREMAKE_FILE} vs2022'
    elif IS_LINUX:
        return f'premake5 --file={PREMAKE_FILE} gmake'

def clean_build() -> str:
    msbuild = 'C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\MSBuild\\Current\Bin\\MSBuild.exe'

    if IS_WINDOWS:
        return f'"{msbuild}" { os.path.join("Build", "vs2022", "PixelByte.sln") } /t:Clean'
    elif IS_LINUX:
        return f'make -C { os.path.join("Build", "gmake") } clean'

def build_project(config:str) -> str:
    msbuild = 'C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\MSBuild\\Current\Bin\\MSBuild.exe'

    if IS_WINDOWS:
        return f'"{msbuild}" { os.path.join("Build", "vs2022", "PixelByte.sln") } /p:Configuration={config}'
    elif IS_LINUX:
        return f'bear --output compile_commands.json -- make -C { os.path.join("Build", "gmake") } config={config}'

TASKS = {
    'clean': {
        'func': clean_files,
        'kwargs': {
            'file_extensions': ('.h', '.cpp', '.c', '.hlsl'),
            'ignore_dir': ('.git', '.vscode', '__pycache__', 'Build', 'Tools', 'Vendors'),
            'working_dir': '.',
        }
    },
    'clean build dir': {
        'func': delete_folder,
        'kwargs': {
            'folder_path': 'Build',
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
```