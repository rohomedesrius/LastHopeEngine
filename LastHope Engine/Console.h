#pragma once

#include "Globals.h"
#include "ImGui/imgui.h"

class EngineConsole
{
public:
	char                  InputBuf[256];
	ImVector<char*>       Items;
	bool                  ScrollToBottom;
	ImVector<char*>       History;
	int                   HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
	ImVector<const char*> Commands;

	EngineConsole();
	~EngineConsole();

	// Portable helpers
	static int   Stricmp(const char* str1, const char* str2) { int d; while ((d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; } return d; }
	static int   Strnicmp(const char* str1, const char* str2, int n) { int d = 0; while (n > 0 && (d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; n--; } return d; }
	static char* Strdup(const char *str) { size_t len = strlen(str) + 1; void* buff = malloc(len); return (char*)memcpy(buff, (const void*)str, len); }

	void    Draw(const char* title, bool* p_open);

	void    ClearLog();

private:

	void    AddLog(const char* fmt, ...) IM_FMTARGS(2);
	void    ExecCommand(const char* command_line);

	// ImGui stuff
	static int TextEditCallbackStub(ImGuiTextEditCallbackData* data);
	int     TextEditCallback(ImGuiTextEditCallbackData* data);

};