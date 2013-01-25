#include <iostream>
using namespace std;

#include <opencv2\opencv.hpp>

#include "../clcnst/clcnst.h"

int main(int argc, char** argv) {
	if(argc < 3) {
		cout << "usage: BlakeAlgorithm.exe [input image] [output image] [threshold]" << endl;
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

	float theta = argc > 3 ? (float)atof(argv[3]) : 0.05f;
	printf("threshold = %f\n", theta);

	cv::Mat out, laplace;

	// ���͂̑ΐ����Ƃ�
	clcnst::logarithm(img, out);

	// ���v���V�A���t�B���^
	clcnst::laplacian(out, laplace);

	// 臒l����
	clcnst::threshold(laplace, laplace, theta);

	// 臒l���͂��񂾃��v���V�A���E�t�B���^
	laplace = cv::Mat::zeros(height, width, CV_32FC3);
	for(int c=0; c<channel; c++) {
		// ���z��̌v�Z��臒l����
		cv::Mat grad = cv::Mat::zeros(height, width, CV_32FC2);
		for(int y=0; y<height-1; y++) {
			for(int x=0; x<width-1; x++) {
				float dx = out.at<float>(y, (x+1)*channel+c) - out.at<float>(y, x*channel+c);
				float dy = out.at<float>(y+1, x*channel+c) - out.at<float>(y, x*channel+c);
				if(fabs(dx) > theta) {
					grad.at<float>(y, x*2+0) = dx;
				}

				if(fabs(dy) > theta) {
					grad.at<float>(y, x*2+1) = dy;
				}
			}
		}

		// ���z���gradient���v�Z
		for(int y=1; y<height; y++) {
			for(int x=1; x<width; x++) {
				float ddx = grad.at<float>(y, x*2+0) - grad.at<float>(y, (x-1)*2+0);
				float ddy = grad.at<float>(y, x*2+1) - grad.at<float>(y-1, x*2+1);
				laplace.at<float>(y, x*channel+c) = ddx + ddy;
			}
		}
	}

	// �K�E�X�E�U�C�f���@
	clcnst::gauss_seidel(out, laplace, 20);

	// �l�̐��K��
	clcnst::normalize(out, out);

	// �l�̎w�����Ƃ�
	clcnst::exponential(out, out);

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