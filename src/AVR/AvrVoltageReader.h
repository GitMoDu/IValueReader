// AvrVoltageReader.h

#if !defined(_AVR_VOLTAGE_READER_h) && defined(ARDUINO_ARCH_AVR)
#define _AVR_VOLTAGE_READER_h

#include "TemplateAvrAdcReader.h"

template<const uint8_t Channel,
	const uint8_t RatioMultiplier,
	const uint8_t RatioDivisor,
	const bool PowerManagementEnabled = true,
	const uint32_t SamplingDuration = AdcSamplingPeriodDefault,
	const uint32_t SettleDuration = AdcSettlePeriodDefault,
	const PrescalerEnum Prescaler = PrescalerDefault>
	class TemplateAvrVoltageReader :
	public TemplateAvrAdcReader<Channel, PowerManagementEnabled, SamplingDuration, SettleDuration, Prescaler>
{

private:
	static const uint16_t InternalReferenceVoltage = 5000;
	static const int16_t CalibrationOffset = 0;

public:
	TemplateAvrVoltageReader() : TemplateAvrAdcReader<Channel, PowerManagementEnabled, SamplingDuration, SettleDuration, Prescaler>()
	{}

	// TODO: Review
	const uint16_t GetMilliVolt(const uint16_t reference = InternalReferenceVoltage, const int16_t offset = CalibrationOffset)
	{		
		return (((((uint32_t)(GetValue() + offset) * reference) * RatioMultiplier) / FullScaleAvrAdc::AdcMax)) / RatioDivisor;
	}
};
#endif

