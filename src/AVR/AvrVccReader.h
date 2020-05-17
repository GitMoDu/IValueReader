// AvrVccReader.h

#if !defined(_AVR_VCC_READER_h) && defined(ARDUINO_ARCH_AVR)
#define _AVR_VCC_READER_h

#include "TemplateAvrAdcReader.h"

template<const bool PowerManagementEnabled = true,
	const uint32_t SamplingDuration = AdcSamplingPeriodDefault,
	const uint32_t SettleDuration = AdcSettlePeriodDefault,
	const PrescalerEnum Prescaler = PrescalerDefault,
	typename OutputType = int16_t>
	class TemplateAvrVccReader :
	public TemplateAvrAdcReader<0x0E, PowerManagementEnabled, SamplingDuration, SettleDuration, Prescaler, OutputType>
{

private:
	// TODO: Get from setup/constructor/template value.
	const int16_t CalibrationOffset = 0;
	const float InternalReferenceVoltage = 1.099;

	const uint32_t IRef = InternalReferenceVoltage * AdcRange * 1000;

public:
	TemplateAvrVccReader() : TemplateAvrAdcReader<0x0E, PowerManagementEnabled, SamplingDuration, SettleDuration, Prescaler, OutputType>()
	{}

protected:
	virtual void SetReference()
	{
		FullScaleAvrAdc::SetReferenceAvcc();
	}

	int16_t GetConverted(const uint16_t rawValue)
	{
		return (IRef / (rawValue + 1)) + 1 + CalibrationOffset;
	}
};
#endif

