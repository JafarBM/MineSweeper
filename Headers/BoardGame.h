//
// Created by morpheus on 1/31/19.
//

#ifndef AP_MAHLA_BOARDGAME_H
#define AP_MAHLA_BOARDGAME_H

#include <QWidget>
#include<QDialog>
#include<QTextEdit>
#include<QPushButton>
#include<QVBoxLayout>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QApplication>
#include <QMainWindow>
#include <QDesktopWidget>
#include <QBoxLayout>
#include <QPushButton>
#include <QtWidgets/QLabel>
#include <QElapsedTimer>

class Cell : public QPushButton {
    Q_OBJECT
public:
    Cell(bool, quint16, quint16, quint32);
    quint32 getNeighbourBombs();
    bool checkForFlag();
    bool checkForBomb();
    void addNeighbourBomb();
    void revealCell();
    bool checkReveal();

private:
    quint32 cellWidth, cellHeight, neighbourBombs = 0, cellIndex;
    bool isBomb, markedAsFlag = false, isRevealed = false;

    void putFlag();
    void takeFlag();

protected:
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void gameOver(bool);
    void clearArea(quint32);
    void addOrRemoveFlag(bool);

};

class BoardGame : public QWidget {
    Q_OBJECT
public:
    BoardGame(QString*, quint16, quint16, quint16, quint16);

private:
    QVector<QVector<Cell*> > cells;
    QElapsedTimer *timer;
    quint16 boardHeight, boardWidth;
    quint32 numberOfBombs, remainingFlags, win, lost;
    QString* displayName;
    QLabel* scoreBoard;
    QTextEdit* remainingFlagsBoard;

    void calculateNeighbourBombs();
    void checkIfGameFinished();
    void updateLayout();

public slots:
    void finishGame(bool);
    void clearArea(quint32);
    void addOrRemoveFlag(bool);


signals:
    void refreshBoard(bool, quint16, quint16);
};

#endif //AP_MAHLA_BOARDGAME_H
