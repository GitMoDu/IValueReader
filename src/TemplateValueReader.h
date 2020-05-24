// TemplateValueReader.h

#ifndef _IVALUEREADERTEMPLATE_h
#define _IVALUEREADERTEMPLATE_h

#include <IValueReader.h>

template <typename ValueType = uint16_t>
class TemplateValueReader : public virtual IValueReader
{
private:
	ValueType Value;

	bool ValidValue = false;

protected:
	void SetValue(const ValueType value)
	{
		Value = GetProcessed(value);
	}

	void SetValid(const bool valueGood)
	{
		ValidValue = valueGood;
	}

	virtual ValueType GetProcessed(const ValueType value)
	{
		return value;
	}

public:
	TemplateValueReader() : IValueReader()
	{
	}

	ValueType GetValue()
	{
		return Value;
	}

	bool IsValid()
	{
		return ValidValue;
	}

	virtual void Clear()
	{
		ValidValue = false;
	}
};
#endif