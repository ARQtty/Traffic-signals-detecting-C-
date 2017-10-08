#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

vector < vector <double> > gaussian_grid(double sig){
   double sigma = max(sig, 0.01);
   int length = ceil(sigma * 4) + 1;
   int m = length / 2;
   vector < vector <double> > x2_plus_y2 = {{2.0, 1.0, 2.0},
                                            {0.0, 0.0, 0.0},
                                            {2.0, 1.0, 2.0}};
   double x2y2_sum = 0;
   // Получим необх. матрицу
   for (int i=0; i<3; i++){
      for (int j=0; j<3; j++){
         x2_plus_y2[i][j] = exp(m*m) * exp(-0.5 * x2_plus_y2[i][j]);
         x2y2_sum = x2y2_sum + x2_plus_y2[i][j];
      }
   }
   // Нормализовать её
   // Получим необх. матрицу
   for (int i=0; i<3; i++){
      for (int j=0; j<3; j++){
        x2_plus_y2[i][j] = x2_plus_y2[i][j] / x2y2_sum;
      }
    }
   return x2_plus_y2;
};

img_type____ filter_image(img_type____ image, vector < vector <double> > mask){
  img_type____ filtred;
  for (int i=0; i<image.sizeX___; ++i){
    int k = i + 2;
    for (var j=0; j<image.sizeY___; ++j){
      int l = j + 2;
      float sum = 0.0f;
      for (int m=0; m<3; ++m){
          for (int n=0; n<3; ++n){
              float pix = image.at<Vec2b>(k-m, l-n);
              float weight = kernel[m][n];
              sum += pix * weight;
          }
      }
      // cv namespace
      uchar value = uchar(min(max(sum, 0.0f), 255.0f));
      filtred.at<Vec3b>(i, j) = value;
    }
  }
  return filtred
}