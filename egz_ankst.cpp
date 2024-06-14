#include <iostream>
#include <unordered_map>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <regex>
#include <set>

using std::string;
using std::ifstream;
using std::ofstream;
using std::stringstream;
using std::regex;
using std::regex_search;
using std::set;
using std::unordered_map;
using std::transform;

void countWords();
void writeUrls();

int main() {
    
    countWords();
    writeUrls();

    return 0;
}

void countWords() {
    unordered_map<string, int> wordCount;

    ifstream DF("df.txt");

    string line;
    string word;

    regex urlRegex(R"(\b(?:https?):\/\/[-A-Z0-9+&@#\/%?=~_|!:,.;]*[-A-Z0-9+&@#\/%=~_|]|(?:www\.)?[a-zA-Z0-9\-\.]+\.[a-zA-Z]{2,63}(?:\/[a-zA-Z0-9\-\/_?=]*)?)", regex::icase);    
    
    while(getline(DF, line)) {
        stringstream ss(line);

        while (ss >> word) {

            if (std::regex_search(word, urlRegex)) {
                continue;
            }

            word.erase (std::remove_if (word.begin (), word.end (), ispunct), word.end ());

            while (!word.empty() && word.size() >= 3 && (word.substr(0, 3) == "\xE2\x80\x93")) {
                word.erase(0, 3);
            }

            while (!word.empty() && word.size() >= 3 && (word.substr(word.length() - 3) == "\xE2\x80\x93")) {
                word.erase(word.length() - 3);
            }

            if (word == "\xE2\x80\x93") {
                word.clear();
            }

            transform(word.begin(), word.end(), word.begin(), ::tolower);

            if(!word.empty()) {
                ++wordCount[word];
            }
        }
    }

    DF.close();
    ofstream RF("rf.txt");

    for (const auto& entry : wordCount) {
        if(entry.second > 1) {
            RF << entry.first << ": " << entry.second << std::endl;
        }
    }
}

void writeUrls() {
    set<string> urls;

    ifstream DF("df.txt");
    ofstream RF("rfUrls.txt");

    string line;
    string word;

    regex urlRegex(R"(\b(?:https?):\/\/[-A-Z0-9+&@#\/%?=~_|!:,.;]*[-A-Z0-9+&@#\/%=~_|]|(?:www\.)?[a-zA-Z0-9\-\.]+\.[a-zA-Z]{2,63}(?:\/[a-zA-Z0-9\-\/_?=]*)?)", regex::icase);

    while(getline(DF, line)) {
        stringstream ss(line);

        while (ss >> word) {
            if (std::regex_search(word, urlRegex)) {
                urls.insert(word);
            }
        }
    }

    for(const auto& url : urls) {
        RF << url << std::endl;
    }

    DF.close();
    RF.close();
}