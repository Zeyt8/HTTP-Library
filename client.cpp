#include <iostream>
#include <string>
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include <unistd.h> /* read, write, close */
#include <regex>
#include "nlohmann/json.hpp"
#include "helpers.hpp"
#include "requests.hpp"

using namespace std;
using json = nlohmann::json;

int sockfd;
string sessionId;
string tokenJWT;
bool loggedIn = false;
bool inLibrary = false;
int responseCode = 0;

void registerUser();
void login();
void enter_library();
void get_books();
void get_book();
void add_book();
void delete_book();
void logout();
void exit();

int main(int argc, char *argv[])
{
    string command;
    char *connectionIP = (char *)"34.241.4.235";

    while (1)
    {
        cin >> command;
        sockfd = open_connection(connectionIP, 8080, AF_INET, SOCK_STREAM, 0);
        if (command == "register")
        {
            registerUser();
        }
        else if (command == "login")
        {
            login();
        }
        else if (command == "enter_library")
        {
            enter_library();
        }
        else if (command == "get_books")
        {
            get_books();
        }
        else if (command == "get_book")
        {
            get_book();
        }
        else if (command == "add_book")
        {
            add_book();
        }
        else if (command == "delete_book")
        {
            delete_book();
        }
        else if (command == "logout")
        {
            logout();
        }
        else if (command == "exit")
        {
            exit();
        }
        else
        {
            cerr << "Invalid command!\n";
        }
    }
}

void registerUser()
{
    string username;
    string password;
    cout << "username=";
    getline(cin, username);
    cout << "password=";
    getline(cin, password);
    json data;
    data["username"] = username;
    data["password"] = password;
    string message;
    string response;
    message = compute_post_request("34.241.4.235", "/api/v1/tema/auth/register", "", "application/json", data.dump(), data.dump().length(), vector<string>(), 0);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    if (response.empty())
    {
        cout << "Error: no message received.\n";
        return;
    }
    response = basic_extract_json_response(response);
    if (!response.empty())
    {
        json responseJSON = json::parse(response);
        if (responseJSON.contains("error"))
        {
            cout << regex_replace(responseJSON["error"].dump(), regex("\""), "") << "\n";
        }
    }
    else
    {
        cout << "200 - OK - User succesfully registered!\n";
    }
}

void login()
{
    cin.ignore();
    if(loggedIn){
        cout << "You are already logged in!\n";
        return;
    }
    string username;
    string password;
    cout << "username=";
    getline(cin, username);
    cout << "password=";
    getline(cin, password);
    json data;
    data["username"] = username;
    data["password"] = password;
    string message;
    string response;
    message = compute_post_request("34.241.4.235", "/api/v1/tema/auth/login", "", "application/json", data.dump(), data.dump().length(), vector<string>(), 0);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    //cout << response << "\n";
    if (response.empty())
    {
        cout << "Error: no message received.\n";
        return;
    }
    if (!basic_extract_json_response(response).empty())
    {
        response = basic_extract_json_response(response);
        json responseJSON = json::parse(response);
        if (responseJSON.contains("error"))
        {
            cout << regex_replace(responseJSON["error"].dump(), regex("\""), "") << "\n";
        }
    }
    else
    {
        response = response.substr(response.find("connect.sid="));
        sessionId = response.substr(0, response.find(";"));
        cout << "200 - OK - Logged in.\n";
        loggedIn = true;
    }
}

void enter_library()
{
    if(inLibrary){
        cout << "Already in library!\n";
        return;
    }
    string message;
    string response;
    vector<string> cookies;
    cookies.push_back(sessionId);
    message = compute_get_request("34.241.4.235", "/api/v1/tema/library/access", "", "", cookies, cookies.size());
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    if(response.empty()){
        cout << "Error: no message received.\n";
        return;
    }
    if (!basic_extract_json_response(response).empty())
    {
        response = basic_extract_json_response(response);
        json responseJSON = json::parse(response);
        if (responseJSON.contains("error"))
        {
            cout << regex_replace(responseJSON["error"].dump(), regex("\""), "") << "\n";
        }
        else if (responseJSON.contains("token"))
        {
            tokenJWT = regex_replace(responseJSON["token"].dump(), regex("\""), "");
            cout << "200 - OK - Entered library.\n";
            inLibrary = true;
        }
    }
    else
    {
        cout << "Error\n";
    }
}

void get_books()
{
    string message;
    string response;
    message = compute_get_request("34.241.4.235", "/api/v1/tema/library/books", "", tokenJWT, vector<string>(), 0);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    if (response.empty())
    {
        cout << "Error: no message received.\n";
        return;
    }
    response = basic_extract_json_response(response);
    json responseJSON = json::parse(response);
    if (responseJSON.contains("error"))
    {
        cout << regex_replace(responseJSON["error"].dump(), regex("\""), "") << "\n";
    }
    else
    {
        cout << response << "\n";
    }
}

void get_book()
{
    int id;
    cout << "id=";
    cin >> id;
    if (cin.fail())
    {
        cout << "You must enter an integer!\n";
        cin.clear();
        cin.ignore();
        return;
    }
    string message;
    string response;
    message = compute_get_request("34.241.4.235", "/api/v1/tema/library/books/" + to_string(id), "", tokenJWT, vector<string>(), 0);
    cout << message << "\n";
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    if (response.empty())
    {
        cout << "Error: no message received.\n";
        return;
    }
    cout << response << "\n";
    if (!basic_extract_json_response(response).empty())
    {
        response = basic_extract_json_response(response);
        json responseJSON = json::parse(response);
        if (responseJSON.contains("error"))
        {
            cout << regex_replace(responseJSON["error"].dump(), regex("\""), "") << "\n";
        }
        else
        {
            cout << response << "\n";
        }
    }
    else{
        cout << "Error.\n";
    }
}

void add_book()
{
    cin.ignore();
    string title;
    string author;
    string genre;
    string publisher;
    int page_count;
    cout << "title=";
    getline(cin, title);
    cout << "author=";
    getline(cin, author);
    cout << "genre=";
    getline(cin, genre);
    cout << "page_count=";
    cin >> page_count;
    if(cin.fail()){
        cout << "You must enter an integer!\n";
        cin.clear();
        cin.ignore();
        return;
    }
    cout << "publisher=";
    cin.ignore();
    getline(cin, publisher);
    json data;
    data["title"] = title;
    data["author"] = author;
    data["genre"] = genre;
    data["page_count"] = page_count;
    data["publisher"] = publisher;
    string message;
    string response;
    message = compute_post_request("34.241.4.235", "/api/v1/tema/library/books", tokenJWT, "application/json", data.dump(), data.dump().length(), vector<string>(), 0);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    if (response.empty())
    {
        cout << "Error: no message received.\n";
        return;
    }
    cout << response << "\n";
    if (!basic_extract_json_response(response).empty())
    {
        response = basic_extract_json_response(response);
        json responseJSON = json::parse(response);
        if (responseJSON.contains("error"))
        {
            cout << regex_replace(responseJSON["error"].dump(), regex("\""), "") << "\n";
        }
    }
    else
    {
        cout << "OK - 200 - Book succesfully added.\n";
    }
}

void delete_book()
{
    int id;
    cout << "id=";
    cin >> id;
    if(cin.fail()){
        cout << "You must enter an integer!\n";
        cin.clear();
        cin.ignore();
        return;
    }
    string message;
    string response;
    message = compute_delete_request("34.241.4.235", "/api/v1/tema/library/books/" + to_string(id), tokenJWT, "", "", 0, vector<string>(), 0);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    if (response.empty())
    {
        cout << "Error: no message received.";
        return;
    }
    if (!basic_extract_json_response(response).empty())
    {
        response = basic_extract_json_response(response);
        json responseJSON = json::parse(response);
        if (responseJSON.contains("error"))
        {
            cout << regex_replace(responseJSON["error"].dump(), regex("\""), "") << "\n";
        }
    }
    else
    {
        cout << "OK - 200 - Book deleted.\n";
    }
}

void logout()
{
    if(!loggedIn){
        cout << "You are not logged in!\n";
        return;
    }
    string message;
    string response;
    vector<string> cookies;
    cookies.push_back(sessionId);
    message = compute_get_request("34.241.4.235", "/api/v1/tema/auth/logout", "", "", cookies, cookies.size());
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    if (response.empty())
    {
        cout << "Error: no message received.\n";
        return;
    }
    if (!basic_extract_json_response(response).empty())
    {
        response = basic_extract_json_response(response);
        json responseJSON = json::parse(response);
        if (responseJSON.contains("error"))
        {
            cout << regex_replace(responseJSON["error"].dump(), regex("\""), "") << "\n";
        }
    }
    else
    {
        cout << "OK - 200 - Logged out.\n";
        sessionId = "";
        tokenJWT = "";
        loggedIn = false;
        inLibrary = false;
    }
}

void exit()
{
    exit(0);
}