
#include "electrodeflaw_main.h"
#include "image_func.h"
#include <ctime>



int main() {

	clock_t mytime_start, mytime_finish;
	mytime_start = clock();
	
	//��ȡͼ��
	Mat src_image = imread("C:\\project\\gitrepo\\githubprj\\electrodeflaw\\test.jpg",CV_LOAD_IMAGE_ANYCOLOR);
	//Mat src_image = imread("C:\\project\\gitrepo\\githubprj\\electrodeflaw\\1.jpg", CV_LOAD_IMAGE_ANYCOLOR);

	if (!src_image.data) {
		cout << "ͼƬ��ȡʧ�ܣ�" << endl;
		system("pause");
		return -1;
	}


	
	Mat pre_image = PreprocessingForContours(src_image);
	vector <Electrode> electrodes;
	FindElectrodeContoursAndSort(pre_image, electrodes);	//��������ʶ����������庯��

	mytime_finish = clock();  	//��ʾ����ܶ��
	cout << mytime_finish - mytime_start << "/" << CLOCKS_PER_SEC << " (s) " << endl;


	ShowContoursAndSort(src_image, electrodes);  //��ʾ�����ͺ���������Ŀ
	

	////��������
	//Mat grow_image;
	//Point2i grow_point;
	//vector <Point2i> grow_target[HTNUM];
	//int thes_number = 150;
	//grow_image = RegionGrowAndStore(blur_image, thes_number, grow_target);


	//imshow("Picture_grow", grow_image);


	////��ʾ����������ɫ
	//ShowElectrodeVectorColor(src_image, grow_target);

	waitKey();
	destroyAllWindows();
	return 0;
}