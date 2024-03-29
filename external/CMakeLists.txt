cmake_minimum_required(VERSION 3.19...3.24 FATAL_ERROR)
set(ignoreMe "${NO_PKGCFG} ${ASAN} ${ASAN_FLAGS} ${ARCH}")
option(SUBPROJECT_BUILD "Enables submodules to build as statically internally linked libs instead of binaries/tools" OFF)
option(ASAN "Enable AddressSanatizer" OFF)
option(NO_PKGCFG "Disable pkgconfig searching of libs, use dep_root for linking" OFF)
execute_process(COMMAND git rev-list --count HEAD WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}" OUTPUT_VARIABLE JSSY_VERSION_COMMIT_COUNT ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
if(NOT SUBPROJECT_BUILD)
	if(ARCH STREQUAL "")
		set(ARCH "${CMAKE_SYSTEM_PROCESSOR}")
	endif()

	if("${CMAKE_HOST_SYSTEM_NAME}" MATCHES "Darwin")
		if((NOT DEFINED MINVER) AND NO_PKGCFG)
			if("${ARCH}" STREQUAL "x86_64")
				set(CMAKE_OSX_DEPLOYMENT_TARGET 10.12)
			else()
				set(CMAKE_OSX_DEPLOYMENT_TARGET 11.0)
			endif()
		endif()
		if(DEFINED MINVER)
			set(CMAKE_OSX_DEPLOYMENT_TARGET "${MINVER}")
		endif()
		set(PROJECT_C_FLAGS "-arch ${ARCH} -fembed-bitcode=off -flto=thin -pthread -mrelax-all -Wno-deprecated-declarations -Wno-int-conversion")
		set(PROJECT_CXX_FLAGS "${PROJECT_C_FLAGS}")
	elseif("${CMAKE_HOST_SYSTEM_NAME}" MATCHES "Linux")
		set(PROJECT_C_FLAGS "-Wl,--allow-multiple-definition -pthread -mrelax-all -Wno-deprecated-declarations -Wno-int-conversion")
		set(PROJECT_CXX_FLAGS "${PROJECT_C_FLAGS}")
	elseif("${CMAKE_HOST_SYSTEM_NAME}" MATCHES "MSYS" OR "${CMAKE_HOST_SYSTEM_NAME}" MATCHES "Windows")
		set(PROJECT_C_FLAGS "-Wl,--allow-multiple-definition -pthread -fPIC -Wno-deprecated-declarations -Wno-int-conversion -DCURL_STATICLIB -Bstatic -O0")
		set(PROJECT_CXX_FLAGS "${PROJECT_C_FLAGS}")
	endif()

	project(jssy VERSION 0.${JSSY_VERSION_COMMIT_COUNT}.0 LANGUAGES C)

	if(ASAN)
		set(ASAN ON)
		set(ASAN_FLAGS "-fsanitize=address -fsanitize-address-use-after-scope -fno-omit-frame-pointer ${ASAN_FLAGS}")
		set(CMAKE_C_LINK_LIBRARY_FLAG "${CMAKE_C_LINK_LIBRARY_FLAG} ${ASAN_FLAGS}")
		set(CMAKE_MACOSX_RPATH 1)
		set(CMAKE_INSTALL_RPATH_USE_LINK_PATH 1)
		set(CMAKE_BUILD_RPATH
				"/usr/local/lib"
				"/usr/lib"
				"/opt/procursus/lib"
				"/Library/Developer/CommandLineTools/usr/lib/clang/14.0.0/lib/darwin"
				"/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/14.0.0/lib/darwin/"
				"/Applications/Xcode_14.1.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/14.0.0/lib/darwin/"
				"/Applications/Xcode_14.2.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/14.0.0/lib/darwin/")
		#hardcoded because actions never updates xcode
	else()
		set(ASAN_FLAGS "")
	endif()

	set(CMAKE_C_FLAGS_MINSIZEREL_INIT "${ADDITIONAL_C_FLAGS} ${PROJECT_C_FLAGS} ${CMAKE_C_FLAGS_MINSIZEREL_INIT}")
	set(CMAKE_C_FLAGS_RELEASE_INIT "${ADDITIONAL_C_FLAGS} ${PROJECT_C_FLAGS} ${CMAKE_C_FLAGS_RELEASE_INIT}")
	set(CMAKE_C_FLAGS_RELWITHDEBINFO_INIT "${ADDITIONAL_C_FLAGS} ${PROJECT_C_FLAGS} ${CMAKE_C_FLAGS_RELWITHDEBINFO_INIT}")
	set(CMAKE_C_FLAGS_DEBUG_INIT "${ADDITIONAL_C_FLAGS} ${PROJECT_C_FLAGS} ${CMAKE_C_FLAGS_DEBUG_INIT} ${ASAN_FLAGS}")

	set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL_INIT}")
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE_INIT}")
	set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO_INIT}")
	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG_INIT}")

	set(CMAKE_C_STANDARD 17)
else()
	project(jssy VERSION 0.${JSSY_VERSION_COMMIT_COUNT}.0 LANGUAGES C)
endif()

add_library(jssy
		jssy/jssy/jssy.c
		jssy/jssy/jssy_create.c)
target_include_directories(jssy PRIVATE
		"${CMAKE_SOURCE_DIR}/dep_root/include")
target_link_directories(jssy PRIVATE
		"${CMAKE_SOURCE_DIR}/dep_root/lib")
