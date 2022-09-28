#pragma once

typedef unsigned VPANEL;

class IPanel
{
private:
	virtual void* __PAD00() = 0;
	virtual void* __PAD01() = 0;
	virtual void* __PAD02() = 0;
	virtual void* __PAD03() = 0;
	virtual void* __PAD04() = 0;
	virtual void* __PAD05() = 0;
	virtual void* __PAD06() = 0;
	virtual void* __PAD07() = 0;
	virtual void* __PAD08() = 0;
	virtual void* __PAD09() = 0;
	virtual void* __PAD10() = 0;
	virtual void* __PAD11() = 0;
	virtual void* __PAD12() = 0;
	virtual void* __PAD13() = 0;
	virtual void* __PAD14() = 0;
	virtual void* __PAD15() = 0;
	virtual void* __PAD16() = 0;
	virtual void* __PAD17() = 0;
	virtual void* __PAD18() = 0;
	virtual void* __PAD19() = 0;
	virtual void* __PAD20() = 0;
	virtual void* __PAD21() = 0;
	virtual void* __PAD22() = 0;
	virtual void* __PAD23() = 0;
	virtual void* __PAD24() = 0;
	virtual void* __PAD25() = 0;
	virtual void* __PAD26() = 0;
	virtual void* __PAD27() = 0;
	virtual void* __PAD28() = 0;
	virtual void* __PAD29() = 0;
	virtual void* __PAD30() = 0;
	virtual void* __PAD31() = 0;
	virtual void* __PAD32() = 0;
	virtual void* __PAD33() = 0;
	virtual void* __PAD34() = 0;
	virtual void* __PAD35() = 0;
public:
	virtual const char* GetName(VPANEL panel) = 0;
};