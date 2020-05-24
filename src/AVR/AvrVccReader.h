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
	static const int32_t InternalReferenceVoltage = 1100;

public:
	TemplateAvrVccReader() : TemplateAvrAdcReader<0x0E, PowerManagementEnabled, SamplingDuration, SettleDuration, Prescaler>()
	{}

	int32_t GetConverted(const int32_t reference = InternalReferenceVoltage, const uint16_t offset = CalibrationOffset)
	{
		return (reference / (GetValue() + offset)) + 1;
	}

protected:
	virtual void SetReference()
	{
		FullScaleAvrAdc::SetReferenceAvcc();
	}
};
#endif

