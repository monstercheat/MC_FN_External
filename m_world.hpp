#include <list>
#include <string>
#include "m_sdk.hpp"
#include <string.h>
#include "m_funcs.hpp"
#include <iostream>
#include "m_aimbot.cpp"
#include "m_xor.h"
#include <map>
HWND window1;
uintptr_t RootComponent(uintptr_t actor)
{
	return Read <uintptr_t>(actor + offset::root_component);
}
fvector GetLocation(uintptr_t actor)
{
	return Read <fvector>(RootComponent(actor) + offset::relative_location);
}
#define FNAMEPOOL_OFFSET 0x12894800  

#define debug_loot false

char* drawBuff = (char*)malloc(4096);