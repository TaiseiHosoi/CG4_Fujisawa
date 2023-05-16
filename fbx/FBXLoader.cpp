﻿#include "FBXLoader.h"

#include <cassert>

using namespace DirectX;


/// <summary>
/// 静的メンバ変数の実体
/// </summary>
const std::string FbxLoader::baseDirectory = "Resources/";
const std::string FbxLoader::defaultTextureFileName = "white1x1.png";

FbxLoader* FbxLoader::GetInstance()
{
	static FbxLoader instance;
	return &instance;
}

void FbxLoader::Initialize(ID3D12Device* device)
{
	// 再初期化チェック
	assert(fbxManager == nullptr);

	// 引数からメンバ変数に代入
	this->device = device;

	// FBXマネージャの生成
	fbxManager = FbxManager::Create();

	// FBXマネージャの入出力設定
	FbxIOSettings* ios = FbxIOSettings::Create(fbxManager, IOSROOT);
	fbxManager->SetIOSettings(ios);

	// FBXインポータの生成
	fbxImporter = FbxImporter::Create(fbxManager, "");
}

void FbxLoader::Finalize()
{
	// 各種FBXインスタンスの破棄
	fbxImporter->Destroy();
	fbxManager->Destroy();
}

FBXModel* FbxLoader::LoadModelFromFile(const string& modelName, bool isSmooth)
{
	// スムージング設定
	smoothing = isSmooth;
	// モデルと同じ名前のフォルダから読み込む
	const string directoryPath = baseDirectory + modelName + "/";
	// 拡張子.fbxを付加
	const string fileName = modelName + ".fbx";
	// 連結してフルパスを得る
	const string fullpath = directoryPath + fileName;

	// ファイル名を指定してFBXファイルを読み込む
	if (!fbxImporter->Initialize(fullpath.c_str(), -1, fbxManager->GetIOSettings())) {
		assert(0);
	}

	// シーン生成
	FbxScene* fbxScene = FbxScene::Create(fbxManager, "fbxScene");

	// ファイルからロードしたFBXの情報をシーンにインポート
	fbxImporter->Import(fbxScene);
	// モデル生成
	FBXModel* fbxmodel = new FBXModel();
	fbxmodel->name = modelName;
	// FBXノードの数を取得
	int nodeCount = fbxScene->GetNodeCount();
	// あらかじめ必要数分のメモリを確保することで、アドレスがずれるのを予防
	fbxmodel->nodes.reserve(nodeCount);
	// ルートノードから順に解析してモデルに流し込む
	ParseNodeRecursive(fbxmodel, fbxScene->GetRootNode());
	// FBXシーン解放
	fbxmodel->fbxScene = fbxScene;
	// バッファ生成
	fbxmodel->CreateBuffers(device);

	return fbxmodel;
}

void FbxLoader::ConvertMatrixFromFbx(DirectX::XMMATRIX* dst, const FbxAMatrix& src)
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			dst->r[i].m128_f32[j] = (float)src.Get(i, j);
		}
	}

}

void FbxLoader::ParseSkin(FBXModel* fbxmodel, FbxMesh* fbxMesh)
{
	FbxSkin* fbxSkin = static_cast<FbxSkin*>(fbxMesh->GetDeformer(0, FbxDeformer::eSkin));

	if (fbxSkin == nullptr) {

		//各頂点について処理
		for (int i = 0; i < fbxmodel->vertices.size(); i++) {
			//最初のボーン(単位行列)の影響を100%にする
			fbxmodel->vertices[i].boneIndex[0] = 0;
			fbxmodel->vertices[i].boneWeight[0] = 1.0f;
		}

		return;
	}

	std::vector<FBXModel::Bone>& bones = fbxmodel->bones;

	int clusterCount = fbxSkin->GetClusterCount();
	bones.reserve(clusterCount);

	for (int i = 0; i < clusterCount; i++) {

		FbxCluster* fbxCluster = fbxSkin->GetCluster(i);

		const char* boneName = fbxCluster->GetLink()->GetName();

		bones.emplace_back(FBXModel::Bone(boneName));
		FBXModel::Bone& bone = bones.back();

		bone.fbxCluster = fbxCluster;

		FbxAMatrix fbxMat;
		fbxCluster->GetTransformLinkMatrix(fbxMat);

		XMMATRIX initialPose;
		ConvertMatrixFromFbx(&initialPose, fbxMat);

		bone.invInitialPose = XMMatrixInverse(nullptr, initialPose);

	}

	struct WeightSet {
		UINT index;
		float weight;
	};

	std::vector<std::list<WeightSet>> weightLists(fbxmodel->vertices.size());
	auto& vertices = fbxmodel->vertices;

	int polygonCount = fbxMesh->GetPolygonCount();

	for (unsigned int deformerIndex = 0; deformerIndex < fbxMesh->GetDeformerCount(); ++deformerIndex)
	{
		for (int i = 0; i < clusterCount; i++) {

			FbxCluster* fbxCluster = fbxSkin->GetCluster(i);
			std::string jointname = fbxCluster->GetLink()->GetName();
			UINT jointIndex = FindJointIndex(jointname);
			int controlPointIndicesCount = fbxCluster->GetControlPointIndicesCount();
			int controlPointCount = fbxMesh->GetControlPointsCount();

			int* controlPointIndices = fbxCluster->GetControlPointIndices();
			double* controlPointWeights = fbxCluster->GetControlPointWeights();



			for (int j = 0; j < controlPointIndicesCount; j++) {
				//
				auto vertexids = meshVerticeControlpoints[fbxCluster->GetControlPointIndices()[j]];

				//for (auto& vertexid : vertexids) {
					if (vertices[j].boneIndex == 0) { *vertices[j].boneIndex = jointIndex; }
					if (vertices[j].boneWeight == 0) { *vertices[j].boneWeight = fbxCluster->GetControlPointWeights()[j]; }

				//}
				//


				/*int vertIndex = controlPointIndices[j];

				float weight = (float)controlPointWeights[j];


				weightLists[vertIndex].emplace_back(WeightSet{ (UINT)i,weight });*/


			}
		}



		//for (int i = 0; i < vertices.size(); i++) {

		//	auto& weightList = weightLists[i];

		//	weightList.sort([](auto const& lhs, auto const& rhs) {

		//		return lhs.weight > rhs.weight;
		//		});

		//	int weightArrayIndex = 0;

		//	for (auto& weightSet : weightList) {

		//		vertices[i].boneIndex[weightArrayIndex] = weightSet.index;
		//		vertices[i].boneWeight[weightArrayIndex] = weightSet.weight;

		//		if (++weightArrayIndex >= FBXModel::MAX_BONE_INDICES) {
		//			float weight = 0.0f;

		//			for (int j = 1; j < FBXModel::MAX_BONE_INDICES; j++) {
		//				weight += vertices[i].boneWeight[j];
		//			}
		//			vertices[i].boneWeight[0] = 1.0f - weight;
		//			break;
		//		}

		//	}

		//}
	}
	int aaa = fbxMesh->GetPolygonCount();
	int z = 0;
	if (z == 0) {};

}

// 法線、UV情報が存在しているか？
bool FbxLoader::IsExistNormalUVInfo(const std::vector<float>& vertexInfo)
{
	return vertexInfo.size() == 8; // 頂点3 + 法線3 + UV2
}

// 頂点情報を生成
std::vector<float> FbxLoader::CreateVertexInfo(const std::vector<float>& vertexInfo, const FbxVector4& normalVec4, const FbxVector2& uvVec2)
{
	std::vector<float> newVertexInfo;
	// 位置座標
	newVertexInfo.push_back(vertexInfo[0]);
	newVertexInfo.push_back(vertexInfo[1]);
	newVertexInfo.push_back(vertexInfo[2]);
	// 法線座標
	newVertexInfo.push_back(normalVec4[0]);
	newVertexInfo.push_back(normalVec4[1]);
	newVertexInfo.push_back(normalVec4[2]);
	// UV座標
	newVertexInfo.push_back(uvVec2[0]);
	newVertexInfo.push_back(uvVec2[1]);
	return newVertexInfo;
}

// 新たな頂点インデックスを生成する
int FbxLoader::CreateNewVertexIndex(const std::vector<float>& vertexInfo, const FbxVector4& normalVec4, const FbxVector2& uvVec2,
	std::vector<std::vector<float>>& vertexInfoList, int oldIndex, std::vector<std::array<int, 2>>& oldNewIndexPairList)
{
	//// 作成済の場合、該当のインデックスを返す
	//for (int i = 0; i < oldNewIndexPairList.size(); i++)
	//{
	//	int newIndex = oldNewIndexPairList[i][1];
	//	if (oldIndex == oldNewIndexPairList[i][0]
	//		&& IsSetNormalUV(vertexInfoList[newIndex], normalVec4, uvVec2))
	//	{
	//		return newIndex;
	//	}
	//}
	// 作成済でない場合、新たな頂点インデックスとして作成
	std::vector<float> newVertexInfo = CreateVertexInfo(vertexInfo, normalVec4, uvVec2);
	vertexInfoList.push_back(newVertexInfo);
	// 作成したインデックス情報を設定
	int newIndex = vertexInfoList.size() - 1;
	std::array<int, 2> oldNewIndexPair{ oldIndex , newIndex };
	oldNewIndexPairList.push_back(oldNewIndexPair);
	return newIndex;
}

// vertexInfoに法線、UV座標が設定済かどうか？
bool FbxLoader::IsSetNormalUV(const std::vector<float> vertexInfo, const FbxVector4& normalVec4, const FbxVector2& uvVec2)
{
	// 法線、UV座標が同値なら設定済とみなす
	return fabs(vertexInfo[3] - normalVec4[0]) < FLT_EPSILON
		&& fabs(vertexInfo[4] - normalVec4[1]) < FLT_EPSILON
		&& fabs(vertexInfo[5] - normalVec4[2]) < FLT_EPSILON
		&& fabs(vertexInfo[6] - uvVec2[0]) < FLT_EPSILON
		&& fabs(vertexInfo[7] - uvVec2[1]) < FLT_EPSILON;
}

unsigned int FbxLoader::FindJointIndex(const std::string& jointname)
{
	/*for (unsigned int i = 0; i < skeleton.mJoints.size(); ++i)
	{
		if (skeleton.mJoints[i].mName == jointname)
		{
			return i;
		}
	}*/
	return 0;
}

void FbxLoader::ParseNodeRecursive(FBXModel* fbxmodel, FbxNode* fbxNode, Node* parent)
{
	// ノード名を取得
	string name = fbxNode->GetName();

	// モデルにノードを追加
	fbxmodel->nodes.emplace_back();
	Node& node = fbxmodel->nodes.back();
	// ノード名を取得
	node.name = fbxNode->GetName();

	// FBXノードのローカル移動情報
	FbxDouble3 rotation = fbxNode->LclRotation.Get();
	FbxDouble3 scaling = fbxNode->LclScaling.Get();
	FbxDouble3 translation = fbxNode->LclTranslation.Get();

	// 形式変換して代入
	node.rotation = { (float)rotation[0], (float)rotation[1], (float)rotation[2], 0.0f };
	node.scaling = { (float)scaling[0], (float)scaling[1], (float)scaling[2], 0.0f };
	node.translation = { (float)translation[0], (float)translation[1], (float)translation[2], 1.0f };

	//回転角をDegree(度)からラジアンに変換
	node.rotation.m128_f32[0] = XMConvertToRadians(node.rotation.m128_f32[0]);
	node.rotation.m128_f32[1] = XMConvertToRadians(node.rotation.m128_f32[1]);
	node.rotation.m128_f32[2] = XMConvertToRadians(node.rotation.m128_f32[2]);

	// スケール、回転、平行移動行列の計算
	XMMATRIX matScaling, matRotation, matTranslation;
	matScaling = XMMatrixScalingFromVector(node.scaling);
	matRotation = XMMatrixRotationRollPitchYawFromVector(node.rotation);
	matTranslation = XMMatrixTranslationFromVector(node.translation);

	// ローカル変形行列の計算
	node.transform = XMMatrixIdentity();
	node.transform *= matScaling; // ワールド行列にスケーリングを反映
	node.transform *= matRotation; // ワールド行列に回転を反映
	node.transform *= matTranslation; // ワールド行列に平行移動を反映

	//グローバル変形行列の計算
	node.globalTransform = node.transform;
	if (parent) {
		node.parent = parent;
		// 親の変形を乗算
		node.globalTransform *= parent->globalTransform;
	}

	// FBXノードのメッシュ情報を解析
	FbxNodeAttribute* fbxNodeAttribute = fbxNode->GetNodeAttribute();

	if (fbxNodeAttribute) {
		if (fbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			fbxmodel->meshNode = &node;
			ParseMesh(fbxmodel, fbxNode);
		}
	}


	// 子ノードに対して再帰呼び出し
	for (int i = 0; i < fbxNode->GetChildCount(); i++) {
		ParseNodeRecursive(fbxmodel, fbxNode->GetChild(i), &node);
	}
}

void FbxLoader::ParseMesh(FBXModel* fbxmodel, FbxNode* fbxNode)
{
	// ノードのメッシュを取得
	FbxMesh* fbxMesh = fbxNode->GetMesh();

	// 頂点座標読み取り
	ParseMeshVertices(fbxmodel, fbxMesh);
	// マテリアルの読み取り
	ParseMaterial(fbxmodel, fbxNode);

	//スキニング情報の読み取り
	ParseSkin(fbxmodel, fbxMesh);
}

void FbxLoader::ParseMeshVertices(FBXModel* fbxmodel, FbxMesh* fbxMesh)
{
	auto& vertices = fbxmodel->vertices;
	auto& indices = fbxmodel->indices;

	// 頂点座標データの数
	const int controlPointsCount = fbxMesh->GetControlPointsCount();
	// 必要数だけ頂点データ配列を確保
	//FBXModel::VertexPosNormalUv vert{};
	//fbxmodel->vertices.resize(controlPointsCount, vert);

	//// FBXメッシュの頂点座標配列を取得
	//FbxVector4* pCoord = fbxMesh->GetControlPoints();

	//// FBXメッシュの全頂点座標をモデル内の配列にコピーする。
	//for (int i = 0; i < controlPointsCount; i++) {
	//	FBXModel::VertexPosNormalUv& vertex = vertices[i];
	//	// 座標のコピー
	//	vertex.pos.x = (float)pCoord[i][0];
	//	vertex.pos.y = (float)pCoord[i][1];
	//	vertex.pos.z = (float)pCoord[i][2];
	//}

	FbxStringList uvSetNameList;
	fbxMesh->GetUVSetNames(uvSetNameList);
	const char* uvSetName = uvSetNameList.GetStringAt(0);

	// 頂点座標情報のリストを生成
	std::vector<std::vector<float>> vertexInfoList;
	for (int i = 0; i < fbxMesh->GetControlPointsCount(); i++)
	{
		// 頂点座標を読み込んで設定
		auto point = fbxMesh->GetControlPointAt(i);
		std::vector<float> vertex;
		vertex.push_back(point[0]);
		vertex.push_back(point[1]);
		vertex.push_back(point[2]);
		vertexInfoList.push_back(vertex);
	}



	// 頂点毎の情報を取得する
	std::vector<std::array<int, 2>> oldNewIndexPairList;
	for (int polIndex = 0; polIndex < fbxMesh->GetPolygonCount(); polIndex++) // ポリゴン毎のループ
	{
		/*int polNum = fbxMesh->GetPolygonSize(polIndex);
		assert(polNum == 3);*/
		FbxVector4 v1, v2, v3, faceNorm;
		if (smoothing == true) {
			
			fbxMesh->GetPolygonVertexNormal(polIndex, 0, v1);
			fbxMesh->GetPolygonVertexNormal(polIndex, 1, v2);
			fbxMesh->GetPolygonVertexNormal(polIndex, 2, v3);
			
			faceNorm = {
				(v2.mData[1] - v1.mData[1]) * (v3.mData[2] - v1.mData[2]) - (v2.mData[2] - v1.mData[2]) * (v3.mData[1] - v1.mData[1]),
				(v2.mData[2] - v1.mData[2]) * (v3.mData[0] - v1.mData[0]) - (v2.mData[0] - v1.mData[0]) * (v3.mData[2] - v1.mData[2]),
				(v2.mData[0] - v1.mData[0]) * (v3.mData[1] - v1.mData[1]) - (v2.mData[1] - v1.mData[1]) * (v3.mData[0] - v1.mData[0]),
				0
			};
		}

		

		for (int polVertexIndex = 0; polVertexIndex < 3; polVertexIndex++) // 頂点毎のループ
		{
			// インデックス座標
			auto vertexIndex = fbxMesh->GetPolygonVertex(polIndex, polVertexIndex);

			// 頂点座標
			std::vector<float> vertexInfo = vertexInfoList[vertexIndex];

			// 法線座標
			FbxVector4 normalVec4;
			fbxMesh->GetPolygonVertexNormal(polIndex, polVertexIndex, normalVec4);

			if (smoothing == true) {
				normalVec4 += faceNorm;
				float len = normalVec4.Length();

				if (len != 0.0f) {
					normalVec4 /= len;
				}

			}
			
			
			// UV座標
			FbxVector2 uvVec2;
			bool isUnMapped;
			fbxMesh->GetPolygonVertexUV(polIndex, polVertexIndex, uvSetName, uvVec2, isUnMapped);

			////ParceSkin
			//FbxSkin* fbxSkin = static_cast<FbxSkin*>(fbxMesh->GetDeformer(0, FbxDeformer::eSkin));

			//if (fbxSkin == nullptr) {

			//	//各頂点について処理
			//	for (int i = 0; i < fbxmodel->vertices.size(); i++) {
			//		//最初のボーン(単位行列)の影響を100%にする
			//		fbxmodel->vertices[i].boneIndex[0] = 0;
			//		fbxmodel->vertices[i].boneWeight[0] = 1.0f;
			//	}

			//	return;
			//}

			//std::vector<FBXModel::Bone>& bones = fbxmodel->bones;

			//int clusterCount = fbxSkin->GetClusterCount();
			//bones.reserve(clusterCount);

			//for (int i = 0; i < clusterCount; i++) {

			//	FbxCluster* fbxCluster = fbxSkin->GetCluster(i);

			//	const char* boneName = fbxCluster->GetLink()->GetName();

			//	bones.emplace_back(FBXModel::Bone(boneName));
			//	FBXModel::Bone& bone = bones.back();

			//	bone.fbxCluster = fbxCluster;

			//	FbxAMatrix fbxMat;
			//	fbxCluster->GetTransformLinkMatrix(fbxMat);

			//	XMMATRIX initialPose;
			//	ConvertMatrixFromFbx(&initialPose, fbxMat);

			//	bone.invInitialPose = XMMatrixInverse(nullptr, initialPose);

			//}

			//struct WeightSet {
			//	UINT index;
			//	float weight;
			//};

			//std::vector<std::list<WeightSet>> weightLists(fbxMesh->GetControlPointsCount() * 3);
			////auto& vertices = fbxmodel->vertices;

			//for (int i = 0; i < clusterCount; i++) {

			//	FbxCluster* fbxCluster = fbxSkin->GetCluster(i);
			//	std::string jointname = fbxCluster->GetLink()->GetName();
			//	UINT jointIndex = FindJointIndex(jointname);
			//	int controlPointIndicesCount = fbxCluster->GetControlPointIndicesCount();
			//	int* controlPointIndices = fbxCluster->GetControlPointIndices();
			//	double* controlPointWeights = fbxCluster->GetControlPointWeights();



			//	for (int j = 0; j < controlPointIndicesCount; j++) {
			//		//
			//		auto vertexids = meshVerticeControlpoints[fbxCluster->GetControlPointIndices()[j]];

			//		/*for (auto& vertexid : vertexids) {
			//			if (vertices[vertexid].boneIndex == 0) { *vertices[vertexid].boneIndex = jointIndex; }
			//			if (vertices[vertexid].boneWeight == 0) { *vertices[vertexid].boneWeight = fbxCluster->GetControlPointWeights()[i]; }

			//		}*/
			//		//
			//		int vertIndex = controlPointIndices[j];

			//		float weight = (float)controlPointWeights[j];

			//		for (int k = 0; k < 3; k++) {
			//			weightLists[vertIndex].emplace_back(WeightSet{ (UINT)i,weight });
			//		}
			//		

			//	}
			//}



			//for (int i = 0; i < fbxMesh->GetPolygonCount() * 3; i++) {

			//	auto& weightList = weightLists[i];

			//	weightList.sort([](auto const& lhs, auto const& rhs) {

			//		return lhs.weight > rhs.weight;
			//		});

			//	int weightArrayIndex = 0;

			//	for (auto& weightSet : weightList) {

			//		vertices[i].boneIndex[weightArrayIndex] = weightSet.index;
			//		vertices[i].boneWeight[weightArrayIndex] = weightSet.weight;

			//		if (++weightArrayIndex >= FBXModel::MAX_BONE_INDICES) {
			//			float weight = 0.0f;

			//			for (int j = 1; j < FBXModel::MAX_BONE_INDICES; j++) {
			//				weight += vertices[i].boneWeight[j];
			//			}
			//			vertices[i].boneWeight[0] = 1.0f - weight;
			//			break;
			//		}

			//	}

			//}

			// インデックス座標のチェックと再採番
			if (vertexInfo.size() != 8)// 頂点3 + 法線3 + UV2
			{
				// 法線座標とUV座標が未設定の場合、頂点情報に付与して再設定
				 
				std::vector<float> newVertexInfo;
				std::vector<int>newBoneIndexInfo;
				std::vector<float>newBoneWeightInfo;
				// 位置座標
				newVertexInfo.push_back(vertexInfo[0]);
				newVertexInfo.push_back(vertexInfo[1]);
				newVertexInfo.push_back(vertexInfo[2]);
				

				newVertexInfo.push_back(normalVec4[0]);
				newVertexInfo.push_back(normalVec4[1]);
				newVertexInfo.push_back(normalVec4[2]);

				
				// UV座標
				newVertexInfo.push_back(uvVec2[0]);
				newVertexInfo.push_back(uvVec2[1]);

				

				vertexInfoList[vertexIndex] = newVertexInfo;
			}
			else
			{
				// ＊同一頂点インデックスの中で法線座標かUV座標が異なる場合、
				// 新たな頂点インデックスとして作成する

				vertexIndex = CreateNewVertexIndex(vertexInfo, normalVec4, uvVec2, vertexInfoList, vertexIndex, oldNewIndexPairList);
			}

			int controlPointIndex = fbxMesh->GetPolygonVertex(polIndex, polVertexIndex);
			meshVerticeControlpoints[controlPointIndex].push_back(indices.size());
			// インデックス座標を設定
			indices.push_back(vertexIndex);
		}
	}

	meshVertice.resize(vertexInfoList.size());
	// 頂点情報を生成
	for (int i = 0; i < vertexInfoList.size(); i++)
	{
		std::vector<float> vertexInfo = vertexInfoList[i];
		vertices.push_back(FBXModel::VertexPosNormalUv{
			{
				vertexInfo[0], vertexInfo[1], vertexInfo[2]
			},
			{
				vertexInfo[3], vertexInfo[4], vertexInfo[5]
			},
			{
				vertexInfo[6], 1.0f - vertexInfo[7] 
			}
		});


	}

	
	

}

void FbxLoader::ParseMeshFaces(FBXModel* fbxmodel, FbxMesh* fbxMesh)
{
	auto& vertices = fbxmodel->vertices;
	auto& indices = fbxmodel->indices;

	// 1ファイルに複数メッシュのモデルは非対応
	assert(indices.size() == 0);

	// 面の数
	const int polygonCount = fbxMesh->GetPolygonCount();
	// UVデータの数
	const int textureUVCount = fbxMesh->GetTextureUVCount();
	// UV名リスト
	FbxStringList uvNames;
	fbxMesh->GetUVSetNames(uvNames);

	// 面ごとの情報読み取り
	for (int i = 0; i < polygonCount; i++) {
		// 面を構成する頂点の数を取得（3なら三角形ポリゴン)
		const int polygonSize = fbxMesh->GetPolygonSize(i);
		assert(polygonSize <= 32);

		// 1頂点ずつ処理
		for (int j = 0; j < polygonSize; j++) {
			// FBX頂点配列のインデックス
			int index = fbxMesh->GetPolygonVertex(i, j);
			assert(index >= 0);

			// 頂点法線読込
			FBXModel::VertexPosNormalUv& vertex = vertices[index];
			FbxVector4 normal;
			if (fbxMesh->GetPolygonVertexNormal(i, j, normal)) {
				vertex.normal.x = (float)normal[0];
				vertex.normal.y = (float)normal[1];
				vertex.normal.z = (float)normal[2];
			}
			

			// テクスチャUV読込
			if (textureUVCount > 0) {
				FbxVector2 uvs;
				bool lUnmappedUV;
				// 0番決め打ちで読込
				if (fbxMesh->GetPolygonVertexUV(i, j, uvNames[0], uvs, lUnmappedUV)) {
					vertex.uv.x = (float)uvs[0];
					vertex.uv.y = (float)uvs[1];
				}
			}


			// インデックス配列に頂点インデックス追加
			// 3頂点目までなら
			if (j < 3) {
				// 1点追加し、他の2点と三角形を構築する
				indices.push_back(index);
			}
			// 4頂点目
			else {
				// 3点追加し、四角形の0,1,2,3の内 2,3,0で三角形を構築する
				int index2 = indices[indices.size() - 1];
				int index3 = index;
				int index0 = indices[indices.size() - 3];
				indices.push_back(index2);
				indices.push_back(index3);
				indices.push_back(index0);
			}
		}
	}

	

}

void FbxLoader::ParseMaterial(FBXModel* fbxmodel, FbxNode* fbxNode)
{
	const int materialCount = fbxNode->GetMaterialCount();
	if (materialCount > 0) {
		// 先頭のマテリアルを取得
		FbxSurfaceMaterial* material = fbxNode->GetMaterial(0);

		bool textureLoaded = false;

		if (material) {

			// FbxSurfaceLambertクラスかどうかを調べる
			if (material->GetClassId().Is(FbxSurfaceLambert::ClassId))
			{
				FbxSurfaceLambert* lambert = static_cast<FbxSurfaceLambert*>(material);

				//環境光係数
				FbxPropertyT<FbxDouble3> ambient = lambert->Ambient;
				fbxmodel->ambient.x = (float)ambient.Get()[0];
				fbxmodel->ambient.y = (float)ambient.Get()[1];
				fbxmodel->ambient.z = (float)ambient.Get()[2];

				//拡散反射光係数
				FbxPropertyT<FbxDouble3> diffuse = lambert->Diffuse;
				fbxmodel->diffuse.x = (float)diffuse.Get()[0];
				fbxmodel->diffuse.y = (float)diffuse.Get()[1];
				fbxmodel->diffuse.z = (float)diffuse.Get()[2];
			}

			// ディフューズテクスチャを取り出す
			const FbxProperty diffuseProperty = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
			if (diffuseProperty.IsValid())
			{
				const FbxFileTexture* texture = diffuseProperty.GetSrcObject<FbxFileTexture>();
				if (texture) {
					const char* filepath = texture->GetFileName();
					// ファイルパスからファイル名抽出
					string path_str(filepath);
					string name = ExtractFileName(path_str);
					// テクスチャ読み込み
					LoadTexture(fbxmodel, baseDirectory + fbxmodel->name + "/" + name);
					textureLoaded = true;
				}
			}
		}

		// テクスチャがない場合は白テクスチャを貼る
		if (!textureLoaded) {
			LoadTexture(fbxmodel, baseDirectory + defaultTextureFileName);
		}
	}
}

void FbxLoader::LoadTexture(FBXModel* fbxmodel, const std::string& fullpath)
{
	HRESULT result = S_FALSE;

	// WICテクスチャのロード
	TexMetadata& metadata = fbxmodel->metadata;
	ScratchImage& scratchImg = fbxmodel->scratchImg;

	// ユニコード文字列に変換
	wchar_t wfilepath[128];
	MultiByteToWideChar(CP_ACP, 0, fullpath.c_str(), -1, wfilepath, _countof(wfilepath));

	result = LoadFromWICFile(
		wfilepath, WIC_FLAGS_NONE,
		&metadata, scratchImg);
	if (FAILED(result)) {
		assert(0);
	}
}

std::string FbxLoader::ExtractFileName(const std::string& path)
{
	size_t pos1;
	// 区切り文字 '\\' が出てくる一番最後の部分を検索
	pos1 = path.rfind('\\');
	if (pos1 != string::npos) {
		return path.substr(pos1 + 1, path.size() - pos1 - 1);
	}
	// 区切り文字 '/' が出てくる一番最後の部分を検索
	pos1 = path.rfind('/');
	if (pos1 != string::npos) {
		return path.substr(pos1 + 1, path.size() - pos1 - 1);
	}

	return path;
}


