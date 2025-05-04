#include "Event.hpp"


void EF::Event::printEventInfo() const {
    cout << "Event ID: " << EnentId << endl;
    cout << "Event Time: " << TF::formatTime(EventTime) << endl;
    cout << "Client Name: " << ClientName << endl;
    cout << "Table Number: " << TableNumber << endl;
  }