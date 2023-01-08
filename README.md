# HTTP Library - Communication Protocols - Project 3

A pdf with the detailed description can be found in the repo.

#

The commands are read consecutively from the console. Additional input required for commands will be requested sequentially.

To display responses, the code and status returned by the server are extracted from the response.

The json, session id, and JWT token are extracted by searching for the corresponding character sequence. Additional characters from errors are found and deleted using regex.

The transport-level functions were taken from the laboratory skeleton and modified for c++.

For parsing json, I used the nlohmann library. I chose to use it because it is the one recommended in the requirements file.

## Commands

**register**

+username+password

The username and password are read from the keyboard. A json with these fields is composed and sent to the server through a POST request. The error is displayed if it exists and success if it does not.

**login**

+username+password

The username and password are read from the keyboard. It is checked if the user is not already logged in. A json with the read data is composed and a POST request is sent to the server. The error is displayed if it exists and success if it does not.

The cookie containing the session id is extracted.

**enter_library**

A GET request is sent which has the session id as a cookie. The error is displayed if it exists and success if it does not.

The JWT token confirming access is extracted.

**get_books**

A GET request is sent to the server which includes the access token. In case of an error it will be displayed, otherwise an array of existing books in json format will be displayed.

Connecting to the library is necessary.

**get_book**

+id

A GET request is sent to the server for the id book which includes the access token. In case of an error it will be displayed, otherwise the book with the given id is displayed in more detail.

Connecting to the library is necessary. It is checked if id is an integer before executing the request.

**add_book**

+title+author+genre+page_count

The necessary data is read from the keyboard and a json is composed. A POST request is sent which includes the access token. The error is displayed if it exists and success if it does not.

Connecting to the library is necessary. It is checked if page_count is an integer before executing the request.

**delete_book**

+id

A DELETE request is sent to the server for the id book which includes the access token. In case of an error it will be displayed, otherwise the book with the given id is displayed in more detail.

Connecting to the library is necessary. It is checked if id is an integer before executing the request.

**logout**

A GET request is sent to the server with the session id cookie. The session id and JWT token are reset. The error is displayed if it exists and success if it does not.
