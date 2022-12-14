#include "PreCompile.h"
#include "GameEngineCore.h"
#include "GameEngineWindow.h"
#include "GameEngineResourcesManager.h"
#include "GameEngineCollision.h"
#include "GameEngineBase/GameEngineDirectory.h"
#include "GameEngineBase/GameEngineFile.h"
#include "GameEngineVertexBuffer.h"
#include "GameEngineVertexShader.h"
#include "GameEngineDepthStencil.h"
#include "EngineVertex.h"

void GameEngineCore::EngineResourcesCreate_Mesh() 
{
	// GameEngineVertexShader* Ptr = GameEngineVertexShaderManager::GetInst().Find("Texture_VS");
// Ptr->LayOutReset();
// Ptr->AddInputLayOut("POSITION", 0, 16, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA);
// Ptr->AddInputLayOut("TEXTURECOORD", 0, 16, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA);
// Ptr->AddInputLayOut("COLOR", 0, 16, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA);
// Ptr->AddInputLayOut("NORMAL", 0, 16, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA);
// Ptr->CreateLayOut();

	GameEngineVertex::LayOut.AddInputLayOut("POSITION", 0, 16, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA);
	GameEngineVertex::LayOut.AddInputLayOut("TEXTURECOORD", 0, 16, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA);
	GameEngineVertex::LayOut.AddInputLayOut("COLOR", 0, 16, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA);
	GameEngineVertex::LayOut.AddInputLayOut("NORMAL", 0, 16, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA);
	GameEngineVertex::LayOut.AddInputLayOut("TANGENT", 0, 16, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA);
	GameEngineVertex::LayOut.AddInputLayOut("BINORMAL", 0, 16, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA);
	GameEngineVertex::LayOut.AddInputLayOut("BLENDWEIGHT", 0, 16, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA);
	GameEngineVertex::LayOut.AddInputLayOut("BLENDINDICES", 0, 16, DXGI_FORMAT_R32G32B32A32_SINT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA);

	// Sphere
		// ??????
	{
		GameEngineVertex V;
		std::vector<GameEngineVertex> VBVector;
		std::vector<UINT> IBVector;

		float Radius = 0.5f;
		// ?????????? ??????????.
		V.POSITION = float4(0.0f, Radius, 0.0f, 1.0f);
		V.TEXTURECOORD = float4(0.5f, 0.0f);
		// ???? ???? ???? ???????????? ??????
		// ???? ?????? ?????? ???????? ????.
		// ???? ???????? ????.
		V.NORMAL = float4(0.0f, Radius, 0.0f, 1.0f);
		V.NORMAL.Normalize3D();
		V.NORMAL.w = 0.0f;
		V.TANGENT = float4(1.0f, 0.0f, 0.0f, 0.0f);
		V.BINORMAL = float4(0.0f, 0.0f, 1.0f, 0.0f);

		VBVector.push_back(V);

		UINT iStackCount = 40; // ???? ???? ??????????.
		UINT iSliceCount = 40; // ???????? ????

		float yRotAngle = GameEngineMath::PI / (float)iStackCount;
		float zRotAngle = (GameEngineMath::PI * 2) / (float)iSliceCount;

		// UV?? ???????? ???????? ??????.
		float yUvRatio = 1.0f / (float)iStackCount;
		float zUvRatio = 1.0f / (float)iStackCount;

		for (UINT y = 1; y < iStackCount; ++y)
		{
			// ?? ?????? ???? ??????
			float phi = y * yRotAngle;
			for (UINT z = 0; z < iSliceCount + 1; ++z)
			{
				float theta = z * zRotAngle;
				V.POSITION = float4{
					Radius * sinf(y * yRotAngle) * cosf(z * zRotAngle),
					Radius * cosf(y * yRotAngle),
					Radius * sinf(y * yRotAngle) * sinf(z * zRotAngle),
					1.0f // ???? ???? ???? ?????? ???? ??????
				};

				// V.Pos *= GameEngineRandom::RandomFloat(-0.9f, 0.1f);

				V.TEXTURECOORD = float4(yUvRatio * z, zUvRatio * y);
				V.NORMAL = V.POSITION.NormalizeReturn3D();
				V.NORMAL.w = 0.0f;

				V.TANGENT.x = -Radius * sinf(phi) * sinf(theta);
				V.TANGENT.y = 0.0f;
				V.TANGENT.z = Radius * sinf(phi) * cosf(theta);
				V.TANGENT = V.TANGENT.NormalizeReturn3D();
				V.TANGENT.w = 0.0f;

				V.BINORMAL = float4::Cross3D(V.TANGENT, V.NORMAL);
				V.BINORMAL = V.BINORMAL.NormalizeReturn3D();
				V.BINORMAL.w = 0.0f;

				VBVector.push_back(V);
			}
		}

		// ??????
		V.POSITION = float4(0.0f, -Radius, 0.0f, 1.0f);
		V.TEXTURECOORD = float4(0.5f, 1.0f);
		V.NORMAL = float4(0.0f, -Radius, 0.0f, 1.0f);
		V.NORMAL.Normalize3D();
		V.NORMAL.w = 0.0f;
		V.TANGENT = float4(-1.0f, 0.0f, 0.0f, 0.0f);
		V.BINORMAL = float4(0.0f, 0.0f, -1.0f, 0.0f);
		VBVector.push_back(V);

		// ?????? ?????? ????????.
		IBVector.clear();

		// ???????? ?????? ???? ????????.
		for (UINT i = 0; i < iSliceCount; i++)
		{
			// ?????? ?????? ??????
			IBVector.push_back(0);
			IBVector.push_back(i + 2);
			IBVector.push_back(i + 1);
		}

		for (UINT y = 0; y < iStackCount - 2; y++)
		{
			for (UINT z = 0; z < iSliceCount; z++)
			{
				IBVector.push_back((iSliceCount + 1) * y + z + 1);
				IBVector.push_back((iSliceCount + 1) * (y + 1) + (z + 1) + 1);
				IBVector.push_back((iSliceCount + 1) * (y + 1) + z + 1);

				IBVector.push_back((iSliceCount + 1) * y + z + 1);
				IBVector.push_back((iSliceCount + 1) * y + (z + 1) + 1);
				IBVector.push_back((iSliceCount + 1) * (y + 1) + (z + 1) + 1);

			}
		}

		// ?????????? ?????? ??????
		UINT iBotIndex = (UINT)VBVector.size() - 1;
		for (UINT i = 0; i < iSliceCount; i++)
		{
			// ?????? ?????? ??????
			IBVector.push_back(iBotIndex);
			IBVector.push_back(iBotIndex - (i + 2));
			IBVector.push_back(iBotIndex - (i + 1));
		}

		GameEngineVertexBufferManager::GetInst().Create("Sphere", VBVector, D3D11_USAGE::D3D11_USAGE_DEFAULT);
		GameEngineIndexBufferManager::GetInst().Create("Sphere", IBVector, D3D11_USAGE::D3D11_USAGE_DEFAULT);
	}


	// Box
	{
		/*
		float4 POSITION;
		float4 TEXTURECOORD;
		float4 COLOR;
		float4 NORMAL;
		float4 TANGENT;
		float4 BINORMAL;
		float4 WEIGHT; // ????????
		int BLENDINDICES[4] = {0, }; // 12 51 78
		*/

		std::vector<GameEngineVertex> Vertex = std::vector<GameEngineVertex>(4 * 6);

		{
			Vertex[0] = { float4({ -0.5f, 0.5f, 0.5f }), {},{},{0.0f, 0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 0.0f} };
			Vertex[1] = { float4({ 0.5f, 0.5f, 0.5f }), {},{},{0.0f, 0.0f, 1.0f, 0.0f} };
			Vertex[2] = { float4({ 0.5f, -0.5f, 0.5f }), {},{},{0.0f, 0.0f, 1.0f, 0.0f} };
			Vertex[3] = { float4({ -0.5f, -0.5f, 0.5f }), {},{},{0.0f, 0.0f, 1.0f, 0.0f} };

			Vertex[4] = { float4::RotateXDegree(Vertex[0].POSITION, 180.0f), {},{},{0.0f, 0.0f, -1.0f, 0.0f} };
			Vertex[5] = { float4::RotateXDegree(Vertex[1].POSITION, 180.0f), {},{},{0.0f, 0.0f, -1.0f, 0.0f} };
			Vertex[6] = { float4::RotateXDegree(Vertex[2].POSITION, 180.0f), {},{},{0.0f, 0.0f, -1.0f, 0.0f} };
			Vertex[7] = { float4::RotateXDegree(Vertex[3].POSITION, 180.0f), {},{},{0.0f, 0.0f, -1.0f, 0.0f} };
		}

		{
			Vertex[8] = { float4::RotateYDegree(Vertex[0].POSITION, 90.0f), {},{},{-1.0f, 0.0f, 0.0f, 0.0f} };
			Vertex[9] = { float4::RotateYDegree(Vertex[1].POSITION, 90.0f), {},{},{-1.0f, 0.0f, 0.0f, 0.0f} };
			Vertex[10] = { float4::RotateYDegree(Vertex[2].POSITION, 90.0f), {},{},{-1.0f, 0.0f, 0.0f, 0.0f} };
			Vertex[11] = { float4::RotateYDegree(Vertex[3].POSITION, 90.0f), {},{},{-1.0f, 0.0f, 0.0f, 0.0f} };

			Vertex[12] = { float4::RotateYDegree(Vertex[0].POSITION, -90.0f), {},{},{1.0f, 0.0f, 0.0f, 0.0f} };
			Vertex[13] = { float4::RotateYDegree(Vertex[1].POSITION, -90.0f), {},{},{1.0f, 0.0f, 0.0f, 0.0f} };
			Vertex[14] = { float4::RotateYDegree(Vertex[2].POSITION, -90.0f), {},{},{1.0f, 0.0f, 0.0f, 0.0f} };
			Vertex[15] = { float4::RotateYDegree(Vertex[3].POSITION, -90.0f), {},{},{1.0f, 0.0f, 0.0f, 0.0f} };
		}

		{
			Vertex[16] = { float4::RotateXDegree(Vertex[0].POSITION, 90.0f), {},{},{0.0f, 1.0f, 0.0f, 0.0f} };
			Vertex[17] = { float4::RotateXDegree(Vertex[1].POSITION, 90.0f), {},{},{0.0f, 1.0f, 0.0f, 0.0f} };
			Vertex[18] = { float4::RotateXDegree(Vertex[2].POSITION, 90.0f), {},{},{0.0f, 1.0f, 0.0f, 0.0f} };
			Vertex[19] = { float4::RotateXDegree(Vertex[3].POSITION, 90.0f), {},{},{0.0f, 1.0f, 0.0f, 0.0f} };

			Vertex[20] = { float4::RotateXDegree(Vertex[0].POSITION, -90.0f), {},{},{0.0f, -1.0f, 0.0f, 0.0f} };
			Vertex[21] = { float4::RotateXDegree(Vertex[1].POSITION, -90.0f), {},{},{0.0f, -1.0f, 0.0f, 0.0f} };
			Vertex[22] = { float4::RotateXDegree(Vertex[2].POSITION, -90.0f), {},{},{0.0f, -1.0f, 0.0f, 0.0f} };
			Vertex[23] = { float4::RotateXDegree(Vertex[3].POSITION, -90.0f), {},{},{0.0f, -1.0f, 0.0f, 0.0f} };
		}

		for (size_t i = 0; i < Vertex.size(); i += 4)
		{
			Vertex[i + 0].TEXTURECOORD = { 0.0f, 0.0f };
			Vertex[i + 1].TEXTURECOORD = { 1.0f, 0.0f };
			Vertex[i + 2].TEXTURECOORD = { 1.0f, 1.0f };
			Vertex[i + 3].TEXTURECOORD = { 0.0f, 1.0f };
		}

		GameEngineVertexBufferManager::GetInst().Create("Box", Vertex, D3D11_USAGE::D3D11_USAGE_DEFAULT);
	}

	{
		std::vector<UINT> BoxIndex;

		for (int i = 0; i < 6; i++)
		{
			BoxIndex.push_back(i * 4 + 2);
			BoxIndex.push_back(i * 4 + 1);
			BoxIndex.push_back(i * 4 + 0);

			BoxIndex.push_back(i * 4 + 3);
			BoxIndex.push_back(i * 4 + 2);
			BoxIndex.push_back(i * 4 + 0);
		}

		GameEngineIndexBufferManager::GetInst().Create("Box", BoxIndex, D3D11_USAGE::D3D11_USAGE_DEFAULT);
	}

	{
		std::vector<GameEngineVertex> RectVertex = std::vector<GameEngineVertex>(4);

		{
			// ????
			RectVertex[0] = { float4({ -0.5f, 0.5f, 0.0f }),  { 0.0f, 0.0f } };
			RectVertex[1] = { float4({ 0.5f, 0.5f, 0.0f }),  { 1.0f, 0.0f } };
			RectVertex[2] = { float4({ 0.5f, -0.5f, 0.0f }),  { 1.0f, 1.0f } };
			RectVertex[3] = { float4({ -0.5f, -0.5f, 0.0f }),  { 0.0f, 1.0f } };
		}

		GameEngineVertexBufferManager::GetInst().Create("Rect", RectVertex, D3D11_USAGE::D3D11_USAGE_DEFAULT);
	}

	{
		std::vector<UINT> RectIndex;

		RectIndex.push_back(0);
		RectIndex.push_back(1);
		RectIndex.push_back(2);

		RectIndex.push_back(0);
		RectIndex.push_back(2);
		RectIndex.push_back(3);

		GameEngineIndexBufferManager::GetInst().Create("Rect", RectIndex, D3D11_USAGE::D3D11_USAGE_DEFAULT);
	}

	{
		std::vector<GameEngineVertex> RectVertex = std::vector<GameEngineVertex>(5);

		{
			// ????
			RectVertex[0] = { float4({ -0.5f, 0.5f, 0.0f }),  { 0.0f, 0.0f } };
			RectVertex[1] = { float4({ 0.5f, 0.5f, 0.0f }),  { 1.0f, 0.0f } };
			RectVertex[2] = { float4({ 0.5f, -0.5f, 0.0f }),  { 1.0f, 1.0f } };
			RectVertex[3] = { float4({ -0.5f, -0.5f, 0.0f }),  { 0.0f, 1.0f } };
			RectVertex[4] = { float4({ -0.5f, 0.5f, 0.0f }),  { 0.0f, 0.0f } };
		}

		GameEngineVertexBufferManager::GetInst().Create("DebugRect", RectVertex, D3D11_USAGE::D3D11_USAGE_DEFAULT);
	}

	{
		std::vector<UINT> RectIndex;

		RectIndex.push_back(0);
		RectIndex.push_back(1);
		RectIndex.push_back(2);
		RectIndex.push_back(3);
		RectIndex.push_back(4);

		GameEngineIndexBufferManager::GetInst().Create("DebugRect", RectIndex, D3D11_USAGE::D3D11_USAGE_DEFAULT);
	}

	{
		{
			std::vector<GameEngineVertex> Vertex = std::vector<GameEngineVertex>(4 * 6);

			{
				Vertex[0] = { float4({ -0.5f, 0.5f, 0.5f }), {},{},{0.0f, 0.0f, 1.0f, 0.0f} };
				Vertex[1] = { float4({ 0.5f, 0.5f, 0.5f }), {},{},{0.0f, 0.0f, 1.0f, 0.0f} };
				Vertex[2] = { float4({ 0.5f, -0.5f, 0.5f }), {},{},{0.0f, 0.0f, 1.0f, 0.0f} };
				Vertex[3] = { float4({ -0.5f, -0.5f, 0.5f }), {},{},{0.0f, 0.0f, 1.0f, 0.0f} };

				Vertex[4] = { float4::RotateXDegree(Vertex[0].POSITION, 180.0f), {},{},{0.0f, 0.0f, -1.0f, 0.0f} };
				Vertex[5] = { float4::RotateXDegree(Vertex[1].POSITION, 180.0f), {},{},{0.0f, 0.0f, -1.0f, 0.0f} };
				Vertex[6] = { float4::RotateXDegree(Vertex[2].POSITION, 180.0f), {},{},{0.0f, 0.0f, -1.0f, 0.0f} };
				Vertex[7] = { float4::RotateXDegree(Vertex[3].POSITION, 180.0f), {},{},{0.0f, 0.0f, -1.0f, 0.0f} };
			}

			{
				Vertex[8] = { float4::RotateYDegree(Vertex[0].POSITION, 90.0f), {},{},{-1.0f, 0.0f, 0.0f, 0.0f} };
				Vertex[9] = { float4::RotateYDegree(Vertex[1].POSITION, 90.0f), {},{},{-1.0f, 0.0f, 0.0f, 0.0f} };
				Vertex[10] = { float4::RotateYDegree(Vertex[2].POSITION, 90.0f), {},{},{-1.0f, 0.0f, 0.0f, 0.0f} };
				Vertex[11] = { float4::RotateYDegree(Vertex[3].POSITION, 90.0f), {},{},{-1.0f, 0.0f, 0.0f, 0.0f} };

				Vertex[12] = { float4::RotateYDegree(Vertex[0].POSITION, -90.0f), {},{},{1.0f, 0.0f, 0.0f, 0.0f} };
				Vertex[13] = { float4::RotateYDegree(Vertex[1].POSITION, -90.0f), {},{},{1.0f, 0.0f, 0.0f, 0.0f} };
				Vertex[14] = { float4::RotateYDegree(Vertex[2].POSITION, -90.0f), {},{},{1.0f, 0.0f, 0.0f, 0.0f} };
				Vertex[15] = { float4::RotateYDegree(Vertex[3].POSITION, -90.0f), {},{},{1.0f, 0.0f, 0.0f, 0.0f} };
			}

			{
				Vertex[16] = { float4::RotateXDegree(Vertex[0].POSITION, 90.0f), {},{},{0.0f, 1.0f, 0.0f, 0.0f} };
				Vertex[17] = { float4::RotateXDegree(Vertex[1].POSITION, 90.0f), {},{},{0.0f, 1.0f, 0.0f, 0.0f} };
				Vertex[18] = { float4::RotateXDegree(Vertex[2].POSITION, 90.0f), {},{},{0.0f, 1.0f, 0.0f, 0.0f} };
				Vertex[19] = { float4::RotateXDegree(Vertex[3].POSITION, 90.0f), {},{},{0.0f, 1.0f, 0.0f, 0.0f} };

				Vertex[20] = { float4::RotateXDegree(Vertex[0].POSITION, -90.0f), {},{},{0.0f, -1.0f, 0.0f, 0.0f} };
				Vertex[21] = { float4::RotateXDegree(Vertex[1].POSITION, -90.0f), {},{},{0.0f, -1.0f, 0.0f, 0.0f} };
				Vertex[22] = { float4::RotateXDegree(Vertex[2].POSITION, -90.0f), {},{},{0.0f, -1.0f, 0.0f, 0.0f} };
				Vertex[23] = { float4::RotateXDegree(Vertex[3].POSITION, -90.0f), {},{},{0.0f, -1.0f, 0.0f, 0.0f} };
			}

			for (size_t i = 0; i < Vertex.size(); i += 4)
			{
				Vertex[i + 0].TEXTURECOORD = { 0.0f, 0.0f };
				Vertex[i + 1].TEXTURECOORD = { 1.0f, 0.0f };
				Vertex[i + 2].TEXTURECOORD = { 1.0f, 1.0f };
				Vertex[i + 3].TEXTURECOORD = { 0.0f, 1.0f };
			}

			GameEngineVertexBufferManager::GetInst().Create("DebugBox", Vertex, D3D11_USAGE::D3D11_USAGE_DEFAULT);
		}

		{
			std::vector<UINT> BoxIndex;

			for (int i = 0; i < 6; i++)
			{
				BoxIndex.push_back(i * 4 + 2);
				BoxIndex.push_back(i * 4 + 1);
				BoxIndex.push_back(i * 4 + 0);

				BoxIndex.push_back(i * 4 + 3);
				BoxIndex.push_back(i * 4 + 2);
				BoxIndex.push_back(i * 4 + 0);
			}

			GameEngineIndexBufferManager::GetInst().Create("DebugBox", BoxIndex, D3D11_USAGE::D3D11_USAGE_DEFAULT);
		}
	}

	{
		std::vector<GameEngineVertex> CircleVertex = std::vector<GameEngineVertex>(21);

		{
			float4 buffer = { 0.0f, 0.0f, 0.0f };
			// ????
			CircleVertex[0] = { float4({ 0.0f, 0.0f, 0.0f }),  { 0.5f, 0.5f } };

			CircleVertex[1] = { float4({ 0.5f, 0.0f, 0.0f }),  { 0.5f, 0.5f } };

			// ?????? ???????? for ?? ???? ?????? ???????? ???????????? ?????? ???? ???? // ?????? 0717
			buffer = buffer.RotateYDegree(CircleVertex[1].POSITION, 18.0f);
			CircleVertex[2] = { buffer,  {0.5f, 0.5f} };

			buffer = buffer.RotateYDegree(CircleVertex[2].POSITION, 18.0f);
			CircleVertex[3] = { buffer,  {0.5f, 0.5f} };

			buffer = buffer.RotateYDegree(CircleVertex[3].POSITION, 18.0f);
			CircleVertex[4] = { buffer,  {0.5f, 0.5f} };

			buffer = buffer.RotateYDegree(CircleVertex[4].POSITION, 18.0f);
			CircleVertex[5] = { buffer,  {0.5f, 0.5f} };

			buffer = buffer.RotateYDegree(CircleVertex[5].POSITION, 18.0f);
			CircleVertex[6] = { buffer,  {0.5f, 0.5f} };

			buffer = buffer.RotateYDegree(CircleVertex[6].POSITION, 18.0f);
			CircleVertex[7] = { buffer,  {0.5f, 0.5f} };

			buffer = buffer.RotateYDegree(CircleVertex[7].POSITION, 18.0f);
			CircleVertex[8] = { buffer,  {0.5f, 0.5f} };

			buffer = buffer.RotateYDegree(CircleVertex[8].POSITION, 18.0f);
			CircleVertex[9] = { buffer,  {0.5f, 0.5f} };

			buffer = buffer.RotateYDegree(CircleVertex[9].POSITION, 18.0f);
			CircleVertex[10] = { buffer,  {0.5f, 0.5f} };

			buffer = buffer.RotateYDegree(CircleVertex[10].POSITION, 18.0f);
			CircleVertex[11] = { buffer,  {0.5f, 0.5f} };

			buffer = buffer.RotateYDegree(CircleVertex[11].POSITION, 18.0f);
			CircleVertex[12] = { buffer,  {0.5f, 0.5f} };

			buffer = buffer.RotateYDegree(CircleVertex[12].POSITION, 18.0f);
			CircleVertex[13] = { buffer,  {0.5f, 0.5f} };

			buffer = buffer.RotateYDegree(CircleVertex[13].POSITION, 18.0f);
			CircleVertex[14] = { buffer,  {0.5f, 0.5f} };

			buffer = buffer.RotateYDegree(CircleVertex[14].POSITION, 18.0f);
			CircleVertex[15] = { buffer,  {0.5f, 0.5f} };

			buffer = buffer.RotateYDegree(CircleVertex[15].POSITION, 18.0f);
			CircleVertex[16] = { buffer,  {0.5f, 0.5f} };

			buffer = buffer.RotateYDegree(CircleVertex[16].POSITION, 18.0f);
			CircleVertex[17] = { buffer,  {0.5f, 0.5f} };

			buffer = buffer.RotateYDegree(CircleVertex[17].POSITION, 18.0f);
			CircleVertex[18] = { buffer,  {0.5f, 0.5f} };

			buffer = buffer.RotateYDegree(CircleVertex[18].POSITION, 18.0f);
			CircleVertex[19] = { buffer,  {0.5f, 0.5f} };

			buffer = buffer.RotateYDegree(CircleVertex[19].POSITION, 18.0f);
			CircleVertex[20] = { buffer,  {0.5f, 0.5f} };
		}

		GameEngineVertexBufferManager::GetInst().Create("DebugCircle", CircleVertex, D3D11_USAGE::D3D11_USAGE_DEFAULT);
	}

	{
		std::vector<UINT> circleIndex;

		for (UINT i = 0; i < 19; i++)
		{
			circleIndex.push_back(0);
			circleIndex.push_back(i + 1);
			circleIndex.push_back(i + 2);
		}

		circleIndex.push_back(0);
		circleIndex.push_back(20);
		circleIndex.push_back(1);


		GameEngineIndexBufferManager::GetInst().Create("DebugCircle", circleIndex, D3D11_USAGE::D3D11_USAGE_DEFAULT);
	}

	{
		std::vector<GameEngineVertex> RectVertex = std::vector<GameEngineVertex>(4);

		{
			// ????
			RectVertex[0] = { float4({ -1.0f, 1.0f, 0.0f }),float4({ 0.0f, 0.0f }) };
			RectVertex[1] = { float4({ 1.0f, 1.0f, 0.0f }), float4({ 1.0f, 0.0f }) };
			RectVertex[2] = { float4({ 1.0f, -1.0f, 0.0f }), float4({ 1.0f, 1.0f }) };
			RectVertex[3] = { float4({ -1.0f, -1.0f, 0.0f }),  float4({ 0.0f, 1.0f }) };
		}

		GameEngineVertexBufferManager::GetInst().Create("FullRect", RectVertex, D3D11_USAGE::D3D11_USAGE_DEFAULT);
	}

	{
		std::vector<UINT> RectIndex;

		RectIndex.push_back(0);
		RectIndex.push_back(1);
		RectIndex.push_back(2);

		RectIndex.push_back(0);
		RectIndex.push_back(2);
		RectIndex.push_back(3);

		GameEngineIndexBufferManager::GetInst().Create("FullRect", RectIndex, D3D11_USAGE::D3D11_USAGE_DEFAULT);
	}

	{
		std::vector<GameEngineVertex> Vertex = std::vector<GameEngineVertex>(3);

		{
			Vertex[0] = { float4({ -0.5f, -0.5f, 0.0f }),  { 0.0f, 0.0f } };
			Vertex[1] = { float4({ 0.5f, -0.5f, 0.0f }),  { 1.0f, 0.0f } };
			Vertex[2] = { float4({ 0.0f, 0.5f, 0.0f }),  { 1.0f, 1.0f } };
		}

		GameEngineVertexBufferManager::GetInst().Create("Tri", Vertex, D3D11_USAGE::D3D11_USAGE_DEFAULT);
	}

	{
		std::vector<UINT> Index;

		Index.push_back(0);
		Index.push_back(1);
		Index.push_back(2);

		GameEngineIndexBufferManager::GetInst().Create("Tri", Index, D3D11_USAGE::D3D11_USAGE_DEFAULT);
	}
}

void GameEngineCore::EngineResourcesCreate_Rasterizer()
{

	{
		D3D11_RASTERIZER_DESC Info = { D3D11_FILL_MODE::D3D11_FILL_SOLID, };
		Info.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		Info.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
		Info.AntialiasedLineEnable = true;
		Info.MultisampleEnable = true;
		Info.DepthClipEnable = true;
		GameEngineRasterizer* Ptr = GameEngineRasterizerManager::GetInst().Create("EngineBaseRasterizerBack", Info);
	}

	{
		D3D11_RASTERIZER_DESC Info = { D3D11_FILL_MODE::D3D11_FILL_SOLID, };
		Info.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		Info.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
		Info.AntialiasedLineEnable = true;
		Info.MultisampleEnable = true;
		Info.DepthClipEnable = true;
		GameEngineRasterizer* Ptr = GameEngineRasterizerManager::GetInst().Create("EngineBaseRasterizerFront", Info);
	}

	{
		D3D11_RASTERIZER_DESC Info = { D3D11_FILL_MODE::D3D11_FILL_SOLID, };
		Info.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		Info.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		Info.AntialiasedLineEnable = true;
		Info.MultisampleEnable = true;
		Info.DepthClipEnable = true;
		GameEngineRasterizer* Ptr = GameEngineRasterizerManager::GetInst().Create("EngineBaseRasterizerNone", Info);
	}

	{
		D3D11_RASTERIZER_DESC Info = { D3D11_FILL_MODE::D3D11_FILL_WIREFRAME, };
		Info.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
		Info.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		Info.AntialiasedLineEnable = true;
		Info.MultisampleEnable = true;
		Info.DepthClipEnable = true;
		GameEngineRasterizer* Ptr = GameEngineRasterizerManager::GetInst().Create("EngineBaseRasterizerWireFrame", Info);
	}
}



void GameEngineCore::EngineResourcesLoad()
{
	{
		GameEngineDirectory EngineDir;

		while (true)
		{
			if (EngineDir.IsRoot())
			{
				GameEngineDebug::MsgBoxError("???? ?????? ?????? ???????? ????????.");
				return;
			}

			std::vector<GameEngineDirectory> AllDir = EngineDir.GetAllDirectory("EngineResources");

			if (0 == AllDir.size())
			{
				EngineDir.MoveParent();
				continue;
			}

			EngineDir.MoveChild("EngineResources");
			break;
		}

		EngineDir.MoveChild("Texture");

		std::vector<GameEngineFile> AllFile = EngineDir.GetAllFile();

		for (size_t i = 0; i < AllFile.size(); i++)
		{
			GameEngineTextureManager::GetInst().Load(AllFile[i].GetFullPath());
		}
	}

	{
		GameEngineDirectory EngineDir;

		while (true)
		{
			if (EngineDir.IsRoot())
			{
				GameEngineDebug::MsgBoxError("???? ?????? ?????? ???????? ????????.");
				return;
			}

			std::vector<GameEngineDirectory> AllDir = EngineDir.GetAllDirectory("EngineResources");

			if (0 == AllDir.size())
			{
				EngineDir.MoveParent();
				continue;
			}

			EngineDir.MoveChild("EngineResources");
			break;
		}

		EngineDir.MoveChild("Shader");

		std::vector<GameEngineFile> AllShader = EngineDir.GetAllFile("fx");

		for (auto& ShaderFile : AllShader)
		{
			GameEngineShader::AutoCompile(ShaderFile);
		}

		// GameEngineVertexShader* Ptr = GameEngineVertexShaderManager::GetInst().Find("Texture_VS");
		// Ptr->LayOutReset();
		// Ptr->AddInputLayOut("POSITION", 0, 16, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA);
		// Ptr->AddInputLayOut("TEXTURECOORD", 0, 16, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA);
		// Ptr->AddInputLayOut("COLOR", 0, 16, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA);
		// Ptr->AddInputLayOut("NORMAL", 0, 16, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA);
		// Ptr->CreateLayOut();

	}

	GameEngineSampler* NewRes = GameEngineSamplerManager::GetInst().Find("PointSmp");
	NewRes->Info_.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	NewRes->ReCreate();
}
void GameEngineCore::EngineResourcesCreate()
{
	EngineResourcesCreate_Mesh();
	EngineResourcesCreate_Rasterizer();

	{
		D3D11_BLEND_DESC BlendInfo = { 0 };

		BlendInfo.AlphaToCoverageEnable = FALSE;
		BlendInfo.IndependentBlendEnable = FALSE;
		BlendInfo.RenderTarget[0].BlendEnable = true;
		BlendInfo.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		BlendInfo.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		BlendInfo.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
		BlendInfo.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
		BlendInfo.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		BlendInfo.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		BlendInfo.RenderTarget[0].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;

		GameEngineBlendManager::GetInst().Create("AlphaBlend", BlendInfo);
	}

	{
		D3D11_BLEND_DESC BlendInfo = { 0 };
		BlendInfo.AlphaToCoverageEnable = FALSE;
		BlendInfo.IndependentBlendEnable = true;
		for (size_t i = 0; i < 8; i++)
		{
			BlendInfo.RenderTarget[i].BlendEnable = true;
			BlendInfo.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
			BlendInfo.RenderTarget[i].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
			BlendInfo.RenderTarget[i].SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
			BlendInfo.RenderTarget[i].DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
			BlendInfo.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
			BlendInfo.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
			BlendInfo.RenderTarget[i].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		}
		GameEngineBlendManager::GetInst().Create("DefferdAlphaBlend", BlendInfo);
	}

	{
		D3D11_BLEND_DESC BlendInfo = { 0 };

		BlendInfo.AlphaToCoverageEnable = true;
		BlendInfo.IndependentBlendEnable = false;
		BlendInfo.RenderTarget[0].BlendEnable = true;
		BlendInfo.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		BlendInfo.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		BlendInfo.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
		BlendInfo.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
		BlendInfo.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		BlendInfo.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		BlendInfo.RenderTarget[0].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;

		GameEngineBlendManager::GetInst().Create("AlphaToCoverage", BlendInfo);
	}

	{
		D3D11_BLEND_DESC BlendInfo = { 0 };

		BlendInfo.AlphaToCoverageEnable = FALSE;
		BlendInfo.IndependentBlendEnable = FALSE;
		BlendInfo.RenderTarget[0].BlendEnable = true;
		BlendInfo.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		BlendInfo.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_MAX;
		BlendInfo.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_ONE;
		BlendInfo.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_DEST_COLOR;
		BlendInfo.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		BlendInfo.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
		BlendInfo.RenderTarget[0].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;

		GameEngineBlendManager::GetInst().Create("Trans", BlendInfo);
	}


	{
		D3D11_DEPTH_STENCIL_DESC DepthInfo = {0};

		DepthInfo.DepthEnable = true;
		DepthInfo.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
		DepthInfo.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		DepthInfo.StencilEnable = false;

		GameEngineDepthStencilManager::GetInst().Create("BaseDepthOn", DepthInfo);
	}

	{
		D3D11_DEPTH_STENCIL_DESC DepthInfo = { 0 };

		DepthInfo.DepthEnable = true;
		DepthInfo.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;
		DepthInfo.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
		DepthInfo.StencilEnable = false;
		GameEngineDepthStencilManager::GetInst().Create("MergeDepth", DepthInfo);
	}

	{ // 221102 SJH ADD : ?????? ????????
		D3D11_DEPTH_STENCIL_DESC DepthInfo = { 0 };

		DepthInfo.DepthEnable = true;
		DepthInfo.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		DepthInfo.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		DepthInfo.StencilEnable = false;
		GameEngineDepthStencilManager::GetInst().Create("ShadowDepth", DepthInfo);
	}

	{
		D3D11_DEPTH_STENCIL_DESC DepthInfo = { 0 };

		DepthInfo.DepthEnable = false;
		DepthInfo.StencilEnable = false;
		GameEngineDepthStencilManager::GetInst().Create("BaseDepthOff", DepthInfo);
	}

	{ // 221027 SJH ADD : ???? ?????????? ???????????? ???????????? ?????????? ????
		D3D11_DEPTH_STENCIL_DESC DepthInfo = { 1, };

		// ???????? ????
		DepthInfo.DepthEnable = true;
		DepthInfo.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		DepthInfo.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

		// ?????????? ????
		DepthInfo.StencilEnable = true;										// Stencil ????????
		DepthInfo.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;		// Stencil Read Mask Default Value(0xFF)
		DepthInfo.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;		// Stencil Write Mask Default Value(0xFF)

		//== ???? ?????????? ????
		DepthInfo.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;		// ???????? ?????? ???????? ???? ????
		DepthInfo.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;			// ?????????? ?????? ???????? ???? ????
		DepthInfo.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;		// ????/?????????? ???? ?????? ???????? ???? ????
		DepthInfo.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;			// ???????? ????(????????)

		//== ???? ?????????? ????
		DepthInfo.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;		// ????/?????? ???? ?????? ???????? ???? ????
		DepthInfo.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;			// ?????????? ?????? ???????? ???? ????
		DepthInfo.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;		// ????/?????????? ???? ?????? ???????? ???? ????
		DepthInfo.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;			// ???????? ????(????????)

		GameEngineDepthStencilManager::GetInst().Create("OutLineState", DepthInfo);
	}

	{
		D3D11_SAMPLER_DESC Smp_Decs = {};

		memset(&Smp_Decs, 0, sizeof(D3D11_SAMPLER_DESC));
		Smp_Decs.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		Smp_Decs.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		Smp_Decs.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		Smp_Decs.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

		Smp_Decs.MipLODBias = 0.0f;
		Smp_Decs.MaxAnisotropy = 1;
		Smp_Decs.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		Smp_Decs.MinLOD = -FLT_MAX;
		Smp_Decs.MaxLOD = FLT_MAX;
		// Smp_Decs.BorderColor;
		// Smp_Decs.MaxAnisotropy;

		GameEngineSampler* NewRes = GameEngineSamplerManager::GetInst().CreateAndFind("LINEARSmp", Smp_Decs);
	}


	{
		D3D11_SAMPLER_DESC Smp_Decs = {};

		memset(&Smp_Decs, 0, sizeof(D3D11_SAMPLER_DESC));
		Smp_Decs.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		Smp_Decs.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		Smp_Decs.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		Smp_Decs.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

		Smp_Decs.MipLODBias = 0.0f;
		Smp_Decs.MaxAnisotropy = 1;
		Smp_Decs.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		Smp_Decs.MinLOD = -FLT_MAX;
		Smp_Decs.MaxLOD = FLT_MAX;
		// Smp_Decs.BorderColor;
		// Smp_Decs.MaxAnisotropy;

		GameEngineSampler* NewRes = GameEngineSamplerManager::GetInst().CreateAndFind("PointSmp", Smp_Decs);
	}

	{
		D3D11_SAMPLER_DESC Smp_Decs = {};

		memset(&Smp_Decs, 0, sizeof(D3D11_SAMPLER_DESC));
		Smp_Decs.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		Smp_Decs.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		Smp_Decs.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		Smp_Decs.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

		Smp_Decs.MipLODBias = 0.0f;
		Smp_Decs.MaxAnisotropy = 1;
		Smp_Decs.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		Smp_Decs.MinLOD = -FLT_MAX;
		Smp_Decs.MaxLOD = FLT_MAX;
		// Smp_Decs.BorderColor;
		// Smp_Decs.MaxAnisotropy;

		GameEngineSampler* NewRes = GameEngineSamplerManager::GetInst().CreateAndFind("WrapSmp", Smp_Decs);
	}

	{
		D3D11_SAMPLER_DESC Smp_Decs = {};

		memset(&Smp_Decs, 0, sizeof(D3D11_SAMPLER_DESC));
		Smp_Decs.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		Smp_Decs.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
		Smp_Decs.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
		Smp_Decs.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;

		Smp_Decs.MipLODBias = 0.0f;
		Smp_Decs.MaxAnisotropy = 1;
		Smp_Decs.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		Smp_Decs.MinLOD = -FLT_MAX;
		Smp_Decs.MaxLOD = FLT_MAX;
		// Smp_Decs.BorderColor;
		// Smp_Decs.MaxAnisotropy;

		GameEngineSampler* NewRes = GameEngineSamplerManager::GetInst().CreateAndFind("MirrorSmp", Smp_Decs);
	}

	{
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("DebugRect");
		Pipe->SetInputAssembler1VertexBufferSetting("DebugRect");
		Pipe->SetVertexShader("Color_VS");
		Pipe->SetInputAssembler2IndexBufferSetting("DebugRect");
		Pipe->SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
		Pipe->SetPixelShader("Color_PS");
	}

	{
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("DebugBox");
		Pipe->SetInputAssembler1VertexBufferSetting("DebugBox");
		Pipe->SetVertexShader("Color_VS");
		Pipe->SetInputAssembler2IndexBufferSetting("DebugBox");
		Pipe->SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
		Pipe->SetPixelShader("Color_PS");
	}

	{
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("DebugSphere");
		Pipe->SetInputAssembler1VertexBufferSetting("Sphere");
		Pipe->SetVertexShader("Color_VS");
		Pipe->SetInputAssembler2IndexBufferSetting("Sphere");
		Pipe->SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
		Pipe->SetPixelShader("Color_PS");
	}

	{
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("DebugCircle");
		Pipe->SetInputAssembler1VertexBufferSetting("DebugCircle");
		Pipe->SetVertexShader("Color_VS");
		Pipe->SetInputAssembler2IndexBufferSetting("DebugCircle");
		Pipe->SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
		Pipe->SetPixelShader("Color_PS");
	}

	{
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("TargetMerge");
		Pipe->SetInputAssembler1VertexBufferSetting("FullRect");
		Pipe->SetInputAssembler2IndexBufferSetting("FullRect");
		Pipe->SetVertexShader("TargetMerge_VS");
		Pipe->SetPixelShader("TargetMerge_PS");
		Pipe->SetOutputMergerDepthStencil("MergeDepth");
	}

	{
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("TestColor");
		Pipe->SetInputAssembler1VertexBufferSetting("Rect");
		Pipe->SetVertexShader("TestColor_VS");
		Pipe->SetInputAssembler2IndexBufferSetting("Rect");
		Pipe->SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Pipe->SetRasterizer("EngineBaseRasterizerBack");
		Pipe->SetPixelShader("TestColor_PS");
		Pipe->SetOutputMergerBlend("AlphaBlend");
	}

	{
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("Color");
		Pipe->SetInputAssembler1VertexBufferSetting("Rect");
		Pipe->SetVertexShader("Color_VS");
		Pipe->SetInputAssembler2IndexBufferSetting("Rect");
		Pipe->SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Pipe->SetRasterizer("EngineBaseRasterizerBack");
		Pipe->SetPixelShader("Color_PS");
		Pipe->SetOutputMergerBlend("AlphaBlend");
	}

	{
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("ColorSpectrum");
		Pipe->SetInputAssembler1VertexBufferSetting("Rect");
		Pipe->SetVertexShader("ColorSpectrum_VS");
		Pipe->SetInputAssembler2IndexBufferSetting("Rect");
		Pipe->SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Pipe->SetRasterizer("EngineBaseRasterizerBack");
		Pipe->SetPixelShader("ColorSpectrum_PS");
		Pipe->SetOutputMergerBlend("AlphaBlend");
	}

	{
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("ColorProhibitedArea");
		Pipe->SetInputAssembler1VertexBufferSetting("Rect");
		Pipe->SetVertexShader("ColorProhibitedArea_VS");
		Pipe->SetInputAssembler2IndexBufferSetting("Rect");
		Pipe->SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Pipe->SetRasterizer("EngineBaseRasterizerFront");
		Pipe->SetPixelShader("ColorProhibitedArea_PS");
		Pipe->SetOutputMergerBlend("AlphaBlend");
	}

	{
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("ColorAni");
		Pipe->SetInputAssembler1VertexBufferSetting("Rect");
		Pipe->SetVertexShader("ColorAni_VS");
		Pipe->SetInputAssembler2IndexBufferSetting("Rect");
		Pipe->SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Pipe->SetRasterizer("EngineBaseRasterizerBack");
		Pipe->SetPixelShader("ColorAni_PS");
		Pipe->SetOutputMergerBlend("AlphaBlend");
	}


	{
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("TextureAtlas");
		Pipe->SetInputAssembler1VertexBufferSetting("Rect");
		Pipe->SetVertexShader("TextureAtlas_VS");
		Pipe->SetInputAssembler2IndexBufferSetting("Rect");
		Pipe->SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Pipe->SetRasterizer("EngineBaseRasterizerBack");
		Pipe->SetPixelShader("TextureAtlas_PS");
		Pipe->SetOutputMergerBlend("AlphaBlend");
	}


	{
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("TextureAni");
		Pipe->SetInputAssembler1VertexBufferSetting("Rect");
		Pipe->SetVertexShader("TextureAni_VS");
		Pipe->SetInputAssembler2IndexBufferSetting("Rect");
		Pipe->SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Pipe->SetRasterizer("EngineBaseRasterizerBack");
		Pipe->SetPixelShader("TextureAni_PS");
		Pipe->SetOutputMergerBlend("AlphaBlend");
	}

	{
		// 0805 ??????
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("TextureDeferredLightAni");
		Pipe->SetInputAssembler1VertexBufferSetting("Rect");
		Pipe->SetVertexShader("TextureDeferredLightAni_VS");
		Pipe->SetInputAssembler2IndexBufferSetting("Rect");
		Pipe->SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Pipe->SetRasterizer("EngineBaseRasterizerBack");
		Pipe->SetPixelShader("TextureDeferredLightAni_PS");
		Pipe->SetOutputMergerBlend("AlphaBlend");
	}

	{
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("TextureDeferredLight");
		Pipe->SetInputAssembler1VertexBufferSetting("Rect");
		Pipe->SetVertexShader("TextureDeferredLight_VS");
		Pipe->SetInputAssembler2IndexBufferSetting("Rect");
		Pipe->SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Pipe->SetRasterizer("EngineBaseRasterizerBack");
		Pipe->SetPixelShader("TextureDeferredLight_PS");
		Pipe->SetOutputMergerBlend("AlphaBlend");
	}

	{
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("DeferredColor");
		Pipe->SetInputAssembler1VertexBufferSetting("Rect");
		Pipe->SetVertexShader("DeferredColor_VS");
		Pipe->SetInputAssembler2IndexBufferSetting("Rect");
		Pipe->SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Pipe->SetRasterizer("EngineBaseRasterizerBack");
		Pipe->SetPixelShader("DeferredColor_PS");
		Pipe->SetOutputMergerBlend("DefferdAlphaBlend");
	}

	{
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("DeferredNavTile");
		Pipe->SetInputAssembler1VertexBufferSetting("Rect");
		Pipe->SetVertexShader("DeferredNavTile_VS");
		Pipe->SetInputAssembler2IndexBufferSetting("Rect");
		Pipe->SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Pipe->SetRasterizer("EngineBaseRasterizerWireFrame");
		Pipe->SetPixelShader("DeferredNavTile_PS");
		Pipe->SetOutputMergerBlend("AlphaBlend");
	}

	{
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("TextureLight");
		Pipe->SetInputAssembler1VertexBufferSetting("Rect");
		Pipe->SetVertexShader("TextureLight_VS");
		Pipe->SetInputAssembler2IndexBufferSetting("Rect");
		Pipe->SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Pipe->SetRasterizer("EngineBaseRasterizerBack");
		Pipe->SetPixelShader("TextureLight_PS");
		Pipe->SetOutputMergerBlend("AlphaBlend");
	}

	{
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("Texture");
		Pipe->SetInputAssembler1VertexBufferSetting("Rect");
		Pipe->SetVertexShader("Texture_VS");
		Pipe->SetInputAssembler2IndexBufferSetting("Rect");
		Pipe->SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Pipe->SetRasterizer("EngineBaseRasterizerBack");
		Pipe->SetPixelShader("Texture_PS");
		Pipe->SetOutputMergerBlend("AlphaBlend");
	}


	{
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("Fade");
		Pipe->SetInputAssembler1VertexBufferSetting("FullRect");
		Pipe->SetInputAssembler2IndexBufferSetting("FullRect");
		Pipe->SetVertexShader("Fade_VS");
		Pipe->SetPixelShader("Fade_PS");
		Pipe->SetOutputMergerDepthStencil("BaseDepthOff");
		Pipe->SetOutputMergerBlend("AlphaBlend");
	}


	{
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("Blur");
		Pipe->SetInputAssembler1VertexBufferSetting("FullRect");
		Pipe->SetInputAssembler2IndexBufferSetting("FullRect");
		Pipe->SetVertexShader("Blur_VS");
		Pipe->SetPixelShader("Blur_PS");
		Pipe->SetOutputMergerDepthStencil("BaseDepthOff");
		Pipe->SetOutputMergerBlend("AlphaBlend");
	}

	{
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("FogOfWar");
		Pipe->SetInputAssembler1VertexBufferSetting("FullRect");
		Pipe->SetInputAssembler2IndexBufferSetting("FullRect");
		Pipe->SetVertexShader("FogOfWar_VS");
		Pipe->SetPixelShader("FogOfWar_PS");
		Pipe->SetOutputMergerDepthStencil("BaseDepthOff");
		Pipe->SetOutputMergerBlend("AlphaBlend");
	}

	{
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("TextureTrans");
		Pipe->SetInputAssembler1VertexBufferSetting("Rect");
		Pipe->SetVertexShader("TextureTrans_VS");
		Pipe->SetInputAssembler2IndexBufferSetting("Rect");
		Pipe->SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Pipe->SetRasterizer("EngineBaseRasterizerNone");
		Pipe->SetPixelShader("TextureTrans_PS");
		Pipe->SetOutputMergerBlend("AlphaToCoverage");
	}

	{
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("DeferredMerge");
		Pipe->SetInputAssembler1VertexBufferSetting("FullRect");
		Pipe->SetVertexShader("DeferredMerge_VS");
		Pipe->SetInputAssembler2IndexBufferSetting("FullRect");
		Pipe->SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Pipe->SetRasterizer("EngineBaseRasterizerNone");
		Pipe->SetPixelShader("DeferredMerge_PS");
		Pipe->SetOutputMergerDepthStencil("MergeDepth");
		Pipe->SetOutputMergerBlend("AlphaBlend");
	}

	{
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("DeferredCalLight");
		Pipe->SetInputAssembler1VertexBufferSetting("FullRect");
		Pipe->SetVertexShader("DeferredCalLight_VS");
		Pipe->SetInputAssembler2IndexBufferSetting("FullRect");
		Pipe->SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Pipe->SetRasterizer("EngineBaseRasterizerNone");
		Pipe->SetPixelShader("DeferredCalLight_PS");
		Pipe->SetOutputMergerDepthStencil("MergeDepth");
		Pipe->SetOutputMergerBlend("AlphaBlend");
	}

	{
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("ColorBox");
		Pipe->SetInputAssembler1VertexBufferSetting("Box");
		Pipe->SetVertexShader("Color_VS");
		Pipe->SetInputAssembler2IndexBufferSetting("Box");
		Pipe->SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Pipe->SetRasterizer("EngineBaseRasterizerBack");
		Pipe->SetPixelShader("Color_PS");
		Pipe->SetOutputMergerBlend("AlphaBlend");
	}

	{
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("ColorSphere");
		Pipe->SetInputAssembler1VertexBufferSetting("Sphere");
		Pipe->SetVertexShader("Color_VS");
		Pipe->SetInputAssembler2IndexBufferSetting("Sphere");
		Pipe->SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Pipe->SetRasterizer("EngineBaseRasterizerBack");
		Pipe->SetPixelShader("Color_PS");
		Pipe->SetOutputMergerBlend("AlphaBlend");
	}

	// ============================================= ???????? UI ???? ============================================= //
	{
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("ProgressBarUI");
		Pipe->SetInputAssembler1VertexBufferSetting("Rect");
		Pipe->SetVertexShader("ProgressBar_VS");
		Pipe->SetInputAssembler2IndexBufferSetting("Rect");
		Pipe->SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Pipe->SetRasterizer("EngineBaseRasterizerNone");
		Pipe->SetPixelShader("ProgressBar_PS");
		Pipe->SetOutputMergerBlend("AlphaBlend");
		Pipe->SetOutputMergerDepthStencil("BaseDepthOff");
	}

	// =============================================== ???????? ???? =============================================== //

	{ // 221107 SJH ADD : ?????? ????????????????(??????????)
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("RendererShadow");
		Pipe->SetInputAssembler1VertexBufferSetting("Rect");
		Pipe->SetVertexShader("RendererShadow_VS");
		Pipe->SetInputAssembler2IndexBufferSetting("Rect");
		Pipe->SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Pipe->SetRasterizer("EngineBaseRasterizerFront");
		Pipe->SetPixelShader("RendererShadow_PS");
		Pipe->SetOutputMergerBlend("AlphaToCoverage");
	}

	{ // 221027 SJH ADD : ?????? ????????????????(??????????)
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("PreprocessingAni");
		Pipe->SetInputAssembler1VertexBufferSetting("Rect");
		Pipe->SetVertexShader("PreprocessingAni_VS");
		Pipe->SetInputAssembler2IndexBufferSetting("Rect");
		Pipe->SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Pipe->SetRasterizer("EngineBaseRasterizerFront");
		Pipe->SetPixelShader("PreprocessingAni_PS");
		Pipe->SetOutputMergerBlend("AlphaToCoverage");
	}

	{ // 221027 SJH ADD : ?????? ????????????????
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("Preprocessing");
		Pipe->SetInputAssembler1VertexBufferSetting("Rect");
		Pipe->SetVertexShader("Preprocessing_VS");
		Pipe->SetInputAssembler2IndexBufferSetting("Rect");
		Pipe->SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Pipe->SetRasterizer("EngineBaseRasterizerFront");
		Pipe->SetPixelShader("Preprocessing_PS");
		Pipe->SetOutputMergerBlend("AlphaToCoverage");
	}

	{ // 221101 SJH ADD : ????(??) ???? ?????????? ????????????????
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("LightShadow");
		Pipe->SetInputAssembler1VertexBufferSetting("FullRect");
		Pipe->SetInputAssembler2IndexBufferSetting("FullRect");
		Pipe->SetVertexShader("LightShadow_VS");
		Pipe->SetPixelShader("LightShadow_PS");
		Pipe->SetOutputMergerDepthStencil("ShadowDepth");
		Pipe->SetRasterizer("EngineBaseRasterizerNone");
		Pipe->SetOutputMergerBlend("AlphaBlend");
	}

	{ // 221109 SJH ADD : ???? ?????? ????
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("DeferredLightMap");
		Pipe->SetInputAssembler1VertexBufferSetting("Rect");
		Pipe->SetInputAssembler2IndexBufferSetting("Rect");
		Pipe->SetVertexShader("DeferredLightMap_VS");
		Pipe->SetPixelShader("DeferredLightMap_PS");
		Pipe->SetOutputMergerBlend("AlphaBlend");
	}
}