#pragma once
#include <string>
#include "DirectXTex/DirectXTex.h"

class TextureConverter
{
public:
	/// <summary>
	/// �e�N�X�`����WIC����DDS�ɕϊ�����
	/// </summary>
	/// <param name="filePath"></param>
	void ConvertTextureWICToDDS(const std::string& filePath);

private:
	/// <summary>
	/// �e�N�X�`����WIC����DDS�ɕϊ�����
	/// </summary>
	/// <param name="filePath"></param>
	void LoadWICTextureFromFile(const std::string& filePath);

private:
	/// <summary>
	/// �}���`�o�C�g����������C�h������ɕϊ�
	/// </summary>
	/// <param name="mString">�}���`�o�C�g������</param>
	/// <returns>���C�h������</returns>
	static std::wstring ConvertMultiByteStringToWideString(const std::string& mString);

private:
	//�摜�̏��
	DirectX::TexMetadata _metadata;
	//�摜�C���[�W�̃R���e�i
	DirectX::ScratchImage _scratchImage;

private:
	///<summary>
	///�t�H���_�p�X�ƃt�@�C�����𕪗�����
	/// </summary>
	/// <param name="filePath">�t�@�C���p�X</param>
	void SeparateFilePath(const std::wstring& filePath);

private:
	//�f�B���N�g���p�X
	std::wstring directoryPath_;
	//�t�@�C����
	std::wstring fileName_;
	//�t�@�C���g���q
	std::wstring fileExt_;

private:
	///<summary>
	///�t�H���_�p�X�ƃt�@�C�����𕪗�����
	/// </summary>
	void SaveDDSTextureToFile();
};

