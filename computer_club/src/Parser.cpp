

#include "Parser.hpp"

#include "Time.hpp"

int PF::parseClubData(ifstream& inputFile, CCF::ComputerClub& club) {
  string line;

  int numTables;
  try {
    getline(inputFile, line);
    numTables = stoi(line);
    if (numTables <= 0) {
      throw runtime_error("Invalid number of tables: " + line);
    }
  } catch (const invalid_argument& e) {
    cerr << "Invalid number of tables: " + line << endl;
    return 1;
  } catch (const runtime_error& e) {
    cerr << e.what() << endl;
    return 1;
  }
  club.setTablesCount(numTables);

  string startTimeStr, endTimeStr;
  try {
    getline(inputFile, line);
    stringstream ss(line);
    ss >> startTimeStr >> endTimeStr;
    if (startTimeStr.empty() || endTimeStr.empty()) {
      throw runtime_error("Invalid start/end time format: " + line);
    }
  } catch (const runtime_error& e) {
    cerr << e.what() << endl;
    return 1;
  }

  try {
    club.setStartTime(TF::parseTime(startTimeStr));
    club.setEndTime(TF::parseTime(endTimeStr));
  } catch (const runtime_error& e) {
    cerr << e.what() << endl;
    return 1;
  }

  int hourlyRate;
  try {
    getline(inputFile, line);
    hourlyRate = stoi(line);
    if (hourlyRate <= 0) {
      throw runtime_error("Invalid hourly rate: " + line);
    }
  } catch (const invalid_argument& e) {
    cerr << "Invalid hourly rate:  " + line << endl;
    return 1;
  } catch (const runtime_error& e) {
    cerr << e.what() << endl;
    return 1;
  }
  club.setHourRate(hourlyRate);

  return 0;
};

int PF::parseClubData(std::istream& inputFile, CCF::ComputerClub& club) {
  string line;

  int numTables;
  try {
    getline(inputFile, line);
    numTables = stoi(line);
    if (numTables <= 0) {
      throw runtime_error("Invalid number of tables: " + line);
    }
  } catch (const invalid_argument& e) {
    cerr << "Invalid number of tables: " + line << endl;
    return 1;
  } catch (const runtime_error& e) {
    cerr << e.what() << endl;
    return 1;
  }
  club.setTablesCount(numTables);

  string startTimeStr, endTimeStr;
  try {
    getline(inputFile, line);
    stringstream ss(line);
    ss >> startTimeStr >> endTimeStr;
    if (startTimeStr.empty() || endTimeStr.empty()) {
      throw runtime_error("Invalid start/end time format: " + line);
    }
  } catch (const runtime_error& e) {
    cerr << e.what() << endl;
    return 1;
  }

  try {
    club.setStartTime(TF::parseTime(startTimeStr));
    club.setEndTime(TF::parseTime(endTimeStr));
  } catch (const runtime_error& e) {
    cerr << e.what() << endl;
    return 1;
  }

  int hourlyRate;
  try {
    getline(inputFile, line);
    hourlyRate = stoi(line);
    if (hourlyRate <= 0) {
      throw runtime_error("Invalid hourly rate: " + line);
    }
  } catch (const invalid_argument& e) {
    cerr << "Invalid hourly rate:  " + line << endl;
    return 1;
  } catch (const runtime_error& e) {
    cerr << e.what() << endl;
    return 1;
  }
  club.setHourRate(hourlyRate);

  return 0;
};

int PF::parseEventData(string& line, EF::Event& event) {
  stringstream ss(line);
  string timeStr, eventIdStr, clientName;

  try {
    if (!(ss >> timeStr >> eventIdStr >> clientName)) {
      throw runtime_error("Invalid event format: " + line);
    }
  } catch (const runtime_error& e) {
    cerr << e.what() << endl;
    return 1;
  }

  TF::Time eventTime;
  try {
    eventTime = TF::parseTime(timeStr);

  } catch (const runtime_error& e) {
    cerr << e.what() << endl;
    return 1;
  }

  int eventId;
  try {
    eventId = stoi(eventIdStr);
    if (eventId <= 0) {
      throw runtime_error("Invalid ID of event: " + line);
    }
  } catch (const invalid_argument& e) {
    cerr << "Invalid ID of event " + line << endl;
    return 1;
  } catch (const runtime_error& e) {
    cerr << e.what() << endl;
    return 1;
  }

  event.setEventTime(eventTime);
  event.setEventId(eventId);
  event.setClientName(clientName);

  string tableNameStr;
  int tableNumber;

  if (eventId == 2) {
    try {
      if (!(ss >> tableNameStr)) {
        throw runtime_error("Invalid table number format: " + line);
      }
    } catch (const runtime_error& e) {
      cerr << e.what() << endl;
      return 1;
    }

    try {
      tableNumber = stoi(tableNameStr);
      if (tableNumber <= 0) {
        throw runtime_error("Invalid table number: " + line);
      }
    } catch (const invalid_argument& e) {
      cerr << "Invalid table number format " + line << endl;
      return 1;
    } catch (const runtime_error& e) {
      cerr << e.what() << endl;
      return 1;
    }
    event.setTableNumber(tableNumber);
    return 0;
  }

  event.setTableNumber(0);
  return 0;
}
