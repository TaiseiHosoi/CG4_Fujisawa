#pragma once
#include "IScene.h"

// í«Ç¢Ç©ÇØÉpÅ[Ég
class GamePart1 :
    public IScene
{
protected:
	SceneManager* _controller;

public:
	GamePart1(SceneManager* controller);
	~GamePart1() override;

	void Initialize(DirectXCommon* dxCommon, GameCamera* camera) override;
	void Update(Input* input, GameCamera* camera) override;
	void Draw(DirectXCommon* dxCommon) override;

private:


public:

};

