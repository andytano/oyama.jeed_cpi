/*---------------------------------------------------------
プロジェクト:	OpenCV USBカメラ画像UDP転送プロジェクト
モジュール：	親機クライアントmainモジュール
作成者:			Yamada Tarou
作成日時:		2020/10/1
機能：			USBカメラ画像のUDPでの受信
バージョン:		0.1
---------------------------------------------------------*/
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

#include <iostream>
#include <vector>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define CVUI_IMPLEMENTATION
#include "cvui.h"

using namespace std;

#include "usbcamcom.h"

#define FRAME_NAME	"Graph UDP Receive"

//-----------------------------------------------------------------------------
//　メイン関数
//-----------------------------------------------------------------------------
int main(int argc, char *argv[]) {
	int numrcv;		//受信バイト数

	int sock_of_con;					//送受信用ソケット
	struct sockaddr_in addr_of_con;		//送受信用ソケットの設定情報
	unsigned int i;

	sock_of_con = socket(AF_INET, SOCK_DGRAM, 0);	//UDPソケット作成

	addr_of_con.sin_family = AF_INET;		//IPv4
	addr_of_con.sin_port = htons(UDP_PORT);	//UDP通信使ポート番号
	addr_of_con.sin_addr.s_addr = INADDR_ANY;	//サーバのIPアドレスは任意

	//受信用バインド、指定したポートをこのプログラムに紐づける
	bind(sock_of_con, (struct sockaddr *)&addr_of_con, sizeof(addr_of_con));

	// cvui frame ------------------------------------------------
	//CV_8UC3はunsigned char３チャンネル、フレームサイズ
	cv::Mat frame = cv::Mat(cv::Size(GRP_IMG_WIDTH+FRM_MGN_SZ, GRP_IMG_HEIGHT+FRM_MGN_SZ),
									 CV_8UC3);

	// WINDOW_NAMEの名前でフレームを生成
	cvui::init(FRAME_NAME);

	// フレームの色を設定する　色はBGRの順番、値は0～255の範囲
	frame = cv::Scalar(150, 150, 50);

	//画像イメージ貼り付け用オブジェクト生成
	cv::Mat	image = cv::Mat(GRP_IMG_HEIGHT,GRP_IMG_WIDTH,CV_8UC3);

	static const int receiveSize = RCV_BUF_SZ;		//受信バッファサイズ設定
	static char buff[receiveSize];		//画像データ受信用バッファ

	vector<uchar> ibuff;		//動的配列を宣言

	while(1)	{
		//UDPで受信サイズ分受信する
		numrcv = recv(sock_of_con, buff, receiveSize, 0);

		for (i = 0; i < sizeof(buff); i++) {
			//push_backで１つずつ要素buff[i]をibuffに追加していく
			ibuff.push_back((uchar)buff[i]);
		}
		if(numrcv == -1) break;

		if (cvui::button(frame, BTN_X_POS, BTN_Y_POS, "QUIT")) {
			break;
		}

		//動的配列ibuffをカラーのMat（メモリ上のバッファ）として、imageに読み込む
		image = cv::imdecode(cv::Mat(ibuff), cv::IMREAD_COLOR);

		//imageをフレームに表示する
		cvui::button(frame, GRP_DSP_OFST_X, GRP_DSP_OFST_Y, image, image, image);

		ibuff.clear();		//動的配列のクリア

		// フレーム内で発生したイベントを処理する関数
		cvui::update();

		// フレームに画像を表示する
		cv::imshow(FRAME_NAME, frame);

		// キー入力待ち10ms、qキーが押されたら終了
		if (cv::waitKey(10) == 'q') {
			break;
		}
	}
	close(sock_of_con);		//UDPソケットクローズ

	printf("Client Recieve Graphic data...end\n");

	return 0;
}

