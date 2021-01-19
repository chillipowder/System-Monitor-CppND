#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // Done: See src/processor.cpp

  // Done: Declare any necessary private members
 private:
  float cpu_perc_, total_, total_d, prev_total_{0}, prev_active_{0}, active_, active_d;
};

#endif