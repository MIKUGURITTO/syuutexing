//Data�n�̃\�[�X�t�@�C��

//�w�b�_�t�@�C���ǂݍ���########################################
#include "game.h"
#include "Data.h"
#include "shape.h"		//�}�`�̏���

//�O���[�o���ϐ�################################################
CHARA_DATA enemy[ENEMY_MAX];	//�G�f�[�^

//�֐�##########################################################

/// <summary>
/// CSV�t�@�C����ǂݍ��ށiCHARA_DATA�j
/// </summary>
/// <param name="path">CSV�t�@�C���̃p�X</param>
/// <param name="chara">�L�����f�[�^�\���̂̐擪�A�h���X</param>
/// <param name="DataMax">CSV�t�@�C���̍ő吔</param>
/// <param name="IsHeader">CSV�t�@�C���̂P�s�ڂ��w�b�_�Ȃ�TRUE</param>
/// <returns>TRUE�͓ǂݍ��߂�/FALSE�͎��s����</returns>
BOOL LoadCSVChara(const char* path, CHARA_DATA* chara, int DataMax, BOOL IsHeader = FALSE)
{
	FILE* fp;				//�t�@�C���|�C���^
	char GetChar = '\0';	//�擾���镶��
	int LoopCnt = 0;		//���[�v�J�E���^
	errno_t FileErr = NULL;	//�t�@�C���ǂݍ��݃G���[���m�F
	int IsEOF = NULL;		//�t�@�C���̏I��肩�`�F�b�N

	FileErr = fopen_s(&fp, path, "r");	//�t�@�C����ǂݍ��݃��[�h(r)�ŊJ��
	if (FileErr != NULL)
	{
		MessageBox(GetMainWindowHandle(), path, CSV_LOAD_ERR_TITLE, MB_OK);	//�ǂݍ��݃G���[
		return FALSE;
	}

	//�������琳��ɓǂݍ��߂��Ƃ��̏���****************************************

	//CSV��1�s�ڂ��w�b�_�̂Ƃ�
	if (IsHeader == TRUE)
	{
		//�P���������o���A���s���q���g�ɂP�s�ڂ�ǂݔ�΂�
		while (GetChar != '\n') { GetChar = fgetc(fp); }
	}

	//���ۂɃf�[�^���i�[����
	while (IsEOF != EOF)	//End Of File�i�t�@�C���̍Ō�j�ł͂Ȃ��Ƃ��J��Ԃ�
	{
		//CSV�̃f�[�^�̂P�s����ǂݎ��
		IsEOF = fscanf_s(
			fp,
			CSV_CHARA_FORMAT,	//�f�[�^���u��s=���R�[�h�v�ŏ�����Ă���̂��|�C���g�I
			&(chara + LoopCnt)->No,
			&(chara + LoopCnt)->Name[0], STR_MAX,	//�������ǂݍ��ނƂ��́A���̈����ŕ�����ő吔���w�肷��
			&(chara + LoopCnt)->HP,
			&(chara + LoopCnt)->ATK,
			&(chara + LoopCnt)->DEF,
			&(chara + LoopCnt)->MAX_HP,
			&(chara + LoopCnt)->MAX_ATK,
			&(chara + LoopCnt)->MAX_DEF
		);

		//���̃f�[�^��
		LoopCnt++;
	}

	//�f�[�^�̐����Ⴄ�Ƃ�
	if (LoopCnt - 1 != DataMax)
	{
		MessageBox(GetMainWindowHandle(), path, CSV_DATANUM_ERR_TITLE, MB_OK);	//�ǂݍ��݃G���[
		return FALSE;
	}

	fclose(fp);	//�t�@�C�������

	return TRUE;
}
