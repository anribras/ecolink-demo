#ifndef FBCONTROL_GLOBAL_H
#define FBCONTROL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(FBCONTROL_LIBRARY)
#  define FBCONTROLSHARED_EXPORT Q_DECL_EXPORT
#else
#  define FBCONTROLSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // FBCONTROL_GLOBAL_H
