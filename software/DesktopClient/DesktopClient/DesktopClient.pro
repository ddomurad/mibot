#-------------------------------------------------
#
# Project created by QtCreator 2015-06-13T22:36:45
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DesktopClient
TEMPLATE = app
CONFIG += c++11

DEFINES += SHOW_SPLASH

SOURCES += main.cpp\
        ./Forms/MainWindow.cpp \
    ./Forms/OSMWidget.cpp \
    ./Forms/SettingsForm.cpp \
    ./Settings/AppSettings.cpp \
    ./Forms/RouteEditorForm.cpp \
    ./Forms/VideoWidget.cpp \
    ./Forms/CtrlPanelForm.cpp \
    ./Clients/GlobalPasswordProvider.cpp \
    ./Forms/InfoServiceForm.cpp \
    ./Clients/Socket.cpp \
    ./Forms/LoggerDialog.cpp \
    ./Clients/mibJSONProtocol.cpp \
    ./Clients/InfoServiceClient.cpp \
    ./Forms/SensorsReaderForm.cpp \
    ./Forms/RoverConnectionForm.cpp \
    ./Clients/RoverSensorClient.cpp \
    ./Clients/RoverClient.cpp \
    ./Clients/RoverClientsProvider.cpp \
    Forms/SoundSignallerForm.cpp \
    Forms/ReferenceCounter.cpp \
    Other/JsInput.cpp \
    Clients/RoverDriveClient.cpp \
    Forms/RoverDriveForm.cpp \
    Forms/PlotFactoryForm.cpp \
    Forms/DataSourceProvider.cpp \
    Forms/Plots/DataSource.cpp \
    Forms/Plots/LinePlot2Form.cpp \
    Forms/Plots/LinePlotForm.cpp \
    Forms/VideoStreamerCtrlForm.cpp \
    Clients/VideoServiceClient.cpp \
    Clients/PictureClient.cpp \
    Forms/VideoReceiverForm.cpp \
    Forms/GpsPreviewForm.cpp \
    Forms/RecordingForm.cpp \
    Other/Recorder.cpp \
    Other/Replayer.cpp \
    Forms/SettingsEditorForm.cpp \
    Other/TrackProvider.cpp \
    Clients/RoverAutopilotClient.cpp \
    Forms/PictureViewForm.cpp

HEADERS  += ./Forms/MainWindow.h \
    ./Forms/OSMWidget.h \
    ./Forms/SettingsForm.h \
    ./Settings/AppSettings.h \
    ./Forms/RouteEditorForm.h \
    ./Forms/VideoWidget.h \
    ./Forms/CtrlPanelForm.h \
    ./Clients/GlobalPasswordProvider.h \
    ./Forms/InfoServiceForm.h \
    ./Clients/Socket.h \
    ./Forms/LoggerDialog.h \
    ./Clients/mibJSONProtocol.h \
    ./Clients/InfoServiceClient.h \
    ./Forms/SensorsReaderForm.h \
    ./Forms/RoverConnectionForm.h \
    ./Clients/RoverSensorClient.h \
    ./Clients/RoverClient.h \
    ./Clients/RoverClientsProvider.h \
    Forms/SoundSignallerForm.h \
    Forms/ReferenceCounter.h \
    Other/JsInput.h \
    Clients/RoverDriveClient.h \
    Forms/RoverDriveForm.h \
    Forms/PlotFactoryForm.h \
    Forms/DataSourceProvider.h \
    Forms/Plots/DataSource.h \
    Forms/Plots/LinePlot2Form.h \
    Forms/Plots/LinePlotForm.h \
    Forms/VideoStreamerCtrlForm.h \
    Clients/VideoServiceClient.h \
    Clients/PictureClient.h \
    Forms/VideoReceiverForm.h \
    Forms/GpsPreviewForm.h \
    Forms/RecordingForm.h \
    Other/Recorder.h \
    Other/Replayer.h \
    Forms/SettingsEditorForm.h \
    Other/TrackProvider.h \
    Clients/RoverAutopilotClient.h \
    Forms/PictureViewForm.h

FORMS    += ./Forms/MainWindow.ui \
    ./Forms/SettingsForm.ui \
    ./Forms/RouteEditorForm.ui \
    ./Forms/CtrlPanelForm.ui \
    ./Forms/InfoServiceForm.ui \
    ./Forms/SensorsReaderForm.ui \
    ./Forms/RoverConnectionForm.ui \
    Forms/SoundSignallerForm.ui \
    Forms/RoverDriveForm.ui \
    Forms/PlotFactoryForm.ui \
    Forms/VideoStreamerCtrlForm.ui \
    Forms/VideoReceiverForm.ui \
    Forms/RecordingForm.ui \
    Forms/SettingsEditorForm.ui \
    Forms/PictureViewForm.ui

RESOURCES += \
    resources.qrc

LIBS += -lavcodec
LIBS += -lavutil
LIBS += -lswscale
