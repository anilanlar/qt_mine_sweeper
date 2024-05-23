// minesweeper.cpp
#include "minesweeper.h"
#include <QPainter>
#include <QMouseEvent>
#include <random>

Minesweeper::Minesweeper(QWidget *parent) : QWidget(parent), numMines(10)
{
    setFixedSize(200, 200); // Assuming each cell is 20x20 pixels
    generateGrid();
}

void Minesweeper::generateGrid()
{
    grid.resize(10, std::vector<CellState>(10, CellState::Hidden));
    mineLocations.resize(10, std::vector<bool>(10, false));
    placeMines();
}

void Minesweeper::placeMines()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 9);

    int numMinesPlaced = 0;
    while (numMinesPlaced < numMines) {
        int row = dis(gen);
        int col = dis(gen);
        if (!mineLocations[row][col]) {
            mineLocations[row][col] = true;
            numMinesPlaced++;
        }
    }
}

void Minesweeper::revealCell(int row, int col)
{
    if (grid[row][col] != CellState::Hidden)
        return;

    if (mineLocations[row][col]) {
        // Game over, handle mine explosion
    } else {
        // Implement logic to reveal adjacent cells
        grid[row][col] = CellState::Revealed;
    }

    // Check win condition
    // If all non-mine cells are revealed, player wins
}

void Minesweeper::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // Paint the grid based on grid and mineLocations
}

void Minesweeper::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        int row = event->y() / 20; // Assuming each cell is 20x20 pixels
        int col = event->x() / 20;

        revealCell(row, col);
        update(); // Update the widget to repaint
    }
    // Add logic for right-click (flagging)
}
