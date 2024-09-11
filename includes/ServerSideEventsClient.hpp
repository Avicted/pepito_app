#pragma once

#include <string>
#include <functional>
#include <nlohmann/json.hpp> // Include a JSON library like nlohmann/json for parsing

using json = nlohmann::json;

// Define a base struct for common event data
struct BaseEvent
{
    std::string event; // Common field for event type
    long time;         // Common field for timestamp
};

class HeartbeatEvent
{
public:
    signed long long time;

    HeartbeatEvent(long t) : time(t) {}
};

struct PepitoEvent
{
    std::string type;
    std::string img;
    signed long long time;

    PepitoEvent(const std::string &t, const std::string &i, long tm)
        : type(t), img(i), time(tm) {}
};

// Forward declaration
using SSEHandler = std::function<void(const std::string &data)>;

class SSEClient
{
public:
    SSEClient(const std::string &url);
    ~SSEClient(void);

    void startListening();
    void setHandler(SSEHandler handler);

private:
    std::string _url;
    SSEHandler _handler;

    // Internal function to process incoming data
    static size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp);
};
