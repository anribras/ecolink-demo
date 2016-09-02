#-------------------------------------------------
#
# Project created by QtCreator 2016-07-11T14:52:29
#
#-------------------------------------------------

QT       += core gui
CONFIG += link_pkgconfig
PKGCONFIG += gstreamer-1.0
PGKCONFIG += gstvideo-1.0


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ecolink.bin
TEMPLATE = app


LIBS += -L. -lplist -lecolink -lcrypto -lgstvideo-1.0 -lgstapp-1.0 -lglib-2.0 -lgmodule-2.0 -lgthread-2.0 -lgobject-2.0 -lffi -lgstpbutils-1.0 -lgstvideo-1.0 -lgstbase-1.0 -lgstaudio-1.0 -lgsttag-1.0 


SOURCES += main.cpp\
       	   debug.cpp\
           mainwindow.cpp\
       	   stream.cpp\
		   demo.cpp\

HEADERS  += mainwindow.h\
			debug.h\
			stream.h\
		   	sdk.h\
		   	demo.h\

FORMS    += mainwindow.ui
