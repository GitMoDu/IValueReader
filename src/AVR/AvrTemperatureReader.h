// AvrTemperatureReader.h

#if !defined(_AVR_TEMPERATURE_READER_h) && defined(ARDUINO_ARCH_AVR)
#define _AVR_TEMPERATURE_READER_h

#include "TemplateAvrAdcReader.h"

template<const bool PowerManagementEnabled = true,
	const uint32_t SamplingDuration = AdcSamplingPeriodDefault,
	const uint32_t SettleDuration = AdcSettlePeriodDefault,
	const PrescalerEnum Prescaler = PrescalerDefault,
	typename OutputType = int16_t>
	class TemplateAvrTemperatureReader :
		public TemplateAvrAdcReader<0x08, PowerManagementEnabled, SamplingDuration, SettleDuration, Prescaler, OutputType>
{
private:
	// TODO: Get from setup/constructor/template value.
	const float InternalReferenceVoltage = 363.0;
	const int16_t IRef = InternalReferenceVoltage * (1024.0 / 1100.0);

public:
	TemplateAvrTemperatureReader() : TemplateAvrAdcReader<0x08, PowerManagementEnabled, SamplingDuration, SettleDuration, Prescaler, OutputType>()
	{
	}

protected:
	virtual void SetReference()
	{
		FullScaleAvrAdc::SetReferenceInternal1100();
	}

	int16_t GetConverted(const uint16_t rawValue)
	{
		return (int16_t)rawValue - IRef;
	}
};
#endif

