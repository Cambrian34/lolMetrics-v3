#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h" // cJSON library header
//#include "cJSON.c" // cJSON library implementation


// Define a structure to represent champion data
struct Champion {

    char key[50];
    char id[50];
    char name[50];
    char title[100];
    };

int main() {
    // Open the JSON file for reading
    FILE* json_file = fopen("champion.json", "r");
    if (json_file == NULL) {
        perror("Error opening JSON file");
        return 1;
    }

    // Read the JSON data from the file
    fseek(json_file, 0, SEEK_END);
    long json_file_size = ftell(json_file);
    fseek(json_file, 0, SEEK_SET);

    char* json_data = (char*)malloc(json_file_size + 1);
    if (json_data == NULL) {
        perror("Memory allocation failed");
        fclose(json_file);
        return 1;
    }

    size_t json_data_len = fread(json_data, 1, json_file_size, json_file);
    json_data[json_data_len] = '\0';

    // Close the JSON file
    fclose(json_file);

    // Initialize cJSON
    cJSON* root = cJSON_Parse(json_data);
    if (root == NULL) {
        fprintf(stderr, "Error parsing JSON\n");
        free(json_data);
        return 1;
    }

     cJSON* data = cJSON_GetObjectItem(root, "data");
    if (data == NULL) {
        fprintf(stderr, "No \"data\" object found in JSON.\n");
        cJSON_Delete(root);
        free(json_data);
        return 1;
    }
    // prompt user for champion id
    char searchId[50];
    printf("Enter the champion id: ");
    scanf("%s", searchId);


    // create array of champion structs
    struct Champion champions[170];
    int i = 0;
    cJSON* champion = NULL;

    // Loop through the JSON array and get the champion data
    cJSON_ArrayForEach(champion, data) {
        // Get the champion data
        cJSON* key = cJSON_GetObjectItem(champion, "key");
        cJSON* id = cJSON_GetObjectItem(champion, "id");
        cJSON* name = cJSON_GetObjectItem(champion, "name");
        cJSON* title = cJSON_GetObjectItem(champion, "title");

        // Copy the data to the array
        strcpy(champions[i].key, key->valuestring);
        strcpy(champions[i].id, id->valuestring);
        strcpy(champions[i].name, name->valuestring);
        strcpy(champions[i].title, title->valuestring);

        i++;
    }

    int found = 0;
    // Search for the champion
    for (int j = 0; j < i; j++) {
        if (strcmp(champions[j].key, searchId) == 0) {
            printf("Key: %s\n", champions[j].key);
            printf("Id: %s\n", champions[j].id);
            printf("Name: %s\n", champions[j].name);
            printf("Title: %s\n", champions[j].title);
            found = 1;
            break;
        }
    }

    

    //print all champions
    /*for (int j = 0; j < i; j++) {
        printf("Key: %s\n", champions[j].key);
        printf("Id: %s\n", champions[j].id);
        printf("Name: %s\n", champions[j].name);
        printf("Title: %s\n", champions[j].title);
    }*/
    

    // Free memory
    cJSON_Delete(root);
    free(json_data);

    return 0;
}

