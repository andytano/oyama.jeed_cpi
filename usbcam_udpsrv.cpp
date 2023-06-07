/*---------------------------------------------------------
プロジェクト:	OpenCV USBカメラ画像UDP転送プロジェクト 
モジュール：	子機サーバmainモジュール			
作成者:			Yamada Tarou
作成日時:		2020/10/1
機能：			USBカメラ撮影、画像のUDPでの転送
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

using namespace std;

#include "usbcamcom.h"

//-----------------------------------------------------------------------------
//　メイン関数
//-----------------------------------------------------------------------------
int main(int argc, char *argv[]){

    int sock_of_con;					//送受信用ソケット
    struct sockaddr_in addr_of_con;

	if (argc != 2) {
		printf ("No IP address error.\n");
		return -1;
	}

    sock_of_con = socket(AF_INET, SOCK_DGRAM, 0);	//UDPソケット作成

    addr_of_con.sin_family = AF_INET;			//IPv4
    addr_of_con.sin_port = htons(UDP_PORT); 	 //UDP通信用ポート番号
    addr_of_con.sin_addr.s_addr = inet_addr(argv[1]); //送信先IPアドレス

    //カメラの設定
    cv::VideoCapture cap(0);

	//キャプチャーする画面の幅、高さを設定
    cap.set(cv::CAP_PROP_FRAME_WIDTH, GRP_IMG_WIDTH);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, GRP_IMG_HEIGHT);

    if(!cap.isOpened()){
        cout << "Camera not Found!" << endl;
        return -1;
    }

    cv::Mat frame;
    cv::Mat jpgimg;
    static const int sendSize = RCV_BUF_SZ;		//送信バッファサイズ設定
								//受信側にとっては、受信バッファサイズ
    char buff[sendSize];		//画像データ送信用バッファ

    vector<unsigned char> ibuff;		//動的配列を宣言

    vector<int> param = vector<int>(2);
	//送信するJPEG画像ファイルの品質を設定する
    param[0] = cv::IMWRITE_JPEG_QUALITY;
    param[1] = JPEG_QUALITY_VALUE;

	printf("Server USB camera start!\n");

    while(cv::waitKey(1) == -1){
        cap >> frame;
		//画像イメージをparamの品質でエンコードしてibuffに格納する
        cv::imencode(".jpg", frame, ibuff, param);
        if(ibuff.size() < sendSize ){
            for(int i=0; i<ibuff.size(); i++) buff[i] = ibuff[i];

			//画像バッファを送信する
            sendto(sock_of_con, buff, sendSize, 0, 
				(struct sockaddr *)&addr_of_con, sizeof(addr_of_con));

			//ibuffをMatにしてデコードする
            jpgimg = cv::imdecode(cv::Mat(ibuff), cv::IMREAD_COLOR);
         }
        ibuff.clear();	//画像を格納するためのibuffをクリアする
    }
    close(sock_of_con);		//UDPソケットクローズ

	printf("Server send USB camera data...end\n");
}
