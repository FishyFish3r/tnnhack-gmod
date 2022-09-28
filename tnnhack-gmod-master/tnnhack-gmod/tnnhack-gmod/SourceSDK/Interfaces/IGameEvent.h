#pragma once

#ifdef CreateEvent
#undef CreateEvent
#endif

class IGameEvent2
{
public:
	virtual ~IGameEvent2() {};
	virtual const char* GetName() const = 0;
	virtual bool IsReliable() const = 0;
	virtual bool IsLocal() const = 0;
	virtual bool IsEmpty(const char* keyName = 0) = 0;
	virtual bool GetBool(const char* keyName = 0, bool defaultValue = false) = 0;
	virtual int GetInt(const char* keyName = 0, int defaultValue = 0) = 0;
	virtual float GetFloat(const char* keyName = 0, float defaultValue = 0.0f) = 0;
	virtual const char* GetString(const char* keyName = 0, const char* defaultValue = "") = 0;
	virtual void SetBool(const char* keyName, bool value) = 0;
	virtual void SetInt(const char* keyName, int value) = 0;
	virtual void SetFloat(const char* keyName, float value) = 0;
	virtual void SetString(const char* keyName, const char* value) = 0;
};

typedef void(__cdecl* tEventHandler)(IGameEvent2*);

class IGameEventListener2
{
private:
	tEventHandler m_PfnEventHandler;
public:
	IGameEventListener2()
	{
		this->m_PfnEventHandler = nullptr;
	}

	IGameEventListener2(tEventHandler a_Handler)
	{
		this->m_PfnEventHandler = a_Handler;
	}

	virtual	~IGameEventListener2(void) {};

	virtual void FireGameEvent(IGameEvent2* ev)
	{
		if (this->m_PfnEventHandler)
		{
			(this->m_PfnEventHandler)(ev);
		}
	}
};

class IGameEventManager2
{
public:
	virtual	~IGameEventManager2(void) {};
	virtual int LoadEventsFromFile(const char* filename) = 0;
	virtual void Reset() = 0;
	virtual bool AddListener(IGameEventListener2* listener, const char* name, bool bServerSide = false) = 0;
	virtual bool FindListener(IGameEventListener2* listener, const char* name) = 0;
	virtual void RemoveListener(IGameEventListener2* listener) = 0;
	virtual IGameEvent2* CreateEvent(const char* name, bool bForce = false) = 0;
	virtual bool FireEvent(IGameEvent2* event, bool bDontBroadcast = false) = 0;
	virtual bool FireEventClientSide(IGameEvent2* event) = 0;
	virtual IGameEvent2* DuplicateEvent(IGameEvent2* event) = 0;
	virtual void FreeEvent(IGameEvent2* event) = 0;
	virtual bool SerializeEvent(IGameEvent2* event, void* buf) = 0;
	virtual IGameEvent2* UnserializeEvent(void* buf) = 0;
};