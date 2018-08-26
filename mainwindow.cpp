#include "mainwindow.h"
#include <QFileDialog>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include "FeaturePoints.h"
#include "MatchFile.h"

void MainWindow::ApplyScene() {
    const auto& index_to_image_file = scene.IndexToImageFile();
    //const auto& image_to_features = scene.ImageToFeatures();
    //const auto& image1_image2_matches = scene.Image1Image2Matches();
    this->image_list->clear();
    this->image_match_list->clear();
    this->image_list->setRowCount(index_to_image_file.size());
    int row = 0;
    for (int i = 0; i < index_to_image_file.size(); i++) {
        QTableWidgetItem *id = new QTableWidgetItem(tr("%1").arg(i));
        this->image_list->setItem(row, 0, id);
        QTableWidgetItem *file = new QTableWidgetItem(QFileInfo(index_to_image_file[i].c_str()).fileName().toStdString().c_str());
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

void MainWindow::on_actionOpen_VisualSFM_mat_triggered() {
    static QString last_file_name = "";
    QString file_name = QFileDialog::getOpenFileName(this, tr("Open VisualSFM nvm exportation"),
        last_file_name,
        tr("NVM (*.nvm)"));
    if (file_name.isNull()) return;
    last_file_name = file_name;

    QDir dir = QFileInfo(file_name).dir();

    std::ifstream infile(file_name.toStdString());
    if (!infile) return;
    std::string line;



    struct ImageInfo
    {
        std::string image_file, sift_file, mat_file;
        float focal, qw, qx, qy, qz, cx, cy, cz, r;
    };

    std::vector<ImageInfo> image_infos;

    std::vector<std::string> index_to_image_file;
    std::vector<std::vector<AbstractFeature>> image_to_features;
    std::vector<std::map<int, std::vector<std::pair<int, int>>>> image1_image2_matches;


    std::istringstream iss;
    int n_img;
    bool ok = false;
    while (std::getline(infile, line))
    {
        if (line.empty()) continue;
        if (line[0] == '#') continue;
        iss.clear(); iss.str(line);
        if (!(iss >> n_img)) continue;
        ok = true;
        break;
    }

    if (!ok) return;

    for (int i = 0; i < n_img; i++) {
        iss.clear(); std::getline(infile, line); iss.str(line);
        ImageInfo info;
        std::string img_name;
        int zero;
        if (!(iss >> img_name >> info.focal >>
            info.qw >> info.qx >> info.qy >> info.qz >>
            info.cx >> info.cy >> info.cz >> info.r >> zero)) return;
        if (zero != 0) return;
        const QFileInfo image_file = dir.filePath(img_name.c_str());
        info.image_file = image_file.filePath().toStdString();
        index_to_image_file.push_back(info.image_file);

        const QString image_base = image_file.completeBaseName();
        info.sift_file = dir.filePath(image_base + ".sift").toStdString();
        info.mat_file = dir.filePath(image_base + ".mat").toStdString();
        if (!QFileInfo(info.sift_file.c_str()).exists() || !QFileInfo(info.mat_file.c_str()).exists()) {
            std::cout << "Error ! mat or sift file of image missing!" << std::endl;
            return;
        }
        image_infos.emplace_back(info);
    }

    for (int im_id = 0; im_id < image_infos.size(); im_id++) {
        FeatureData data;
        QString sift_file = image_infos[im_id].sift_file.c_str();
        data.ReadSIFTB(sift_file.toStdString().c_str());

        FeatureData::LocationData& loc_data = *data._locData;
        auto ptr = Points<float>::TV5(loc_data);
        image_to_features.emplace_back();
        std::vector<AbstractFeature>& features = image_to_features.back();
        for (int i = 0; i < data._npoint; i++) {
            AbstractFeature f;
            float *p = *ptr;
            f.pos = QPointF(p[0], p[1]);
            f.scale = p[3];
            f.orient = p[4];
            features.push_back(f);
            ptr++;
        }
    }

    for (int im1 = 0; im1 < image_infos.size(); im1++) {
        image1_image2_matches.emplace_back();
        auto& im1_imn_matches = image1_image2_matches.back();
        for (int im2 = 0; im2 < image_infos.size(); im2++) {
            if (im1 == im2) continue;
            int n_putative;
            int n_inliner;
            QFileInfo im_1(image_infos[im1].image_file.c_str());
            std::string im1_name = im_1.dir().filePath(im_1.completeBaseName()).toStdString();
            MatchFile mat(im1_name.c_str()); 
            if (!mat.IsValid()) continue;
            QFileInfo im_2(image_infos[im2].image_file.c_str());
            std::string im2_name = im_2.dir().filePath(im_2.completeBaseName()).toStdString();
            if (!mat.GetMatchCount(im2_name.c_str(), n_putative, n_inliner)) continue;

            const int n_feature_im2 = image_to_features[im2].size();
            int n_putative_2;
            Points<int>	matches;
            if (!mat.GetPMatch(im2_name.c_str(), n_feature_im2, n_putative_2, matches)) continue;
            if (n_putative_2 != n_putative) continue;

            auto& im1_im2_matches = im1_imn_matches[im2];
            for (int k = 0; k < n_putative; k++)
                im1_im2_matches.emplace_back(matches[0][k], matches[1][k]);
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

    const auto& index_to_image_file = scene.IndexToImageFile();
    const auto& image_to_features = scene.ImageToFeatures();
    const auto& image_matches = scene.Image1Image2Matches()[img_id];
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

    this->image_holder->LoadImageLeft(QString(index_to_image_file.at(r).c_str()), image_to_features[img_id]);
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


    const auto& index_to_image_file = scene.IndexToImageFile();
    const auto& image_to_features = scene.ImageToFeatures();
    const std::vector<std::pair<int, int>>& image_matches = scene.Image1Image2Matches()[img1_id].at(img2_id);



    this->image_holder->LoadImageRight(QString(index_to_image_file.at(img2_id).c_str()), image_to_features[img2_id]);
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
