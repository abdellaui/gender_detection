#include "Brain.h"
#include <QDebug>
Brain::Brain(QWidget *parent) : QWidget(parent)
{
    m_cascadeClassiefier = cv::CascadeClassifier("/usr/local/share/opencv4/haarcascades//haarcascade_frontalface_alt.xml");
}

Brain::~Brain()
{
}

void Brain::loadData(const QString test, const QString training)
{
	
    m_trainer.clear();
    m_trainer_gender.clear();
    m_tester.clear();
    m_tester_gender.clear();
    
	qDebug() << "Brain::loadData # training" << training;
	readDir(training, m_trainer, m_trainer_gender);
    
    m_labeledData = shark::createLabeledDataFromRange(m_trainer, m_trainer_gender);
    
    qDebug() << "Brain::loadData # test:" << test;
    readDir(test, m_tester, m_tester_gender, false);
    m_tester_gender.resize(m_tester.size());
    m_labeledTester = shark::createLabeledDataFromRange(m_tester, m_tester_gender);
}


void Brain::readDir(const QString path, std::vector<shark::RealVector> & toPush, std::vector<unsigned int> & toPushGender, bool isTrain)
{
	QDir directory(path);
	QStringList images = directory.entryList(QStringList() << "*.jpg" << "*.JPG", QDir::Files);

	foreach(QString filename, images)
	{
		//TODO: Signal an Main und text ausgeben
		QString gender = readLable(filename, path);
        
		cv::Mat img = cv::imread(QString(path+"/"+filename).toStdString());
		if (img.empty())
			return;
		cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
		std::vector<cv::Rect> objekte;
		m_cascadeClassiefier.detectMultiScale(img, objekte);

		if (objekte.size()==1) {
            if(isTrain){
                unsigned int currentGender = (gender=="Female")?0:1;
                toPushGender.push_back(currentGender);
            }
			toPush.push_back(getHead(img, objekte.at(0)));
		}

		Q_EMIT(onReadedFile(img, filename, gender, objekte.size()));
	}
}

shark::RealVector Brain::getHead(cv::Mat img, cv::Rect objRect){
    cv::Mat head(img, objRect);
    cv::cvtColor(head, head, cv::COLOR_RGB2GRAY);
    cv::Size size(30, 30);
    cv::resize(head, head, size);
    cv::normalize(head, head, 0, 1, cv::NORM_MINMAX, CV_32F);
    
    
    std::vector<double> toPushingDouble;
    for (int i = 0; i < head.rows; i++) {
        for (int j = 0; j < head.cols; j++) {
            toPushingDouble.push_back(head.at<float>(i, j));
        }
    }
    
    shark::RealVector tmp(head.rows * head.cols);
    std::copy(tmp.begin(), tmp.end(), toPushingDouble.begin());
    return tmp;
}

QString Brain::readLable(const QString filename, const QString path)
{
	QStringList splitter = filename.split("_");
	QFile fileLabel(path + "/labels/" + splitter.at(0)+"/" + splitter.at(0) + ".txt");
	if (fileLabel.open(QIODevice::ReadOnly))
	{
		QString content = fileLabel.readAll();
		QStringList values = content.split("\n");
		foreach(QString lines, values)
		{
	
			QStringList keys = lines.split("=");
			if (keys.at(0) == "gender") {
				return keys.at(1);
			}
		}

	}
    return QString("");
}

void Brain::setTrain(const double c, const double gamma)
{
    qDebug()<<"Brain::setTrain # c:"<< c << " gamma:" << gamma;

    m_kernel = shark::GaussianRbfKernel<>(gamma);

	shark::CSvmTrainer<shark::RealVector> trainer(&m_kernel, c, true);
	trainer.train(m_classifier, m_labeledData);
    QMessageBox::information(this, "Alert", "Train finished");
}

void Brain::test()
{
    qDebug() << "Brain::test() # test";
	shark::Data<unsigned int> output = m_classifier(m_labeledTester.inputs());
	shark::ZeroOneLoss<unsigned int> loss;
	double error = loss.eval(m_labeledTester.labels(), output);
    QMessageBox::information(this, "Evalutation", QString("it is true: %1 %").arg(error*100));
}

void Brain::testOnImage(const QString imgPath)
{
    qDebug() << "Brain::testOnImage() # "<< imgPath;
    cv::Mat img = cv::imread(imgPath.toStdString());
    if (img.empty())
        return;
    cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
    std::vector<cv::Rect> objekte;
    m_cascadeClassiefier.detectMultiScale(img, objekte);
    
    if (objekte.size()==1)
    {
        unsigned int label = m_classifier(getHead(img, objekte.at(0)));
        QString gender = (label==0)?"Female":"Male";
        Q_EMIT(onReadedFile(img, imgPath, gender, objekte.size()));
    }
    else
    {
        QMessageBox::critical(this, "Fehler", "Kein Gesicht gefunden!");
    }
}

void Brain::loadClassifier(const QString classifierLoadPath)
{
    qDebug() << "Brain::loadClassifier() # " << classifierLoadPath;
    
    m_classifier = shark::KernelClassifier<shark::RealVector>(&m_kernel);
    std::ifstream ifs(classifierLoadPath.toStdString());
    shark::TextInArchive ia(ifs);
    m_classifier.read(ia);
    ifs.close();

}

void Brain::saveClassifier(const QString classiefierStorePath)
{
    qDebug() << "Brain::saveClassifier() # " << classiefierStorePath;

    std::ofstream ofs(classiefierStorePath.toStdString());
    shark::TextOutArchive oa(ofs);
    m_classifier.write(oa);
    ofs.close();
    

}
