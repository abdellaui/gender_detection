#pragma once
#include "ui_MainWindow.h"
#include <QObject>
#include <QFileDialog>
#include <QMessageBox>
#include "Brain.h"
#include "LoadDataWidget.h"
#include "TrainWidget.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public Q_SLOTS:
	void onLoadData(const QString filename_test, const QString filename_train);
	void onTrain(const double c, const double gamma);
	void onReadedFile(cv::Mat img, const QString filename, const QString gender, const int cvCountFaces);

	void onTest();
	void onTestOnImage();
	void onLoadClassifier();
	void onSaveClassifier();

public:
	MainWindow(QMainWindow *parent = 0);
	~MainWindow();
private:
	Ui::MainWindow m_gui;
	Brain * m_brain;
	LoadDataWidget * m_loaddatawidget;
	TrainWidget * m_trainwidget;
	QImage imgQt;
};
