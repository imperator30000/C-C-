QT       += core gui
include(path/to/datavisualization/datavisualization.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += core gui widgets datavisualization

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    plot.cpp \
    plotsurface.cpp \
    sincdistancefromzero.cpp \
    sincxsincz.cpp \
    tools.cpp

HEADERS += \
    mainwindow.h \
    plot.h \
    plotsurface.h \
    sincdistancefromzero.h \
    sincxsincz.h \
    tools.h

FORMS += \
    mainwindow.ui
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
