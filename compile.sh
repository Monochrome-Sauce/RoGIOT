#!/bin/bash

BUILD_DIR=build
COMMANDS_DIR=$(mktemp -d)

mkdir -p ${BUILD_DIR}
cmake ./CMakeLists.txt -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -B${BUILD_DIR}
cd ${BUILD_DIR}

mv ./compile_commands.json $COMMANDS_DIR/compile_commands.json
compdb -p $COMMANDS_DIR list > compile_commands.json
rm -r $COMMANDS_DIR

make
cd ..
