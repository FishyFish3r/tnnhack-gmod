#pragma once

enum ELuaSpecial
{
	ELuaSpecial_GLOBAL,
	ELuaSpecial_ENV,
	ELuaSpecial_REG
};

class ILuaInterface
{
public:
	virtual int Top() = 0;
	virtual void Push(int pos) = 0;
	virtual void Pop(int amount) = 0;
	virtual void GetTable(int pos) = 0;
	virtual void GetField(int pos, const char* name) = 0;
	virtual void SetField(int pos, const char* name) = 0;
	virtual void CreateTable() = 0;
	virtual void SetTable(int pos) = 0;
	virtual void SetMetaTable(int pos) = 0;
	virtual bool GetMetaTable(int pos) = 0;
	virtual void Call(int argc, int returnc) = 0;
	virtual int PCall(int argc, int returnc, int errorfuncid) = 0;
	virtual int Equal(int pos1, int pos2) = 0;
	virtual int RawEqual(int pos1, int pos2) = 0;
	virtual void Insert(int pos) = 0;
	virtual void Remove(int pos) = 0;
	virtual int Next(int pos) = 0;
	virtual void* NewUserdata(unsigned int size) = 0;
	virtual void ThrowError(const char* message) = 0;
	virtual void CheckType(int pos, int type) = 0;
	virtual void ArgError(int argc, const char* message) = 0;
	virtual void RawGet(int pos) = 0;
	virtual void RawSet(int pos) = 0;
	virtual const char* GetString(int pos = -1, unsigned int* outLen = nullptr) = 0;
	virtual double GetNumber(int pos = -1) = 0;
	virtual bool GetBool(int pos = -1) = 0;
	virtual void* GetCFunction(int pos = -1) = 0;
	virtual void* GetUserdata(int pos = -1) = 0;
	virtual void PushNil() = 0;
	virtual void PushString(const char* val, unsigned int len = 0) = 0;
	virtual void PushNumber(double val) = 0;
	virtual void PushBool(bool val) = 0;
	virtual void PushCFunction(void* val) = 0;
	virtual void PushCClosure(void* val, int varc) = 0;
	virtual void PushUserdata(void* udata) = 0;
	virtual int ReferenceCreate() = 0;
	virtual void ReferenceFree(int pos) = 0;
	virtual void ReferencePush(int pos) = 0;
	virtual void PushSpecial(ELuaSpecial type) = 0;
	virtual bool IsType(int pos, int type) = 0;
	virtual int GetType(int pos) = 0;
	virtual const char* GetTypeName(int type) = 0;
	virtual void CreateMetaTableType(const char* name, int type) = 0;
	virtual const char* CheckString(int pos = -1) = 0;
	virtual double CheckNumber(int pos = -1) = 0;
	virtual int ObjLen(int pos = -1) = 0;
	virtual QAngle& GetAngle(int pos = -1) = 0;
	virtual Vector3f& GetVector(int pos = -1) = 0;
	virtual void PushAngle(const QAngle& va) = 0;
	virtual void PushVector(const Vector3f& va) = 0;
};

enum ELuaInterfaceType
{
	ELuaInterfaceType_CLIENT,
	ELuaInterfaceType_SERVER,
	ELuaInterfaceType_MENU
};

class ILuaShared
{
private:
	virtual void* Init() = 0;
	virtual void* Shutdown() = 0;
	virtual void* DumpStats() = 0;
	virtual void* CreateLuaInterface() = 0;
	virtual void* CloseLuaInterface() = 0;
	virtual void* PAD() = 0;
public:
	virtual ILuaInterface* GetLuaInterface(ELuaInterfaceType type) = 0;
};