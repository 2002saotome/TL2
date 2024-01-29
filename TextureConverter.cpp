#include "TextureConverter.h"

using namespace DirectX;


void TextureConverter::ConvertTextureWICToDDS(const std::string& filePath)
{
	//テクスチャファイルを読み込む
	LoadWICTextureFromFile(filePath);

	SaveDDSTextureToFile();
}

void TextureConverter::LoadWICTextureFromFile(const std::string& filePath)
{
	HRESULT result;
	std::wstring wFilePath = ConvertMultiByteStringToWideString(filePath);

	//WICテクスチャのロード
	result = LoadFromWICFile(wFilePath.c_str(), WIC_FLAGS_NONE, &_metadata, _scratchImage);

	assert(SUCCEEDED(result));

	//フォルダパスとファイル名を分離する
	SeparateFilePath(wFilePath);
}

std::wstring TextureConverter::ConvertMultiByteStringToWideString(const std::string& mString)
{
	//ワイド文字列に変換した文字数を計算
	int filePathBufferSize = MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, nullptr, 0);

	//ワイド文字列
	std::wstring wString;
	wString.resize(filePathBufferSize);

	//ワイド文字列に変換
	MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, &wString[0], filePathBufferSize);

	return wString;
}



void TextureConverter::SeparateFilePath(const std::wstring& filePath)
{
	size_t pos1;
	std::wstring exceptExt;

	//区切り文字'.'が出てくる一番最後の部分を検索
	pos1 = filePath.rfind('.');
	//検索がヒットしたら
	if (pos1 != std::wstring::npos)
	{
		//区切り文字の後ろを拡張子ファイルとして保存
		fileExt_ = filePath.substr(pos1 + 1, filePath.size() - pos1 - 1);
		//区切り文字の前までを抜き出す
		exceptExt = filePath.substr(0, pos1);
	}
	else
	{
		fileExt_ = L"";
		exceptExt = filePath;
	}
	//区切り文字'//'が出てくる一番最後の部分を検索
	pos1 = exceptExt.rfind('//');
	if (pos1 != std::wstring::npos)
	{
		//区切り文字前までをディレクトリパスとして保存
		directoryPath_ = exceptExt.substr(0, pos1 + 1);
		//区切り文字の後ろをファイル名として保存
		fileName_ = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;
	}
	//区切り文字'/'が出てくる一番最後の部分を検索
	pos1 = exceptExt.rfind('/');
	if (pos1 != std::wstring::npos)
	{
		//考えよう
		//区切り文字前までをディレクトリパスとして保存
		directoryPath_ = exceptExt.substr(0, pos1 + 1);
		//区切り文字の後ろをファイル名として保存
		fileName_ = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;
	}
	//区切り文字が無いのでファイル名のみとして扱う
	directoryPath_ = L"";
	fileName_ = exceptExt;
}

void TextureConverter::SaveDDSTextureToFile()
{
	HRESULT result;
	ScratchImage mipChain;
	size_t mipLevel = 0;

	//ミップマップ生成
	result = GenerateMipMaps(
		_scratchImage.GetImages(), _scratchImage.GetImageCount(), _scratchImage.GetMetadata(),
		TEX_FILTER_DEFAULT, mipLevel, mipChain);
	if (SUCCEEDED(result))
	{
		_scratchImage = std::move(mipChain);
		_metadata = _scratchImage.GetMetadata();
	}

	//圧縮形式に変換
	ScratchImage converted;
	result = Compress(_scratchImage.GetImages(), _scratchImage.GetImageCount(), _metadata,
		DXGI_FORMAT_BC7_UNORM_SRGB, TEX_COMPRESS_BC7_QUICK | TEX_COMPRESS_SRGB_OUT |
		TEX_COMPRESS_PARALLEL, 1.0f, converted);
	if (SUCCEEDED(result))
	{
		_scratchImage = std::move(converted);
		_metadata = _scratchImage.GetMetadata();
	}

	//出力ファイル名を設定する
	std::wstring filePath = directoryPath_ + fileName_ + L".dds";

	//読み込んだテクスチャをSRGBをとして
	_metadata.format = MakeSRGB(_metadata.format);

	//DDSファイル書き出し
	result = SaveToDDSFile(_scratchImage.GetImages(), _scratchImage.GetImageCount(),
		_metadata, DDS_FLAGS_NONE, filePath.c_str());
	assert(SUCCEEDED(result));
}
