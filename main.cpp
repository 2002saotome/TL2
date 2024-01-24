#include<cstdio>
#include<cstdlib>
#include <cassert>
#include <Windows.h>
#include "TextureConverter.h"

//コマンドライン引数
enum Argument {
	kApplicationPath,   //アプリケーションパス
	kFilePath,          //渡されたファイルのパス

	NumArgument
};
//argv = 0x00000214f4f30780 {0x00000214f4f30798 "C:\\Users\\k021g1125\\Desktop\\学校\\LE3C\\後期\\TL2\\TextureConverter\\x64\\Debug\\TextureConverter.exe"}
int main(int argc, char* argv[])
{
	for (int i = 0; i < argc; i++)
	{
		printf(argv[i]);
		printf("\n");

	} 

	assert(argc >= NumArgument);

	//COMライブラリの初期化
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(SUCCEEDED(hr));

	//テクスチャコンパータ
	TextureConverter converter;

	//テクスチャ変換
	converter.ConvertTextureWICToDDS(argv[kFilePath]);

	//COMライブラリの終了
	CoUninitialize();

	system("pause");
	return 0;
}