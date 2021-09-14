//Data系のソースファイル

//ヘッダファイル読み込み########################################
#include "game.h"
#include "Data.h"
#include "shape.h"		//図形の処理

//グローバル変数################################################
CHARA_DATA enemy[ENEMY_MAX];	//敵データ

//関数##########################################################

/// <summary>
/// CSVファイルを読み込む（CHARA_DATA）
/// </summary>
/// <param name="path">CSVファイルのパス</param>
/// <param name="chara">キャラデータ構造体の先頭アドレス</param>
/// <param name="DataMax">CSVファイルの最大数</param>
/// <param name="IsHeader">CSVファイルの１行目がヘッダならTRUE</param>
/// <returns>TRUEは読み込めた/FALSEは失敗した</returns>
BOOL LoadCSVChara(const char* path, CHARA_DATA* chara, int DataMax, BOOL IsHeader = FALSE)
{
	FILE* fp;				//ファイルポインタ
	char GetChar = '\0';	//取得する文字
	int LoopCnt = 0;		//ループカウンタ
	errno_t FileErr = NULL;	//ファイル読み込みエラーを確認
	int IsEOF = NULL;		//ファイルの終わりかチェック

	FileErr = fopen_s(&fp, path, "r");	//ファイルを読み込みモード(r)で開く
	if (FileErr != NULL)
	{
		MessageBox(GetMainWindowHandle(), path, CSV_LOAD_ERR_TITLE, MB_OK);	//読み込みエラー
		return FALSE;
	}

	//ここから正常に読み込めたときの処理****************************************

	//CSVの1行目がヘッダのとき
	if (IsHeader == TRUE)
	{
		//１文字ずつ取り出し、改行をヒントに１行目を読み飛ばす
		while (GetChar != '\n') { GetChar = fgetc(fp); }
	}

	//実際にデータを格納する
	while (IsEOF != EOF)	//End Of File（ファイルの最後）ではないとき繰り返す
	{
		//CSVのデータの１行分を読み取る
		IsEOF = fscanf_s(
			fp,
			CSV_CHARA_FORMAT,	//データが「一行=レコード」で書かれているのがポイント！
			&(chara + LoopCnt)->No,
			&(chara + LoopCnt)->Name[0], STR_MAX,	//文字列を読み込むときは、次の引数で文字列最大数を指定する
			&(chara + LoopCnt)->HP,
			&(chara + LoopCnt)->ATK,
			&(chara + LoopCnt)->DEF,
			&(chara + LoopCnt)->MAX_HP,
			&(chara + LoopCnt)->MAX_ATK,
			&(chara + LoopCnt)->MAX_DEF
		);

		//次のデータへ
		LoopCnt++;
	}

	//データの数が違うとき
	if (LoopCnt - 1 != DataMax)
	{
		MessageBox(GetMainWindowHandle(), path, CSV_DATANUM_ERR_TITLE, MB_OK);	//読み込みエラー
		return FALSE;
	}

	fclose(fp);	//ファイルを閉じる

	return TRUE;
}
