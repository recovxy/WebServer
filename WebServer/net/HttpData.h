#pragma once

#include <string>
#include "../base/copyable.h"
using std::string;

enum HttpMethod { METHOD_POST = 1, METHOD_GET, METHOD_HEAD };
enum HttpVersion { HTTP_10 = 1, HTTP_11 };

class HttpData : public copyable
{
public:
    HttpData(string& msg);
    ~HttpData();

private:
    HttpMethod method_;
    string path_;
    string filename_;
    HttpVersion version_;
    bool keepAlive_;

    void HttpParser(string& msg);
};
