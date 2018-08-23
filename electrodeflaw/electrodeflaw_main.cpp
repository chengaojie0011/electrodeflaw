
#include "electrodeflaw_main.h"
#include "image_func.h"
#include <ctime>



int main()
{
	clock_t mytime_start, mytime_finish;
	mytime_start = clock();
	
	//读取图像
	Mat src_image = imread("C:\\project\\gitrepo\\githubprj\\electrodeflaw\\test.jpg",CV_LOAD_IMAGE_ANYCOLOR);

	if (!src_image.data)
	{
		cout << "图片读取失败！" << endl;
		system("pause");
		return -1;
	}
	namedWindow("Picture_src", 1);
	imshow("Picture_src", src_image);

	Mat gray_image , thres_image , blur_image;
	cvtColor(src_image, gray_image, CV_BGR2GRAY);     //将图像转换为灰度图
	medianBlur(gray_image, blur_image, 3);           //中值滤波 

	//namedWindow("Picture_blur", 1);
	//imshow("Picture_blur", blur_image);


	//threshold(blur_image, thres_image, 150,255,THRESH_BINARY);   // 阈值分割
	//namedWindow("Picture_thes", CV_WINDOW_AUTOSIZE);
	//imshow("Picture_thes", thres_image);


	Mat grow_image;
	//Point2i grow_point = Point(196, 1);
	Point2i grow_point;
	//int nSrcValue = thres_image.at<uchar>(1, 196);			//记录生长点的灰度值
	//grow_image=RegionGrow(thresImage, grow_point, 150);

	vector <Point2i> grow_target[HTNUM];
	//vector <tarBorderPoint> growTargetBodrer[HtNum];
	int thes_number = 150;
	//grow_image = RegionGrowAndStore(blur_image, thes_number, grow_target, growTargetBodrer);
	grow_image = RegionGrowAndStore(blur_image, thes_number, grow_target);
	//Mat RegionGrowAndStore(Mat src, int th, vector<Point2i> grow_target_in[10])

	//显示软件跑多久
	mytime_finish = clock();
	cout << mytime_finish - mytime_start << "/" << CLOCKS_PER_SEC << " (s) " << endl;

	imshow("Picture_grow", grow_image);


	//显示焊条区域颜色
	ShowElectrodeVectorColor(src_image, grow_target);
	

	waitKey();
	destroyAllWindows();
	return 0;
}