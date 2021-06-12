//
// Created by morpheus on 1/31/19.
//

#ifndef AP_MAHLA_CLIENT_H
#define AP_MAHLA_CLIENT_H

#include <QObject>
#include "Login.h"
#include "BoardGame.h"
#include <string>

class Client : public QObject {
    Q_OBJECT
public:
    Client();

private:
    QString *username;
    qint32 wins, losts;
    LoginMenu *loginMenu;
    BoardGame *boardGame;

public slots:
    void newClientLogin(QString*, quint16, quint16);
    void refreshBoard(bool, quint16, quint16);
};

#endif //AP_MAHLA_CLIENT_H
