#!/bin/bash

if ! [ $(command -v cmake) ]
then
	printf ">>> Could not locate $(tput setaf 1)CMake$(tput sgr0). Aborting...\n"
	exit 1
fi


hasCompdb=$(command -v compdb)

BUILD_DIR="./build"
COMMANDS_DIR=$(mktemp -d)
COMPILE_COMMANDS_FILE="compile_commands.json"

mkdir -p ${BUILD_DIR}
cmake ./CMakeLists.txt -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -B$BUILD_DIR
cd $BUILD_DIR

if [ $hasCompdb ]
then
	mv ./$COMPILE_COMMANDS_FILE $COMMANDS_DIR/$COMPILE_COMMANDS_FILE
	compdb -p $COMMANDS_DIR list > $COMPILE_COMMANDS_FILE
	rm -r $COMMANDS_DIR
fi

make
cd ..
printf "\nBuild files location:\n"
printf ">>> $(tput setaf 3)%b$(tput sgr0)\n" $(readlink -f $BUILD_DIR)

if ! [ $hasCompdb ]
then
	printf "Note:\n"
	printf ">>> failed to create $(tput setaf 3)%b$(tput sgr0) - $(tput setaf 1)compdb$(tput sgr0) was not found.\n" $COMPILE_COMMANDS_FILE
fi
