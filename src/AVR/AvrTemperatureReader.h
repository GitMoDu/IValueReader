// AvrTemperatureReader.h

#if !defined(_AVR_TEMPERATURE_READER_h) && defined(ARDUINO_ARCH_AVR)
#define _AVR_TEMPERATURE_READER_h

#include "TemplateAvrAdcReader.h"

template<const bool PowerManagementEnabled = true,
	const uint32_t SamplingDuration = AdcSamplingPeriodDefault,
	const uint32_t SettleDuration = AdcSettlePeriodDefault,
	const PrescalerEnum Prescaler = PrescalerDefault>
	class TemplateAvrTemperatureReader :
		public TemplateAvrAdcReader<0x08, PowerManagementEnabled, SamplingDuration, SettleDuration, Prescaler>
{
private:
	static const int16_t InternalReferenceVoltage = 307;

public:
	TemplateAvrTemperatureReader() : TemplateAvrAdcReader<0x08, PowerManagementEnabled, SamplingDuration, SettleDuration, Prescaler>()
	{
	}

	int16_t GetConverted(const int16_t reference = InternalReferenceVoltage)
	{
		return (int16_t)GetValue() - reference;
	}

protected:
	virtual void SetReference()
	{
		FullScaleAvrAdc::SetReferenceInternal1100();
	}
};
#endif

