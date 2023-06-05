/*---------------------------------------------------------
プロジェクト:	スレッドでのOpenCV画像ファイル表示 
モジュール：	OpenCV画像ファイル表示モジュール			
作成者:			Yamada Tarou
作成日時:		2020/9/1
機能：			OpenCVでの画像ファイル表示
バージョン:		0.1
---------------------------------------------------------*/
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

cv::Mat mat;

extern "C" int init_window(char *fname)
{
	//cv2.WINDOW_NORMALにすると、ウィンドウの大きさを自分で調整できる
	cv::namedWindow(fname, cv::WINDOW_NORMAL);

	return 0;
}

extern "C" int disp_graph(int idx, char *fname)
{
	switch(idx)	{
		case 0:
			mat = cv::imread("penguin.png", cv::IMREAD_COLOR);
			break;
		case 1:
			mat = cv::imread("alpaca.png", cv::IMREAD_COLOR);
			break;
		case 2:
			mat = cv::imread("wallaby.png", cv::IMREAD_COLOR);
			break;
		default:
			mat = cv::imread("penguin.png", cv::IMREAD_COLOR);
			break;
	}

	cv::imshow(fname, mat);

	cv::waitKey(100);

	return 0;
}

extern "C" int close_window()
{
	cv::destroyAllWindows();

	return 0;
}
