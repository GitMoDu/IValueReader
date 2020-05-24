// TemplateLowPassReader.h

#ifndef _LOW_PASS_FILTER_VALUE_READER_h
#define _LOW_PASS_FILTER_VALUE_READER_h

#include <LowPassFilter.h>


template<class TemplateValueReader,
	typename FilterClass,
	typename ValueType>
	class AbstractLowPassReader : public TemplateValueReader
{
private:
	FilterClass Filter;

public:
	AbstractLowPassReader() : TemplateValueReader()
		, Filter()
	{
	}

	void ResetFilter(ValueType value)
	{
		Filter.ForceReset(value);
	}

protected:
	virtual ValueType GetProcessed(const ValueType value)
	{
		Filter.Set(value);
		Filter.Step();

		return Filter.Get();
	}
};

// Filters have different implementations for different bit-widths, so these templates just match them.
template<class TemplateValueReader,
	const uint8_t FilterFactor = 4>
	class TemplateLowPassReader8 : public AbstractLowPassReader<TemplateValueReader, LowPassFilter8<FilterFactor>, uint8_t>
{
public:
	TemplateLowPassReader8()
		: AbstractLowPassReader<TemplateValueReader, LowPassFilter8<FilterFactor>, uint8_t>()
	{}
};

template<class TemplateValueReader,
	const uint8_t FilterFactor = 4>
	class TemplateLowPassReader16 : public AbstractLowPassReader<TemplateValueReader, LowPassFilter16<FilterFactor>, uint16_t>
{
public:
	TemplateLowPassReader16()
		: AbstractLowPassReader<TemplateValueReader, LowPassFilter16<FilterFactor>, uint16_t>()
	{}
};

template<class TemplateValueReader,
	const uint8_t FilterFactor = 4>
	class TemplateLowPassReader32 : public AbstractLowPassReader<TemplateValueReader, LowPassFilter32<FilterFactor>, uint32_t>
{
public:
	TemplateLowPassReader32()
		: AbstractLowPassReader<TemplateValueReader, LowPassFilter32<FilterFactor>, uint32_t>()
	{}
};
#endif