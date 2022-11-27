#include "PreCompile.h"
#include "GameEngineTexture.h"
#include "GameEngineBase/GameEngineFile.h"
#include "GameEngineBase/GameEngineString.h"

#pragma comment(lib, "DirectXTex.lib")

GameEngineTexture::GameEngineTexture()
	: TextureDesc_{}
	, Texture2D_(nullptr)
	, RenderTargetView_(nullptr)
	, ShaderResourceView_(nullptr)
	, DepthStencilView_(nullptr)
	, Image_{}
{
}

GameEngineTexture::~GameEngineTexture()
{
	if (nullptr != DepthStencilView_)
	{
		DepthStencilView_->Release();
		DepthStencilView_ = nullptr;
	}

	if (nullptr != ShaderResourceView_)
	{
		ShaderResourceView_->Release();
		ShaderResourceView_ = nullptr;
	}

	if (nullptr != RenderTargetView_)
	{
		RenderTargetView_->Release();
		RenderTargetView_ = nullptr;
	}

	if (nullptr != Texture2D_)
	{
		Texture2D_->Release();
		Texture2D_ = nullptr;
	}
}

void GameEngineTexture::Create(
	float4 _TextureSize,
	DXGI_FORMAT _Format,
	D3D11_USAGE _Usage /*= D3D11_USAGE::D3D11_USAGE_DEFAULT*/,
	unsigned int _BindFlag /*= D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE*/)
{
	D3D11_TEXTURE2D_DESC TextureInfo = {0,};
	TextureInfo.ArraySize = 1;
	TextureInfo.Width = _TextureSize.uix();
	TextureInfo.Height = _TextureSize.uiy();
	TextureInfo.Format = _Format;
	TextureInfo.SampleDesc.Count = 1;
	TextureInfo.SampleDesc.Quality = 0;
	TextureInfo.MipLevels = 1;
	TextureInfo.Usage = _Usage;

	if (_Usage == D3D11_USAGE::D3D11_USAGE_DYNAMIC)
	{
		TextureInfo.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else 
	{
		TextureInfo.CPUAccessFlags = 0;
	}

	TextureInfo.BindFlags = _BindFlag;

	Create(TextureInfo);
}

void GameEngineTexture::Create(D3D11_TEXTURE2D_DESC _Desc, bool _BindCreate)
{
	TextureDesc_ = _Desc;

	GameEngineDevice::GetDevice()->CreateTexture2D(&TextureDesc_, nullptr, &Texture2D_);

	if (nullptr == Texture2D_)
	{
		GameEngineDebug::MsgBoxError("Texture Create Error");
		return;
	}

	if (false == _BindCreate)
	{
		return;
	}

	if (_Desc.BindFlags & D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET)
	{
		CreateRenderTargetView();
	}

	if (_Desc.BindFlags & D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE)
	{
		CreateShaderResourceView();
	}

	if (_Desc.BindFlags & D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL)
	{
		CreateDepthStencilView();
	}
}

void GameEngineTexture::Create(ID3D11Texture2D* _Texture2D) 
{
	if (nullptr == _Texture2D)
	{
		GameEngineDebug::MsgBoxError("Texture Is null GameEngine Texture Create Error");
	}

	Texture2D_ = _Texture2D;
	Texture2D_->GetDesc(&TextureDesc_);
}

ID3D11RenderTargetView* GameEngineTexture::CreateRenderTargetView()
{
	if (nullptr != RenderTargetView_)
	{
		return RenderTargetView_;
	}

	D3D11_RENDER_TARGET_VIEW_DESC Desc;
	Desc.Format = TextureDesc_.Format;
	if (TextureDesc_.ArraySize == 1)
	{
		Desc.ViewDimension = D3D11_RTV_DIMENSION::D3D11_RTV_DIMENSION_TEXTURE2D;
		Desc.Texture2D.MipSlice = 0;
	}
	else
	{
		Desc.ViewDimension = D3D11_RTV_DIMENSION::D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
		Desc.Texture2DArray.ArraySize = TextureDesc_.ArraySize;// 3
		Desc.Texture2DArray.FirstArraySlice = 0; // 0 3
		Desc.Texture2DArray.MipSlice = -1;
	}

	if (S_OK != GameEngineDevice::GetDevice()->CreateRenderTargetView(Texture2D_, &Desc, &RenderTargetView_))
	{
		GameEngineDebug::MsgBoxError("RenderTargetView Create Error");
	}

	return RenderTargetView_;
}

ID3D11ShaderResourceView* GameEngineTexture::CreateShaderResourceView()
{
	if (nullptr != ShaderResourceView_)
	{
		return ShaderResourceView_;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC Desc;
	Desc.Format = TextureDesc_.Format;
	if (TextureDesc_.ArraySize == 1)
	{
		Desc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D;
		Desc.Texture2D.MipLevels = 1;
		Desc.Texture2D.MostDetailedMip = 0;
	}
	else
	{
		Desc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		Desc.Texture2DArray.ArraySize = TextureDesc_.ArraySize;
		Desc.Texture2DArray.FirstArraySlice = 0;
		Desc.Texture2DArray.MipLevels = 1;
		Desc.Texture2DArray.MostDetailedMip = 0;
	}

	if (S_OK != GameEngineDevice::GetDevice()->CreateShaderResourceView(Texture2D_, &Desc, &ShaderResourceView_))
	{
		GameEngineDebug::MsgBoxError("RenderTargetView Create Error");
		return nullptr;
	}

	return ShaderResourceView_;
}

ID3D11DepthStencilView* GameEngineTexture::CreateDepthStencilView() 
{
	if (nullptr != DepthStencilView_)
	{
		return DepthStencilView_;
	}

	if (S_OK != GameEngineDevice::GetDevice()->CreateDepthStencilView(Texture2D_, nullptr, &DepthStencilView_))
	{
		GameEngineDebug::MsgBoxError("RenderTargetView Create Error");
		return nullptr;
	}

	return DepthStencilView_;
}

ID3D11RenderTargetView* GameEngineTexture::CreateRenderTargetViewArrayIndex(int _Index)
{
	D3D11_RENDER_TARGET_VIEW_DESC Desc = {};
	Desc.Format = TextureDesc_.Format;
	if (TextureDesc_.ArraySize == 1)
	{
		Desc.ViewDimension = D3D11_RTV_DIMENSION::D3D11_RTV_DIMENSION_TEXTURE2D;
		Desc.Texture2D.MipSlice = 0;
	}
	else
	{
		Desc.ViewDimension = D3D11_RTV_DIMENSION::D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
		Desc.Texture2DArray.ArraySize = 1;
		Desc.Texture2DArray.FirstArraySlice = _Index;
		Desc.Texture2DArray.MipSlice = 0;
	}

	ID3D11RenderTargetView* Result = nullptr;
	if (S_OK != GameEngineDevice::GetDevice()->CreateRenderTargetView(Texture2D_, &Desc, &Result))
	{
		GameEngineDebug::MsgBoxError("RenderTargetView Create Error");
		return Result;
	}

	return Result;
}

ID3D11DepthStencilView* GameEngineTexture::CreateDepthStencilViewArrayIndex(int _Index)
{
	D3D11_DEPTH_STENCIL_VIEW_DESC Desc = {};
	Desc.Format = TextureDesc_.Format;
	Desc.Flags = 0;
	if (TextureDesc_.ArraySize == 1)
	{
		Desc.ViewDimension = D3D11_DSV_DIMENSION::D3D11_DSV_DIMENSION_TEXTURE2D;
		Desc.Texture2D.MipSlice = 0;
	}
	else
	{
		Desc.ViewDimension = D3D11_DSV_DIMENSION::D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
		Desc.Texture2DArray.ArraySize = 1;// 3
		Desc.Texture2DArray.FirstArraySlice = _Index; // 0 3
		Desc.Texture2DArray.MipSlice = 0;
	}

	ID3D11DepthStencilView* Result = nullptr;
	if (S_OK != GameEngineDevice::GetDevice()->CreateDepthStencilView(Texture2D_, &Desc, &Result))
	{
		GameEngineDebug::MsgBoxError("RenderTargetView Create Error");
		return Result;
	}

	return Result;
}

void GameEngineTexture::Load(const std::string& _Path)
{
	GameEngineFile NewFile = GameEngineFile(_Path);

	std::string Extension = NewFile.GetExtension();
	Extension = GameEngineString::toupper(Extension);

	// PNG 풍화설

	std::wstring wPath;
	GameEngineString::AnsiToUnicode(_Path, wPath);

	// PNG
	if (Extension == ".TGA")
	{
		if (S_OK != DirectX::LoadFromTGAFile(wPath.c_str(), DirectX::TGA_FLAGS_NONE, nullptr, Image_))
		{
			GameEngineDebug::MsgBoxError("로드할수 없는 이미지 포맷입니다" + _Path);
		}
	}
	else if (Extension == ".DDS")
	{
		GameEngineDebug::MsgBoxError("로드할수 없는 이미지 포맷입니다" + Extension);
	}
	else 
	{
		if (S_OK != DirectX::LoadFromWICFile(wPath.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, Image_))
		{
			GameEngineDebug::MsgBoxError("로드할수 없는 이미지 포맷입니다" + _Path);
		}
	}

	if (S_OK != DirectX::CreateShaderResourceView(GameEngineDevice::GetDevice(),
		Image_.GetImages(),
		Image_.GetImageCount(),
		Image_.GetMetadata(), &ShaderResourceView_))
	{
		GameEngineDebug::MsgBoxError("쉐이더 리소스 뷰를 생성하는데 실패했습니다." + _Path);
	}

	TextureDesc_.Width = static_cast<unsigned int>(Image_.GetMetadata().width);
	TextureDesc_.Height = static_cast<unsigned int>(Image_.GetMetadata().height);
	TextureDesc_.ArraySize = 1;
	TextureDesc_.Format = Image_.GetMetadata().format;
}

void GameEngineTexture::PushCutIndex(const float4& _Size, const float4& _Pos)
{
	float4 CutUv;

	CutUv.x = _Pos.x;
	CutUv.y = _Pos.y;

	CutUv.z = _Size.x;
	CutUv.w = _Size.y;

	CutList_.push_back(CutUv);
}

void GameEngineTexture::Cut(int _x, int _y) 
{
	for (int y = 0; y < _y; y++)
	{
		float4 CurStart;

		CurStart.z = 1.0f / _x;
		CurStart.w = 1.0f / _y;

		CurStart.y = CurStart.w * y;

		for (int x = 0; x < _x; x++)
		{
			CurStart.x = CurStart.z * x;
			CutList_.push_back(CurStart);
		}
	}

}


float4 GameEngineTexture::GetCutData(int _Index)
{
	if (0 == CutList_.size())
	{
		GameEngineDebug::MsgBoxError("자르지 않은 텍스처에서 인덱스를 얻어오려고 했습니다.");
	}

	if (_Index > CutList_.size())
	{
		GameEngineDebug::MsgBoxError("자른 개수에 비해서 인덱스가 너무 큽니다.");
	}

	return CutList_[_Index];
}

bool GameEngineTexture::IsCut() 
{
	return CutList_.size() != 0;
}

float4 GameEngineTexture::GetPixel(int _x, int _y) 
{
	// 1111
	// RGBA
	// 

	if (0 > _x)
	{
		return float4::ZERO;
	}

	if (0 > _y)
	{
		return float4::ZERO;
	}

	if (Image_.GetMetadata().width <= _x)
	{
		return float4::ZERO;
	}

	if (Image_.GetMetadata().height <= _y)
	{
		return float4::ZERO;
	}

	DXGI_FORMAT Fmt = Image_.GetMetadata().format;

	uint8_t* Color = Image_.GetImages()->pixels;
	// int* ColorPtr = reinterpret_cast<int*>(Color);

	int Index = _y * static_cast<int>(Image_.GetMetadata().width) + _x;
	Color = Color + (Index * 4);

	unsigned char R = Color[0];
	unsigned char G = Color[1];
	unsigned char B = Color[2];
	unsigned char A = Color[3];

	return float4(R / 255.0f, G / 255.0f, B / 255.0f, A / 255.0f);
}