#include "url_parser.h"

#include <map>
#include <string>
#include <memory>
#include <algorithm>
#include <cctype>


UrlParser();
~UrlParser();

bool ValidURLCharacters(std::string url){
	// TODO
	return true;
}

//  Common Internet Scheme Syntax //<user>:<password>@<host>:<port>/<url-path>
bool ValidCommonInternetUrl(std::string schemepart){
	if(!ValidURLCharacters(schemepart))
		return false;
	if(schemepart.length < 5)//Smallest posible url schemepart is //a.c
		return false;
	if(schemepart.compare(0,2, "//", 0, 2) != 0)// Begins with //
		return false;


	std::string full_host;

	std::size_t host_end = schemepart.find('/', 3);
	std::size_t login_end = schemepart.find('@');
	if(login_end == schemepart.length()) // Host is present after login
		return false;


	

	// TODO

	return true;
}

//  Common Internet Scheme Syntax //<user>:<password>@<host>:<port>/<url-path>
std::map<std::string, std::string> parseCommonInternetScheme(std::string schemepart){
	std::map<std::string, std::string> url_map;

	if(!ValidCommonInternetUrl(schemepart))
		return url_map;

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
		host = schemepart.substr(i, colon_index);
		port = schemepart.substr(colon_index+1, schemepart.find('/', i));
	}else{
		host = schemepart.substr(i, schemepart.find('/', i));
	}

	url_map["host"] = host;
	url_map["port"] = port;

	// Add any path present
	std::size_t path_separator = schemepart.find('/', i)
	if(path_separator != schemepart.length())
		url_map["url_path"] = schemepart.substr(path_separator + 1);

	return url_map;
}

std::map<std::string, std::string> parseNewsScheme(std::string schemepart);

std::map<std::string, std::string> parseMailtoScheme(std::string schemepart);

std::map<std::string, std::string> parse(std::string url){

	std::map<std::string, std::string> url_map;

	// Extract Scheme from the URL
	std::size_t colon_index = url.find(':');
	if(colon_index == std::string::npos && colon_index == 0 && colon_index != url.length()-1){
		return url_map; //URL does not contain scheme separator
	}
	std::string scheme = url.substr(0, colon_index);
	std::string scheme_part = url.substr(colon_index+1);

	//Handle schemes defined in RFC 1738
	switch(scheme){
		case "mailto":
		url_map = parseMailtoScheme(schemepart);
		break;

		case "news":
		url_map = parseMailtoScheme(schemepart);
		break;

		
		default:
		url_map = parseCommonInternetScheme(schemepart);
		case "http":
		case "https":
		case "ftp":
		case "gopher":
		case "ss":
		case "telnet":
		case "wais":
		case "rs":
		case "file":
		case "prospero":
	}

	if(!url_map.isEmpty())
		url_map.insert(url_map.begin(), std::pair<std::string, std::string>("scheme",scheme));

	return url_map;
}