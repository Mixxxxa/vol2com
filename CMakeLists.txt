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
                        /Wall        # enable all errors
                        /WX          # treat all warnings as errors
                        /external:W0 # disable compiler warnings in external code
                        /permissive- # strict standards conformance
                        /GS          # enable buffer security check
                        /FC          # full path of source code file in diagnostics
                        /Zc:inline   # remove unreferenced functions or data
                        "$<$<CONFIG:DEBUG>:/Od>"
                        "$<$<CONFIG:RELEASE>:/O2>"
                        /wd4061 # enumerator in switch of enum is not explicitly handled by a case label
                        /wd4464 # relative include path contains '..' (because moc files)
                        /wd4625 # copy constructor was implicitly defined as deleted
                        /wd4626 # assignment operator was implicitly defined as deleted
                        /wd4702 # unreachable code (because can't suppress them on QtQml)
                        /wd4820 # n bytes padding added after data member
                        /wd4866 # compiler may not enforce left-to-right evaluation order for call to
                        /wd5026 # move constructor was implicitly defined as deleted
                        /wd5027 # move assignment operator was implicitly defined as deleted
                        /wd5045 # compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
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
