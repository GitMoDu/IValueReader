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
	const PrescalerEnum Prescaler = PrescalerDefault>
	class TemplateAvrVoltageReader :
	public TemplateAvrAdcReader<Channel, PowerManagementEnabled, SamplingDuration, SettleDuration, Prescaler>
{

private:
	static const int16_t CalibrationOffset = 0;
	static const int32_t InternalReferenceVoltage = 5000;

public:
	TemplateAvrVoltageReader() : TemplateAvrAdcReader<Channel, PowerManagementEnabled, SamplingDuration, SettleDuration, Prescaler>()
	{}


	int32_t GetConverted(const uint32_t reference = InternalReferenceVoltage, const int16_t offset = CalibrationOffset)
	{		
		return (int32_t)(((((GetValue() + offset) * reference) * RatioDivisor) / FullScaleAvrAdc::AdcMax)) / RatioMultiplier;
	}

protected:
	virtual void SetReference()
	{
		FullScaleAvrAdc::SetReferenceAvcc();
	}
};
#endif

