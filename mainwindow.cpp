#include "mainwindow.h"
#include <QFileDialog>
void MainWindow::on_actionOpen_triggered() {
    static QString last_file_name = "";
    QString file_name = QFileDialog::getOpenFileName(this, tr("Open Image"),
        last_file_name,
        tr("Images (*.png *.xpm *.jpg)"));
    if (file_name.isNull()) return;
    last_file_name = file_name;


    std::vector<AbstractFeature> features;
    features.push_back({ {0.,0.}, 1.,0.,0. });
    features.push_back({ {100.,0.}, 1.,0.,0. });
    features.push_back({ {100.,100.}, 1.,0.,0. });
    features.push_back({ {200.,100.}, 1.,0.,0. });
    this->image_holder->LoadImageLeft(file_name, features);
    this->image_holder->LoadImageRight(file_name, features);


    std::vector<std::pair<int, int>> matches;
    matches.emplace_back(0, 0);
    matches.emplace_back(0, 1);
    matches.emplace_back(1, 1);
    matches.emplace_back(2, 2);
    this->image_holder->SetMatches(matches);
}
