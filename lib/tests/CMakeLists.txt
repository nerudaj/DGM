cmake_minimum_required ( VERSION 3.19 )

project ( lib-testrunner )

include_directories ( ${CATCH_FOLDER} )

set ( SOURCES
	Main.cpp
	Math.cpp
)

add_executable ( ${PROJECT_NAME}
	${SOURCES}
)
target_link_libraries ( ${PROJECT_NAME} 
	libdgm
)

add_test (
	NAME 	${PROJECT_NAME}
	COMMAND ${PROJECT_NAME}
)
