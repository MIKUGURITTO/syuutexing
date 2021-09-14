//Data系のヘッダファイル
#pragma once

//ヘッダファイル読み込み########################################

//ゲームのメインヘッダファイル
#include "game.h"

//マイクロ定義##################################################

//エラーメッセージ
#define CSV_LOAD_EAR_TITLE	TEXT("CSV読み込みエラー")

//キャラデータのパス
#define CSV_PATH_ENEMY		TEXT(".Data\\enemy.csv")	//ANSIの文字コードでファイル保存

//データの書式指定子
//キャラクタの書式指定子 (%[^,]は文字列）
#define CSV_CHARA_FORMAT	TEXT("%d,%[^,],%d,%d,%d,%d,%d,%d")

//敵データ
#define ENEMY_MAX	5

//キャラデータ構造体（あくまでサンプル）
struct CHARA_DATA
{
	int No;				//No
	char Name[STR_MAX];	//名前

	//実際に使う値
	int HP;				//HP
	int ATK;			//攻撃力
	int DEF;			//防御力

	//最大値
	int MAX_HP;			//HP
	int MAX_ATK;		//攻撃力
	int MAX_DEF;		//防御力
};

//外部のグローバル変数##########################################
extern CHARA_DATA enemy[ENEMY_MAX];		//敵データ

//プロトタイプ宣言##############################################
//※externは、外部に関数がありますよ！という目印をつけています。

extern BOOL LoadSVChara(const char* path, CHARA_DATA* chara, int DataMax, BOOL IsHeader);
