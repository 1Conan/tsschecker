cmake_minimum_required(VERSION 3.19...3.24 FATAL_ERROR)
#SET(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(ignoreMe "${NO_PKGCFG} ${ASAN} ${ASAN_FLAGS} ${ARCH}")
option(SUBPROJECT_BUILD "Enables submodules to build as statically internally linked libs instead of binaries/tools" OFF)
option(ASAN "Enable AddressSanatizer" OFF)
option(NO_PKGCFG "Disable pkgconfig searching of libs, use dep_root for linking" OFF)
if(NOT DEFINED VERSION_COMMIT_COUNT)
	execute_process(COMMAND git rev-list --count HEAD WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}" OUTPUT_VARIABLE VERSION_COMMIT_COUNT ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()
if(NOT DEFINED VERSION_COMMIT_SHA)
	execute_process(COMMAND git rev-parse HEAD WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}" OUTPUT_VARIABLE VERSION_COMMIT_SHA ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()

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
		set(PROJECT_C_FLAGS "-arch ${ARCH} -fembed-bitcode=off -pthread -mrelax-all -Wno-deprecated-declarations -Wno-int-conversion")
		if(NOT "${CMAKE_BUILD_TYPE}" MATCHES "Debug")
			set(PROJECT_C_FLAGS "-flto=thin ${PROJECT_C_FLAGS}")
		endif()
	elseif("${CMAKE_HOST_SYSTEM_NAME}" MATCHES "Linux")
		set(PROJECT_C_FLAGS "-Wl,--allow-multiple-definition -pthread -mrelax-all -Wno-deprecated-declarations -Wno-int-conversion")
	elseif("${CMAKE_HOST_SYSTEM_NAME}" MATCHES "MSYS" OR "${CMAKE_HOST_SYSTEM_NAME}" MATCHES "Windows")
		set(PROJECT_C_FLAGS "-Wl,--allow-multiple-definition -pthread -fPIC -Wno-deprecated-declarations -Wno-int-conversion -DCURL_STATICLIB -Bstatic -O0")
	endif()

	project(tsschecker VERSION 0.${VERSION_COMMIT_COUNT}.0 LANGUAGES C)

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

	set(CMAKE_C_FLAGS_MINSIZEREL_INIT "-flto=thin ${ADDITIONAL_C_FLAGS} ${PROJECT_C_FLAGS} ${CMAKE_C_FLAGS_MINSIZEREL_INIT}")
	set(CMAKE_C_FLAGS_RELEASE_INIT "-flto=thin ${ADDITIONAL_C_FLAGS} ${PROJECT_C_FLAGS} ${CMAKE_C_FLAGS_RELEASE_INIT}")
	set(CMAKE_C_FLAGS_RELWITHDEBINFO_INIT "-flto=thin ${ADDITIONAL_C_FLAGS} ${PROJECT_C_FLAGS} ${CMAKE_C_FLAGS_RELWITHDEBINFO_INIT}")
	set(CMAKE_C_FLAGS_DEBUG_INIT "-g -O0 ${ADDITIONAL_C_FLAGS} ${PROJECT_C_FLAGS} ${CMAKE_C_FLAGS_DEBUG_INIT} ${ASAN_FLAGS}")

	set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL_INIT}")
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE_INIT}")
	set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO_INIT}")
	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG_INIT}")

	set(CMAKE_C_STANDARD 17)

	add_library(tsschecker STATIC SHARED
			tsschecker/tsschecker.c
			tsschecker/download.c
			tsschecker/common.c
			tsschecker/tss.c
			tsschecker/main.c)
else()
	project(tsschecker VERSION 0.${VERSION_COMMIT_COUNT}.0 LANGUAGES C)
	add_library(tsschecker STATIC
			tsschecker/tsschecker.c
			tsschecker/download.c)
endif()

target_include_directories(tsschecker PRIVATE
		"${CMAKE_SOURCE_DIR}/dep_root/include/"
		tsschecker
		external/jssy/jssy)

if(NOT NO_PKGCFG)
	find_package(PkgConfig REQUIRED FATAL_ERROR)
	pkg_check_modules(DEPS REQUIRED
			libplist-2.0
			libirecovery-1.0
			libfragmentzip)
	target_include_directories(tsschecker PRIVATE "${DEPS_INCLUDE_DIRS}")
else()
	target_link_directories(tsschecker PRIVATE
			"${CMAKE_SOURCE_DIR}/dep_root/lib/")
	target_link_libraries(tsschecker PRIVATE
			curl
			plist-2.0
			irecovery-1.0
			fragmentzip
			usbmuxd-2.0
			imobiledevice-glue-1.0
			imobiledevice-1.0
			z
			jssy)
	if("${CMAKE_HOST_SYSTEM_NAME}" MATCHES "Darwin")
		target_link_libraries(tsschecker PRIVATE
				compression
				"-framework CoreFoundation"
				"-framework IOKit")
	endif()
endif()

#set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)
add_definitions(
		-DTSSCHECKER_VERSION_COUNT="${VERSION_COMMIT_COUNT}"
		-DTSSCHECKER_VERSION_SHA="${VERSION_COMMIT_SHA}"
		-DCMAKE_BUILD=1)
add_subdirectory(external)

if(NOT SUBPROJECT_BUILD)
	set_target_properties(tsschecker PROPERTIES OUTPUT_NAME "tsschecker.0")
	install(TARGETS tsschecker
			DESTINATION "${CMAKE_INSTALL_PREFIX}/lib")
else()
	add_definitions(-DTSSCHECKER_NOMAIN=1)
endif()

#get_cmake_property(_variableNames VARIABLES)
#list (SORT _variableNames)
#foreach (_variableName ${_variableNames})
#	message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()
