cmake_minimum_required(VERSION 3.22)

qt_add_executable(appvol2com
    main.cpp
    resource.rc
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


        datamodels/workmodesmodel.cpp
        datamodels/workmodesmodel.h
        gui/abstractbandviewer.cpp
        gui/abstractbandviewer.h


        #systemevents.cpp
        #systemevents.h
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

        utils/boundedvalue.h
        utils/boundedvalue.cpp
        utils/math.hpp
        utils/gitinfo.h

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
