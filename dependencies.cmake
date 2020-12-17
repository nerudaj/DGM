set ( DSH_VERSION  "1.6.1" )
set ( SFML_VERSION "2.5.1" )
set ( JSON_VERSION "3.7.3" )

set ( DSH_URL "https://github.com/nerudaj/dsh/releases/download/v${DSH_VERSION}/dsh-${DSH_VERSION}-vc16-64-bit.zip" )
set ( SFML_URL "https://github.com/SFML/SFML/releases/download/${SFML_VERSION}/SFML-${SFML_VERSION}-windows-vc15-64-bit.zip" )
set ( JSON_URL "https://github.com/nlohmann/json/releases/download/v${JSON_VERSION}/json.hpp" )

# fetch dependencies
include ( FetchContent )

## dsh
#function (fetch_dependencies)
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
#endfunction(fetch_dependencies)