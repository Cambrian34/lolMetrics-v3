#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

// Structure to hold the response data
struct ResponseData {
    char *data;
    size_t size;
};

// Callback function to write the response data to a buffer
size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t total_size = size * nmemb;
    struct ResponseData *response = (struct ResponseData *)userp;

    // Allocate memory for the data or expand the existing buffer
    response->data = realloc(response->data, response->size + total_size + 1);
    if (response->data == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 0;
    }

    // Copy the response data to the buffer
    memcpy(&(response->data[response->size]), contents, total_size);
    response->size += total_size;
    response->data[response->size] = '\0'; // Null-terminate the string

    return total_size;
}

//main function

int main(void)
{
    CURL *curl;
    CURLcode res;
    // prompt to ask for which api to use
    // 1. account info
    // 2. champion rotation
    // 3. summoner info
    // 4. match history
    // 5. match info
    // 6. league info
    // 7. league position
    // 8. spectator info
    // 9. champion mastery
    // 10. champion mastery score

    printf("Which API would you like to use?\n");
    printf("1. account info\n");
    printf("2. champion rotation\n");
    printf("3. summoner info\n");
    printf("4. match history\n");
    printf("5. match info\n");
    printf("6. league info\n");
    printf("7. league position\n");
    printf("8. spectator info\n");
    printf("9. champion mastery\n");
    printf("10. champion mastery score\n");
    int api;
    scanf("%d", &api);
    printf("You chose %d\n", api);

    // api key
    const char *api_key = ""; // Replace with your API key

    char username[256] = "";// Replace with your username

    // URL to send the GET request
    char url[256]; // Adjust the buffer size as needed
   

    // create a switch statement to choose which api to use
    switch (api)
    {
    case 1:
        // http for account info
        snprintf(url, sizeof(url), "https://na1.api.riotgames.com/riot/account/v1/accounts/by-riot-id/%s?api_key=%s", username,api_key);
        break;
    case 2:
        // https for champion rotation
        snprintf(url, sizeof(url), "https://na1.api.riotgames.com/lol/platform/v3/champion-rotations?api_key=%s", api_key);
        break;
    case 3:
        // https for summoner info
        snprintf(url, sizeof(url), "https://na1.api.riotgames.com/lol/summoner/v4/summoners/by-name/%s?api_key=%s", username,api_key);
        break;
    case 4:
        // https for match history
        break;
    case 5:
        // https for match info
        break;
    case 6:
        // https for league info
        break;
    case 7:
        // https for league position
        break;
    case 8:
        // https for spectator info
        break;
    case 9:
        // https for champion mastery
        break;
    case 10:
        // https for champion mastery score
        break;
    default:
        break;
    }

    // Initialize libcurl
    curl = curl_easy_init();

    if (curl) {
        // Structure to hold the response data
        struct ResponseData response_data = {NULL, 0};

        // Set the URL to fetch
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // Set the callback function to write the response to the buffer
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

        // Perform the GET request
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            // Print or use the response data
            printf("Response Data:\n%s\n", response_data.data);
        }

        // Clean up the response buffer and close the file
        free(response_data.data);

        // Cleanup libcurl
        curl_easy_cleanup(curl);
    }
    // option to reset or exit
    printf("Would you like to use another API?\n");
    printf("1. Yes\n");
    printf("2. No\n");
    int reset;
    scanf("%d", &reset);
    if (reset == 1)
    {
        main();
    }
    else
    {
        return 0;
    }
    

}
