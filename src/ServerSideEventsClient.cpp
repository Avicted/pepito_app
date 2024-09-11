#include "includes/ServerSideEventsClient.hpp"

#include <curl/curl.h>
#include <iostream>

SSEClient::SSEClient(const std::string &url) : _url(url), _handler(nullptr)
{
    std::cout << "SSEClient constructor called" << std::endl;
}

SSEClient::~SSEClient()
{
    std::cout << "SSEClient destructor called" << std::endl;
}

// Static callback function for handling SSE data
size_t SSEClient::writeCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t totalSize = size * nmemb;
    SSEClient *client = static_cast<SSEClient *>(userp);
    std::string data(static_cast<char *>(contents), totalSize);

    if (client->_handler)
    {
        client->_handler(data);
    }

    return totalSize;
}

void SSEClient::startListening()
{
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, _url.c_str());

        // Set up callback for handling incoming data
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

        // Enable verbose mode for debugging
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        // Perform the request
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK)
        {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}

// Set the handler for SSE messages
void SSEClient::setHandler(SSEHandler handler)
{
    _handler = handler;
}
