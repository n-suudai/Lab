
#include <cstdint>

/*
    このコミット時点で警告が出ている
    https://github.com/lvandeve/lodepng/commit/f6155a4206046a31a9532d70caf517845af64c83
*/

#pragma warning(push)
#pragma warning(disable:4267) 

// cpp include
#include <lodepng.cpp>

#pragma warning(pop)

