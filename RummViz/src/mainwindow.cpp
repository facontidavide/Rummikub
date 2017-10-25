#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsTextItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _player_scene = new QGraphicsScene(this);
    _table_scene  = new QGraphicsScene(this);


    ui->graphicsViewTable->setScene( _table_scene );
    ui->graphicsViewPlayer->setScene( _player_scene );

    drawPiece( QPoint(0,0), Qt::blue, 1);
    drawPiece( QPoint(0,1), Qt::blue, 2);
    drawPiece( QPoint(1,0), Qt::blue, 3);
    drawPiece( QPoint(1,1), Qt::blue, 4);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::drawPiece(QPoint pos, QColor color, int number)
{
    const float width  = 35;
    const float height = 50;
    QPen pen;
    pen.setColor( Qt::darkGray );
    pen.setWidth(1);
    _table_scene->addRect( pos.x()*40.0, pos.y()*90.0, width, height, pen );

    QFont font;
    font.setPixelSize(36);
    font.setBold(true);
    font.setFamily("Calibri");

    QGraphicsTextItem *text = _table_scene->addText( QString::number(number) );
    text->setPos( pos.x()*40.0, pos.y()*90.0 );
    text->setFont( font );
    text->setDefaultTextColor(color);
}
