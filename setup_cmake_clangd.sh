#!/bin/bash

BUILD_DIR=$(mktemp -d)
cmake ./CMakeLists.txt -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

mv ./compile_commands.json $BUILD_DIR/compile_commands.json
compdb -p $BUILD_DIR list > compile_commands.json

rm -r $BUILD_DIR
