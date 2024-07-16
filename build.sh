#!/bin/bash -e

SAVED_DIR=$(pwd)
PROJECT_DIR=$(dirname "$0")
PROJECT_DIR=$(readlink -f "$PROJECT_DIR")

function usage() {
    column -t -s "@" <<<"""
usage: $0 [options]
  -c, --clean@Clean before building.
  -l, --log@Enable logger.
  -d, --debug@Build with debugging symbols.
  -v [ver], --version [ver]@Select a speficic Godot version.
  -h, --help@Display this.
"""
    exit 1
}

unset ARG_CLEAN
unset ARG_DEBUG
unset ARG_GODOT_VERSION
unset ARG_LOG
unset MAKE_ARGS

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
    -h | --help | *) shift
        usage ;;
    esac
done

if [[ ! -d "$PROJECT_DIR/externals/godot" ]]; then
    cd "$PROJECT_DIR/externals"
    git clone https://github.com/godotengine/godot.git
fi

if [[ -n "$ARG_GODOT_VERSION" ]]; then
    cd "$PROJECT_DIR/externals/godot"
    git fetch origin "$ARG_GODOT_VERSION"
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
make ${MAKE_ARGS:1}
make ${MAKE_ARGS:1} WINDOWS=true

cd "$PROJECT_DIR/externals/godot/bin/"

#./godot.linuxbsd.editor.x86_64 --export-release godot.linuxbsd.template_release.x86_64 "$PROJECT_DIR/client/SparkHolders.exe"

cd "$SAVED_DIR"