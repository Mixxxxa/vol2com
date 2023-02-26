cmake_minimum_required(VERSION 3.22)

project(vol2com VERSION 0.8
                DESCRIPTION "Open source tool to manage LED strips"
                HOMEPAGE_URL "https://github.com/Mixxxxa/vol2com"
                LANGUAGES CXX
)

set(FETCHCONTENT_QUIET OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_DEBUG Off)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_MINSIZEREL ON)

################################################################################
# Includes
################################################################################
include(FetchContent)

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
  URL_HASH SHA1=7709EC0FF565319493AF835FB9BE7486C581C5D8
  DOWNLOAD_EXTRACT_TIMESTAMP ON
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
)

FetchContent_Declare(bass_wasapi
  URL https://www.un4seen.com/files/basswasapi24.zip
  URL_HASH SHA1=53685BD9BDD379F4F7F1BA4E1AE2AAA1636B3ACE
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
                        QuickControls2
                        SerialPort
                        #QuickTemplates2
)

################################################################################
# Compilers setup
################################################################################
if (MSVC)
    add_compile_options(/Zi
                        /nologo
#                        /W4
#                        /WX
                        /diagnostics:classic
                        "$<$<CONFIG:DEBUG>:/Od>"
                        "$<$<CONFIG:RELEASE>:/O2>"
                        "$<$<CONFIG:RELEASE>:/Ob2>"
                        "$<$<CONFIG:RELEASE>:/Oi>"
                        "$<$<CONFIG:RELEASE>:/Ot>"
                        "$<$<CONFIG:RELEASE>:/Oy>"
                        /EHa
                        /GS
                        /fp:precise
                        /Zc:wchar_t
                        /Zc:forScope
                        /Zc:inline
                        /openmp-
                        /Gd
                        /FC
                        /errorReport:prompt
                        /bigobj
                        /wd4324
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
add_subdirectory(src)
