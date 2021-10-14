//Data系のヘッダファイル
#pragma once

//ヘッダファイル読み込み########################################

//ゲームのメインヘッダファイル
#include "game.h"

//マイクロ定義##################################################

//エラーメッセージ
#define CSV_LOAD_ERR_TITLE		TEXT("CSV読み込みエラー")
#define CSV_DATANUM_ERR_TITLE	TEXT("CSV読み込みエラー")

//キャラデータのパス
#define CSV_PATH_ENEMY		TEXT(".\\Data\\enemy.csv")	//ANSIの文字コードでファイル保存

//データの書式指定子
//キャラクタの書式指定子 (%[^,]は文字列）
#define CSV_CHARA_FORMAT	TEXT("%d,%[^,],%d,%d,%d,%d,%d,%d")

//敵データ
#define ENEMY_MAX	5

//エラーメッセージ
#define SCORE_CREATE_ERR_TITLE		TEXT("スコアデータ作成エラー")
#define SCORE_LOAD_ERR_TITLE		TEXT("スコアデータ読込エラー")
#define SCORE_CREATE_TITLE			TEXT("スコアデータ作成OK!!")
#define SCORE_SAVE_TITLE			TEXT("スコアデータ保存OK!!")
#define SCORE_DELETE_TITLE			TEXT("スコアデータ削除OK!!")
#define SCORE_DELETE_ERR_TITLE		TEXT("スコアデータ削除エラー")

//スコアデータパス（あえて拡張子を変なのにすることで、スコアデータと思われにくい）
#define SCORE_DATA_PATH				TEXT(".\\Data\\score.a")	//ANSIの文字コードでファイル保存

//データ書式指定子
#define SCORE_DATA_FORMAT			TEXT("%[^,],%d,%[^,],%d,%[^,],%d")	//スコアデータの書式指定子(%[^,]は文字列)

//マップチップの画像のファイルパス
#define IMG_PATH_MAP1				TEXT(".\\Map\\mapchip.png")

//マップデータのファイルパス
#define CSV_PATH_MAP1_SHITA				TEXT(".\\Map\\map_shita.csv")			//下レイヤー
#define CSV_PATH_MAP1_NAKA				TEXT(".\\Map\\map_naka.csv")			//中レイヤー
#define CSV_PATH_MAP1_NAKA_ATARI		TEXT(".\\Map\\map_naka_atari.csv")		//中（当たり判定）レイヤー
#define CSV_PATH_MAP1_UE				TEXT(".\\Map\\map_ue.csv")				//上レイヤー

//マップの初期指定子
#define CSV_MAP_FORMAT		TEXT("%d,")

#define MAP1_YOKO_MAX				20	//マップの横サイズ
#define MAP1_TATE_MAX				20	//マップの縦サイズ

#define MAP1_YOKO_DIV				60	//マップの横分割数
#define MAP1_TATE_DIV				60	//マップの縦分割数

#define MAP1_MOVE_ID				56	//通れるマップID
#define MAP1_STOP_ID				57	//通れないマップID
#define MAP1_NOME_ID				58	//何もないマップID

//キャラデータ構造体（あくまでサンプル）
struct CHARA_DATA
{
	int No;				//No
	char Name[STR_MAX];	//名前

	//実際に使う値
	int HP;					//HP
	int ATK;				//攻撃力
	int DEF;				//防御力

	//最大値
	int MAX_HP;				//HP
	int MAX_ATK;			//攻撃力
	int MAX_DEF;			//防御力
};

//スコアデータ構造体（あくまでサンプル）
struct SCORE_DATA
{
	char path[PATH_MAX];	//パス
	char Name1[STR_MAX];	//名前１
	int Score1;				//スコア１
	char Name2[STR_MAX];	//名前２
	int Score2;				//スコア２
	char Name3[STR_MAX];	//名前３
	int Score3;				//スコア３
};

//マップデータ構造体（あくまでサンプル）
struct MAP_DATA
{
	//CSVデータ（分割画像のIDが入る）
	int CSV_shita[MAP1_TATE_MAX][MAP1_YOKO_MAX];
	int CSV_naka[MAP1_TATE_MAX][MAP1_YOKO_MAX];
	int CSV_naka_atari[MAP1_TATE_MAX][MAP1_YOKO_MAX];
	int CSV_ue[MAP1_TATE_MAX][MAP1_YOKO_MAX];

	//マップのハンドル
	int handle[MAP1_TATE_DIV * MAP1_YOKO_DIV];

	//マップの場所
	int x[MAP1_TATE_MAX][MAP1_YOKO_MAX];
	int y[MAP1_TATE_MAX][MAP1_YOKO_MAX];

	int width;
	int height;

	int DivMax;

	BOOL IsDraw = FALSE;	//描画するか？

	//当たり判定
		RECT coll[MAP1_TATE_MAX][MAP1_YOKO_MAX];
};

//外部のグローバル変数##########################################
extern CHARA_DATA enemy[ENEMY_MAX];		//敵データ
extern SCORE_DATA score_data;			//スコアデータ
extern SCORE_DATA score_dataInit;		//スコアデータ(初期化用)

//プロトタイプ宣言##############################################
//※externは、外部に関数がありますよ！という目印をつけています。

extern BOOL LoadCSVChara(const char* path, CHARA_DATA* chara, int DataMax, BOOL IsHeader);
extern BOOL LoadScoreData(const char* path, SCORE_DATA* score, BOOL IsHeader);

extern BOOL LoadCSVMap(
	const char* ImgPath,
	const char* ShitaPath,
	const char* NakaPath,
	const char* NakaAtariPath,
	const char* UePath,
	MAP_DATA* map,
	int divYoko,
	int divTate);

extern BOOL CreateScoreData(VOID);
extern BOOL SaveScoreData(VOID);
extern BOOL DeleteScoreData(VOID);
extern const char* GetScoreDataPath(VOID);
extern VOID SetScoreDataPath(const char* path);

extern BOOL CollMap(RECT rect, MAP_DATA map);
extern VOID DrawMap(MAP_DATA map);