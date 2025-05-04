#pragma once

#include <iomanip>
#include <stdexcept>
#include <string>

using std::string, std::runtime_error, std::invalid_argument, std::out_of_range, std::setfill,
    std::setw, std::stringstream;

// пространство имен Time Function
namespace TF {

// Структура для представления времени
class Time {
 public:
  int hour;
  int minute;
  Time(){};
  Time(int h, int m);

  // Перегрузка оператора сравнения
  // (меньше)
  bool operator<(const Time& other) const;

  // Перегрузка оператора сравнения
  // (больше или равно)
  bool operator>=(const Time& other) const;
};

// Функция для преобразования строки
// времени в структуру Time
Time parseTime(const string& timeStr);

// Функция для форматирования времени в строку
string formatTime(const Time& time);

// Функция для вычисления разницы во времени в минутах
int timeDiffMinutes(const Time& start, const Time& end);

}  // namespace TF