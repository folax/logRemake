QT += widgets

CONFIG += c++11

win32 {
QMAKE_CXXFLAGS = -Wall -Wextra -Werror
QMAKE_CFLAGS = -Wall -Wextra -Werror
}

SOURCES += \
    main.cpp \
    logremake.cpp

HEADERS += \
    logremake.h
