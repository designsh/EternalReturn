#include "PreCompile.h"
#include "NavMesh.h"

#include "GameEngine/GameEngineRendererBase.h"

NavMesh::NavMesh()
	: NaviRenderer(nullptr)
	, NewVertex(nullptr)
	, NewIndex(nullptr)
	, Color(float4::BLACK)
{
	NewVertex = new GameEngineVertexBuffer();
	NewIndex = new GameEngineIndexBuffer();

	NaviMeshFolder.MoveParent("EternalReturn");
	NaviMeshFolder.MoveChild("Resources");
	NaviMeshFolder.MoveChild("FBX");
	NaviMeshFolder.MoveChild("UserMesh");
	NaviMeshFolder.MoveChild("Map");
}

NavMesh::~NavMesh()
{
	delete NewVertex;
	delete NewIndex;
}

void NavMesh::CreateNaviMesh(const std::vector<GameEngineVertex>& _Vertex, 
	const std::vector<UINT>& _Index)
{
	Navis.resize(_Index.size() / 3);

	for (UINT i = 0; i < Navis.size(); i++)
	{
		Navis[i].info.Vertex[0] = _Vertex[_Index[i * 3 + 0]].POSITION;
		Navis[i].info.Vertex[1] = _Vertex[_Index[i * 3 + 1]].POSITION;
		Navis[i].info.Vertex[2] = _Vertex[_Index[i * 3 + 2]].POSITION;

		Navis[i].info.Index = i;
		Navis[i].AllNavi = &Navis;
		Navis[i].Parent = this;
	}

	// Link 정보 세팅
	for (UINT i = 0; i < Navis.size(); i++)
	{
		NavFace& Navi = Navis[i];

		for (UINT j = 0; j < Navis.size(); j++)
		{
			if (i == j)
			{
				continue;
			}

			if (LinkCheck(Navi, Navis[j]))
			{
				Navi.info.Link.push_back(j);
			}
		}
	}

	NewVertex->Create(_Vertex, D3D11_USAGE::D3D11_USAGE_DEFAULT);
	NewIndex->Create(_Index, D3D11_USAGE::D3D11_USAGE_DEFAULT);

	/*GameEngineRenderer* Renderer = CreateTransformComponent<GameEngineRenderer>(GetTransform());
	Renderer->SetRenderingPipeLine("Color");
	Renderer->SetMesh(NewVertex, NewIndex);
	Renderer->ShaderHelper.SettingConstantBufferLink("ResultColor", Color);
	NaviRenderer = Renderer;*/
}

void NavMesh::CreateNaviMesh(GameEngineFBXRenderer* _FBXRenderer, 
	const std::string& _FileName /*= "Default"*/)
{
	std::vector<FbxMeshSet>& AllMesh = _FBXRenderer->GetMesh()->GetAllMeshMap();

	std::string FileName = _FileName;
	FileName.append(".NaviMesh");

	std::vector<GameEngineFile> vecFile = NaviMeshFolder.GetAllFile(".NaviMesh");

	bool isLoaded = false;

	for(GameEngineFile& File : vecFile)
	{
		if (std::string::npos != File.GetFileName().find(_FileName))
		{
			UserLoad(File.GetFullPath());
			isLoaded = true;
		}
	}

	if (false == isLoaded)
	{
		SaveNavisData(AllMesh);
		UserSave(NaviMeshFolder.PathToPlusFileName(FileName));
	}

	NaviRenderer = _FBXRenderer;
}

NavFace* NavMesh::CurrentCheck(GameEngineTransform* _Transform, const float4& _Dir)
{
	if (nullptr == NaviRenderer)
	{
		GameEngineDebug::MsgBoxError("NaviRenderer가 지정되지 않았습니다.");
	}

	float4 RayPos = _Transform->GetWorldPosition() + float4(0.0f, 100.0f, 0.0f);
	float Dist = 0.0f;

	bool Check = false;

	for (auto& Navi : Navis)
	{
		float4 V0 = Navi.info.Vertex[0] * NaviRenderer->GetTransform()->GetTransformData().WorldWorld_;
		float4 V1 = Navi.info.Vertex[1] * NaviRenderer->GetTransform()->GetTransformData().WorldWorld_;
		float4 V2 = Navi.info.Vertex[2] * NaviRenderer->GetTransform()->GetTransformData().WorldWorld_;

		Check = DirectX::TriangleTests::Intersects(RayPos.DirectVector,
			_Dir.DirectVector,
			V0.DirectVector,
			V1.DirectVector,
			V2.DirectVector,
			Dist);

		if (true == Check)
		{
			return &Navi;
		}
	}

	return nullptr;
}

NavFace* NavMesh::GetNavFaceFromPositionXZ(const float4& _position, const float4& _direction, float& _height)
{
	if (nullptr == NaviRenderer)
	{
		GameEngineDebug::MsgBoxError("NaviRenderer가 지정되지 않았습니다.");
	}

	float4 rayPosition = _position;
	rayPosition.y = FT::Map::MAX_HEIGHT;
	float height = 0.0f;

	bool result = false;
	for (auto& face : Navis)
	{
		float4x4 matWorld = NaviRenderer->GetTransform()->GetTransformData().WorldWorld_;

		float4 V0 = face.info.Vertex[0] * matWorld;

		if (4000.f < float4::Calc_Len3D(V0, _position))
		{
			continue;
		}

		float4 V1 = face.info.Vertex[1] * matWorld;
		float4 V2 = face.info.Vertex[2] * matWorld;

		result = DirectX::TriangleTests::Intersects(rayPosition.DirectVector,
			_direction.DirectVector,
			V0.DirectVector,
			V1.DirectVector,
			V2.DirectVector,
			_height);

		_height = FT::Map::MAX_HEIGHT - _height;

		if (result)
		{
			return &face;
		}
	}

	return nullptr;
}

bool NavMesh::CheckIntersects(const float4& _Position, const float4& _Direction, float& _Distance)
{
	bool Check = false;

	if (nullptr == NaviRenderer)
	{
		GameEngineDebug::MsgBoxError("NaviRenderer가 지정되지 않았습니다.");
	}

	float4 Dir = _Direction.NormalizeReturn3D();

	float4x4 matWorld = NaviRenderer->GetTransform()->GetTransformData().WorldWorld_;

	for (size_t i = 0; i < Navis.size(); i++)
	{
		float4 V0 = Navis[i].info.Vertex[0] * matWorld;
		float4 V1 = Navis[i].info.Vertex[1] * matWorld;
		float4 V2 = Navis[i].info.Vertex[2] * matWorld;

		Check = DirectX::TriangleTests::Intersects(_Position.DirectVector,
			Dir.DirectVector,
			V0.DirectVector,
			V1.DirectVector,
			V2.DirectVector,
			_Distance);

		if (true == Check)
		{
			return true;
		}
	}

	return false;
}

float4 NavMesh::CalculateCameraDir()
{
	float4 MousePos = GameEngineInput::GetInst().GetMousePos();

	UINT ViewPortNo = 1;
	D3D11_VIEWPORT ViewPort_ = {};
	GameEngineDevice::GetInst().GetContext()->RSGetViewports(&ViewPortNo, &ViewPort_);

	if (0 == ViewPortNo)
	{
		return float4::ZERO;
	}

	float4 Dir;

	Dir.x = ((2.0f * MousePos.x) / ViewPort_.Width) - 1.0f;
	Dir.y = (((2.0f * MousePos.y) / ViewPort_.Height) - 1.0f) * -1.0f;
	Dir.z = 1.0f;
	Dir.w = 0.0f;

	// 2. 광선을 투영영역 -> 뷰영역
	float4x4 ProjMat = GetLevel()->GetMainCamera()->GetTransform()->GetTransformData().Projection_;

	Dir.x /= ProjMat._11;
	Dir.y /= ProjMat._22;

	// 3. 광선을 뷰영역 -> 월드영역
	float4x4 ViewMat = GetLevel()->GetMainCamera()->GetTransform()->GetTransformData().View_;
	float4x4 InverseViewMat = ViewMat.InverseReturn();

	Dir *= InverseViewMat;

	return Dir.NormalizeReturn3D();
}

float4 NavMesh::GetMousePos()
{
	float4 Dir = CalculateCameraDir();

	float4 CameraPos = GetLevel()->GetMainCamera()->GetTransform()->GetWorldPosition();

	float Dist = 0.0f;

	// 교차성공시 교차점까지의 거리를 이용하여 해당 좌표를 반환
	if (true == CheckIntersects(CameraPos, Dir, Dist))
	{
		return CameraPos + (Dir * Dist);;
	}

	return float4::ZERO;
}

bool NavMesh::GetIntersectionPointFromMouseRay(float4& _out)
{
	float4 Dir = CalculateCameraDir();

	float4 CameraPos = GetLevel()->GetMainCamera()->GetTransform()->GetWorldPosition();

	float Dist = 0.0f;

	if (true == CheckIntersects(CameraPos, Dir, Dist))
	{
		_out = CameraPos + (Dir * Dist);
		return true;
	}

	return false;
}

bool NavMesh::IsMouseIntersects()
{
	float4 Dir = CalculateCameraDir();
	float4 CameraPos = GetLevel()->GetMainCamera()->GetTransform()->GetWorldPosition();
	float Dist = 0.0f;

	return CheckIntersects(CameraPos, Dir, Dist);
}

void NavMesh::SaveNavisData(const std::vector<FbxMeshSet>& _AllMesh)
{
	int Index = 0;

	for (size_t i = 0; i < _AllMesh.size(); i++)
	{
		for (size_t j = 0; j < _AllMesh[i].Indexs[0][0].size() / 3; j++)
		{
			NavFace& Navi = Navis.emplace_back();

			if (1 != _AllMesh[i].GameEngineIndexBuffers[0].size())
			{
				GameEngineDebug::MsgBoxError("서브셋이 존재합니다.");
			}

			Navi.info.Vertex[0] = _AllMesh[i].Vertexs[_AllMesh[i].Indexs[0][0][j * 3 + 0]].POSITION;
			Navi.info.Vertex[1] = _AllMesh[i].Vertexs[_AllMesh[i].Indexs[0][0][j * 3 + 1]].POSITION;
			Navi.info.Vertex[2] = _AllMesh[i].Vertexs[_AllMesh[i].Indexs[0][0][j * 3 + 2]].POSITION;

			Navi.info.Index = Index++;
			Navi.AllNavi = &Navis;
			Navi.Parent = this;
		}
	}

	// Link 정보 세팅
	for (UINT i = 0; i < Navis.size(); i++)
	{
		NavFace& Navi = Navis[i];

		for (UINT j = 0; j < Navis.size(); j++)
		{
			if (i == j)
			{
				continue;
			}

			if (LinkCheck(Navi, Navis[j]))
			{
				Navi.info.Link.push_back(j);
			}
		}
	}
}

void NavMesh::UserLoad(const std::string& _Path)
{
	GameEngineFile NewFile = GameEngineFile(_Path, "rb");

	int Size = 0;

	NewFile.Read(Size);

	if (0 == Size)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 NaviMesh 파일입니다.");
	}

	Navis.resize(Size);

	for (auto& Data : Navis)
	{
		NewFile.Read(Data.info.Vertex[0]);
		NewFile.Read(Data.info.Vertex[1]);
		NewFile.Read(Data.info.Vertex[2]);
		NewFile.Read(Data.info.Index);

		int LinkSize = 0;

		NewFile.Read(LinkSize);

		Data.info.Link.resize(LinkSize);

		for (size_t i = 0; i < Data.info.Link.size(); i++)
		{
			NewFile.Read(Data.info.Link[i]);
		}

		Data.AllNavi = &Navis;
		Data.Parent = this;
	}
}

void NavMesh::UserSave(const std::string& _Path)
{
	GameEngineFile NewFile = GameEngineFile(_Path, "wb");

	NewFile.Write(static_cast<int>(Navis.size()));

	for (auto& Data : Navis)
	{
		NewFile.Write(Data.info.Vertex[0]);
		NewFile.Write(Data.info.Vertex[1]);
		NewFile.Write(Data.info.Vertex[2]);
		NewFile.Write(Data.info.Index);

		NewFile.Write(static_cast<int>(Data.info.Link.size()));

		for (size_t i = 0; i < Data.info.Link.size(); i++)
		{
			NewFile.Write(Data.info.Link[i]);
		}
	}
}

bool NavMesh::LinkCheck(const NavFace& _Left, const NavFace& _Right)
{
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			if (_Left.info.Vertex[i] == _Right.info.Vertex[j])
			{
				return true;
			}
		}
	}

	return false;
}

float NavFace::YCheck(GameEngineTransform* _Transform)
{
	float Dist = 0.0f;

	if (true == OutCheck(_Transform, Dist))
	{
		return -1.0f;
	}

	return Dist - FT::Map::MAX_HEIGHT;
}

bool NavFace::OutCheck(GameEngineTransform* _Transform, float& _Dist)
{
	float4 RayPos = _Transform->GetWorldPosition() + float4(0.0f, FT::Map::MAX_HEIGHT, 0.0f);

	bool Check = false;

	if (nullptr == Parent->GetNaviRenderer())
	{
		GameEngineDebug::MsgBoxError("NaviRenderer가 지정되지 않았습니다.");
	}

	float4 V0 = info.Vertex[0] * Parent->GetNaviRenderer()->GetTransform()->GetTransformData().WorldWorld_;
	float4 V1 = info.Vertex[1] * Parent->GetNaviRenderer()->GetTransform()->GetTransformData().WorldWorld_;
	float4 V2 = info.Vertex[2] * Parent->GetNaviRenderer()->GetTransform()->GetTransformData().WorldWorld_;

	Check = DirectX::TriangleTests::Intersects(RayPos.DirectVector,
		float4::DOWN.DirectVector,
		V0.DirectVector,
		V1.DirectVector,
		V2.DirectVector,
		_Dist);

	if (false == Check)
	{
		return true;
	}

	return false;
}

NavFace* NavFace::MoveCheck(GameEngineTransform* _Transform)
{
	for (size_t i = 0; i < info.Link.size(); i++)
	{
		NavFace* Navi = &AllNavi->at(info.Link[i]);

		if (false == Navi->OutCheck(_Transform))
		{
			return Navi;
		}
	}

	return nullptr;
}

void NavMesh::Start()
{

}
void NavMesh::Update(float _DeltaTime)
{
	
}