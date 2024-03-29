//ヘッダファイル読み込み
#include "game.h"		//ゲーム全体のヘッダファイル
#include "keyboard.h"	//キーボードの処理
#include "FPS.h"		//FPSの処理
#include "mouse.h"		//マウスの処理
#include "shape.h"		//図形の処理
#include "Font.h"		//フォントの処理
#include "Data.h"		//データの処理

//独自のマクロ定義

//独自の列挙型の定義
enum MUKI
{
	muki_none,
	muki_ue,
	muki_hidari,
	muki_shita,
	muki_migi
};//向きのサンプル

//独自の構造体の定義


//★★★ゲーム共通のグローバル変数★★★
//シーンを管理する変数
GAME_SCENE GameScene;		//現在のゲームのシーン
GAME_SCENE OldGameScene;	//前回のゲームのシーン
GAME_SCENE NextGameScene;	//次のゲームのシーン

//画面の切り替え
BOOL IsFadeOut = FALSE;		//フェードアウト
BOOL IsFadeIn = FALSE;		//フェードイン

int fadeTimeMill = 2000;					//切り替えミリ秒
int fadeTimeMax = fadeTimeMill / 1000 * 60;	//ミリ秒をフレーム秒に変換

//フェードアウト
int fadeOutCntInit = 0;				//初期値
int fadeOutCnt = fadeOutCntInit;	//フェードアウトのカウンタ
int fadeOutCntMax = fadeTimeMax;	//フェードアウトのカウンタMAX

//フェードイン
int fadeInCntInit = fadeTimeMax;	//初期値
int fadeInCnt = fadeInCntInit;		//フェードアウトのカウンタ
int fadeInCntMax = fadeTimeMax;		//フェードアウトのカウンタMAX
//★★★ゲーム共通のグローバル変数★★★


//独自のグローバル変数


//★★★ゲーム共通のプロトタイプ宣言★★★
BOOL GameLoad(VOID);	//データを読込
VOID GameInit(VOID);	//データの初期化
VOID GameDelete(VOID);	//データを削除

VOID Title(VOID);		//タイトル画面
VOID TitleProc(VOID);	//タイトル画面(処理)
VOID TitleDraw(VOID);	//タイトル画面(描画)

VOID Play(VOID);		//プレイ画面
VOID PlayProc(VOID);	//プレイ画面(処理)
VOID PlayDraw(VOID);	//プレイ画面(描画)

VOID End(VOID);			//エンド画面
VOID EndProc(VOID);		//エンド画面(処理)
VOID EndDraw(VOID);		//エンド画面(描画)

VOID Change(VOID);		//切り替え画面
VOID ChangeProc(VOID);	//切り替え画面(処理)
VOID ChangeDraw(VOID);	//切り替え画面(描画)

VOID ChangeScene(GAME_SCENE scene);		//シーン切り替え
BOOL OnCollRect(RECT a, RECT b);		//矩形と矩形の当たり判定
VOID CollUpdateImage(IMAGE* img);		//画像の当たり判定の更新
VOID CollUpdateDivImage(DIVIMAGE* div);	//分割画像の当たり判定の更新
BOOL LoadImageMem(IMAGE* image, const char* path);
										//ゲームの画像を読み込み
BOOL LoadAudio(AUDIO* audio, const char* path, int Volume, int playType);
													//ゲームの音楽を読み込み
BOOL LoadImageDivMem(DIVIMAGE* div, const char* path, int bunkatuYoko, int bunkatuTate);
															//ゲームの画像の分割読み込み

VOID PlayAudio(AUDIO audio);					//音楽再生
int GetVolumeAudio(AUDIO audio);				//音楽のボリューム取得
VOID SetVolumeAudio(AUDIO* audio, int vol);		//音楽のボリューム設定
VOID ChangeVolumeAudio(AUDIO* audio, int vol);	//音楽のボリューム変更
VOID StopAudio(AUDIO* audio);					//音楽停止

VOID DrawImage(IMAGE image);					//画像描画
VOID DrawRect(RECT r, unsigned int color, bool b);	//RECT描画
VOID DrawDivImage(DIVIMAGE* image);				//分割画像の描画
VOID DrawDivImageChara(DIVIMAGE* image);		//分割画像の描画

//★★★ゲーム共通のプロトタイプ宣言★★★

//サンプルの素材
IMAGE sampleImg;
DIVIMAGE sampleDivImg;
DIVIMAGE samplePlayerImg;
MUKI muki = muki_shita;		//サンプル向き

AUDIO sampleBGM;
AUDIO playBGM;

MAP_DATA map1;				//マップ１

RECT event;	//イベントが起こるマス

// プログラムは WinMain から始まります
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);				//Log.txtを出力しない
	ChangeWindowMode(TRUE);								//ウィンドウモードに設定
	SetMainWindowText(GAME_TITLE);						//ウィンドウのタイトルの文字
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//ウィンドウの解像度を設定
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);				//ウィンドウの大きさを設定
	SetBackgroundColor(255, 255, 255);					//デフォルトの背景の色
	SetWindowIconID(GAME_ICON_ID);						//アイコンファイルを読込
	SetWindowStyleMode(GAME_WINDOW_BAR);				//ウィンドウバーの状態
	SetWaitVSyncFlag(TRUE);								//ディスプレイの垂直同期を有効にする
	SetAlwaysRunFlag(TRUE);								//ウィンドウをずっとアクティブにする

	//ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)
	{
		// エラーが起きたら直ちに終了
		return -1;
	}

	//ダブルバッファリング有効化
	SetDrawScreen(DX_SCREEN_BACK);

	//最初のシーンは、タイトル画面から
	GameScene = GAME_SCENE_TITLE;

	//フォント追加
	if (FontAdd() == FALSE) { return FALSE; }

	//ゲーム読み込み
	if (!GameLoad())
	{
		//データの読み込みに失敗したとき
		GameDelete();	//データ削除
		DxLib_End();	//DxLib終了
		return -1;		//異常終了
	}

	//ゲームの初期化
	GameInit();

	//無限ループ
	while (1)
	{
		if (ProcessMessage() != 0) { break; }	//メッセージを受け取り続ける
		if (ClearDrawScreen() != 0) { break; }	//画面を消去する

		//キーボード入力の更新
		AllKeyUpdate();

		//マウス入力の更新
		MouseUpdate();

		//FPS値の更新
		FPSUpdate();

		//ESCキーで強制終了
		if (KeyClick(KEY_INPUT_ESCAPE) == TRUE) { break; }

		//以前のシーンを取得
		if (GameScene != GAME_SCENE_CHANGE)
		{
			OldGameScene = GameScene;
		}

		//シーンごとに処理を行う
		switch (GameScene)
		{
		case GAME_SCENE_TITLE:
			Title();			//タイトル画面
			break;
		case GAME_SCENE_PLAY:
			Play();				//プレイ画面
			break;
		case GAME_SCENE_END:
			End();				//エンド画面
			break;
		case GAME_SCENE_CHANGE:
			Change();			//切り替え画面
			break;
		default:
			break;
		}

		//シーンを切り替える
		if (OldGameScene != GameScene)
		{
			//現在のシーンが切り替え画面でないとき
			if (GameScene != GAME_SCENE_CHANGE)
			{
				NextGameScene = GameScene;		//次のシーンを保存
				GameScene = GAME_SCENE_CHANGE;	//画面切り替えシーンに変える
			}
		}

		//現在のマウスの情報を描画
		MouseDraw();

		//FPS値を描画
		FPSDraw();

		//FPS値を待つ
		FPSWait();

		ScreenFlip();	//ダブルバッファリングした画面を描画
	}

	//フォント削除
	FontRemove();

	//データ削除
	GameDelete();

	//ＤＸライブラリ使用の終了処理
	DxLib_End();

	return 0;	// ソフトの終了 
}

/// <summary>
/// ゲームのデータを読み込み
/// </summary>
/// <returns>読み込めたらTRUE / 読み込めなかったらFALSE</returns>
BOOL GameLoad(VOID)
{
	//サンプル画像を読み込み
	if (LoadImageMem(&sampleImg, ".\\Image\\player.png") == FALSE) { return FALSE; }

	//サンプル分割画像を読み込み
	if (LoadImageDivMem(&sampleDivImg, ".\\Image\\baku1.png", 8, 2) == FALSE) { return FALSE; }

	//サンプル分割画像を読み込み
	if (LoadImageDivMem(&samplePlayerImg, ".\\Image\\charachip_huran.png", 3, 4) == FALSE) { return FALSE; }

	//★音楽変えたい
	//サンプルBGMを読み込み
	if (LoadAudio(&sampleBGM, ".\\Audio\\ブリキのPARADE.mp3", 128, DX_PLAYTYPE_LOOP) == FALSE) { return FALSE; }

	//プレイ画面BGMを読み込み
	if (LoadAudio(&playBGM, ".\\Audio\\playBGM.mp3", 128, DX_PLAYTYPE_LOOP) == FALSE) { return FALSE; }

	//フォントハンドルを作成
	if (FontCreate() == FALSE) { return FALSE; }

	//サンプル敵データ読み込み
	if (LoadCSVChara(CSV_PATH_ENEMY, &enemy[0], ENEMY_MAX, TRUE) == FALSE) { return FALSE; }
	
	//サンプルスコアデータを読み込み
	if (LoadScoreData(SCORE_DATA_PATH, &score_data, TRUE) == FALSE) { return FALSE; }

	//サンプルマップデータを読み込み
	if (LoadCSVMap(
		IMG_PATH_MAP1,
		CSV_PATH_MAP1_SHITA,
		CSV_PATH_MAP1_NAKA,
		CSV_PATH_MAP1_NAKA_ATARI,
		CSV_PATH_MAP1_UE,
		&map1,
		MAP1_YOKO_DIV, MAP1_TATE_DIV
	) == FALSE) {
		return FALSE;
	}


	return TRUE;	//全て読み込みた！
}

/// <summary>
/// ゲームのデータを削除
/// </summary>
VOID GameDelete(VOID)
{
	//サンプル画像を削除
	DeleteGraph(sampleImg.handle);

	//サンプル分割画像を削除
	for (int i = 0; i < sampleDivImg.DivMax; i++) { DeleteGraph(sampleDivImg.handle[i]); }

	//サンプル分割画像を削除
	for (int i = 0; i < samplePlayerImg.DivMax; i++) { DeleteGraph(samplePlayerImg.handle[i]); }

	//サンプル音楽を削除
	DeleteMusicMem(sampleBGM.handle);

	//プレイ画面BGMを削除
	DeleteMusicMem(playBGM.handle);

	//フォントデータ削除
	FontDelete();

	return;
}

/// <summary>
/// ゲームデータを初期化
/// </summary>
/// <param name=""></param>
VOID GameInit(VOID)
{

	//ゲーム内時間リセット
	ResetGameTime();

	//イベントが起こるマス
	event.top = map1.height * (10) + 1;
	event.left = map1.width * (1) + 1;
	event.bottom = map1.height * (10 + 1) - 1;
	event.right = map1.width * (1 + 1) - 1;

	return;
}

/// <summary>
/// シーンを切り替える関数
/// </summary>
/// <param name="scene">シーン</param>
VOID ChangeScene(GAME_SCENE scene)
{
	GameScene = scene;	//シーンを切り替え
	IsFadeIn = FALSE;	//フェードインしない
	IsFadeOut = TRUE;	//フェードアウトする

	return;
}

/// <summary>
/// タイトル画面
/// </summary>
VOID Title(VOID)
{
	TitleProc();	//処理
	TitleDraw();	//描画

	return;
}

/// <summary>
/// タイトル画面の処理
/// </summary>
VOID TitleProc(VOID)
{
	//セーブデータ系サンプル
	{
		//セーブデータ削除サンプル
		if (KeyDown(KEY_INPUT_Q)) { DeleteScoreData(); }

		//セーブデータ保存サンプル
		if (KeyDown(KEY_INPUT_E))
		{
			//スコアデータを適当に設定
			score_data.Score1 = (int)GetGameTime();
			strcpyDx(score_data.Name1, "Taro");

			//スコアデータの並び返しながら・・・

			//スコアデータ保存
			SaveScoreData();
		}
	}

	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//シーン切り替え
		//次のシーンの初期化をここで行うと楽になる

		//ゲームの初期化
		GameInit();

		//BGMを止める
		StopAudio(&sampleBGM);

		return;
	}

	PlayAudio(sampleBGM);	//BGMを鳴らす

	/*
	//プレイヤーの動作サンプル
	{
		muki = muki_none;	//最初は向きを無しにする

		if (KeyDown(KEY_INPUT_W)) 
		{ 
			muki = muki_ue; samplePlayerImg.y--; 
		}
		else if (KeyDown(KEY_INPUT_S)) 
		{ 
			muki = muki_shita; samplePlayerImg.y++; 
		}
		if (KeyDown(KEY_INPUT_A)) 
		{ 
			muki = muki_hidari; samplePlayerImg.x--; 
		}
		else if (KeyDown(KEY_INPUT_D)) 
		{ 
			muki = muki_migi; samplePlayerImg.x++; 
		}
		
		CollUpdateDivImage(&samplePlayerImg);	//当たり判定の更新
	}
	*/

	//マップの当たり判定を考慮
	{
		muki = muki_none;	//最初は向きをなしにする
		DIVIMAGE dummy = samplePlayerImg;	//当たり判定のダミーを作っておく

		if (KeyDown(KEY_INPUT_W))			{ muki = muki_ue; dummy.y--; }
		else if (KeyDown(KEY_INPUT_S))		{ muki = muki_shita; dummy.y++; }
		if (KeyDown(KEY_INPUT_A))			{ muki = muki_hidari; dummy.x--; }
		else if (KeyDown(KEY_INPUT_D))		{ muki = muki_migi; dummy.x++; }

		CollUpdateDivImage(&dummy);	//当たり判定の更新

		if (CollMap(dummy.coll, map1) == FALSE)
		{
			samplePlayerImg = dummy;	//ダミー情報を戻す
		}

		//イベントのマスとプレイヤーがあたっているか
		if (CheckCollRectToRect(samplePlayerImg.coll, event) == TRUE)
		{
			//ゲームの初期化
			GameInit();

			//BGMを止める
			StopAudio(&sampleBGM);

			//プレイ画面に切り替え
			ChangeScene(GAME_SCENE_PLAY);
		}
	}
	
	return;
}

int size = 0;

/// <summary>
/// タイトル画面の描画
/// </summary>
VOID TitleDraw(VOID)
{

	DrawImage(sampleImg);				//サンプル画像の描画
	DrawDivImage(&sampleDivImg);		//サンプル分割画像の描画

	//プレイヤーの動作サンプル
	{
		DrawDivImageChara(&samplePlayerImg);//サンプル分割画像の描画
	}

	//ゲーム内時間
	DrawFormatString(500, 310, GetColor(0, 0, 0), "TIME:%3.2f", GetGameTime());

	//ゲーム内時間
	DrawFormatString(500, 300, GetColor(255, 0, 0), "TIME:%3.2f", GetGameTime());

	//制限時間を表示
	DrawFormatString(500, 90, GetColor(0, 0, 0), "残り:%3.2f", 30.0f - GetGameTime());

	//現在の日付と時刻
	DrawFormatString(500, 70, GetColor(0, 0, 0), "DATE:%4d/%2d/%2d %2d:%2d:%2d", fps.NowDataTime.Year, fps.NowDataTime.Mon, fps.NowDataTime.Day, fps.NowDataTime.Hour, fps.NowDataTime.Min, fps.NowDataTime.Sec);
	
	//フォントのサンプル
	DrawStringToHandle(100, 100, "シンデレラフォント", GetColor(0, 0, 0), sampleFont1.handle);

	//数値を出したとき
	DrawFormatStringToHandle(200, 200, GetColor(0, 0, 0), sampleFont2.handle, "残り:%3.2f", 30.0f - GetGameTime());

	//フォントのサンプル
	DrawStringToHandle(600, 100, CinderellaFont.Name, GetColor(0, 0, 0), CinderellaFont.handle);

	//読み込んだデータを描画
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		DrawFormatString(300, 300 + i * 20, GetColor(0, 0, 0), "%s", enemy[i].Name);
	}

	//マップのサンプル
	DrawMap(map1);

	//イベントが起こるマス
	DrawRect(event, GetColor(0, 255, 0), TRUE);

	DrawString(0, 0, "タイトル画面", GetColor(0, 0, 0));

	return;
}

/// <summary>
/// プレイ画面
/// </summary>
VOID Play(VOID)
{
	PlayProc();	//処理
	PlayDraw();	//描画

	return;
}

/// <summary>
/// プレイ画面の処理
/// </summary>
VOID PlayProc(VOID)
{
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//音楽を止める
		StopAudio(&playBGM);

		//プレイ画面に切り替え
		ChangeScene(GAME_SCENE_END);
		return;
	}

	PlayAudio(playBGM);	//BGMを鳴らす

	return;
}

/// <summary>
/// プレイ画面の描画
/// </summary>
VOID PlayDraw(VOID)
{

	DrawString(0, 0, "プレイ画面", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// エンド画面
/// </summary>
VOID End(VOID)
{
	EndProc();	//処理
	EndDraw();	//描画

	return;
}

/// <summary>
/// エンド画面の処理
/// </summary>
VOID EndProc(VOID)
{
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//タイトル画面に切り替え
		ChangeScene(GAME_SCENE_TITLE);

		return;
	}

	return;
}

/// <summary>
/// エンド画面の描画
/// </summary>
VOID EndDraw(VOID)
{
	DrawString(0, 0, "エンド画面", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// 切り替え画面
/// </summary>
VOID Change(VOID)
{
	ChangeProc();	//処理
	ChangeDraw();	//描画

	return;
}

/// <summary>
/// 切り替え画面の処理
/// </summary>
VOID ChangeProc(VOID)
{

	//フェードイン
	if (IsFadeIn == TRUE)
	{
		if (fadeInCnt > fadeInCntMax)
		{
			fadeInCnt--;	//カウンタを減らす
		}
		else
		{
			//フェードイン処理が終わった

			fadeInCnt = fadeInCntInit;	//カウンタ初期化
			IsFadeIn = FALSE;			//フェードイン処理終了
		}
	}

	//フェードアウト
	if (IsFadeOut == TRUE)
	{
		if (fadeOutCnt < fadeOutCntMax)
		{
			fadeOutCnt++;	//カウンタを増やす
		}
		else
		{
			//フェードアウト処理が終わった

			fadeOutCnt = fadeOutCntInit;	//カウンタ初期化
			IsFadeOut = FALSE;				//フェードアウト処理終了
		}
	}

	//切り替え処理終了
	if (IsFadeIn == FALSE && IsFadeOut == FALSE)
	{
		//フェードインしていない、フェードアウトもしていないとき
		GameScene = NextGameScene;	//次のシーンに切り替え
		OldGameScene = GameScene;	//以前のゲームシーン更新
	}


	return;
}

/// <summary>
/// 切り替え画面の描画
/// </summary>
VOID ChangeDraw(VOID)
{
	//以前のシーンを描画
	switch (OldGameScene)
	{
	case GAME_SCENE_TITLE:
		TitleDraw();	//タイトル画面の描画
		break;
	case GAME_SCENE_PLAY:
		PlayDraw();		//プレイ画面の描画
		break;
	case GAME_SCENE_END:
		EndDraw();		//エンド画面の描画
		break;
	default:
		break;
	}

	//フェードイン
	if (IsFadeIn == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeInCnt / fadeInCntMax) * 255);
	}

	//フェードアウト
	if (IsFadeOut == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeOutCnt / fadeOutCntMax) * 255);
	}

	//四角を描画
	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 0), TRUE);

	//半透明終了
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawString(0, 0, "切り替え画面", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// 当たり判定の領域更新（画像）
/// </summary>
/// <param name="img">画像構造体のポインタ</param>
VOID CollUpdateImage(IMAGE* img)
{
	img->coll.left = img->x;
	img->coll.top = img->y;

	img->coll.right = img->x + img->width;
	img->coll.bottom = img->y + img->height;

	return;
}

/// <summary>
/// 当たり判定の領域更新（分割画像）
/// </summary>
/// <param name="img">画像構造体のポインタ</param>
VOID CollUpdateDivImage(DIVIMAGE* div)
{
	div->coll.left = div->x +15;
	div->coll.top = div->y+20;

	div->coll.right = div->x + div->width-10;
	div->coll.bottom = div->y + div->height;

	return;
}

/// <summary>
/// 矩形と矩形の当たり判定
/// </summary>
/// <param name="a">矩形A</param>
/// <param name="b">矩形B</param>
/// <returns>あたったらTRUE/あたらないならFALSE</returns>
BOOL OnCollRect(RECT a, RECT b)
{
	if (
		a.left < b.right &&		//　矩形Aの左辺X座標 < 矩形Bの右辺X座標　かつ
		a.right > b.left &&		//　矩形Aの右辺X座標 > 矩形Bの左辺X座標　かつ
		a.top  < b.bottom &&	//　矩形Aの上辺Y座標 < 矩形Bの下辺Y座標　かつ
		a.bottom > b.top		//　矩形Aの下辺Y座標 > 矩形Bの上辺Y座標
		)
	{
		//あたっているとき
		return TRUE;
	}
	else
	{
		//あたっていないとき
		return FALSE;
	}
}


/// <summary>
/// 画像を分割してメモリに読み込み
/// </summary>
/// <param name="handle">ハンドル配列の先頭アドレス</param>
/// <param name="path">画像のパス</param>
/// <param name="bunkatuYoko">分割するときの横の数</param>
/// <param name="bunkatuTate">分割するときの縦の数</param>
/// <returns></returns>
BOOL LoadImageDivMem(DIVIMAGE* div, const char* path, int bunkatuYoko, int bunkatuTate)
{
	int IsLoad = -1;	//画像が読み込みたか？

	//一時的に画像のハンドルを用意する
	int TempHandle = LoadGraph(path);

	//読み込みエラー
	if (TempHandle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//ウィンドウハンドル
			path,					//本文
			"画像読み込みエラー",	//タイトル
			MB_OK					//ボタン
		);

		return FALSE;	//読み込み失敗
	}

	//画像の幅と高さを取得
	int Width = -1;		//幅
	int Height = -1;	//高さ
	GetGraphSize(TempHandle, &Width, &Height);

	//分割して読み込み
	IsLoad = LoadDivGraph(
		path,							//画像のパス
		bunkatuYoko * bunkatuTate,		//分割総数
		bunkatuYoko, bunkatuTate,					//横の分割,縦の分割
		Width / bunkatuYoko, Height / bunkatuTate,	//画像1つ分の幅,高さ
		div->handle									//連続で管理する配列の先頭アドレス
	);

	//分割エラー
	if (IsLoad == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//ウィンドウハンドル
			path,					//本文
			"画像分割エラー",		//タイトル
			MB_OK					//ボタン
		);

		return FALSE;	//読み込み失敗
	}

	//情報を設定
	div->DivYoko = bunkatuYoko;
	div->DivTate = bunkatuTate;
	div->DivMax = div->DivYoko * div->DivTate;
	div->x = 0;
	div->y = 0;
	GetGraphSize(div->handle[0], &div->width, &div->height);
	div->AnimCnt = 0;
	div->IsAnimLoop = FALSE;
	div->nowIndex = 0;

	//当たり判定の更新
	CollUpdateDivImage(div);

	//画像を描画する
	div->IsDraw = TRUE;

	//一時的に読み込んだハンドルを解放
	DeleteGraph(TempHandle);

	return TRUE;
}

/// <summary>
/// 画像をメモリに読み込み
/// </summary>
/// <param name="image">画像構造体のアドレス</param>
/// <param name="path">画像のパス</param>
/// <returns></returns>
BOOL LoadImageMem(IMAGE* image, const char* path)
{
	//ゴールの画像を読み込み
	strcpyDx(image->path, path);			//パスのコピー
	image->handle = LoadGraph(image->path);	//画像の読み込み

	//画像が読み込めなかったときは、エラー(-1)が入る
	if (image->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//メインのウィンドウハンドル
			image->path,			//メッセージ本文
			"画像読み込みエラー！",	//メッセージタイトル
			MB_OK					//ボタン
		);

		return FALSE;	//読み込み失敗
	}

	//画像の幅と高さを取得
	GetGraphSize(image->handle, &image->width, &image->height);

	//画像の位置を初期化
	image->x = 0; image->y = 0;

	//当たり判定の更新
	CollUpdateImage(image);

	//画像を描画する
	image->IsDraw = TRUE;

	//読み込めた
	return TRUE;
}

/// <summary>
/// 画像描画
/// </summary>
/// <param name="image">描画する画像構造体</param>
/// <returns></returns>
/// <returns></returns>
VOID DrawImage(IMAGE image)
{
	//画像が描画できるときは
	if (image.IsDraw == TRUE)
	{
		DrawGraph(image.x, image.y, image.handle, TRUE);
	}

	//デバッグモードのときは
	if (GAME_DEBUG == TRUE)
	{
		//当たり判定を描画
		DrawRect(image.coll, GetColor(255, 255, 255), FALSE);
	}
	return;
}

/// <summary>
/// 分割画像の描画
/// </summary>
/// <param name="image">描画する画像構造体</param>
/// <returns></returns>
/// <returns></returns>
VOID DrawDivImage(DIVIMAGE* image)
{
	//画像が描画できるときは
	if (image->IsDraw == TRUE)
	{
		DrawGraph(image->x, image->y, image->handle[image->nowIndex], TRUE);

		//アニメーションのカウントアップ
		if (image->AnimCnt < image->AnimCntMAX) { image->AnimCnt++; }
		else
		{
			//すべてのアニメーションを描画し終わったら
			if (image->nowIndex == image->DivMax - 1)
			{
				//アニメーションを繰り返さないならば
				if (image->IsAnimLoop == FALSE)
				{
					image->IsDraw = FALSE;	//描画を止める
				}
				image->AnimCnt = 0;		//カウンタ0クリア
				image->nowIndex = 0;	//先頭に戻す
			}
			//次の分割画像があるときは
			else if (image->nowIndex < image->DivMax - 1)
			{
				image->nowIndex++; //次の画像へ
			}
			image->AnimCnt = 0;	//カウンタ0クリア
		}
	}

	//デバッグモードのときは
	if (GAME_DEBUG == TRUE)
	{
		//当たり判定を描画
		DrawRect(image->coll, GetColor(255, 255, 255), FALSE);
	}
	return;
}

/// <summary>
/// 分割画像の描画(キャラなど)
/// </summary>
/// <param name="image">描画する画像構造体</param>
/// <returns></returns>
/// <returns></returns>
VOID DrawDivImageChara(DIVIMAGE* image)
{
	//画像が描画できるときは
	if (image->IsDraw == TRUE)
	{
		DrawGraph(image->x, image->y, image->handle[image->nowIndex], TRUE);

		//アニメーションのカウントアップ
		if (image->AnimCnt < image->AnimCntMAX) { image->AnimCnt++; }
		else
		{

			//右向きのとき(画像によって数字が違うので、合わせること！)
			if (muki == muki_migi)
			{
				if (image->nowIndex >= 6 && image->nowIndex < 8)
				{
					image->nowIndex++;
				}
				else
				{
					image->nowIndex = 6;
				}
			}

			//左向きのとき(画像によって数字が違うので、合わせること！)
			if (muki == muki_hidari)
			{
				if (image->nowIndex >= 3 && image->nowIndex < 5)
				{
					image->nowIndex++;
				}
				else
				{
					image->nowIndex = 3;
				}
			}

			//上向きのとき(画像によって数字が違うので、合わせること！)
			if (muki == muki_ue)
			{
				if (image->nowIndex >= 9 && image->nowIndex < 11)
				{
					image->nowIndex++;
				}
				else
				{
					image->nowIndex = 9;
				}
			}

			//下向きのとき(画像によって数字が違うので、合わせること！)
			if (muki == muki_shita)
			{
				if (image->nowIndex >= 0 && image->nowIndex < 2)
				{
					image->nowIndex++;
				}
				else
				{
					image->nowIndex = 0;
				}
			}

			//向き無しのときは、直前の向きの真ん中の画像にする(画像に合わせて決めてネ)
			if (muki == muki_none)
			{
				if (image->nowIndex >= 6 && image->nowIndex <= 8) { image->nowIndex = 7; }
				if (image->nowIndex >= 3 && image->nowIndex <= 5) { image->nowIndex = 4; }
				if (image->nowIndex >= 9 && image->nowIndex <= 11) { image->nowIndex = 10; }
				if (image->nowIndex >= 0 && image->nowIndex <= 2) { image->nowIndex = 1; }
			}

			image->AnimCnt = 0;	//カウンタ0クリア
		}
	}

	//デバッグモードのときは
	if (GAME_DEBUG == TRUE)
	{
		//当たり判定を描画
		DrawRect(image->coll, GetColor(255, 255, 255), FALSE);
	}
	return;
}

/// <summary>
/// 音楽をメモリに読み込み
/// </summary>
/// <param name="audio">Audio構造体変数のアドレス</param>
/// <param name="path">Audioの音楽パス</param>
/// <param name="volume">ボリューム</param>
/// <param name="playType">DX_PLAYTYPE_LOOP or DX_PLAYTYPE_BACK</param>
/// <returns></returns>
BOOL LoadAudio(AUDIO* audio, const char* path, int Volume, int playType)
{
	//音楽の読み込み
	strcpyDx(audio->path, path);					//パスのコピー
	audio->handle = LoadSoundMem(audio->path);		//音楽の読み込み

	//音楽が読み込めなかったときは、エラー(-1)が入る
	if (audio->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//メインのウィンドウハンドル
			audio->path,			//メッセージ本文
			"音楽読み込みエラー！",		//メッセージタイトル
			MB_OK					//ボタン
		);

		return FALSE;	//読み込み失敗
	}

	//その他の設定
	audio->Volume = Volume;
	audio->playType = playType;

	return TRUE;
}

/// <summary>
/// 音楽再生
/// </summary>
/// <param name="audio">再生する音楽構造体</param>
/// <returns></returns>
VOID PlayAudio(AUDIO audio)
{
	//ループ再生でなければ、そのまま再生する
	if (audio.playType == DX_PLAYTYPE_BACK)
	{
		PlaySoundMem(audio.handle, audio.playType);//音楽の再生
	}
	//ループ再生のときは、音楽が再生されていなければ、再生する
	else if (audio.playType == DX_PLAYTYPE_LOOP)
	{
		if (CheckSoundMem(audio.handle) == 0)
		{
			PlaySoundMem(audio.handle, audio.playType);//音楽の再生
		}
	}

	return;
}

/// <summary>
/// 音楽のボリューム取得
/// </summary>
/// <param name="audio">取得する音楽構造体</param>
/// <param name = "volume">ボリューム(0〜255)</param>
/// <returns></returns>
int GetVolumeAudio(AUDIO audio)
{
	//音量を取得
	return audio.Volume;
}

/// <summary>
/// 音楽のボリューム設定
/// </summary>
/// <param name="audio">設定する音楽構造体</param>
/// <param name = "volume">ボリューム(0〜255)</param>
/// <returns></returns>
VOID SetVolumeAudio(AUDIO* audio, int vol)
{
	//音量を設定
	audio->Volume = vol;
	ChangeVolumeSoundMem(audio->Volume, audio->handle);

	return;
}

/// <summary>
/// 音楽のボリューム変更
/// </summary>
/// <param name="audio">変更する音楽構造体</param>
/// <param name = "volume">ボリューム(0〜255)</param>
/// <returns></returns>
VOID ChangeVolumeAudio(AUDIO* audio, int vol)
{
	//音量を設定（構造体のボリュームは変更しない）
	ChangeVolumeSoundMem(vol, audio->handle);

	return;
}

/// <summary>
/// 音楽停止
/// </summary>
/// <param name="audio">停止する音楽構造体</param>
/// <returns></returns>
VOID StopAudio(AUDIO* audio)
{
	//音楽の停止
	StopSoundMem(audio->handle);

	return;
}