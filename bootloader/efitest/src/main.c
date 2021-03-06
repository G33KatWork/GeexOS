#include <stdint.h>

#include <efi.h>
#include <efilib.h>

EFI_STATUS EFIAPI efi_main(EFI_HANDLE image_handle, EFI_SYSTEM_TABLE *systab)
{
    EFI_STATUS status;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
    EFI_GUID gop_guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;

    InitializeLib(image_handle, systab);

    status = uefi_call_wrapper(systab->BootServices->LocateProtocol,
                               3,
                               &gop_guid,
                               NULL,
                               &gop);
    Print(L"Framebuffer base is at %lx\n", gop->Mode->FrameBufferBase);

    return EFI_SUCCESS;
}
