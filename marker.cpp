#include "marker.h"
#include <QtWidgets>
Marker::Marker(int id)
{
    //setFlags(ItemIsSelectable);
    //setAcceptHoverEvents(true);
    setZValue(10);
    radius = 15.0;
    innerRadius = 10.0;
    this->color = this->idToColor(id);
}

QRectF Marker::boundingRect() const
{
    return QRectF(-radius, -radius, radius * 2, radius * 2);

}

QPainterPath Marker::shape() const
{
    QPainterPath path;
    path.addEllipse(-radius, -radius, radius * 2, radius * 2);
    return path;
}

void Marker::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget * /*widget*/)
{
    QColor penColor = this->color;

    if (option->state & QStyle::State_Selected)
        //penColor=penColor.lighter();
        penColor = penColor.darker(400);
    if (option->state & QStyle::State_MouseOver)
        penColor = penColor.darker(200);

    QPainterPath circle;
    circle.addEllipse(-radius, -radius, radius * 2, radius * 2);
    circle.addEllipse(-innerRadius, -innerRadius, innerRadius * 2, innerRadius * 2);
    circle.setFillRule(Qt::OddEvenFill);

    painter->save();
    painter->setBrush(penColor);
    painter->setPen(Qt::NoPen);
    //painter->setPen(Qt::blue);
    painter->drawPath(circle);
    painter->setPen(Qt::blue);
    painter->drawPoint(0., 0.);
    //painter->drawLine(-innerRadius,0,innerRadius,0);
    //painter->drawLine(0,-innerRadius,0,innerRadius);
    painter->restore();
}

QColor Marker::idToColor(int id)
{
    const int magicNum = (id * 11) % 36;
    QColor c = QColor::fromHsv(magicNum / 36. * 360, 255, 255);
    return c;
}