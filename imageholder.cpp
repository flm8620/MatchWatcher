#include "imageholder.h"

ImageHolder::ImageHolder(QWidget* parent): QWidget(parent) {
    QHBoxLayout* l = new QHBoxLayout(this);
    left_view = new ImageView(nullptr, 0);
    right_view = new ImageView(nullptr, 1);
    l->addWidget(left_view);
    l->addWidget(right_view);
    line = new Line(left_view, right_view, this);
    line->setAttribute(Qt::WA_TransparentForMouseEvents);
    line->raise();
    connect(left_view, SIGNAL(userMoved()), this, SLOT(updateForce()));
    connect(right_view, SIGNAL(userMoved()), this, SLOT(updateForce()));
}

void ImageHolder::SetMaxFeatureSize(double size) {
    left_view->SetMaxSize(size);
    right_view->SetMaxSize(size);
}
