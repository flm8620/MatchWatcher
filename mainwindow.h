#pragma once
#include "ui_mainwindow.h"
#include "imagescene.h"
class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

    ImageScene scene;

    void ApplyScene();
public:
    MainWindow(QWidget* parent = nullptr);
public slots:
    void on_actionOpen_triggered();
    void on_actionOpen_VisualSFM_mat_triggered();
    void WhenImageSelected(int row, int col);
    void WhenMatchedImageSelected(int row, int col);
};
