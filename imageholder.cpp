#include "imageholder.h"

void ImageHolder::DrawEpipolarOnRight(double x, double y) {
    if(!show_epipolar) return;
    if (right_view->current_img_idx() < 0) return;
    Eigen::Vector3d x1(x, y, 1.);
    Eigen::Vector3d Fx1 = fundamental_matrix * x1;
    const double a = Fx1[0], b = Fx1[1], c = Fx1[2];

    right_view->DrawEpipolarLine(a, b, c);

    double xx, yy;
    if (std::abs(a) > std::abs(b)) {
        yy = 0.; xx = -c / a;
    }
    else {
        xx = 0.; yy = -c / b;
    }
    Eigen::Vector3d x2(xx, yy, 1.);
    Eigen::Vector3d x2F = x2.transpose() * fundamental_matrix;
    left_view->DrawEpipolarLine(x2F[0], x2F[1], x2F[2]);
}

void ImageHolder::DrawEpipolarOnLeft(double x, double y) {
    if(!show_epipolar) return;
    if (left_view->current_img_idx() < 0) return;
    Eigen::Vector3d x2(x, y, 1.);
    Eigen::Vector3d x2F = x2.transpose() * fundamental_matrix;
    const double a = x2F[0], b = x2F[1], c = x2F[2];
    left_view->DrawEpipolarLine(x2F[0], x2F[1], x2F[2]);

    double xx, yy;
    if (std::abs(a) > std::abs(b)) {
        yy = 0.; xx = -c / a;
    }
    else {
        xx = 0.; yy = -c / b;
    }
    Eigen::Vector3d x1(xx, yy, 1.);
    Eigen::Vector3d Fx1 = fundamental_matrix * x1;
    right_view->DrawEpipolarLine(Fx1[0], Fx1[1], Fx1[2]);
}

ImageHolder::ImageHolder(QWidget* parent) : QWidget(parent) {
    QHBoxLayout* l = new QHBoxLayout(this);
    left_view = new ImageView(nullptr, 0);
    right_view = new ImageView(nullptr, 1);
    l->addWidget(left_view);
    l->addWidget(right_view);
    line = new Line(left_view, right_view, this);
    line->setAttribute(Qt::WA_TransparentForMouseEvents);
    line->raise();
    connect(left_view, &ImageView::userMoved, this, &ImageHolder::updateForce);
    connect(right_view, &ImageView::userMoved, this, &ImageHolder::updateForce);
    connect(left_view, &ImageView::mouseAt, this, &ImageHolder::DrawEpipolarOnRight);
    connect(right_view, &ImageView::mouseAt, this, &ImageHolder::DrawEpipolarOnLeft);
}
