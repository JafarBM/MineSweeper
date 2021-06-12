//
// Created by morpheus on 1/31/19.
//

#ifndef AP_MAHLA_LOGIN_H
#define AP_MAHLA_LOGIN_H

#include <QWidget>
#include <QPushButton>
#include <QApplication>
#include <QMainWindow>
#include <QDesktopWidget>
#include <QLineEdit>
#include <QLabel>
#include <QBoxLayout>
#include <QDialogButtonBox>

class QPushButton;
class QLineEdit;

class LoginMenu : public QWidget {
    Q_OBJECT
public:
    LoginMenu();

private:
    QPushButton *exit, *play;
    QLineEdit *username, *board_width, *board_height;
signals:
    void newLogin(QString*, quint16, quint16);

public slots:
    void newGame();
};

#endif //AP_MAHLA_LOGIN_H
