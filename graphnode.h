#ifndef GRAPHNODE_H
#define GRAPHNODE_H

#include <QGraphicsItem>
#include <QObject>
#include <QWidget>
#include <QPainter>

class Edge;
class GraphWidget;


class GraphNode : public QGraphicsItem
{
public:
    GraphNode(GraphWidget *graphWidget); //Параметр необходим для отрисовки графического элемента вершины графа

    void addEdge(Edge *edge); //Добавление ребра графа
    QList<Edge *> edges() const; //Геттер для получения всех ребер
    enum { Type = UserType + 1 };
    int type() const override { return Type; }
    void setNumberOfNode(int number); //Присвоение номера вершины графа(показывается при наведении на вершину
    void calculateForces(); //Расчет действующих на вершину сил
    bool advancePosition(); //Расчет позиции вершины при учете всех воздействий

    QRectF boundingRect() const override;
    QPainterPath shape() const override; //Задание фигуры для вершины
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override; //Отрисовка вершины

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override; //Изменение вершины и ребер связанных с ней

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override; //Метод, вызываемый при нажатии кнопки мыши
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override; //Метод, вызываемый после того как кнопку мыши отпустили

private:
    QList<Edge *> edgeList; //Список, хранящий ребра
    QPointF newPos; //Новая позиция графа
    GraphWidget *graph; //Объект, отвечающий за отрисовку графа
};

#endif // GRAPHNODE_H
