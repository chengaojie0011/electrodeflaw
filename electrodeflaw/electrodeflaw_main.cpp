
#include "electrodeflaw_main.h"
#include "image_func.h"
#include <ctime>



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


	
	Mat pre_image = PreprocessingForContours(src_image);
	vector <Electrode> electrodes;
	FindElectrodeContoursAndSort(pre_image, electrodes);	//焊条轮廓识别并排序的整体函数

	mytime_finish = clock();  	//显示软件跑多久
	cout << mytime_finish - mytime_start << "/" << CLOCKS_PER_SEC << " (s) " << endl;


	ShowContoursAndSort(src_image, electrodes);  //显示轮廓和焊条排序数目
	

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