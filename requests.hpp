#include <string>
#include <vector>

#ifndef _REQUESTS_
#define _REQUESTS_

// computes and returns a GET request string (query_params
// and cookies can be set to NULL if not needed)
std::string compute_get_request(std::string host, std::string url, std::string query_params,
							std::vector<std::string> cookies, int cookies_count);

// computes and returns a POST request string (cookies can be NULL if not needed)
std::string compute_post_request(std::string host, std::string url, std::string content_type, std::vector<std::string> body_data,
							int body_data_fields_count, std::vector<std::string> cookies, int cookies_count);

std::string compute_delete_request(std::string host, std::string url, std::string content_type, std::vector<std::string> body_data,
							int body_data_fields_count, std::vector<std::string> cookies, int cookies_count);

#endif
