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
	static const uint16_t InternalReferenceVoltage = 307;
	static const uint16_t ZeroDegreesKelvin = 27315; // -273.15

public:
	TemplateAvrTemperatureReader() : TemplateAvrAdcReader<0x08, PowerManagementEnabled, SamplingDuration, SettleDuration, Prescaler>()
	{
	}

	// TODO: Review
	int16_t GetCentiDegree(const uint16_t reference = InternalReferenceVoltage)
	{
		return (int16_t)(GetValue() - reference);
	}

	// TODO: Review
	uint16_t GetCentiKelvin(const uint16_t reference = InternalReferenceVoltage)
	{
		return (uint16_t)((int32_t)(GetValue() - reference) + ZeroDegreesKelvin);
	}

protected:
	virtual void SetReference()
	{
		FullScaleAvrAdc::SetReferenceInternal1100();
	}
};
#endif

