#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <stdlib.h>
//#define DEBUG 0

using namespace std;
using namespace cv;

void filter_contours(vector<vector<Point>> &contours, const int size);
void reference_regions(const char * file_name);
vector<vector<Point>> contours;
float roi_x = 0, roi_y = 1/4;
Mat reference;

int main() {
	reference_regions("empty_table.png");
	Mat src = imread("full_table.png");
	cout<<"Count"<<contours.size()<<endl;
	Rect rec(0, src.rows / 2, src.cols, src.rows / 2);
	Mat roi = src(rec);
	drawContours(roi, contours, -1, Scalar(255,11,200), 4);
	imshow("source", roi);
	imshow("reference", reference*255);
	waitKey(-1);
	return 0;
}

void filter_contours(vector<vector<Point>> &contours, const int size) {
	for (auto i = 0; i < contours.size(); i++) {
		cout << "Contour id : " << i << " area "
				<< contourArea(contours[i], false) << endl;
		if (contourArea(contours[i]) < 2000) {
			contours.erase(contours.begin() + i);

		}
	}
}
void reference_regions(const char * file_name) {
	Scalar th(190, 182, 175);
	Scalar delta(20, 20, 20);
	Mat src = imread("empty_table.png");
	Mat thresholded;
	Rect rec(0, src.rows / 2, src.cols, src.rows / 2);
	Mat roi = src(rec);
	inRange(roi, th - delta, Scalar(255, 255, 255), thresholded);
	morphologyEx(thresholded, thresholded, MORPH_CLOSE,
			getStructuringElement(MORPH_RECT, Size(5, 5)));
	morphologyEx(thresholded, thresholded, MORPH_OPEN,
			getStructuringElement(MORPH_RECT, Size(5, 5)));
	vector<Vec4i> hierarchy;
	Mat contoursMap(thresholded.rows, thresholded.cols, src.type());
	findContours(thresholded, contours, hierarchy, CV_RETR_EXTERNAL,
			CV_CHAIN_APPROX_NONE);
	filter_contours(contours, 2000);

	drawContours(contoursMap, contours, -1, Scalar(255, 39, 100), 2);
	reference = thresholded/255;
#ifdef DEBUG
	namedWindow("contoursMap");
	imshow("contoursMap", contoursMap);
	imshow("result", thresholded);
	imshow("Image", src);
	imshow("ROI", roi);
	waitKey(0);
#endif
}
