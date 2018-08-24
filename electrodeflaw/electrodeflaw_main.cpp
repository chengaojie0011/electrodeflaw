
#include "electrodeflaw_main.h"
#include "image_func.h"
#include <ctime>

////�����켣��
//int threshval =160;			//�켣�������Ӧ��ֵ������ֵ160
//Mat blur_image;
//Mat track_image;
//static void trackbar(int , void*)
//{
//
//	threshold(blur_image, track_image, threshval, 255, 0);   // ��ֵ�ָ�
//	//��ʾ����
//	imshow("Connected Components", track_image);
//}


int main() {

	clock_t mytime_start, mytime_finish;
	mytime_start = clock();
	
	//��ȡͼ��
	//Mat src_image = imread("C:\\project\\gitrepo\\githubprj\\electrodeflaw\\test.jpg",CV_LOAD_IMAGE_ANYCOLOR);
	Mat src_image = imread("C:\\project\\gitrepo\\githubprj\\electrodeflaw\\1.jpg", CV_LOAD_IMAGE_ANYCOLOR);

	if (!src_image.data) {
		cout << "ͼƬ��ȡʧ�ܣ�" << endl;
		system("pause");
		return -1;
	}

	//namedWindow("Picture_src", 1);
	//imshow("Picture_src", src_image);

	Mat gray_image , thres_image;
	Mat blur_image;
	cvtColor(src_image, gray_image, CV_BGR2GRAY);     //��ͼ��ת��Ϊ�Ҷ�ͼ
	medianBlur(gray_image, blur_image, 3);           //��ֵ�˲� 

	//namedWindow("Picture_blur", 1);
	//imshow("Picture_blur", blur_image);

	////����������
	//namedWindow("Connected Components", 1);
	//createTrackbar("Threshold", "Connected Components", &threshval, 255, trackbar);


	threshold(blur_image, thres_image, 150,255,0);   // ��ֵ�ָ�
	//imwrite("thres_image.png",thres_image);   //��matд�뵽�ļ�

	//namedWindow("Picture_thes", CV_WINDOW_AUTOSIZE);
	//imshow("Picture_thes", thres_image);

	vector <Electrode> electrodes;
	FindElectrodeContours(thres_image, electrodes);

	//Sort_Electrode(electrodes);



	//////��������
	//Mat grow_image;
	//Point2i grow_point;
	//vector <Point2i> grow_target[HTNUM];
	//int thes_number = 150;
	////grow_image = RegionGrowAndStore(blur_image, thes_number, grow_target);


	//��ʾ����ܶ��
	mytime_finish = clock();
	cout << mytime_finish - mytime_start << "/" << CLOCKS_PER_SEC << " (s) " << endl;

	////���û����ı�����ز���
	//int font_face = cv::FONT_HERSHEY_COMPLEX;
	//double font_scale = 0.5;
	//int thickness = 1;
	//int baseline;


	//Point2i center_sort;
	//int sort_num;
	//for (int i = 0; i < electrodes.size(); i++) {
	//	center_sort = electrodes[i].OutputContourCenter();
	//	sort_num = electrodes[i].sort_number_;
	//	string text = to_string(sort_num);
	//	//Size text_size = getTextSize(text, font_face, font_scale, thickness, &baseline);
	//	putText(src_image, text, center_sort, font_face, font_scale, cv::Scalar(0, 0, 255), thickness, 8, 0);
	//}
	//namedWindow("Picture_thes", CV_WINDOW_AUTOSIZE);
	//imshow("Picture_thes", src_image);


	////imshow("Picture_grow", grow_image);


	////��ʾ����������ɫ
	////ShowElectrodeVectorColor(src_image, grow_target);
	//

	waitKey();
	destroyAllWindows();
	return 0;
}