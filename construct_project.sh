#!/bin/bash

Usage()
{
	printf 'Bash script providing more efficient usage running CMake with Make for C++ project construction.\n'
	printf 'The script stores the CMake files into a build directory and exports compile commands for LSP.\n'
	printf '\nUsage:\n'
	printf '\tbash %s [options]\n' "${0}"
	
	PrintOption()
	{
		printf '\t%s \n\t\t%s\n' "${1}" "${2}"
	}
	
	printf '\nOptions:\n'
	PrintOption '-h|--help' 'Print usage information and exit.'
	PrintOption '-b|--build=<path/to/build/folder>' 'Choose a different build directory.'
	PrintOption '-c|--compile' 'Compile the project by calling Make.'
}


ProcessOpts()
{
	BUILD_DIR='./build'
	SHOULD_COMPILE=false
	
	local failure=false
	local arg
	for arg in "$@"; do
		case ${arg} in
			-h|--help)
				Usage
				exit 0
				;;
			-b=*|--build-dir=*)
				BUILD_DIR="${arg#*=}"
				if [[ ${BUILD_DIR} = '' ]]; then
					printf "Must provide a directory for "
					failure=true
					break
				fi
				shift # past argument=value
				;;
			-b|--build)
				printf "Must provide a directory for "
				failure=true
				break
				;;
			-c|--compile)
				SHOULD_COMPILE=true
				shift # past argument
				;;
			-c=*|--compile=*)
				printf "Arguments may not be passed for "
				failure=true
				break
				;;
			-*|*)
				printf "Unknown argument: "
				failure=true
				break
				;;
		esac
	done
	
	if [[ ${failure} = true ]]; then
		printf "$(tput setaf 3)%s$(tput sgr0)\n" "${arg}"
		printf "Run '$(tput setaf 3)%s --help$(tput sgr0)' for all supported options.\n" "${0}"
		exit 1
	fi
	
	readonly SHOULD_COMPILE
	readonly BUILD_DIR
}



if [[ ! $(command -v cmake) ]]
then
	echo "Could not locate $(tput setaf 3)CMake$(tput sgr0). Aborting..."
	exit 1
fi

HAS_COMPDB=$(command -v compdb)
readonly HAS_COMPDB

ProcessOpts "${@}"


COMP_COMMS_DIR=$(mktemp -d)
COMP_COMMS_FILE="compile_commands.json"

mkdir -p "${BUILD_DIR}"
cd "${BUILD_DIR}" || exit 1

cmake ../CMakeLists.txt -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -B./
if [[ ${HAS_COMPDB} ]]; then
	mv "./${COMP_COMMS_FILE}" "${COMP_COMMS_DIR}/${COMP_COMMS_FILE}"
	compdb -p "${COMP_COMMS_DIR}" list > "${COMP_COMMS_FILE}"
	rm -r "${COMP_COMMS_DIR}"
fi

if [[ ${SHOULD_COMPILE} = true ]]; then
	make
fi


printf '\nBuild files location:\n'
printf ">>> $(tput setaf 3)%s$(tput sgr0)\n" "$(pwd)"

if [[ ! ${HAS_COMPDB} ]]; then
	printf "Note: failed to create $(tput setaf 3)%s$(tput sgr0) - $(tput setaf 1)compdb$(tput sgr0) was not found.\n" "${COMP_COMMS_FILE}"
fi
