#ifndef IMAGE_FUNC_H_
#define IMAGE_FUNC_H_


#include<core/core.hpp>  
#include<highgui/highgui.hpp>  
#include <imgproc/imgproc.hpp>

using namespace cv;

//定义四个边界点并初始化
class TarBorderPoint {
public:
	Point2i tar_up_;
	Point2i tar_bottom_;
	Point2i tar_left_;
	Point2i tar_right_;
	void TarInit(Mat tar_image);

};



//区域生长函数
Mat RegionGrow(Mat src, Point2i pt, int th);

//区域生长函数，并存入十个容器，并插入边界点
//Mat RegionGrowAndStore(Mat src, int th, vector<Point2i> growTargetIn[HtNum], vector <tarBorderPoint>growTargetBodrerIn[HtNum]);


//区域生长函数，并存入十个容器
Mat RegionGrowAndStore(Mat src, int th, vector<Point2i> grow_target_in[]);

//将区域增长容器显示
void ShowElectrodeVectorColor(Mat grow_image, const vector<Point2i> grow_target_in[]);

#endif
