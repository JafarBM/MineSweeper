//
// Created by morpheus on 1/31/19.
//

#include <Headers/Client.h>

Client::Client() {
    wins = 0; losts = 0;
    loginMenu = new LoginMenu;
    connect(loginMenu, SIGNAL(newLogin(QString*, quint16, quint16)) ,
            this, SLOT(newClientLogin(QString*, quint16, quint16)));
}

void Client::newClientLogin(QString* username, quint16 height, quint16 width) {
    this->username = username;
    boardGame = new BoardGame(username, height, width, wins, losts);
    connect(boardGame, SIGNAL(refreshBoard(bool, quint16, quint16)),
            this ,SLOT(refreshBoard(bool, quint16, quint16)));
}

void Client::refreshBoard(bool lastGameState, quint16 height, quint16 width) {
    if(lastGameState) {
        wins ++;
    } else {
        losts ++;
    }
    boardGame = new BoardGame(username, height, width, wins, losts);
    connect(boardGame, SIGNAL(refreshBoard(bool, quint16, quint16)),
            this ,SLOT(refreshBoard(bool, quint16, quint16)));
}
