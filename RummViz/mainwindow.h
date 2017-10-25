#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

namespace Ui {
class MainWindow;
}



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:

    void drawPiece(QPoint pos, QColor color, int number);

    Ui::MainWindow *ui;

    QGraphicsScene* _table_scene;
    QGraphicsScene* _player_scene;
};

#endif // MAINWINDOW_H
