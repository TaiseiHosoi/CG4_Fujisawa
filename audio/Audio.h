#pragma once
#include <xaudio2.h>
#include <fstream>
#include <wrl.h>
#include <map>
#include <string>
#include <cassert>

class Audio
{
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	//チャンクヘッダ
	struct ChunkHeader
	{
		char id[4];		//チャンクごとのid
		int32_t size;	//チャンクサイズ
	};

	//RIFFヘッダチャンク
	struct RiffHeader
	{
		ChunkHeader chunk;	//"RIFF"
		char type[4];		//"WAVE"
	};
	//FMTチャンク
	struct FormatChunk
	{
		ChunkHeader chunk;	//"fmt"
		WAVEFORMATEX fmt;	//波形フォーマット
	};
	//音声データ
	struct SoundData
	{
		//波形フォーマット
		WAVEFORMATEX wfex;
		//バッファの先頭アドレス
		BYTE* pBuffer;
		//バッファのサイズ
		unsigned int bufferSize;
	};
public:
	//初期化
	void Initialize(const std::string directoryPath = "Resources/");

	//終了処理
	void Finalize();

	/// <summary>
	/// WAV音声読み込み
	/// </summary>
	/// <param name="filename">WAVファイル名</param>
	void LoadWave(const std::string filename);


	/// <summary>
	///	サウンドデータの開放
	/// </summary>
	void Unload(SoundData* soundData);

	/// <summary>
	/// サウンドデータ名
	/// </summary>
	/// <param name="filename">WAVファイル名</param>
	void PlayWave(const std::string filename);

private:
	//xAudioのインスタンス
	ComPtr<IXAudio2>xAudio2_;
	//サウンドデータの配列
	std::map<std::string, SoundData> soundDatas_;

	//サウンド格納ディレクトリ
	std::string directoryPath_;
};