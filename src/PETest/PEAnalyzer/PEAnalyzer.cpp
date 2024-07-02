#include <iostream>
#include <windows.h>
#include <imagehlp.h>

#pragma comment ( lib, "imagehlp.lib" ) 


typedef struct _MAP_FILE_STRUCT
{
	HANDLE hFile;		//文件句柄
	HANDLE hMapping;	//映射文件句柄
	LPVOID ImageBase;	//映射基址
} MAP_FILE_STRUCT, * PMAP_FILE_STRUCT;

bool LoadFile(LPTSTR lpFileName, PMAP_FILE_STRUCT pstMapFile)
{
	if (lpFileName == nullptr)
	{
		return false;
	}
	HANDLE hFile;
	HANDLE hMapping;
	LPVOID ImageBase;

	memset(pstMapFile, 0, sizeof(MAP_FILE_STRUCT));

	//1、只读方式打开文件，返回文件句柄
	hFile = CreateFile(lpFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (!hFile)
		return false;

	//2、创建内存映射文件对象
	hMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, 0);
	if (!hMapping)
	{
		CloseHandle(hFile);
		return false;
	}

	//3、创建内存映射文件的视图
	ImageBase = MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0);
	if (!ImageBase)
	{
		CloseHandle(hFile);
		CloseHandle(hMapping);
		return false;
	}
	pstMapFile->hFile = hFile;
	pstMapFile->hMapping = hMapping;
	pstMapFile->ImageBase = ImageBase;
	return true;
}


void UnLoadFile(PMAP_FILE_STRUCT pstMapFile)
{
	if (pstMapFile->hFile)
		CloseHandle(pstMapFile->hFile);

	if (pstMapFile->hMapping)
		CloseHandle(pstMapFile->hMapping);

	//撤销映射并使用CloseHandle函数关闭内存映射文件对象句柄
	if (pstMapFile->ImageBase)
		UnmapViewOfFile(pstMapFile->ImageBase);
}

bool IsPEFile64(LPVOID ImageBase)
{
	PIMAGE_DOS_HEADER pDH = NULL;
	PIMAGE_NT_HEADERS pNtH = NULL;

	if (!ImageBase)				//判断映像基址
		return false;
	pDH = (PIMAGE_DOS_HEADER)ImageBase;
	if (pDH->e_magic != IMAGE_DOS_SIGNATURE)	//判断是否为 MZ
		return false;
	pNtH = (PIMAGE_NT_HEADERS)((BYTE*)ImageBase + pDH->e_lfanew);
	if (pNtH->Signature != IMAGE_NT_SIGNATURE)
		return false;
	return true;
}

PIMAGE_NT_HEADERS GetNtHeaders64(LPVOID ImageBase)
{
	PIMAGE_DOS_HEADER pDH = NULL;
	PIMAGE_NT_HEADERS pNtH = NULL;

	if (!IsPEFile64(ImageBase))
		return NULL;

	pDH = (PIMAGE_DOS_HEADER)ImageBase;
	pNtH = (PIMAGE_NT_HEADERS)((BYTE*)ImageBase + pDH->e_lfanew);
	return pNtH;
}

PIMAGE_FILE_HEADER WINAPI GetFileHeader(LPVOID ImageBase)
{
	PIMAGE_NT_HEADERS pNtH = GetNtHeaders64(ImageBase);
	if (!pNtH)
		return NULL;
	return &pNtH->FileHeader;
}

PIMAGE_OPTIONAL_HEADER WINAPI GetOptionalHeader(LPVOID ImageBase)
{
	PIMAGE_NT_HEADERS pNtH = GetNtHeaders64(ImageBase);
	if (!pNtH)
		return NULL;
	return &pNtH->OptionalHeader;
}


//typedef struct {
//	UINT ID_RVA;
//	UINT ID_SIZE;
//} DataDir_EditID;
//
//DataDir_EditID EditID_Array[] =
//{
//	 {IDC_EDIT_DD_RVA_EXPORT,     IDC_EDIT_DD_SIZE_EXPORT},
//	{IDC_EDIT_DD_RVA_IMPORT,     IDC_EDIT_DD_SIZE_IMPORT},
//	{IDC_EDIT_DD_RVA_RES,        IDC_EDIT_DD_SZIE_RES},
//	{IDC_EDIT_DD_RVA_EXCEPTION,  IDC_EDIT_DD_SZIE_EXCEPTION},
//  {IDC_EDIT_DD_RVA_SECURITY,   IDC_EDIT_DD_SIZE_SECURITY},
//	{IDC_EDIT_DD_RVA_RELOC,     IDC_EDIT_DD_SIZE_RELOC},
//	{IDC_EDIT_DD_RVA_DEBUG,     IDC_EDIT_DD_SIZE_DEBUG},
//  {IDC_EDIT_DD_RVA_COPYRIGHT,   IDC_EDIT_DD_SIZE_COPYRIGHT},
//  {IDC_EDIT_DD_RVA_GP,     IDC_EDIT_DD_SIZE_GP},
//	{IDC_EDIT_DD_RVA_TLS,        IDC_EDIT_DD_SIZE_TLS},
//  {IDC_EDIT_DD_RVA_LOADCONFIG, IDC_EDIT_DD_SIZE_LOADCONFIG},
//  {IDC_EDIT_DD_RVA_IAT,     IDC_EDIT_DD_SIZE_IAT},
//  {IDC_EDIT_DD_RVA_BOUND,     IDC_EDIT_DD_SIZE_BOUND},
//  {IDC_EDIT_DD_RVA_COM,     IDC_EDIT_DD_SIZE_COM},
//  {IDC_EDIT_DD_RVA_DELAYIMPORT,IDC_EDIT_DD_SIZE_DELAYIMPORT},
//  {IDC_EDIT_DD_RVA_NOUSE,     IDC_EDIT_DD_SIZE_NOUSE}
//};

PIMAGE_SECTION_HEADER GetFirstSectionHeader(PIMAGE_NT_HEADERS pNtH)
{
	return IMAGE_FIRST_SECTION(pNtH);
}

LPVOID GetDirectoryEntryToData(LPVOID ImageBase, USHORT DirectoryEntry)
{
	DWORD dwDataStartRVA;
	PIMAGE_NT_HEADERS pNtH = NULL;
	PIMAGE_OPTIONAL_HEADER pOH = NULL;
	LPVOID pData = NULL;
	pNtH = GetNtHeaders64(ImageBase);
	if (!pNtH)
		return NULL;

	pOH = GetOptionalHeader(ImageBase);
	if (!pOH)
		return NULL;

	dwDataStartRVA = pOH->DataDirectory[DirectoryEntry].VirtualAddress;
	// ImageRvaToVa 用于从相对虚拟地址向磁盘地址的转换，返回一个指向磁盘地址的指针 实现在imagehlp.lib中
	pData = ImageRvaToVa(pNtH, ImageBase, dwDataStartRVA, NULL);
	if (!pData)
		return NULL;
	return pData;
}

PIMAGE_EXPORT_DIRECTORY GetExportDirectory(LPVOID ImageBase)
{
	if (!ImageBase)
		return NULL;
	PIMAGE_EXPORT_DIRECTORY pED = (PIMAGE_EXPORT_DIRECTORY)GetDirectoryEntryToData(ImageBase, IMAGE_DIRECTORY_ENTRY_EXPORT);
	return pED;
}

PIMAGE_IMPORT_DESCRIPTOR GetFirstImportDesc(LPVOID ImageBase)
{
	if (!ImageBase)
		return NULL;
	PIMAGE_IMPORT_DESCRIPTOR pID = (PIMAGE_IMPORT_DESCRIPTOR)GetDirectoryEntryToData(ImageBase, IMAGE_DIRECTORY_ENTRY_IMPORT);
	return pID;
}

void ShowPEInfo(LPVOID ImageBase)
{
	std::cout << std::endl << u8"头文件指针:" << std::endl;
	PIMAGE_FILE_HEADER pFH = GetFileHeader(ImageBase);
	std::cout << "Machine : " << pFH->Machine << std::endl;

	std::cout << std::endl << u8"可选头文件指针:" << std::endl;
	PIMAGE_OPTIONAL_HEADER pOH = GetOptionalHeader(ImageBase);
	std::cout << "EntryPoint : " << pOH->AddressOfEntryPoint << std::endl;

	printf(u8"\n数据目录表信息[Data Directory]:\n");
	printf("Name\t\t\t  RVA\t\t\t  Size\n");
	const char data[16][20] = {
		"Export Table:", //"导出表：",
		"Import Table:", //"导入表：",
		"Resource:    ",
		"Exception:   ", //"异常：",
		"Security:    ", //"安全：",
		"Relocation:  ", //"搬迁：",
		"Debug:       ",
		"Copyright:   ",
		"Globalptr:   ",
		"Tls Table:   ",
		"LoadConfig:  ",
		"IAT:         ",
		"Bound Import:",
		"COM:         ",
		"Delay Import:",
		"No Use:      " };

	for (size_t i = 0; i < 16; i++)
		printf("%s\t\t%08lX\t\t%08lX\n", data[i], pOH->DataDirectory[i].VirtualAddress, pOH->DataDirectory[i].Size);

	printf(u8"\n区块表信息[Section Table]:\n");
	printf(" Name   VAddress  VSize     RAddress  RSize     Flags\n");
	PIMAGE_NT_HEADERS pNtH = GetNtHeaders64(ImageBase);
	PIMAGE_SECTION_HEADER pSH = GetFirstSectionHeader(pNtH);
	for (size_t i = 0; i < pFH->NumberOfSections; i++)
	{
		printf("%s\t%08lX  %08lX  %08lX  %08lX  %08lX\n", pSH->Name, pSH->VirtualAddress, pSH->Misc.VirtualSize,
			pSH->PointerToRawData, pSH->SizeOfRawData, pSH->Characteristics);
		++pSH;
	}

	PIMAGE_EXPORT_DIRECTORY pED = GetExportDirectory(ImageBase);
	if (!pED)
		printf("Get Export Directory Failed\n");
	else
	{
		printf(u8"\n输出表信息[Export Table]:\n");
		const char eData[9][48] = {
			"Characteristics:            ",
			"TimeDateStamp:              ",
			"Name:                       ",
			"Base:                       ",
			"NumberOfFunctions:          ",
			"NumberOfNames:              ",
			"AddressOfFunctions:         ",
			"AddressOfNames:             ",
			"AddressOfNameOrdinals:      " };
		char* szName = NULL;
		szName = (char*)ImageRvaToVa(pNtH, ImageBase, pED->Name, NULL);
		time_t t = pED->TimeDateStamp;
		char timeStr[26] = { 0 };
		tm* ptmBeijin = localtime(&t);
		strftime(timeStr, 26, "%Y/%d %H:%M:%S", ptmBeijin);
		printf("%s %08lX\n", eData[0], pED->Characteristics);
		printf("%s %08lX (%s)\n", eData[1], pED->TimeDateStamp, timeStr);
		printf("%s %08lX (%s)\n", eData[2], pED->Name, szName);
		printf("%s %08lX\n", eData[3], pED->Base);
		printf("%s %08lX\n", eData[4], pED->NumberOfFunctions);
		printf("%s %08lX\n", eData[5], pED->NumberOfNames);
		printf("%s %08lX\n", eData[6], pED->AddressOfFunctions);
		printf("%s %08lX\n", eData[7], pED->AddressOfNames);
		printf("%s %08lX\n", eData[8], pED->AddressOfNameOrdinals);

		// Functions
		char* funcName = NULL;
		PDWORD pFunctions = (PDWORD)ImageRvaToVa(pNtH, ImageBase, pED->AddressOfFunctions, NULL);
		PDWORD pFuncNames = (PDWORD)ImageRvaToVa(pNtH, ImageBase, pED->AddressOfNames, NULL);
		PWORD pFuncOrds = (PWORD)ImageRvaToVa(pNtH, ImageBase, pED->AddressOfNameOrdinals, NULL);
		if (pFunctions)
		{
			printf(u8"\n导出函数列表[Export Funcs]:\n");
			printf("Ordinal    RVA           Name\n");
			for (size_t i = 0; i < pED->NumberOfFunctions; i++)
			{
				if (pFunctions[i])
				{
					funcName = NULL;
					for (size_t j = 0; j < pED->NumberOfNames; j++)
					{
						if (i == pFuncOrds[j])
						{
							funcName = (char*)ImageRvaToVa(pNtH, ImageBase, pFuncNames[i], NULL);
							break;
						}
					}
					printf("%04lX       %08lX      %s\n", (UINT)(pED->Base + i), pFunctions[i], funcName);
				}
			}
		}
	}

	PIMAGE_IMPORT_DESCRIPTOR pID = GetFirstImportDesc(ImageBase);
	if (!pID)
		printf("Get Import Directory Failed\n");
	else
	{
		printf(u8"\n输入表信息[Import Table]:\n");
		printf("%-20s  %s   %s      %s    %s          %s\n", "DLLName", "OrigFstThunk", "TDStamp", "ForderChin",
			"Name", "FirstThunk");
		while (pID->FirstThunk)
		{
			char* dllName = (char*)ImageRvaToVa(pNtH, ImageBase, pID->Name, NULL);
			printf("%-20s  %08lX       %08lX     %08lX      %08lX      %08lX\n", dllName, pID->OriginalFirstThunk, pID->TimeDateStamp, pID->ForwarderChain,
				pID->Name, pID->FirstThunk);
			pID++;
		}
	}
}


MAP_FILE_STRUCT stMapFile = { nullptr, nullptr, nullptr };

int main(int argc, char* argv[])
{
	LPTSTR filePath = TEXT("D:\\WorkSpace\\C++\\CMakeDemo\\src\\build\\PETest\\DemoDLL\\Debug\\DemoDLL.dll");
	UnLoadFile(&stMapFile);
	if (!LoadFile(filePath, &stMapFile))
	{
		std::cout << "Load File Failed:" << filePath << std::endl;
		return -1;
	}
	if (IsPEFile64(stMapFile.ImageBase))
	{
		std::cout << "Load DLL: PE" << std::endl;
		ShowPEInfo(stMapFile.ImageBase);
	}
	else
		std::cout << "Load DLL: Not PE" << std::endl;
}