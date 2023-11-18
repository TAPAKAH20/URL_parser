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

TEST_F(UrlParserTest, EmptyUrl){
	std::string url = "";
	EXPECT_EQ(url_parser->parse(url).empty(), true);
}

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


int main(int argc, char *argv[]){
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}