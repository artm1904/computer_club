#include "Club.hpp"

void CCF::ComputerClub::print() const {
  cout << "Tables Count: " << Tables_Count << endl;
  cout << "Start Time: " << Start_Time.hour << ":" << Start_Time.minute << endl;
  cout << "End Time: " << End_Time.hour << ":" << End_Time.minute << endl;
  cout << "Hourly Rate: " << Hour_Rate << endl;
}

void CCF::ComputerClub::processEvent(const EF::Event& event) {
  if (clientsInTheClub.empty()) {
    tables.resize(Tables_Count);
  }

  switch (event.getEventId()) {
    case 1:
      processEventID1(event);
      break;
    case 2:
      processEventID2(event);
      break;
    case 3:
      processEventID3(event);
      break;
    case 4:
      processEventID4(event);
      break;

    default:
      cerr << "Invalid client action!!! " << endl;
  }
}

void CCF::printErrorEvent13(const TF::Time& time, const std::string& message) {
  cerr << TF::formatTime(time) << " 13 " << message << endl;
}

void CCF::printEvent11(const TF::Time& time, const string& name) {
  cerr << TF::formatTime(time) << " 11 " << name << endl;
}

void CCF::printEvent12(const TF::Time& time, const string& name, const int& tableNumber) {
  cerr << TF::formatTime(time) << " 12 " << name << " " << tableNumber << endl;
}

void CCF::ComputerClub::processEventID1(const EF::Event& event) {
  if (clientsInTheClub.count(event.getClientName())) {
    printErrorEvent13(event.getEventTime(), "YouShallNotPass");
  } else if (event.getEventTime() < Start_Time || event.getEventTime() >= End_Time) {
    printErrorEvent13(event.getEventTime(), "NotOpenYet");
  } else {
    clientsInTheClub.insert(event.getClientName());
  }
}

void CCF::ComputerClub::processEventID2(const EF::Event& event) {
  if (clientsInTheClub.count(event.getClientName()) == 0) {
    printErrorEvent13(event.getEventTime(), "ClientUnknown");
  } else if (event.getTableNumber() < 1 ||
             (long unsigned int)event.getTableNumber() > tables.size() ||
             !tables[event.getTableNumber() - 1].isTableFree()) {
    printErrorEvent13(event.getEventTime(), "PlaceIsBusy");

  } else {
    // Логика смены стола следующая: нужно найти стол за которым клиент сидит, осободить его, и
    // занять новый. Если желаемый стол занят, то нужно добавить клиента в очередь.

    for (auto it = tables.begin(); it != tables.end(); ++it) {
      if (it->getClientName() == event.getClientName()) {
        it->leaveFromTable(event.getEventTime(), Hour_Rate);
        break;
      }
    }

    tables[event.getTableNumber() - 1].seatToTable(event.getClientName(),
                                                   event.getEventTime());
  }
}

void CCF::ComputerClub::processEventID3(const EF::Event& event) {
  bool haveFreeTable = false;
  for (auto it = tables.begin(); it != tables.end(); ++it) {
    if (it->isTableFree()) {
      haveFreeTable = true;
      break;
    }
  }

  if (haveFreeTable) {
    printErrorEvent13(event.getEventTime(), "ICanWaitNoLonger");
  } else if (clientsInQueue.size() >= tables.size()) {
    printEvent11(event.getEventTime(), event.getClientName());
    clientsInTheClub.erase(event.getClientName());
  } else {
    clientsInQueue.push(event.getClientName());
  }
}

void CCF::ComputerClub::processEventID4(const EF::Event& event) {
  if (clientsInTheClub.count(event.getClientName()) == 0) {
    printErrorEvent13(event.getEventTime(), "ClientUnknown");
  } else {
    auto it = tables.begin();
    for (; it != tables.end(); ++it) {
      if (it->getClientName() == event.getClientName()) {
        it->leaveFromTable(event.getEventTime(), Hour_Rate);
        break;
      }
    }

    clientsInTheClub.erase(event.getClientName());

    queue<string> tempQueue;
    // Если в очереди был этот клиенты, то нужно его найти и убрать из очереди.
    while (!clientsInQueue.empty()) {
      string currentValue = clientsInQueue.front();
      clientsInQueue.pop();

      if (currentValue != event.getClientName()) {
        tempQueue.push(currentValue);
      }
    }

    // Переносим элементы обратно в исходную очередь
    while (!tempQueue.empty()) {
      clientsInQueue.push(tempQueue.front());
      tempQueue.pop();
    }

    // Может быть такое, что ушедший клиент совсем не сидел за столом, поэтому нельзя сажать
    // клиента их очереди за несуществующий стол
    if (it == tables.end()) {
      return;
    }

    if (!clientsInQueue.empty()) {
      string client = clientsInQueue.front();
      clientsInQueue.pop();
      it->seatToTable(client, event.getEventTime());

      printEvent12(event.getEventTime(), client, it - tables.begin() + 1);

      std::queue<string> displayQueue = clientsInQueue;
    }
  }
}

void CCF::ComputerClub::closeClub() {
  string lastClient;

  for (auto it = tables.begin(); it != tables.end(); ++it) {
    if (!(it->isTableFree())) {
      lastClient = it->getClientName();

      it->leaveFromTable(End_Time, Hour_Rate);
    }
  }

  for (auto it = clientsInTheClub.begin(); it != clientsInTheClub.end(); ++it) {
    printEvent11(End_Time, *it);
  }

  cout << TF::formatTime(End_Time) << endl;
}
