#pragma once
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPathItem>
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
    ImageScene * image_scene = nullptr;
    QGraphicsScene scene;
    QGraphicsPixmapItem* image_item = nullptr;
    QGraphicsPathItem* epipolar_line = nullptr;
    std::vector<std::pair<AbstractFeature, Marker*>> features;
    const int id;
    int current_img_idx_;
    double max_feature_size;
    bool show_features;
    bool only_show_matched;
    std::set<int> matched;
protected:
    void wheelEvent(QWheelEvent *) override;
    void mouseMoveEvent(QMouseEvent*) override;
signals:
    void userMoved();
    void mouseAt(double x, double y);
public slots:
    void zoomIn();
    void zoomOut();
public:
    ImageView(QWidget* parent = nullptr, int id = -1) : id(id), current_img_idx_(-1) {
        this->setScene(&scene);
        this->setDragMode(ScrollHandDrag);
        //this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        //this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        this->viewport()->setMouseTracking(true);
        this->setResizeAnchor(AnchorViewCenter);
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

    void DrawEpipolarLine(double a, double b, double c);

    void Clear();

    int current_img_idx() const { return current_img_idx_; }
};