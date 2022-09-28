#pragma once

#include <string>
#include <deque>
#include <windows.h>

typedef void(__cdecl* tCommandFunction)(std::deque<std::string>);

class CConsoleCommand
{
private:
	std::string			m_Name;
	tCommandFunction	m_Func;
public:
	CConsoleCommand(std::string a_Name, tCommandFunction a_Func);

	tCommandFunction	GetFunction() const;
	std::string			GetName() const;
	void				CallFunction(std::deque<std::string> a_Args) const;
};

class CConsoleCommandHandler
{
private:
	std::deque<CConsoleCommand*> m_Commands;
public:
	CConsoleCommandHandler();
	~CConsoleCommandHandler();

	bool								RegisterCommand(std::string a_Name, tCommandFunction a_Func); // register command, returns false if command already exists, true otherwise
	bool								SendLine(std::string a_Line); // run command, returns false if command not exist, true otherwise
	std::deque<CConsoleCommand*>		GetCommands() const;
};

class CConsole
{
private:
	std::string					m_InputField;
	std::string					m_LastInputText;
	unsigned					m_InputFieldMax;

	std::deque<std::string>		m_History;
	unsigned					m_HistoryMax;

	int							m_CursorPos;

	CConsoleCommandHandler*		m_CommandHandler;

	bool						m_Enabled;
public:
	CConsole();
	CConsole(unsigned a_InputFieldMax, unsigned a_HistoryMax);
	~CConsole();

	void				Init(); // greeting, register basic commands

	void				Redraw(); // renders the console
	void				OnKey(WPARAM a_Key); // key press handler

	void				RegisterCommand(std::string a_Name, tCommandFunction a_Func);
	void				SendLine(std::string a_Line); // handles the input line
	void				Print(std::string a_Line);
	void				PrintF(const char* a_Line, ...);

	void				SetEnabled(bool a_Enabled);
	bool				GetEnabled() const;
	void				Toggle(); // enabled = !enabled

	void				Clear(); // clear history
};

extern CConsole* g_pConsole;