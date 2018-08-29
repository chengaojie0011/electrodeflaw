
#include "electrodeflaw_main.h"
#include "image_func.h"
#include <ctime>

////创建轨迹条
//int threshval =160;			//轨迹条滑块对应的值，给初值160
//Mat blur_image;
//Mat track_image;
//static void trackbar(int , void*)
//{
//
//	threshold(blur_image, track_image, threshval, 255, 0);   // 阈值分割
//	//显示窗口
//	imshow("Connected Components", track_image);
//}


int main() {

	clock_t mytime_start, mytime_finish;
	mytime_start = clock();
	
	//读取图像
	Mat src_image = imread("C:\\project\\gitrepo\\githubprj\\electrodeflaw\\test.jpg",CV_LOAD_IMAGE_ANYCOLOR);
	//Mat src_image = imread("C:\\project\\gitrepo\\githubprj\\electrodeflaw\\1.jpg", CV_LOAD_IMAGE_ANYCOLOR);

	if (!src_image.data) {
		cout << "图片读取失败！" << endl;
		system("pause");
		return -1;
	}

	//namedWindow("Picture_src", 1);
	//imshow("Picture_src", src_image);

	Mat gray_image , thres_image;
	Mat blur_image;
	cvtColor(src_image, gray_image, CV_BGR2GRAY);     //将图像转换为灰度图
	medianBlur(gray_image, blur_image, 3);           //中值滤波 

	//namedWindow("Picture_blur", 1);
	//imshow("Picture_blur", blur_image);

	////创建处理窗口
	//namedWindow("Connected Components", 1);
	//createTrackbar("Threshold", "Connected Components", &threshval, 255, trackbar);


	threshold(blur_image, thres_image, 150,255,0);   // 阈值分割
	//imwrite("thres_image.png",thres_image);   //将mat写入到文件

	//namedWindow("Picture_thes", CV_WINDOW_AUTOSIZE);
	//imshow("Picture_thes", thres_image);

	vector <Electrode> electrodes;
	FindElectrodeContours(thres_image, electrodes);

	SortElectrode(electrodes);

	//显示软件跑多久
	mytime_finish = clock();
	cout << mytime_finish - mytime_start << "/" << CLOCKS_PER_SEC << " (s) " << endl;

	//显示焊条排序序号
	ShowSortNumber(src_image, electrodes);

	Mat find_image = Mat::zeros(src_image.size(), CV_8UC3);
	RNG rng(0);
	for (int i = 0; i < electrodes.size(); i++){
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		for (auto it = electrodes[i].contour_points_.cbegin();
			it != electrodes[i].contour_points_.cend(); ++it) {
			/*src_image.at<Vec3b>((*it).y, (*it).x)[0] = color[0];
			src_image.at<Vec3b>((*it).y, (*it).x)[1] = color[1];
			src_image.at<Vec3b>((*it).y, (*it).x)[2] = color[2];*/
			src_image.at<Vec3b>((*it).y, (*it).x)[0] = 0;
			src_image.at<Vec3b>((*it).y, (*it).x)[1] =0;
			src_image.at<Vec3b>((*it).y, (*it).x)[2] = 255;
		}
		//cout << electrodes[i].contour_points_.size() << endl;
	}

	namedWindow("Picture_contour", CV_WINDOW_AUTOSIZE);
	imshow("Picture_contour", src_image);
	//imwrite("thres_image.png", src_image);   //将mat写入到文件

	////区域增长
	//Mat grow_image;
	//Point2i grow_point;
	//vector <Point2i> grow_target[HTNUM];
	//int thes_number = 150;
	//grow_image = RegionGrowAndStore(blur_image, thes_number, grow_target);


	//imshow("Picture_grow", grow_image);


	////显示焊条区域颜色
	//ShowElectrodeVectorColor(src_image, grow_target);

	waitKey();
	destroyAllWindows();
	return 0;
}