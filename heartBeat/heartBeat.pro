TEMPLATE = app
TARGET = heartBeat

load(ubuntu-click)

QT += qml quick bluetooth

SOURCES += main.cpp \
    heartsensor.cpp \
    devicemanager.cpp \
    deviceinfo.cpp

RESOURCES += \
    resources.qrc

QML_FILES += $$files(*.qml,true) \
             $$files(*.js,true)

CONF_FILES +=  heartBeat.apparmor \
               heartBeat.png

AP_TEST_FILES += tests/autopilot/run \
                 $$files(tests/*.py,true)

#show all the files in QtCreator
OTHER_FILES += $${CONF_FILES} \
               $${QML_FILES} \
               $${AP_TEST_FILES} \
               heartBeat.desktop

#specify where the config files are installed to
config_files.path = /heartBeat
config_files.files += $${CONF_FILES}
INSTALLS+=config_files

#install the desktop file, a translated version is 
#automatically created in the build directory
desktop_file.path = /heartBeat
desktop_file.files = $$OUT_PWD/heartBeat.desktop
desktop_file.CONFIG += no_check_exist
INSTALLS+=desktop_file

# Default rules for deployment.
target.path = $${UBUNTU_CLICK_BINARY_PATH}
INSTALLS+=target

HEADERS += \
    heartsensor.h \
    devicemanager.h \
    deviceinfo.h

DISTFILES += \
    ScanView.qml \
    MonitoringView.qml \
    items/HeartBeatBox.qml \
    items/BatteryBox.qml

