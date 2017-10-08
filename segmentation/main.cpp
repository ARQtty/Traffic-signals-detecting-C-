#include <string>
#include <cmath>
#include <unordered_map>
#include <vector<>
//#include <GL/glut.h>
using namespace std;

const int Q_PICTURES = 1; // Количество обрабатываемых пикч
const int DOWNSIZE_K = 4; // Коэф. уменьш. пикч
const string HUE_CUTTED_IMG_PATH = "./data/HUE_cutted/";
const string ORIGINAL_IMG_PATH = "./data/originals";

/*
double diff_grey(imgType___ img, int x1, int y1, int x2, int y2){
   // Эвристика расстояния на greyscale пикчах
   double v = abs(img[x1, y1] - img[x2, y2]);
   return v
};
*/


unordered_map groupByParent(Forest forest, int width, int height){
   vector<pair<int, int>> v;
   unordered_map <int, vector<pair<<int, int>>> groups;

   for (int y=0; y<height; y++){
      for (int x=0; x<width; x++){
         comp = forest.find(y * width + x);
         if (groups.count(comp) > 0){
            groups[comp].emplace_back = (x, y);
         }else{
            groups[comp] = v;
            groups[comp].emplace_back = (x, y);
         }
      }
   }
   return groups
};

int rectS(x1, y1, x2, y2){
   int w = abs(x2 - x1);
   int h = abs(y2 - y1);
   int S = w*h;
   return S;
};

struct Rect
{
   int x1;
   int x2;
   int y1;
   int y2;
};

vector<int, int, int, int> getRectsFromGroups(unordered_map groups, int imWidht, int imHeight){

};


int main(){
   
}