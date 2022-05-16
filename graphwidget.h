#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsView>
#include <QMenu>
#include <QMenuBar>
#include "mainwindow.h"
#include <QMenu>
#include <QMenuBar>

class GraphNode;


class GraphWidget : public QGraphicsView
{
    Q_OBJECT

public:
    GraphWidget(QWidget *parent = nullptr, int countOfVerticles = 0, int** adjMatrix = 0);
    void itemMoved(); //Изменения при перемещении объекта

public slots:
    void shuffle(); //Перемешивание положения вершин
    void zoomIn(); //Приближение (колесиком мыши или клавишей +)
    void zoomOut(); //Отдаление (колесиком мыши или клавишей -)

protected:
    //void keyPressEvent(QKeyEvent *event) override; //Событие, генерируемое при нажатии на кнопку
    void timerEvent(QTimerEvent *event) override; //Обработка  событий перемещения вершин
#if QT_CONFIG(wheelevent)
    void wheelEvent(QWheelEvent *event) override; //Событие, генерируемое при прокрутке колесика мыши
#endif
    void scaleView(qreal scaleFactor); //Приближение / отдаление экрана

private:
    int timerId = 0;

};

#endif // GRAPHWIDGET_H
