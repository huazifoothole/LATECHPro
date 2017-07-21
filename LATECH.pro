######################################################################
# Automatically generated by qmake (2.01a) Mon Feb 25 09:21:35 2013
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH +=
#QT +=sql

# Input
HEADERS += hscanner.h printer.h scanner.h windows.h \
    scannerthread.h \
    hscannerthread.h \
    videoplay.h \
    imageplay.h \
    hwinfoquery.h \
    zdybranddig.h \
    cthread.h \
    iccard.h \
    iccardlib.h \
    dialogmagcardlogin.h \
    dialogmagcard.h
FORMS += windows.ui \
    videoplay.ui \
    imageplay.ui \
    zdybranddig.ui \
    iccard.ui \
    dialogmagcardlogin.ui \
    dialogmagcard.ui
SOURCES += hscanner.cpp main.cpp printer.cpp scanner.cpp windows.cpp \
    scannerthread.cpp \
    hscannerthread.cpp \
    videoplay.cpp \
    imageplay.cpp \
    hwinfoquery.cpp \
    cthread.cpp \
    zdybranddig.cpp \
    iccard.cpp \
    iccardlib.cpp \
    dialogmagcardlogin.cpp \
    dialogmagcard.cpp

RESOURCES += \
    imagesRC.qrc \
    images2.qrc


unix:!macx:!symbian: LIBS += -L$$PWD/../../../../../usr/lib/qt4/plugins/designer/

INCLUDEPATH += $$PWD/../../../../../usr/lib/qt4/plugins/designer
DEPENDPATH += $$PWD/../../../../../usr/lib/qt4/plugins/designer
