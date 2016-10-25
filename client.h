#ifndef CLIENT_H
#define CLIENT_H

#include <QJsonObject>
#include <QString>
#include <QDebug>
#include <QJsonValue>
#include <QMessageLogger>
#include "hs_socket_app.h"
#include "debug.h"

#define SOCKET_INI_PATH "/tmp/socket_module_conf.ini"



int send_response(QString msg, QString para);
void start_mainclient();

#endif // CLIENT_H

