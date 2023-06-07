/*---------------------------------------------------------
プロジェクト:	サーバ←→クライアントUDP通信プロジェクト 
モジュール：	通信用共通用ヘッダ				
作成者:			Yamada Tarou
作成日時:		2020/9/9
機能：			TCP通信用共通ヘッダ
バージョン:		0.1　ポート、送信受信文字最大数
---------------------------------------------------------*/

#define UDP_PORT 10002		//デフォルトのUDPポート番号

#define GRP_DSP_OFST_X	50		//画像表示X方向オフセット
#define GRP_DSP_OFST_Y	50		//画像表示Y方向オフセット

#define	GRP_IMG_WIDTH	640		//画像イメージ幅ピクセル数
#define	GRP_IMG_HEIGHT	480		//画像イメージ高さピクセル数

#define FRM_MGN_SZ		150		//フレーム余裕サイズ

#define RCV_BUF_SZ	65500	//UDP受信バッファサイズ

#define JPEG_QUALITY_VALUE	85	//jpeg画像品質

#define BTN_X_POS	50							//QUIT Button X pos
#define BTN_Y_POS	(GRP_IMG_HEIGHT + 50)		//QUIT Button X pos

