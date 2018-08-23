#ifndef IMAGE_FUNC_H_
#define IMAGE_FUNC_H_


#include<core/core.hpp>  
#include<highgui/highgui.hpp>  
#include <imgproc/imgproc.hpp>

using namespace cv;

//定义四个边界点并初始化
class tarBorderPoint {
public:
	Point2i tarUp;
	Point2i tarBottom;
	Point2i tarLeft;
	Point2i tarRight;
	void tarInit(Mat tarImage);

};



//区域生长函数
Mat RegionGrow(Mat src, Point2i pt, int th);

//区域生长函数，并存入十个容器，并插入边界点
//Mat RegionGrowAndStore(Mat src, int th, vector<Point2i> growTargetIn[HtNum], vector <tarBorderPoint>growTargetBodrerIn[HtNum]);


//区域生长函数，并存入十个容器
Mat RegionGrowAndStore(Mat src, int th, vector<Point2i> growTargetIn[]);

#endif
