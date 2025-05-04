#include <fstream>
#include <iostream>
#include <sstream>

#include "Club.hpp"
#include "Event.hpp"
#include "Parser.hpp"
#include "Time.hpp"
#include "gtest/gtest.h"

//------------------------------------------------------------------------------------------------------
// Захват потока cerr
std::string captureCerr(std::function<void()> func) {
  std::stringstream buffer;
  std::streambuf* oldCerr = std::cerr.rdbuf();
  std::cerr.rdbuf(buffer.rdbuf());
  func();

  std::cerr.rdbuf(oldCerr);
  return buffer.str();
}

// Захват всех потоков
std::string captureOstream(std::function<void()> func, std::ostream& stream) {
  std::stringstream buffer;
  std::streambuf* oldStream = stream.rdbuf();
  stream.rdbuf(buffer.rdbuf());

  func();

  stream.rdbuf(oldStream);
  return buffer.str();
}

//------------------------------------------------------------------------------------------------------
// Создаем клуб и устанавливаем начальные параметры
class ComputerClubTest : public ::testing::Test {
 protected:
  void SetUp() override {
    club.setTablesCount(3);
    club.setStartTime(TF::Time(9, 0));
    club.setEndTime(TF::Time(19, 0));
    club.setHourRate(10);
  }

  CCF::ComputerClub club;
};
//------------------------------------------------------------------------------------------------------
TEST_F(ComputerClubTest, ProcessEventID1_ValidEntry) {
  EF::Event event;
  event.setEventId(1);
  event.setEventTime(TF::Time(10, 0));
  event.setClientName("Alice");

  club.processEvent(event);
  ASSERT_EQ(club.getClientsInTheClub().count("Alice"), 1);
}
//------------------------------------------------------------------------------------------------------
TEST_F(ComputerClubTest, ProcessEventID1_EntryBeforeOpening) {
  EF::Event event;
  event.setEventId(1);
  event.setEventTime(TF::Time(8, 0));
  event.setClientName("Bob");

  std::string cerrOutput = captureCerr([&]() { club.processEvent(event); });

  ASSERT_NE(cerrOutput.find("NotOpenYet"), std::string::npos);
  ASSERT_EQ(club.getClientsInTheClub().count("Bob"), 0);
}
//------------------------------------------------------------------------------------------------------
TEST_F(ComputerClubTest, ProcessEventID2_ClientUnknown) {
  EF::Event event;
  event.setEventId(2);
  event.setEventTime(TF::Time(11, 0));
  event.setClientName("Charlie");
  event.setTableNumber(1);

  std::string cerrOutput = captureCerr([&]() { club.processEvent(event); });

  ASSERT_NE(cerrOutput.find("ClientUnknown"), std::string::npos);
}
//------------------------------------------------------------------------------------------------------
TEST_F(ComputerClubTest, ProcessEventID2_PlaceIsBusy) {
  EF::Event event1;
  event1.setEventId(1);
  event1.setEventTime(TF::Time(10, 0));
  event1.setClientName("Alice");
  club.processEvent(event1);

  EF::Event event2;
  event2.setEventId(2);
  event2.setEventTime(TF::Time(10, 15));
  event2.setClientName("Alice");
  event2.setTableNumber(1);
  club.processEvent(event2);

  EF::Event event3;
  event3.setEventId(1);
  event3.setEventTime(TF::Time(10, 20));
  event3.setClientName("Bob");
  club.processEvent(event3);

  EF::Event event4;
  event4.setEventId(2);
  event4.setEventTime(TF::Time(10, 30));
  event4.setClientName("Bob");
  event4.setTableNumber(1);

  std::string cerrOutput = captureCerr([&]() { club.processEvent(event4); });

  ASSERT_NE(cerrOutput.find("PlaceIsBusy"), std::string::npos);
}
//------------------------------------------------------------------------------------------------------
TEST_F(ComputerClubTest, ProcessEventID2_ValidSeating) {
  EF::Event event1;
  event1.setEventId(1);
  event1.setEventTime(TF::Time(10, 0));
  event1.setClientName("Alice");
  club.processEvent(event1);

  EF::Event event2;
  event2.setEventId(2);
  event2.setEventTime(TF::Time(10, 15));
  event2.setClientName("Alice");
  event2.setTableNumber(1);

  club.processEvent(event2);
  ASSERT_EQ(club.getTables()[0].getClientName(), "Alice");
}
//------------------------------------------------------------------------------------------------------
TEST_F(ComputerClubTest, ProcessEventID3_HaveFreeTable) {
  EF::Event event1;
  event1.setEventId(1);
  event1.setEventTime(TF::Time(10, 0));
  event1.setClientName("Alice");
  club.processEvent(event1);

  EF::Event event3;
  event3.setEventId(3);
  event3.setEventTime(TF::Time(10, 30));
  event3.setClientName("Alice");
  std::string cerrOutput = captureCerr([&]() { club.processEvent(event3); });
  ASSERT_NE(cerrOutput.find("ICanWaitNoLonger"), std::string::npos);
}
//------------------------------------------------------------------------------------------------------
TEST_F(ComputerClubTest, ProcessEventID3_ClientNotInClub) {
  EF::Event event3;
  event3.setEventId(3);
  event3.setEventTime(TF::Time(10, 30));
  event3.setClientName("Alice");

  club.processEvent(event3);

  ASSERT_EQ(club.getClientsInQueue().size(), 0);
  ASSERT_EQ(club.getClientsInTheClub().count("Alice"), 0);
}
//------------------------------------------------------------------------------------------------------
TEST_F(ComputerClubTest, ProcessEventID4_ClientLeaves) {
  EF::Event event1;
  event1.setEventId(1);
  event1.setEventTime(TF::Time(10, 0));
  event1.setClientName("Alice");
  club.processEvent(event1);

  EF::Event event2;
  event2.setEventId(2);
  event2.setEventTime(TF::Time(10, 15));
  event2.setClientName("Alice");
  event2.setTableNumber(1);
  club.processEvent(event2);

  EF::Event event4;
  event4.setEventId(4);
  event4.setEventTime(TF::Time(11, 0));
  event4.setClientName("Alice");
  club.processEvent(event4);

  ASSERT_EQ(club.getClientsInTheClub().count("Alice"), 0);
  ASSERT_TRUE(club.getTables()[0].isTableFree());
}
//------------------------------------------------------------------------------------------------------
class ParserTest : public ::testing::Test {};
//------------------------------------------------------------------------------------------------------
TEST(ParserTest, ParseClubData_ValidData) {
  std::stringstream input;
  input << "2\n";
  input << "09:00 19:00\n";
  input << "50\n";

  CCF::ComputerClub club;
  int result = PF::parseClubData(input, club);
  ASSERT_EQ(result, 0);

  ASSERT_EQ(club.getTablesCount(), 2);
  ASSERT_EQ(club.getStartTime().hour, 9);
  ASSERT_EQ(club.getEndTime().hour, 19);
  ASSERT_EQ(club.getHourRate(), 50);
}
//------------------------------------------------------------------------------------------------------
TEST(ParserTest, ParseClubData_InvalidNumTables) {
  std::stringstream input;
  input << "0\n";
  input << "09:00 19:00\n";
  input << "50\n";

  CCF::ComputerClub club;
  std::string cerrOutput =
      captureOstream([&]() { PF::parseClubData(input, club); }, std::cerr);

  ASSERT_NE(cerrOutput.find("Invalid number of tables"), std::string::npos);
}
//------------------------------------------------------------------------------------------------------
TEST(ParserTest, ParseClubData_InvalidTimeFormat) {
  std::stringstream input;
  input << "2\n";
  input << "invalid_time\n";
  input << "50\n";

  CCF::ComputerClub club;
  std::string cerrOutput =
      captureOstream([&]() { PF::parseClubData(input, club); }, std::cerr);

  ASSERT_NE(cerrOutput.find("Invalid start/end time format: invalid_time"), std::string::npos);
}
//------------------------------------------------------------------------------------------------------
TEST(ParserTest, ParseClubData_InvalidHourlyRate) {
  std::stringstream input;
  input << "2\n";
  input << "09:00 19:00\n";
  input << "-50\n";

  CCF::ComputerClub club;
  std::string cerrOutput =
      captureOstream([&]() { PF::parseClubData(input, club); }, std::cerr);

  ASSERT_NE(cerrOutput.find("Invalid hourly rate"), std::string::npos);
}
//------------------------------------------------------------------------------------------------------
TEST(ParserTest, ParseEventData_ValidData) {
  std::string line = "09:15 1 Alice";
  EF::Event event;
  int result = PF::parseEventData(line, event);
  ASSERT_EQ(result, 0);

  ASSERT_EQ(event.getEventTime().hour, 9);
  ASSERT_EQ(event.getEventId(), 1);
  ASSERT_EQ(event.getClientName(), "Alice");
}
//------------------------------------------------------------------------------------------------------
TEST(ParserTest, ParseEventData_InvalidEventFormat) {
  std::string line = "invalid_event";
  EF::Event event;
  std::string cerrOutput =
      captureOstream([&]() { PF::parseEventData(line, event); }, std::cerr);

  ASSERT_NE(cerrOutput.find("Invalid event format"), std::string::npos);
}
//------------------------------------------------------------------------------------------------------
TEST(ParserTest, ParseEventData_InvalidTimeFormat) {
  std::string line = "invalid_time 1 Alice";
  EF::Event event;
  std::string cerrOutput =
      captureOstream([&]() { PF::parseEventData(line, event); }, std::cerr);

  ASSERT_NE(cerrOutput.find("Invalid time format"), std::string::npos);
}
//------------------------------------------------------------------------------------------------------
TEST(ParserTest, ParseEventData_InvalidEventId) {
  std::string line = "09:15 -1 Alice";
  EF::Event event;
  std::string cerrOutput =
      captureOstream([&]() { PF::parseEventData(line, event); }, std::cerr);

  ASSERT_NE(cerrOutput.find("Invalid ID of event"), std::string::npos);
}
//------------------------------------------------------------------------------------------------------
TEST(ParserTest, ParseEventData_ValidEvent2) {
  std::string line = "09:15 2 Alice 1";
  EF::Event event;
  int result = PF::parseEventData(line, event);
  ASSERT_EQ(result, 0);

  ASSERT_EQ(event.getEventTime().hour, 9);
  ASSERT_EQ(event.getEventId(), 2);
  ASSERT_EQ(event.getClientName(), "Alice");
  ASSERT_EQ(event.getTableNumber(), 1);
}
//------------------------------------------------------------------------------------------------------
TEST(ParserTest, ParseEventData_InvalidTableNumberFormat) {
  std::string line = "09:15 2 Alice invalid_table";
  EF::Event event;
  std::string cerrOutput =
      captureOstream([&]() { PF::parseEventData(line, event); }, std::cerr);

  ASSERT_NE(cerrOutput.find("Invalid table number format"), std::string::npos);
}
//------------------------------------------------------------------------------------------------------
TEST(ParserTest, ParseEventData_InvalidTableNumber) {
  std::string line = "09:15 2 Alice -1";
  EF::Event event;
  std::string cerrOutput =
      captureOstream([&]() { PF::parseEventData(line, event); }, std::cerr);

  ASSERT_NE(cerrOutput.find("Invalid table number"), std::string::npos);
}
//------------------------------------------------------------------------------------------------------
int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}