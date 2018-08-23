#include "electrodeflaw_main.h"
#include "image_func.h"


void TarBorderPoint::TarInit(Mat tar_image)
{
	tar_up_ = Point2i(0, tar_image.rows);
	tar_bottom_ = Point2i(0, 0);
	tar_left_ = Point2i(tar_image.cols, 0);
	tar_right_ = Point2i(0, 0);
}


/***************************************************************************************
Function:  ���������㷨(��)
Input:     src ������ԭͼ�� pt ��ʼ������ th ��������ֵ����
Output:    ��ʵ�ʵ����ڵ����� ʵ�����ǰ�ɫ�����������Ǻ�ɫ
Description: �������������Ϊ��ɫ(255),����ɫΪ��ɫ(0)
Return:    Mat
Others:    NULL
***************************************************************************************/

Mat RegionGrow(Mat src, Point2i pt, int th)
{
	Point2i ptGrowing;						//��������λ��
	int nGrowLable = 0;								//����Ƿ�������
	int nSrcValue = 0;								//�������Ҷ�ֵ
	int nCurValue = 0;								//��ǰ������Ҷ�ֵ
	Mat matDst = Mat::zeros(src.size(), CV_8UC1);	//����һ���հ��������Ϊ��ɫ
													//��������˳������
	int DIR[8][2] = { { -1,-1 },{ 0,-1 },{ 1,-1 },{ 1,0 },{ 1,1 },{ 0,1 },{ -1,1 },{ -1,0 } };
	vector<Point2i> vcGrowPt;						//������ջ
	vcGrowPt.push_back(pt);							//��������ѹ��ջ��
	matDst.at<uchar>(pt.y, pt.x) = 255;				//���������
	nSrcValue = src.at<uchar>(pt.y, pt.x);			//��¼������ĻҶ�ֵ

	while (!vcGrowPt.empty())						//����ջ��Ϊ��������
	{
		pt = vcGrowPt.back();						//�������һ��������
		vcGrowPt.pop_back();						//ɾ����������

													//�ֱ�԰˸������ϵĵ��������
		for (int i = 0; i<9; ++i)
		{
			ptGrowing.x = pt.x + DIR[i][0];
			ptGrowing.y = pt.y + DIR[i][1];
			//����Ƿ��Ǳ�Ե��
			if (ptGrowing.x < 0 || ptGrowing.y < 0 || ptGrowing.x >(src.cols - 1) || (ptGrowing.y > src.rows - 1))
				continue;

			nGrowLable = matDst.at<uchar>(ptGrowing.y, ptGrowing.x);		//��ǰ��������ĻҶ�ֵ

			if (nGrowLable == 0)					//�����ǵ㻹û�б�����
			{
				nCurValue = src.at<uchar>(ptGrowing.y, ptGrowing.x);
				if (abs(nSrcValue - nCurValue) < th)					//����ֵ��Χ��������
				{
					matDst.at<uchar>(ptGrowing.y, ptGrowing.x) = 255;		//���Ϊ��ɫ
					vcGrowPt.push_back(ptGrowing);					//����һ��������ѹ��ջ��
				}
			}
		}
	}
	return matDst.clone();
}


/***************************************************************************************
Function:  ���������㷨������ʮ��ʶ��������ʮ��������
Input:     src ������ԭͼ��   th ��������ֵ����
Output:    �����ǰ�ɫ�������Ǻ�ɫ
Description: �������������Ϊ��ɫ(255),����ɫΪ��ɫ(0)
Return:    Mat
Others:    NULL
***************************************************************************************/
Mat RegionGrowAndStore(Mat src, int th, vector<Point2i> grow_target_in[])
{

	Mat mat_dst = Mat::zeros(src.size(), CV_8UC1);	//����һ���հ��������Ϊ��ɫ
	Point2i ptGrowing;						//��������λ��
	int nGrowLable = 0;								//����Ƿ�������
	int nSrcValue = 0;								//�������Ҷ�ֵ
	int nCurValue = 0;								//��ǰ������Ҷ�ֵ
	Point2i pt, pe;										//�������ӵ㣬��������˳������
	int DIR[8][2] = { { -1,-1 },{ 0,-1 },{ 1,-1 },{ 1,0 },{ 1,1 },{ 0,1 },{ -1,1 },{ -1,0 } };
	vector<Point2i> vc_grow_pt;						//������ջ
	int tarnum = 0;

	for (int i = 1; i < src.rows - 1; ++i)
	{
		//��ȡ�� i-1,i,i+1 ��������ָ��
		uchar * s_ct = src.ptr(i);
		uchar * m_ct = mat_dst.ptr(i);

		//�Ե� i �е�ÿ������(byte)����
		for (int j = 1; j < src.cols - 1; ++j)
		{

			if (s_ct[j] < th && m_ct[j] == 0)
			{
				pt = Point(j, i);                        //�������ӵ㣨x��y��
				vc_grow_pt.push_back(pt);							//��������ѹ��ջ��
				*m_ct = 255;				//���������
				nSrcValue = s_ct[j];			//��¼������ĻҶ�ֵ
				while (!vc_grow_pt.empty())						//����ջ��Ϊ��������
				{
					pe = vc_grow_pt.back();						//�������һ��������
					vc_grow_pt.pop_back();						//ɾ����������
					for (int s = 0; s < 9; ++s)	                 //�ֱ�԰˸������ϵĵ��������
					{
						ptGrowing.x = pe.x + DIR[s][0];
						ptGrowing.y = pe.y + DIR[s][1];
						//����Ƿ��Ǳ�Ե��
						if (ptGrowing.x < 0 || ptGrowing.y < 0 || ptGrowing.x >(src.cols - 1) || (ptGrowing.y > src.rows - 1))
							continue;
						nGrowLable = mat_dst.at<uchar>(ptGrowing.y, ptGrowing.x);		//��ǰ��������ĻҶ�ֵ						
						if (nGrowLable == 0)					//�����ǵ㻹û�б�����
						{
							nCurValue = src.at<uchar>(ptGrowing.y, ptGrowing.x);
							if (nCurValue < th)					//����ֵ��Χ��������
							{
								//matDst.at<uchar>(ptGrowing.y, ptGrowing.x) = 255;		//���Ϊ��ɫ
								mat_dst.at<uchar>(ptGrowing.y, ptGrowing.x) = 255;		//���Ϊ��ɫ
								vc_grow_pt.push_back(ptGrowing);					//����һ��������ѹ��ջ��
								grow_target_in[tarnum].push_back(ptGrowing);     //����һ��������ѹ��Ŀ��������
							}
						}
					}
				}
				tarnum = tarnum + 1;
			}

		}

	}
	return mat_dst.clone();
}


void ShowElectrodeVectorColor(Mat grow_image,const vector<Point2i> grow_target_in[])
{
	Point2i grow_test_point;
	Mat show_image = Mat::zeros(grow_image.size(), CV_8UC3);	//����һ���հ��������Ϊ��ɫ

	for (int tar_num = 0; tar_num < HTNUM; ++tar_num)
	{
		switch (tar_num)
		{
		case 0:
		{
			for (size_t i = 0; i < grow_target_in[tar_num].size(); i++)
			{
				grow_test_point = grow_target_in[tar_num][i];
				show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[0] = 255;
				show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[1] = 0;
				show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[2] = 0;
			}
			break;
		}
		case 1:
		{
			for (size_t i = 0; i < grow_target_in[tar_num].size(); i++)
			{
				grow_test_point = grow_target_in[tar_num][i];
				show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[0] = 255;
				show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[1] = 165;
				show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[2] = 0;
			}
			break;
		}
		case 2:
		{
			for (size_t i = 0; i < grow_target_in[tar_num].size(); i++)
			{
				grow_test_point = grow_target_in[tar_num][i];
				show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[0] = 255;
				show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[1] = 255;
				show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[2] = 0;
			}
			break;
		}
		case 3:
		{
			for (size_t i = 0; i < grow_target_in[tar_num].size(); i++)
			{
				grow_test_point = grow_target_in[tar_num][i];
				show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[0] = 0;
				show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[1] = 255;
				show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[2] = 0;
			}
			break;
		}
		case 4:
		{
			for (size_t i = 0; i < grow_target_in[tar_num].size(); i++)
			{
				grow_test_point = grow_target_in[tar_num][i];
				show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[0] = 0;
				show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[1] = 127;
				show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[2] = 255;
			}
			break;
		}

		case 5:
		{
			for (size_t i = 0; i < grow_target_in[tar_num].size(); i++)
			{
				grow_test_point = grow_target_in[tar_num][i];
				show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[0] = 0;
				show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[1] = 0;
				show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[2] = 255;
			}
			break;
		}
		case 6:
		{
			for (size_t i = 0; i < grow_target_in[tar_num].size(); i++)
			{
				grow_test_point = grow_target_in[tar_num][i];
				show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[0] = 139;
				show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[1] = 0;
				show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[2] = 255;
			}
			break;
		}
		case 7:
		{
			for (size_t i = 0; i < grow_target_in[tar_num].size(); i++)
			{
				grow_test_point = grow_target_in[tar_num][i];
				show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[0] = 255;
				show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[1] = 192;
				show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[2] = 203;
			}
			break;
		}
		case 8:
		{
			for (size_t i = 0; i < grow_target_in[tar_num].size(); i++)
			{
				grow_test_point = grow_target_in[tar_num][i];
				show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[0] = 160;
				show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[1] = 82;
				show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[2] = 45;
			}
			break;
		}
		case 9:
		{
			for (size_t i = 0; i < grow_target_in[tar_num].size(); i++)
			{
				grow_test_point = grow_target_in[tar_num][i];
				show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[0] = 0;
				show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[1] = 199;
				show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[2] = 140;
			}
			break;
		}
		case 10:
		{
			for (size_t i = 0; i < grow_target_in[tar_num].size(); i++)
			{
				grow_test_point = grow_target_in[tar_num][i];
				show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[0] = 255;
				show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[1] = 255;
				show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[2] = 255;
			}
			break;
		}

		}

	}
	imshow("Picture_color", show_image);
}