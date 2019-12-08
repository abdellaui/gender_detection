#include "LoadDataWidget.h"

LoadDataWidget::LoadDataWidget(QWidget *parent) : QWidget(parent) {
	m_gui.setupUi(this);

    QWidget::setWindowTitle("Select files!");
    m_gui.lineEdit_test->setText("/Users/abdullah/Desktop/gender_detection/src/material/test");
    m_gui.lineEdit_training->setText("/Users/abdullah/Desktop/gender_detection/src/material/training");
    
	QObject::connect(m_gui.toolButton_test, &QAbstractButton::clicked, this, &LoadDataWidget::onClickTest);
	QObject::connect(m_gui.toolButton_training, &QAbstractButton::clicked, this, &LoadDataWidget::onClickTraining);
	QObject::connect(m_gui.pushButton_load, &QAbstractButton::clicked, this, &LoadDataWidget::onClickedLoad);
}

LoadDataWidget::~LoadDataWidget() {}

void LoadDataWidget::onClickTraining()
{

	QString filename = QFileDialog::getExistingDirectory(
		this,
		"Open Data",
		QDir::currentPath(),
		(QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks | QFileDialog::Option::DontUseNativeDialog));

	if (filename.isNull()) {
		QMessageBox::critical(this, "Error", "Datei konnte nicht gefunden werden!");
		return;
	}

	m_gui.lineEdit_training->setText(filename);
}

void LoadDataWidget::onClickTest()
{
	QString filename = QFileDialog::getExistingDirectory(
		this,
		"Open Data",
		QDir::currentPath(),
		(QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks | QFileDialog::Option::DontUseNativeDialog));

	if (filename.isNull()) {
		QMessageBox::critical(this, "Error", "Datei konnte nicht gefunden werden!");
		return;
	}
	m_gui.lineEdit_test->setText(filename);

}

void LoadDataWidget::onClickedLoad()
{

	if (m_gui.lineEdit_test->text().isEmpty() || m_gui.lineEdit_training->text().isEmpty()) {
		QMessageBox::critical(this, "Error", "Einer der Datein existiert nicht!");
		return;
	}
	Q_EMIT(gotFiles(m_gui.lineEdit_test->text(), m_gui.lineEdit_training->text()));
	QWidget::close();
}
