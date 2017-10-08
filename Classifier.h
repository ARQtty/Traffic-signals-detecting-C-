//
// Created by mostafa on 07.10.17.
//

#ifndef HACKCV_CLASSIFIER_H
#define HACKCV_CLASSIFIER_H

#include <iostream>
#include <vector>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/ml.hpp"
#include "opencv2/objdetect.hpp"
#include <string>

using namespace cv::ml;
class Classifier {

public:
    cv::Mat img;            // container for the image
    cv::Mat sobel_x,sobel_y,img_HOG, gradeitnMag,gradientDir;

    cv::Ptr<SVM> svm;

    //*******************************************************************//
    // for HOG  //
    //*******************************************************************//
    std::vector< float> descriptorsValues;
    std::vector< cv::Point> locations;
    std::vector< std::vector< float> > v_descriptorsValues;
    std::vector< std::vector< cv::Point> > v_locations;


    Classifier();
    void computeHOG();
    cv::Mat visualizeHOG(const cv::Mat&, std::vector<float>&,const cv::Size&);

    void getTrainFeatures(std::string);
    void train(std::string,std::string);
};


#endif //HACKCV_CLASSIFIER_H
