#include "MainWindow.h"
#include <QDebug>

MainWindow::MainWindow(QMainWindow *parent) : QMainWindow(parent) {
	m_gui.setupUi(this);
    QMainWindow::setWindowTitle("Gender detection");
    QMainWindow::setAttribute(Qt::WA_DeleteOnClose);
	m_loaddatawidget = new LoadDataWidget();
	m_trainwidget = new TrainWidget();
	m_brain = new Brain(this);


	QObject::connect(m_brain, &Brain::onReadedFile, this, &MainWindow::onReadedFile);



	// BUTTON CONNECTIONS
	QObject::connect(m_gui.pushButton_loadData, &QAbstractButton::clicked, m_loaddatawidget, &LoadDataWidget::show);
	QObject::connect(m_loaddatawidget, &LoadDataWidget::gotFiles, this, &MainWindow::onLoadData);


	QObject::connect(m_gui.pushButton_train, &QAbstractButton::clicked, m_trainwidget, &TrainWidget::show);
	QObject::connect(m_trainwidget, &TrainWidget::gotValues, this, &MainWindow::onTrain);


	QObject::connect(m_gui.pushButton_test, &QAbstractButton::clicked, this, &MainWindow::onTest);
	QObject::connect(m_gui.pushButton_testOnImage, &QAbstractButton::clicked, this, &MainWindow::onTestOnImage);

	QObject::connect(m_gui.pushButton_loadClassifier, &QAbstractButton::clicked, this, &MainWindow::onLoadClassifier);
	QObject::connect(m_gui.pushButton_saveClassifier, &QAbstractButton::clicked, this, &MainWindow::onSaveClassifier);
	// BUTTON CONNECTION END
}
MainWindow::~MainWindow(){
	//delete m_loaddatawidget;
	//delete m_trainwidget;
}

void MainWindow::onLoadData(const QString filename_test, const QString filename_train) {
    m_loaddatawidget->hide();
    
    m_gui.pushButton_loadClassifier->setEnabled(false);
    m_gui.pushButton_loadData->setEnabled(false);
    m_gui.pushButton_train->setEnabled(false);
    m_gui.label_classiefiedProcess->setText("files loading!");

	m_brain->loadData(filename_test, filename_train);
    
    
    m_gui.label_classiefiedProcess->setText("files loaded!");
    m_gui.pushButton_train->setEnabled(true);
    m_gui.pushButton_loadData->setEnabled(true);
    m_gui.pushButton_loadClassifier->setEnabled(true);
}


void MainWindow::onTrain(const double c, const double gamma)
{
    m_trainwidget->hide();
    
    m_gui.pushButton_loadClassifier->setEnabled(false);
    m_gui.pushButton_loadData->setEnabled(false);
    m_gui.pushButton_train->setEnabled(false);
    m_gui.pushButton_test->setEnabled(false);
    m_gui.pushButton_testOnImage->setEnabled(false);
    m_gui.pushButton_saveClassifier->setEnabled(false);
    m_gui.label_classiefiedProcess->setText("train is running!");
    
	m_brain->setTrain(c, gamma);
    
    
    
    m_gui.label_classiefiedProcess->setText("train finished!");
    m_gui.pushButton_test->setEnabled(true);
    m_gui.pushButton_testOnImage->setEnabled(true);
    m_gui.pushButton_saveClassifier->setEnabled(true);
    m_gui.pushButton_train->setEnabled(true);
    m_gui.pushButton_loadData->setEnabled(true);
    m_gui.pushButton_loadClassifier->setEnabled(true);
    
}


void MainWindow::onTest()
{
    m_gui.pushButton_test->setEnabled(false);
    m_gui.label_classiefiedProcess->setText("test running!");
    
	m_brain->test();
    
    m_gui.label_classiefiedProcess->setText("test finished!");
    m_gui.pushButton_test->setEnabled(true);
}


void MainWindow::onTestOnImage()
{
    
	QString filename = QFileDialog::getOpenFileName(
		this,
		"Open Image",
		QDir::currentPath(),
		"All files (*.*)",
		nullptr,
		QFileDialog::Option::DontUseNativeDialog);

	if (filename.isNull()) {
		QMessageBox::critical(this, "Error", "Datei konnte nicht gefunden werden!");
		return;
	}

    m_gui.label_classiefiedProcess->setText("test running on image!");
    m_gui.pushButton_testOnImage->setEnabled(false);
    
	m_brain->testOnImage(filename);
    
    m_gui.pushButton_testOnImage->setEnabled(true);
    

}


void MainWindow::onLoadClassifier()
{
	QString filename = QFileDialog::getOpenFileName(
		this,
		"Open Classifier",
		QDir::currentPath(),
		"Modal files (*.modal)",
		nullptr,
		QFileDialog::Option::DontUseNativeDialog);

	if (filename.isNull()) {
		QMessageBox::critical(this, "Error", "Datei konnte nicht gefunden werden!");
		return;
	}

    m_gui.pushButton_loadClassifier->setEnabled(false);
    m_gui.pushButton_testOnImage->setEnabled(false);
    m_gui.label_classiefiedProcess->setText("loading classiefier!");
    
    m_brain->loadClassifier(filename);
    
    m_gui.label_classiefiedProcess->setText("loaded classiefier!");
    m_gui.pushButton_loadClassifier->setEnabled(true);
    m_gui.pushButton_testOnImage->setEnabled(true);
}



void MainWindow::onSaveClassifier() {
	QString filename = QFileDialog::getSaveFileName(
		this,
		"Save Classifier",
		QDir::currentPath(),
		"Modal files (*.modal)",
		nullptr,
		QFileDialog::Option::DontUseNativeDialog);

	if (filename.isNull()) {
		QMessageBox::critical(this, "Error", "Datei konnte nicht gespeichert werden!");
		return;
	}
	else {
        m_gui.pushButton_saveClassifier->setEnabled(false);
        m_gui.label_classiefiedProcess->setText("saving classiefier!");
        
		m_brain->saveClassifier(filename);
        
        m_gui.label_classiefiedProcess->setText("saved classiefier!");
        m_gui.pushButton_saveClassifier->setEnabled(true);

	}

}

void MainWindow::onReadedFile(cv::Mat img, const QString filename, const QString gender, const int cvCountFaces) {
	m_gui.textEdit_status->moveCursor(QTextCursor::End);
    QString inputText(" file: %1\n gender: %2\n detected faces: %3\n");
	m_gui.textEdit_status->insertPlainText(inputText.arg(filename).arg(gender).arg(cvCountFaces));
	m_gui.textEdit_status->moveCursor(QTextCursor::End);

	QSize imgSizeQt(img.cols, img.rows);
	cv::Size imgSizeCv(img.cols, img.rows);
	
	imgQt = QImage(imgSizeQt, QImage::Format::Format_RGB888);
	cv::Mat header_imgQt(imgSizeCv, CV_8UC3, imgQt.bits());
	img.copyTo(header_imgQt);

    int w = m_gui.label_imageShow->width();
    int h = m_gui.label_imageShow->height();
	m_gui.label_imageShow->setPixmap(QPixmap::fromImage(imgQt).scaled(w,h,Qt::KeepAspectRatio));

    m_gui.label_classiefiedProcess->setText("Detected gender: "+gender);
    
	QApplication::processEvents();
}

