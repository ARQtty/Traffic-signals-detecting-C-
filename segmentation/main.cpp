#include "graph.cpp"
#include <string>
#include <cmath>
#include <unordered_map>
#include <iostream>
#include <vector>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/ml.hpp"
#include "opencv2/objdetect.hpp"

using namespace std;

const int Q_PICTURES = 1; // Количество обрабатываемых пикч
const int DOWNSIZE_K = 4; // Коэф. уменьш. пикч
const string HUE_CUTTED_IMG_PATH = "./data/HUE_cutted/";
const string ORIGINAL_IMG_PATH = "./data/originals";


double diff_grey(Mat img, int x1, int y1, int x2, int y2) {
    // Эвристика расстояния на greyscale пикчах
    double v = abs(img[x1, y1] - img[x2, y2]);
    return v;
};

unordered_map groupByParent(Forest forest, int width, int height) {
    vector <pair<int, int>> v;
    unordered_map < int, vector < pair < int, int > > > groups;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            comp = forest.find(y * width + x);
            if (groups.count(comp) > 0) {
                groups[comp].emplace_back = (x, y);
            } else {
                groups[comp] = v;
                groups[comp].emplace_back = (x, y);
            }
        }
    }
    return groups;
};

int rectS(x1, y1, x2, y2) {
    int w = abs(x2 - x1);
    int h = abs(y2 - y1);
    int S = w * h;
    return S;
};

struct Rect {
    Rect(int x1, int x2, int y1, int y2)
        : x1(x1), x2(x2), y1(y1), y2(y2) {}

    int k = 4;
    int x1;
    int x2;
    int y1;
    int y2;
};

vector <Rect> getRectsFromGroups(unordered_map groups, int imWidth, int imHeight) {
    vector <Rect> rects;
    int s = imWidth * imHeight;
    for (int key : groups.keys()) {
        x_min = 99999;
        x_max = -1;
        y_min = 99999;
        y_max = -1;
        for (std::pair<int, int> xy :groups[key]) {
            if (x_max < xy.first) {
                x_max = xy.first;
            } else if (x_min > xy.first) {
                x_min = xy.first;
            } else if (y_max < xy.second) {
                y_max = xy.second;
            } else if (y_min > xy.second) {
                y_min = xy.second;
            }
        }
        int S = rect_S(xmin, ymin, xmax, ymax);
        maxS_k = 0.4;
        if (S < maxS_k * imS) {
            rects.append(Rect(xmin, ymin, xmax, ymax));
        }
    }
    return rects;
};


int main() {
    cv::Mat img = cv::imread("test.jpg");
    cv::Mat hsv;
    cv::cvtColor(img, hsv, cv::COLOR_BGR2HSV);
    cv::Mat mask;
    cv::inRange(hsv, {90, 120, 0}, {180, 200, 255}, mask);
    cv::inRange(hsv, {0, 120, 80}, {40, 255, 255}, mask);
    cv::Mat res;
    bitwise_and(res, res, res, mask);

    cv::Mat res_gray;
    cv::cvtColor(res, res_gray, cv::COLOR_HSV2GRAY);

    cv::Mat temp_img;
    std::pair<int> size1{res_gray.size()[0], res_gray.size()[1]};
    std::pair<int> size2{res_gray.size()[0] / 4, res_gray.size()[1] / 4};
    cv::resize(res_gray, temp_img, size2);

    const int neighbor = 4;
    const float sigma = .2;
    const float k = .3;
    const int min_size = 100;

    cv::Mat grid = gaussian_grid(sigma);

    cv::Mat smooth = filter_image(temp_img, grid);

    std::vector <Edge> img_graph = build_graph(smooth, size2.second, size2.first);

    Forest forest = segment_graph(img_graph, size2.first * size2.secont, k, min_size);

    unordered_map groups = groupByParent(forest, size2.second, size2.first);

    std::vector <Rect> rects = getRectsFromGroups(groups, size2.second, size2.first);

    for (int i = 0; i < rects.size(); ++i) {
        Rect& r = rects[i];
        vector<int> box = {rect.y1, rect.x1, rect.y2, rect.x2};
        vector<int> box_orig;
        for (int b : box) {
            box_orig.push_back(b * r.k);
        }
        CVMat* imgsrc = &img;
        cv::Mat image(imgsrc);
        cv::Rect my_roi(rect.y1, rect.x1, rect.y2, rect.x2);
        char* c = nullptr;
        sprintf(c, "../train/fragments/%d.jpg", i);
        cv::imwrite(c, image(my_roi));
    }

    return 0;
}