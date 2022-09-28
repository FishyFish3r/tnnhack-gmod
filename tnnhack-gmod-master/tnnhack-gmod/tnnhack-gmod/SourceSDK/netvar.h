#pragma once

class RecvProp;

class CRecvProxyData
{
public:
	const RecvProp* m_pRecvProp;
	void*			m_Value;
	int				m_iElement;
	int				m_ObjectID;
};

typedef void (*RecvVarProxyFn)(const CRecvProxyData* pData, void* pStruct, void* pOut);
typedef void (*ArrayLengthRecvProxyFn)(void* pStruct, int objectID, int currentArrayLength);
typedef void (*DataTableRecvVarProxyFn)(const RecvProp* pProp, void** pOut, void* pData, int objectID);

class RecvTable;

class RecvProp
{
public:
	const char*				m_pVarName;
	void*					m_RecvType;
	int						m_Flags;
	int						m_StringBufferSize;
	bool					m_bInsideArray;
	const void*				m_pExtraData;
	RecvProp*				m_pArrayProp;
	ArrayLengthRecvProxyFn	m_ArrayLengthProxy;
	RecvVarProxyFn			m_ProxyFn;
	DataTableRecvVarProxyFn	m_DataTableProxyFn;
	RecvTable*				m_pDataTable;
	int						m_Offset;
	int						m_ElementStride;
	int						m_nElements;
	const char*				m_pParentArrayPropName;
};

typedef RecvProp PropType;
class CRecvDecoder;

class RecvTable
{
public:
	RecvProp*			m_pProps;
	int					m_nProps;
	CRecvDecoder*		m_pDecoder;
	const char*			m_pNetTableName;
	bool				m_bInitialized;
	bool				m_bInMainList;
};

class IClientNetworkable;

typedef IClientNetworkable* (*CreateClientClassFn)(int entnum, int serialNum);
typedef IClientNetworkable* (*CreateEventFn)();

class ClientClass
{
public:
	CreateClientClassFn		m_pCreateFn;
	CreateEventFn			m_pCreateEventFn;
	const char*				m_pNetworkName;
	RecvTable*				m_pRecvTable;
	ClientClass*			m_pNext;
	int						m_ClassID;
};