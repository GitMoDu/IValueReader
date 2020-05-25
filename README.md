# IValueReader

Template value reader with background task.



Value Reader:
  - Common Interface (IValueReader).
  - Provided template.


Value Reader Task:
  - Depends on TaskScheduler (depends on https://github.com/arkhipenko/TaskScheduler).
  - Tries to run all readers in the background, once every TargetPeriod.
  - Extensible with OnReadingsUpdated(), called once per cycle.


Included Value readers:
  - AVR Generic ADC
  - AVR VCC.
  - AVR Temperature.


Included reader processors:
  - Low Pass Filter (depends on https://github.com/GitMoDu/IFilter)
