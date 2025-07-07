#ifndef RIOT_API_H
#define RIOT_API_H

#include <stddef.h>

// Structure to hold HTTP response data
typedef struct {
    char *data;
    size_t size;
} ResponseData;

// Initializes a ResponseData structure
void init_response_data(ResponseData *response);

// Callback function for handling HTTP responses
size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp);

// Generic function to make an HTTP GET request
char *http_get(const char *url);

// Extracts a value from JSON response
char *extract_json_value(const char *json_str, const char *key);

// Fetches the PUUID of a user by gameName and tagLine
char *get_puuid(const char *gameName, const char *tagLine, const char *api_key);

// Fetches account information using PUUID
char *get_account_by_puuid(const char *puuid, const char *api_key);

// Fetches match history using PUUID
char *get_match_history(const char *puuid, const char *api_key);

// Fetches match details using match ID
char *get_match_details(const char *match_id, const char *api_key);

#endif // RIOT_API_H