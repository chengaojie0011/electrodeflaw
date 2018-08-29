#include "electrodeflaw_main.h"
#include "image_func.h"

//������ķ���
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
Mat RegionGrowAndStore(Mat src, int th, vector<Point2i> grow_target_in[]) {

	Mat mat_dst = Mat::zeros(src.size(), CV_8UC1);	//����һ���հ��������Ϊ��ɫ
	Point2i ptGrowing;						//��������λ��
	int nGrowLable = 0;								//����Ƿ�������
	int nSrcValue = 0;								//�������Ҷ�ֵ
	int nCurValue = 0;								//��ǰ������Ҷ�ֵ
	Point2i pt, pe;										//�������ӵ㣬��������˳������
	int DIR[8][2] = { { -1,-1 },{ 0,-1 },{ 1,-1 },{ 1,0 },{ 1,1 },{ 0,1 },{ -1,1 },{ -1,0 } };
	vector<Point2i> vc_grow_pt;						//������ջ
	int tarnum = 0;

	for (int i = 1; i < src.rows - 1; ++i) {
		//��ȡ�� i-1,i,i+1 ��������ָ��
		uchar * s_ct = src.ptr(i);
		uchar * m_ct = mat_dst.ptr(i);

		//�Ե� i �е�ÿ������(byte)����
		for (int j = 1; j < src.cols - 1; ++j) {
			if (s_ct[j] < th && m_ct[j] == 0) {
				pt = Point(j, i);                        //�������ӵ㣨x��y��
				vc_grow_pt.push_back(pt);							//��������ѹ��ջ��
				*m_ct = 255;				//���������
				nSrcValue = s_ct[j];			//��¼������ĻҶ�ֵ
				while (!vc_grow_pt.empty())	{					//����ջ��Ϊ��������

					pe = vc_grow_pt.back();						//�������һ��������
					vc_grow_pt.pop_back();						//ɾ����������

					for (int s = 0; s < 9; ++s) {	                 //�ֱ�԰˸������ϵĵ��������
					
						ptGrowing.x = pe.x + DIR[s][0];
						ptGrowing.y = pe.y + DIR[s][1];
						//����Ƿ��Ǳ�Ե��

						if (ptGrowing.x < 0 || ptGrowing.y < 0 || ptGrowing.x >(src.cols - 1) || (ptGrowing.y > src.rows - 1))
							continue;

						nGrowLable = mat_dst.at<uchar>(ptGrowing.y, ptGrowing.x);		//��ǰ��������ĻҶ�ֵ						
						if (nGrowLable == 0) {					//�����ǵ㻹û�б�����
						
							nCurValue = src.at<uchar>(ptGrowing.y, ptGrowing.x);
							if (nCurValue < th) {				//����ֵ��Χ��������
	
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


void ShowElectrodeVectorColor(Mat grow_image,const vector<Point2i> grow_target_in[]) {
	Point2i grow_test_point;
	Mat show_image = Mat::zeros(grow_image.size(), CV_8UC3);	//����һ���հ��������Ϊ��ɫ

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
	//imwrite("find_image.png", show_image);   //��matд�뵽�ļ�
}

void FindElectrodeContours(Mat src, vector <Electrode> &electrodes_output) {

	//int element_value = src.cols / 150;       //���ø�ʴ����ֵ
	//Mat element = getStructuringElement(MORPH_RECT, Size(element_value, element_value));
	//������̬ѧ��ʴ����
	//morphologyEx(src, src, MORPH_ERODE, element);
	vector<vector<Point>> contours;   //����������
	vector<Vec4i> hierarchy;
	findContours(src, contours, hierarchy, RETR_TREE,CHAIN_APPROX_NONE, Point(0, 0));
	Mat find_image = Mat::zeros(src.size(), CV_8UC3);
	RNG rng(0);
	//�趨����������ܳ���ֵ
	//int area_min = 6000, area_max = 10000;
	//int length_min = 1300, length_max = 1700;
	RotatedRect result_rect;
	Electrode electrode_model;
	//int electrodes_number = 0;  //ͳ�ƺ�������
	Point2f pt[4];
	//�趨�����������ֵ
	float contours_width, contours_height,contours_scale;
	float contours_factor = float(src.rows/740.0);
	float width_min = 12* contours_factor, width_max = 18*contours_factor;
	float height_min = 700* contours_factor, height_max = 750* contours_factor;
	float scale_min = 45.0, scale_max =60.0;
	for (int i = 0; i < contours.size(); i++) {
			result_rect = minAreaRect(contours[i]);//��ȡ��������С��Ӿ��� 
			result_rect.points(pt);//��ȡ��С��Ӿ��ε��ĸ���������
			//��Ӿ��ε�width��height��ѡȡ����νǶ��йأ�����������бȶԣ�ѡȡ����Ϊheight
			if (result_rect.size.width < result_rect.size.height) {
				contours_width = result_rect.size.width;
				contours_height = result_rect.size.height;
			}else
			{
				contours_height = result_rect.size.width;
				contours_width = result_rect.size.height;
			}
			//cout << "��" << contours_width << endl;
			//cout << "�ߣ�" << contours_height << endl;
			contours_scale = contours_height / contours_width;
			//�Ի�ȡ�����ĳ������ɸѡ
			if (contours_width > width_min && contours_width < width_max && 
				contours_height > height_min && contours_height < height_max &&
				contours_scale > scale_min && contours_scale < scale_max) {
				cout << "��" << contours_width << endl;
				cout << "�ߣ�" << contours_height << endl;
				cout << "������" << contours_scale << endl;
				Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
				drawContours(find_image, contours, i, color, 1, 8, hierarchy, 0, Point(0, 0));
				electrode_model.GetContourCenter(result_rect.center.x, result_rect.center.y);
				electrode_model.GetContourPoints(contours[i]);
				electrode_model.GetContourWidthHeight(contours_width, contours_height);
				electrode_model.sort_number_ = 0;
				electrodes_output.push_back(electrode_model);
				//������С��Ӿ���
				/*line(find_image, pt[0], pt[1], color, 2, 8);
				line(find_image, pt[1], pt[2], color, 2, 8);
				line(find_image, pt[2], pt[3], color, 2, 8);
				line(find_image, pt[3], pt[0], color, 2, 8);*/
			}
		
	}
	//namedWindow( "findContours", CV_WINDOW_AUTOSIZE);
	//imshow( "findContours", find_image);
	//imwrite("find_image.png", find_image);   //��matд�뵽�ļ�

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
				//cout << "��"<< i <<"���������к�Ϊ:"<< electrodes_sort[i].sort_number_ << endl;
			}
		}
		
	}
	delete sort;

}

void ShowSortNumber(Mat src, vector <Electrode> &electrodes_number) {
	//���û����ı�����ز���
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