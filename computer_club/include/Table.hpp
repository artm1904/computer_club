#pragma once

#include <string>

#include "Time.hpp"

using std::string, std::to_string;

namespace TF {

class Table {
 public:
  Table(){};

  string getTotalStaus() const;

  void seatToTable(const string& client, const Time& startTime);

  void leaveFromTable(const TF::Time& endTime, int hourlyRate);

  bool isTableFree() const;

  string getClientName() const { return clientName; }

 private:
  int totalMinutes;
  int totalIncome;
  string clientName="";
  Time entryTime;
};

}  // namespace TF
