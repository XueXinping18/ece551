#include <fstream>
#include <sstream>
#include <stdexcept>
#include <utility>

#include "cyoa.hpp"

Choice::Choice() {
}
Choice::~Choice() {
}
Choice::Choice(const Choice & rhs) :
    message(rhs.message),
    source_num(rhs.source_num),
    dest_num(rhs.dest_num),
    condition(rhs.condition),
    conditional(rhs.conditional) {
}
Choice & Choice::operator=(const Choice & rhs) {
  if (&rhs == this) {
    return *this;
  }
  message = rhs.message;
  source_num = rhs.source_num;
  dest_num = rhs.dest_num;
  condition = rhs.condition;
  conditional = rhs.conditional;
  return *this;
}
const string & Choice::getMessage() const {
  return message;
}
size_t Choice::getSource() const {
  return source_num;
}
size_t Choice::getDest() const {
  return dest_num;
}
bool Choice::isAvailable(const map<string, long> user_history) const {
  // regular block, available to everyone
  if (!conditional) {
    return true;
  }
  // conditional block
  if (user_history.find(condition.first) == user_history.end()) {
    if (condition.second == 0) {
      return true;
    }
    else {
      return false;
    }
  }
  if (user_history.at(condition.first) != condition.second) {
    return false;
  }
  return true;
}
string Choice::getMessageToUser(const map<string, long> user_history) const {
  if (isAvailable(user_history)) {
    return message;
  }
  else {
    return "<UNAVAILABLE>";
  }
}
/* parse the line into a choice, without the validity of the choice being checked in the context of the story*/
Choice Choice::parseChoice(const string & line) {
  Choice choice;
  /* parse the line */
  stringstream ss;
  ss << line;
  // parse the source_num integer
  if (!(ss >> choice.source_num)) {
    throw runtime_error("Fail to read the source page number of a choice");
  };
  // parse the first delimiter
  char delimiterA;
  if (!(ss.get(delimiterA))) {
    throw runtime_error("Fail to read the first delimiter of a choice");
  }
  if (delimiterA != ':' && delimiterA != '[') {
    throw runtime_error("The first delimiter of a choice is parsed in wrong form");
  }
  // handle the conditional choice
  if (delimiterA == '[') {
    choice.conditional = true;
    string variable_name;
    long variable_value;
    getVariableFromStream(ss, variable_name, variable_value);
    choice.condition.first = variable_name;
    choice.condition.second = variable_value;
    char endBracket;
    if (!ss.get(endBracket) || endBracket != ']') {
      throw runtime_error("Failure to read ']' when parse conditional choice");
    }
    char colon;
    if (!ss.get(colon) || colon != ':') {
      throw runtime_error("Fail to read second colon when parse condition choice");
    }
  }
  else {
    // choice is non-conditional
    choice.conditional = false;
  }
  // parse the page type
  if (!(ss >> choice.dest_num)) {
    cout << line << endl;
    throw runtime_error("Fail to read the destination page number of a choice");
  };

  // parse the second delimiter
  char delimiterB;
  if (!(ss.get(delimiterB))) {
    throw runtime_error("Fail to read the second delimitor for a choice");
  }
  if (delimiterB != ':') {
    throw runtime_error("Second delimitor for a choice is incorrect");
  }
  // parse the remaining part into message
  if (!getline(ss, choice.message)) {
    throw runtime_error("Fail to extract message of a choice");
  }
  return choice;
}
ostream & operator<<(ostream & stream, const Choice & choice) {
  stream << choice.message;
  return stream;
}
