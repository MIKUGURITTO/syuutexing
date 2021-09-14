#pragma once
//フォント処理のヘッダファイル

//ルートデレクトリ
//|-Font
//	|-Jiyucho.ttf(デフォルトフォント)

//ヘッダファイル読み込み############################################################################

//ゲームのメインヘッダファイル
#include "game.h"

//マクロ定義########################################################################################

//フォント系
#define FONT_JIYU_PATH	TEXT(".\\Font\\Jiyucho.ttf")	//フォントの場所
#define FONT_JIYU_NAME	TEXT("じゆうちょうフォント")	//フォント名前

#define FONT_CINDERELLA_PATH	TEXT(".\\Font\\Cinderella.otf")	//フォントの場所
#define FONT_CINDERELLA_NAME	TEXT("シンデレラ")	//フォント名前


#define FONT_MSGOTH_NAME	TEXT("MS ゴシック")			//フォント名（MS ゴシック）

#define DEF_FONT_NAME	FONT_JIYU_NAME				//どフォルトのフォント名
#define DEF_FONT_SIZE	20							//デフォルトのサイズ
#define DEF_FONT_THINCK	1							//デフォルトの太さ
#define DEF_FONT_TYPE	DX_FONTTYPE_ANTIALIASING	//デフォルトタイプ

//エラーメッセージ
#define FONT_INSTALL_ERR_TITLE	TEXT("フォントインストールエラー")
#define FONT_CREATE_ERR_TITLE	TEXT("フォント作成エラー")

//構造体############################################################################################

//フォント構造体
struct FONT
{
	int handle;								//フォントハンドル
	int Size;								//フォントサイズ
	int Thinck;								//フォントの太さ
	int Type = DX_FONTTYPE_ANTIALIASING;	//フォントのタイプ
	char Name[STR_MAX];						//フォントの名前
};

//外部のグローバル変数##############################################################################

//フォント関連
extern FONT fontdef;			//デフォルトフォント

extern FONT sampleFont1;		//サンプルのフォント（PCに入っていそうなフォント）
extern FONT sampleFont2;		//サンプルのフォント（フォントファイルから読み込んだフォント）

extern FONT CinderellaFont;		//シンデレラ

//プロトタイプ宣言#######################################################################################
extern BOOL FontAdd(VOID);														//フォントの読込
extern BOOL FontCreate(VOID);													//フォント作成
extern VOID SetFont(FONT* f, const char* name, int size, int thinck, int type);	//フォントに設定する
extern BOOL FontCreateHandle(FONT* f);											//フォントハンドルを作成
extern VOID SetDefaultFont(FONT f);												//フォントを全体に反映させる
extern VOID FontDelete(VOID);													//フォント削除
extern VOID FontRemove(VOID);													//フォントの削除