#pragma once
#include "ui_TrainWidget.h"


class TrainWidget : public QWidget
{
	Q_OBJECT
public:
	TrainWidget(QWidget *parent = 0);
	~TrainWidget();

Q_SIGNALS:
	void gotValues(double, double);
private:
	Ui::TrainWidget m_gui;
private Q_SLOTS:
	void onClickedTrain();
};