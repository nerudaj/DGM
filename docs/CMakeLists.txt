cmake_minimum_required ( VERSION 3.19 )

project ( lib-docs )

include ( FindDoxygen )
find_package( Doxygen )

configure_file (
	${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
	${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
)

add_custom_target ( doxygen-docs
	COMMAND
		${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
	WORKING_DIRECTORY
		${CMAKE_CURRENT_BINARY_DIR}
)

# Install
install (
	DIRECTORY    ${CMAKE_CURRENT_BINARY_DIR}/html
	DESTINATION  docs
)

install (
	FILES        ${CMAKE_CURRENT_SOURCE_DIR}/quickstart.md
	DESTINATION  docs
)

install (
	FILES        ${CMAKE_CURRENT_SOURCE_DIR}/buffer.md
	DESTINATION  docs
)