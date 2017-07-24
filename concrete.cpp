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


	Sobel(img, sobelX, CV_8U, 1, 0, 3);   // ����
	Sobel(img, sobelY, CV_8U, 0, 1, 3);   // ����

	Mat sobelX2 = sobelX.clone(), sobelX3 = sobelX.clone();
	Mat sobelY2 = sobelY.clone(), sobelY3 = sobelY.clone();

	NF2(sobelX2, 6, 12, 17);    // ����
	NF2(sobelY2, 12, 6, 17);    // ����

	imshow("SobelX2", sobelX2);
	imshow("SobleY2", sobelY2);

	NF2(sobelX3, 8, 8, 12);
	NF2(sobelY3, 8, 8, 12);

	imshow("sobelX3", sobelX3);
	imshow("sobelY3", sobelY3);

	// 110 ������ ���� ������ �ȼ��� ����(0)���� ��ü//
	
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

			Mat ROI(image, Rect(y, x, matY, matX));       // ������ 0�� �ƴϴ�. 17�� ������ ��� ������� �����Ͽ� �����Ѵ�.

			nos = 0;
			
			for(m =0; m < matX; m++) {
				for(n = 0; n < matY; n++) {
					
					if((ROI.at<uchar>(m, n)) > 0)        // �ش� �ȼ��� ���� ������ ��� noise++
						nos++;
				}
			}
			
			if(nos < noise) {                             // noise�� 3���� ���� ��� ��� �ȼ��� ��������
				
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