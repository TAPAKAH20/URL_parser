#include "url_parser.h"

#include <map>
#include <string>
#include <memory>
#include <algorithm>


UrlParser::UrlParser(){}
UrlParser::~UrlParser(){}
	

bool UrlParser::ValidXChar(std::string url){
	return std::all_of(url.begin(), url.end(),
		[](unsigned char c){return std::isalnum(c) || std::ispunct(c);});
}

//  Common Internet Scheme Syntax //<user>:<password>@<host>:<port>/<url-path>
bool UrlParser::ValidCommonInternetUrl(std::string schemepart){
	if(!ValidXChar(schemepart))
		return false;
	if(schemepart.length() < 5)//Smallest posible url schemepart is //a.c
		return false;
	if(schemepart.compare(0,2, "//", 0, 2) != 0)// Begins with //
		return false;
	if(schemepart.compare(0,3, "///", 0, 3) == 0)// Begins with //
		return false;


	std::string full_host;

	std::size_t host_end = schemepart.find('/', 3);
	std::size_t login_end = schemepart.find('@');
	if(login_end == schemepart.length()) // Host is not present after login
		return false;
	else if(login_end == std::string::npos)
		full_host = schemepart.substr(2, host_end);
	else
		full_host = schemepart.substr(login_end + 1, host_end);


	if(full_host.find('.') == std::string::npos || full_host.find('.') == full_host.length() || !ValidXChar(full_host))
		return false;

	// TODO

	return true;
}

//  Common Internet Scheme Syntax //<user>:<password>@<host>:<port>/<url-path>
std::map<std::string, std::string> UrlParser::parseCommonInternetScheme(std::string schemepart){
	std::map<std::string, std::string> url_map;

	if(!ValidCommonInternetUrl(schemepart))
		return url_map; //Invalid URL

	std::size_t i = 2;// Starting after "//"


	// Extract username and password if present
	//	<user>:<password>@
	std::size_t login_end = schemepart.find('@');
	if(login_end != std::string::npos){
		std::string login = schemepart.substr(2,login_end-2);
		std::size_t colon_index = login.find(':');
		std::string user = login.substr(0, colon_index);
		std::string password = "";

		//Non empty password
		if(colon_index != std::string::npos && colon_index < login.length())
			password = login.substr(colon_index+1);
		i += login.length()+1;

		if(i < schemepart.length()){
			url_map["user"] = user;
			url_map["password"] = password;
		}
	}


	// Findout if port is specified
	std::string host = "";
	std::string port = "";
	std::size_t colon_index = schemepart.find(':', i);
	if(colon_index != std::string::npos){
		host = schemepart.substr(i, colon_index - i);
		port = schemepart.substr(colon_index+1, schemepart.find('/', i) - i);
	}else{
		std::size_t ending_slash = schemepart.find('/', i);
		if(ending_slash == std::string::npos)
			host = schemepart.substr(i);
		else
			host = schemepart.substr(i, ending_slash - i);
	}

	url_map["host"] = host;
	url_map["port"] = port;

	// Add any path present
	std::size_t path_separator = schemepart.find('/', i);
	if(path_separator != schemepart.length())
		url_map["path"] = schemepart.substr(path_separator + 1);

	return url_map;
}

std::map<std::string, std::string> UrlParser::parseNewsScheme(std::string schemepart){
	std::map<std::string, std::string> url_map;

	if(!ValidXChar(schemepart))
		return url_map; //Invalid URL
	if(schemepart.length() < 1)
		return url_map; //Invalid URL


	std::size_t id_separator = schemepart.find('@');

	if(id_separator == std::string::npos)
		url_map["newsgroup-name"] = schemepart;
	else{
		if(id_separator == schemepart.length())
			return url_map; //Invalid URL
		url_map["message-id"] = schemepart.substr(0, id_separator);
		url_map["domain"] = schemepart.substr(id_separator+1);
	}

	return url_map;
}

std::map<std::string, std::string> UrlParser::parseMailtoScheme(std::string schemepart){
	std::map<std::string, std::string> url_map;

	if(!ValidXChar(schemepart))
		return url_map; //Invalid URL
	if(schemepart.length() < 1)
		return url_map; //Invalid URL

	url_map["addr"] = schemepart;

	return url_map;
}

// Determine the scheme and call corresponding function
std::map<std::string, std::string> UrlParser::parse(std::string url){

	std::map<std::string, std::string> url_map;

	// Extract Scheme from the URL
	std::size_t colon_index = url.find(':');
	if(colon_index == std::string::npos && colon_index == 0 && colon_index != url.length()-1){
		return url_map; //URL does not contain scheme separator
	}
	std::string scheme = url.substr(0, colon_index);
	std::string schemepart = url.substr(colon_index+1);

	//Handle schemes defined in RFC 1738
	if(scheme == "mailto")
		url_map = parseMailtoScheme(schemepart);
	else if(scheme =="news")
		url_map = parseMailtoScheme(schemepart);
	else
		url_map = parseCommonInternetScheme(schemepart);
		


	// If parsed succsesfully prepend scheme
	if(!url_map.empty())
		url_map.insert(url_map.begin(), std::pair<std::string, std::string>("scheme",scheme));

	return url_map;
}