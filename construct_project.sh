#!/bin/bash

Usage()
{
	printf "Bash script providing more efficient usage running CMake with Make for C++ project construction.\n"
	printf "The script runs CMake's output into a build directory and exports compile commands for LSP.\n"
	printf "\nUsage:\n"
	printf "\tbash ${0} [options]\n"
	
	printf "\nOptions:\n"
	printf "\t-h|--help                         : Print usage information and exit.\n"
	printf "\t-b|--build=<path/to/build/folder> : Choose a different build directory.\n"
	printf "\t-c|--compile                      : Also compile the project by calling Make.\n"
}


BUILD_DIR="./build"
SHOULD_COMPILE=0
ProcessOpts()
{
	local failure=0
	for i in $@; do
		case ${i} in
			-h|--help)
				Usage
				exit 0
				;;
			-b=*|--build-dir=*)
				BUILD_DIR="${i#*=}"
				shift # past argument=value
				;;
			-b|--build)
				printf "Must provide a directory for "
				failure=1
				break
				;;
			-c|--compile)
				SHOULD_COMPILE=1
				shift # past argument
				;;
			-c=*|--compile=*)
				printf "Arguments may not be passed for "
				failure=1
				break
				;;
			-*|--*|*)
				printf "Unknown argument: "
				failure=1
				break
				;;
		esac
	done
	
	if [[ failure -ne 0 ]]; then
		printf "$(tput setaf 3)${i}$(tput sgr0)\n"
		printf "Run '$(tput setaf 3)${0} --help$(tput sgr0)' for all supported options.\n"
		exit 1
	fi
}



if [[ ! $(command -v cmake) ]]
then
	printf "Could not locate $(tput setaf 3)CMake$(tput sgr0). Aborting...\n"
	exit 1
fi
HAS_COMPDB=$(command -v compdb)

ProcessOpts $@


COMP_COMMS_DIR=$(mktemp -d)
COMP_COMMS_FILE="compile_commands.json"

mkdir -p ${BUILD_DIR}
cd ${BUILD_DIR}

cmake ../CMakeLists.txt -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -B./
if [[ ${HAS_COMPDB} ]]; then
	mv "./${COMP_COMMS_FILE}" "${COMP_COMMS_DIR}/${COMP_COMMS_FILE}"
	compdb -p "${COMP_COMMS_DIR}" list > "${COMP_COMMS_FILE}"
	rm -r ${COMP_COMMS_DIR}
fi

if [[ ${SHOULD_COMPILE} -ne 0 ]]; then
	make
fi


printf "\nBuild files location:\n"
printf ">>> $(tput setaf 3)%b$(tput sgr0)\n" $(pwd)

if [[ ! ${HAS_COMPDB} ]]; then
	printf "Note: failed to create $(tput setaf 3)%b$(tput sgr0) - $(tput setaf 1)compdb$(tput sgr0) was not found.\n" "${COMP_COMMS_FILE}"
fi
