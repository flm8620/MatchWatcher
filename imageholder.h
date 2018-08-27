#pragma once
#include <QHBoxLayout>
#include <iostream>
#include <QPaintEvent>
#include "imageview.h"
#include <Eigen/Dense>
class Line : public QWidget {
    Q_OBJECT
private:
    ImageView * left_view;
    ImageView * right_view;
    std::map<int, int> matches;

    void paintEvent(QPaintEvent *e) override {
        QPainter p{ this };
        p.setPen(QColor(0, 255, 0));
        p.setRenderHint(QPainter::Antialiasing);
        p.setClipRect(this->rect());
        const QPointF offset_left = left_view->mapToParent({ 0,0 });
        const QPointF offset_right = right_view->mapToParent({ 0,0 });
        for (const auto& m : matches) {
            QPointF pos1, pos2;
            const bool ok1 = left_view->GetFeaturePos(m.first, pos1);
            const bool ok2 = right_view->GetFeaturePos(m.second, pos2);
            if (ok1&&ok2) {
                const QPointF p1 = left_view->MapImagePointToWidget(pos1);
                const QPointF p2 = right_view->MapImagePointToWidget(pos2);
                if (p1.x() < 0.f || p1.y() < 0.f || p1.x() > left_view->rect().width() || p1.y() > left_view->rect().height())
                    continue;
                if (p2.x() < 0.f || p2.y() < 0.f || p2.x() > right_view->rect().width() || p2.y() > right_view->rect().height())
                    continue;
                p.drawLine(p1 + offset_left, p2 + offset_right);
            }
        }
    }
public:
    Line(ImageView* left_view, ImageView* right_view, QWidget* parent = nullptr)
        : QWidget(parent), left_view(left_view), right_view(right_view) {
    }
    void set_matching_lines(const std::map<int, int>& matches) {
        this->matches = matches;
    }
};

class ImageHolder : public QWidget
{
    Q_OBJECT
private:
    ImageScene * scene;

    ImageView * left_view;
    ImageView * right_view;
    Line *line;
    std::map<int, int> matches;
    Eigen::Matrix3d fundamental_matrix;
public slots:
    void updateForce() {
        left_view->viewport()->update();
        right_view->viewport()->update();
        update();
    }

    void DrawEpipolarOnRight(double x, double y);
    void DrawEpipolarOnLeft(double x, double y);
public:
    ImageHolder(QWidget* parent = nullptr);

    void set_scene(ImageScene* scene) {
        this->scene = scene;
        left_view->set_scene(scene);
        right_view->set_scene(scene);
    }

    void resizeEvent(QResizeEvent *) override {
        line->setGeometry(rect());
    }

    void LoadImageLeft(int idx) {
        left_view->LoadImage(idx);
        if(right_view->current_img_idx()>=0) {
            fundamental_matrix = ImageInfo::calcFundamentalMatrix(scene->Images()[idx], scene->Images()[right_view->current_img_idx()]);
        }
    }

    void LoadImageRight(int idx) {
        right_view->LoadImage(idx);
        if(left_view->current_img_idx()>=0) {
            fundamental_matrix = ImageInfo::calcFundamentalMatrix(scene->Images()[left_view->current_img_idx()], scene->Images()[idx]);
        }
    }

    void SetMatches(const std::map<int, int>& matches) {
        this->matches = matches;
        std::set<int> img1, img2;
        for (const auto it : matches) {
            img1.insert(it.first);
            img2.insert(it.second);
        }
        left_view->SetMatchedFeatures(img1);
        right_view->SetMatchedFeatures(img2);
        UpdateFeatureVisibility();
    }

    void SetMatchesToLines() {
        line->set_matching_lines(matches);
    }

    void paintEvent(QPaintEvent *e) override {
        SetMatchesToLines();
        this->QWidget::paintEvent(e);
    }

    void SetMaxFeatureSize(double size) {
        left_view->SetMaxSize(size);
        right_view->SetMaxSize(size);
        UpdateFeatureVisibility();
    }

    void SetShowFeatures(bool show) {
        left_view->SetShowFeatures(show);
        right_view->SetShowFeatures(show);
        UpdateFeatureVisibility();
    }

    void SetOnlyShowMatchedFeatures(bool checked) {
        left_view->SetOnlyShowMatched(checked);
        right_view->SetOnlyShowMatched(checked);
        UpdateFeatureVisibility();
    }

    void UpdateFeatureVisibility() {
        left_view->UpdateFeaturesVisibility();
        right_view->UpdateFeaturesVisibility();
    }
};
