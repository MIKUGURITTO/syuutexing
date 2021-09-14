//フォント処理のソースファイル

//ヘッダーファイル読み込み ###############################################################
#include "game.h"
#include "font.h"

//グローバル変数 #########################################################################

//フォントデータを管理
FONT fontdef;		//デフォルトのフォント

FONT sampleFont1;	//サンプルのフォント(PCに入っていそうなフォント)
FONT sampleFont2;	//サンプルのフォント(フォントファイルから読み込んだフォント)

FONT CinderellaFont;	//シンデレラ

//関数 ###################################################################################

/// <summary>
/// フォントの読込
/// </summary>
/// <returns>読み込めたらTRUE/読み込めなかったらFALSE</returns>
BOOL FontAdd(VOID)
{
	//フォントを一時的に読み込み(WinAPI)
	if (AddFontResourceEx(FONT_JIYU_PATH, FR_PRIVATE, NULL) == 0)
	{
		//エラーメッセージ表示
		MessageBox(GetMainWindowHandle(), FONT_JIYU_NAME, FONT_INSTALL_ERR_TITLE, MB_OK);
		return FALSE;
	}

	//フォントを一時的に読み込み(WinAPI)
	if (AddFontResourceEx(FONT_CINDERELLA_PATH, FR_PRIVATE, NULL) == 0)
	{
		//エラーメッセージ表示
		MessageBox(GetMainWindowHandle(), FONT_CINDERELLA_NAME, FONT_INSTALL_ERR_TITLE, MB_OK);
		return FALSE;
	}

	return TRUE;
}

/// <summary>
/// フォント作成
/// </summary>
BOOL FontCreate(VOID)
{
	//デフォルトフォントの情報を初期化
	SetFont(
		&fontdef,			//設定するフォント
		DEF_FONT_NAME,		//フォントの名前をコピー
		DEF_FONT_SIZE,		//フォントのサイズ
		DEF_FONT_THINCK,	//フォントの太さ
		DEF_FONT_TYPE		//フォントのタイプ
	);

	//デフォルトフォントのハンドル作成
	if (FontCreateHandle(&fontdef) == FALSE) { return FALSE; }

	//デフォルトフォントを全体に反映
	SetDefaultFont(fontdef);

	//サンプルフォントの情報を設定
	SetFont(
		&sampleFont1,
		FONT_MSGOTH_NAME,
		32,
		5,
		DX_FONTTYPE_ANTIALIASING
	);

	//フォントハンドルを作成
	if (FontCreateHandle(&sampleFont1) == FALSE) { return FALSE; }

	//サンプルフォントの情報を設定
	SetFont(
		&sampleFont2,
		FONT_JIYU_NAME,
		64,
		1,
		DX_FONTTYPE_ANTIALIASING
	);

	//フォントハンドルを作成
	if (FontCreateHandle(&sampleFont2) == FALSE) { return FALSE; }

	//サンプルフォントの情報を設定
	SetFont(
		&CinderellaFont,
		FONT_CINDERELLA_NAME,
		64,
		1,
		DX_FONTTYPE_ANTIALIASING
	);

	//フォントハンドルを作成
	if (FontCreateHandle(&CinderellaFont) == FALSE) { return FALSE; }

	return TRUE;
}

/// <summary>
/// フォントハンドルを作成
/// </summary>
/// <param name="f">フォント構造体</param>
/// <returns></returns>
BOOL FontCreateHandle(FONT* f)
{
	f->handle = CreateFontToHandle(
		f->Name,
		f->Size,
		f->Thinck,
		f->Type
	);

	if (f->handle == -1) { return FALSE; }

	return TRUE;
}


/// <summary>
/// フォント削除
/// </summary>
VOID FontDelete(VOID)
{
	//デフォルトフォント削除
	DeleteFontToHandle(fontdef.handle);

	DeleteFontToHandle(sampleFont1.handle);	//サンプルフォント削除
	DeleteFontToHandle(sampleFont2.handle);	//サンプルフォント削除

	DeleteFontToHandle(CinderellaFont.handle);	//サンプルフォント削除

	return;
}

/// <summary>
/// 引数のフォント構造体へ設定する
/// </summary>
/// <param name="f">設定するフォント構造体</param>
/// <param name="name">フォント名</param>
/// <param name="size">フォントのサイズ</param>
/// <param name="thinck">フォントの太さ</param>
/// <param name="type">フォントのタイプ</param>
/// <returns></returns>
VOID SetFont(FONT* f, const char* name, int size, int thinck, int type)
{
	strcpy_sDx(f->Name, STR_MAX, name);	//フォントの名前をコピー
	f->Size = size;						//フォントのサイズ
	f->Thinck = thinck;					//フォントの太さ
	f->Type = type;						//フォントのタイプ
	return;
}

/// <summary>
/// フォントを画面全体に反映させる
/// </summary>
/// <param name="f">設定するフォント構造体</param>
/// <returns></returns>
VOID SetDefaultFont(FONT f)
{
	ChangeFont(f.Name);				//フォント名を設定
	SetFontSize(f.Size);			//フォントのサイズを設定
	SetFontThickness(f.Thinck);		//フォントの太さを設定
	ChangeFontType(f.Type);			//フォントのタイプを設定

	return;
}

/// <summary>
/// フォントの削除
/// </summary>
/// <returns></returns>
VOID FontRemove(VOID)
{
	//一時的に読み込んだフォントを削除(WinAPI)
	RemoveFontResourceEx(FONT_JIYU_PATH, FR_PRIVATE, NULL);

	return;
}