#include <Host/Memory.h>

#define GTFW_PLATFORM_WINDOWS

#define SET_IF_HAS(v, f, s) (((v & f) != 0) ? s : 0)

#ifdef GTFW_PLATFORM_WINDOWS

#include <Windows.h>
namespace Goto
{
    void* AllocPage(size_t pages, unsigned int flags)
    {
        
    }

    bool FreePage(void* page)
    {

    }

    bool ModifyPage(void* page, size_t pages, unsigned int flags)
    {

    }

    struct 
    {
        bool         bIsCached               = false;
        unsigned int dwAllocationGranularity = 0;
        unsigned int dwPageSize              = 0;
    } OSCache;

    // Cache OS page allocation informations by using GetSystemInfo.
    void TryCacheOSInformation()
    {
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);

        OSCache.dwPageSize              = sysInfo.dwPageSize;
        OSCache.dwAllocationGranularity = sysInfo.dwAllocationGranularity;
    }

    size_t QueryPageSize()
    {
        if(!OSCache.bIsCached)
        {
            TryCacheOSInformation();
        }

        return (size_t)OSCache.dwPageSize;
    }

    size_t QueryPageGranularity()
    {
        if(!OSCache.bIsCached)
        {
            TryCacheOSInformation();
        }

        return (size_t)OSCache.dwAllocationGranularity;
    }
}

#else
#error Unsupported Platform
#endif