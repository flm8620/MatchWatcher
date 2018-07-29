#include "imageview.h"
#include <QWheelEvent>
#include "marker.h"
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

void ImageView::invalidateAll() {
    std::cout << id << " invalidate all" << std::endl;
    this->invalidateScene(this->rect());
}

void ImageView::LoadImage(const QString& file) {
    QPixmap img(file);
    if (img.isNull()) return;
    image_item = new QGraphicsPixmapItem(img);
    this->scene.clear();
    this->scene.addItem(image_item);
    this->fitViewAllObject();
}

void ImageView::LoadFeatures(const std::vector<AbstractFeature>& features) {
    this->features = features;
    for (int i = 0; i < this->features.size(); i++) {
        const auto& f = this->features[i];
        auto* marker = new Marker(i);
        marker->setPos(f.pos);
        this->scene.addItem(marker);
    }
}

void ImageView::fitViewAllObject() {
    fitInView(scene.sceneRect(), Qt::KeepAspectRatio);
}
