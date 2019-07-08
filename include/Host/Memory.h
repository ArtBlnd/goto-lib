#ifndef __GTFW_LIB_HOST_MEMORY_H__
#define __GTFW_LIB_HOST_MEMORY_H__

#define HPF_RESERVED    0x00000001
#define HPF_COMMIT      0x00000002
#define HPF_EXECUTABLE  0x00000004
#define HPF_READ        0x00000008
#define HPF_WRITE       0x00000016
#define HPF_NO_ACCESS   0x00000032

namespace Goto
{
    namespace Host
    {
        // Support cross platform os level page allocations, managements.
        
        void* AllocPage(size_t pages, unsigned int flags);
        bool FreePage(void* page);
        bool ModifyPage(void* page, size_t pages, unsigned int flags);

        // Returns size of one page.
        // this OS information result will be cached once when its called.
        size_t QueryPageSize();

        // Returns maximum size that page memory can be aligned
        // this OS information result will be cached once when its called.
        size_t QueryPageGranularity();
    }
}

#endif