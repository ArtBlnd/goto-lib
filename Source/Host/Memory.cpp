#include <Host/Memory.h>

#ifdef GTFW_PLATFORM_WINDOWS
#include <Windows.h>
namespace Goto
{   
    namespace Host 
    {
        unsigned int TransformHPF2MemFlags(unsigned int flags)
        {
            unsigned int osFlags = 0;

            if (flags & HPF_RESERVED)
            {
                osFlags |= MEM_RESERVE;
            }

            if (flags & HPF_COMMIT)
            {
                osFlags |= MEM_COMMIT;
            }
        }

        unsigned int TransformHPF2PageFlags(unsigned int flags)
        {
            if (flags & HPF_NO_ACCESS)
            {
                return PAGE_NOACCESS;
            }

            if (flags & HPF_EXECUTABLE)
            {
                if (flags & HPF_READ && flags & HPF_WRITE)
                {
                    return PAGE_EXECUTE_READWRITE;
                }

                if (flags & HPF_READ)
                {
                    return PAGE_EXECUTE_READ;
                }

                if (flags & HPF_WRITE)
                {
                    return PAGE_EXECUTE_WRITECOPY;
                }
            }

            if (flags & HPF_READ && flags & HPF_WRITE)
            {
                return PAGE_READWRITE;
            }

            if (flags & HPF_READ)
            {
                return PAGE_READONLY;
            }

            if (flags & HPF_WRITE)
            {
                return PAGE_WRITECOPY;
            }

            return 0;
        }

        void* AllocPage(size_t pages, unsigned int flags)
        {
            // Tried to allocate zero pages
            if (pages == 0)
            {
                return nullptr;
            }

            // Treid to allocate with no flags
            if (flags == 0)
            {
                return nullptr;
            }

            return VirtualAlloc(nullptr, pages * QueryPageSize(), TransformHPF2MemFlags(flags), TransformHPF2PageFlags(flags));
        }

        bool FreePage(void* page)
        {
            // If we are not decommiting it, the size should be zero
            return VirtualFree(page, 0, MEM_RELEASE);
        }

        bool ModifyPage(void* page, size_t pages, unsigned int flags)
        {
            // Tried to allocate zero pages
            if (pages == 0)
            {
                return nullptr;
            }

            // Treid to allocate with no flags
            if (flags == 0)
            {
                return nullptr;
            }

            return VirtualProtect(page, QueryPageSize() * pages, TransformHPF2PageFlags(flags), nullptr);
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
}   

#else
#error Unsupported Platform
#endif