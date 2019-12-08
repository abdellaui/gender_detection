#include "TrainWidget.h"

TrainWidget::TrainWidget(QWidget *parent) : QWidget(parent) {
	m_gui.setupUi(this);
    QWidget::setWindowTitle("Put valuess!");
	QObject::connect(m_gui.pushButton_train, &QAbstractButton::clicked, this, &TrainWidget::onClickedTrain);

}


TrainWidget::~TrainWidget() {}


void TrainWidget::onClickedTrain()
{

	Q_EMIT(gotValues(m_gui.doubleSpinBox_c->value(), m_gui.doubleSpinBox_gamma->value()));
    QWidget::close();
}
