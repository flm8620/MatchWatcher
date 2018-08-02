#pragma once
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <iostream>
#include <QEvent>
struct AbstractFeature
{
    QPointF pos;
    float scale;
    float orient_x, orient_y;
};

class ImageView : public QGraphicsView
{
    Q_OBJECT

private:
    QGraphicsScene scene;
    QGraphicsPixmapItem* image_item;
    std::map<int, AbstractFeature> features;
    const int id;
protected:
    void wheelEvent(QWheelEvent *) override;
signals:
    void userMoved();
public slots:
    void zoomIn();
    void zoomOut();
public:
    ImageView(QWidget* parent = nullptr, int id = -1) : id(id) {
        this->setScene(&scene);
        this->setDragMode(ScrollHandDrag);
        this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }

    void LoadImage(const QString& file);

    void LoadFeatures(const std::map<int, AbstractFeature>& features);

    void fitViewAllObject();

    bool GetFeaturePos(int id, QPointF& pos) {
        if (features.find(id) == features.end())
            return false;

        pos = features[id].pos;
        return true;
    }

    QPointF MapImagePointToWidget(QPointF pos) const {
        QPoint p;
        QPointF pf = image_item->mapToScene(pos);
        p.setX(pf.x());
        p.setY(pf.y());
        return mapFromScene(p);
    }

    void scrollContentsBy(int x, int y) override {
        emit userMoved();
        this->QGraphicsView::scrollContentsBy(x, y);
    }
};