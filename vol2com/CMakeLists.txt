cmake_minimum_required(VERSION 3.22)

qt_add_library(vol2com_lib STATIC)

qt_add_qml_module(vol2com_lib
    URI vol2com
    VERSION 1.0
    QML_FILES
        qml/main.qml
        qml/ConnectView.qml
        qml/SerialDelegate.qml
        qml/FadeMode.qml
        qml/GeneralMode.qml
        qml/AiMode.qml
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
        traymenu.h
        traymenu.cpp
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
        workmodes/fademode.cpp
        workmodes/fademode.h
        workmodes/generalmode.cpp
        workmodes/generalmode.h
        workmodes/aimode.cpp
        workmodes/aimode.h
        workmodes/manualmode.cpp
        workmodes/manualmode.h
        workmodes/randommode.cpp
        workmodes/randommode.h
        workmodes/workmodewithselector.cpp
        workmodes/workmodewithselector.h
        workmodes/genredetector.h
        workmodes/genredetector.cpp
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
        res/modes/ai.svg
)

target_link_libraries(vol2com_lib PRIVATE
    Qt::Core
    Qt::Widgets
    Qt::Gui
    Qt::Quick
    Qt::QuickControls2
    Qt::QuickTemplates2
    Qt::SerialPort
    Qt::Qml
    bass_lib
    basswasapi_lib
)

target_compile_options(vol2com_lib PRIVATE
    /Wall        # enable all errors
    /WX          # treat all warnings as errors
    /external:W0 # disable compiler warnings in external code
    /wd4061 # enumerator in switch of enum is not explicitly handled by a case label
    /wd4464 # relative include path contains '..' (because moc files)
    /wd4625 # copy constructor was implicitly defined as deleted
    /wd4626 # assignment operator was implicitly defined as deleted
    /wd4702 # unreachable code (because can't suppress them on QtQml)
    /wd4710 # function not inlined
    /wd4711 # function selected for automatic inline expansion
    /wd4820 # n bytes padding added after data member
    /wd4866 # compiler may not enforce left-to-right evaluation order for call to
    /wd5026 # move constructor was implicitly defined as deleted
    /wd5027 # move assignment operator was implicitly defined as deleted
    /wd5045 # compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
)

#qt_generate_deploy_qml_app_script(
#    TARGET appvol2com
#    FILENAME_VARIABLE deploy_script
#)
#install(SCRIPT ${deploy_script})

target_compile_definitions(vol2com_lib PRIVATE
    NOMINMAX
)

target_include_directories(vol2com_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/viewmodels
    ${CMAKE_CURRENT_SOURCE_DIR}/gui
    ${CMAKE_CURRENT_SOURCE_DIR}/datamodels
    ${CMAKE_CURRENT_SOURCE_DIR}/workmodes
    ${bass_SOURCE_DIR}/c
    ${bass_wasapi_SOURCE_DIR}/c
)
