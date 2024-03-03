#include "Utils.hpp"
#include "server.hpp"

void my_trim_(std::string& s, char delimiter) {
    size_t p = s.find_first_not_of(delimiter);
    s.erase(0, p);
    p = s.find_last_not_of(delimiter);
    if (std::string::npos != p)
        s.erase(p + 1);
}

std::vector<std::string> split_space(const std::string &input) {
	//split by spaces
	std::vector<std::string> result;
	std::size_t start = 0;
	std::size_t found = input.find(' ');

	while (found != std::string::npos) {
		if (found != start)
			result.push_back(input.substr(start, found - start));
		start = found + 1;
		found = input.find(' ', start);
	}

	// Push the last part of the string if it's not empty
	std::string lastPart = input.substr(start);
	if (!lastPart.empty())
		result.push_back(lastPart);
	for (std::vector<std::string>::iterator it = result.begin(); it != result.end(); it++)
		my_trim_(*it, ' ');
	return result;
}

std::vector<std::string> split(const std::string input, const std::string &separator)
{
	std::cout << "splitting: \'" << input << "\' by \'"<< separator<<"\'" << std::endl;
	std::vector<std::string> result;
	std::size_t start = 0;
	std::size_t found = input.find(separator);

	while (found != std::string::npos)
	{
        // Only push non-empty substrings
		if (found != start)
        	result.push_back(input.substr(start, found - start));
		else
			result.push_back("");
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

std::string setPrefix(std::string hostname, std::string nickname, std::string realname)
{
	return (":" + nickname + "!" + realname + "@" + hostname);
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

std::string to_string(int i) {
	std::stringstream ss;
	ss << i;
	return ss.str();
}

void toUpper(std::string &str)
{
	for (size_t i = 0; i < str.length(); i++)
		str[i] = toupper(str[i]);
}

void Server::sendReply(const std::string &message, int clientFd)
{
	if (send(clientFd, message.c_str(), message.size(), 0) == -1)
		perror("send sys call failed: ");
}

int to_int(std::string i) {
	std::stringstream ss(i);
	int x;
	ss >> x;
	return x;
}