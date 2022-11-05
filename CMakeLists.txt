cmake_minimum_required(VERSION 3.20)


set(OUTPUT_EXE a.out)

set(SOURCE_LIST
	proj/main.c
)


project(${OUTPUT_EXE})
add_executable(${OUTPUT_EXE} ${SOURCE_LIST})


target_include_directories(${OUTPUT_EXE}
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/proj
)

target_link_libraries(${OUTPUT_EXE}
	m pthread
)

target_compile_options(${OUTPUT_EXE}
	PRIVATE -std=c17 -Werror -O1
	PRIVATE -Wall -Wextra -Wsign-compare -Wconversion
)
