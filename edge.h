#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsItem>

//Деструктор не реализован, т.к. qt рекурсивно освобождает память у классов, унаследованных от QtObject
class GraphNode;

class Edge : public QGraphicsItem
{
public:
    Edge(GraphNode *sourceNode, GraphNode *destNode);

    GraphNode *sourceNode() const;
    GraphNode *destNode() const;

    void adjust(); //Функция проверки и задания линий (ребер графа)

    enum { Type = UserType + 2 };
    int type() const override { return Type; }

protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override; //Функция отрисовки

private:
    GraphNode *source, *dest; //Начальная и конечная вершина графа
    QPointF sourcePoint; //Начальная точка (координаты)
    QPointF destPoint; //Конечная точка (координаты)
};

#endif // EDGE_H
