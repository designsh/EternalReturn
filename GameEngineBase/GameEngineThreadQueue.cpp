#include "PreCompile.h"
#include "GameEngineThreadQueue.h"

std::atomic<int> GameEngineThreadQueue::workingCount_ = 0;