#pragma once

#include <QGraphicsObject>
class Marker: public QGraphicsObject
{
    Q_OBJECT
public:
    Marker(int row);
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget*) Q_DECL_OVERRIDE;
private:
    QColor idToColor(int id);
    double radius;
    double innerRadius;
    QColor color;
};