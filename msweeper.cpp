#include <QApplication>
#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QSignalMapper>
#include <QDebug>

class Minesweeper : public QWidget {
    Q_OBJECT

public:
    Minesweeper(QWidget *parent = nullptr) : QWidget(parent) {
        QGridLayout *layout = new QGridLayout;
        mapper = new QSignalMapper(this);

        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < COLS; ++j) {
                QPushButton *button = new QPushButton;
                button->setFixedSize(30, 30);
                buttons[i][j] = button;
                layout->addWidget(button, i, j);
                connect(button, SIGNAL(clicked()), mapper, SLOT(map()));
                mapper->setMapping(button, i * COLS + j);
            }
        }

        connect(mapper, SIGNAL(mapped(int)), this, SLOT(handleClick(int)));

        setLayout(layout);
        setWindowTitle("Minesweeper");
    }

private slots:
    void handleClick(int index) {
        int row = index / COLS;
        int col = index % COLS;
        qDebug() << "Clicked on cell" << row << col;
        // Here you can implement your logic for revealing cells,
        // checking for mines, and updating the game state.
    }

private:
    static const int ROWS = 10;
    static const int COLS = 10;
    QPushButton *buttons[ROWS][COLS];
    QSignalMapper *mapper;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Minesweeper window;
    window.show();

    return app.exec();
}

