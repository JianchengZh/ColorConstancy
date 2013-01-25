#include <iostream>
using namespace std;

#include <opencv2\opencv.hpp>

#include "../clcnst/clcnst.h"

int main(int argc, char** argv) {
	if(argc < 3) {
		cout << "usage: HornAlgorithm.exe [input image] [output image] [threshold]" << endl;
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

	float theta = (argc > 3) ? atof(argv[3]) : 0.05f;
	printf("threshold = %f\n", theta);

	cv::Mat out, laplace;

	// �P�x�̑ΐ������
	clcnst::logarithm(img, out);
	
	// ���v���V�A���t�B���^��������
	clcnst::laplacian(out, laplace);

	// 臒l����
	clcnst::threshold(laplace, laplace, theta);
		
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

	out.convertTo(out, CV_8UC3, 255.0);
	cv::imwrite(argv[2], out);
}
