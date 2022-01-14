#include <Windows.h>
#include <stdio.h>
#include <ImageHlp.h>

int main(int argc, char* argv[])
{
    LOADED_IMAGE LoadedImage;
    PUCHAR BaseAddress;
    DWORD RVAImDir, VAImAddress;
    IMAGE_IMPORT_DESCRIPTOR* ImportDescriptor;
    if(!MapAndLoad(argv[1], NULL, &LoadedImage, TRUE, TRUE))
    {
        printf("Something's wrong!\n");
        exit(1);
    }
    BaseAddress = LoadedImage.MappedAddress;
    printf("0x%lx - Base Address\n", BaseAddress);
    RVAImDir = LoadedImage.FileHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
    printf("0x%lx - RVA\n", RVAImDir);
    VAImAddress = (DWORD)ImageRvaToVa(LoadedImage.FileHeader, BaseAddress, RVAImDir, NULL);
    printf("0x%lx - VA\n", VAImAddress);
    ImportDescriptor = (IMAGE_IMPORT_DESCRIPTOR*)VAImAddress;
    while (ImportDescriptor->Name) {
            PIMAGE_THUNK_DATA OriginalFirstThunk = (PIMAGE_THUNK_DATA)ImageRvaToVa(LoadedImage.FileHeader, LoadedImage.MappedAddress, ImportDescriptor->OriginalFirstThunk, NULL);
            PIMAGE_THUNK_DATA FirstThunk = (PIMAGE_THUNK_DATA)ImageRvaToVa(LoadedImage.FileHeader, LoadedImage.MappedAddress, ImportDescriptor->FirstThunk, NULL);
            PIMAGE_THUNK_DATA ThunkData = OriginalFirstThunk;
            if (!ThunkData) ThunkData = FirstThunk;
            while (ThunkData->u1.AddressOfData) {
                if (!IMAGE_SNAP_BY_ORDINAL(ThunkData->u1.Ordinal)) {
                    PIMAGE_IMPORT_BY_NAME ImportFunctionName = (PIMAGE_IMPORT_BY_NAME)ImageRvaToVa(LoadedImage.FileHeader, LoadedImage.MappedAddress, ThunkData->u1.AddressOfData, NULL);
                    CHAR ImportAddressTableBuffer[MAXWORD] = "";
                    sprintf(ImportAddressTableBuffer, "%-48s | %s", (PUCHAR)ImageRvaToVa(LoadedImage.FileHeader, LoadedImage.MappedAddress, ImportDescriptor->Name, NULL),
                        ImportFunctionName->Name);
                    printf("%s\n", ImportAddressTableBuffer);
                }
                ThunkData++;
                OriginalFirstThunk++;
                FirstThunk++;
            }
            ImportDescriptor++;
        }
    UnMapAndLoad(&LoadedImage);
}
 