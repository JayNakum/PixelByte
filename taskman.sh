#!/bin/bash

echo -ne "\033]0;taskman\007"

source ./Tools/TaskMan/venv/bin/activate

python ./Tools/TaskMan/taskman.py
