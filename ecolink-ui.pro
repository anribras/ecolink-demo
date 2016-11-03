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


LIBS += -L. -lplist -lecolink  -lgstvideo-1.0 -lgstapp-1.0 -lglib-2.0 -lgmodule-2.0 -lgthread-2.0 -lgobject-2.0 -lffi -lgstpbutils-0.10 -lgstvideo-0.10 -lgstbase-0.10 -lgstaudio-0.10 -lgsttag-0.10 -lcrypto  


SOURCES += main.cpp\
       	   debug.cpp\
           mainwindow.cpp\
       	   stream.cpp\
		   demo.cpp\
           newpushbutton.cpp

HEADERS  += mainwindow.h\
			debug.h\
			stream.h\
		   	sdk.h\
		   	demo.h\
            newpushbutton.h

FORMS    += mainwindow.ui
