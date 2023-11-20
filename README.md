# URL_parser

## Usage

```c++
UrlParser* url_parser;
url_parser = new UrlParser();
std::string url = "hhttp://user:pass@host.com:240/dir/dir2/file.asd?some_querry#asd";
std::map<std::string, std::string> url_map = url_parser->parse(url);

/*For Common Internet Schemes
url_map["scheme"] 					"http"
url_map["user"] 					"user"
url_map["password"] 				"pass"
url_map["host"] 					"host.com"
url_map["port"] 					"240"
url_map["path"] 					"dir/dir2/file.asd?some_querry#asd"


 Mailto
std::string url = "mailto:email.addres@site.com";
url_map["scheme"]				 	"mailto"
url_map["addres"]				 	"email.addres@site.com"


 News
std::string url = "news:somegroup";
url_map["scheme"]				 	"news"
url_map["newsgroup-name"]		 	"somegroup"

std::string url = "news:article1@host.domain";
url_map["scheme"]					"news"
url_map["message-id"]				"article1"
url_map["domain"]				 	"host.domain"
*/
delete url_parser;
```
On invalid urls `parse` function returns an empty map.

## Testing

```sh
cmake -S . -B build
cmake --build build
./build/testing
```

### Testing results
![image](https://github.com/TAPAKAH20/URL_parser/assets/24612435/31f6f9cd-ac29-4b73-b059-9ffc482493fc)

