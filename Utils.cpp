#include "Utils.hpp"

void my_trim_(std::string& s, char delimiter) {
    size_t p = s.find_first_not_of(delimiter);
    s.erase(0, p);
    p = s.find_last_not_of(delimiter);
    if (std::string::npos != p)
        s.erase(p + 1);
}

std::vector<std::string> split(const std::string &input, const std::string &separator)
{
	std::vector<std::string> result;
	std::size_t start = 0;
	std::size_t found = input.find(separator);

	while (found != std::string::npos)
	{
        // Only push non-empty substrings
		if (found != start)
            result.push_back(input.substr(start, found - start));
		start = found + separator.size(); // Move past the separator
		found = input.find(separator, start);
	}

	// Push the last part of the string if it's not empty
	std::string lastPart = input.substr(start);
	if (!lastPart.empty())
		result.push_back(lastPart);
    for (std::vector<std::string>::iterator it = result.begin(); it != result.end(); it++)
        my_trim_(*it, ' ');
	return result;
}

std::string setPrefix(std::string hostname, std::string nickname, std::string username)
{
	return (":" + nickname + "!" + username + "@" + hostname);
}

bool isNumber(const std::string &s)
{
	for (std::string::const_iterator it = s.begin(); it != s.end(); it++)
	{
		if (!std::isdigit(*it))
			return false;
	}
	return true;
}