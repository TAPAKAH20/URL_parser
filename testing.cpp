#include "url_parser.h"

#include <iostream>
#include <gtest/gtest.h>


class UrlParserTest : public ::testing::Test
{
protected:
	UrlParser* url_parser;
	void SetUp(){
		url_parser = new UrlParser();
	}

	void TearDown(){
		delete url_parser;
	}
};

// Universal tests

TEST_F(UrlParserTest, EmptyUrl){
	std::string url = "";
	std::map<std::string, std::string> url_map = url_parser->parse(url);
	EXPECT_EQ(url_map.empty(), true);
}

TEST_F(UrlParserTest, NoScheme){
	std::string url = "://asd.cae";
	std::map<std::string, std::string> url_map = url_parser->parse(url);
	EXPECT_EQ(url_map.empty(), true);
}


// Common Internet Scheme tests

TEST_F(UrlParserTest, InvalidSymbol){
	std::string url = "http:///host at.com";
	std::map<std::string, std::string> url_map = url_parser->parse(url);
	EXPECT_EQ(url_map.empty(), true);
}

TEST_F(UrlParserTest, ExtraSlash){
	std::string url = "http:///host.com";
	std::map<std::string, std::string> url_map = url_parser->parse(url);
	EXPECT_EQ(url_map.empty(), true);
}

TEST_F(UrlParserTest, LessSlashes){
	std::string url = "http:/host.com";
	std::map<std::string, std::string> url_map = url_parser->parse(url);
	EXPECT_EQ(url_map.empty(), true);
}

TEST_F(UrlParserTest, EmptyHost){
	std::string url = "http:/user:pass@";
	std::map<std::string, std::string> url_map = url_parser->parse(url);
	EXPECT_EQ(url_map.empty(), true);
}

TEST_F(UrlParserTest, HttpEndingSlash){
	std::string url = "http://host.com/";
	std::map<std::string, std::string> url_map = url_parser->parse(url);
	EXPECT_EQ(url_map["scheme"], "http");
	EXPECT_EQ(url_map["host"], "host.com");
}

TEST_F(UrlParserTest, HttpSpecifiedPort){
	std::string url = "http://host.com:934";
	std::map<std::string, std::string> url_map = url_parser->parse(url);
	EXPECT_EQ(url_map["host"], "host.com");
	EXPECT_EQ(url_map["port"], "934");
}

TEST_F(UrlParserTest, HttpSpecifiedUserAndPass){
	std::string url = "http://user:pass@host.com/";
	std::map<std::string, std::string> url_map = url_parser->parse(url);
	EXPECT_EQ(url_map["user"], "user");
	EXPECT_EQ(url_map["password"], "pass");
	EXPECT_EQ(url_map["host"], "host.com");
}

TEST_F(UrlParserTest, HttpEmptyPass){
	std::string url = "http://user:@host.com/";
	std::map<std::string, std::string> url_map = url_parser->parse(url);
	EXPECT_EQ(url_map["user"], "user");
	EXPECT_EQ(url_map["password"], "");
	EXPECT_EQ(url_map["host"], "host.com");
}

TEST_F(UrlParserTest, HttpEmptyUserAndPassword){
	std::string url = "http://@host.com/";
	std::map<std::string, std::string> url_map = url_parser->parse(url);
	EXPECT_EQ(url_map["user"], "");
	EXPECT_EQ(url_map["password"], "");
	EXPECT_EQ(url_map["host"], "host.com");
}

TEST_F(UrlParserTest, HttpNoPassword){
	std::string url = "http://user@host.com/";
	std::map<std::string, std::string> url_map = url_parser->parse(url);
	EXPECT_EQ(url_map["user"], "user");
	EXPECT_EQ(url_map["password"], "");
	EXPECT_EQ(url_map["host"], "host.com");
}

TEST_F(UrlParserTest, HttpSpecifiedPath){
	std::string url = "http://user@host.com/dir/dir2/file.";
	std::map<std::string, std::string> url_map = url_parser->parse(url);
	EXPECT_EQ(url_map["user"], "user");
	EXPECT_EQ(url_map["password"], "");
	EXPECT_EQ(url_map["host"], "host.com");
}

TEST_F(UrlParserTest, FullHttp){
	std::string url = "http://user:pass@host.com:240/dir/dir2/file.asd?some_querry#asd";
	std::map<std::string, std::string> url_map = url_parser->parse(url);
	EXPECT_EQ(url_map["scheme"], "http");
	EXPECT_EQ(url_map["user"], "user");
	EXPECT_EQ(url_map["password"], "pass");
	EXPECT_EQ(url_map["host"], "host.com");
	EXPECT_EQ(url_map["port"], "240");
	EXPECT_EQ(url_map["path"], "dir/dir2/file.asd?some_querry#asd");
}

TEST_F(UrlParserTest, FullTelnet){
	std::string url = "telnet://user:pass@host.com:240";
	std::map<std::string, std::string> url_map = url_parser->parse(url);
	EXPECT_EQ(url_map["scheme"], "telnet");
	EXPECT_EQ(url_map["user"], "user");
	EXPECT_EQ(url_map["password"], "pass");
	EXPECT_EQ(url_map["host"], "host.com");
	EXPECT_EQ(url_map["port"], "240");
}

TEST_F(UrlParserTest, FullGeneric){
	std::string url = "somegenericscheme://user:pass@127.0.0.1:240/dir/dir2/file.asd?some_querry#asd";
	std::map<std::string, std::string> url_map = url_parser->parse(url);
	EXPECT_EQ(url_map["scheme"], "somegenericscheme");
	EXPECT_EQ(url_map["user"], "user");
	EXPECT_EQ(url_map["password"], "pass");
	EXPECT_EQ(url_map["host"], "127.0.0.1");
	EXPECT_EQ(url_map["port"], "240");
	EXPECT_EQ(url_map["path"], "dir/dir2/file.asd?some_querry#asd");
}

// News Scheme tests

TEST_F(UrlParserTest, NewsGroup){
	std::string url = "news:somegroup";
	std::map<std::string, std::string> url_map = url_parser->parse(url);
	EXPECT_EQ(url_map["scheme"], "news");
	EXPECT_EQ(url_map["newsgroup-name"], "somegroup");
}

TEST_F(UrlParserTest, NewsDomain){
	std::string url = "news:article1@host.domain";
	std::map<std::string, std::string> url_map = url_parser->parse(url);
	EXPECT_EQ(url_map["scheme"], "news");
	EXPECT_EQ(url_map["message-id"], "article1");
	EXPECT_EQ(url_map["domain"], "host.domain");
}

// Mailto scheme tests

TEST_F(UrlParserTest, Mailto){
	std::string url = "mailto:email.addres@site.com";
	std::map<std::string, std::string> url_map = url_parser->parse(url);
	EXPECT_EQ(url_map["scheme"], "mailto");
	EXPECT_EQ(url_map["addres"], "email.addres@site.com");
}

int main(int argc, char *argv[]){
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}