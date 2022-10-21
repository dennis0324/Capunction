# libUIOHook: Cross-platform keyboard and mouse hooking from userland.
# Copyright (C) 2006-2022 Alexander Barker.  All Rights Reserved.
# https://github.com/kwhat/libuiohook/
#
# libUIOHook is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published
# by the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# libUIOHook is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.


# this program is only for window can compile at window, and linux(ubuntu)

cmake_minimum_required(VERSION 3.10)

project(uiohook VERSION 1.2.0 LANGUAGES C)

# this line make complie as mingw32 or mingw64 in ubuntu

set(CMAKE_SYSTEM_NAME Windows)
set(TOOLCHAIN_PREFIX x86_64-w64-mingw32)

# cross compilers to use for C, C++ and Fortran
set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++)
set(CMAKE_Fortran_COMPILER ${TOOLCHAIN_PREFIX}-gfortran)
set(CMAKE_RC_COMPILER ${TOOLCHAIN_PREFIX}-windres)

# target environment on the build host system
set(CMAKE_FIND_ROOT_PATH /usr/${TOOLCHAIN_PREFIX})

# modify default behavior of FIND_XXX() commands
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

enable_language("RC")
set(APP_ICON_RESOURCE_WINDOWS "src/appicon.rc")

if (WIN32 OR WIN64)
    set(UIOHOOK_SOURCE_DIR "windows")
elseif (APPLE)
    set(UIOHOOK_SOURCE_DIR "darwin")
else()
    set(UIOHOOK_SOURCE_DIR "windows")
endif()

add_library(uiohook SHARED
    "src/logger.c"
    "src/${UIOHOOK_SOURCE_DIR}/input_helper.c"
    "src/${UIOHOOK_SOURCE_DIR}/input_hook.c"
    "src/${UIOHOOK_SOURCE_DIR}/post_event.c"
    "src/${UIOHOOK_SOURCE_DIR}/system_properties.c"
)

set_target_properties(uiohook PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED ON
    POSITION_INDEPENDENT_CODE 1
    OUTPUT_NAME "${PROJECT_NAME}"
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    PUBLIC_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/include/uiohook.h
)

include(GNUInstallDirs)
target_include_directories(uiohook
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>

    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/${UIOHOOK_SOURCE_DIR}
)


install(TARGETS uiohook
    EXPORT ${PROJECT_NAME}-config
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)

export(TARGETS uiohook FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake")
install(EXPORT ${PROJECT_NAME}-config DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})


################# this is for function keyboard for window 
add_library(combinationInput SHARED
    "src/function_keyboard/combinationInput.c"
)

set_target_properties(combinationInput PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED ON
    POSITION_INDEPENDENT_CODE 1
    OUTPUT_NAME "combinationInput"
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    PUBLIC_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/include/combinationInput.h
)

include(GNUInstallDirs)
target_include_directories(combinationInput
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>

    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/function_keyboard
)


install(TARGETS combinationInput
    EXPORT combinationInput-config
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)
####################


add_executable(demo_hook
    "./src/main/personal_tray.c"
    "./src/main/hook.c"
    "./src/main/main.c"
    ${APP_ICON_RESOURCE_WINDOWS}
    
)
add_dependencies(demo_hook uiohook combinationInput)
add_dependencies(combinationInput uiohook)
target_link_libraries(demo_hook uiohook combinationInput "${CMAKE_THREAD_LIBS_INIT}")
target_link_libraries(combinationInput uiohook "${CMAKE_THREAD_LIBS_INIT}")

add_custom_target(all_demos DEPENDS
        demo_hook
    )

set_target_properties(all_demos PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED ON
)

# this is for compiling at window 
if(MSVC)
add_compile_definitions(all_demos PRIVATE inline=__inline)
add_compile_definitions(all_demos PRIVATE _CRT_SECURE_NO_WARNINGS)

    if (MSVC_VERSION LESS "1900")
        add_compile_definitions(all_demos PRIVATE snprintf=_snprintf)
    endif()
endif()

if(UNIX AND NOT APPLE)
    find_package(PkgConfig REQUIRED)
    target_link_libraries(uiohook)
    if(PUBLISH)
        target_link_options(demo_hook PRIVATE -mwindows)
    endif()
elseif(APPLE)
        message("do not support apple")
elseif(WIN32)
    target_link_libraries(uiohook Advapi32)
    if(PUBLISH)
        target_link_options(demo_hook PRIVATE -mwindows)
    endif()
endif()


list(REMOVE_DUPLICATES INTERFACE_LINK_LIBRARIES)
string(REPLACE ";" " " COMPILE_LIBRARIES "${INTERFACE_LINK_LIBRARIES}")
configure_file("pc/uiohook.pc.in" "${PROJECT_BINARY_DIR}/uiohook.pc" @ONLY)
install(FILES "${PROJECT_BINARY_DIR}/uiohook.pc" DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")