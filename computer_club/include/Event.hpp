#pragma once

#include <iostream>
#include <stdexcept>
#include <string>

#include "Time.hpp"

using std::string, std::runtime_error, std::invalid_argument, std::out_of_range, std::cout,
    std::endl;

// пространство имен Event Function
namespace EF {

class Event {
 public:
  Event(){};

  void printEventInfo() const;

  void setEventTime(const TF::Time& startTime) { EventTime = startTime; }
  void setEventId(int id) { EnentId = id; }
  void setClientName(const string& clientName) { ClientName = clientName; }
  void setTableNumber(int numPC) { TableNumber = numPC; }

  TF::Time getEventTime() const { return EventTime; }
  int getEventId() const { return EnentId; }
  string getClientName() const { return ClientName; }
  int getTableNumber() const { return TableNumber; }

 private:
  TF::Time EventTime;
  int EnentId;
  string ClientName;
  int TableNumber;
};
}  // namespace EF