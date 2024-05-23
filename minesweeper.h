// minesweeper.h
#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include <QWidget>

class Minesweeper : public QWidget
{
    Q_OBJECT

public:
    explicit Minesweeper(QWidget *parent = nullptr);

private:
    enum class CellState {
        Hidden,
        Revealed,
        Flagged
    };

    std::vector<std::vector<CellState>> grid;
    std::vector<std::vector<bool>> mineLocations;
    int numMines;

    void generateGrid();
    void placeMines();
    void revealCell(int row, int col);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // MINESWEEPER_H
