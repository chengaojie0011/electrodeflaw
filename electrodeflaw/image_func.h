#ifndef IMAGE_FUNC_H_
#define IMAGE_FUNC_H_

#include <iostream>
#include <vector>

#include<core/core.hpp>  
#include<highgui/highgui.hpp>  
#include <imgproc/imgproc.hpp>

using namespace cv;


//定义焊条类
class Electrode {
 public:
	//Electrode(Point2i center, vector <Point2i> points, int sort): 
	//		   contour_center_(center), contour_points_(points), sort_number_(sort) {}
	 void GetContourWidthHeight(float width,float height);
	void GetContourCenter(int x, int y); 
	void GetContourPoints(vector <Point2i> points);
	//void Display_contour_center();
	void DisplayContourCenter();
	Point2i OutputContourCenter();
	vector <Point2i> contour_points_;   //焊条轮廓的点集
	int sort_number_;            //焊条从左至右的排序序号

 private:
	 float e_width;    //焊条的宽
	 float e_height;	   //焊条的长
	 Point2i contour_center_;  //焊条的中心点

};


//区域生长函数
Mat RegionGrow(Mat src, Point2i pt, int th);

//区域生长函数，并存入十个容器，并插入边界点
//Mat RegionGrowAndStore(Mat src, int th, vector<Point2i> growTargetIn[HtNum], vector <tarBorderPoint>growTargetBodrerIn[HtNum]);


//区域生长函数，并存入十个容器
Mat RegionGrowAndStore(Mat src, int th, vector<Point2i> grow_target_in[]);

//将区域增长容器显示
void ShowElectrodeVectorColor(Mat grow_image, const vector<Point2i> grow_target_in[]);

//获取图像轮廓，并进行筛选
void FindElectrodeContours(Mat src, vector <Electrode> &electrodes_output);

//对焊条进行排序
void SortElectrode(vector <Electrode> &electrodes_sort);

//显示焊条排序序号
void ShowSortNumber(Mat src, vector <Electrode> &electrodes_number);

#endif   //IMAGE_FUNC_H_
