#pragma once
#include <regex>
#include <string>
using namespace std;
struct VAlidatorsintax
{
	bool validate(string command) {
		regex R_createtable ("(create  *table  *\w* *\( *.*  *.* *(, *.*  *.* *)*\);)");
		regex R_inserttable("(insert  *into  *\w* *\( *.*( *, *.* *)*\);)");
		regex R_updatetable("(update  *.*  *set  *.* *= *.*( *, *.* *= *.*)  *where  *.* *[<>=] *.* *;)");
		smatch matches;
		while (regex_search(command, matches, R_createtable)) {
			if (matches.size()) {
				return true;
			}
		}
		while (regex_search(command, matches, R_inserttable)) {
			if (matches.size()) {
				return true;
			}
		}
		while (regex_search(command, matches, R_updatetable)) {
			if (matches.size()) {
				return true;
			}
		}
		return false;
	}
};

