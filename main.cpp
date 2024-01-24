#include<cstdio>
#include<cstdlib>
#include <cassert>
#include <Windows.h>
#include "TextureConverter.h"

//�R�}���h���C������
enum Argument {
	kApplicationPath,   //�A�v���P�[�V�����p�X
	kFilePath,          //�n���ꂽ�t�@�C���̃p�X

	NumArgument
};
//argv = 0x00000214f4f30780 {0x00000214f4f30798 "C:\\Users\\k021g1125\\Desktop\\�w�Z\\LE3C\\���\\TL2\\TextureConverter\\x64\\Debug\\TextureConverter.exe"}
int main(int argc, char* argv[])
{
	for (int i = 0; i < argc; i++)
	{
		printf(argv[i]);
		printf("\n");

	} 

	assert(argc >= NumArgument);

	//COM���C�u�����̏�����
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(SUCCEEDED(hr));

	//�e�N�X�`���R���p�[�^
	TextureConverter converter;

	//�e�N�X�`���ϊ�
	converter.ConvertTextureWICToDDS(argv[kFilePath]);

	//COM���C�u�����̏I��
	CoUninitialize();

	system("pause");
	return 0;
}