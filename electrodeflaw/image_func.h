#ifndef IMAGE_FUNC_H_
#define IMAGE_FUNC_H_


#include<core/core.hpp>  
#include<highgui/highgui.hpp>  
#include <imgproc/imgproc.hpp>

using namespace cv;

//�����ĸ��߽�㲢��ʼ��
class TarBorderPoint {
public:
	Point2i tar_up_;
	Point2i tar_bottom_;
	Point2i tar_left_;
	Point2i tar_right_;
	void TarInit(Mat tar_image);

};



//������������
Mat RegionGrow(Mat src, Point2i pt, int th);

//��������������������ʮ��������������߽��
//Mat RegionGrowAndStore(Mat src, int th, vector<Point2i> growTargetIn[HtNum], vector <tarBorderPoint>growTargetBodrerIn[HtNum]);


//��������������������ʮ������
Mat RegionGrowAndStore(Mat src, int th, vector<Point2i> grow_target_in[]);

//����������������ʾ
void ShowElectrodeVectorColor(Mat grow_image, const vector<Point2i> grow_target_in[]);

#endif
