#pragma once

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <fstream>

#include "Club.hpp"
#include "Event.hpp"

using std::string, std::runtime_error, std::invalid_argument, std::out_of_range, std::ifstream,
    std::cerr;

// пространство имен Parser Function
namespace PF {

// Функция для парсинга данных о клубе из файла. Принимает ссылку на Клуб и заполняет его
int parseClubData(ifstream& inputFile, CCF::ComputerClub& club);
int parseClubData(std::istream& inputFile, CCF::ComputerClub& club);


int parseEventData(string& inputFile, EF::Event& event);
int parseEventData(std::istream& inputFile, EF::Event& event);


}  // namespace PF