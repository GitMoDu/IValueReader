// ValueReaderTask.h

#ifndef _VALUEREADERTASK_h
#define _VALUEREADERTASK_h

#define DEBUG_VALUE_READER

#define _TASK_OO_CALLBACKS
#include <IValueReader.h>
#include <TaskSchedulerDeclarations.h>


template<const uint8_t MaxReadersCount = 32>
class ValueReaderTask : Task
{
private:
	static const uint32_t BACK_OFF_INTERVAL_MILLIS = 1;

	enum ReaderStateEnum : uint8_t
	{
		Reading,
		Done
	} ReaderState;

	enum ReadingStateEnum : uint8_t
	{
		Preparing,
		Acquiring
	} ReadingState;

	IValueReader* ValueReaders[MaxReadersCount];

	uint8_t ReadersCount = 0;

	uint8_t ReaderIndex = 0;

#ifdef DEBUG_VALUE_READER
	uint32_t ErrorCount = 0;
	uint32_t ReadingStartMillis = 0;
	uint32_t ReadingDurationMillis = 0;
	uint8_t DebugCount = 3;
#endif

private:
	void ResetReader()
	{
		ReaderIndex = 0;
		ReadingState = ReadingStateEnum::Preparing;
	}

public:
	ValueReaderTask(Scheduler* scheduler)
		: Task(BACK_OFF_INTERVAL_MILLIS, TASK_FOREVER, scheduler, false)
	{
		for (uint8_t i = 0; i < MaxReadersCount; i++)
		{
			ValueReaders[i] = nullptr;
		}

		ResetReader();
	}

	bool AddValueReader(IValueReader* valueReader)
	{
		if (valueReader != nullptr && ReadersCount < MaxReadersCount)
		{
			ValueReaders[ReadersCount] = valueReader;
			ReadersCount++;
			return true;
		}
		else
		{
			return false;
		}
	}

	uint8_t GetReaderCount()
	{
		return ReadersCount;
	}

	uint8_t GetMaxReaderCount()
	{
		return MaxReadersCount;
	}

	bool IsDone()
	{
		return ReaderState == ReaderStateEnum::Done;
	}

	void ReadAll()
	{
		ReaderState = ReaderStateEnum::Reading;
		enableIfNot();
#ifdef DEBUG_VALUE_READER
		ReadingStartMillis = millis();
#endif
	}

#ifdef DEBUG_VALUE_READER
	uint32_t GetReadingDuration()
	{
		return ReadingDurationMillis;
	}
#endif

	bool OnEnable()
	{
		return true;
	}

	void OnDisable()
	{
	}

	bool Callback()
	{
		switch (ReaderState)
		{
		case ReaderStateEnum::Done:
			disable();
			break;
		case ReaderStateEnum::Reading:
			switch (ReadingState)
			{
			case ReadingStateEnum::Preparing:
				if (ReaderIndex >= ReadersCount)
				{
					ReaderState = ReaderStateEnum::Done;
					ResetReader();
					Task::delay(BACK_OFF_INTERVAL_MILLIS);

#ifdef DEBUG_VALUE_READER
					ReadingDurationMillis = millis() - ReadingStartMillis;
					if (DebugCount > 0)
					{
						DebugCount--;
						Serial.print(F("Value reader for "));
						Serial.print(ReadersCount);
						Serial.print(F(" values took "));
						Serial.print(ReadingDurationMillis);
						Serial.println(F(" ms"));
					}
#endif
				}
				else if (ValueReaders[ReaderIndex]->Prepare())
				{
					ReadingState = ReadingStateEnum::Acquiring;
					Task::delay(ValueReaders[ReaderIndex]->GetDelay());
				}
				else
				{
					ReaderIndex++;
					Task::delay(BACK_OFF_INTERVAL_MILLIS);
#ifdef DEBUG_VALUE_READER
					ErrorCount++;
#endif
				}
				return true;
			case ReadingStateEnum::Acquiring:
				if (ValueReaders[ReaderIndex]->Acquire())
				{
					if (!ValueReaders[ReaderIndex]->GetValueReady())
					{
						ValueReaders[ReaderIndex]->SetValueReady(true);
					}
				}
				else {
#ifdef DEBUG_VALUE_READER
					ErrorCount++;
#endif
				}
				ReaderIndex++;
				ReadingState = ReadingStateEnum::Preparing;
				Task::delay(BACK_OFF_INTERVAL_MILLIS);
				return true;
			default:
				break;
			}
			break;
		default:
			disable();
			break;
		}
		return false;
	}
};

#endif