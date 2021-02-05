// TemplateValueReader.h

#ifndef _IVALUEREADERTEMPLATE_h
#define _IVALUEREADERTEMPLATE_h

#include <IValueReader.h>


class TemplateReader : public virtual IValueReader
{
private:
	bool ValidValue = false;

protected:
	void SetValid(const bool valueGood)
	{
		ValidValue = valueGood;
	}

public:
	TemplateReader() : IValueReader()
	{
	}

	const bool IsValid()
	{
		return ValidValue;
	}

	virtual void Clear()
	{
		ValidValue = false;
	}
};

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

	virtual const ValueType GetProcessed(const ValueType value)
	{
		return value;
	}

public:
	TemplateValueReader() : IValueReader()
	{
	}

	const ValueType GetValue()
	{
		return Value;
	}

	const bool IsValid()
	{
		return ValidValue;
	}

	virtual void Clear()
	{
		ValidValue = false;
	}
};

#endif