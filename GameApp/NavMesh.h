#pragma once
#include <GameEngine/GameEngineComponent.h>

class NavFaceData
{
	friend class NavFace;
	friend class NavMesh;
	friend class NavActor;
	friend class LumiaMap;
public:
	NavFaceData()
		: Index(0)
	{

	}
	~NavFaceData()
	{

	}

	float4 Vertex[3];
	int Index;
	std::vector<UINT> Link;
};

class NavFace
{
	friend class NavMesh;
public:
	NavFace()
		: AllNavi(nullptr)
		, Parent(nullptr)
	{

	}
	~NavFace()
	{

	}

	// _Transform 에서 NaviMesh 사이의 거리를 반환한다.
	float YCheck(GameEngineTransform* _Transform);

	// NaviActor가 가지고있는 currentNavFace_ 바깥으로 나갔는지 여부를 체크한다.
	bool OutCheck(GameEngineTransform* _Transform)
	{
		float Dist = 0.0f;
		return OutCheck(_Transform, Dist);
	}

	NavFace* MoveCheck(GameEngineTransform* _Transform);

	NavFaceData GetInfo()
	{
		return info;
	}

private:
	bool OutCheck(GameEngineTransform* _Transform, float& _Dist);

	NavFaceData info;
	std::vector<NavFace>* AllNavi;
	NavMesh* Parent;
};

class NavMesh : public GameEngineComponent
{
public:
	NavMesh();
	~NavMesh();

	NavMesh(const NavMesh& _other) = delete; 
	NavMesh(NavMesh&& _other) noexcept = delete;
	NavMesh& operator=(const NavMesh& _other) = delete;
	NavMesh& operator=(const NavMesh&& _other) = delete;

	void CreateNaviMesh(const std::vector<GameEngineVertex>& _Vertex, const std::vector<UINT>& _Index);

	// 유저 세이브 파일이 없다면 만들고, 있다면 UserLoad()를 실행한다. 
	void CreateNaviMesh(GameEngineFBXRenderer* _FBXRenderer, const std::string& _FileName = "Default");

	NavFace* CurrentCheck(GameEngineTransform* _Transform, const float4& _Dir);
	NavFace* GetNavFaceFromPositionXZ(const float4& _position, const float4& _direction, float& _height);

	void SetColor(float4 _Color)
	{
		Color = _Color;
	}

	void SetNaviRenderer(GameEngineRendererBase* _Renderer)
	{
		NaviRenderer = _Renderer;
	}

	GameEngineRendererBase* GetNaviRenderer()
	{
		return NaviRenderer;
	}

	bool CheckIntersects(const float4& _Position, const float4& _Direction, float& _Distance);

	float4 GetMousePos();

	bool GetIntersectionPointFromMouseRay(float4& _out);

	bool IsMouseIntersects();

	std::vector<NavFace> GetAllNavi()
	{
		return Navis;
	}

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	void SaveNavisData(const std::vector<FbxMeshSet>& _AllMesh);

	void UserLoad(const std::string& _Path);
	void UserSave(const std::string& _Path);

	bool LinkCheck(const NavFace& _Left, const NavFace& _Right);

	float4 CalculateCameraDir();

private:
	GameEngineDirectory NaviMeshFolder;

	std::vector<NavFace> Navis;

	GameEngineRendererBase* NaviRenderer;

	GameEngineVertexBuffer* NewVertex;
	GameEngineIndexBuffer* NewIndex;

	float4 Color;
};

