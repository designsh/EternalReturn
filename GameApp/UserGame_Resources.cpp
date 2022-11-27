#include "PreCompile.h"
#include "UserGame.h"
#include <GameEngine\EngineVertex.h>
#include "UserGame_Resources_Shader.h"
#include <GameEngine\GameEngineFontManager.h>

void TextureLoading(GameEngineDirectory Dir)
{
	std::vector<GameEngineFile> AllFile = Dir.GetAllFile();

	for (size_t i = 0; i < AllFile.size(); i++)
	{
		GameEngineTextureManager::GetInst().Load(AllFile[i].GetFullPath());
	}

	--UserGame::LoadingFolder;
}

void UserGame::ResourcesLoad()
{
	GameEngineFontManager::GetInst().Load("궁서");
	GameEngineFontManager::GetInst().Load("malgunbd");
	GameEngineFontManager::GetInst().Load("HMKMRHD");

	//{
	//	GameEngineDirectory SoundDir;
	//	SoundDir.MoveParent("EternalReturn");
	//	SoundDir.MoveChild("Resources");
	//	SoundDir.MoveChild("Sound");

	//	std::vector<GameEngineFile> AllFile = SoundDir.GetAllFile("mp3");

	//	for (size_t i = 0; i < AllFile.size(); i++)
	//	{
	//		GameEngineSoundManager::GetInstance()->CreateSound(AllFile[i].FileName(), AllFile[i].GetFullPath());
	//	}
	//}

	// FoxTeam\Resources\Image\ 하위 디렉터리 로딩
	{
		GameEngineDirectory TextureDir;
		TextureDir.MoveParent("EternalReturn");
		TextureDir.MoveChild("Resources");
		TextureDir.MoveChild("Image");
		std::vector<GameEngineDirectory> AllDir = TextureDir.GetAllDirectoryRecursive();
		LoadingFolder = static_cast<int>(AllDir.size());
		for (size_t i = 0; i < AllDir.size(); i++)
		{
			GameEngineCore::ThreadQueue.JobPost(std::bind(TextureLoading, AllDir[i]));
		}
	}

	{
		GameEngineDirectory TextureDir;
		TextureDir.MoveParent("EternalReturn");
		TextureDir.MoveChild("Resources");
		TextureDir.MoveChild("Texture");
		TextureDir.MoveChild("Etc");

		std::vector<GameEngineDirectory> AllDir = TextureDir.GetAllDirectoryRecursive();
		LoadingFolder += static_cast<int>(AllDir.size());

		for (size_t i = 0; i < AllDir.size(); i++)
		{
			GameEngineCore::ThreadQueue.JobPost(std::bind(TextureLoading, AllDir[i]));
		}
	}

	{
		GameEngineDirectory SoundDir;
		SoundDir.MoveParent("EternalReturn");
		SoundDir / "Resources" / "Sound" / "SFX";

		std::vector<GameEngineFile> AllFile = SoundDir.GetAllFile("wav");

		for (size_t i = 0; i < AllFile.size(); i++)
		{
			GameEngineSoundManager::GetInstance()->CreateSound(AllFile[i].FileName(), AllFile[i].GetFullPath());
		}
	}

	{
		GameEngineDirectory SoundDir;
		SoundDir.MoveParent("EternalReturn");
		SoundDir / "Resources" / "Sound" / "Bgm";

		std::vector<GameEngineFile> AllFile = SoundDir.GetAllFile("wav");

		for (size_t i = 0; i < AllFile.size(); i++)
		{
			GameEngineSoundManager::GetInstance()->CreateSound(AllFile[i].FileName(), AllFile[i].GetFullPath());
		}
	}
	
	{
		GameEngineDirectory SoundDir;
		SoundDir.MoveParent("EternalReturn");
		SoundDir / "Resources" / "Sound" / "ItemBox";

		std::vector<GameEngineFile> AllFile = SoundDir.GetAllFile("wav");

		for (size_t i = 0; i < AllFile.size(); i++)
		{
			GameEngineSoundManager::GetInstance()->CreateSound(AllFile[i].FileName(), AllFile[i].GetFullPath());
		}
	}

	{
		GameEngineDirectory SoundDir;
		SoundDir.MoveParent("EternalReturn");
		SoundDir / "Resources" / "Sound" / "Char";

		std::vector<GameEngineFile> AllFile = SoundDir.GetAllFile("wav");

		for (size_t i = 0; i < AllFile.size(); i++)
		{
			GameEngineSoundManager::GetInstance()->CreateSound(AllFile[i].FileName(), AllFile[i].GetFullPath());
		}
	}

	AppShaderLoad();

}
