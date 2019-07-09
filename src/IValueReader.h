// IValueReader.h

#ifndef _IVALUEREADER_h
#define _IVALUEREADER_h

#include <stdint.h>

class IValueReader
{
private:
	bool ValueReady = false;

public:
	virtual bool Prepare() { return false; }
	virtual bool Acquire() { return false; }
	virtual uint8_t GetDelay() const { return 0; };
	virtual uint16_t GetId() { return 0; }


public:
	IValueReader()
	{
		ValueReady = false;
	}

	bool HasValueReady()
	{
		return ValueReady;
	}

	void SetValueReady(const bool valueReady)
	{
		ValueReady = valueReady;
	}
};
#endif