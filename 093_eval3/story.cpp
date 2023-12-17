#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <utility>

#include "cyoa.hpp"
// Default initalize the story with an empty vector
Story::Story() : pages() {
}
Story::~Story() {
}
Story & Story::operator=(const Story & rhs) {
  if (&rhs == this) {
    return *this;
  }
  pages = rhs.pages;
  return *this;
}
Story::Story(const Story & rhs) : pages(rhs.pages) {
}

void Story::addPage(const Page & newPage) {
  pages.push_back(newPage);
}
// parse the entire story, assembles story, pages, choices accordingly
Story Story::parseStory(const string & directory) {
  string filename = directory + "/story.txt";
  // open the story.txt file
  ifstream fs(filename.c_str());
  if (!fs.good()) {
    throw runtime_error("Fail to open " + filename);
  }
  // create the story
  Story story;
  // parse each line
  string temp_str;
  while (getline(fs, temp_str)) {
    story.parseLine(temp_str, directory);
  }

  fs.close();
  return story;
}
// parse a line in story.txt and perform the appropriate action
void Story::parseLine(const string & line, const string & directoryPrefix) {
  // check if a string is blank
  bool isBlank = true;
  for (size_t i = 0; i < line.length(); i++) {
    if (line[i] != ' ') {
      isBlank = false;
      break;
    }
  }
  if (isBlank)
    return;
  /* parse the line to determine it is for a choice or page */
  stringstream ss;
  ss << line;
  // parse the first integer
  size_t pageNum;
  if (!(ss >> pageNum)) {
    throw runtime_error("Fail to read the first integer of a line");
  }
  // parse the first delimiter
  char delimiterA;
  if (!(ss.get(delimiterA))) {
    throw runtime_error("Fail to read the first delimiter of a line");
  }
  switch (delimiterA) {
    case '@': {
      // Check the page number is valid, parse the page and add the page into story
      if (pageNum != pages.size()) {
        throw runtime_error("The page numbers are not in order");
      }
      addPage(Page::parsePage(line, directoryPrefix));
      break;
    }
    case '[':
    case ':': {
      // create a choice to the corresponding page
      const Choice & tmp = Choice::parseChoice(line);
      /* check the validity of the choice */
      if (tmp.getSource() >= pages.size()) {
        throw runtime_error(
            "A choice is defined in a page that has yet to be initialized");
      }
      char sourceType = pages[tmp.getSource()].getType();
      if (sourceType == 'W' || sourceType == 'L') {
        throw runtime_error("A choice in a win or loss page is unexpectedly added");
      }
      // add the choice into the correct page
      pages[pageNum].addChoice(tmp);
      break;
    }
    case '$': {
      /* check the validity of the variable */
      if (pageNum >= pages.size()) {
        throw runtime_error("A variable is added to a uninitialzied page");
      }
      string variable_name;
      long variable_value;
      getVariableFromStream(ss, variable_name, variable_value);
      pages[pageNum].addVariable(variable_name, variable_value);
      // check if extra character appears after the number
      char extraChar;
      ss.get(extraChar);
      if (!ss.fail()) {
        throw std::runtime_error("Extra characters found after variable value");
      }
      ss.clear();
      break;
    }
    default: {
      throw runtime_error("First delimiter of a line is incorrect, ASCII: " +
                          toString(delimiterA));
    }
  }
}

/* check if the story refers to nonexistant page, or without winning or losing page, or having redundant page */
void Story::postInstantiationCheck() {
  bool hasWinPage = false;
  bool hasLossPage = false;
  vector<bool> isReferred(pages.size(), false);

  if (!pages.empty()) {  // avoid empty story causes out-of-bound error
    isReferred[0] = true;
  }
  // mark the labels
  for (size_t i = 0; i < pages.size(); i++) {
    if (pages[i].getType() == 'W') {
      hasWinPage = true;
    }
    if (pages[i].getType() == 'L') {
      hasLossPage = true;
    }
    for (size_t j = 0; j < pages[i].getNumberOfChoices(); j++) {
      size_t dest = pages[i][j].getDest();
      if (dest >= pages.size()) {
        throw runtime_error("Nonexistant Page " + toString(dest) +
                            " is referred by some other choice");
      }
      // mark as referred when referred by a different page
      if (dest != pages[i][j].getSource()) {
        isReferred[dest] = true;
      }
    }
  }
  // check the labels set
  if (!hasWinPage) {
    throw runtime_error("No winning page");
  }
  if (!hasLossPage) {
    throw runtime_error("No losing page");
  }
  for (size_t i = 0; i < pages.size(); i++) {
    if (!isReferred[i]) {
      throw runtime_error("Page " + toString(i) + " is never referred by some page");
    }
  }
}
// Interactive reading with user
void Story::readInteractively() const {
  // the map contains all the user variables
  map<string, long> user_history;
  size_t currentIndex = 0;
  while (true) {
    // update the current page, user variables and display
    const Page & currentPage = pages[currentIndex];
    currentPage.attachVariableToUser(user_history);
    currentPage.displayPageToUser(user_history);
    // determine if finished and exit accordingly
    if (currentPage.getType() == 'W' || currentPage.getType() == 'L') {
      return;
    }
    // read a number from a user and check if it is a number
    while (true) {
      string userInput;
      char * endNumPointer;
      if (!(getline(cin, userInput))) {
        // if eof reached, report as an error.
        throw runtime_error("EOF is reached before the game is finished!");
      }
      // scan through the userInput to avoid overflow and convert it into size_t.
      // e.g. -18446744073709551606 will be cast into 10 by strtoul
      for (size_t i = 0; i < userInput.length(); i++) {
        if (userInput[i] == '-') {
          cout << "That is not a valid choice, please try again" << endl;
        }
      }
      size_t user_input_ll = strtoul(userInput.c_str(), &endNumPointer, 10);
      // make sure the input is an appropriate number and a valid choice from the current page
      if (user_input_ll > currentPage.getNumberOfChoices() || user_input_ll <= 0 ||
          *endNumPointer != '\0') {
        cout << "That is not a valid choice, please try again" << endl;
        // cout << "--------ERROR---------" << endl;
        // cout << "number read: " + toString(user_input_ll) << endl;
        cin.clear();
        continue;
      }
      else {  // input is valid, fetch the choice and update the page if available
        const Choice & choice = currentPage[user_input_ll - 1];
        if (choice.isAvailable(user_history)) {
          currentIndex = choice.getDest();
          break;
        }
        else {
          // case where the page is not available
          cout << "That choice is not available at this time, please try again" << endl;
          cin.clear();
        }
      }
    }
  }
}
// For a valid story, print all the winning paths without cycles
void Story::printWinningPaths() const {
  vector<vector<size_t> > container;
  // prefix for backtracking algorithm, recording all the index of choices being made
  vector<size_t> pre_decisions;
  // record whether each node is visited on the current path
  vector<bool> visited(pages.size(), false);
  visited[0] = true;
  // find all winning paths
  addWinningPathsGivenPrefix(pre_decisions, 0, visited, container);
  if (container.empty()) {
    cout << "This story is unwinnable!" << endl;
  }
  else {
    // for each winnning path print out the path
    for (size_t i = 0; i < container.size(); i++) {
      printPathToWin(container[i]);
    }
  }
}
// print out the path given the indexes of choices made
void Story::printPathToWin(vector<size_t> choices) const {
  size_t current = 0;
  size_t i = 0;
  while (i <= choices.size()) {
    cout << current;
    cout << '(';
    if (pages[current].getType() == 'W') {
      cout << "win)" << endl;
      return;
    }
    else if (pages[current].getType() == 'L') {
      throw runtime_error("Implementation Error: Lose page in winning path found!");
    }
    // case for regular page, print and jump to the next page
    cout << choices[i] + 1 << "),";
    current = pages[current][choices[i]].getDest();
    i++;
  }
}

// recursively adding all the path that starts with a vector of decisions
// backtracking algorithm is used (DFS with undos)
void Story::addWinningPathsGivenPrefix(vector<size_t> & pre_decisions,
                                       size_t currentPage,
                                       vector<bool> & visited,
                                       vector<vector<size_t> > & container) const {
  const Page & page = pages[currentPage];
  if (page.getType() == 'W') {
    vector<size_t> fullPath(pre_decisions);
    container.push_back(fullPath);
    return;
  }
  else if (page.getType() == 'L') {
    return;
  }
  // for a regular page, if a next-page is not in the path yet, find all winning paths with that next-page recursively
  for (size_t i = 0; i < page.getNumberOfChoices(); i++) {
    size_t nextPage = page[i].getDest();
    if (!visited[nextPage]) {
      visited[nextPage] = true;
      pre_decisions.push_back(i);
      addWinningPathsGivenPrefix(pre_decisions, nextPage, visited, container);
      pre_decisions.pop_back();
      visited[nextPage] = false;
    }
  }
}

ostream & operator<<(ostream & stream, const Story & story) {
  for (size_t i = 0; i < story.pages.size(); i++) {
    stream << "Page " << story.pages[i].getNum() << endl;
    stream << "==========" << endl;
    stream << story.pages[i];
  }
  return stream;
}
