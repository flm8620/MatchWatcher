#include "imageview.h"
#include <QWheelEvent>
void ImageView::wheelEvent(QWheelEvent *e) {
    if (e->delta() > 0)
        this->zoomIn();
    else
        this->zoomOut();
    e->accept();
    emit userMoved();
}

void ImageView::mouseMoveEvent(QMouseEvent* e) {
    QGraphicsView::mouseMoveEvent(e);
    if (image_item) {
        QPointF p = mapToScene(e->pos());
        QPointF p_img = image_item->mapFromScene(p);
        emit mouseAt(p_img.x(), p_img.y());
    }
}

void ImageView::zoomIn() {
    QTransform t = this->transform();
    t.scale(1.25, 1.25);
    this->setTransform(t);
}

void ImageView::zoomOut() {
    QTransform t = this->transform();
    t.scale(0.8, 0.8);
    this->setTransform(t);
}

void ImageView::LoadImage(int idx) {
    Clear();
    current_img_idx_ = idx;
    QPixmap img(image_scene->Images()[idx].image_file.c_str());
    if (img.isNull()) return;
    image_item = new QGraphicsPixmapItem(img);
    this->scene.addItem(image_item);
    this->fitViewAllObject();

    const std::vector<AbstractFeature>& features = image_scene->ImageToFeatures()[idx];
    for (int i = 0; i < features.size(); i++) {
        const auto& f = features[i];
        auto* marker = new Marker(i);
        marker->setPos(f.pos);
        this->scene.addItem(marker);
        marker->setScale(f.scale / 10.);
        this->features.emplace_back(f, marker);
    }
}

void ImageView::fitViewAllObject() {
    const QRectF rect = scene.sceneRect();
    const QPointF size = QPointF(rect.width(), rect.height());
    const QRectF rect_large(rect.topLeft() - size * 10, rect.bottomRight() + size * 10);
    scene.setSceneRect(rect_large);
    fitInView(rect, Qt::KeepAspectRatio);
}

void ImageView::UpdateFeaturesVisibility() {
    if (!show_features) {
        for (const auto& it : features)
            it.second->setVisible(false);
    }
    else {
        if (!only_show_matched)
            for (const auto& it : features) {
                const auto& f = it.first;
                it.second->setVisible(f.scale < max_feature_size);
            }
        else {
            for (auto& feature : features)
                feature.second->setVisible(false);
            for (int i : matched)
                features[i].second->setVisible(true);
        }
    }
}

void ImageView::DrawEpipolarLine(double a, double b, double c) { // ax + by + c = 0
    delete epipolar_line;
    epipolar_line = new QGraphicsPathItem;
    int width = image_item->pixmap().width();
    int height = image_item->pixmap().height();
    double x0 = -width, x1 = 2 * width, y0 = -height, y1 = 2 * height;
    std::vector<QPointF> intersections;
    double x, y;
    y = (-a * x0 - c) / b;
    if (y <= y1 && y >= y0) intersections.push_back({ x0, y });
    y = (-a * x1 - c) / b;
    if (y <= y1 && y >= y0) intersections.push_back({ x1, y });
    x = (-b * y0 - c) / a;
    if (x <= x1 && x >= x0) intersections.push_back({ x, y0 });
    x = (-b * y1 - c) / a;
    if (x <= x1 && x >= x0) intersections.push_back({ x, y1 });
    if (intersections.size() == 2) {
        QPainterPath path;
        path.moveTo(intersections[0]);
        path.lineTo(intersections[1]);
        epipolar_line->setPath(path);
        epipolar_line->setPen(QColor(255, 0, 0));
        epipolar_line->pen().setCosmetic(true);
        //epipolar_line->pen().setWidth(10);//int(std::max(0.1, 1.0 / this->transform().m11())));
        this->scene.addItem(epipolar_line);
    }
}

void ImageView::Clear() {
    current_img_idx_ = -1;
    matched.clear();
    this->scene.clear();
    features.clear();
    image_item = nullptr;
    epipolar_line = nullptr;
}
