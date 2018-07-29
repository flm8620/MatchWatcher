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
    std::vector<std::pair<int, int>> matches;

    void paintEvent(QPaintEvent *e) override {
        QPainter p{ this };
        p.setRenderHint(QPainter::Antialiasing);
        p.setClipRect(this->rect());
        const QPointF offset_left = left_view->mapToParent({ 0,0 });
        const QPointF offset_right = right_view->mapToParent({ 0,0 });
        for (const auto& pair : matches) {
            QPointF pos1, pos2;
            const bool ok1 = left_view->GetFeaturePos(pair.first, pos1);
            const bool ok2 = right_view->GetFeaturePos(pair.second, pos2);
            if (ok1&&ok2) {
                p.drawLine(left_view->MapImagePointToWidget(pos1) + offset_left,
                    right_view->MapImagePointToWidget(pos1) + offset_right);
            }
        }
        //std::cout << e->rect().x() << " " << e->rect().y() << " " << e->rect().width() << " " << e->rect().height() << std::endl;
    }
public:
    Line(ImageView * left_view, ImageView * right_view, QWidget* parent = nullptr)
        : QWidget(parent), left_view(left_view), right_view(right_view) {
    }
    void set_matching_lines(const std::vector<std::pair<int, int>>& matches) {
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
    std::vector<std::pair<int, int>> matches;
public slots:
    void updateForce() {
        std::cout << "force update" << std::endl;
        left_view->viewport()->update();
        right_view->viewport()->update();
        update();
    }
public:
    ImageHolder(QWidget* parent = nullptr) : QWidget(parent) {
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

    void SetMatches(const std::vector<std::pair<int, int>>& matches) {
        this->matches = matches;
    }

    void SetMatchesToLines() {
        line->set_matching_lines(matches);
    }

    void paintEvent(QPaintEvent *e) override {

        SetMatchesToLines();
        this->QWidget::paintEvent(e);
    }
};