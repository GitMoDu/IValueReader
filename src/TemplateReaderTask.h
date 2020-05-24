// TemplateReaderTask.h

#ifndef _TEMPLATE_READER_TASK_h
#define _TEMPLATE_READER_TASK_h

//#define DEBUG_VALUE_READER

#define _TASK_OO_CALLBACKS
#define _TASK_SLEEP_ON_IDLE_RUN
#include <TaskSchedulerDeclarations.h>


#include <IValueReader.h>
#include <IReaderTask.h>


static const uint32_t ReaderTaskDefaultPeriodMillis = 1000;

template<const uint32_t TargetPeriodMillis = ReaderTaskDefaultPeriodMillis,
	const uint8_t MaxReadersCount = 10>
	class TemplateReaderTask : public Task
	, public virtual IReaderTask
{
protected:
	enum ReaderStateEnum : uint8_t
	{
		Disabled,
		Reading,
		Restarting,
	} ReaderState = ReaderStateEnum::Disabled;

private:
	static const uint32_t BackOffDurationMillis = 1;

	IValueReader* ValueReaders[MaxReadersCount];
	uint8_t ReadersCount = 0;

	uint32_t ReadingStartMillis = 0;
	uint8_t ReaderIndex = 0;


#ifdef DEBUG_VALUE_READER
public:
	uint32_t ReadingDurationMillis = TargetPeriodMillis;
	const uint8_t MaxSteps = 10; //Sanity check for locked IReaders.
	uint8_t ReaderSteps = 0;
	uint32_t ReadCount = 0;
#endif

protected:
	virtual void OnReadingsDone() {}

public:
	TemplateReaderTask(Scheduler* scheduler)
		: Task(BackOffDurationMillis, TASK_FOREVER, scheduler, false)
		, IReaderTask()
	{
		ResetReader();
	}

	uint8_t GetReaderCount()
	{
		return ReadersCount;
	}

	bool AddValueReader(IValueReader* valueReader)
	{
		if (ReadersCount < MaxReadersCount)
		{
			ValueReaders[ReadersCount] = valueReader;
			ValueReaders[ReadersCount]->Clear();
			ReadersCount++;

			return true;
		}
		else
		{
			return false;
		}
	}

	void StartReading()
	{
		if (ReaderState == ReaderStateEnum::Disabled)
		{
			ReaderState = ReaderStateEnum::Restarting;
			Task::delay(BackOffDurationMillis);
			Task::enableIfNot();
		}
	}

	void StopReading()
	{
		if (ReaderState == ReaderStateEnum::Reading ||
			ReaderState == ReaderStateEnum::Restarting)
		{

			ReaderState = ReaderStateEnum::Disabled;
			Task::delay(BackOffDurationMillis);
			Task::enableIfNot();
		}
	}

	bool Callback()
	{
		switch (ReaderState)
		{
		case ReaderStateEnum::Disabled:
			for (uint8_t i = 0; i < ReadersCount; i++)
			{
				ValueReaders[ReaderIndex]->Stop();
			}
			Task::disable();

			break;
		case ReaderStateEnum::Restarting:
			ResetReader();
			Task::delay(BackOffDurationMillis);
			ReaderState = ReaderStateEnum::Reading;
			ReadingStartMillis = millis();
			break;
		case ReaderStateEnum::Reading:
			if (ReaderIndex < ReadersCount)
			{
				uint32_t StepDelay = BackOffDurationMillis;

				if (ValueReaders[ReaderIndex]->Read(StepDelay))
				{
					ReaderIndex++;
#ifdef DEBUG_VALUE_READER
					ReaderSteps = 0;
#endif
					Task::delay(BackOffDurationMillis);
				}
				else
				{
#ifdef DEBUG_VALUE_READER
					ReaderSteps++;
#endif
					StepDelay = max(StepDelay, BackOffDurationMillis);
					Task::delay(StepDelay);
				}
			}
			else
#ifdef DEBUG_VALUE_READER
				if (ReaderSteps > MaxSteps)
				{
					Task::delay(BackOffDurationMillis);
					Serial.print(F("Reader "));
					Serial.print(ReaderIndex);
					Serial.print(F(" is taking too many steps ("));
					Serial.print(ReaderSteps);
					Serial.println(F(" ) to finish."));
					ReaderIndex++;
				}
				else
#endif
				{
					OnReadingsDone();

#ifdef DEBUG_VALUE_READER
					ReadCount++;
					ReadingDurationMillis = Now - ReadingStartMillis;
#endif
					uint32_t Now = millis();


					// Round up the period to the last reading duration, if it exceeds the target period.
					uint32_t NextTargetMillis = ReadingStartMillis + max(TargetPeriodMillis, Now - ReadingStartMillis);

					if (Now >= NextTargetMillis)
					{
						Task::delay(BackOffDurationMillis);
					}
					else
					{
						Task::delay(NextTargetMillis - Now + BackOffDurationMillis);
					}

					ReaderState = ReaderStateEnum::Restarting;
				}
			break;
		default:
			Task::disable();
			break;
		}

		return true;
	}

private:
	void ResetReader()
	{
		ReaderIndex = 0;

#ifdef DEBUG_VALUE_READER
		ReadCount = 0;
		ReaderSteps = 0;
#endif
	}
};

#endif