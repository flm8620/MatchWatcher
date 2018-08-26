#pragma once
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <iostream>
#include <QEvent>
#include "imagescene.h"
#include "marker.h"
#include <set>


class ImageView : public QGraphicsView
{
    Q_OBJECT

private:
    ImageScene* image_scene;
    QGraphicsScene scene;
    QGraphicsPixmapItem* image_item;
    std::vector<std::pair<AbstractFeature, Marker*>> features;
    const int id;
    double max_feature_size;
    bool show_features;
    bool only_show_matched;
    std::set<int> matched;
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
        max_feature_size = 1e100;
        show_features = true;
        only_show_matched = false;
    }

    void set_scene(ImageScene* image_scene) {
        this->image_scene = image_scene;
    }

    void LoadImage(int idx);

    void fitViewAllObject();

    bool GetFeaturePos(int id, QPointF& pos) {
        if (id < 0 || id >= features.size())
            return false;

        pos = features[id].first.pos;
        return true;
    }

    QPointF MapImagePointToWidget(QPointF pos) const {
        QPointF pf = image_item->mapToScene(pos);
        return mapFromScene(pf);
    }

    void scrollContentsBy(int x, int y) override {
        emit userMoved();
        this->QGraphicsView::scrollContentsBy(x, y);
    }

    void SetMaxSize(double size) {
        max_feature_size = size;
    }

    void SetShowFeatures(bool show) {
        show_features = show;
    }

    void UpdateFeaturesVisibility();

    void SetMatchedFeatures(std::set<int> matched) {
        this->matched = matched;
    }

    void SetOnlyShowMatched(bool checked) {
        only_show_matched = checked;
    }

    void Clear();
};