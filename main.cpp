#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <QMessageBox>
#include <QHBoxLayout>

#include <QVBoxLayout>

#include <QLabel>
#include <QMouseEvent>


const int ROWS = 10;
const int COLS = 10;
const int MINES = 20;

class FlagButton : public QPushButton {

public:
    explicit FlagButton(QWidget *parent = nullptr);

signals:
    void rightClicked();

protected:
    void mousePressEvent(QMouseEvent *event) override;
};

FlagButton::FlagButton(QWidget *parent) : QPushButton(parent) {}

void FlagButton::mousePressEvent(QMouseEvent *event) {
    // if (event->button() == Qt::RightButton) {
    //     emit rightClicked();
    // } else {
    //     QPushButton::mousePressEvent(event);
    // }
}



class Minesweeper : public QWidget {

public:
    Minesweeper(QWidget *parent = nullptr);

private:

    std::vector<std::vector<int>> grid;

    std::vector<std::vector<bool>> revealed;

    std::vector<std::vector<bool>> flagged;  // Track flagged cells


    QGridLayout *gridLayout;

    QLabel *scoreLabel;

    QPushButton *restartButton;

    QPushButton *hintButton;

    int score;



    void placeMines();

    int countAdjacentMines(int row, int col);

    void revealCell(int row, int col);

    void gameOver();

    void checkWin();

    void resetGame();

    void giveHint();

    std::pair<int, int> findSafeCell();

private slots:
    void handleButtonClick(int row, int col);
    void handleRestartClick();

    void handleHintClick();

};

Minesweeper::Minesweeper(QWidget *parent) : QWidget(parent), score(0) {
    grid.resize(ROWS, std::vector<int>(COLS, 0));
    revealed.resize(ROWS, std::vector<bool>(COLS, false));

    srand(time(NULL));
    placeMines();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);



    // Create score display

    scoreLabel = new QLabel("Score: 0", this);

    mainLayout->addWidget(scoreLabel);



    // Create buttons for Restart and Hint

    QHBoxLayout *topLayout = new QHBoxLayout();

    restartButton = new QPushButton("Restart", this);

    hintButton = new QPushButton("Hint", this);



    connect(restartButton, &QPushButton::clicked, this, &Minesweeper::handleRestartClick);

    connect(hintButton, &QPushButton::clicked, this, &Minesweeper::handleHintClick);



    topLayout->addWidget(restartButton);

    topLayout->addWidget(hintButton);

    mainLayout->addLayout(topLayout);



    // Create the grid layout for the minesweeper buttons

    gridLayout = new QGridLayout();

    // gridLayout = new QGridLayout(this);
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            QPushButton *button = new QPushButton(this);
            button->setFixedSize(30, 30);
            button->setProperty("row", i);
            button->setProperty("col", j);
            connect(button, &QPushButton::clicked, this, [=]() {
                int row = button->property("row").toInt();
                int col = button->property("col").toInt();
                handleButtonClick(row, col);
            });
            gridLayout->addWidget(button, i, j);
        }
    }
    mainLayout->addLayout(gridLayout);

    setLayout(mainLayout);
}

void Minesweeper::placeMines() {
    int count = 0;
    while (count < MINES) {
        int row = rand() % ROWS;
        int col = rand() % COLS;
        if (grid[row][col] != 9) {
            grid[row][col] = 9; // 9 represents a mine
            ++count;
        }
    }
}

int Minesweeper::countAdjacentMines(int row, int col) {
    int count = 0;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            int newRow = row + i;
            int newCol = col + j;
            if (newRow >= 0 && newRow < ROWS && newCol >= 0 && newCol < COLS && grid[newRow][newCol] == 9) {
                ++count;
            }
        }
    }
    return count;
}

void Minesweeper::revealCell(int row, int col) {
    if (revealed[row][col]) {
        return;
    }
    revealed[row][col] = true;

    ++score;

    scoreLabel->setText("Score: " + QString::number(score));

    QPushButton *button = qobject_cast<QPushButton *>(gridLayout->itemAtPosition(row, col)->widget());
    button->setEnabled(false);

    int adjacentMines = countAdjacentMines(row, col);
    if (adjacentMines > 0) {
        button->setText(QString::number(adjacentMines));
    } else {
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                int newRow = row + i;
                int newCol = col + j;
                if (newRow >= 0 && newRow < ROWS && newCol >= 0 && newCol < COLS) {
                    revealCell(newRow, newCol);
                }
            }
        }
    }
}

void Minesweeper::gameOver() {
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            if (grid[i][j] == 9) {
                QPushButton *button = qobject_cast<QPushButton *>(gridLayout->itemAtPosition(i, j)->widget());
                button->setText("*");
            }
        }
    }
    QMessageBox::information(this, "Game Over", "You hit a mine!");
    resetGame();
}

void Minesweeper::checkWin() {
    bool win = true;
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            if (!revealed[i][j] && grid[i][j] != 9) {
                win = false;
                break;
            }
        }
    }
    if (win) {
        QMessageBox::information(this, "Congratulations", "You won the game!");
        resetGame();
    }
}


void Minesweeper::resetGame() {

    score = 0;

    scoreLabel->setText("Score: 0");

    grid = std::vector<std::vector<int>>(ROWS, std::vector<int>(COLS, 0));

    revealed = std::vector<std::vector<bool>>(ROWS, std::vector<bool>(COLS, false));

    placeMines();



    for (int i = 0; i < ROWS; ++i) {

        for (int j = 0; j < COLS; ++j) {

            QPushButton *button = qobject_cast<QPushButton *>(gridLayout->itemAtPosition(i, j)->widget());

            button->setEnabled(true);

            button->setText("");

        }

    }

}



std::pair<int, int> Minesweeper::findSafeCell() {

    for (int i = 0; i < ROWS; ++i) {

        for (int j = 0; j < COLS; ++j) {

            if (!revealed[i][j] && grid[i][j] != 9) {

                return {i, j};

            }

        }

    }

    return {-1, -1};

}



void Minesweeper::giveHint() {


    auto [row, col] = findSafeCell();



    if (row != -1 && col != -1) {

        revealCell(row, col);

    }

}

void Minesweeper::handleButtonClick(int row, int col) {
    if (grid[row][col] == 9) {
        gameOver();
    } else {
        revealCell(row, col);
        checkWin();
    }
}


void Minesweeper::handleRestartClick() {

    resetGame();

}



void Minesweeper::handleHintClick() {

    giveHint();

}


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Minesweeper minesweeper;
    minesweeper.setWindowTitle("Minesweeper");
    minesweeper.show();
    return app.exec();
}
