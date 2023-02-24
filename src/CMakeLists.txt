cmake_minimum_required(VERSION 3.22)

project(vol2com VERSION 0.1 
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

include(FetchContent)
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
FetchContent_MakeAvailable(bass)
FetchContent_MakeAvailable(bass_wasapi)

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
                        /wd4324)
    if (NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        add_compile_options(/MP
                            /Gm-
        )
    endif()
    add_link_options(/DEBUG
                     $<$<CONFIG:RELEASE>:/OPT:REF>
                     $<$<CONFIG:RELEASE>:/OPT:ICF>)
endif()

qt_add_executable(appvol2com
    main.cpp
)

set_target_properties(appvol2com PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appvol2com PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Quick
    Qt::QuickControls2
    Qt::SerialPort
    bass_lib
    basswasapi_lib
)
if(UNIX AND NOT APPLE AND CMAKE_CROSSCOMPILING)
    find_package(Qt6 COMPONENTS QuickTemplates2)
     # Work around QTBUG-86533
    target_link_libraries(appvol2com PRIVATE Qt::QuickTemplates2)
endif()

qt_add_qml_module(appvol2com
    URI vol2com
    VERSION 1.0
    QML_FILES
        qml/main.qml
        qml/ConnectView.qml
        qml/SerialDelegate.qml
        qml/ChristmasMode.qml
        qml/FadeMode.qml
        qml/GeneralMode.qml
        qml/ManualMode.qml
        qml/WorkModeSelector.qml
        qml/WorkModeView.qml
        qml/AboutView.qml
        qml/EqualizerView.qml
        qml/MainView.qml
        qml/Settings.qml
        qml/ColorButton.qml
        qml/GradientSlider.qml
        qml/Header.qml
        qml/TitleBarButton.qml
        qml/ListDelegate.qml
        qml/TitleBar.qml
        qml/VerticalToolBar.qml
        qml/StandartButton.qml
        qml/RandomMode.qml
        qml/StandartView.qml
        qml/EpilepsyWarningPopup.qml

    SOURCES
        appstyle.cpp
        appstyle.h
        connectmethodbase.h connectmethodbase.cpp
        controller.cpp
        controller.h
        datamodels/basicqmlmodel.h datamodels/basicqmlmodel.cpp
        equalizer.cpp
        equalizer.h
        gui/bandselector.cpp
        gui/bandselector.h
        gui/equalizerviewer.cpp
        gui/equalizerviewer.h
        serialconnector.h serialconnector.cpp
        serialworker.h serialworker.cpp
        settings.cpp
        settings.h
        viewmodels/aboutpageviewmodel.cpp
        viewmodels/aboutpageviewmodel.h
        viewmodels/connectpageviewmodel.cpp
        viewmodels/connectpageviewmodel.h
        viewmodels/equalizerviewmodel.cpp
        viewmodels/equalizerviewmodel.h
        viewmodels/serialviewmodel.cpp
        viewmodels/serialviewmodel.h
        viewmodels/toolbarviewmodel.cpp
        viewmodels/toolbarviewmodel.h
        viewmodels/workmodeviewmodel.cpp
        viewmodels/workmodeviewmodel.h
        workmodes/workmodebase.h

        basslibwrapper.cpp
        basslibwrapper.h
        boundedvalue.cpp
        boundedvalue.h
        datamodels/workmodesmodel.cpp
        datamodels/workmodesmodel.h
        gui/abstractbandviewer.cpp
        gui/abstractbandviewer.h
        guihelper.cpp
        guihelper.h


        #systemevents.cpp
        #systemevents.h
        translator.cpp
        translator.h
        utility.h
        v2c.h
        v2cbase.cpp
        v2cbase.h
        workmodes/christmasmode.cpp
        workmodes/christmasmode.h
        workmodes/fademode.cpp
        workmodes/fademode.h
        workmodes/generalmode.cpp
        workmodes/generalmode.h
        workmodes/manualmode.cpp
        workmodes/manualmode.h
        workmodes/randommode.cpp
        workmodes/randommode.h
        workmodes/submodebase.h
        workmodes/workmodewithselector.cpp
        workmodes/workmodewithselector.h
        workmodesfactory.cpp
        workmodesfactory.h

    RESOURCES
        res/icon-active.svg
        res/icon-unactive.svg
        res/icon-active.ico
        res/icon-unactive.ico
        res/modes/christmas.svg
        res/modes/fade.svg
        res/modes/fireplace.svg
        res/modes/gaming.svg
        res/modes/general.svg
        res/modes/manual.svg
        res/modes/party.svg
        res/modes/random.svg
)

install(TARGETS appvol2com
    #RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

#FetchContent_Declare(
#    wintoast
#    GIT_REPOSITORY https://github.com/mohabouje/WinToast.git
#    GIT_TAG        5e441fd03543b999edb663caf8df7be37c0d575c # Merge pull request #61 from agashlin/disable-create-shortcut
#)

#FetchContent_MakeAvailable(wintoast)

#add_library(wintoast_lib STATIC 
#    ${wintoast_SOURCE_DIR}/src/wintoastlib.h
#    ${wintoast_SOURCE_DIR}/src/wintoastlib.cpp
#)

#target_include_directories(
#    appvol2com 
#    PUBLIC ${CMAKE_SOURCE_DIR}/thirdparty/somelib/include
#)

target_compile_definitions(appvol2com PRIVATE
    NOMINMAX
)
target_include_directories(appvol2com PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/viewmodels
    ${CMAKE_CURRENT_SOURCE_DIR}/gui
    ${CMAKE_CURRENT_SOURCE_DIR}/datamodels
    ${CMAKE_CURRENT_SOURCE_DIR}/workmodes
    ${bass_SOURCE_DIR}/c
    ${bass_wasapi_SOURCE_DIR}/c
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
