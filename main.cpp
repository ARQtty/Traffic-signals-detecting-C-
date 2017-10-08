#include <iostream>
#include "Classifier.h"
int main() {
    std::cout << "Hello, World!" << std::endl;
    Classifier myClassifier;
    cv::Mat img =cv::imread("/home/mostafa/Desktop/github/HackCV/test.jpg");
    //cv::imshow("img",img);
    //cv::waitKey(1000);
    //myClassifier.getTrainFeatures("/home/mostafa/Downloads/rtsd-r3/train/*.jpg");
    myClassifier.train("/home/mostafa/Downloads/rtsd-r3/train/*.jpg","/home/mostafa/Downloads/rtsd-r3/gt_train.csv");
    cv::Mat img_gray;
    cv::cvtColor(img,img_gray,cv::COLOR_RGB2GRAY);
    myClassifier.img=img_gray;
    cv::imshow("img",img_gray);
    cv::waitKey(1000);
    myClassifier.computeHOG();
    cv::Mat temp_img;
    cv::resize(img,temp_img,cv::Size(32,32));
    cv::Mat hog_img=myClassifier.visualizeHOG(temp_img,myClassifier.descriptorsValues,cv::Size(32,32));
    cv::resize(hog_img,temp_img,cv::Size(128,128));
    cv::imshow("HOG",temp_img);
    cv::waitKey(0);
    return 0;
}