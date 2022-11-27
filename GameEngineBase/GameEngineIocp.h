#pragma once
#include <Windows.h>
#include <process.h>

// 설명 :
class GameEngineIocp
{
public:
	// constrcuter destructer
	GameEngineIocp()
		: IocpHandle(nullptr)
	{
	}	
	
	~GameEngineIocp() {}



	bool Start(int ThreadCount = 0)
	{
		IocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, ThreadCount);
		if (nullptr == IocpHandle)
		{
			// 치명적인 에러.
			return false;
		}

		return true;
	}

	// 쓰레드 안에서 실행은 시켜줘야 합니다.
	BOOL WaitforWork(DWORD& lpNumberOfBytesTransferred, ULONG_PTR& lpCompletionKey, LPOVERLAPPED& lpOverlapped, DWORD dwMilliseconds = INFINITE)
	{
		// 파일입출력 == 소켓통신

		//HANDLE CompletionPort, 이 쓰레드를 관리하는 IOCP
		//LPDWORD lpNumberOfBytesTransferred, 소켓통신 혹은 파일입출력 하고 있다면 
		//                                    내가 유튜브 지금 동영상을 xxx바이트 만큼 받았어요.
		//                                    내가 지금 3기가짜리 파일은 200메가 읽었어요

		//PULONG_PTR lpCompletionKey, // 일을 넘길때 8바이트 정수를 넘길수 있다.

		//LPOVERLAPPED* lpOverlapped, // 소켓통신을 할때 현재까지 읽은 비동기 입출력 정보.
									  // 이녀석은 진짜 리얼 안쓸겁니다.

		//DWORD dwMilliseconds      // 이건 얼마나 기다릴거냐.
		//                          INFINITY를 넣으면 진짜 일이 있을때까지 영원히 기다림.

		return GetQueuedCompletionStatus(IocpHandle, &lpNumberOfBytesTransferred, &lpCompletionKey, &lpOverlapped, dwMilliseconds);

		// 어떤 쓰레드에서 이 함수를 실행시키면
		// 일이 생길때까지 .
	}

	// WorkParameter 아무거나 집어넣으면 전달해준다.
	// -1은 예약.
	BOOL Makework(DWORD _WorkParameter, void* _Ptr)
	{
		return PostQueuedCompletionStatus(IocpHandle, _WorkParameter, reinterpret_cast<ULONG_PTR>(_Ptr), nullptr);
	}


protected:

private:
	HANDLE IocpHandle;
};

