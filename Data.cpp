//Data系のソースファイル

//ヘッダファイル読み込み########################################
#include "game.h"
#include "Data.h"
#include "shape.h"		//図形の処理

//グローバル変数################################################
CHARA_DATA enemy[ENEMY_MAX];	//敵データ

SCORE_DATA score_data;			//スコアデータ
SCORE_DATA score_dataInit = { SCORE_DATA_PATH,"NAME1",-1,"NAME2",-1, "NAME3",-1 };
//スコアデータ(初期化用)

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
		while (GetChar != '\n')
		{
			GetChar = fgetc(fp);
		}
	}

	//実際にデータを格納する
	while (IsEOF != EOF)	//End Of File（ファイルの最後）ではないとき繰り返す
	{
		//CSVのデータの１行分を読み取る
		IsEOF = fscanf_s(
			fp,
			CSV_CHARA_FORMAT,	//データが「一行=レコード」で書かれているのがポイント！
			&(chara + LoopCnt)->No,
			&(chara + LoopCnt)->Name[0], STR_MAX,
			//文字列を読み込むときは、次の引数で文字列最大数を指定する
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
		MessageBox(GetMainWindowHandle(), path, CSV_DATANUM_ERR_TITLE, MB_OK);
		//読み込みエラー
		return FALSE;
	}

	fclose(fp);	//ファイルを閉じる

	return TRUE;
}

/// <summary>
/// スコアデータを読み込む（SCORE_DATA）
/// </summary>
/// <param name="path">スコアデータのパス</param>
/// <param name="chara">スコアデータ構造体の先頭アドレス</param>
/// <param name="IsHeader">CSVファイルの１行目がヘッダならTRUE</param>
/// <returns>TRUEは読み込めた/FALSEは失敗した</returns>
BOOL LoadScoreData(const char* path, SCORE_DATA* score, BOOL IsHeader = FALSE)
{
	FILE* fp;				//ファイルポインタ
	char GetChar = '\0';	//取得する文字
	errno_t FileErr = NULL;	//ファイル読み込みエラーを確認
	int IsEOF = NULL;		//ファイルの終わりかチェック

	//パスのコピー
	SetScoreDataPath(path);

	FileErr = fopen_s(&fp, path, "r");	//ファイルを読み込みモード(r)で開く
	if (FileErr != NULL)
	{
		//スコアデータ作成
		if (CreateScoreData() == FALSE) { return FALSE; }

		//ここまでの処理でスコアデータは作られた！

		FileErr = fopen_s(&fp, path, "r");	//ファイルを読み込みモード（r）で開く
		if (FileErr != NULL)
		{
			MessageBox(GetMainWindowHandle(), path, SCORE_LOAD_ERR_TITLE, MB_OK);
		//読み込みエラー
			return FALSE;
		}
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
			SCORE_DATA_FORMAT,	//データが「一行=レコード」で書かれているのがポイント！
			&(score)->Name1, STR_MAX,	//文字列を読み込むときは、次の引数で文字列最大数を指定する
			&(score)->Score1,
			&(score)->Name2, STR_MAX,	//文字列を読み込むときは、次の引数で文字列最大数を指定する
			&(score)->Score2,
			&(score)->Name3, STR_MAX,	//文字列を読み込むときは、次の引数で文字列最大数を指定する
			&(score)->Score3
		);
	}

	fclose(fp);	//ファイルを閉じる

	return TRUE;
}

/// <summary>
/// スコアデータ作成
/// </summary>
/// <returns></returns>
BOOL CreateScoreData(VOID)
{
	//なかったらスコアデータを作る
	FILE* creFp;
	errno_t creFileErr = NULL;	//ファイル読み込みエラーを確認
	creFileErr = fopen_s(&creFp, GetScoreDataPath(), "w");
	//ファイルを書き込みモード（ｗ）で開く（作成する）

	if (creFileErr != NULL)
	{
		MessageBox(GetMainWindowHandle(), GetScoreDataPath(), SCORE_CREATE_ERR_TITLE, MB_OK);
		return FALSE;
	}

	//.ヘッダを書き込み
	fprintf_s(creFp,
		"Name1,Score1,Name2,Score2,Name3,Score3\n"
	);

	//空のスコアデータを書き込み
	fprintf_s(creFp,
		"%s,%d,%s,%d,%s,%d",
		score_dataInit.Name1, score_dataInit.Score1,
		score_dataInit.Name2, score_dataInit.Score2,
		score_dataInit.Name3, score_dataInit.Score3
	);

	fclose(creFp);	//スコアデータを閉じる

	MessageBox(GetMainWindowHandle(), GetScoreDataPath(), SCORE_LOAD_ERR_TITLE, MB_OK);
	return FALSE;
}

/// <summary>
/// スコアデータ保存
/// </summary>
/// <returns></returns>
BOOL SaveScoreData(VOID)
{
	//なかったらスコアデータを作る
	FILE* creFp;
	errno_t creFileErr = NULL;	//ファイル読み込みエラーを確認
	creFileErr = fopen_s(&creFp, GetScoreDataPath(), "w");	//ファイルを書き込みモード（ｗ）で開く（作成する）

	if (creFileErr != NULL)
	{
		MessageBox(GetMainWindowHandle(), GetScoreDataPath(), SCORE_CREATE_ERR_TITLE, MB_OK);
		return FALSE;
	}

	//.ヘッダを書き込み
	fprintf_s(creFp,
		"Name1,Score1,Name2,Score2,Name3,Score3\n"
	);

	//スコアデータを書き込み
	fprintf_s(creFp,
		"%s,%d,%s,%d,%s,%d",
		score_dataInit.Name1, score_dataInit.Score1,
		score_dataInit.Name2, score_dataInit.Score2,
		score_dataInit.Name3, score_dataInit.Score3
	);

	fclose(creFp);	//スコアデータを閉じる

	MessageBox(GetMainWindowHandle(), GetScoreDataPath(), SCORE_SAVE_TITLE, MB_OK);
	return FALSE;
}

/// <summary>
/// スコアデータの削除
/// </summary>
/// <param name="path">スコアデータのパス</param>
BOOL DeleteScoreData(VOID)
{
	//ファイルを削除
	if (remove(GetScoreDataPath()) != 0)
	{
		MessageBox(GetMainWindowHandle(), GetScoreDataPath(), SCORE_DELETE_ERR_TITLE, MB_OK);
	}
	else
	{
		MessageBox(GetMainWindowHandle(), GetScoreDataPath(), SCORE_DELETE_TITLE, MB_OK);
	}

	//スコアデータを作成
	return CreateScoreData();
}

/// <summary>
/// スコアデータのパスを返す
/// </summary>
/// <returns></returns>
const char* GetScoreDataPath(VOID)
{
	return score_data.path;
}

/// <summary>
/// スコアデータのパスを設定する
/// </summary>
/// <returns></returns>
VOID SetScoreDataPath(const char* path)
{
	strcpyDx(score_data.path, path);
	return;
}

/// <summary>
/// CSVファイルを読み込む（MAP_DATA）
/// </summary>
/// <param name="ImgPath">マップチップの画像ファイルのパス</pram>
/// <param name="ShitaPath">下ファイルのパス</pram>
/// <param name="Nacarat">中ファイルのパス</pram>
/// <param name="NakaAtariPath">中当たりファイルのパス</pram>
/// <param name="UePath">上ファイルのパス</pram>
/// <param name="map">マップデータ構造体の先頭アドレス</pram>
/// <param name="divYoko">横の分割数</pram>
/// <param name="divTate">縦の分割数</pram>
/// <returns>TRUEは読み込めた/FALSEは失敗した</returns>
BOOL LoadCSVMap(const char* ImgPath, const char* ShitaPath, const char* NakaPath, const char* NakaAtariPath, const char* UePath, MAP_DATA* map, int divYoko, int divTate)
{
	//マップチップを読み込み
	{
		int IsLoad = -1;		//画像が読み込みたか？

		//一時的に画像のハンドルを用意する
		int TempHandle = LoadGraph(ImgPath);

		//読み込みエラー
		if (TempHandle == -1)
		{
			MessageBox(
				GetMainWindowHandle(),	//ウィンドウハンドル
				ImgPath,					//本文
				"画像読み込みエラー",	//タイトル
				MB_OK						//ボタン
			);

			return FALSE;	//読み込み失敗
		}

		//画像の幅と高さを取得
		int Width = -1;			//幅
		int Height = -1;		//高さ
		GetGraphSize(TempHandle, &Width, &Height);

		//分割して読み込み
		IsLoad = LoadDivGraph(
			ImgPath,							//画像のパス
			divYoko * divTate,					//分割総数
			divYoko, divTate,					//横の分割、縦の分割
			Width / divYoko, Height / divTate,	//画像１つ分の幅、高さ
			map->handle							//連続で管理する配列の先頭アドレス
		);

		//分割エラー
		if (IsLoad == -1)
		{
			MessageBox(
				GetMainWindowHandle(),	//ウィンドウハンドル
				ImgPath,				//本文
				"画像分割エラー",	//タイトル
				MB_OK					//ボタン
			);

			return FALSE;	//読み込み失敗
		}
		//情報を設定
		map->DivMax = divYoko * divTate;
		GetGraphSize(map->handle[0], &map->width, &map->height);

		//画像を描画する
		map->IsDraw = TRUE;

		//一時的に読み込んだハンドルを解散
		DeleteGraph(TempHandle);
	}

	FILE* fp;				//ファイルポインタ
	char GetChar = '\0';	//取得する文字
	int LoopCnt = 0;		//ループカウンタ
	errno_t FileErr = NULL;//ファイル読み込みエラーを確認
	int IsEOF = NULL;		//ファイルの終わりかチェック

	//下CSVを読みとり
	{
		FileErr = fopen_s(&fp, ShitaPath, "r");	//ファイルを読み込みモード（ｒ）で開く
		if (FileErr != NULL)
		{
			MessageBox(GetMainWindowHandle(), ShitaPath, CSV_LOAD_ERR_TITLE, MB_OK);	//読み込みエラー
		}

		//ここから正常に読み込めたときの処理******************************************

		//実際にデータを格納する
		IsEOF = NULL; LoopCnt = 0;
		while (IsEOF != EOF)	//End Of File (ファイルの最後)ではないとき繰り返す
		{
			//ファイルから数値を一つ読み込み（%d,）、配列に格納する
			//[LoopCnt / MAP_YOKO_MAX] [LoopCnt % MAP_YOKO_MAX]は、
			//例えば[3][3]の配列のとき、LoopCntが5であれば、
			//[5 / 3 = 1][5 % 3 = 2]となり、１次元配列を２次元配列として変換できる
			IsEOF = fscanf_s(
				fp,
				CSV_MAP_FORMAT, //データが「XX,」で書かれているのがポイント！
				&map->CSV_shita[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]
			);

			//次のデータへ
			LoopCnt++;
		}

		fclose(fp);	//ファイルを閉じる
	}

	//中CSVを読み取り
	{
		FileErr = fopen_s(&fp, NakaPath, "r");	//ファイルを読み込みモード（ｒ）で開く
		if (FileErr != NULL)
		{
			MessageBox(GetMainWindowHandle(), NakaPath, CSV_LOAD_ERR_TITLE, MB_OK);	//読み込みエラー
		}

		//ここから正常に読み込めたときの処理******************************************

		//実際にデータを格納する
		IsEOF = NULL; LoopCnt = 0;
		while (IsEOF != EOF)	//End Of File (ファイルの最後)ではないとき繰り返す
		{
			//ファイルから数値を一つ読み込み（%d,）、配列に格納する
			//[LoopCnt / MAP_YOKO_MAX] [LoopCnt % MAP_YOKO_MAX]は、
			//例えば[3][3]の配列のとき、LoopCntが5であれば、
			//[5 / 3 = 1][5 % 3 = 2]となり、１次元配列を２次元配列として変換できる
			IsEOF = fscanf_s(
				fp,
				CSV_MAP_FORMAT, //データが「XX,」で書かれているのがポイント！
				&map->CSV_naka[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]
			);

			//次のデータへ
			LoopCnt++;
		}

		fclose(fp);	//ファイルを閉じる
	}

	//中当たりCSVを読み取り
	{
		FileErr = fopen_s(&fp, NakaAtariPath, "r");	//ファイルを読み込みモード（ｒ）で開く
		if (FileErr != NULL)
		{
			MessageBox(GetMainWindowHandle(), NakaAtariPath, CSV_LOAD_ERR_TITLE, MB_OK);	//読み込みエラー
		}

		//ここから正常に読み込めたときの処理******************************************

		//実際にデータを格納する
		IsEOF = NULL; LoopCnt = 0;
		while (IsEOF != EOF)	//End Of File (ファイルの最後)ではないとき繰り返す
		{
			//ファイルから数値を一つ読み込み（%d,）、配列に格納する
			//[LoopCnt / MAP_YOKO_MAX] [LoopCnt % MAP_YOKO_MAX]は、
			//例えば[3][3]の配列のとき、LoopCntが5であれば、
			//[5 / 3 = 1][5 % 3 = 2]となり、１次元配列を２次元配列として変換できる
			IsEOF = fscanf_s(
				fp,
				CSV_MAP_FORMAT, //データが「XX,」で書かれているのがポイント！
				&map->CSV_naka_atari[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]
			);

			//次のデータへ
			LoopCnt++;
		}

		fclose(fp);	//ファイルを閉じる
	}

	//上CSVを読み取り
	{
		FileErr = fopen_s(&fp, UePath, "r");	//ファイルを読み込みモード（ｒ）で開く
		if (FileErr != NULL)
		{
			MessageBox(GetMainWindowHandle(), UePath, CSV_LOAD_ERR_TITLE, MB_OK);	//読み込みエラー
		}

		//ここから正常に読み込めたときの処理******************************************

		//実際にデータを格納する
		IsEOF = NULL; LoopCnt = 0;
		while (IsEOF != EOF)	//End Of File (ファイルの最後)ではないとき繰り返す
		{
			//ファイルから数値を一つ読み込み（%d,）、配列に格納する
			//[LoopCnt / MAP_YOKO_MAX] [LoopCnt % MAP_YOKO_MAX]は、
			//例えば[3][3]の配列のとき、LoopCntが5であれば、
			//[5 / 3 = 1][5 % 3 = 2]となり、１次元配列を２次元配列として変換できる
			IsEOF = fscanf_s(
				fp,
				CSV_MAP_FORMAT, //データが「XX,」で書かれているのがポイント！
				&map->CSV_ue[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]
			);

			//次のデータへ
			LoopCnt++;
		}

		fclose(fp);	//ファイルを閉じる
	}

	//マップの当たり判定を作成
	{
		for (int tate = 0; tate < MAP1_TATE_MAX; tate++)
		{
			for (int yoko = 0; yoko < MAP1_YOKO_MAX; yoko++)
			{
				//通れないIDであれば
				if (map->CSV_naka_atari[tate][yoko] == MAP1_STOP_ID)
				{
					//当たり判定を作成
					map->coll[tate][yoko].left = (yoko + 0) * map->width + 1;
					map->coll[tate][yoko].right = (yoko + 1) * map->width - 1;
					map->coll[tate][yoko].top = (tate + 0) * map->height + 1;
					map->coll[tate][yoko].bottom = (tate + 1) * map->height - 1;
				}
				else
				{
					map->coll[tate][yoko].left = 0;
					map->coll[tate][yoko].right = 0;
					map->coll[tate][yoko].top = 0;
					map->coll[tate][yoko].bottom = 0;
				}

				//マップの場所を設定
				map->x[tate][yoko] = (yoko + 0) * map->width;
				map->y[tate][yoko] = (tate + 0) * map->height;
			}
		}
	}

	return TRUE;
}
///<summary>
///マップ当たり判定
///</summary>
///<param name="rect"></param>
///<param name="map"></param>
///<returns></returns>
BOOL CollMap(RECT rect, MAP_DATA map)
{
	//当たり判定チェック
	for (int tate = 0; tate < MAP1_TATE_MAX; tate++)
	{
		for (int yoko = 0; yoko < MAP1_YOKO_MAX; yoko++)
		{
			if (CheckCollRectToRect(rect, map.coll[tate][yoko]) == TRUE)
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}
	
///<summary>
///マップ描画
///</summary>
///<param name="map">マップ構造体</param>
///<returns></returns>
///<returns></returns>
VOID DrawMap(MAP_DATA map)
{
	//画像が描画できるときは
	if (map.IsDraw == TRUE)
	{


		for (int tate = 0; tate < MAP1_TATE_MAX; tate++)
		{
			for (int yoko = 0; yoko < MAP1_YOKO_MAX; yoko++)
			{
				DrawGraph(map.x[tate][yoko], map.y[tate][yoko], map.handle[map.CSV_shita[tate][yoko]], TRUE);	//下レイヤ
			}
		}

		for (int tate = 0; tate < MAP1_TATE_MAX; tate++)
		{
			for (int yoko = 0; yoko < MAP1_YOKO_MAX; yoko++)
			{
				DrawGraph(map.x[tate][yoko], map.y[tate][yoko], map.handle[map.CSV_naka[tate][yoko]], TRUE);	//中レイヤ
			}
		}

		//ここにプレイヤーの画像を描画する
		DrawDivImageChara(&samplePlayerImg);//サンプル分割画像

		for (int tate = 0; tate < MAP1_TATE_MAX; tate++)
		{
			for (int yoko = 0; yoko < MAP1_YOKO_MAX; yoko++)
			{
				DrawGraph(map.x[tate][yoko], map.y[tate][yoko], map.handle[map.CSV_ue[tate][yoko]], TRUE);	//上レイヤ
			}
		}
	}

	//デバックモードのときは
	if (GAME_DEBUG == TRUE)
	{
		for (int tate = 0; tate < MAP1_TATE_MAX; tate++)
		{
			for (int yoko = 0; yoko < MAP1_YOKO_MAX; yoko++)
			{
				//当たり判定を描画
				DrawRect(map.coll[tate][yoko], GetColor(255, 255, 255), FALSE);
			}
		}
	}
	return;
}