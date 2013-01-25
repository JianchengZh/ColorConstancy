#include <iostream>
using namespace std;

#include <opencv2\opencv.hpp>

#include "../clcnst/clcnst.h"

int main(int argc, char** argv) {
	if(argc < 3) {
		cout << "usage: MooreAlgorithm.exe [input image] [output image] [threshold]" << endl;
		return -1;
	}

	cv::Mat img = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);
	if(img.empty()) {
		cout << "Failed to load file \"" << argv[1] << "\"." << endl;
		return -1;
	}
	int width = img.cols;
	int height = img.rows;
	int channel = img.channels();
	img.convertTo(img, CV_32FC3, 1.0 / 255.0);
	cout << "Image file \"" << argv[1] << "\" has been loaded." << endl;

	cv::Mat out, gauss;

	// ���͉摜�̑ΐ����Ƃ�
	clcnst::logarithm(img, out);

	// �K�E�V�A���E�t�B���^��������
	clcnst::gaussian(out, gauss, 1.0f, 5);

	// �����Z
	cv::subtract(out, gauss, out);

	// �I�t�Z�b�g
	cv::subtract(out, cv::Mat::ones(height, width, CV_32FC3), out);

	// �w�������
	clcnst::exponential(out, out);

	// ���K��
	clcnst::normalize(out, out, 0.0f, 1.0f);

	// ���ʂ̏o��
	cv::namedWindow("Input");
	cv::namedWindow("Output");
	cv::imshow("Input", img);
	cv::imshow("Output", out);
	cv::waitKey(0);
	cv::destroyAllWindows();

	out.convertTo(out, CV_8UC3, 255.0);
	cv::imwrite(argv[2], out);
}