cmake_minimum_required(VERSION 3.22)

project(vol2com VERSION 0.8
                DESCRIPTION "Open source tool to manage LED strips"
                HOMEPAGE_URL "https://github.com/Mixxxxa/vol2com"
                LANGUAGES CXX
)

set(FETCHCONTENT_QUIET OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_DEBUG Off)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_MINSIZEREL ON)

################################################################################
# Includes
################################################################################
include(FetchContent)
include(cmake/gitinfo.cmake)

################################################################################
# Libraries
################################################################################
#FetchContent_Declare(
#    wintoast
#    GIT_REPOSITORY https://github.com/mohabouje/WinToast.git
#    GIT_TAG        5e441fd03543b999edb663caf8df7be37c0d575c # Merge pull request #61 from agashlin/disable-create-shortcut
#)

FetchContent_Declare(bass
  URL https://www.un4seen.com/files/bass24.zip
  URL_HASH SHA1=0696189F72FB360E535FC7C3E07D29AF5C7D0E6D
  DOWNLOAD_EXTRACT_TIMESTAMP ON
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
)

FetchContent_Declare(bass_wasapi
  URL https://www.un4seen.com/files/basswasapi24.zip
  URL_HASH SHA1=F1F2C0ADCAA905406CE6B50F43A57DEFE10C20D8
  DOWNLOAD_EXTRACT_TIMESTAMP ON
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
)

FetchContent_MakeAvailable(bass bass_wasapi)

ADD_LIBRARY(bass_lib SHARED IMPORTED)
SET_TARGET_PROPERTIES(bass_lib PROPERTIES
    IMPORTED_IMPLIB ${bass_SOURCE_DIR}/c/x64/bass.lib
    IMPORTED_LOCATION ${bass_SOURCE_DIR}/x64/bass.dll
)

ADD_LIBRARY(basswasapi_lib SHARED IMPORTED)
SET_TARGET_PROPERTIES(basswasapi_lib PROPERTIES
    IMPORTED_IMPLIB ${bass_wasapi_SOURCE_DIR}/c/x64/basswasapi.lib
    IMPORTED_LOCATION ${bass_wasapi_SOURCE_DIR}/x64/basswasapi.dll
)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
set(QtX Qt${QT_VERSION_MAJOR})
find_package(${QtX} REQUIRED
                    COMPONENTS
                        Gui
                        Quick
                        Qml
                        QuickControls2
                        SerialPort
                        #QuickTemplates2
)

################################################################################
# Compilers setup
################################################################################
if (MSVC)
    add_compile_options(/Zi          # generate separate .pdb file
                        /nologo      # disable MSVC logo
                        /permissive- # strict standards conformance
                        /GS          # enable buffer security check
                        /FC          # full path of source code file in diagnostics
                        /Zc:inline   # remove unreferenced functions or data
                        "$<$<CONFIG:DEBUG>:/Od>"
                        "$<$<CONFIG:RELEASE>:/O2>"
    )
    if (NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        add_compile_options(/MP
                            /Gm-
        )
    endif()
    add_link_options(/DEBUG
                     $<$<CONFIG:RELEASE>:/OPT:REF>
                     $<$<CONFIG:RELEASE>:/OPT:ICF>)
endif()

################################################################################
# Targets setup
################################################################################
qt_standard_project_setup(REQUIRES 6.5)

add_subdirectory(vol2com)

###########################################################################

qt_add_executable(appvol2com
    main.cpp
    resource.rc
)

target_link_libraries(appvol2com PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Quick
    Qt::QuickControls2
    Qt::QuickTemplates2
    Qt::SerialPort
    Qt::Qml
    vol2com_libplugin
)

set_target_properties(appvol2com PROPERTIES
    #MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appvol2com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_include_directories(appvol2com PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if (WIN32)
    # Copy only bass and bass_wasapi dlls.
    # Otherwise app is crashing on startup because not all Qt's libs copied by TARGET_RUNTIME_DLLS
    add_custom_command(TARGET appvol2com POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_PROPERTY:basswasapi_lib,IMPORTED_LOCATION> $<TARGET_FILE_DIR:appvol2com>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_PROPERTY:bass_lib,IMPORTED_LOCATION> $<TARGET_FILE_DIR:appvol2com>
            COMMAND_EXPAND_LISTS
    )
endif()

install(FILES $<TARGET_PROPERTY:bass_lib,IMPORTED_LOCATION>
              $<TARGET_PROPERTY:basswasapi_lib,IMPORTED_LOCATION>
        DESTINATION ${CMAKE_INSTALL_BINDIR}
)
