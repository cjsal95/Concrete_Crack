#include <iostream>
 
#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;



Mat NF1(Mat image);
Mat NF2(Mat image, int matX, int matY, int noise);

int main() {

	Mat img, grey_img, sobelX, sobelY;
	

	img = imread("cement_cracked (94).jpg", CV_8U);   

	
	if(img.empty()) {
		cout<<"Error reading image"<<endl;
		return 0;
	}


	Sobel(img, sobelX, CV_8U, 1, 0, 3);   // 수평
	Sobel(img, sobelY, CV_8U, 0, 1, 3);   // 수직

	Mat sobelX2 = sobelX.clone(), sobelX3 = sobelX.clone();
	Mat sobelY2 = sobelY.clone(), sobelY3 = sobelY.clone();

	NF2(sobelX2, 6, 12, 17);    // 수평
	NF2(sobelY2, 12, 6, 17);    // 수직

	imshow("SobelX2", sobelX2);
	imshow("SobleY2", sobelY2);

	NF2(sobelX3, 8, 8, 12);
	NF2(sobelY3, 8, 8, 12);

	imshow("sobelX3", sobelX3);
	imshow("sobelY3", sobelY3);

	// 110 이하의 값을 가지는 픽셀을 검정(0)으로 대체//
	
	NF1(sobelX);
	NF1(sobelY);

	imshow("SobelY", sobelY);
	imshow("SobelX", sobelX);

	namedWindow("Original Image");
	imshow("Original Image", img);
	
	waitKey(0);

	return 0;

}

Mat NF1(Mat image) {

	int i, j;

	for(i = 0; i < image.cols; i++) {                      
		for(j = 0; j < image.rows; j++) {

			if(image.at<uchar>(j, i) < 111) 
				image.at<uchar>(j, i) = 0;
			
		}
	}

	return image;
}

Mat NF2(Mat image, int matX, int matY, int noise) {

	int nos, m, n;

	for(int x = 0; x < image.rows - matX; x++) {
		for(int y = 0; y < image.cols - matY; y++) {

			Mat ROI(image, Rect(y, x, matY, matX));       // 잡음은 0이 아니다. 17개 이하일 경우 노이즈로 간주하여 제거한다.

			nos = 0;
			
			for(m =0; m < matX; m++) {
				for(n = 0; n < matY; n++) {
					
					if((ROI.at<uchar>(m, n)) > 0)        // 해당 픽셀에 값이 잡음일 경우 noise++
						nos++;
				}
			}
			
			if(nos < noise) {                             // noise가 3보다 작을 경우 모든 픽셀을 검정으로
				
				for(m =0; m < matX; m++) {
					for(n = 0; n < matY; n++) {
						ROI.at<uchar>(m, n) = 0;
					}
				}
			}		
		}
	}

	return image;
}