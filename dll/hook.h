#pragma once

bool Detour32(char* src, char* dst, const intptr_t len);
char* TrampHook32(char* src, char* dst, const intptr_t len);