#include "Table.hpp"

#include <iostream>

string TF::Table::getTotalStaus() const {
  int hours = totalMinutes / 60;
  int minutes = totalMinutes % 60;
  Time totalTime(hours, minutes);

  return to_string(totalIncome) + " " + formatTime(totalTime);
}

void TF::Table::seatToTable(const string& client, const TF::Time& startTime) {
  clientName = client;
  entryTime = startTime;
}

void TF::Table::leaveFromTable(const TF::Time& endTime, int hourlyRate) {
  int duratioan = timeDiffMinutes(entryTime, endTime);

  //std::cout << "Duration: " << duratioan << std::endl;

  totalMinutes += duratioan;
  int income = (duratioan + 59) / 60 * hourlyRate;
  totalIncome += income;
  clientName = "";
  entryTime = TF::Time();
}

bool TF::Table::isTableFree() const { return clientName.empty(); }