#pragma once
#include "ui_mainwindow.h"
class MainWindow : public QMainWindow, private Ui::MainWindow
{
	Q_OBJECT
public:
	MainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
		setupUi(this);
	}
public slots:
	void on_actionOpen_triggered();
};
