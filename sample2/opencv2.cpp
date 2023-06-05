/*---------------------------------------------------------
プロジェクト:	スレッドでのOpenCV画像ファイル表示 
モジュール：	OpenCV画像ファイル表示モジュール			
作成者:			Yamada Tarou
作成日時:		2020/8/19
機能：			OpenCVでの画像ファイル表示
バージョン:		0.1
---------------------------------------------------------*/
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

cv::Mat mat;

extern "C" int dsp_win()
{
	mat = cv::imread("penguin.png", cv::IMREAD_COLOR);
	cv::namedWindow("sample", cv::WINDOW_AUTOSIZE);
	cv::imshow("sample", mat);

	/*100msだけキー入力待ち、かつWindow表示メッセージ処理のため*/
	cv::waitKey(200);

	return 0;
}

extern "C" int cls_win()	{
	cv::destroyAllWindows();	//全てのWindowを閉じる

	return 0;
}
