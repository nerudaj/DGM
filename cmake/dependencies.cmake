set ( DSH_VERSION   "1.6.1" )
set ( SFML_VERSION  "2.5.1" )
set ( JSON_VERSION  "3.7.3" )
set ( CATCH_VERSION "2.10.2" )

set ( DSH_URL   "https://github.com/nerudaj/dsh/releases/download/v${DSH_VERSION}/dsh-${DSH_VERSION}-vc16-64-bit.zip" )
set ( SFML_URL  "https://github.com/SFML/SFML/releases/download/${SFML_VERSION}/SFML-${SFML_VERSION}-windows-vc15-64-bit.zip" )
set ( JSON_URL  "https://github.com/nlohmann/json/releases/download/v${JSON_VERSION}/json.hpp" )
set ( CATCH_URL "https://github.com/catchorg/Catch2/releases/download/v${CATCH_VERSION}/catch.hpp" )

# Download dependencies
include ( FetchContent )

## dsh
FetchContent_Declare ( dsh
	URL "${DSH_URL}"
)
FetchContent_GetProperties(dsh)
if (NOT dsh_POPULATED)
	FetchContent_Populate(dsh)
endif()

## sfml
FetchContent_Declare ( sfml
	URL "${SFML_URL}"
)
FetchContent_GetProperties(sfml)
if (NOT sfml_POPULATED)
	FetchContent_Populate(sfml)
endif()

## json
FetchContent_Declare ( json
	URL                   "${JSON_URL}"
	DOWNLOAD_NO_PROGRESS  TRUE
	DOWNLOAD_NO_EXTRACT   TRUE
)
FetchContent_GetProperties(json)
if (NOT json_POPULATED)
	FetchContent_Populate(json)
endif()

## catch2
FetchContent_Declare ( catch
	URL                   "${CATCH_URL}"
	DOWNLOAD_NO_PROGRESS  TRUE
	DOWNLOAD_NO_EXTRACT   TRUE
)
FetchContent_GetProperties(catch)
if (NOT catch_POPULATED)
	FetchContent_Populate(catch)
endif()

# Shorthands for folders with downloaded data
set ( DSH_FOLDER   "${dsh_SOURCE_DIR}" )
set ( SFML_FOLDER  "${sfml_SOURCE_DIR}" )
set ( JSON_FOLDER  "${CMAKE_CURRENT_BINARY_DIR}/_deps/json-subbuild/json-populate-prefix/src" )
set ( CATCH_FOLDER "${CMAKE_CURRENT_BINARY_DIR}/_deps/catch-subbuild/catch-populate-prefix/src" )

# Make libraries visible to cmake linker
link_directories("${DSH_FOLDER}/lib")
link_directories("${SFML_FOLDER}/lib")

# dsh is internally bundled with DGM while SFML is dynamically linked
find_library(LIB_CFG_D config-d  NAMES config-d.lib  HINTS "${DSH_FOLDER}/lib")
find_library(LIB_STR_D strings-d NAMES strings-d.lib HINTS "${DSH_FOLDER}/lib")
find_library(LIB_LVLD_D leveld-d NAMES leveld-d.lib HINTS "${DSH_FOLDER}/lib")

find_library(LIB_CFG_R config  NAMES config.lib  HINTS "${DSH_FOLDER}/lib")
find_library(LIB_STR_R strings NAMES strings.lib HINTS "${DSH_FOLDER}/lib")
find_library(LIB_LVLD_R leveld NAMES leveld.lib HINTS "${DSH_FOLDER}/lib")

set(LIB_CFG optimized ${LIB_CFG_R} debug ${LIB_CFG_D})
set(LIB_STR optimized ${LIB_STR_R} debug ${LIB_STR_D})
set(LIB_LVLD optimized ${LIB_LVLD_R} debug ${LIB_LVLD_D})
