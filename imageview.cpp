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

void ImageView::LoadImage(const QString& file) {
    Clear();
    QPixmap img(file);
    if (img.isNull()) return;
    image_item = new QGraphicsPixmapItem(img);
    this->scene.addItem(image_item);
    this->fitViewAllObject();
}

void ImageView::LoadFeatures(const std::vector<AbstractFeature>& features) {
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
    const QRectF rect_large(rect.topLeft() - size, rect.bottomRight() + size);
    fitInView(rect_large, Qt::KeepAspectRatio);
}

void ImageView::SetMaxSize(double size) {
    for (const auto& it : features) {
        const auto& f = it.first;
        it.second->setVisible(f.scale < size);
    }
}

void ImageView::Clear() {
    this->scene.clear();
    features.clear();
    image_item = nullptr;
}
