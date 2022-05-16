#include "graphwidget.h"
#include "edge.h"
#include "graphnode.h"
#include <math.h>
#include <QKeyEvent>

GraphWidget::GraphWidget(QWidget *parent, int countOfVerticles, int** adjMatrix)
    : QGraphicsView(parent)
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(-200, -200, 400, 400);
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(qreal(0.8), qreal(0.8));
    setMinimumSize(400, 400);
    setWindowTitle(tr("Graph"));
    QVector <GraphNode*> nodes; //Создание вектора, хранящего все вершины графа
    if(countOfVerticles != 0 && adjMatrix != nullptr)
    {
        for(int i = 0; i < countOfVerticles; i++)
        {
            GraphNode* node = new GraphNode(this);
            nodes.push_back(node);
            nodes[i]->setNumberOfNode(i+1);
            scene->addItem(nodes[i]); //Заполнение сцены вершинами графа
        }
        for(int i = 0; i < countOfVerticles; i++)
        {
            for(int j = 0; j < countOfVerticles; j++)
            {
                if (adjMatrix[i][j] == 1 && i > j)
                    scene->addItem(new Edge(nodes[i], nodes[j]));//Добавление ребер графа
            }
        }
    }
    shuffle(); //Перемешивание позиций вершин
}

void GraphWidget::itemMoved() //Расчет точки отсчета при перемещении вершины
{
    if (!timerId)
        timerId = startTimer(1000 / 25);
}

//void GraphWidget::keyPressEvent(QKeyEvent *event)
//{
//    switch (event->key())
//    {
//    case Qt::Key_Plus:
//        zoomIn();
//        break;
//    case Qt::Key_Minus:
//        zoomOut();
//        break;
//    case Qt::Key_Space:
//    case Qt::Key_Enter:
//        shuffle();
//        break;
//    default:
//        QGraphicsView::keyPressEvent(event);
//    }
//}

void GraphWidget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    QList<GraphNode *> nodes; //создание списка вершин
    const QList<QGraphicsItem *> items = scene()->items(); //считывание всех элементов на сцене
    for (QGraphicsItem *item : items)
    {
        if (GraphNode *node = qgraphicsitem_cast<GraphNode *>(item)) //считывание вершин и заполнение списка
            nodes << node;
    }

    for (GraphNode *node : qAsConst(nodes)) // расчет сил действующих на вершины
        node->calculateForces();

    bool itemsMoved = false;
    for (GraphNode *node : qAsConst(nodes)) //проработка изменений позиций
    {
        if (node->advancePosition())
            itemsMoved = true;
    }

    if (!itemsMoved)
    {
        killTimer(timerId);
        timerId = 0;
    }
}


#if QT_CONFIG(wheelevent)

void GraphWidget::wheelEvent(QWheelEvent *event)
{
    scaleView(pow(2., -event->angleDelta().y() / 240.0));
}

#endif



void GraphWidget::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.1 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}


void GraphWidget::shuffle() //Перемешивание положения вершин
{
    const QList<QGraphicsItem *> items = scene()->items();
    for (QGraphicsItem *item : items)
    {
        if (qgraphicsitem_cast<GraphNode *>(item))
            item->setPos(-150 + QRandomGenerator::global()->bounded(300), -150 + QRandomGenerator::global()->bounded(300));//задание случайной
                                                                                                                            //позиции вершины
    }
}

void GraphWidget::zoomIn()//Приближение экрана
{
    scaleView(qreal(1.1));
}

void GraphWidget::zoomOut()//Отдаление экрана
{
    scaleView(1 / qreal(1.1));
}
