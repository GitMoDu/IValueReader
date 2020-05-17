// TemplateValueReader.h

#ifndef _IVALUEREADERTEMPLATE_h
#define _IVALUEREADERTEMPLATE_h

#include <IValueReader.h>

template <typename OutputType,
	typename ValueType = uint16_t>
	class TemplateValueReader : public virtual IValueReader
{
private:
	OutputType Value;

	bool ValidValue = false;

protected:
	void SetValue(const ValueType value)
	{
		Value = GetConverted(GetProcessed(value));
	}

	void SetValid(const bool valueGood)
	{
		ValidValue = valueGood;
	}

	virtual OutputType GetConverted(const ValueType value)
	{
		return value;
	}

	virtual ValueType GetProcessed(const ValueType value)
	{
		return value;
	}

public:
	TemplateValueReader() : IValueReader()
	{
	}

	OutputType GetValue()
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