#pragma once
#include <windows.h>
#include <dbghelp.h>
#include <tchar.h>
#include <stdio.h>
#pragma comment(lib, "dbghelp.lib") 



namespace CrashHandler {

    BOOL IsDataSectionNeeded(const WCHAR* pModuleName)
    {
        if (pModuleName == 0)
        {
            return FALSE;
        }

        WCHAR szFileName[_MAX_FNAME] = L"";
        _wsplitpath(pModuleName, NULL, NULL, szFileName, NULL);

        if (wcsicmp(szFileName, L"ntdll") == 0)
            return TRUE;

        return FALSE;
    }

    bool CALLBACK MiniDumpCallBack(PVOID pParam, const PMINIDUMP_CALLBACK_INPUT pInput, PMINIDUMP_CALLBACK_OUTPUT pOutput)
    {
        if (pInput == 0 || pOutput == 0)
            return FALSE;

        switch (pInput->CallbackType)
        {
        case ModuleCallback:
            if (pOutput->ModuleWriteFlags & ModuleWriteDataSeg)
                if (!IsDataSectionNeeded(pInput->Module.FullPath))
                    pOutput->ModuleWriteFlags &= (~ModuleWriteDataSeg);
            break;
        case IncludeModuleCallback:
        case IncludeThreadCallback:
        case ThreadCallback:
        case ThreadExCallback:
            return true;
        default:
            break;
        }
        return false;
    }

    void CreateMiniDump(PEXCEPTION_POINTERS pException)
    {
        HANDLE hDumpFile = CreateFile("test.dmp",
            GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

        if (hDumpFile && hDumpFile != INVALID_HANDLE_VALUE)
        {
            // 创建MINIDUMP_EXCEPTION_INFORMATION结构
            MINIDUMP_EXCEPTION_INFORMATION mdei;
            mdei.ThreadId = GetCurrentThreadId();
            mdei.ExceptionPointers = pException;
            mdei.ClientPointers = TRUE;

            // 调用MiniDumpWriteDump生成dump文件
            MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &mdei, NULL, NULL);

            CloseHandle(hDumpFile);
        }
        else
        {
            printf("Failed to create dump file.\n");
        }
    }

    LONG WINAPI UnhandledExceptionFilterEx(PEXCEPTION_POINTERS pException)
    {
        HANDLE hProcess = GetCurrentProcess();

        CONTEXT context = *(pException->ContextRecord);

        STACKFRAME64 stackFrame;
        ZeroMemory(&stackFrame, sizeof(STACKFRAME));
#ifdef _WIN64
        int machineType = IMAGE_FILE_MACHINE_AMD64;
        stackFrame.AddrPC.Offset = context.Rip;
        stackFrame.AddrPC.Mode = AddrModeFlat;
        stackFrame.AddrFrame.Offset = context.Rbp;
        stackFrame.AddrFrame.Mode = AddrModeFlat;
        stackFrame.AddrStack.Offset = context.Rsp;
        stackFrame.AddrStack.Mode = AddrModeFlat;
#else
        int machineType = IMAGE_FILE_MACHINE_I386;
        stackFrame.AddrPC.Offset = context.Eip;
        stackFrame.AddrPC.Mode = AddrModeFlat;
        stackFrame.AddrFrame.Offset = context.Ebp;
        stackFrame.AddrFrame.Mode = AddrModeFlat;
        stackFrame.AddrStack.Offset = context.Esp;
        stackFrame.AddrStack.Mode = AddrModeFlat;
#endif
        if (SymInitialize(hProcess, NULL, TRUE))
        {
            printf("SymInitialize : init dbghelp ok.\n");
        }

        while (StackWalk64(machineType, hProcess, GetCurrentThread(), &stackFrame, &context, NULL, SymFunctionTableAccess64, SymGetModuleBase64, NULL))
        {
            // 获取符号信息
            BYTE symbolBuffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
            PSYMBOL_INFO pSymbol = (PSYMBOL_INFO)symbolBuffer;
            pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
            pSymbol->MaxNameLen = MAX_SYM_NAME;

            if (SymFromAddr(hProcess, stackFrame.AddrPC.Offset, 0, pSymbol))
            {
                printf("[0x0%x] %s\n", stackFrame.AddrPC.Offset, pSymbol->Name);
            }
            else
            {
                DWORD error = GetLastError();
                printf("[0x0%x] unknown (SymFromAddr returned error : %d\n)", stackFrame.AddrPC.Offset, error);
            }
        }
        SymCleanup(hProcess);

        // Create MiniDump
        CreateMiniDump(pException);

        return EXCEPTION_EXECUTE_HANDLER;
    }

    void InitCrashHandler()
    {
        SetUnhandledExceptionFilter(UnhandledExceptionFilterEx);
    }


    void CrashTestTTT()
    {
        int* p = nullptr;
        *p = 0;
    }

    void CrashTestB()
    {
        printf("B");
        CrashTestTTT();
    }

    void CrashTest()
    {
        CrashTestB();
    }

} // namespace CrashHandler