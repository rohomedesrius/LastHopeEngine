#include "Console.h"
#include "Application.h"

EngineConsole::EngineConsole()
{
	ClearLog();
	memset(input_buff, 0, sizeof(input_buff));
	history_pos = -1;
	commands.push_back("HELP");
	commands.push_back("HISTORY");
	commands.push_back("CLEAR");
	commands.push_back("QUIT");
	AddLog("LastHope Engine Console!");
	AddLog("Type 'HELP' to view the list of commands.");
}

EngineConsole::~EngineConsole()
{
	ClearLog();
	for (int i = 0; i < history.Size; i++)
		free(history[i]);
}

void EngineConsole::ClearLog()
{
	for (int i = 0; i < items.Size; i++)
		free(items[i]);
	items.clear();
	scroll_to_bottom = true;
}

void EngineConsole::AddLog(const char* fmt, ...) IM_FMTARGS(2)
{
	// FIXME-OPT
	char buf[1024];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
	buf[IM_ARRAYSIZE(buf) - 1] = 0;
	va_end(args);
	items.push_back(Strdup(buf));
	scroll_to_bottom = true;
}

void EngineConsole::Draw(const char * title, bool * p_open)
{
	ImGuiWindowFlags flag = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoResize;
	if (!ImGui::Begin(title, p_open, flag))
	{
		ImGui::End();
		return;
	}

	// As a specific feature guaranteed by the library, after calling Begin() the last Item represent the title bar. So e.g. IsItemHovered() will return true when hovering the title bar.
	// Here we create a context menu only available from the title bar.
	if (ImGui::BeginPopupContextItem())
	{
		if (ImGui::MenuItem("Close"))
			*p_open = false;
		ImGui::EndPopup();
	}

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.1f, 0.1f, 1.0f));
	if (ImGui::SmallButton(" Clear LOGs ")) { ClearLog(); } ImGui::SameLine();
	ImGui::PopStyleColor();
	if (ImGui::SmallButton("Separator")) { AddLog(" "); AddLog(" "); } ImGui::SameLine();
	bool copy_to_clipboard = ImGui::SmallButton("Copy"); ImGui::SameLine();
	if (ImGui::SmallButton("Scroll to bottom")) scroll_to_bottom = true; 
	//static float t = 0.0f; if (ImGui::GetTime() - t > 0.02f) { t = ImGui::GetTime(); AddLog("Spam %f", t); }

	ImGui::Separator();

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 0));
	static ImGuiTextFilter filter;
	if (ImGui::Button("Clear Filter"))
	{
		filter.Clear();
	}
	ImGui::PopStyleVar();
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
	ImGui::SameLine();
	filter.Draw("Search (Filter)", 180);
	ImGui::PopStyleVar();
	ImGui::Separator();

	const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing(); // 1 separator, 1 input text
	ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar); // Leave room for 1 separator + 1 InputText
	if (ImGui::BeginPopupContextWindow())
	{
		if (ImGui::Selectable("Clear")) ClearLog();
		ImGui::EndPopup();
	}

	// Display every line as a separate entry so we can change their color or add custom widgets. If you only want raw text you can use ImGui::TextUnformatted(log.begin(), log.end());
	// NB- if you have thousands of entries this approach may be too inefficient and may require user-side clipping to only process visible items.
	// You can seek and display only the lines that are visible using the ImGuiListClipper helper, if your elements are evenly spaced and you have cheap random access to the elements.
	// To use the clipper we could replace the 'for (int i = 0; i < Items.Size; i++)' loop with:
	//     ImGuiListClipper clipper(Items.Size);
	//     while (clipper.Step())
	//         for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
	// However take note that you can not use this code as is if a filter is active because it breaks the 'cheap random-access' property. We would need random-access on the post-filtered list.
	// A typical application wanting coarse clipping and filtering may want to pre-compute an array of indices that passed the filtering test, recomputing this array when user changes the filter,
	// and appending newly elements as they are inserted. This is left as a task to the user until we can manage to improve this example code!
	// If your items are of variable size you may want to implement code similar to what ImGuiListClipper does. Or split your data into fixed height items to allow random-seeking into your list.
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
	if (copy_to_clipboard)
		ImGui::LogToClipboard();
	for (int i = 0; i < items.Size; i++)
	{
		const char* item = items[i];
		if (!filter.PassFilter(item))
			continue;
		ImVec4 col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		// Color for Messages
		if (strncmp(item, "# ", 2) == 0) col = ImColor(1.0f, 0.78f, 0.58f, 1.0f);
		else if (strstr(item, "Err")) col = ImColor(1.0f, 0.4f, 0.4f, 1.0f);
		else if (strstr(item, "error")) col = ImColor(1.0f, 0.4f, 0.4f, 1.0f);
		else if (strstr(item, "ERROR")) col = ImColor(1.0f, 0.4f, 0.4f, 1.0f);
		else if (strstr(item, "Warn")) col = ImColor(1.0f, 0.78f, 0.58f, 1.0f);
		else if (strstr(item, "warning")) col = ImColor(1.0f, 0.78f, 0.58f, 1.0f);
		else if (strstr(item, "WARNING")) col = ImColor(1.0f, 0.78f, 0.58f, 1.0f);
		else if (strstr(item, "Unknown")) col = ImColor(1.0f, 0.4f, 0.4f, 1.0f);

		ImGui::PushStyleColor(ImGuiCol_Text, col);
		ImGui::TextUnformatted(item);
		ImGui::PopStyleColor();
	}
	if (copy_to_clipboard)
		ImGui::LogFinish();
	if (scroll_to_bottom)
		ImGui::SetScrollHere();
	scroll_to_bottom = false;
	ImGui::PopStyleVar();
	ImGui::EndChild();
	ImGui::Separator();

	// Command-line
	if (ImGui::InputText("Input", input_buff, IM_ARRAYSIZE(input_buff), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackHistory, &TextEditCallbackStub, (void*)this))
	{
		char* input_end = input_buff + strlen(input_buff);
		while (input_end > input_buff && input_end[-1] == ' ') { input_end--; } *input_end = 0;
		if (input_buff[0])
			ExecCommand(input_buff);
		strcpy(input_buff, "");
	}

	// Demonstrate keeping auto focus on the input box
	if (ImGui::IsItemHovered() || (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0)))
		ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

	ImGui::End();
}

void EngineConsole::ExecCommand(const char * command_line)
{
	AddLog("# %s\n", command_line);

	// Insert into history. First find match and delete it so it can be pushed to the back. This isn't trying to be smart or optimal.
	history_pos = -1;
	for (int i = history.Size - 1; i >= 0; i--)
		if (Stricmp(history[i], command_line) == 0)
		{
			free(history[i]);
			history.erase(history.begin() + i);
			break;
		}
	history.push_back(Strdup(command_line));

	// Process command
	if (Stricmp(command_line, "CLEAR") == 0)
	{
		ClearLog();
	}
	else if (Stricmp(command_line, "HELP") == 0)
	{
		AddLog("Commands:");
		for (int i = 0; i < commands.Size; i++)
			AddLog("- %s", commands[i]);
	}
	else if (Stricmp(command_line, "HISTORY") == 0)
	{
		int first = history.Size - 10;
		for (int i = first > 0 ? first : 0; i < history.Size; i++)
			AddLog("%3d: %s\n", i, history[i]);
	}
	else if (Stricmp(command_line, "QUIT") == 0)
	{
		App->ExitRequest();
		LOG("Engine Console: Requested to Exit");
	}
	else
	{
		AddLog("Unknown command: '%s'\n", command_line);
	}
}

int EngineConsole::TextEditCallbackStub(ImGuiTextEditCallbackData * data)
// In C++11 you are better off using lambdas for this sort of forwarding callbacks
{
	EngineConsole* console = (EngineConsole*)data->UserData;
	return console->TextEditCallback(data);
}

int EngineConsole::TextEditCallback(ImGuiTextEditCallbackData * data)
{
	//AddLog("cursor: %d, selection: %d-%d", data->CursorPos, data->SelectionStart, data->SelectionEnd);
	if (data->EventFlag == ImGuiInputTextFlags_CallbackHistory)
	{
		// Example of HISTORY
		const int prev_history_pos = history_pos;
		if (data->EventKey == ImGuiKey_UpArrow)
		{
			if (history_pos == -1)
				history_pos = history.Size - 1;
			else if (history_pos > 0)
				history_pos--;
		}
		else if (data->EventKey == ImGuiKey_DownArrow)
		{
			if (history_pos != -1)
				if (++history_pos >= history.Size)
					history_pos = -1;
		}

		// A better implementation would preserve the data on the current input line along with cursor position.
		if (prev_history_pos != history_pos)
		{
			data->CursorPos = data->SelectionStart = data->SelectionEnd = data->BufTextLen = (int)snprintf(data->Buf, (size_t)data->BufSize, "%s", (history_pos >= 0) ? history[history_pos] : "");
			data->BufDirty = true;
		}
	}
	return 0;
}
