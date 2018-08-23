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
Function:  区域生长算法(例)
Input:     src 待处理原图像 pt 初始生长点 th 生长的阈值条件
Output:    肺实质的所在的区域 实质区是白色，其他区域是黑色
Description: 生长结果区域标记为白色(255),背景色为黑色(0)
Return:    Mat
Others:    NULL
***************************************************************************************/

Mat RegionGrow(Mat src, Point2i pt, int th)
{
	Point2i ptGrowing;						//待生长点位置
	int nGrowLable = 0;								//标记是否生长过
	int nSrcValue = 0;								//生长起点灰度值
	int nCurValue = 0;								//当前生长点灰度值
	Mat matDst = Mat::zeros(src.size(), CV_8UC1);	//创建一个空白区域，填充为黑色
													//生长方向顺序数据
	int DIR[8][2] = { { -1,-1 },{ 0,-1 },{ 1,-1 },{ 1,0 },{ 1,1 },{ 0,1 },{ -1,1 },{ -1,0 } };
	vector<Point2i> vcGrowPt;						//生长点栈
	vcGrowPt.push_back(pt);							//将生长点压入栈中
	matDst.at<uchar>(pt.y, pt.x) = 255;				//标记生长点
	nSrcValue = src.at<uchar>(pt.y, pt.x);			//记录生长点的灰度值

	while (!vcGrowPt.empty())						//生长栈不为空则生长
	{
		pt = vcGrowPt.back();						//返回最后一个生长点
		vcGrowPt.pop_back();						//删除该生长点

													//分别对八个方向上的点进行生长
		for (int i = 0; i<9; ++i)
		{
			ptGrowing.x = pt.x + DIR[i][0];
			ptGrowing.y = pt.y + DIR[i][1];
			//检查是否是边缘点
			if (ptGrowing.x < 0 || ptGrowing.y < 0 || ptGrowing.x >(src.cols - 1) || (ptGrowing.y > src.rows - 1))
				continue;

			nGrowLable = matDst.at<uchar>(ptGrowing.y, ptGrowing.x);		//当前待生长点的灰度值

			if (nGrowLable == 0)					//如果标记点还没有被生长
			{
				nCurValue = src.at<uchar>(ptGrowing.y, ptGrowing.x);
				if (abs(nSrcValue - nCurValue) < th)					//在阈值范围内则生长
				{
					matDst.at<uchar>(ptGrowing.y, ptGrowing.x) = 255;		//标记为白色
					vcGrowPt.push_back(ptGrowing);					//将下一个生长点压入栈中
				}
			}
		}
	}
	return matDst.clone();
}


/***************************************************************************************
Function:  区域生长算法，并将十根识别物填入十个容器中
Input:     src 待处理原图像   th 生长的阈值条件
Output:    焊条是白色，背景是黑色
Description: 生长结果区域标记为白色(255),背景色为黑色(0)
Return:    Mat
Others:    NULL
***************************************************************************************/
Mat RegionGrowAndStore(Mat src, int th, vector<Point2i> grow_target_in[])
{

	Mat mat_dst = Mat::zeros(src.size(), CV_8UC1);	//创建一个空白区域，填充为黑色
	Point2i ptGrowing;						//待生长点位置
	int nGrowLable = 0;								//标记是否生长过
	int nSrcValue = 0;								//生长起点灰度值
	int nCurValue = 0;								//当前生长点灰度值
	Point2i pt, pe;										//定义种子点，生长方向顺序数据
	int DIR[8][2] = { { -1,-1 },{ 0,-1 },{ 1,-1 },{ 1,0 },{ 1,1 },{ 0,1 },{ -1,1 },{ -1,0 } };
	vector<Point2i> vc_grow_pt;						//生长点栈
	int tarnum = 0;

	for (int i = 1; i < src.rows - 1; ++i)
	{
		//获取第 i-1,i,i+1 行首像素指针
		uchar * s_ct = src.ptr(i);
		uchar * m_ct = mat_dst.ptr(i);

		//对第 i 行的每个像素(byte)操作
		for (int j = 1; j < src.cols - 1; ++j)
		{

			if (s_ct[j] < th && m_ct[j] == 0)
			{
				pt = Point(j, i);                        //定义种子点（x，y）
				vc_grow_pt.push_back(pt);							//将生长点压入栈中
				*m_ct = 255;				//标记生长点
				nSrcValue = s_ct[j];			//记录生长点的灰度值
				while (!vc_grow_pt.empty())						//生长栈不为空则生长
				{
					pe = vc_grow_pt.back();						//返回最后一个生长点
					vc_grow_pt.pop_back();						//删除该生长点
					for (int s = 0; s < 9; ++s)	                 //分别对八个方向上的点进行生长
					{
						ptGrowing.x = pe.x + DIR[s][0];
						ptGrowing.y = pe.y + DIR[s][1];
						//检查是否是边缘点
						if (ptGrowing.x < 0 || ptGrowing.y < 0 || ptGrowing.x >(src.cols - 1) || (ptGrowing.y > src.rows - 1))
							continue;
						nGrowLable = mat_dst.at<uchar>(ptGrowing.y, ptGrowing.x);		//当前待生长点的灰度值						
						if (nGrowLable == 0)					//如果标记点还没有被生长
						{
							nCurValue = src.at<uchar>(ptGrowing.y, ptGrowing.x);
							if (nCurValue < th)					//在阈值范围内则生长
							{
								//matDst.at<uchar>(ptGrowing.y, ptGrowing.x) = 255;		//标记为白色
								mat_dst.at<uchar>(ptGrowing.y, ptGrowing.x) = 255;		//标记为白色
								vc_grow_pt.push_back(ptGrowing);					//将下一个生长点压入栈中
								grow_target_in[tarnum].push_back(ptGrowing);     //将下一个生长点压入目标容器中
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
	Mat show_image = Mat::zeros(grow_image.size(), CV_8UC3);	//创建一个空白区域，填充为黑色

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