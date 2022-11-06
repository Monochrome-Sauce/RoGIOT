cmake_minimum_required(VERSION 3.20)


set(LIB_OUTPUT rogiot)
set(LIB_TESTER test.out)

set(SOURCE_LIST
	proj/src/rogiot_init.c
)


# ------------------------------------------------------------------------------
# Static library
add_library(${LIB_OUTPUT} STATIC ${SOURCE_LIST})

target_include_directories(${LIB_OUTPUT}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/proj/include
)

target_link_libraries(${LIB_OUTPUT}
	m pthread
)

target_compile_options(${LIB_OUTPUT}
	PRIVATE -std=c17 -Werror -O1
	PRIVATE -Wall -Wextra -Wsign-compare -Wconversion
)



# ------------------------------------------------------------------------------
# Testing executable
project(${LIB_TESTER})
add_executable(${LIB_TESTER} ${SOURCE_LIST} proj/main.c)

target_link_libraries(${LIB_TESTER}
	${LIB_OUTPUT}
)

target_compile_options(${LIB_TESTER}
	PRIVATE -std=c17 -Werror -O1
	PRIVATE -Wall -Wextra -Wsign-compare -Wconversion
)
