#ifndef URL_PARSER_H
#define URL_PARSER_H

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
	bool ValidXChar(std::string url);
	bool ValidCommonInternetUrl(std::string schemepart);

	std::map<std::string, std::string> parseCommonInternetScheme(std::string schemepart);
	std::map<std::string, std::string> parseNewsScheme(std::string schemepart);
	std::map<std::string, std::string> parseMailtoScheme(std::string schemepart);
};

#endif // URL_PARSER_H
