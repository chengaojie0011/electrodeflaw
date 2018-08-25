#ifndef IMAGE_FUNC_H_
#define IMAGE_FUNC_H_

#include <iostream>
#include <vector>

#include<core/core.hpp>  
#include<highgui/highgui.hpp>  
#include <imgproc/imgproc.hpp>

using namespace cv;


//���庸����
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
	vector <Point2i> contour_points_;   //���������ĵ㼯
	int sort_number_;            //�����������ҵ��������

 private:
	 float e_width;    //�����Ŀ�
	 float e_height;	   //�����ĳ�
	 Point2i contour_center_;  //���������ĵ�

};


//������������
Mat RegionGrow(Mat src, Point2i pt, int th);

//��������������������ʮ��������������߽��
//Mat RegionGrowAndStore(Mat src, int th, vector<Point2i> growTargetIn[HtNum], vector <tarBorderPoint>growTargetBodrerIn[HtNum]);


//��������������������ʮ������
Mat RegionGrowAndStore(Mat src, int th, vector<Point2i> grow_target_in[]);

//����������������ʾ
void ShowElectrodeVectorColor(Mat grow_image, const vector<Point2i> grow_target_in[]);

//��ȡͼ��������������ɸѡ
void FindElectrodeContours(Mat src, vector <Electrode> &electrodes_output);

//�Ժ�����������
void SortElectrode(vector <Electrode> &electrodes_sort);

//��ʾ�����������
void ShowSortNumber(Mat src, vector <Electrode> &electrodes_number);

#endif   //IMAGE_FUNC_H_
