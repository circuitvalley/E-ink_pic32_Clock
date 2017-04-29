QT += widgets serialport

TARGET = TimeProgrammer
TEMPLATE = app

HEADERS += \
    dialog.h \
    masterthread.h

SOURCES += \
    main.cpp \
    dialog.cpp \
    masterthread.cpp

target.path = ./
INSTALLS += target
