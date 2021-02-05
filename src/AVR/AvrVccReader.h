// AvrVccReader.h

#if !defined(_AVR_VCC_READER_h) && defined(ARDUINO_ARCH_AVR)
#define _AVR_VCC_READER_h

#include "TemplateAvrAdcReader.h"

template<const bool PowerManagementEnabled = true,
	const uint32_t SamplingDuration = AdcSamplingPeriodDefault,
	const uint32_t SettleDuration = AdcSettlePeriodDefault,
	const PrescalerEnum Prescaler = PrescalerDefault>
	class TemplateAvrVccReader :
	public TemplateAvrAdcReader<0x0E, PowerManagementEnabled, SamplingDuration, SettleDuration, Prescaler>
{

private:
	static const int16_t CalibrationOffset = 1;
	static const uint16_t InternalReferenceVoltage = 1100;

public:
	TemplateAvrVccReader() : TemplateAvrAdcReader<0x0E, PowerManagementEnabled, SamplingDuration, SettleDuration, Prescaler>()
	{}

	uint16_t GetMilliVolt(const uint16_t reference = InternalReferenceVoltage, const int16_t offset = CalibrationOffset)
	{
		return ((reference * AdcRange)/ (GetValue() + offset));
	}

protected:
#ifdef ATTINY_CORE
	virtual void SetReference()
	{
		// TODO:
	}
#else
	virtual void SetReference()
	{
		FullScaleAvrAdc::SetReferenceAvcc();
	}
#endif
};
#endif

