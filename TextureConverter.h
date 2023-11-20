#pragma once
#include <string>
#include "DirectXTex/DirectXTex.h"

class TextureConverter
{
public:
	/// <summary>
	/// テクスチャをWICからDDSに変換する
	/// </summary>
	/// <param name="filePath"></param>

	void ConvertTextureWICToDDS(const std::string& filePath);
private:
	/// <summary>
	/// テクスチャをWICからDDSに変換する
	/// </summary>
	/// <param name="filePath"></param>
	void LoadWICTextureFromFile(const std::string& filePath);
private:
	/// <summary>
	/// マルチバイト文字列をワイド文字列に変換
	/// </summary>
	/// <param name="mString">マルチバイト文字列</param>
	/// <returns>ワイド文字列</returns>
	static std::wstring ConvertMultiByteStringToWideString(const std::string& mString);
};

