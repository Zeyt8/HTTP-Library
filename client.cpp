#include <iostream>
#include <string>
#include "nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

void register();
void login();
void enter_library();
void get_books();
void get_book();
void add_book();
void delete_book();
void logout();
void exit();

int main(int argc, char* argv[])
{
    string command;
    while(1)
    {
        cin >> command;
        if(command == "register")
        {
            register();
        }
        else if(command == "login")
        {
            login();
        }
        else if(command == "enter_library")
        {
            enter_library();
        }
        else if(command == "get_books")
        {
            get_books();
        }
        else if(command == "get_book")
        {
            get_book();
        }
        else if(command == "add_book")
        {
            add_book();
        }
        else if(command == "delete_book")
        {
            delete_book();
        }
        else if(command == "logout")
        {
            logout();
        }
        else if(command == "exit")
        {
            exit();
        }
        else
        {
            cerr << "Invalid command!";
        }
    }
}

void register()
{
    string username;
    string password;
    cout << "username=";
    cin >> username;
    cout << "password=";
    cin >> password;
    json data;
    data["username"] = username;
    data["password"] = password;
}

void login()
{
    string username;
    string password;
    cout << "username=";
    cin >> username;
    cout << "password=";
    cin >> password;
    data["username"] = username;
    data["password"] = password;
}

void enter_library()
{

}

void get_books()
{

}

void get_book()
{
    int id;
    cout << "id=";
    cin >> id;
}

void add_book()
{
    string title;
    string author;
    string genre;
    string publisher;
    int page_count;
    cout << "title=";
    cin >> title;
    cout << "author=";
    cin >> author;
    cout << "genre=";
    cin >> genre;
    cout << "publisher=";
    cin >> publisher;
    cout << "page_count=";
    cin >> page_count;
    json data;
    data["title"] = title;
    data["author"] = author;
    data["genre"] = genre;
    data["page_count"] = page_count;
    data["publisher"] = publisher;
}

void delete_book()
{
    int id;
    cout << "id=";
    cin >> id;
}

void logout()
{

}

void exit()
{

}