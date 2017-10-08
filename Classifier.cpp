//
// Created by mostafa on 07.10.17.
//

#include "Classifier.h"
#include <sstream>


Classifier::Classifier() {
    svm=SVM::create();
    svm->setType(SVM::C_SVC);
    svm->setKernel(SVM::LINEAR);
    svm->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER,100,1e-6));

    std::cout<<"SVM is ready"<<std::endl;
    std::cout<<svm->getType()<<std::endl;
}

void Classifier::computeHOG() {
    cv::Mat img_resized;
    cv::resize(img,img_resized,cv::Size(64,64));
    cv::HOGDescriptor hog(cv::Size(32,32),cv::Size(16,16),cv::Size(8,8),cv::Size(8,8),9);
    hog.compute(img_resized,descriptorsValues,cv::Size(0,0),cv::Size(0,0),locations);

    //std::cout<<descriptorsValues.size()<<std::endl;
    //std::cout<<locations<<std::endl;

}

cv::Mat Classifier::visualizeHOG(const cv::Mat &color_origImg, std::vector<float> &descriptorValues, const cv::Size &size) {
    {

        // http://www.juergenbrauer.org/old_wiki/doku.php?id=public:hog_descriptor_computation_and_visualization
        const int DIMX = size.width;
        const int DIMY = size.height;
        float zoomFac = 3;
        cv::Mat visu;
        resize(color_origImg, visu, cv::Size( (int)(color_origImg.cols*zoomFac), (int)(color_origImg.rows*zoomFac) ) );

        int cellSize        = 8;
        int gradientBinSize = 9;
        float radRangeForOneBin = (float)(CV_PI/(float)gradientBinSize); // dividing 180 into 9 bins, how large (in rad) is one bin?

        // prepare data structure: 9 orientation / gradient strenghts for each cell
        int cells_in_x_dir = DIMX / cellSize;
        int cells_in_y_dir = DIMY / cellSize;
        float*** gradientStrengths = new float**[cells_in_y_dir];
        int** cellUpdateCounter   = new int*[cells_in_y_dir];
        for (int y=0; y<cells_in_y_dir; y++)
        {
            gradientStrengths[y] = new float*[cells_in_x_dir];
            cellUpdateCounter[y] = new int[cells_in_x_dir];
            for (int x=0; x<cells_in_x_dir; x++)
            {
                gradientStrengths[y][x] = new float[gradientBinSize];
                cellUpdateCounter[y][x] = 0;

                for (int bin=0; bin<gradientBinSize; bin++)
                    gradientStrengths[y][x][bin] = 0.0;
            }
        }

        // nr of blocks = nr of cells - 1
        // since there is a new block on each cell (overlapping blocks!) but the last one
        int blocks_in_x_dir = cells_in_x_dir - 1;
        int blocks_in_y_dir = cells_in_y_dir - 1;

        // compute gradient strengths per cell
        int descriptorDataIdx = 0;
        int cellx = 0;
        int celly = 0;

        for (int blockx=0; blockx<blocks_in_x_dir; blockx++)
        {
            for (int blocky=0; blocky<blocks_in_y_dir; blocky++)
            {
                // 4 cells per block ...
                for (int cellNr=0; cellNr<4; cellNr++)
                {
                    // compute corresponding cell nr
                    cellx = blockx;
                    celly = blocky;
                    if (cellNr==1) celly++;
                    if (cellNr==2) cellx++;
                    if (cellNr==3)
                    {
                        cellx++;
                        celly++;
                    }

                    for (int bin=0; bin<gradientBinSize; bin++)
                    {
                        float gradientStrength = descriptorValues[ descriptorDataIdx ];
                        descriptorDataIdx++;

                        gradientStrengths[celly][cellx][bin] += gradientStrength;

                    } // for (all bins)


                    // note: overlapping blocks lead to multiple updates of this sum!
                    // we therefore keep track how often a cell was updated,
                    // to compute average gradient strengths
                    cellUpdateCounter[celly][cellx]++;

                } // for (all cells)


            } // for (all block x pos)
        } // for (all block y pos)


        // compute average gradient strengths
        for (celly=0; celly<cells_in_y_dir; celly++)
        {
            for (cellx=0; cellx<cells_in_x_dir; cellx++)
            {

                float NrUpdatesForThisCell = (float)cellUpdateCounter[celly][cellx];

                // compute average gradient strenghts for each gradient bin direction
                for (int bin=0; bin<gradientBinSize; bin++)
                {
                    gradientStrengths[celly][cellx][bin] /= NrUpdatesForThisCell;
                }
            }
        }

        // draw cells
        for (celly=0; celly<cells_in_y_dir; celly++)
        {
            for (cellx=0; cellx<cells_in_x_dir; cellx++)
            {
                int drawX = cellx * cellSize;
                int drawY = celly * cellSize;

                int mx = drawX + cellSize/2;
                int my = drawY + cellSize/2;

                rectangle(visu, cv::Point((int)(drawX*zoomFac), (int)(drawY*zoomFac)), cv::Point((int)((drawX+cellSize)*zoomFac), (int)((drawY+cellSize)*zoomFac)), cv::Scalar(100,100,100), 1);

                // draw in each cell all 9 gradient strengths
                for (int bin=0; bin<gradientBinSize; bin++)
                {
                    float currentGradStrength = gradientStrengths[celly][cellx][bin];

                    // no line to draw?
                    if (currentGradStrength==0)
                        continue;

                    float currRad = bin * radRangeForOneBin + radRangeForOneBin/2;

                    float dirVecX = cos( currRad );
                    float dirVecY = sin( currRad );
                    float maxVecLen = (float)(cellSize/2.f);
                    float scale = 2.5; // just a visualization scale, to see the lines better

                    // compute line coordinates
                    float x1 = mx - dirVecX * currentGradStrength * maxVecLen * scale;
                    float y1 = my - dirVecY * currentGradStrength * maxVecLen * scale;
                    float x2 = mx + dirVecX * currentGradStrength * maxVecLen * scale;
                    float y2 = my + dirVecY * currentGradStrength * maxVecLen * scale;

                    // draw gradient visualization
                    line(visu, cv::Point((int)(x1*zoomFac),(int)(y1*zoomFac)), cv::Point((int)(x2*zoomFac),(int)(y2*zoomFac)), cv::Scalar(0,255,0), 1);

                } // for (all bins)

            } // for (cellx)
        } // for (celly)


        // don't forget to free memory allocated by helper data structures!
        for (int y=0; y<cells_in_y_dir; y++)
        {
            for (int x=0; x<cells_in_x_dir; x++)
            {
                delete[] gradientStrengths[y][x];
            }
            delete[] gradientStrengths[y];
            delete[] cellUpdateCounter[y];
        }
        delete[] gradientStrengths;
        delete[] cellUpdateCounter;

        return visu;

    } // get_hogdescriptor_visu
}

void Classifier::getTrainFeatures(std::string file_path) {
    std::vector<cv::String> file_names;
    cv::glob(file_path,file_names);
    for(int i=0;i<file_names.size();i++){

        //******************************************************
        // convert png to jpg
        //******************************************************
        /*cv::Mat im=cv::imread(file_names[i]);
        std::ostringstream convert;
        convert << "/home/mostafa/Downloads/rtsd-r3/train/" << i << ".jpg";
        cv::imwrite(convert.str().c_str(), im);*/
        cv::Mat im=cv::imread(file_names[i]);
        img=im;
        computeHOG();
        v_descriptorsValues.push_back(descriptorsValues);
        v_locations.push_back(locations);


    }

    //std::cout<<"Number of sampels is "<<v_descriptorsValues.size()<<std::endl;
}

void Classifier::train(std::string features_path, std::string labels_path) {
    getTrainFeatures(features_path);
    cv::Ptr<TrainData> tdata=TrainData::loadFromCSV(labels_path,1,1,-1);
    cv::Mat trainLabels=tdata->getTrainResponses();
    //cv::Mat tainData=(cv::Mat)v_descriptorsValues;
    /*std::cout << trainLabels.size()<<std::endl;
    for(int i=0;i<trainLabels.size().height;i++) {
        std::cout << trainLabels<< std::endl;
    }*/
    /*cv::Mat features;
    for(int i = 0; i < v_descriptorsValues.size(); i++)
    {
        for(int j = 0; j < descriptorsValues.size(); j++)
        {
            features.at<double>(i,j) = v_descriptorsValues[i][j];
        }
    }*/
    std::cout<<"Training"<<std::endl;
    cv::Ptr<TrainData> td=TrainData::create(v_descriptorsValues,ROW_SAMPLE,trainLabels);
    svm->train(td);

   // svm->save("/home/mostafa/Desktop/Hack_SVM");
}