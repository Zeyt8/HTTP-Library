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
string responseCode;

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
        //get user command
        getline(cin, command);
        //connect to the server
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
            cout << "Invalid command!\n";
        }
    }
}

void registerUser()
{
    if(loggedIn){
        cout << "You must log out first!\n";
        return;
    }
    string username;
    string password;
    cout << "username=";
    getline(cin, username);
    cout << "password=";
    getline(cin, password);
    //create json
    json data;
    data["username"] = username;
    data["password"] = password;
    string message;
    string response;
    //create message
    message = compute_post_request("34.241.4.235", "/api/v1/tema/auth/register", "", "application/json", data.dump(), data.dump().length(), vector<string>(), 0);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    if (response.empty())
    {
        cout << "Error: no message received.\n";
        return;
    }
    //get response code
    responseCode = response.substr(response.find(" ") + 1);
    responseCode = responseCode.substr(0, responseCode.find("\n") - 1);
    //extract payload
    response = basic_extract_json_response(response);
    if (!response.empty())
    {
        json responseJSON = json::parse(response);
        if (responseJSON.contains("error"))
        {
            cout << responseCode << " - " << regex_replace(responseJSON["error"].dump(), regex("\""), "") << "\n";
        }
    }
    else
    {
        cout << responseCode << " - User succesfully registered!\n";
    }
}

void login()
{
    //check if already logged in
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
    //create json
    json data;
    data["username"] = username;
    data["password"] = password;
    string message;
    string response;
    //create message
    message = compute_post_request("34.241.4.235", "/api/v1/tema/auth/login", "", "application/json", data.dump(), data.dump().length(), vector<string>(), 0);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    if (response.empty())
    {
        cout << "Error: no message received.\n";
        return;
    }
    //get response code
    responseCode = response.substr(response.find(" ") + 1);
    responseCode = responseCode.substr(0, responseCode.find("\n") - 1);
    if (!basic_extract_json_response(response).empty())
    {
        response = basic_extract_json_response(response);
        json responseJSON = json::parse(response);
        if (responseJSON.contains("error"))
        {
            cout << responseCode << " - " << regex_replace(responseJSON["error"].dump(), regex("\""), "") << "\n";
        }
    }
    else
    {
        response = response.substr(response.find("connect.sid="));
        sessionId = response.substr(0, response.find(";"));
        cout << responseCode << " - Logged in.\n";
        loggedIn = true;
    }
}

void enter_library()
{
    //check if already in library
    if(inLibrary){
        cout << "Already in library!\n";
        return;
    }
    string message;
    string response;
    //create cookies
    vector<string> cookies;
    cookies.push_back(sessionId);
    message = compute_get_request("34.241.4.235", "/api/v1/tema/library/access", "", "", cookies, cookies.size());
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    if(response.empty()){
        cout << "Error: no message received.\n";
        return;
    }
    //get response code
    responseCode = response.substr(response.find(" ") + 1);
    responseCode = responseCode.substr(0, responseCode.find("\n") - 1);
    if (!basic_extract_json_response(response).empty())
    {
        response = basic_extract_json_response(response);
        json responseJSON = json::parse(response);
        if (responseJSON.contains("error"))
        {
            cout << responseCode << " - " << regex_replace(responseJSON["error"].dump(), regex("\""), "") << "\n";
        }
        else if (responseJSON.contains("token"))
        {
            //save token JWT
            tokenJWT = regex_replace(responseJSON["token"].dump(), regex("\""), "");
            cout  << responseCode << " - Entered library.\n";
            inLibrary = true;
        }
    }
    else
    {
        cout << "Undefined Error\n";
    }
}

void get_books()
{
    string message;
    string response;
    //create message
    message = compute_get_request("34.241.4.235", "/api/v1/tema/library/books", "", tokenJWT, vector<string>(), 0);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    if (response.empty())
    {
        cout << "Error: no message received.\n";
        return;
    }
    //get response code
    responseCode = response.substr(response.find(" ") + 1);
    responseCode = responseCode.substr(0, responseCode.find("\n") - 1);
    response = basic_extract_json_response(response);
    json responseJSON = json::parse(response);
    if (responseJSON.contains("error"))
    {
        cout << responseCode << " - " << regex_replace(responseJSON["error"].dump(), regex("\""), "") << "\n";
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
    cin.ignore();
    //check if is integer
    if (cin.fail())
    {
        cout << "You must enter an integer!\n";
        cin.clear();
        cin.ignore();
        return;
    }
    string message;
    string response;
    //create message
    message = compute_get_request("34.241.4.235", "/api/v1/tema/library/books/" + to_string(id), "", tokenJWT, vector<string>(), 0);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    if (response.empty())
    {
        cout << "Error: no message received.\n";
        return;
    }
    //get response code
    responseCode = response.substr(response.find(" ") + 1);
    responseCode = responseCode.substr(0, responseCode.find("\n") - 1);
    if (!basic_extract_json_response(response).empty())
    {
        response = basic_extract_json_response(response);
        json responseJSON = json::parse(response);
        if (responseJSON.contains("error"))
        {
            cout << responseCode << " - " << regex_replace(responseJSON["error"].dump(), regex("\""), "") << "\n";
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
    //check if integer
    if(cin.fail()){
        cout << "You must enter an integer!\n";
        cin.clear();
        cin.ignore();
        return;
    }
    cout << "publisher=";
    cin.ignore();
    getline(cin, publisher);
    //create json
    json data;
    data["title"] = title;
    data["author"] = author;
    data["genre"] = genre;
    data["page_count"] = page_count;
    data["publisher"] = publisher;
    string message;
    string response;
    //create message
    message = compute_post_request("34.241.4.235", "/api/v1/tema/library/books", tokenJWT, "application/json", data.dump(), data.dump().length(), vector<string>(), 0);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    if (response.empty())
    {
        cout << "Error: no message received.\n";
        return;
    }
    //get response code
    responseCode = response.substr(response.find(" ") + 1);
    responseCode = responseCode.substr(0, responseCode.find("\n") - 1);
    if (!basic_extract_json_response(response).empty())
    {
        response = basic_extract_json_response(response);
        json responseJSON = json::parse(response);
        if (responseJSON.contains("error"))
        {
            cout << responseCode << " - " << regex_replace(responseJSON["error"].dump(), regex("\""), "") << "\n";
        }
    }
    else
    {
        cout << responseCode << " - Book succesfully added.\n";
    }
}

void delete_book()
{
    int id;
    cout << "id=";
    cin >> id;
    cin.ignore();
    //check if integer
    if(cin.fail()){
        cout << "You must enter an integer!\n";
        cin.clear();
        cin.ignore();
        return;
    }
    string message;
    string response;
    //create message
    message = compute_delete_request("34.241.4.235", "/api/v1/tema/library/books/" + to_string(id), tokenJWT, "", "", 0, vector<string>(), 0);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    if (response.empty())
    {
        cout << "Error: no message received.";
        return;
    }
    //get response code
    responseCode = response.substr(response.find(" ") + 1);
    responseCode = responseCode.substr(0, responseCode.find("\n") - 1);
    if (!basic_extract_json_response(response).empty())
    {
        response = basic_extract_json_response(response);
        json responseJSON = json::parse(response);
        if (responseJSON.contains("error"))
        {
            cout << responseCode << " - " << regex_replace(responseJSON["error"].dump(), regex("\""), "") << "\n";
        }
    }
    else
    {
        cout << responseCode << " - Book deleted.\n";
    }
}

void logout()
{
    //check if logged in
    if(!loggedIn){
        cout << "You are not logged in!\n";
        return;
    }
    string message;
    string response;
    //create cookies
    vector<string> cookies;
    cookies.push_back(sessionId);
    //create message
    message = compute_get_request("34.241.4.235", "/api/v1/tema/auth/logout", "", "", cookies, cookies.size());
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    if (response.empty())
    {
        cout << "Error: no message received.\n";
        return;
    }
    //get response code
    responseCode = response.substr(response.find(" ") + 1);
    responseCode = responseCode.substr(0, responseCode.find("\n") - 1);
    if (!basic_extract_json_response(response).empty())
    {
        response = basic_extract_json_response(response);
        json responseJSON = json::parse(response);
        if (responseJSON.contains("error"))
        {
            cout << responseCode << " - " << regex_replace(responseJSON["error"].dump(), regex("\""), "") << "\n";
        }
    }
    else
    {
        cout << responseCode << " - Logged out.\n";
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