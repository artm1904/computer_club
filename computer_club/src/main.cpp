
#include <fstream>
#include <iostream>

#include "Club.hpp"
#include "Event.hpp"
#include "Parser.hpp"

using std::endl, std::cerr, std::getline, std::string, std::ifstream;

//===================================================main_func================================================================
int main(int argc, char* argv[]) {
  if (argc != 2) {
    cerr << "Usage for Windows: task.exe <input_file> \nUsage for Linux: "
            "./task <input_file>"
         << endl;
    return 1;
  }

  ifstream inputFile(argv[1]);
  if (!inputFile.is_open()) {
    cerr << "Error opening input file." << endl;
    return 1;
  }

  CCF::ComputerClub club;
  if (PF::parseClubData(inputFile, club) != 0) {
    cerr << "Error parsing club data." << endl;
    inputFile.close();
    return 1;
  }

  // std::cout << "Club data parsed successfully." << endl;
  // club.print();

  string line;
  // Чтение и обработка событий

  bool isFirstEvent = true;
  while (getline(inputFile, line)) {
    EF::Event event;

    if (isFirstEvent) {
      cout << TF::formatTime(club.getStartTime()) << endl;
      isFirstEvent = false;
    }

    if (PF::parseEventData(line, event) != 0) {
      cerr << "Error parsing enent data." << endl;
      inputFile.close();
      return 1;
    }

    // std::cout << "Event data parsed successfully." << endl;
    // event.printEventInfo();
    // std::cout << "========================================================================="
    // << endl;

    cout << line << endl;
    club.processEvent(event);
  }

  club.closeClub();

  int index =1;
  for (auto table : club.getTables()) {

    cout << index++ <<" "<< table.getTotalStaus() << endl;
  }

  
}
//===================================================main_func================================================================
