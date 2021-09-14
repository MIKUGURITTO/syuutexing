//Data�n�̃w�b�_�t�@�C��
#pragma once

//�w�b�_�t�@�C���ǂݍ���########################################

//�Q�[���̃��C���w�b�_�t�@�C��
#include "game.h"

//�}�C�N����`##################################################

//�G���[���b�Z�[�W
#define CSV_LOAD_EAR_TITLE	TEXT("CSV�ǂݍ��݃G���[")

//�L�����f�[�^�̃p�X
#define CSV_PATH_ENEMY		TEXT(".Data\\enemy.csv")	//ANSI�̕����R�[�h�Ńt�@�C���ۑ�

//�f�[�^�̏����w��q
//�L�����N�^�̏����w��q (%[^,]�͕�����j
#define CSV_CHARA_FORMAT	TEXT("%d,%[^,],%d,%d,%d,%d,%d,%d")

//�G�f�[�^
#define ENEMY_MAX	5

//�L�����f�[�^�\���́i�����܂ŃT���v���j
struct CHARA_DATA
{
	int No;				//No
	char Name[STR_MAX];	//���O

	//���ۂɎg���l
	int HP;				//HP
	int ATK;			//�U����
	int DEF;			//�h���

	//�ő�l
	int MAX_HP;			//HP
	int MAX_ATK;		//�U����
	int MAX_DEF;		//�h���
};

//�O���̃O���[�o���ϐ�##########################################
extern CHARA_DATA enemy[ENEMY_MAX];		//�G�f�[�^

//�v���g�^�C�v�錾##############################################
//��extern�́A�O���Ɋ֐�������܂���I�Ƃ����ڈ�����Ă��܂��B

extern BOOL LoadSVChara(const char* path, CHARA_DATA* chara, int DataMax, BOOL IsHeader);
