#include <iostream>
#include <string>
using namespace std;

#include <opencv2\opencv.hpp>

#include "../clcnst/clcnst.h"

float threshold;
string ifname, ofname;

int main(int argc, char** argv) {
	if(argc < 1) {
		cout << "usage: HornAlgorithm.exe" << endl;
		return -1;
	}

	cout << "[HornAlgorithm] input file name: ";
	cin >> ifname;

	cv::Mat img = cv::imread(ifname, CV_LOAD_IMAGE_COLOR);
	if(img.empty()) {
		cout << "Failed to load file \"" << ifname << "\"." << endl;
		return -1;
	}
	int width = img.cols;
	int height = img.rows;
	int channel = img.channels();
	img.convertTo(img, CV_32FC3, 1.0 / 255.0);

	cout << "[HornAlgorithm] input threshold value (default = 0.05): ";
	cin >> threshold;

	cv::Mat out, laplace;

	// �P�x�̑ΐ������
	clcnst::logarithm(img, out);
	
	// ���v���V�A���t�B���^��������
	clcnst::laplacian(out, laplace);

	// 臒l����
	clcnst::threshold(laplace, laplace, threshold);
		
	// �K�E�X�E�U�C�f���@
	clcnst::gauss_seidel(out, laplace, 20);		

	// ���K��
	clcnst::normalize(out, out);

	// �w�����Ƃ�v�Z
	clcnst::exponential(out, out);
		
	cv::namedWindow("Input");
	cv::namedWindow("Output");
	cv::imshow("Input", img);
	cv::imshow("Output", out);
	cv::waitKey(0);
	cv::destroyAllWindows();

	// Save output
	cout << "[HornAlgorithm] save as: ";
	cin >> ofname;
	out.convertTo(out, CV_8UC3, 255.0);
	cv::imwrite(ofname, out);
}
