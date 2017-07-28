#include <iostream>
 
#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

Mat NF1(Mat image);
Mat NF2(Mat image, int matX, int matY, int noise);
Mat NFY2(Mat image, int matX, int matY, int noise);
Mat NF4(Mat image, int edge);

int main() {

	Mat img, sobelX, sobelY;
	Mat dstX, dstX2, dstY, dstY2;
	
	img = imread("cement_cracked (94).jpg", CV_8U);   

	if(img.empty()) {
		cout<<"Error reading image"<<endl;
		return 0;
	}

	Sobel(img, sobelX, CV_8U, 1, 0, 3);   // 수평
	Sobel(img, sobelY, CV_8U, 0, 1, 3);   // 수직

	imshow("sobelX", sobelX);
	imshow("sobelY", sobelY);

	Mat sobelX2 = sobelX.clone(), sobelX3 = sobelX.clone(), sobelX4 = sobelX.clone();
	Mat sobelY2 = sobelY.clone(), sobelY3 = sobelY.clone(), sobelY4 = sobelY.clone();

	
	NF1(sobelX);
	NF1(sobelY);

	NF2(sobelX2, 10, 2, 13);    // 수평
	NF2(sobelY2, 2, 10, 13);    // 수직

	imshow("NF2X", sobelX2);
	imshow("NF2Y", sobelY2);


	// Noise filtering 3
	NF2(sobelX3, 4, 4, 12);
	NF2(sobelY3, 4, 4, 12);

	//imshow("sobelX3", sobelX3);
	//imshow("sobelY3", sobelY3);

	NF4(sobelX4, 0);
	NF4(sobelY4, 0);
	
	
	// 수평 성분끼리 합치기
	addWeighted(sobelX, 1, sobelX2, 1, 0.0, dstX);  // sobelX + sobelX2
	addWeighted(dstX, 1, sobelX3, 1, 0.0, dstX2);   // + sobelX3
	addWeighted(dstX2, 1, sobelX4, 1, 0.0, dstX);   // + sobelX4
	

	// 수직 성분끼리 합치기
	addWeighted(sobelY , 1, sobelY2, 1, 0.0, dstY);
	addWeighted(dstY, 1, sobelY3, 1, 0.0, dstY2);
	addWeighted(dstY2, 1, sobelY4, 1, 0.0, dstY);   // + sobelY4

	
	//imshow("dstY2", dstY);
	//imshow("dstX2", dstX);

	
	Mat result(Size(dstX.rows, dstX.cols), CV_8U);
	
	/*
	for(int x = 0; x < dstX.rows; x++) {
		for(int y = 0; y < dstX.cols; y++) {
			
			if(dstX.at<uchar>(x, y) > 127 && dstY.at<uchar>(x, y) < 127)
				result.at<uchar>(x, y) = 0;
			else if(dstX.at<uchar>(x, y) < 127 && dstY.at<uchar>(x, y) > 127)
				result.at<uchar>(x, y) = 0;
			else
				result.at<uchar>(x, y) = 255;
		}
	}
	*/
	
	for(int x = 0; x < dstX.rows; x++) {
		for(int y = 0; y < dstX.cols; y++) {
			
			if(dstX.at<uchar>(x, y) > 127 && dstY.at<uchar>(x, y) > 127)
				result.at<uchar>(x, y) = 255;
				
			else if(dstX.at<uchar>(x, y) < 127 && dstY.at<uchar>(x, y) < 127)
				result.at<uchar>(x, y) = 0;
				
			else
				result.at<uchar>(x, y) = 50;
		}
	}
	
	bitwise_not(result, result);   // 반전

	imshow("result", result);
	
	namedWindow("Original Image");
	imshow("Original Image", img);
	
	waitKey(0);

	return 0;

}

Mat NF1(Mat image) {

	int i, j;

	for(i = 0; i < image.cols; i++) {                      
		for(j = 0; j < image.rows; j++) {

			if(image.at<uchar>(j, i) < 180) 
				image.at<uchar>(j, i) = 0;
			
		}
	}

	return image;
}


/*
Mat NF2(Mat image, int matX, int matY, int noise) {

	int nos, m, n;

	for(int x = 0; x < image.rows - matX; x++) {
		for(int y = 0; y < image.cols - matY; y++) {

			Mat ROI(image, Rect(y, x, matY, matX));       // 잡음은 0이 아니다. 17개 이하일 경우 노이즈로 간주하여 제거한다.

			nos = 0;
			
			for(m =0; m < matX; m++) {
				for(n = 0; n < matY; n++) {
					
					if((ROI.at<uchar>(m, n)) > 130)        // 해당 픽셀에 값이 잡음일 경우 noise++
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
*/
Mat NF4(Mat image, int edge) {

	int x, y;

	for(x = 0; x < image.rows; x++) {
		for(y = 0; y < image.cols; y++) {
				  
			
			if(y + 2 < image.cols) {
				if(image.at<uchar>(x, y) > edge && 
				   image.at<uchar>(x, y + 1) > edge &&
				   image.at<uchar>(x, y + 2) > edge) {
				
					   y += 2;
				}
				else {
					image.at<uchar>(x, y) = 0;
					image.at<uchar>(x, y + 1) = 0;
					image.at<uchar>(x, y + 2) = 0;

					y += 2;
				}
			}
			
		}
	}

	return image;
}


Mat NF2(Mat image, int matX, int matY, int noise) {

	int nos, m, n;

	for(int x = 0; x <= image.rows - matX; x++) {
		for(int y = 0; y <= image.cols - matY; y++) {

			Mat ROI(image, Rect(y, x, matY, matX));       // 잡음은 0이 아니다. 17개 이하일 경우 노이즈로 간주하여 제거한다.

			nos = 0;
			
			for(m =0; m < matX; m++) {
				for(n = 0; n < matY; n++) {
					
					if(ROI.at<uchar>(m, n) > 0)        // 해당 픽셀에 값이 잡음일 경우 noise++
						nos++;
				}
			}

			//printf("nos = %d\n", nos);

			if(nos < noise) {                             // noise가 3보다 작을 경우 모든 픽셀을 검정으로
				
				for(m =0; m < matX; m++) {
					for(n = 0; n < matY; n++) {
						ROI.at<uchar>(m, n) = 0;
					}
				}
			}

			y += (matY - 1); 

		}
		x += (matX - 1);
	}

	return image;
}




