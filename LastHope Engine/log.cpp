#pragma once
#include "Globals.h"
#include "Application.h"

void log(const char file[], int line, const char* format, ...)
{
	static char tmp_string[4096];
	static char tmp_string2[4096];
	static va_list  ap;

	// Construct the string from variable arguments
	va_start(ap, format);
	vsprintf_s(tmp_string, 4096, format, ap);
	va_end(ap);

	// Trimming File Path
	std::string input = file;
	std::string output = input.substr(input.find_last_of('\\') + 1);
	sprintf_s(tmp_string2, 4096, "\n%s(%d) : %s", output.c_str(), line, tmp_string);
	OutputDebugString(tmp_string2);

	// For Console
	sprintf_s(tmp_string2, 4096, "%s", tmp_string);
	
	// Sending to Console
	if (App && App->console_enabled)
	{
		App->editor->RegisterLog(tmp_string2);
	}
}