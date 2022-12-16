cmake_minimum_required(VERSION 3.22)
set(CMAKE_C_STANDARD 17)
set(ignoreMe "${NO_PKGCFG}")
set(ignoreMe "${ASAN}")
set(ignoreMe "${ARCH}")
if(NOT DEFINED SUBPROJECT_BUILD)
	set(ASAN_FLAG "")
	if(DEFINED ASAN OR "$ENV{ASAN}" MATCHES "1")
		set(ASAN 1)
		set(ASAN_FLAG -fsanitize=address)
		set(CMAKE_C_LINK_LIBRARY_FLAG "${ASAN_FLAG}")
	endif()
	if("${CMAKE_HOST_SYSTEM_NAME}" MATCHES "Darwin")
		if(NOT DEFINED NO_XCODE AND NOT DEFINED ENV{NO_XCODE})
			execute_process(COMMAND xcrun --find clang WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}" OUTPUT_VARIABLE CC OUTPUT_STRIP_TRAILING_WHITESPACE)
			execute_process(COMMAND xcrun --find ar WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}" OUTPUT_VARIABLE AR OUTPUT_STRIP_TRAILING_WHITESPACE)
			execute_process(COMMAND xcrun --find ld WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}" OUTPUT_VARIABLE LD OUTPUT_STRIP_TRAILING_WHITESPACE)
			execute_process(COMMAND xcrun --show-sdk-path WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}" OUTPUT_VARIABLE SYSROOT OUTPUT_STRIP_TRAILING_WHITESPACE)
			set(CMAKE_C_COMPILER "${CC}")
		endif()
		if(NOT DEFINED ARCH)
			set(ARCH "$ENV{ARCH}")
			if(ARCH STREQUAL "")
				set(ARCH "${CMAKE_SYSTEM_PROCESSOR}")
			endif()
		endif()
		if(NOT DEFINED MINVER OR NOT DEFINED "$ENV{MINVER}" AND DEFINED NO_PKGCFG AND "$ENV{NO_PKGCFG}" MATCHES "1")
			if("${ARCH}" STREQUAL "x86_64" OR "$ENV{ARCH}" STREQUAL "x86_64")
				set(MINVER -mmacosx-version-min=10.12)
				set(CMAKE_OSX_DEPLOYMENT_TARGET 10.12)
			else()
				set(MINVER -mmacosx-version-min=11.0)
				set(CMAKE_OSX_DEPLOYMENT_TARGET 11.0)
			endif()
		endif()
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -arch ${ARCH} -isysroot ${SYSROOT} ${MINVER} -fembed-bitcode=off -flto=thin -pthread -mrelax-all -std=gnu17 ${ASAN_FLAG}")
	elseif("${CMAKE_HOST_SYSTEM_NAME}" MATCHES "Linux")
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,--allow-multiple-definition -pthread -mrelax-all -std=gnu17 ${ASAN_FLAG}")
	endif()
endif()
set(CMAKE_C_FLAGS_RELEASE "-Os -DNDEBUG")
set(CMAKE_C_FLAGS_DEBUG "-g -O0 -DDEBUG")
set(CMAKE_C_STANDARD 17)
set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)
add_library(tsschecker STATIC
		tsschecker/tsschecker.c
		tsschecker/download.c)
if(NOT DEFINED SUBPROJECT_BUILD)
	add_library(tsschecker_shared SHARED
			tsschecker/tsschecker.c
			tsschecker/download.c
			tsschecker/common.c
			tsschecker/tss.c
			tsschecker/main.c)
endif()
target_include_directories(tsschecker PRIVATE
		"${CMAKE_SOURCE_DIR}/dep_root/include/"
		tsschecker
		external/jssy/jssy)
if(NOT DEFINED SUBPROJECT_BUILD)
	target_include_directories(tsschecker_shared PRIVATE
			"${CMAKE_SOURCE_DIR}/dep_root/include/"
			tsschecker
			external/jssy/jssy)
endif()
target_link_directories(tsschecker PRIVATE
		"${CMAKE_SOURCE_DIR}/dep_root/lib/")
if(NOT DEFINED SUBPROJECT_BUILD)
	target_link_directories(tsschecker_shared PRIVATE
			"${CMAKE_SOURCE_DIR}/dep_root/lib/"
			.)
endif()
if(NOT DEFINED NO_PKGCFG AND NOT "$ENV{NO_PKGCFG}" MATCHES "1")
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(DEPS REQUIRED
			libplist-2.0
			libirecovery-1.0
			libfragmentzip)
	target_include_directories(tsschecker PRIVATE "${DEPS_INCLUDE_DIRS}")
else()
	if(NOT DEFINED SUBPROJECT_BUILD)
		target_link_libraries(tsschecker_shared PRIVATE
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
			target_link_libraries(tsschecker_shared PRIVATE
					compression
					"-framework CoreFoundation"
					"-framework IOKit")
		endif()
	endif()
endif()
add_definitions(-DTSSCHECKER_NOMAIN=1)
project(tsschecker)
add_subdirectory(external)
if(NOT DEFINED SUBPROJECT_BUILD)
	set_target_properties(tsschecker_shared PROPERTIES OUTPUT_NAME "tsschecker.0")
	if(NOT DEFINED VERSION_COMMIT_COUNT)
		execute_process(COMMAND git rev-list --count HEAD WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}" OUTPUT_VARIABLE VERSION_COMMIT_COUNT ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
	endif()
	if(NOT DEFINED VERSION_COMMIT_SHA)
		execute_process(COMMAND git rev-parse HEAD WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}" OUTPUT_VARIABLE VERSION_COMMIT_SHA ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
	endif()
	add_definitions(
			-DTSSCHECKER_VERSION_COUNT="${VERSION_COMMIT_COUNT}"
			-DTSSCHECKER_VERSION_SHA="${VERSION_COMMIT_SHA}")
	install(TARGETS tsschecker
			DESTINATION "${CMAKE_INSTALL_PREFIX}/lib")
endif()

