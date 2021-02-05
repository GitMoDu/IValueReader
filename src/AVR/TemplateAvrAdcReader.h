// TemplateAvrAdcReader.h

#if !defined(_AVR_ADC_READER_h) && defined(ARDUINO_ARCH_AVR)
#define _AVR_ADC_READER_h

#include <Arduino.h>
#include <avr/power.h>

// https://github.com/GitMoDu/AVR-ADC-Wrapper
#include <AvrAdc.h>

#include "..\IValueReader.h"
#include "..\TemplateValueReader.h"



template<const uint8_t ADCChannel,
	const bool PowerManagementEnabled = true,
	const uint32_t SamplingDuration = AdcSamplingPeriodDefault,
	const uint32_t SettleDuration = AdcSettlePeriodDefault,
	const PrescalerEnum Prescaler = PrescalerDefault>
	class TemplateAvrAdcReader : public TemplateValueReader<uint16_t>
{
private:
	enum StateEnum : uint8_t
	{
		Preparing,
		Settling,
		Sampling,
	};
	
	StateEnum State = StateEnum::Preparing;

	static const uint32_t CheckPeriodMillis = 1;

public:
	TemplateAvrAdcReader() : TemplateValueReader<uint16_t>()
	{
		SetupPin();
	}

	// Read value, return true when done.
	// If false is returned, the next delay can be set.
	virtual const bool Read(uint32_t& delay)
	{
		switch (State)
		{
		case StateEnum::Preparing:
			State = StateEnum::Settling;
			delay = SettleDuration;
			Prepare();

			return false;
		case StateEnum::Settling:
			State = StateEnum::Sampling;
			delay = SamplingDuration;
			FullScaleAvrAdc::StartSample();

			return false;
		case StateEnum::Sampling:
			if (FullScaleAvrAdc::IsSampleDone())
			{
				SetValue(FullScaleAvrAdc::GetSample());
				SetValid(true);

				FullScaleAvrAdc::AdcOff();
				PowerOff();

				State = StateEnum::Preparing;

				return true;
			}
			else
			{
				delay = CheckPeriodMillis;
				return false;
			}
			break;
		default:
			return false;
		}
	}

	void Prepare()
	{
		PowerOn();
		FullScaleAvrAdc::AdcOn();
		SetReference();
		FullScaleAvrAdc::SetPrescaler(Prescaler);
		FullScaleAvrAdc::SetScale();
		FullScaleAvrAdc::SetChannel(ADCChannel);
	}

protected:
	virtual void SetReference() 
	{
		FullScaleAvrAdc::SetReferenceAvcc();
	}

	virtual void PowerOn()
	{
		if (PowerManagementEnabled)
		{
			power_adc_enable();
		}
	}

	virtual void PowerOff()
	{
		if (PowerManagementEnabled)
		{
			power_adc_disable();
		}
	}

protected:
	void SetupPin()
	{
		if (ADCChannel <= 0x05)
		{
			pinMode(A0 + ADCChannel, INPUT);
		}		
	}
};
#endif

