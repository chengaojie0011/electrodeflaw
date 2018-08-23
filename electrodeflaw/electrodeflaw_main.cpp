
#include "electrodeflaw_main.h"
#include "image_func.h"
#include <ctime>



int main()
{
	clock_t start, finish;
	start = clock();
	//��ȡͼ��
	Mat srcImage = imread("C:\\project\\ElectrodeDetect\\test.jpg",CV_LOAD_IMAGE_ANYCOLOR);
	//Mat srcImage = imread("C:\\project\\ElectrodeDetect\\3.jpg", CV_LOAD_IMAGE_ANYCOLOR);

	if (!srcImage.data)
	{
		cout << "ͼƬ��ȡʧ�ܣ�" << endl;
		system("pause");
		return -1;
	}
	namedWindow("Picture_src", 1);
	imshow("Picture_src", srcImage);

	Mat grayImage , thresImage , blurImage;
	cvtColor(srcImage, grayImage, CV_BGR2GRAY);     //��ͼ��ת��Ϊ�Ҷ�ͼ
	medianBlur(grayImage, blurImage, 3);           //��ֵ�˲� 

	//namedWindow("Picture_blur", 1);
	//imshow("Picture_blur", blurImage);


	//threshold(blurImage, thresImage, 150,255,THRESH_BINARY);   // ��ֵ�ָ�
	//namedWindow("Picture_thes", CV_WINDOW_AUTOSIZE);
	//imshow("Picture_thes", thresImage);


	Mat grow_image;
	//Point2i grow_point = Point(196, 1);
	Point2i grow_point;
	//int nSrcValue = thresImage.at<uchar>(1, 196);			//��¼������ĻҶ�ֵ
	//grow_image=RegionGrow(thresImage, grow_point, 150);

	vector <Point2i> growTarget[HtNum];
	//vector <tarBorderPoint> growTargetBodrer[HtNum];
	int thesNumber = 150;
	//grow_image = RegionGrowAndStore(blurImage, thesNumber, growTarget, growTargetBodrer);
	grow_image = RegionGrowAndStore(blurImage, thesNumber, growTarget);
	//Mat RegionGrowAndStore(Mat src, int th, vector<Point2i> grow_target_in[10])

	//��ʾ����ܶ��
	finish = clock();
	cout << finish - start << "/" << CLOCKS_PER_SEC << " (s) " << endl;

	imshow("Picture_grow", grow_image);


	//��ʾ����������ɫ
	Mat test1 = Mat::zeros(grow_image.size(), CV_8UC3);	//����һ���հ��������Ϊ��ɫ
	Point2i grow_test_point;
	for (int tarNum = 0; tarNum < HtNum; ++tarNum)
	{
			switch (tarNum)
			{
				case 0:
				{
					for (size_t i = 0; i < growTarget[tarNum].size(); i++)
					{
						grow_test_point = growTarget[tarNum][i];
						test1.at<Vec3b>(grow_test_point.y, grow_test_point.x)[0] = 255;
						test1.at<Vec3b>(grow_test_point.y, grow_test_point.x)[1] = 0;
						test1.at<Vec3b>(grow_test_point.y, grow_test_point.x)[2] = 0;
					}
					break;
				}
				case 1:
				{
					for (size_t i = 0; i < growTarget[tarNum].size(); i++)
					{
						grow_test_point = growTarget[tarNum][i];
						test1.at<Vec3b>(grow_test_point.y, grow_test_point.x)[0] = 255;
						test1.at<Vec3b>(grow_test_point.y, grow_test_point.x)[1] = 165;
						test1.at<Vec3b>(grow_test_point.y, grow_test_point.x)[2] = 0;
					}
					break;
				}
				case 2:
				{
					for (size_t i = 0; i < growTarget[tarNum].size(); i++)
					{
						grow_test_point = growTarget[tarNum][i];
						test1.at<Vec3b>(grow_test_point.y, grow_test_point.x)[0] = 255;
						test1.at<Vec3b>(grow_test_point.y, grow_test_point.x)[1] = 255;
						test1.at<Vec3b>(grow_test_point.y, grow_test_point.x)[2] = 0;
					}
					break;
				}
				case 3:
				{
					for (size_t i = 0; i < growTarget[tarNum].size(); i++)
					{
						grow_test_point = growTarget[tarNum][i];
						test1.at<Vec3b>(grow_test_point.y, grow_test_point.x)[0] = 0;
						test1.at<Vec3b>(grow_test_point.y, grow_test_point.x)[1] = 255;
						test1.at<Vec3b>(grow_test_point.y, grow_test_point.x)[2] = 0;
					}
					break;
				}
				case 4:
				{
					for (size_t i = 0; i < growTarget[tarNum].size(); i++)
					{
						grow_test_point = growTarget[tarNum][i];
						test1.at<Vec3b>(grow_test_point.y, grow_test_point.x)[0] = 0;
						test1.at<Vec3b>(grow_test_point.y, grow_test_point.x)[1] = 127;
						test1.at<Vec3b>(grow_test_point.y, grow_test_point.x)[2] = 255;
					}
					break;
				}

				case 5:
				{
					for (size_t i = 0; i < growTarget[tarNum].size(); i++)
					{
						grow_test_point = growTarget[tarNum][i];
						test1.at<Vec3b>(grow_test_point.y, grow_test_point.x)[0] = 0;
						test1.at<Vec3b>(grow_test_point.y, grow_test_point.x)[1] = 0;
						test1.at<Vec3b>(grow_test_point.y, grow_test_point.x)[2] = 255;
					}
					break;
				}
				case 6:
				{
					for (size_t i = 0; i < growTarget[tarNum].size(); i++)
					{
						grow_test_point = growTarget[tarNum][i];
						test1.at<Vec3b>(grow_test_point.y, grow_test_point.x)[0] = 139;
						test1.at<Vec3b>(grow_test_point.y, grow_test_point.x)[1] = 0;
						test1.at<Vec3b>(grow_test_point.y, grow_test_point.x)[2] = 255;
					}
					break;
				}
				case 7:
				{
					for (size_t i = 0; i < growTarget[tarNum].size(); i++)
					{
						grow_test_point = growTarget[tarNum][i];
						test1.at<Vec3b>(grow_test_point.y, grow_test_point.x)[0] = 255;
						test1.at<Vec3b>(grow_test_point.y, grow_test_point.x)[1] = 192;
						test1.at<Vec3b>(grow_test_point.y, grow_test_point.x)[2] = 203;
					}
					break;
				}
				case 8:
				{
					for (size_t i = 0; i < growTarget[tarNum].size(); i++)
					{
						grow_test_point = growTarget[tarNum][i];
						test1.at<Vec3b>(grow_test_point.y, grow_test_point.x)[0] = 160;
						test1.at<Vec3b>(grow_test_point.y, grow_test_point.x)[1] = 82;
						test1.at<Vec3b>(grow_test_point.y, grow_test_point.x)[2] = 45;
					}
					break;
				}
				case 9:
				{
					for (size_t i = 0; i < growTarget[tarNum].size(); i++)
					{
						grow_test_point = growTarget[tarNum][i];
						test1.at<Vec3b>(grow_test_point.y, grow_test_point.x)[0] = 0;
						test1.at<Vec3b>(grow_test_point.y, grow_test_point.x)[1] = 199;
						test1.at<Vec3b>(grow_test_point.y, grow_test_point.x)[2] = 140;
					}
					break;
				}
				case 10:
				{
					for (size_t i = 0; i < growTarget[tarNum].size(); i++)
					{
						grow_test_point = growTarget[tarNum][i];
						test1.at<Vec3b>(grow_test_point.y, grow_test_point.x)[0] = 255;
						test1.at<Vec3b>(grow_test_point.y, grow_test_point.x)[1] = 255;
						test1.at<Vec3b>(grow_test_point.y, grow_test_point.x)[2] = 255;
					}
					break;
				}

			}

	}
	imshow("Picture_test", test1);

	waitKey();
	destroyAllWindows();
	return 0;
}