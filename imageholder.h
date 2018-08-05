#pragma once
#include <QHBoxLayout>
#include "imageview.h"
#include <iostream>
#include <QPaintEvent>
class Line : public QWidget {
    Q_OBJECT
private:
    ImageView * left_view;
    ImageView * right_view;
    std::map<int, int> matches;

    void paintEvent(QPaintEvent *e) override {
        QPainter p{ this };
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
                if(p1.x() < 0.f || p1.y() < 0.f || p1.x() > left_view->rect().width() || p1.y() > left_view->rect().height())
                    continue;
                if(p2.x() < 0.f || p2.y() < 0.f || p2.x() > right_view->rect().width() || p2.y() > right_view->rect().height())
                    continue;
                p.drawLine(p1 + offset_left, p2 + offset_right);
            }
        }
    }
public:
    Line(ImageView * left_view, ImageView * right_view, QWidget* parent = nullptr)
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
    ImageView * left_view;
    ImageView * right_view;
    Line *line;
    std::map<int, int> matches;
public slots:
    void updateForce() {
        left_view->viewport()->update();
        right_view->viewport()->update();
        update();
    }
public:
    ImageHolder(QWidget* parent = nullptr);

    void resizeEvent(QResizeEvent *) override {
        line->setGeometry(rect());
    }

    void LoadImageLeft(const QString& filename, const std::vector<AbstractFeature>& features) {
        left_view->LoadImage(filename);
        left_view->LoadFeatures(features);
    }

    void LoadImageRight(const QString& filename, const std::vector<AbstractFeature>& features) {
        right_view->LoadImage(filename);
        right_view->LoadFeatures(features);
    }

    void SetMatches(const std::map<int, int>& matches) {
        this->matches = matches;
    }

    void SetMatchesToLines() {
        line->set_matching_lines(matches);
    }

    void paintEvent(QPaintEvent *e) override {
        SetMatchesToLines();
        this->QWidget::paintEvent(e);
    }

    void SetMaxFeatureSize(double size);
};
