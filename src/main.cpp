#include <iostream>
#include <string>
#include <curl/curl.h>
#include <spotify/json.hpp>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

int main() {
    std::string songName;
    std::cout << "Enter the name of a song: ";
    std::getline(std::cin, songName);

    std::string queryUrl = "https://api.spotify.com/v1/search?q=" + songName + "&type=track";

    curl_global_init(CURL_GLOBAL_ALL);
    CURL* curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, queryUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

        std::string response;
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);

        if (res == CURLE_OK) {
            try {
                spotify::json::value json = spotify::json::parse_string(response);

                // Process the JSON data and display the desired information
                // For example, you can display the first result's name and artist
                const auto& tracks = json["tracks"]["items"];
                if (tracks.is_array() && !tracks.empty()) {
                    const auto& track = tracks[0];
                    std::string trackName = track["name"].get<std::string>();
                    std::string artistName = track["artists"][0]["name"].get<std::string>();

                    std::cout << "Track: " << trackName << std::endl;
                    std::cout << "Artist: " << artistName << std::endl;
                } else {
                    std::cout << "No results found for the song." << std::endl;
                }
            } catch (const std::exception& e) {
                std::cerr << "Error parsing JSON response: " << e.what() << std::endl;
            }
        } else {
            std::cerr << "Failed to perform HTTP request: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
    } else {
        std::cerr << "Failed to initialize curl" << std::endl;
    }

    curl_global_cleanup();

    return 0;
}
