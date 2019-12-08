#pragma once
#include "ui_LoadDataWidget.h"
#include <QMessageBox>
#include <QFileDialog>
class LoadDataWidget : public QWidget
{
	Q_OBJECT
public:
	LoadDataWidget(QWidget *parent = 0);
	~LoadDataWidget();
Q_SIGNALS:
void gotFiles(const QString, const QString);
private:
	Ui::LoadDataWidget m_gui;
private Q_SLOTS:
	void onClickTraining();
	void onClickTest();
	void onClickedLoad();
};