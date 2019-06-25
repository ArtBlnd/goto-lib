#ifndef __GTFW_LIB_HOST_MEMORY_H__
#define __GTFW_LIB_HOST_MEMORY_H__

namespace Goto
{
    void* AllocatePage();
    size_t QueryPageSize();
    size_t QueryPageAlignment();
}

#endif