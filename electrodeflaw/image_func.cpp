#include "electrodeflaw_main.h"
#include "image_func.h"


void tarBorderPoint::tarInit(Mat tarImage)
{
	tarUp = Point2i(0, tarImage.rows);
	tarBottom = Point2i(0, 0);
	tarLeft = Point2i(tarImage.cols, 0);
	tarRight = Point2i(0, 0);
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
Function:  ���������㷨,����ʮ��ʶ��������ʮ�������У�������߽��
Input:     src ������ԭͼ�� pt ��ʼ������ th ��������ֵ����
Output:    �����ǰ�ɫ�����������Ǻ�ɫ
Description: �������������Ϊ��ɫ(255),����ɫΪ��ɫ(0)
Return:    Mat
Others:    NULL
***************************************************************************************/

//Mat RegionGrowAndStore(Mat src, int th, vector<Point2i> growTargetIn[HtNum], vector <tarBorderPoint> growTargetBodrerIn[HtNum])
//{
//
//	Mat matDst = Mat::zeros(src.size(), CV_8UC1);	//����һ���հ��������Ϊ��ɫ
//	Point2i ptGrowing;						//��������λ��
//	int nGrowLable = 0;								//����Ƿ�������
//	int nSrcValue = 0;								//�������Ҷ�ֵ
//	int nCurValue = 0;								//��ǰ������Ҷ�ֵ
//	Point2i pt, pe;										//��������˳������
//	int DIR[8][2] = { { -1,-1 },{ 0,-1 },{ 1,-1 },{ 1,0 },{ 1,1 },{ 0,1 },{ -1,1 },{ -1,0 } };
//	vector<Point2i> vcGrowPt;						//������ջ
//	int tarNum = 0;                                //����ʶ����Ŀ
//
//	//��ʼ�����������ĸ���
//	Point2i tarUpIn = Point2i(0, src.rows);
//	Point2i tarBottomIn = Point2i(0, 0);
//	Point2i tarLeftIn = Point2i(src.cols, 0);
//	Point2i tarRightIn = Point2i(0, 0);
//
//	for (int i = 1; i < src.rows - 1; ++i)
//	{
//		//��ȡ�� i-1,i,i+1 ��������ָ��
//		uchar * s_ct = src.ptr(i);
//		uchar * m_ct = matDst.ptr(i);
//
//		//�Ե� i �е�ÿ������(byte)����
//		for (int j = 1; j < src.cols - 1; ++j)
//		{
//
//			if (s_ct[j] < th && m_ct[j] == 0)
//			{
//				pt = Point(j, i);                        //�������ӵ㣨x��y��
//				vcGrowPt.push_back(pt);							//��������ѹ��ջ��
//				growTargetIn[tarNum].push_back(pt);	                   //��������ѹ��Ŀ��������
//
//
//				//matDst.at<uchar>(pt.y, pt.x) = 255;				//���������
//				*m_ct = 255;				//���������
//											//nSrcValue = src.at<uchar>(pt.y, pt.x);			//��¼������ĻҶ�ֵ
//				nSrcValue = s_ct[j];			//��¼������ĻҶ�ֵ
//				while (!vcGrowPt.empty())						//����ջ��Ϊ��������
//				{
//					pe = vcGrowPt.back();						//�������һ��������
//					vcGrowPt.pop_back();						//ɾ����������
//					for (int s = 0; s < 9; ++s)	                 //�ֱ�԰˸������ϵĵ��������
//					{
//						ptGrowing.x = pe.x + DIR[s][0];
//						ptGrowing.y = pe.y + DIR[s][1];
//						//����Ƿ��Ǳ�Ե��
//						if (ptGrowing.x < 0 || ptGrowing.y < 0 || ptGrowing.x >(src.cols - 1) || (ptGrowing.y > src.rows - 1))
//							continue;
//						nGrowLable = matDst.at<uchar>(ptGrowing.y, ptGrowing.x);		//��ǰ��������ĻҶ�ֵ						
//						if (nGrowLable == 0)					//�����ǵ㻹û�б�����
//						{
//							nCurValue = src.at<uchar>(ptGrowing.y, ptGrowing.x);
//							if (nCurValue < th)					//����ֵ��Χ��������
//							{
//								//matDst.at<uchar>(ptGrowing.y, ptGrowing.x) = 255;		//���Ϊ��ɫ
//								matDst.at<uchar>(ptGrowing.y, ptGrowing.x) = 255;		//���Ϊ��ɫ
//								vcGrowPt.push_back(ptGrowing);					//����һ��������ѹ��ջ��
//								growTargetIn[tarNum].push_back(ptGrowing);     //����һ��������ѹ��Ŀ��������
//								
//							}
//						}
//					}
//				}
//				tarNum = tarNum + 1;
//
//			}
//
//		}
//
//	}
//	return matDst.clone();
//}


/***************************************************************************************
Function:  ���������㷨(��)
Input:     src ������ԭͼ�� pt ��ʼ������ th ��������ֵ����
Output:    ��ʵ�ʵ����ڵ����� ʵ�����ǰ�ɫ�����������Ǻ�ɫ
Description: �������������Ϊ��ɫ(255),����ɫΪ��ɫ(0)
Return:    Mat
Others:    NULL
***************************************************************************************/
Mat RegionGrowAndStore(Mat src, int th, vector<Point2i> growTargetIn[])
{

	Mat matDst = Mat::zeros(src.size(), CV_8UC1);	//����һ���հ��������Ϊ��ɫ
	Point2i ptGrowing;						//��������λ��
	int nGrowLable = 0;								//����Ƿ�������
	int nSrcValue = 0;								//�������Ҷ�ֵ
	int nCurValue = 0;								//��ǰ������Ҷ�ֵ
	Point2i pt, pe;										//��������˳������
	int DIR[8][2] = { { -1,-1 },{ 0,-1 },{ 1,-1 },{ 1,0 },{ 1,1 },{ 0,1 },{ -1,1 },{ -1,0 } };
	vector<Point2i> vcGrowPt;						//������ջ
	int tarNum = 0;

	for (int i = 1; i < src.rows - 1; ++i)
	{
		//��ȡ�� i-1,i,i+1 ��������ָ��
		uchar * s_ct = src.ptr(i);
		uchar * m_ct = matDst.ptr(i);

		//�Ե� i �е�ÿ������(byte)����
		for (int j = 1; j < src.cols - 1; ++j)
		{

			if (s_ct[j] < th && m_ct[j] == 0)
			{
				pt = Point(j, i);                        //�������ӵ㣨x��y��
				vcGrowPt.push_back(pt);							//��������ѹ��ջ��
																//growTargetIn[tarNum].push_back(pt);	                   //��������ѹ��Ŀ��������
																//	growTargetIn[tarNum].push_back(pt);
																//matDst.at<uchar>(pt.y, pt.x) = 255;				//���������
				*m_ct = 255;				//���������
											//nSrcValue = src.at<uchar>(pt.y, pt.x);			//��¼������ĻҶ�ֵ
				nSrcValue = s_ct[j];			//��¼������ĻҶ�ֵ
				while (!vcGrowPt.empty())						//����ջ��Ϊ��������
				{
					pe = vcGrowPt.back();						//�������һ��������
					vcGrowPt.pop_back();						//ɾ����������
					for (int s = 0; s < 9; ++s)	                 //�ֱ�԰˸������ϵĵ��������
					{
						ptGrowing.x = pe.x + DIR[s][0];
						ptGrowing.y = pe.y + DIR[s][1];
						//����Ƿ��Ǳ�Ե��
						if (ptGrowing.x < 0 || ptGrowing.y < 0 || ptGrowing.x >(src.cols - 1) || (ptGrowing.y > src.rows - 1))
							continue;
						nGrowLable = matDst.at<uchar>(ptGrowing.y, ptGrowing.x);		//��ǰ��������ĻҶ�ֵ						
						if (nGrowLable == 0)					//�����ǵ㻹û�б�����
						{
							nCurValue = src.at<uchar>(ptGrowing.y, ptGrowing.x);
							if (nCurValue < th)					//����ֵ��Χ��������
							{
								//matDst.at<uchar>(ptGrowing.y, ptGrowing.x) = 255;		//���Ϊ��ɫ
								matDst.at<uchar>(ptGrowing.y, ptGrowing.x) = 255;		//���Ϊ��ɫ
								vcGrowPt.push_back(ptGrowing);					//����һ��������ѹ��ջ��
								growTargetIn[tarNum].push_back(ptGrowing);     //����һ��������ѹ��Ŀ��������
							}
						}
					}
				}
				tarNum = tarNum + 1;
			}

		}

	}
	return matDst.clone();
}