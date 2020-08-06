# allows to add DEPLOYMENTFOLDERS and links to the Felgo library and QtCreator auto-completion
CONFIG += felgo

QT += websockets multimedia-private multimedia

PRODUCT_IDENTIFIER = com.bunistack.VideoCall
PRODUCT_VERSION_NAME = 1.0.0
PRODUCT_VERSION_CODE = 1

qmlFolder.source = qml
#DEPLOYMENTFOLDERS += qmlFolder

assetsFolder.source = assets
#DEPLOYMENTFOLDERS += assetsFolder

RESOURCES += resources.qrc

SOURCES += main.cpp \
    vcaudio.cpp \
    vcfilter.cpp \
    vcvideoimage.cpp \
    vcwebsocketmanager.cpp


android {
    QT += androidextras

    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
    OTHER_FILES += android/AndroidManifest.xml       android/build.gradle
}

ios {
    QMAKE_INFO_PLIST = ios/Project-Info.plist
    OTHER_FILES += $$QMAKE_INFO_PLIST
}

# set application icons for win and macx
win32 {
    RC_FILE += win/app_icon.rc
}
macx {
    ICON = macx/app_icon.icns
}

HEADERS += \
    vcaudio.h \
    vcfilter.h \
    vcvideoimage.h \
    vcwebsocketmanager.h
