#include "Model.h"
#include "Direct3D.h"

//ネームスペースは別のところからでも追加できる余！！
namespace Model {
	std::vector<ModelData*> modelList;
}

int Model::Load(std::string filename)
{

	ModelData* pData = new ModelData;

	pData->filename_ = filename;

	for (auto& e : modelList) {
		//同名のファイルがあった場合新しく保存しない
		if (e->filename_ == filename) {
			pData->pFbx_ = e->pFbx_;
			break;
		}
	}
	//同名のファイルがなかった場合
	if (pData->pFbx_ == nullptr) {
		pData->pFbx_ = new FBX;
		pData->pFbx_->Load(filename);
	}

	modelList.push_back(pData);

	return modelList.size() - 1;

}

void Model::SetTransform(int hModel, Transform transform)
{
	modelList[hModel]->transform_ = transform;
}

void Model::Draw(int hModel)
{
	modelList[hModel]->pFbx_->Draw(modelList[hModel]->transform_);
}

void Model::Release()
{
	bool isReferred = false;
	//モデルの番号を調べて参照されているのがあれば消していく
	for (int j = 0; j < modelList.size(); j++) {
		for (int i = j + 1; i < modelList.size(); i++) {
			if (modelList[j]->pFbx_ == modelList[i]->pFbx_)
			{
				isReferred = true;
				break;
			}
		}
		if (!isReferred) {
			SAFE_DELETE(modelList[j]->pFbx_);;
		}
		SAFE_DELETE(modelList[j]);
	}
	modelList.clear();
}
