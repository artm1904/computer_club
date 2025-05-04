#include "Time.hpp"

TF::Time::Time(int h = 0, int m = 0) : hour(h), minute(m) {}

bool TF::Time::operator<(const Time& other) const {
  if (hour != other.hour) {
    return hour < other.hour;
  }
  return minute < other.minute;
}

bool TF::Time::operator>=(const Time& other) const { return !(*this < other); }

TF::Time TF::parseTime(const string& timeStr) {
  size_t pos = timeStr.find(':');
  if (pos == string::npos || pos == 0 || pos == timeStr.length() - 1) {
    throw runtime_error("Invalid time format: " + timeStr);
  }

  int hour, minute;
  try {
    hour = stoi(timeStr.substr(0, pos));
    minute = stoi(timeStr.substr(pos + 1));
  } catch (const invalid_argument& e) {
    throw runtime_error("Invalid time format: " + timeStr);
  } catch (const out_of_range& e) {
    throw runtime_error("Time value out of range: " + timeStr);
  }

  if (hour < 0 || hour > 23 || minute < 0 || minute > 59) {
    throw runtime_error("Time value out of range: " + timeStr);
  }

  return Time(hour, minute);
}

string TF::formatTime(const Time& time) {
  stringstream ss;
  ss << setw(2) << setfill('0') << time.hour << ":" << setw(2) << setfill('0') << time.minute;
  return ss.str();
}

int TF::timeDiffMinutes(const Time& start, const Time& end) {
  return (end.hour - start.hour) * 60 + (end.minute - start.minute);
}
