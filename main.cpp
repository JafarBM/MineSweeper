#include <iostream>
#include <Headers/Client.h>
#include <QtWidgets/QApplication>

int main(int argc, char **argv) {
    QApplication *client = new QApplication(argc, argv);
    new Client;
    return client -> exec();
}