#ifndef UrlParser_H
#define UrlParser_H

#include <map>
#include <string>


class UrlParser
{
public:
	UrlParser();
	~UrlParser();
	
	// On invalid url return empty map
	std::map<std::string, std::string> parse(std::string url);

private:
	bool ValidURLCharacters(std::string url);
	bool ValidCommonInternetUrl(std::string schemepart);

	std::map<std::string, std::string> parseCommonInternetScheme(std::string schemepart);
	std::map<std::string, std::string> parseNewsScheme(std::string schemepart);
	std::map<std::string, std::string> parseMailtoScheme(std::string schemepart);
};

#endif // UrlParser_H
