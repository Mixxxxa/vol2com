QT += qml serialport quickcontrols2

CONFIG += c++17 qmltypes qtquickcompiler

QML_IMPORT_NAME = vol2com
QML_IMPORT_MAJOR_VERSION = 1

# Get version and branch from Git
GIT_COMMIT=$$system(git --git-dir $$PWD/../.git --work-tree $$PWD/../ describe --always --tags)
GIT_BRANCH=$$system(git branch --show-current)

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS \
           _SILENCE_CLANG_COROUTINE_MESSAGE \
           NOMINMAX \
           GIT_COMMIT=\\\"$${GIT_COMMIT}\\\" \
           GIT_BRANCH=\\\"$${GIT_BRANCH}\\\"

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    datamodels/basicqmlmodel.h \
    datamodels/workmodesmodel.h \
    gui/abstractbandviewer.h \
    gui/bandselector.h \
    gui/equalizerviewer.h \
    libs/WinToast/wintoastlib.h \
    viewmodels/aboutpageviewmodel.h \
    viewmodels/connectpageviewmodel.h \
    viewmodels/equalizerviewmodel.h \
    viewmodels/serialviewmodel.h \
    viewmodels/toolbarviewmodel.h \
    viewmodels/workmodeviewmodel.h \
    workmodes/christmasmode.h \
    workmodes/fademode.h \
    workmodes/generalmode.h \
    workmodes/manualmode.h \
    workmodes/randommode.h \
    workmodes/submodebase.h \
    workmodes/workmodebase.h \
    workmodes/workmodewithselector.h \
    appstyle.h \
    basslibwrapper.h \
    boundedvalue.h \
    connectmethodbase.h \
    controller.h \
    equalizer.h \
    guihelper.h \
    serial.h \
    serialport.h \
    settings.h \
    systemevents.h \
    translator.h \
    utility.h \
    v2c.h \
    v2cbase.h \
    workmodesfactory.h

SOURCES += \
    datamodels/basicqmlmodel.cpp \
    datamodels/workmodesmodel.cpp \
    gui/abstractbandviewer.cpp \
    gui/bandselector.cpp \
    gui/equalizerviewer.cpp \
    libs/WinToast/wintoastlib.cpp \
    viewmodels/aboutpageviewmodel.cpp \
    viewmodels/connectpageviewmodel.cpp \
    viewmodels/equalizerviewmodel.cpp \
    viewmodels/serialviewmodel.cpp \
    viewmodels/toolbarviewmodel.cpp \
    viewmodels/workmodeviewmodel.cpp \
    workmodes/christmasmode.cpp \
    workmodes/fademode.cpp \
    workmodes/generalmode.cpp \
    workmodes/manualmode.cpp \
    workmodes/randommode.cpp \
    workmodes/workmodewithselector.cpp \
    appstyle.cpp \
    basslibwrapper.cpp \
    boundedvalue.cpp \
    controller.cpp \
    equalizer.cpp \
    guihelper.cpp \
    main.cpp \
    serial.cpp \
    serialport.cpp \
    settings.cpp \
    systemevents.cpp \
    translator.cpp \
    v2cbase.cpp \
    workmodesfactory.cpp

RESOURCES += qml.qrc

TRANSLATIONS += \
    translations/$${TARGET}_ru_RU.ts

DISTFILES += \
    resource.rc

RC_FILE += \
    resource.rc

INCLUDEPATH += \
    $$shell_quote($$shell_path($${_PRO_FILE_PWD_}/libs/bass/c)) \
    $$shell_quote($$shell_path($${_PRO_FILE_PWD_}/libs/basswasapi/c))

RESULT_DIR = $$OUT_PWD/result

win32: {
    contains(QMAKE_TARGET.arch, x86_64): {
        basslibfiles.files = \
            $$shell_quote($$shell_path($${_PRO_FILE_PWD_}/libs/bass/x64/bass.dll)) \
            $$shell_quote($$shell_path($${_PRO_FILE_PWD_}/libs/bass_wasapi/x64/basswasapi.dll))
        LIBS += \
            $$shell_quote($$shell_path($${_PRO_FILE_PWD_}/libs/bass/c/x64/bass.lib)) \
            $$shell_quote($$shell_path($${_PRO_FILE_PWD_}/libs/bass_wasapi/c/x64/basswasapi.lib))
    } else {
        basslibfiles.files = \
            $$shell_quote($$shell_path($${_PRO_FILE_PWD_}/libs/bass/bass.dll)) \
            $$shell_quote($$shell_path($${_PRO_FILE_PWD_}/libs/bass_wasapi/basswasapi.dll))
        LIBS += \
            $$shell_quote($$shell_path($${_PRO_FILE_PWD_}/libs/bass/c/bass.lib)) \
            $$shell_quote($$shell_path($${_PRO_FILE_PWD_}/libs/bass_wasapi/c/basswasapi.lib))
    }
}
basslibfiles.path = $$RESULT_DIR

translation.files = \
    $$shell_quote($$shell_path($${_PRO_FILE_PWD_}/translations/$${TARGET}_ru_RU.qm))
translation.path = $$RESULT_DIR/translations

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = $$PWD/qml

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target  

# Auto deploy release
CONFIG(release, debug|release): {
    # Copy result exe
    DLLDESTDIR += $$RESULT_DIR
    # Disable debug output
    #DEFINES += QT_NO_DEBUG_OUTPUT
    # Copy libs
    INSTALLS += basslibfiles
    INSTALLS += translation
    # Prepare paths
    DEPLOY_APP = $$shell_quote($$shell_path($$(QTDIR)/bin/windeployqt.exe))
    TARGET_EXE = $$shell_quote($$shell_path($${OUT_PWD}/release/$${TARGET}.exe))
    QML_SEARCH_PATH = $$shell_quote($$shell_path($${QML_IMPORT_PATH}))
    DEPLOY_DIR = $$shell_quote($$shell_path($$RESULT_DIR))
    # Run windeployqt
    QMAKE_POST_LINK += $${DEPLOY_APP} $${TARGET_EXE} --qmldir $${QML_SEARCH_PATH} --dir $${DEPLOY_DIR} --translations en,ru --release
}
