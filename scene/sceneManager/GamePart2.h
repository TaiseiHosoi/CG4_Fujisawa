#pragma once
#include "IScene.h"
#include "SceneManager.h"

// É{ÉX êÌì¨1
class GamePart2 :
    public IScene
{
protected:
	SceneManager* _controller;

public:
	GamePart2(SceneManager* controller);
	~GamePart2() override;

	void Initialize(DirectXCommon* dxCommon, GameCamera* camera) override;
	void Update(Input* input, GameCamera* camera) override;
	void Draw(DirectXCommon* dxCommon) override;

private:


public:

};

