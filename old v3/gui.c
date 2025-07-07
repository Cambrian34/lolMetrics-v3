#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <gtk/gtk.h>
#include "cJSON.h"

// api key is stored in api_key.txt
// which makes it vulnerable to being stolen
// need to find a way to store api key securely
// maybe use a database
// or store api key in a variable
// or store api key in a file that is not in the same directory as the executable
// or store api key in a file that is encrypted
// or store api key in a file that is hidden
// how to store api key securely?

// Define a structure to represent champion data
struct Champion
{

    char key[50];
    char id[50];
    char name[50];
    char title[100];
};

// intrudce global variables
char value1[100]; // api_key[100];
char value2[100]; // summoner_name[100];
char summoner_info[100];
char league_info[100];
char champion_mastery_info[100];
char match_history[100];
char match_info[100];

struct ResponseData
{
    char *data;
    size_t size;
};

size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t total_size = size * nmemb;
    struct ResponseData *response = (struct ResponseData *)userp;

    response->data = realloc(response->data, response->size + total_size + 1);
    if (response->data == NULL)
    {
        fprintf(stderr, "Memory allocation failed.\n");
        return 0;
    }

    memcpy(&(response->data[response->size]), contents, total_size);
    response->size += total_size;
    response->data[response->size] = '\0';

    return total_size;
}

char *getchampbykey(char searchId[50])
{
    // Open the JSON file for reading
    FILE *json_file = fopen("champion.json", "r");
    if (json_file == NULL)
    {
        perror("Error opening JSON file");
        return NULL;
    }

    // Read the JSON data from the file
    fseek(json_file, 0, SEEK_END);
    long json_file_size = ftell(json_file);
    fseek(json_file, 0, SEEK_SET);

    char *json_data = (char *)malloc(json_file_size + 1);
    if (json_data == NULL)
    {
        perror("Memory allocation failed");
        fclose(json_file);
        return NULL;
    }

    size_t json_data_len = fread(json_data, 1, json_file_size, json_file);
    json_data[json_data_len] = '\0';

    // Close the JSON file
    fclose(json_file);

    // Initialize cJSON
    cJSON *root = cJSON_Parse(json_data);
    if (root == NULL)
    {
        fprintf(stderr, "Error parsing JSON\n");
        free(json_data);
        return NULL;
    }

    cJSON *data = cJSON_GetObjectItem(root, "data");
    if (data == NULL)
    {
        fprintf(stderr, "No \"data\" object found in JSON.\n");
        cJSON_Delete(root);
        free(json_data);
        return NULL;
    }

    // Create an array of champion structs
    struct Champion champions[170];
    int i = 0;
    cJSON *champion = NULL;

    // Loop through the JSON array and get the champion data
    cJSON_ArrayForEach(champion, data)
    {
        // Get the champion data
        cJSON *key = cJSON_GetObjectItem(champion, "key");
        cJSON *id = cJSON_GetObjectItem(champion, "id");
        cJSON *name = cJSON_GetObjectItem(champion, "name");
        cJSON *title = cJSON_GetObjectItem(champion, "title");

        // Copy the data to the array
        strcpy(champions[i].key, key->valuestring);
        strcpy(champions[i].id, id->valuestring);
        strcpy(champions[i].name, name->valuestring);
        strcpy(champions[i].title, title->valuestring);

        i++;
    }

    // Search for the champion
    int found = 0;
    for (int j = 0; j < i; j++)
    {
        if (strcmp(champions[j].key, searchId) == 0)
        {
            // Create a string to hold the champion details
            char *champion_details = (char *)malloc(250); // Adjust size as needed
            if (champion_details == NULL)
            {
                perror("Memory allocation failed");
                cJSON_Delete(root);
                free(json_data);
                return NULL;
            }

            // Format the champion details as a string
            snprintf(champion_details, 250, "Key: %s\nId: %s\nName: %s\nTitle: %s\n",
                     champions[j].key, champions[j].id, champions[j].name, champions[j].title);

            found = 1;
            cJSON_Delete(root);
            free(json_data);
            return champion_details;
        }
    }

    cJSON_Delete(root);
    free(json_data);

    if (!found)
    {
        return strdup("Champion not found."); // Return a message indicating the champion was not found
    }

    return NULL;
}

// array of champion ids
int champion_id[30];

// function to get champion ids from json file
void getchap()
{
    FILE *file;
    char *jsonString = NULL;
    cJSON *root, *freeChampionIds;

    // Open the JSON file for reading
    file = fopen("summoner_info.json", "r");
    if (!file)
    {
        fprintf(stderr, "Error opening JSON file\n");
    }

    // Get the size of the file
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory for the JSON string
    jsonString = (char *)malloc(fileSize + 1);
    if (!jsonString)
    {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(file);
    }

    // Read the JSON file into the string
    fread(jsonString, 1, fileSize, file);
    jsonString[fileSize] = '\0';

    // Parse the JSON string
    root = cJSON_Parse(jsonString);
    if (!root)
    {
        fprintf(stderr, "JSON parsing error: %s\n", cJSON_GetErrorPtr());
        free(jsonString);
        fclose(file);
    }

    // Get the "freeChampionIds" array
    freeChampionIds = cJSON_GetObjectItem(root, "freeChampionIds");
    if (!freeChampionIds || !cJSON_IsArray(freeChampionIds))
    {
        fprintf(stderr, "JSON parsing error: Unable to find 'freeChampionIds' array\n");
        cJSON_Delete(root);
        free(jsonString);
        fclose(file);
    }

    // Get the number of elements in the array
    int arraySize = cJSON_GetArraySize(freeChampionIds);

    // Allocate memory for the C array
    int *championIds = (int *)malloc(arraySize * sizeof(int));
    if (!championIds)
    {
        fprintf(stderr, "Memory allocation failed\n");
        cJSON_Delete(root);
        free(jsonString);
        fclose(file);
    }

    // Copy the elements from the JSON array to the C array
    for (int i = 0; i < arraySize; i++)
    {
        cJSON *item = cJSON_GetArrayItem(freeChampionIds, i);
        championIds[i] = item->valueint;
    }

    // Print the values in the C array
    /*printf("freeChampionIds array:\n");
    for (int i = 0; i < arraySize; i++)
    {
        printf("%d ", championIds[i]);
    }
    printf("\n");*/

    // add champion ids to global array
    // loop through array
    for (int i = 0; i < arraySize; i++)
    {
        // add champion id to global array
        champion_id[i] = championIds[i];
    }

    // Clean up
    cJSON_Delete(root);
    free(jsonString);
    free(championIds);
    fclose(file);
}

void create_file(const char *filename, const char *api_key)
{
    FILE *fptr = fopen(filename, "w");
    if (fptr == NULL)
    {
        fprintf(stderr, "Failed to open file '%s' for writing\n", filename);
        return;
    }

    fprintf(fptr, "%s", api_key);
    fclose(fptr);
}

// function to read file and store api key
void read_file(const char *filename, char *api_key, size_t max_len)
{
    FILE *fptr = fopen(filename, "r");
    if (fptr == NULL)
    {
        fprintf(stderr, "Failed to open file '%s' for reading\n", filename);
        return;
    }

    fgets(api_key, max_len, fptr);

    // add api key to value1
    // strncpy(value1, api_key, sizeof(value1) - 1);
    // value1[sizeof(value1) - 1] = '\0'; // Ensure null-termination

    fclose(fptr);
}
// function to get champion info
// get array of champion ids from get free champs

void write_data_to_file(const char *filename, const struct ResponseData *response)
{
    FILE *fptr = fopen(filename, "w");
    if (fptr == NULL)
    {
        fprintf(stderr, "Failed to open file '%s' for writing\n", filename);
        return;
    }

    fwrite(response->data, 1, response->size, fptr);
    fclose(fptr);
}

void get_api_key(GtkWidget *widget, gpointer data)
{
    const char *entered_value1 = gtk_entry_get_text(GTK_ENTRY(data));
    strncpy(value1, entered_value1, sizeof(value1) - 1);
    value1[sizeof(value1) - 1] = '\0'; // Ensure null-termination

    const char *entered_value2 = gtk_entry_get_text(GTK_ENTRY(data));
    strncpy(value2, entered_value2, sizeof(value2) - 1);
    value2[sizeof(value2) - 1] = '\0'; // Ensure null-termination

    // tested to see if the api key and summoner name are being stored
    // printf("Value 1: %s\n", value1);
    // printf("Value 2: %s\n", value2);

    create_file("api_key.txt", value1);
    create_file("summoner_name.txt", value2);
}

// function to update textbox with response data
void update_textbox(GtkWidget *textbox, const struct ResponseData *response)
{
    if (response->data != NULL)
    {
        // Get the text from the response data and set it in the GTK entry
        gtk_entry_set_text(GTK_ENTRY(textbox), response->data);
    }
    else
    {
        // Handle the case where response data is empty or NULL (optional)
        gtk_entry_set_text(GTK_ENTRY(textbox), "No data available");
    }
}

// function to update textbox with string
void update_textbox2(GtkWidget *textbox, const char *string)
{
    if (string != NULL)
    {
        // Get the text from the response data and set it in the GTK entry
        gtk_entry_set_text(GTK_ENTRY(textbox), string);
    }
    else
    {
        // Handle the case where response data is empty or NULL (optional)
        gtk_entry_set_text(GTK_ENTRY(textbox), "No data available");
    }
}
// function to read api key
void read_api_key(char *api_key, size_t max_len)
{
    read_file("api_key.txt", api_key, max_len);
}

// iterate through array of champion ids
// get champion info for each champion id
char *getchamp(int champ_id[300])
{
    // Allocate memory to hold champion info
    // this had to be increased to 10000
    // because the champion info is too long
    // and the program crashes if the memory is not large enough
    char *champ_info = (char *)malloc(10000);
    if (champ_info == NULL)
    {
        perror("Memory allocation failed");
        return NULL;
    }

    // Initialize champ_info with an empty string
    champ_info[0] = '\0';

    for (int i = 0; i < 30; i++)
    {
        // Print champion info
        // printf("Champion id: %d\n", champ_id[i]);
        while (champ_id[i] != 0)
        {
            // Get champion info for the current ID
            char current_champ_id[50];
            snprintf(current_champ_id, sizeof(current_champ_id), "%d", champ_id[i]); // Convert int to string
            char *current_champ_info = getchampbykey(current_champ_id);

            // Check if the champion info is not NULL
            if (current_champ_info != NULL)
            {
                // Concatenate the current champion info to champ_info
                strcat(champ_info, current_champ_info);

                // Free the memory allocated for current_champ_info
                // free(current_champ_info);
            }
        }
    }

    return champ_info;
}

// function to read summoner info
void read_summoner_info(char *summoner_info, size_t max_len)
{
    read_file("summoner_info.json", summoner_info, max_len);
}
// set action for button1
//  button1 will get summoner information
// g_signal_connect(button1, "clicked", G_CALLBACK(get_summoner_info), NULL);
// set action for button2
//  button2 will get league information
// g_signal_connect(button2, "clicked", G_CALLBACK(get_league_info), NULL);
// set action for button3
//  button3 will get champion mastery information
// g_signal_connect(button3, "clicked", G_CALLBACK(get_champion_mastery_info), NULL);
// set action for button4
//  button4 will get match history
// g_signal_connect(button4, "clicked", G_CALLBACK(get_match_history), NULL);
// set action for button5
//  button5 will get match information
// g_signal_connect(button5, "clicked", G_CALLBACK(get_match_info), NULL);

// function to get summoner info
//  Function to get summoner info

void get_summoner_info(GtkWidget *widget, gpointer data)
{
    // Read the API key from the file
    char api_key[100];
    read_api_key(api_key, sizeof(api_key));

    // Read the summoner name from the file
    char summoner_name[100];
    read_summoner_info(summoner_name, sizeof(summoner_name));

    // Create the URL
    char url[256]; // Increased buffer size for the URL
    // snprintf(url, sizeof(url), "https://na1.api.riotgames.com/lol/summoner/v4/summoners/by-name/%s?api_key=%s", summoner_name, api_key);
    snprintf(url, sizeof(url), "https://na1.api.riotgames.com/riot/account/v1/accounts/by-riot-id/%s?api_key=%s", summoner_name, api_key);
    // snprintf(url, sizeof(url), "https://na1.api.riotgames.com/riot/account/v1/accounts/by-riot-id/%s?api_key=%s", username,api_key);

    // Initialize the response data
    struct ResponseData response = {
        .data = NULL, // Initialize to NULL
        .size = 0};

    // Initialize the curl session
    CURL *curl = curl_easy_init();
    if (curl == NULL)
    {
        fprintf(stderr, "Failed to initialize curl session\n");
        return;
    }

    // Set the curl options
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    // Perform the curl request
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        return;
    }

    // Check if the response data is empty
    if (response.size == 0 || response.data == NULL)
    {
        fprintf(stderr, "Empty response data\n");
        curl_easy_cleanup(curl);
        return;
    }

    // Write the response data to a file (optional)
    write_data_to_file("summoner_info.json", &response);

    // Update the GTK GUI with the retrieved information
    update_textbox(data, &response);

    // Clean up the curl session
    curl_easy_cleanup(curl);

    // Free the response data memory
    free(response.data);
}

// get current free champion rotation
void get_free_champs(GtkWidget *widget, gpointer data)
{
    // Read the API key from the file
    char api_key[100];
    read_api_key(api_key, sizeof(api_key));

    // Read the summoner name from the file
    char summoner_name[100];
    read_summoner_info(summoner_name, sizeof(summoner_name));

    // Create the URL
    char url[256]; // Increased buffer size for the URL
    // snprintf(url, sizeof(url), "https://na1.api.riotgames.com/lol/summoner/v4/summoners/by-name/%s?api_key=%s", summoner_name, api_key);
    snprintf(url, sizeof(url), "https://na1.api.riotgames.com/lol/platform/v3/champion-rotations?api_key=%s", api_key);
    // snprintf(url, sizeof(url), "https://na1.api.riotgames.com/lol/platform/v3/champion-rotations?api_key=%s", api_key);

    // Initialize the response data
    struct ResponseData response = {
        .data = NULL, // Initialize to NULL
        .size = 0};

    // Initialize the curl session
    CURL *curl = curl_easy_init();
    if (curl == NULL)
    {
        fprintf(stderr, "Failed to initialize curl session\n");
        return;
    }

    // Set the curl options
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    // Perform the curl request
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        return;
    }

    // Check if the response data is empty
    if (response.size == 0 || response.data == NULL)
    {
        fprintf(stderr, "Empty response data\n");
        curl_easy_cleanup(curl);
        return;
    }

    // Write the response data to a file (optional)
    write_data_to_file("summoner_info.json", &response);

    // Update the GTK GUI with the retrieved information
    // update_textbox(data, &response);

    // Clean up the curl session
    curl_easy_cleanup(curl);

    // Free the response data memory
    free(response.data);

    // get champion info
    // get champion ids from json file
    getchap();
    // wait 10 seconds
    sleep(10);

    // print champion iinfo to text box
    update_textbox2(data, getchamp(champion_id));
}
void get_champion_mastery_info(GtkWidget *widget, gpointer data)
{
}
void get_match_history(GtkWidget *widget, gpointer data){

};
void get_match_info(GtkWidget *widget, gpointer data){

};

// uses response data from get free champs to get champion info
// list of all champions is in champion.csv
// access champion info using champion id
// champion id is in response data from get free champs
// champion info is in champion.csv
// uses respomse data in summoner_info.json to get champion id
// champion id is in response data from get summoner info

// function to print champion info to console
void print_champion_info(int test[], int size)
{
    // Loop through the array
    for (int i = 0; i < size; i++)
    {
        // Print champion info
        printf("Champion id: %d\n", test[i]);
    }
}

// main gui function

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    int width = 800;
    int height = 600;

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "lolMetrics");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_window_set_default_size(GTK_WINDOW(window), width, height);

    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    GtkWidget *label1 = gtk_label_new("Enter your Riot API key: ");
    gtk_grid_attach(GTK_GRID(grid), label1, 0, 0, 1, 1);

    GtkWidget *textbox1 = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), textbox1, 1, 0, 1, 1);

    GtkWidget *label2 = gtk_label_new("Enter the summoner name: ");
    gtk_grid_attach(GTK_GRID(grid), label2, 0, 1, 1, 1);

    GtkWidget *textbox2 = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), textbox2, 1, 1, 1, 1);

    GtkWidget *button = gtk_button_new_with_label("Submit");
    gtk_grid_attach(GTK_GRID(grid), button, 0, 2, 2, 1);
    g_signal_connect(button, "clicked", G_CALLBACK(get_api_key), textbox1);
    // g_signal_connect(button, "clicked", G_CALLBACK(get_api_key), textbox2);

    GtkWidget *label = gtk_label_new("What would you like to do?\n 1. Get summoner information\n 2. Get league information\n 3. Get champion mastery information\n 4. Get match history\n 5. Get match information\n");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 3, 2, 1);

    GtkWidget *button1 = gtk_button_new_with_label("1");
    gtk_grid_attach(GTK_GRID(grid), button1, 0, 4, 1, 1);
    GtkWidget *button2 = gtk_button_new_with_label("2");
    gtk_grid_attach(GTK_GRID(grid), button2, 1, 4, 1, 1);
    GtkWidget *button3 = gtk_button_new_with_label("3");
    gtk_grid_attach(GTK_GRID(grid), button3, 0, 5, 1, 1);
    GtkWidget *button4 = gtk_button_new_with_label("4");
    gtk_grid_attach(GTK_GRID(grid), button4, 1, 5, 1, 1);
    GtkWidget *button5 = gtk_button_new_with_label("5");
    gtk_grid_attach(GTK_GRID(grid), button5, 0, 6, 1, 1);

    GtkWidget *label3 = gtk_label_new("Response data: ");
    gtk_grid_attach(GTK_GRID(grid), label3, 0, 7, 1, 1);

    GtkWidget *textbox3 = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), textbox3, 0, 8, 2, 1);
    // increase size of textbox
    gtk_entry_set_max_length(GTK_ENTRY(textbox3), 10000);
    // increase width of textbox and height of textbox
    gtk_widget_set_size_request(textbox3, 800, 600);

    g_signal_connect(button1, "clicked", G_CALLBACK(get_summoner_info), textbox3);
    // Connect other buttons to their respective functions.
    g_signal_connect(button2, "clicked", G_CALLBACK(get_free_champs), textbox3);

    // make windows translucent
    gtk_widget_set_opacity(window, 0.9);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}