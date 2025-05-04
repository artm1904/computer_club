#pragma once

#include <iostream>
#include <queue>
#include <set>
#include <vector>

#include "Event.hpp"
#include "Table.hpp"
#include "Time.hpp"

using std::endl, std::set, std::string, std::cout, std::queue, std::vector, std::cerr;

// Простронство имен ConputerClub Function
namespace CCF {
class ComputerClub {
 public:
  ComputerClub(){};

  void print() const;

  void processEvent(const EF::Event& event);

  void processEventID1(const EF::Event& event);
  void processEventID2(const EF::Event& event);
  void processEventID3(const EF::Event& event);
  void processEventID4(const EF::Event& event);

  void closeClub();

  int getTablesCount() const { return Tables_Count; }
  TF::Time getStartTime() const { return Start_Time; }
  TF::Time getEndTime() const { return End_Time; }
  int getHourRate() const { return Hour_Rate; }
  vector<TF::Table> getTables() const { return tables; }
  set<string> getClientsInTheClub() const { return clientsInTheClub; }
  queue<string> getClientsInQueue() const { return clientsInQueue; }

  void setTablesCount(int tablesCount) { Tables_Count = tablesCount; }
  void setStartTime(TF::Time startTime) { Start_Time = startTime; }
  void setEndTime(TF::Time endTime) { End_Time = endTime; }
  void setHourRate(int hourRate) { Hour_Rate = hourRate; }

 private:
  int Tables_Count;
  TF::Time Start_Time;
  TF::Time End_Time;
  int Hour_Rate;

  set<string> clientsInTheClub;
  vector<TF::Table> tables;
  queue<string> clientsInQueue;
};

void printErrorEvent13(const TF::Time& time, const std::string& message);

void printEvent11(const TF::Time& time, const string& name);
void printEvent12(const TF::Time& time, const string& name, const int& tableNumber);

}  // namespace CCF
