QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    login.cpp \
    main.cpp \
    emailclient.cpp \
    sendtouser.cpp

HEADERS += \
    emailclient.h \
    login.h \
    sendtouser.h

FORMS += \
    emailclient.ui \
    login.ui \
    sendtouser.ui

LIBS += -lSmtpMime2

unix|win32: LIBS += -L$$PWD/../build-SMTPEmail-Desktop_Qt_6_6_2_MinGW_64_bit-Release/release -lSmtpMime2

INCLUDEPATH += $$PWD/../build-SMTPEmail-Desktop_Qt_6_6_2_MinGW_64_bit-Release
DEPENDPATH += $$PWD/../build-SMTPEmail-Desktop_Qt_6_6_2_MinGW_64_bit-Release

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../build-SMTPEmail-Desktop_Qt_6_6_2_MinGW_64_bit-Release/release/SmtpMime2.dll
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/../build-SMTPEmail-Desktop_Qt_6_6_2_MinGW_64_bit-Release/release/libSmtpMime2.a

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
