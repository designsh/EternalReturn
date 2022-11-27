#pragma once
#include "GameEngineImageRenderer.h"

// 분류 : 컴포넌트
// 용도 : 렌더러
// 설명 : 하나의 랜더 단위
class GameEngineRenderingPipeLine;
class GameEngineUIRenderer : public GameEngineImageRenderer
{
	friend class GameEngineLevel;
	friend class CameraComponent;

public:
	static int UIRendererCount;
	static GameEngineRenderTarget* FontTarget_;

public:
	inline std::string GetPrintText() const
	{
		return PrintText_;
	}

	inline int GetPrintTextLen() const
	{
		return static_cast<int>(PrintText_.length());
	}

public:

	inline void SetTextColor(const float4& _Color)
	{
		Color_ = _Color;
	}

	inline void SetPrintText(const std::string& _PrintText)
	{
		PrintText_ = _PrintText;
	}

public:
	inline bool AddText(std::string _PrintText)
	{
		int Length = static_cast<int>(PrintText_.length());
		if (Length <= MaxLen_)
		{
			PrintText_ += _PrintText;
			return true;
		}

		return false;
	}

	inline bool DelText()
	{
		int CurLen = static_cast<int>(PrintText_.length());
		if (CurLen <= 0)
		{
			return false;
		}

		std::string::iterator EndIter = PrintText_.end() - 1;
		PrintText_.erase(EndIter);

		return true;
	}

	inline void AllDelText()
	{
		if (false == PrintText_.empty())
		{
			PrintText_.clear();
		}
	}

public:
	void SetRenderGroup(int _Order) override;

public: // Text
	void TextSetting(std::string _FontName, std::string _PrintText, float _FontSize, unsigned int _Flags = 0, float4 _Color = float4::WHITE, const float4& _FontPivot = float4::ZERO, int _MaxLen = 15);

private:
	void Start() override;
	void Render(float _DeltaTime, bool _IsDeferred) override;

public:
	GameEngineUIRenderer();
	~GameEngineUIRenderer();

protected:
	GameEngineUIRenderer(const GameEngineUIRenderer& _Other) = delete;
	GameEngineUIRenderer(GameEngineUIRenderer&& _Other) noexcept = delete;

private:
	GameEngineUIRenderer& operator=(const GameEngineUIRenderer& _Other) = delete;
	GameEngineUIRenderer& operator=(GameEngineUIRenderer&& _Other) noexcept = delete;

private:
	bool IsText_;
	std::string FontName_;
	std::string PrintText_;
	float FontSize_;
	float4 FontPivot_;
	float4 Color_;
	unsigned int Flags_;
	int MaxLen_;
};

