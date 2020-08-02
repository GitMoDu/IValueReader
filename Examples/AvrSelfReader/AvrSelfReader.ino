/*
	Name:       AvrSelfReader.ino
	Example showing AVR internal temperature and supply voltage,
	and 2 generic voltage readers.
	Depends on AvrADC wrapper : https://github.com/GitMoDu/AVR-ADC-Wrapper
*/

#define SERIAL_BAUD_RATE 115200


#define _TASK_OO_CALLBACKS
#define _TASK_SLEEP_ON_IDLE_RUN
#include <TaskScheduler.h>


//#define DEBUG_VALUE_READER

#include <ValueReaderTask.h>
#include <ValueReadersAVR.h>
#include <ReaderLowPass.h>

///Process scheduler.
Scheduler SchedulerBase;
///

TemplateLowPassReader16<TemplateAvrTemperatureReader<>, 2>  TemperatureReader;
TemplateAvrVccReader<> VccReader;
TemplateAvrVoltageReader<0, 1, 1> Reader0; // Simple reader.
TemplateAvrVoltageReader<1, 3, 5> Reader1; // Higher voltage measure.

#define ValueReaderTaskClass TemplateReaderTask<100, 4>

ValueReaderTaskClass ReadersTask(&SchedulerBase);

class TestTaskClass : public Task
{
private:
	ValueReaderTaskClass* ReaderTask = nullptr;

	bool DebuggedDuration = false;

public:
	TestTaskClass(Scheduler* scheduler, ValueReaderTaskClass* readerTask)
		: Task(1000, TASK_FOREVER, scheduler, true)
	{
		ReaderTask = readerTask;
	}

	bool Callback()
	{
		if (TemperatureReader.IsValid() &&
			VccReader.IsValid() &&
			Reader0.IsValid() &&
			Reader1.IsValid())
		{
#ifdef DEBUG_VALUE_READER
			Serial.print(F("Actual duration: "));
			Serial.print(ReaderTask->ReadingDuration);
			Serial.print(F(" ms for "));
			Serial.print(ReaderTask->ReaderCount);
			Serial.println(F(" value readers."));
#endif

			Serial.print(F("Temperature: "));
			Serial.print(TemperatureReader.GetConverted(325));
			Serial.println(F(" C"));

			Serial.print(F("Vcc: "));
			Serial.print(VccReader.GetConverted());
			Serial.println(F(" mV"));

			Serial.print(F("ADC0 value: "));
			Serial.print(Reader0.GetConverted());
			Serial.println(F(" mV"));

			Serial.print(F("ADC1 value: "));
			Serial.print(Reader1.GetConverted());
			Serial.println(F(" mV"));

			return;
		}
		else
		{
			Serial.println(F("Values Not ready."));
		}


		return true;
	}
} TestTask(&SchedulerBase, &ReadersTask);

void setup()
{
	Serial.begin(SERIAL_BAUD_RATE);


	TemperatureReader.ResetFilter(350); // Set the starting filter value closer to real value.

	// Using interface pointer for testing.	
	IReaderTask* ReaderTaskPointer = &ReadersTask;

	ReaderTaskPointer->AddValueReader(&TemperatureReader);
	ReaderTaskPointer->AddValueReader(&VccReader);
	ReaderTaskPointer->AddValueReader(&Reader0);
	ReaderTaskPointer->AddValueReader(&Reader1);

	bitSet(DIDR0, ADC0D);  // disable digital buffer on A0
	bitSet(DIDR0, ADC1D);  // disable digital buffer on A1
	bitSet(DIDR0, ADC2D);  // disable digital buffer on A2
	bitSet(DIDR0, ADC3D);  // disable digital buffer on A3
	bitSet(DIDR0, ADC4D);  // disable digital buffer on A4
	bitSet(DIDR0, ADC5D);  // disable digital buffer on A5

	ReadersTask.StartReading();
}

void loop()
{
	SchedulerBase.execute();
}
