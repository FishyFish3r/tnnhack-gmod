#include "console.h"
#include "drawing.h"
#include "interface.h"
#include "sperma_cast.h"
#include "var.h"

CConsole* g_pConsole = nullptr;

void ClearCommandHandler(std::deque<std::string> args)
{
	g_pConsole->Clear();
}

CConsoleCommand::CConsoleCommand(std::string a_Name, tCommandFunction a_Func)
{
	this->m_Name = a_Name;
	this->m_Func = a_Func;
}

tCommandFunction CConsoleCommand::GetFunction() const
{
	return this->m_Func;
}

std::string CConsoleCommand::GetName() const
{
	return this->m_Name;
}

void CConsoleCommand::CallFunction(std::deque<std::string> a_Args) const
{
	if (!this->m_Func)
	{
		return;
	}

	(this->m_Func)(a_Args);
}

CConsoleCommandHandler::CConsoleCommandHandler()
{
	this->m_Commands = {};
}

CConsoleCommandHandler::~CConsoleCommandHandler()
{
	for (CConsoleCommand* c : this->m_Commands)
	{
		delete c;
	}
}

bool CConsoleCommandHandler::SendLine(std::string a_Line)
{
	if (a_Line.empty())
	{
		return false;
	}

	unsigned pos = a_Line.find(' ');
	unsigned initPos = 0;

	std::string command = "";
	std::deque<std::string> args = {};

	if (pos == std::string::npos)
	{
		command = a_Line;
		args.push_back(command);
	}
	else
	{
		command = a_Line.substr(0, pos);
		args.push_back(command);

		while (pos != std::string::npos)
		{
			initPos = pos + 1;
			pos = a_Line.find(' ', initPos);

			std::string arg = a_Line.substr(initPos, pos - initPos);
			args.push_back(arg);
		}
	}

	bool executed = false;

	for (CConsoleCommand* m : this->m_Commands)
	{
		if (m->GetName() == command)
		{
			m->CallFunction(args);
			executed = true;
			break;
		}
	}

	return executed;
}

bool CConsoleCommandHandler::RegisterCommand(std::string a_Name, tCommandFunction a_Func)
{
	for (CConsoleCommand* c : this->m_Commands)
	{
		if (c->GetName() == a_Name)
		{
			return false;
		}
	}

	this->m_Commands.push_back(new CConsoleCommand(a_Name, a_Func));
	return true;
}

std::deque<CConsoleCommand*> CConsoleCommandHandler::GetCommands() const
{
	return this->m_Commands;
}

CConsole::CConsole()
{
	this->m_InputField = "";
	this->m_LastInputText = "";
	this->m_InputFieldMax = 256;

	this->m_History = {};
	this->m_HistoryMax = 30;

	this->m_CursorPos = -1;

	this->m_CommandHandler = new CConsoleCommandHandler();
}

CConsole::CConsole(unsigned a_InputFieldMax, unsigned a_HistoryMax)
{
	this->m_InputField = "";
	this->m_LastInputText = "";
	this->m_InputFieldMax = a_InputFieldMax;

	this->m_History = {};
	this->m_HistoryMax = a_HistoryMax;

	this->m_CursorPos = -1;

	this->m_CommandHandler = new CConsoleCommandHandler();
}

void CConsole::Init()
{
	this->RegisterCommand("clear", &ClearCommandHandler);

#ifdef _DEBUG
	this->PrintF("TNNHACK GMOD, Build date: %s [DEBUG]", __DATE__);
#else
	this->PrintF("TNNHACK GMOD, Build date: %s", __DATE__);
#endif
}

/*
"\n\n.TTTTTTTTTTTTT.NNN.....NN.NNN.....NN...HH........HH...AAAAA....CCCCCCCC.KK.....KK.\n"
"......TTT......NN.N....NN.NN.N....NN...HH........HH..AA...AA..CC........KK....KK..\n"
"......TTT......NN..N...NN.NN..N...NN...HH........HH.AA.....AA.CC........KK...KK...\n"
"......TTT......NN...N..NN.NN...N..NN...HHHHHHHHHHHH.AAAAAAAAA.CC........KKKKKK....\n"
"......TTT......NN....N.NN.NN....N.NN...HH........HH.AA.....AA.CC........KK...KK...\n"
"......TTT......NN.....NNN.NN.....NNN...HH........HH.AA.....AA.CC........KK....KK..\n"
"......TTT......NN......NN.NN......NN...HH........HH.AA.....AA..CCCCCCCC.KK.....KK.\n"

"\n\n.TTTTTTTTTTTTT.NNN.....NN.NNN.....NN...HH........HH...AAAAA....CCCCCCCC.KK.....KK.\n......TTT......NN.N....NN.NN.N....NN...HH........HH..AA...AA..CC........KK....KK..\n......TTT......NN..N...NN.NN..N...NN...HH........HH.AA.....AA.CC........KK...KK...\n......TTT......NN...N..NN.NN...N..NN...HHHHHHHHHHHH.AAAAAAAAA.CC........KKKKKK....\n......TTT......NN....N.NN.NN....N.NN...HH........HH.AA.....AA.CC........KK...KK...\n......TTT......NN.....NNN.NN.....NNN...HH........HH.AA.....AA.CC........KK....KK..\n......TTT......NN......NN.NN......NN...HH........HH.AA.....AA..CCCCCCCC.KK.....KK.\n"
*/
CConsole::~CConsole()
{
	delete this->m_CommandHandler;
}

void CConsole::Redraw()
{
	Vector2i screenSize;
	Interfaces::Engine()->GetScreenSize(screenSize.x, screenSize.y);

	static float	consoleStartX = 0.f;
	float			consoleStartY = (screenSize.y / 2) + 200.f;
	float			consoleEndX = screenSize.x;
	float			consoleEndY = screenSize.y;

	// background

	g_pDrawing->PushColor(40, 40, 40, 110);
	g_pDrawing->DrawFilledRect(consoleStartX, consoleStartY, consoleEndX, consoleEndY);
	g_pDrawing->PushColor(3, 3, 3, 150);
	g_pDrawing->DrawFilledRect(consoleStartX, consoleEndY - 25.f, consoleEndX, consoleEndY);

	g_pDrawing->PushColor(208, 208, 208, 255);

	static float	x = consoleStartX + 3.f;
	float			y = consoleEndY - 40.f;

	// history

	for (std::string w : this->m_History)
	{
		if (y <= consoleStartY)
		{
			break;
		}

		g_pDrawing->DrawString(x, y, w.c_str());
		y -= CONSOLE_FONT_HEIGHT + 2.f;
	}

	// input field

	static DWORD cursorNextTick = 0;
	static bool cursorShouldShow = true;
	if (GetTickCount() >= cursorNextTick)
	{
		cursorNextTick = GetTickCount() + 400;
		cursorShouldShow = !cursorShouldShow;
	}

	std::string drawString = "> ";
	std::string drawStringInput = this->m_InputField;

	drawStringInput.insert(this->m_CursorPos + 1, 1, cursorShouldShow ? '_' : ' ');
	drawString += drawStringInput;

	g_pDrawing->PushColor(255, 255, 255, 255);
	g_pDrawing->DrawString(x, consoleEndY - (CONSOLE_FONT_HEIGHT + 6.f), drawString.c_str());

	// helper box

	if (!this->m_InputField.empty())
	{
		Vector2i drawStringSize = g_pDrawing->GetTextSize(drawString.c_str());
		drawStringSize.x += 2.f;

		float cmdY = consoleEndY - ((CONSOLE_FONT_HEIGHT + 6.f) / 2.f);
		static float boxWidth = 450.f;

		for (CConsoleCommand* cmd : this->m_CommandHandler->GetCommands())
		{
			if (cmd->GetName().rfind(this->m_InputField, 0) == 0)
			{
				if (cmdY <= consoleStartY)
				{
					break;
				}

				g_pDrawing->PushColor(3, 3, 3);
				g_pDrawing->DrawFilledRect(x + drawStringSize.x - 1.f, cmdY - (CONSOLE_FONT_HEIGHT + 2.f) - 1.f, x + drawStringSize.x + boxWidth, cmdY + 1.f);
				g_pDrawing->PushColor(40, 40, 40);
				g_pDrawing->DrawFilledRect(x + drawStringSize.x, cmdY - (CONSOLE_FONT_HEIGHT + 2.f), x + drawStringSize.x + boxWidth, cmdY);

				g_pDrawing->PushColor(255, 255, 255, 255);
				g_pDrawing->DrawString(x + drawStringSize.x, cmdY - (CONSOLE_FONT_HEIGHT + 2.f), cmd->GetName().c_str());

				cmdY -= CONSOLE_FONT_HEIGHT + 2.f;
			}
		}
	}
}

void CConsole::OnKey(WPARAM a_Key)
{
	switch (a_Key)
	{
		case(VK_RETURN):
		{
			if (!this->m_InputField.empty())
			{
				this->SendLine(this->m_InputField);
				this->m_LastInputText = this->m_InputField;
				this->m_InputField = "";
				this->m_CursorPos = -1;
			}
			break;
		}
		case(VK_TAB):
		{
			for (CConsoleCommand* cmd : this->m_CommandHandler->GetCommands())
			{
				if (cmd->GetName().rfind(this->m_InputField, 0) == 0 && cmd->GetName().size() < this->m_InputFieldMax)
				{
					this->m_InputField = cmd->GetName() + " ";
					this->m_CursorPos = zalypa_cast<int>(cmd->GetName().size());
					break;
				}
			}
			break;
		}
		case(VK_LEFT):
		{
			if (this->m_CursorPos > -1)
			{
				this->m_CursorPos--;
			}
			break;
		}
		case(VK_RIGHT):
		{
			if (this->m_CursorPos < zalypa_cast<int>(this->m_InputField.size()) - 1)
			{
				this->m_CursorPos++;
			}
			break;
		}
		case(VK_UP):
		{
			if (!this->m_LastInputText.empty() && this->m_LastInputText.size() < this->m_InputFieldMax)
			{
				this->m_InputField = this->m_LastInputText;
				this->m_CursorPos = zalypa_cast<int>(this->m_LastInputText.size()) - 1;
			}
			break;
		}
		case(VK_BACK):
		{
			if (this->m_InputField.size() > 0 && this->m_CursorPos > -1)
			{
				this->m_InputField.erase(this->m_CursorPos, 1);
				this->m_CursorPos--;
			}
			break;
		}
		default:
		{
			if (this->m_InputField.size() < this->m_InputFieldMax)
			{
				this->m_InputField.insert(this->m_CursorPos + 1, 1, zalypa_cast<char>(a_Key));
				this->m_CursorPos++;
			}
			break;
		}
	}
}

void CConsole::RegisterCommand(std::string a_Name, tCommandFunction a_Func)
{
	if (!this->m_CommandHandler->RegisterCommand(a_Name, a_Func))
	{
		std::string errorMsg = "Failed to register command " + a_Name + "!";
		MessageBoxA(NULL, errorMsg.c_str(), "Error", MB_OK | MB_ICONERROR);
	}
}

void CConsole::SendLine(std::string a_Line)
{
	this->Print("] " + a_Line);

	if (!this->m_CommandHandler->SendLine(a_Line))
	{
		this->Print("Unknown command!");
	}
}

void CConsole::Print(std::string a_Line)
{
	if (a_Line.empty())
	{
		return;
	}

	if (this->m_History.size() >= this->m_HistoryMax)
	{
		this->m_History.pop_back();
	}

	this->m_History.push_front(a_Line);
}

void CConsole::PrintF(const char* a_Line, ...)
{
	if (!a_Line)
	{
		return;
	}

	if (this->m_History.size() >= this->m_HistoryMax)
	{
		this->m_History.pop_back();
	}

	va_list varargs;
	char buf[256];

	va_start(varargs, a_Line);
	vsprintf(buf, a_Line, varargs);
	va_end(varargs);

	this->m_History.push_front(buf);
}

void CConsole::SetEnabled(bool a_Enabled)
{
	this->m_Enabled = a_Enabled;
}

bool CConsole::GetEnabled() const
{
	return this->m_Enabled;
}

void CConsole::Toggle()
{
	this->m_Enabled = !this->m_Enabled;
}

void CConsole::Clear()
{
	this->m_History.clear();
}