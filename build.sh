#!/bin/bash -e

SAVED_DIR=$(pwd)
PROJECT_DIR=$(dirname "$0")
PROJECT_DIR=$(readlink -f "$PROJECT_DIR")

function usage() {
    column -t -s "@" <<<"""
Note: This script is expected to run on Ubuntu/Debian
usage: $0 [options]
  -c, --clean@Clean before building.
  -l, --log@Enable logger.
  -d, --debug@Build with debugging symbols.
  -v [ver], --version [ver]@Select a speficic Godot version.
  -u, --unix@Compile for Unix
  -w, --windows@Compile for Windows
  -h, --help@Display this.
"""
    exit 1
}

unset ARG_CLEAN
unset ARG_DEBUG
unset ARG_GODOT_VERSION
unset ARG_LOG
unset ARG_UNIX
unset ARG_WINDOWS
MAKE_ARGS="-j $(($(nproc) / 2))"

while [[ $# -ne 0 ]]; do
    case $1 in
    -c | --clean)  shift
        ARG_CLEAN=true ;;
    -l | --log) shift
        ARG_LOG=true ;;
    -d | --debug)  shift
        ARG_DEBUG=true ;;
    -v | -version) shift
        ARG_GODOT_VERSION=$1
        shift ;;
    -u | --unix) shift
        ARG_UNIX=true ;;
    -w | --windows) shift
        ARG_WINDOWS=true ;;
    -h | --help | *) shift
        usage ;;
    esac
done

if [[ -z $ARG_UNIX && -z $ARG_WINDOWS ]]; then
    ARG_UNIX=true
    ARG_WINDOWS=true
fi

# check dependencies
PACKAGES="make g++ libstdc++6 pkg-config scons"
if [[ -n $ARG_WINDOWS ]]; then
    PACKAGES+=" mingw-w64"
fi
if [[ -n $ARG_UNIX ]]; then
    PACKAGES+=" libncurses-dev"
fi
for PACKAGE in $PACKAGES; do
    dpkg -s $PACKAGE >/dev/null 2>&1 || { echo "Installing $PACKAGE..."; sudo apt install $PACKAGE -y; }
done

# Posix Threads
if [[ -n $ARG_WINDOWS ]]; then
    if [[ $(update-alternatives --display x86_64-w64-mingw32-g++ | grep currently | awk '{ print $NF }') != "/usr/bin/x86_64-w64-mingw32-g++-posix" ]]; then
        sudo update-alternatives --set x86_64-w64-mingw32-g++ /usr/bin/x86_64-w64-mingw32-g++-posix
    fi
fi

# Godot
if [[ ! -d "$PROJECT_DIR/externals/godot" ]]; then
    mkdir -p "$PROJECT_DIR/externals"
    cd "$PROJECT_DIR/externals"
    git clone https://github.com/godotengine/godot.git
fi

if [[ -n "$ARG_GODOT_VERSION" ]]; then
    cd "$PROJECT_DIR/externals/godot"
    git fetch origin "$ARG_GODOT_VERSION"
    git pull
    git checkout "$ARG_GODOT_VERSION"
fi

cd "$PROJECT_DIR"

if [[ -n "$ARG_LOG" ]]; then
    MAKE_ARGS+=" LOG=true"
fi

if [[ -n "$ARG_DEBUG" ]]; then
    MAKE_ARGS+=" DEBUG=true"
fi

if [[ -n "$ARG_CLEAN" ]]; then
    make clean
fi

if [[ -n $ARG_UNIX ]]; then
    make $MAKE_ARGS
fi

if [[ -n $ARG_WINDOWS ]]; then
    make $MAKE_ARGS WINDOWS=true
fi

cd "$PROJECT_DIR/externals/godot/bin/"

#./godot.linuxbsd.editor.x86_64 --export-release godot.linuxbsd.template_release.x86_64 "$PROJECT_DIR/client/SparkHolders.exe"

cd "$SAVED_DIR"
