// ClangformatWrap.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <Windows.h>
#include <iostream>
#include <string>

size_t ExecuteProcess(std::string FullPathToExe, std::string Parameters, size_t SecondsToWait)
{
    size_t iMyCounter = 0, iReturnVal = 0, iPos = 0;
    DWORD dwExitCode = 0;
    std::string sTempStr = "";

    /* Add a space to the beginning of the Parameters */
    if (Parameters.size() != 0)
    {
        if (Parameters[0] != ' ')
        {
            Parameters.insert(0," ");
        }
    }

    /* The first parameter needs to be the exe itself */
    sTempStr = FullPathToExe;
    iPos = sTempStr.find_last_of("\\");
    sTempStr.erase(0, iPos +1);
    Parameters = sTempStr.append(Parameters);

     /* CreateProcessW can modify Parameters thus we allocate needed memory */
    char * pwszParam = new char[Parameters.size() + 1];
    if (pwszParam == 0)
    {
        return 1;
    }
    const char* pchrTemp = Parameters.c_str();
    strcpy_s(pwszParam, Parameters.size() + 1, pchrTemp);

    /* CreateProcess API initialization */
    STARTUPINFOA siStartupInfo;
    PROCESS_INFORMATION piProcessInfo;
    memset(&siStartupInfo, 0, sizeof(siStartupInfo));
    memset(&piProcessInfo, 0, sizeof(piProcessInfo));
    siStartupInfo.cb = sizeof(siStartupInfo);

    if (CreateProcessA(const_cast<LPCSTR>(FullPathToExe.c_str()),
                            pwszParam, 0, 0, false,
                            CREATE_DEFAULT_ERROR_MODE, 0, 0,
                            &siStartupInfo, &piProcessInfo) != false)
    {
         /* Watch the process. */
		DWORD dwWait=INFINITE;
		if (SecondsToWait > 0)
		{
			dwWait = SecondsToWait * 1000;
		}

		dwExitCode = WaitForSingleObject(piProcessInfo.hProcess, dwWait);

    }
    else
    {
        /* CreateProcess failed */
        iReturnVal = GetLastError();
    }

    /* Free memory */
    delete[]pwszParam;
    pwszParam = 0;

    /* Release handles */
    CloseHandle(piProcessInfo.hProcess);
    CloseHandle(piProcessInfo.hThread);

    return iReturnVal;
} 

int main(int argc, char* argv[])
{
    //MessageBoxA(NULL, "DD", "AS", 0);

	std::string args = R"(--style=file:C:\work\mthreads\mtrepos\mt_jxm_toolbox\.clang-format)";
	if (argc > 1)
	{
		for (int i = 1; i < argc; i++) {
            if (_stricmp("-style=file", argv[i]) == 0)
            {
                continue;
            }
			args += " ";
			args += argv[i];
		}
	}

    // MessageBoxA(NULL, args.c_str(), "AS", 0);
	ExecuteProcess(R"(C:\GnuWin32\bin\clang-format.exe)", args, 10);
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
