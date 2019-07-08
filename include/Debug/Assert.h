#ifndef __GOTO_LIB_DEBUG_ASSERT_H__
#define __GOTO_LIB_DEBUG_ASSERT_H__

#define noway_assert(cond, message)
#define debug_assert(cond, message)
#define assert(cond, message)

namespace Goto
{
    namespace Debug
    {
        void AssertMessage();
    }
}

#endif