# lolMetrics-v3

# Riot API Command Line Tool

This command line application allows users to make requests to the Riot Games Developer API to retrieve JSON data associated with various endpoints. Users can enter their Riot API key, username, and select from a range of available API endpoints to query.

## Features

- **User Input:** Prompt the user to enter their Riot API key and username.

- **API Endpoint Selection:** Users can choose from the following Riot API endpoints:
  1. Account Info
  2. Champion Rotation
  3. Summoner Info
  4. Match History
  5. Match Info
  6. League Info
  7. League Position
  8. Spectator Info
  9. Champion Mastery
  10. Champion Mastery Score

- **HTTP Requests:** Construct and send HTTP GET requests to the selected Riot API endpoint using the provided API key and username.

- **Response Handling:** Receive and display the JSON response data from the Riot API.

- **Option for Multiple Queries:** After receiving the response, users have the option to make additional API queries or exit the application.

## How to Use

1. Compile the program.
2. Run the executable.
3. Enter your Riot API key when prompted.
4. Enter your username when prompted.
5. Select the desired Riot API endpoint.
6. View the response data.
7. Choose whether to make another API query or exit.

## Example Usage

```shell
$ ./riot-api-tool

Enter your API key: <Your Riot API Key>
Enter your username: <Your Username>

Which API would you like to use?
1. Account Info
2. Champion Rotation
3. Summoner Info
4. Match History
5. Match Info
6. League Info
7. League Position
8. Spectator Info
9. Champion Mastery
10. Champion Mastery Score

Enter your choice: 3

You chose Summoner Info.

Response Data:
{
    "summonerId": "123456",
    "name": "SummonerName",
    ...
}

Would you like to use another API?
1. Yes
2. No

Enter your choice: 2

Goodbye!
```

## Dependencies

- [cURL](https://curl.se/): This program uses the cURL library for making HTTP requests. Make sure you have cURL installed on your system.

## Notes

- Some API endpoints (e.g., Match History, Match Info, League Info, etc.) are not yet implemented and will require further development.

- Be cautious with sharing your API key and personal information.

## License

This program is available under the MIT License.
