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
 long long int totalMinutes=0;
 long long int totalIncome=0;
  string clientName="";
  Time entryTime;
};

}  // namespace TF
