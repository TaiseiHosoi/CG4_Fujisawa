#pragma once
#include <Windows.h>

class WinApp
{
public:// 静的メンバ関数
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
public:
	// 初期化
	void Initialize();

	// 更新
	bool ProcessMessage();

	//ウィンドウクラスを登録解除
	void Finalize();

	// getter
	HWND GetHwnd()const { return hwnd; }
	HINSTANCE GetHInstance() const { return w.hInstance; }


private:
	// ウィンドウハンドル
	HWND hwnd = nullptr;
	//ウィンドウクラス
	WNDCLASSEX w{};


public:

	//ウィンドウサイズ
	static const int window_width = 1280;	//横幅
	static const int window_height = 720;	//縦幅
};

