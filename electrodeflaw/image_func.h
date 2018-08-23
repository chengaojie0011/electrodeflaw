#ifndef IMAGE_FUNC_H_
#define IMAGE_FUNC_H_


#include<core/core.hpp>  
#include<highgui/highgui.hpp>  
#include <imgproc/imgproc.hpp>

using namespace cv;

//�����ĸ��߽�㲢��ʼ��
class tarBorderPoint {
public:
	Point2i tarUp;
	Point2i tarBottom;
	Point2i tarLeft;
	Point2i tarRight;
	void tarInit(Mat tarImage);

};



//������������
Mat RegionGrow(Mat src, Point2i pt, int th);

//��������������������ʮ��������������߽��
//Mat RegionGrowAndStore(Mat src, int th, vector<Point2i> growTargetIn[HtNum], vector <tarBorderPoint>growTargetBodrerIn[HtNum]);


//��������������������ʮ������
Mat RegionGrowAndStore(Mat src, int th, vector<Point2i> growTargetIn[]);

#endif
