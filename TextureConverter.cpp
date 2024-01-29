#include "TextureConverter.h"

using namespace DirectX;


void TextureConverter::ConvertTextureWICToDDS(const std::string& filePath)
{
	//�e�N�X�`���t�@�C����ǂݍ���
	LoadWICTextureFromFile(filePath);

	SaveDDSTextureToFile();
}

void TextureConverter::LoadWICTextureFromFile(const std::string& filePath)
{
	HRESULT result;
	std::wstring wFilePath = ConvertMultiByteStringToWideString(filePath);

	//WIC�e�N�X�`���̃��[�h
	result = LoadFromWICFile(wFilePath.c_str(), WIC_FLAGS_NONE, &_metadata, _scratchImage);

	assert(SUCCEEDED(result));

	//�t�H���_�p�X�ƃt�@�C�����𕪗�����
	SeparateFilePath(wFilePath);
}

std::wstring TextureConverter::ConvertMultiByteStringToWideString(const std::string& mString)
{
	//���C�h������ɕϊ��������������v�Z
	int filePathBufferSize = MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, nullptr, 0);

	//���C�h������
	std::wstring wString;
	wString.resize(filePathBufferSize);

	//���C�h������ɕϊ�
	MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, &wString[0], filePathBufferSize);

	return wString;
}



void TextureConverter::SeparateFilePath(const std::wstring& filePath)
{
	size_t pos1;
	std::wstring exceptExt;

	//��؂蕶��'.'���o�Ă����ԍŌ�̕���������
	pos1 = filePath.rfind('.');
	//�������q�b�g������
	if (pos1 != std::wstring::npos)
	{
		//��؂蕶���̌����g���q�t�@�C���Ƃ��ĕۑ�
		fileExt_ = filePath.substr(pos1 + 1, filePath.size() - pos1 - 1);
		//��؂蕶���̑O�܂ł𔲂��o��
		exceptExt = filePath.substr(0, pos1);
	}
	else
	{
		fileExt_ = L"";
		exceptExt = filePath;
	}
	//��؂蕶��'//'���o�Ă����ԍŌ�̕���������
	pos1 = exceptExt.rfind('//');
	if (pos1 != std::wstring::npos)
	{
		//��؂蕶���O�܂ł��f�B���N�g���p�X�Ƃ��ĕۑ�
		directoryPath_ = exceptExt.substr(0, pos1 + 1);
		//��؂蕶���̌����t�@�C�����Ƃ��ĕۑ�
		fileName_ = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;
	}
	//��؂蕶��'/'���o�Ă����ԍŌ�̕���������
	pos1 = exceptExt.rfind('/');
	if (pos1 != std::wstring::npos)
	{
		//�l���悤
		//��؂蕶���O�܂ł��f�B���N�g���p�X�Ƃ��ĕۑ�
		directoryPath_ = exceptExt.substr(0, pos1 + 1);
		//��؂蕶���̌����t�@�C�����Ƃ��ĕۑ�
		fileName_ = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;
	}
	//��؂蕶���������̂Ńt�@�C�����݂̂Ƃ��Ĉ���
	directoryPath_ = L"";
	fileName_ = exceptExt;
}

void TextureConverter::SaveDDSTextureToFile()
{
	HRESULT result;
	ScratchImage mipChain;
	size_t mipLevel = 0;

	//�~�b�v�}�b�v����
	result = GenerateMipMaps(
		_scratchImage.GetImages(), _scratchImage.GetImageCount(), _scratchImage.GetMetadata(),
		TEX_FILTER_DEFAULT, mipLevel, mipChain);
	if (SUCCEEDED(result))
	{
		_scratchImage = std::move(mipChain);
		_metadata = _scratchImage.GetMetadata();
	}

	//���k�`���ɕϊ�
	ScratchImage converted;
	result = Compress(_scratchImage.GetImages(), _scratchImage.GetImageCount(), _metadata,
		DXGI_FORMAT_BC7_UNORM_SRGB, TEX_COMPRESS_BC7_QUICK | TEX_COMPRESS_SRGB_OUT |
		TEX_COMPRESS_PARALLEL, 1.0f, converted);
	if (SUCCEEDED(result))
	{
		_scratchImage = std::move(converted);
		_metadata = _scratchImage.GetMetadata();
	}

	//�o�̓t�@�C������ݒ肷��
	std::wstring filePath = directoryPath_ + fileName_ + L".dds";

	//�ǂݍ��񂾃e�N�X�`����SRGB���Ƃ���
	_metadata.format = MakeSRGB(_metadata.format);

	//DDS�t�@�C�������o��
	result = SaveToDDSFile(_scratchImage.GetImages(), _scratchImage.GetImageCount(),
		_metadata, DDS_FLAGS_NONE, filePath.c_str());
	assert(SUCCEEDED(result));
}
