//
// Created by morpheus on 1/31/19.
//


#include <Headers/Login.h>
#include <Headers/Client.h>

LoginMenu::LoginMenu() {
    QRect rect = QApplication::desktop()->screenGeometry();
    this->setGeometry(rect.width() / 2, rect.height() / 2, 400, 400);


    QDialogButtonBox *buttonBox = new QDialogButtonBox;
    exit = new QPushButton("Exit", this);
    play = new QPushButton("Play", this);
    buttonBox->addButton(exit, QDialogButtonBox::RejectRole);
    buttonBox->addButton(play, QDialogButtonBox::ActionRole);

    QHBoxLayout *usernameLayout = new QHBoxLayout;
    username = new QLineEdit("Player");
    QLabel *userLabel = new QLabel("Username: ");
    usernameLayout->addWidget(userLabel); usernameLayout->addWidget(username);

    QHBoxLayout *boardSizeLayout = new QHBoxLayout;
    board_height = new QLineEdit("10");
    QLabel *heightLabel = new QLabel("Height: ");
    boardSizeLayout->addWidget(heightLabel); boardSizeLayout->addWidget(board_height);

    board_width = new QLineEdit("10");
    QLabel *widthLabel = new QLabel("Width");
    boardSizeLayout->addWidget(widthLabel); boardSizeLayout->addWidget(board_width);

    QVBoxLayout *LoginPage = new QVBoxLayout;
    LoginPage->addLayout(usernameLayout);
    LoginPage->addLayout(boardSizeLayout);
    LoginPage->addWidget(buttonBox);

    setLayout(LoginPage);
    connect(exit, SIGNAL(clicked()), this, SLOT(close()));
    connect(play, SIGNAL(clicked()), this, SLOT(newGame()));
    show();
}


void LoginMenu::newGame() {
    this->hide();
    emit newLogin(new QString(username->text()), board_height->text().toUShort(), board_width->text().toUShort());
}
