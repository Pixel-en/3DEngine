#pragma once
#include "GameObject.h"

enum SCENE_ID
{
	SCENE_ID_TEST=0,
	SCENE_ID_PLAY,
	SCENE_ID_END,
	SCENE_ID_MAX,
};

class SceneManager :
	public GameObject
{
	SCENE_ID currentSceneID_;
	SCENE_ID nextSceneID_;
public:

	SceneManager(GameObject* parent);
	~SceneManager();


	void ChangeScene(SCENE_ID nextScene) {
		nextSceneID_ = nextScene;
	}


	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

};

