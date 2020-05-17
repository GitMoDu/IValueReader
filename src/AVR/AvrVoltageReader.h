// AvrVoltageReader.h

#if !defined(_AVR_VOLTAGE_READER_h) && defined(ARDUINO_ARCH_AVR)
#define _AVR_VOLTAGE_READER_h

#include "TemplateAvrAdcReader.h"

template<const uint8_t Channel,
	const uint16_t RatioMultiplier,
	const uint16_t RatioDivisor,
	const bool PowerManagementEnabled = true,
	const uint32_t SamplingDuration = AdcSamplingPeriodDefault,
	const uint32_t SettleDuration = AdcSettlePeriodDefault,
	const PrescalerEnum Prescaler = PrescalerDefault,
	typename OutputType = uint32_t>
	class TemplateAvrVoltageReader :
	public TemplateAvrAdcReader<Channel, PowerManagementEnabled, SamplingDuration, SettleDuration, Prescaler, OutputType >
{

private:
	// TODO: Get from setup/constructor/template value.
	const int16_t CalibrationOffset = 0;
	const float InternalReferenceVoltage = 5.0;

	const uint32_t IRef = InternalReferenceVoltage * 1000;

public:
	TemplateAvrVoltageReader() : TemplateAvrAdcReader<Channel, PowerManagementEnabled, SamplingDuration, SettleDuration, Prescaler, OutputType>()
	{}

protected:
	virtual void SetReference()
	{
		FullScaleAvrAdc::SetReferenceAvcc();
	}

	uint32_t GetConverted(const uint16_t rawValue)
	{
		return (((rawValue * IRef) / FullScaleAvrAdc::AdcMax) * RatioDivisor) / RatioMultiplier;
	}
};
#endif

