cmake_minimum_required(VERSION 3.20)


set(LIB_OUTPUT rogiot)
set(LIB_TESTER test.out)


# ------------------------------------------------------------------------------
# Static library
set(SOURCE_LIST
	proj/rogiot/src/[api]_rgt.c
	proj/rogiot/src/mainloop.c
)

add_library(${LIB_OUTPUT} STATIC ${SOURCE_LIST})

target_include_directories(${LIB_OUTPUT}
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/proj/
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

target_include_directories(${LIB_TESTER}
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/proj/
)

target_link_libraries(${LIB_TESTER}
	${LIB_OUTPUT}
)

target_compile_options(${LIB_TESTER}
	PRIVATE -g -std=c17 -Werror -O1
	PRIVATE -Wall -Wextra -Wsign-compare -Wconversion
)
