//
// Created by morpheus on 1/31/19.
//

#include <iostream>
#include <QDialogButtonBox>
#include <random>
#include <QTextEdit>
#include "Headers/BoardGame.h"

BoardGame::BoardGame(QString* username, quint16 height, quint16 width, quint16 wins, quint16 losts) {
    displayName = username;
    boardHeight = height;
    boardWidth = width;
    numberOfBombs = quint32(width * height / 8);
    remainingFlags = numberOfBombs;
    win = wins;
    lost = losts;

    this->setWindowTitle(*displayName);
    QRect rect = QApplication::desktop()->screenGeometry();
    this->setGeometry(rect.width() / 2, rect.height() / 2, width * 68, height * 72);

    QString winsAsString;
    winsAsString.setNum(int(wins));
    QString lostAsString;
    lostAsString.setNum(int(losts));
    QString flags;
    flags.setNum(int(remainingFlags));
    scoreBoard = new QLabel("Win: " +
                            winsAsString + "\tLost: " + lostAsString);
    remainingFlagsBoard = new QTextEdit(flags);
    remainingFlagsBoard->setReadOnly(true);
    remainingFlagsBoard->setFixedWidth(40);
    remainingFlagsBoard->setFixedHeight(30);
    QHBoxLayout *scoreboardLayout = new QHBoxLayout;
    scoreboardLayout->addWidget(scoreBoard);
    scoreboardLayout->addWidget(new QLabel("\t\t\tRemaining Flags: "));
    scoreboardLayout->addWidget(remainingFlagsBoard);

    int bombCells[numberOfBombs];
    for(int i = 0; i < numberOfBombs; ++ i) {
        bombCells[i] = rand() % (width * height);
        for(int j = 0; j < i; ++ j)
            if(bombCells[j] == bombCells[i]) {
                i--;
                break;
            }
    }

    for(unsigned int i = 0; i < boardWidth; ++ i) {
        QVector<Cell*> newRow;
        for(unsigned int j = 0; j < boardHeight; ++ j) {
            unsigned int cellIndex = i * boardHeight + j;
            bool is_bomb = false;
            for(int k = 0; k < numberOfBombs; ++ k)
                if(cellIndex == bombCells[k])
                    is_bomb = true;
            Cell *newCell = new Cell(is_bomb, 60, 60, cellIndex);
            connect(newCell, SIGNAL(gameOver(bool)), this, SLOT(finishGame(bool)));
            connect(newCell, SIGNAL(clearArea(quint32)), this, SLOT(clearArea(quint32)));
            connect(newCell, SIGNAL(addOrRemoveFlag(bool)), this, SLOT(addOrRemoveFlag(bool)));
            newRow.append(newCell);
        }
        std::cout << std::endl;
        cells.append(newRow);
    }

    calculateNeighbourBombs();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(scoreboardLayout);
    for(int i = 0; i < boardWidth; ++ i) {
        QDialogButtonBox *qDialogButtonBox = new QDialogButtonBox;
        for(int j = 0; j < boardHeight; ++ j) {
            qDialogButtonBox->addButton(cells.at(i).at(j), QDialogButtonBox::ActionRole);
        }
        mainLayout->addWidget(qDialogButtonBox);
    }

    setLayout(mainLayout);

    this->show();
}

void BoardGame::checkIfGameFinished() {
    for(int i = 0; i < boardWidth; ++ i) {
        for(int j = 0; j < boardHeight; ++ j) {
            if(!cells.at(i).at(j)->checkReveal() && !cells.at(i).at(j)->checkForBomb())
                return;
        }
    }
    finishGame(true);
}

void BoardGame::calculateNeighbourBombs() {

    int neix[] = {1, -1, 0, 0, -1, 1, 1, -1};
    int neiy[] = {0, 0, 1, -1, -1, 1, -1, 1};
    for(unsigned int i = 0; i < boardWidth; ++ i) {
        for(unsigned int j = 0; j < boardHeight; ++ j) {
            for(int k = 0; k < 8; ++ k) {
                int curNeiX = i + neix[k], curNeiY = j + neiy[k];
                if (curNeiX >= boardWidth || curNeiX < 0)
                    continue;
                if (curNeiY >= boardHeight || curNeiY < 0)
                    continue;
                if(cells.at(curNeiX).at(curNeiY)->checkForBomb()) {
                    cells.at(i).at(j)->addNeighbourBomb();
                }
            }
        }
    }
    this->update();
}

void BoardGame::clearArea(quint32 cellIndex) {
    int startRow = cellIndex / boardHeight, startColumn = cellIndex % boardHeight;

    int neix[] = {1, -1, 0, 0};
    int neiy[] = {0, 0, 1, -1};
    std::pair<int, int> que[boardHeight * boardWidth + 10];
    int dis[boardWidth + 10][boardHeight + 10];
    for(int i = 0; i < boardWidth; i ++) {
        for(int j = 0; j < boardHeight; j ++) {
            dis[i][j] = 0;
        }
    }

    int head = 0, tail = 0;
    que[head++] = std::make_pair(startRow, startColumn);

    while(tail < head) {
        std::pair<int, int> current = que[tail ++];
        cells.at(current.first).at(current.second)->revealCell();
        for(int i = 0; i < 4; ++ i) {
            int curNeiX = current.first + neix[i], curNeiY = current.second + neiy[i];
            if(curNeiX >= boardWidth || curNeiX < 0){
                continue;
            }
            if(curNeiY >= boardHeight || curNeiY < 0) {
                continue;
            }
            if(dis[curNeiX][curNeiY] != 0 || cells.at(current.first).at(current.second)->getNeighbourBombs() > 0) {
                continue;
            }
            if(cells.at(curNeiX).at(curNeiY)->checkForFlag() || cells.at(curNeiX).at(curNeiY)->checkForBomb()) {
                continue;
            }
            dis[curNeiX][curNeiY] = dis[current.first][current.second] + 1;
            que[head++] = std::make_pair(curNeiX, curNeiY);
        }
    }
    checkIfGameFinished();
    this->update();
}

void BoardGame::finishGame(bool gameState) {
    this->close();
    emit refreshBoard(gameState, boardHeight, boardWidth);
}

void BoardGame::addOrRemoveFlag(bool addOrRemove) {
    if(addOrRemove) {
        remainingFlags --;
    } else {
        remainingFlags ++;
    }
    updateLayout();
}

void BoardGame::updateLayout() {
    QString winsAsString;
    winsAsString.setNum(int(win));
    QString lostAsString;
    lostAsString.setNum(int(lost));
    QString flags;
    flags.setNum(int(remainingFlags));
    scoreBoard = new QLabel("Win: " +
                            winsAsString + "\tLost: " + lostAsString);
    remainingFlagsBoard = new QTextEdit(flags);
    remainingFlagsBoard->setReadOnly(true);
    remainingFlagsBoard->setFixedWidth(40);
    remainingFlagsBoard->setFixedHeight(30);
    QHBoxLayout *scoreboardLayout = new QHBoxLayout;
    scoreboardLayout->addWidget(scoreBoard);
    scoreboardLayout->addWidget(new QLabel("\t\t\tRemaining Flags: "));
    scoreboardLayout->addWidget(remainingFlagsBoard);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(scoreboardLayout);
    for(int i = 0; i < boardWidth; ++ i) {
        QDialogButtonBox *qDialogButtonBox = new QDialogButtonBox;
        for(int j = 0; j < boardHeight; ++ j) {
            qDialogButtonBox->addButton(cells.at(i).at(j), QDialogButtonBox::ActionRole);
        }
        mainLayout->addWidget(qDialogButtonBox);
    }

    QLayoutItem *child;
    while ((child = this->layout()->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }
    delete this->layout();

    setLayout(mainLayout);
    this->show();
}

Cell::Cell(bool is_bomb, quint16 height, quint16 width, quint32 index) {
    isBomb = is_bomb;
    cellHeight = height;
    cellWidth = width;
    cellIndex = index;
    QPalette pal = palette();
    pal.setColor(QPalette::Button, QColor(Qt::gray));
    QFont font = this->font();
    font.setPointSize(18);
    font.setBold(true);
    this->setFont(font);
    this->setPalette(pal);
    this->setAutoFillBackground(true);
    this->setFixedWidth(width); this->setFixedHeight(height);
    this->show();
}

quint32 Cell::getNeighbourBombs() {
    return neighbourBombs;
}

bool Cell::checkForFlag() {
    return markedAsFlag;
}

bool Cell::checkForBomb() {
    return isBomb;
}

void Cell::addNeighbourBomb() {
    neighbourBombs ++;
}

void Cell::revealCell() {
    this->isRevealed = true;
    this->setDisabled(true);
    QPalette pal = this->palette();
    pal.setColor(QPalette::Button, QColor(Qt::green));
    this->setPalette(pal);
    if(neighbourBombs == 0) {
        this->update();
        return;
    }
    QString num;
    num.setNum(int(neighbourBombs));
    this->setText(num);
    this->update();
}

void Cell::mousePressEvent(QMouseEvent *event) {
    if(event->buttons() & Qt::LeftButton & !markedAsFlag) {
        if (isBomb) {
            emit gameOver(false);
        }
        else {
            emit clearArea(cellIndex);
        }
    }
    else if (event->buttons() & Qt::RightButton) {
        if(!markedAsFlag) {
            markedAsFlag = true;
            putFlag();
        } else {
            markedAsFlag = false;
            takeFlag();
        }
    }
}

void Cell::putFlag() {
    QPalette pal = this->palette();
    pal.setColor(QPalette::Button, QColor(Qt::blue));
    this->setPalette(pal);
    this->update();
    emit this->addOrRemoveFlag(true);
}

void Cell::takeFlag() {
    QPalette pal = palette();
    pal.setColor(QPalette::Button, QColor(Qt::gray));
    this->setPalette(pal);
    this->update();
    emit this->addOrRemoveFlag(false);
}

bool Cell::checkReveal() {
    return isRevealed;
}