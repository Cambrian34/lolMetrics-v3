#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "cJSON.h"
#include "riot_api.h"

// Initializes response data structure
void init_response_data(ResponseData *response) {
    response->data = NULL;
    response->size = 0;
}

// Callback function for CURL to handle responses
size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t total_size = size * nmemb;
    ResponseData *response = (ResponseData *)userp;

    // Allocate or expand buffer
    response->data = realloc(response->data, response->size + total_size + 1);
    if (!response->data) {
        fprintf(stderr, "Memory allocation failed\n");
        return 0;
    }

    // Copy response data
    memcpy(&(response->data[response->size]), contents, total_size);
    response->size += total_size;
    response->data[response->size] = '\0'; // Null-terminate

    return total_size;
}

// Makes an HTTP GET request and returns the response
char *http_get(const char *url) {
    CURL *curl;
    CURLcode res;
    ResponseData response;
    init_response_data(&response);

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Curl initialization failed\n");
        return NULL;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "CURL request failed: %s\n", curl_easy_strerror(res));
        free(response.data);
        response.data = NULL;
    }

    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return response.data; // Caller must free the response
}

// Extracts a value from JSON response using cJSON
char *extract_json_value(const char *json_str, const char *key) {
    cJSON *json = cJSON_Parse(json_str);
    if (!json) {
        fprintf(stderr, "JSON parsing failed\n");
        return NULL;
    }

    cJSON *value_json = cJSON_GetObjectItemCaseSensitive(json, key);
    char *value = NULL;

    if (cJSON_IsString(value_json) && (value_json->valuestring != NULL)) {
        value = strdup(value_json->valuestring);
    }

    cJSON_Delete(json);
    return value; // Caller must free this
}

// Fetches the PUUID using gameName and tagLine
char *get_puuid(const char *gameName, const char *tagLine, const char *api_key) {
    char url[256];
    snprintf(url, sizeof(url),
             "https://americas.api.riotgames.com/riot/account/v1/accounts/by-riot-id/%s/%s?api_key=%s",
             gameName, tagLine, api_key);

    char *response = http_get(url);
    if (!response) {
        return NULL;
    }

    char *puuid = extract_json_value(response, "puuid");
    free(response); // Free response after extracting required data

    return puuid; // Caller must free this
}