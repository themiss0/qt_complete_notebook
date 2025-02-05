QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aboutdialog.cpp \
    codeeditor.cpp \
    codehighlighter.cpp \
    favdialog.cpp \
    highlightrulereader.cpp \
    labeldialog.cpp \
    main.cpp \
    mainwindow.cpp \
    replacedialog.cpp \
    searchdialog.cpp  \
    idatabase.cpp

HEADERS += \
    BookMark.h \
    aboutdialog.h \
    codeeditor.h \
    codehighlighter.h \
    favdialog.h \
    highlightingrule.h \
    highlightrulereader.h \
    labeldialog.h \
    mainwindow.h \
    replacedialog.h \
    searchdialog.h  \
    idatabase.h

FORMS += \
    aboutdialog.ui \
    favdialog.ui \
    labeldialog.ui \
    mainwindow.ui \
    replacedialog.ui \
    searchdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc


RC_ICONS = note.ico
