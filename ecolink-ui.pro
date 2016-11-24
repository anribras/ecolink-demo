#-------------------------------------------------
#
# Project created by QtCreator 2016-07-11T14:52:29
#
#-------------------------------------------------

QT       += core gui
CONFIG += link_pkgconfig
PKGCONFIG += gstreamer-0.10
#CONFIG   += console
#PGKCONFIG += gstvideo-0.10


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ecolink.bin
TEMPLATE = app

#/home/wwp/MentorGraphics/Sourcery_CodeBench_for_ARM_Embedded/arm-none-linux-gnueabi/libc/usr/include
INCPATH += $$(D27_SYSROOT)/usr/include/gstreamer-0.10
INCPATH += $$(D27_SYSROOT)/usr/include/glib-2.0
INCPATH += $$(D27_SYSROOT)/usr/include/libxml2
INCPATH += $$(D27_SYSROOT)/usr/lib/glib-2.0/include
INCPATH += $$(D27_SYSROOT)/arm-none-linux-gnueabi/include
INCPATH += $$(D27_SYSROOT)/arm-none-linux-gnueabi/include/c++/4.8.3
INCPATH += $$(D27_SYSROOT)/arm-none-linux-gnueabi/include/c++/4.8.3/arm-none-linux-gnueabi
INCPATH += $$(D27_SYSROOT)/arm-none-linux-gnueabi/include/c++/4.8.3/backward
INCPATH += $$(D27_SYSROOT)/arm-none-linux-gnueabi/include/c++/4.8.3/arm-none-linux-gnueabi/armv7-a-hard
INCPATH += $$(D27_SYSROOT)/arm-none-linux-gnueabi/libc/usr/include
LIBS += -Wl,-rpath-link=$$(D27_TOOLCHAIN_ROOT)/arm-none-linux-gnueabi/lib -Wl,-rpath-link=$$(D27_SYSROOT)/usr/lib
LIBS += -Wl,-rpath-link=$$(D27_SYSROOT)/lib 
LIBS += -L$$(D27_SYSROOT)/usr/lib  -L$$(D27_SYSROOT)/lib


LIBS += -L. -lplist -lecolink  -lgstvideo-0.10 -lgstapp-0.10 -lglib-2.0 -lgmodule-2.0 -lgthread-2.0 -lgobject-2.0 -lffi -lgstpbutils-0.10 -lgstvideo-0.10 -lgstbase-0.10 -lgstaudio-0.10 -lgsttag-0.10 -lcrypto -lfbcontrol 


LIBS += -L$$PWD/../socketlib/lib -lHSSocket

INCLUDEPATH += $$PWD/../socketlib/inc
DEPENDPATH += $$PWD/../socketlib/lib

HEADERS += \
    ../socketlib/inc/hs_socket_app.h \
    ../socketlib/inc/socket_driver.h \
    client.h


SOURCES += main.cpp\
       	   debug.cpp\
           mainwindow.cpp\
       	   stream.cpp\
		   demo.cpp\
           newpushbutton.cpp \
    client.cpp

HEADERS  += mainwindow.h\
			debug.h\
			stream.h\
		   	sdk.h\
		   	demo.h\
            newpushbutton.h

FORMS    += mainwindow.ui
