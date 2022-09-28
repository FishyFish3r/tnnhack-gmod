#pragma once

class IInputSystem
{
private:
	virtual void* Connect() = 0;
	virtual void* Disconnect() = 0;
	virtual void* QueryInterface() = 0;
	virtual void* Init() = 0;
	virtual void* Shutdown() = 0;
	virtual void* AttachToWindow() = 0;
	virtual void* DetachFromWindow() = 0;
public:
	virtual void EnableInput(bool enabled) = 0;
};