#include "mainwindow.h"
#include <QFileDialog>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include "FeaturePoints.h"

void MainWindow::ApplyScene() {
    const std::map<int, std::string>& index_to_image_file = scene.IndexToImageFile();
    const std::map<int, std::map<int, QPointF>>& image_to_features = scene.ImageToFeatures();
    const std::map<int, std::map<int, std::vector<std::pair<int, int>>>>& image1_image2_matches = scene.Image1Image2Matches();
    this->image_list->clear();
    this->image_match_list->clear();
    this->image_list->setRowCount(index_to_image_file.size());
    int row = 0;
    for (const auto& it : index_to_image_file) {
        QTableWidgetItem *id = new QTableWidgetItem(tr("%1").arg(it.first));
        this->image_list->setItem(row, 0, id);
        QTableWidgetItem *file = new QTableWidgetItem(QFileInfo(it.second.c_str()).fileName().toStdString().c_str());
        this->image_list->setItem(row, 1, file);
        row++;
    }
    this->image_list->resizeColumnsToContents();
}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setupUi(this);
    connect(this->image_list, &QTableWidget::currentCellChanged, this, &MainWindow::WhenImageSelected);
    connect(this->image_match_list, &QTableWidget::currentCellChanged, this, &MainWindow::WhenMatchedImageSelected);
}

void MainWindow::on_actionOpen_triggered() {
    static QString last_file_name = "";
    QString file_name = QFileDialog::getOpenFileName(this, tr("Open Image"),
        last_file_name,
        tr("Images (*.png *.xpm *.jpg)"));
    if (file_name.isNull()) return;
    last_file_name = file_name;


    std::vector<AbstractFeature> features(4);
    features[0] = { {0.,0.}, 1.,0. };
    features[1] = { {100.,0.}, 1.,0. };
    features[2] = { {100.,100.}, 1.,0. };
    features[3] = { {200.,100.}, 1.,0. };
    this->image_holder->LoadImageLeft(file_name, features);
    this->image_holder->LoadImageRight(file_name, features);


    std::map<int, int> matches;
    matches[0] = 0;
    matches[1] = 1;
    this->image_holder->SetMatches(matches);
}

void MainWindow::on_actionOpen_VisualSFM_mat_triggered() {
    static QString last_file_name = "";
    QString file_name = QFileDialog::getOpenFileName(this, tr("Open VisualSFM matches exportation"),
        last_file_name,
        tr("matches (*.txt)"));
    if (file_name.isNull()) return;
    last_file_name = file_name;

    std::ifstream infile(file_name.toStdString());
    if (!infile) return;
    std::string line;

    std::map<int, std::string> index_to_image_file;
    std::map<int, std::map<int, QPointF>> image_to_features;
    std::map<int, std::map<int, std::vector<std::pair<int, int>>>> image1_image2_matches;

    while (std::getline(infile, line))
    {
        if (line.empty()) continue;
        if (line[0] == '#') continue;
        std::istringstream iss(line);


        int id1, id2, n;
        std::string img1, img2;

        if (!(iss >> id1 >> img1)) break;
        iss.clear(); std::getline(infile, line); iss.str(line);
        if (!(iss >> id2 >> img2)) break;
        iss.clear(); std::getline(infile, line); iss.str(line);
        if (!(iss >> n)) break;

        index_to_image_file[id1] = img1;
        index_to_image_file[id2] = img2;

        auto& img_f1 = image_to_features[id1];
        auto& img_f2 = image_to_features[id2];

        for (int i = 0; i < n; i++) {
            iss.clear(); std::getline(infile, line); iss.str(line);
            int f1, f2;
            float x1, y1, x2, y2;
            if (!(iss >> f1 >> x1 >> y1 >> f2 >> x2 >> y2)) break;

            img_f1[f1] = QPointF(x1, y1);
            img_f2[f2] = QPointF(x2, y2);

            image1_image2_matches[id1][id2].emplace_back(f1, f2);
            image1_image2_matches[id2][id1].emplace_back(f2, f1);
        }
    }

    scene.setScene(index_to_image_file, image_to_features, image1_image2_matches);
    ApplyScene();
}

void MainWindow::WhenImageSelected(int r, int c) {
    const auto item = this->image_list->item(r, 0);
    bool ok;
    const int img_id = item->text().toInt(&ok);
    if (!ok) return;

    const std::map<int, std::string>& index_to_image_file = scene.IndexToImageFile();
    const std::map<int, std::map<int, QPointF>>& image_to_features = scene.ImageToFeatures();
    const std::map<int, std::vector<std::pair<int, int>>>& image_matches = scene.Image1Image2Matches().at(img_id);
    this->image_match_list->clear();
    this->image_match_list->setRowCount(image_matches.size());
    int row = 0;
    for (const auto& it : image_matches) {
        QTableWidgetItem *id2 = new QTableWidgetItem(tr("%1").arg(it.first));
        this->image_match_list->setItem(row, 0, id2);
        QTableWidgetItem *file = new QTableWidgetItem(QFileInfo(index_to_image_file.at(it.first).c_str()).fileName().toStdString().c_str());
        this->image_match_list->setItem(row, 1, file);
        QTableWidgetItem *count = new QTableWidgetItem(tr("%1").arg(it.second.size()));
        this->image_match_list->setItem(row, 2, count);
        row++;
    }
    this->image_match_list->resizeColumnsToContents();

    FeatureData data;
    QFileInfo img_file(index_to_image_file.at(img_id).c_str());
    QString sift_file = img_file.dir().filePath(img_file.completeBaseName() + ".sift");
    data.ReadSIFTB(sift_file.toStdString().c_str());

    FeatureData::LocationData& loc_data = *data._locData;
    auto ptr = Points<float>::TV5(loc_data);
    std::vector<AbstractFeature> features;
    for (int i = 0; i < data._npoint; i++) {
        AbstractFeature f;
        float *p = *ptr;
        f.pos = QPointF(p[0], p[1]);
        f.scale = p[3];
        f.orient = p[4];
        features.push_back(f);
        ptr++;
    }


    this->image_holder->LoadImageLeft(QString(index_to_image_file.at(r).c_str()), features);
    this->image_holder->updateForce();
}

void MainWindow::WhenMatchedImageSelected(int r, int c) {
    if (r < 0 || c < 0) return;
    const int row1 = this->image_list->currentRow();
    const auto item1 = this->image_list->item(row1, 0);
    const auto item2 = this->image_match_list->item(r, 0);
    bool ok;
    const int img2_id = item2->text().toInt(&ok);
    if (!ok) return;
    const int img1_id = item1->text().toInt(&ok);
    if (!ok) return;


    const std::map<int, std::string>& index_to_image_file = scene.IndexToImageFile();
    const std::map<int, std::map<int, QPointF>>& image_to_features = scene.ImageToFeatures();
    const std::vector<std::pair<int, int>>& image_matches = scene.Image1Image2Matches().at(img1_id).at(img2_id);


    FeatureData data;
    QFileInfo img2_file(index_to_image_file.at(img2_id).c_str());
    QString sift_file = img2_file.dir().filePath(img2_file.completeBaseName() + ".sift");
    data.ReadSIFTB(sift_file.toStdString().c_str());

    FeatureData::LocationData& loc_data = *data._locData;
    auto ptr = Points<float>::TV5(loc_data);
    std::vector<AbstractFeature> features;
    for (int i = 0; i < data._npoint; i++) {
        AbstractFeature f;
        float *p = *ptr;
        f.pos = QPointF(p[0], p[1]);
        f.scale = p[3];
        f.orient = p[4];
        features.push_back(f);
        ptr++;
    }

    this->image_holder->LoadImageRight(QString(index_to_image_file.at(img2_id).c_str()), features);
    std::map<int, int> matches;
    for (const auto it : image_matches) {
        matches[it.first] = it.second;
    }
    this->image_holder->SetMatches(matches);
    this->image_holder->updateForce();
}

void MainWindow::on_size_slider_valueChanged(int size) {
    this->image_holder->SetMaxFeatureSize(std::pow(10.0, double(size) / 100.*5.0));
    this->image_holder->updateForce();
}

void MainWindow::on_button_hide_feature_marker_toggled(bool checked) {
    this->image_holder->SetShowFeatures(!checked);
    this->image_holder->updateForce();
}

void MainWindow::on_button_show_only_matched_toggled(bool checked) {
    this->image_holder->SetOnlyShowMatchedFeatures(checked);
    this->image_holder->updateForce();
}
