#include "electrodeflaw_main.h"
#include "image_func.h"

//焊条类的方法
void Electrode::GetContourWidthHeight(float width, float height) {
	e_width = width;
	e_height = height;
	return;
}
void Electrode::GetContourCenter(int x, int y) {
	contour_center_.x = x;
	contour_center_.y = y;
	return;
}

void Electrode::GetContourPoints(vector <Point2i> points) {
	contour_points_ = points;
	return;
}

void Electrode::DisplayContourCenter() {
	cout << "contour_center_.x:" << contour_center_.x << endl;
	cout << "contour_center_.y:" << contour_center_.y << endl;

	return;
}
Point2i Electrode::OutputContourCenter() {
	return contour_center_;
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
Mat RegionGrowAndStore(Mat src, int th, vector<Point2i> grow_target_in[]) {

	Mat mat_dst = Mat::zeros(src.size(), CV_8UC1);	//创建一个空白区域，填充为黑色
	Point2i ptGrowing;						//待生长点位置
	int nGrowLable = 0;								//标记是否生长过
	int nSrcValue = 0;								//生长起点灰度值
	int nCurValue = 0;								//当前生长点灰度值
	Point2i pt, pe;										//定义种子点，生长方向顺序数据
	int DIR[8][2] = { { -1,-1 },{ 0,-1 },{ 1,-1 },{ 1,0 },{ 1,1 },{ 0,1 },{ -1,1 },{ -1,0 } };
	vector<Point2i> vc_grow_pt;						//生长点栈
	int tarnum = 0;

	for (int i = 1; i < src.rows - 1; ++i) {
		//获取第 i-1,i,i+1 行首像素指针
		uchar * s_ct = src.ptr(i);
		uchar * m_ct = mat_dst.ptr(i);

		//对第 i 行的每个像素(byte)操作
		for (int j = 1; j < src.cols - 1; ++j) {
			if (s_ct[j] < th && m_ct[j] == 0) {
				pt = Point(j, i);                        //定义种子点（x，y）
				vc_grow_pt.push_back(pt);							//将生长点压入栈中
				*m_ct = 255;				//标记生长点
				nSrcValue = s_ct[j];			//记录生长点的灰度值
				while (!vc_grow_pt.empty())	{					//生长栈不为空则生长

					pe = vc_grow_pt.back();						//返回最后一个生长点
					vc_grow_pt.pop_back();						//删除该生长点

					for (int s = 0; s < 9; ++s) {	                 //分别对八个方向上的点进行生长
					
						ptGrowing.x = pe.x + DIR[s][0];
						ptGrowing.y = pe.y + DIR[s][1];
						//检查是否是边缘点

						if (ptGrowing.x < 0 || ptGrowing.y < 0 || ptGrowing.x >(src.cols - 1) || (ptGrowing.y > src.rows - 1))
							continue;

						nGrowLable = mat_dst.at<uchar>(ptGrowing.y, ptGrowing.x);		//当前待生长点的灰度值						
						if (nGrowLable == 0) {					//如果标记点还没有被生长
						
							nCurValue = src.at<uchar>(ptGrowing.y, ptGrowing.x);
							if (nCurValue < th) {				//在阈值范围内则生长
	
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


void ShowElectrodeVectorColor(Mat grow_image,const vector<Point2i> grow_target_in[]) {
	Point2i grow_test_point;
	Mat show_image = Mat::zeros(grow_image.size(), CV_8UC3);	//创建一个空白区域，填充为黑色

	for (int tar_num = 0; tar_num < HTNUM; ++tar_num) {
		switch (tar_num) {
			case 0: {
				for (size_t i = 0; i < grow_target_in[tar_num].size(); i++) {
					grow_test_point = grow_target_in[tar_num][i];
					show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[0] = 255;
					show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[1] = 0;
					show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[2] = 0;
				}
				break;
			}
			case 1: {
				for (size_t i = 0; i < grow_target_in[tar_num].size(); i++) {
					grow_test_point = grow_target_in[tar_num][i];
					show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[0] = 255;
					show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[1] = 165;
					show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[2] = 0;
				}
				break;
			}
			case 2: {
				for (size_t i = 0; i < grow_target_in[tar_num].size(); i++) {
					grow_test_point = grow_target_in[tar_num][i];
					show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[0] = 255;
					show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[1] = 255;
					show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[2] = 0;
				}
				break;
			}
			case 3: {
				for (size_t i = 0; i < grow_target_in[tar_num].size(); i++) {
					grow_test_point = grow_target_in[tar_num][i];
					show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[0] = 0;
					show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[1] = 255;
					show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[2] = 0;
				}
				break;
			}
			case 4: {
				for (size_t i = 0; i < grow_target_in[tar_num].size(); i++) {
					grow_test_point = grow_target_in[tar_num][i];
					show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[0] = 0;
					show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[1] = 127;
					show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[2] = 255;
				}
				break;
			}

			case 5: {
				for (size_t i = 0; i < grow_target_in[tar_num].size(); i++) {
					grow_test_point = grow_target_in[tar_num][i];
					show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[0] = 0;
					show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[1] = 0;
					show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[2] = 255;
				}
				break;
			}
			case 6: {
				for (size_t i = 0; i < grow_target_in[tar_num].size(); i++) {
					grow_test_point = grow_target_in[tar_num][i];
					show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[0] = 139;
					show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[1] = 0;
					show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[2] = 255;
				}
				break;
			}
			case 7: {
				for (size_t i = 0; i < grow_target_in[tar_num].size(); i++) {
					grow_test_point = grow_target_in[tar_num][i];
					show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[0] = 255;
					show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[1] = 192;
					show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[2] = 203;
				}
				break;
			}
			case 8: {
				for (size_t i = 0; i < grow_target_in[tar_num].size(); i++) {
					grow_test_point = grow_target_in[tar_num][i];
					show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[0] = 160;
					show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[1] = 82;
					show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[2] = 45;
				}
				break;
			}
			case 9: {
				for (size_t i = 0; i < grow_target_in[tar_num].size(); i++) {
					grow_test_point = grow_target_in[tar_num][i];
					show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[0] = 0;
					show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[1] = 199;
					show_image.at<Vec3b>(grow_test_point.y, grow_test_point.x)[2] = 140;
				}
				break;
			}
			case 10: {
				for (size_t i = 0; i < grow_target_in[tar_num].size(); i++) {
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
	//imwrite("find_image.png", show_image);   //将mat写入到文件
}

void FindElectrodeContours(Mat src, vector <Electrode> &electrodes_output) {

	//int element_value = src.cols / 150;       //设置腐蚀的阈值
	//Mat element = getStructuringElement(MORPH_RECT, Size(element_value, element_value));
	//进行形态学腐蚀操作
	//morphologyEx(src, src, MORPH_ERODE, element);
	vector<vector<Point>> contours;   //轮廓的容器
	vector<Vec4i> hierarchy;
	findContours(src, contours, hierarchy, RETR_TREE,CHAIN_APPROX_NONE, Point(0, 0));
	Mat find_image = Mat::zeros(src.size(), CV_8UC3);
	RNG rng(0);
	//设定焊条面积和周长阈值
	//int area_min = 6000, area_max = 10000;
	//int length_min = 1300, length_max = 1700;
	RotatedRect result_rect;
	Electrode electrode_model;
	//int electrodes_number = 0;  //统计焊条数量
	Point2f pt[4];
	//设定焊条长宽的阈值
	float contours_width, contours_height,contours_scale;
	float contours_factor = float(src.rows/740.0);
	float width_min = 12* contours_factor, width_max = 18*contours_factor;
	float height_min = 700* contours_factor, height_max = 750* contours_factor;
	float scale_min = 45.0, scale_max =60.0;
	for (int i = 0; i < contours.size(); i++) {
			result_rect = minAreaRect(contours[i]);//获取轮廓的最小外接矩形 
			result_rect.points(pt);//获取最小外接矩形的四个顶点坐标
			//外接矩形的width和height的选取与矩形角度有关，我们这里进行比对，选取长的为height
			if (result_rect.size.width < result_rect.size.height) {
				contours_width = result_rect.size.width;
				contours_height = result_rect.size.height;
			}else
			{
				contours_height = result_rect.size.width;
				contours_width = result_rect.size.height;
			}
			//cout << "宽：" << contours_width << endl;
			//cout << "高：" << contours_height << endl;
			contours_scale = contours_height / contours_width;
			//对获取轮廓的长宽进行筛选
			if (contours_width > width_min && contours_width < width_max && 
				contours_height > height_min && contours_height < height_max &&
				contours_scale > scale_min && contours_scale < scale_max) {
				cout << "宽：" << contours_width << endl;
				cout << "高：" << contours_height << endl;
				cout << "比例：" << contours_scale << endl;
				Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
				drawContours(find_image, contours, i, color, 1, 8, hierarchy, 0, Point(0, 0));
				electrode_model.GetContourCenter(result_rect.center.x, result_rect.center.y);
				electrode_model.GetContourPoints(contours[i]);
				electrode_model.GetContourWidthHeight(contours_width, contours_height);
				electrode_model.sort_number_ = 0;
				electrodes_output.push_back(electrode_model);
				//绘制最小外接矩形
				/*line(find_image, pt[0], pt[1], color, 2, 8);
				line(find_image, pt[1], pt[2], color, 2, 8);
				line(find_image, pt[2], pt[3], color, 2, 8);
				line(find_image, pt[3], pt[0], color, 2, 8);*/
			}
		
	}
	//namedWindow( "findContours", CV_WINDOW_AUTOSIZE);
	//imshow( "findContours", find_image);
	//imwrite("find_image.png", find_image);   //将mat写入到文件

}

void SortElectrode(vector <Electrode> &electrodes_sort) {
	
	int num = electrodes_sort.size();
	int *sort= new int[electrodes_sort.size()];
	Point2i center,center_sort;
	for (int i = 0; i < electrodes_sort.size(); i++){
		center= electrodes_sort[i].OutputContourCenter();
		sort[i] = center.x;
	}
	for (int i = 0; i < electrodes_sort.size() - 1; i++) { // times
		for (int j = 0; j < electrodes_sort.size() - i - 1; j++) { // position
			if (sort[j] > sort[j + 1]) {
				int temp = sort[j];
				sort[j] = sort[j + 1];
				sort[j + 1] = temp;
			}
		}
	}
	for (int i = 0; i < electrodes_sort.size(); i++) {
		center_sort = electrodes_sort[i].OutputContourCenter();
		for (int j = 0; j < electrodes_sort.size(); j++) {
			if (center_sort.x== sort[j])
			{
				electrodes_sort[i].sort_number_ = j;
				//cout << "第"<< i <<"个焊条序列号为:"<< electrodes_sort[i].sort_number_ << endl;
			}
		}
		
	}
	delete sort;

}

void ShowSortNumber(Mat src, vector <Electrode> &electrodes_number) {
	//设置绘制文本的相关参数
	int font_face = cv::FONT_HERSHEY_COMPLEX;
	double font_scale = 0.5;
	int thickness = 1;
	int baseline;
	Point2i center_sort;
	int sort_num;
	for (int i = 0; i < electrodes_number.size(); i++) {
		center_sort = electrodes_number[i].OutputContourCenter();
		sort_num = electrodes_number[i].sort_number_;
		string text = to_string(sort_num);
		//Size text_size = getTextSize(text, font_face, font_scale, thickness, &baseline);
		putText(src, text, center_sort, font_face, font_scale, cv::Scalar(0, 0, 255), thickness, 8, 0);
	}
	//namedWindow("Picture_thes", CV_WINDOW_AUTOSIZE);
	//imshow("Picture_thes", src);
}