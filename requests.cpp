#include <unistd.h>     /* read, write, close */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "requests.hpp"

using namespace std;

string compute_get_request(std::string host, std::string url, std::string query_params,
							std::vector<std::string> cookies, int cookies_count)
{
    string message;
    string line;
    string cookiesString;

    // Step 1: write the method name, URL, request params (if any) and protocol type
    if (!query_params.empty()) {
        line = "Get " + url + "?" + query_params + " HTTP/1.1";
    } else {
        line = "Get " + url + " HTTP/1.1";
    }

    message += line + "\r\n";

    // Step 2: add the host
    line = "Host: " + host;
    message += line + "\r\n";
    // Step 3 (optional): add headers and/or cookies, according to the protocol format
    cookiesString = "Cookie:";
    if (!cookies.empty()) {
        cookiesString += " ";
        cookiesString += cookies.front();
        cookies.pop_back();
        cookiesString += ";";
    }
    message += cookiesString + "\r\n";
    // Step 4: add final new line
    message += "\r\n";
    return message;
}

string compute_post_request(std::string host, std::string url, std::string content_type, std::string body_data,
							int body_data_fields_count, std::vector<std::string> cookies, int cookies_count)
{
    string message;
    string line;
    string cookiesString;

    // Step 1: write the method name, URL and protocol type
    line = "POST " + url + " HTTP/1.1";
    message += line + "\r\n";
    
    // Step 2: add the host
    line = "Host: " + host;
    message += line + "\r\n";
    /* Step 3: add necessary headers (Content-Type and Content-Length are mandatory)
            in order to write Content-Length you must first compute the message size
    */
    line = "Content-Type: " + content_type;
    message += line + "\r\n";
    line = "Content-Length: " + to_string(body_data_fields_count);
    message += line + "\r\n";
    // Step 4 (optional): add cookies
    cookiesString = "Cookie:";
    if (!cookies.empty()) {
        cookiesString += " ";
        cookiesString += cookies.front();
        cookies.pop_back();
        cookiesString += ";";
    }
    message += cookiesString + "\r\n";
    // Step 5: add new line at end of header
    message += "\r\n";
    // Step 6: add the actual payload data
    message += body_data + "\r\n";
    return message;
}

std::string compute_delete_request(std::string host, std::string url, std::string content_type, std::string body_data,
							int body_data_fields_count, std::vector<std::string> cookies, int cookies_count)
{
    string message;
    string line;
    string cookiesString;

    // Step 1: write the method name, URL and protocol type
    line = "DELETE " + url + " HTTP/1.1";
    message += line + "\r\n";
    
    // Step 2: add the host
    line = "Host: " + host;
    message += line + "\r\n";
    /* Step 3: add necessary headers (Content-Type and Content-Length are mandatory)
            in order to write Content-Length you must first compute the message size
    */
    line = "Content-Type: " + content_type;
    message += line + "\r\n";
    line = "Content-Length: " + to_string(body_data_fields_count);
    message += line + "\r\n";
    cookiesString = "Cookie:";
    if (!cookies.empty()) {
        cookiesString += " ";
        cookiesString += cookies.front();
        cookies.pop_back();
        cookiesString += ";";
    }
    message += cookiesString + "\r\n";
    // Step 5: add new line at end of header
    message += "\r\n";
    // Step 6: add the actual payload data
    message += body_data + "\r\n";
    return message;
}