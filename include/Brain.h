#pragma once
#include <QObject>
#include <QMessageBox>
#include <QDir>
#include <opencv2/opencv.hpp>
#include <shark/LinAlg/Base.h>
#include <shark/Models/Kernels/GaussianRbfKernel.h>
#include <shark/Algorithms/Trainers/CSvmTrainer.h>
#include <shark/Data/Dataset.h>
#include <shark/Data/DataDistribution.h>
#include <shark/Data/Csv.h>
#include <shark/Data/SparseData.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
class Brain : public QWidget{

	Q_OBJECT

public:
	Brain(QWidget *parent = 0);
	~Brain();

	void loadData(const QString test, const QString training);


	void setTrain(const double c, const double gamma);
	void test();
	void testOnImage(const QString imgPath);


	void loadClassifier(const QString classifierLoadPath);
	void saveClassifier(const QString classiefierStorePath);
private:
    cv::CascadeClassifier m_cascadeClassiefier;
    
	std::vector<shark::RealVector> m_trainer;
	std::vector<unsigned int> m_trainer_gender;
    shark::LabeledData<shark::RealVector, unsigned int> m_labeledData;
    
	std::vector<shark::RealVector> m_tester;
	std::vector<unsigned int> m_tester_gender;
    shark::LabeledData<shark::RealVector, unsigned int> m_labeledTester;
    
    
	shark::KernelClassifier<shark::RealVector> m_classifier;
	shark::GaussianRbfKernel<> m_kernel;
    
    
    shark::RealVector getHead(cv::Mat img, cv::Rect objRect);
    
    void readDir(const QString path, std::vector<shark::RealVector> &, std::vector<unsigned> &, bool isTrain = true);
    QString readLable(const QString filename, const QString path);
    
Q_SIGNALS:
	void onReadedFile(cv::Mat, const QString, const QString, const int);
};
