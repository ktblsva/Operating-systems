#include <Windows.h>
#include <stdio.h>
#include <ImageHlp.h>
//cmd.exe /k "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvars32.bat" `& powershell 
int main(int argc, char* argv[])
{
    LOADED_IMAGE LoadedImage;
    PUCHAR BaseAddress;
    DWORD RVAImDir, VAImAddress;
    IMAGE_IMPORT_DESCRIPTOR* ImTable;

    //Загружаем PE-файл
    if(!MapAndLoad(argv[1], NULL, &LoadedImage, TRUE, TRUE))
    {
        printf("Something's wrong!\n");
        exit(1);
    }
    //Считываем базовый адрес загрузочного модуля
    BaseAddress = LoadedImage.MappedAddress;
    printf("0x%lx - Base Address\n", BaseAddress);
    //Определяем относительный виртуальный адреса - RVA, таблицы импорта
    RVAImDir = LoadedImage.FileHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
    printf("0x%lx - RVA\n", RVAImDir);
    //Определяем виртуальный адрес массива стрк по его RVA
    VAImAddress = (DWORD)ImageRvaToVa(LoadedImage.FileHeader, BaseAddress, RVAImDir, NULL);
    printf("0x%lx - VA\n", VAImAddress);
    ImTable = (IMAGE_IMPORT_DESCRIPTOR*)VAImAddress;
    while(1)
    {
        if(ImTable->Characteristics == 0)
        {
            break;
        }
        char* sName = (char*)ImageRvaToVa(LoadedImage.FileHeader, BaseAddress, ImTable->Name, NULL);
        printf("DLL Name: %s \n", sName);
        char* originalFt = (char*)ImageRvaToVa(LoadedImage.FileHeader, BaseAddress, ImTable->OriginalFirstThunk, NULL);
        char* Ft = (char*)ImageRvaToVa(LoadedImage.FileHeader, BaseAddress, ImTable->FirstThunk, NULL);
        printf("NameAddr: %08X \t  OriginalFirstThunk: %08X \t FirstThunk: %08X \t TimeDateStamp: %08X \n\n", sName, originalFt, Ft, ImTable->TimeDateStamp);
        ImTable++;
    }
   
    UnMapAndLoad(&LoadedImage);
    return 0;
}