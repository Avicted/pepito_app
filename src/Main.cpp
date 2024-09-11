#include "Includes.hpp"
#include "ServerSideEventsClient.hpp"

// Global atomic flag for handling signals
std::atomic<bool> running(true);

static void
signal_handler(int signal)
{
    if (signal == SIGINT)
    {
        running = false;
    }
}

static void
handle_input_args(int argc, char **argv)
{
    if (argc > 1)
    {
        std::cout << "Arguments:" << std::endl;
        for (int i = 1; i < argc; i++)
        {
            std::cout << "  " << i << ": " << argv[i] << std::endl;
        }
    }
}

static void
handleSSEMessage(const std::string &data)
{
    try
    {
        // Strip the "data: " prefix
        std::string jsonDataStr = data.substr(6);

        // Parse the JSON data
        auto jsonData = json::parse(jsonDataStr);

        // Check the event type
        std::string eventType = jsonData.at("event").get<std::string>();

        if (eventType == "heartbeat")
        {
            long time = jsonData.at("time").get<long>();
            std::cout << "Heartbeat event at time: " << time << std::endl;
        }
        else if (eventType == "pepito")
        {
            std::string type = jsonData.at("type").get<std::string>();
            std::string img = jsonData.at("img").get<std::string>();
            long time = jsonData.at("time").get<long>();
            std::cout << "Pepito event: " << type << " at time: " << time << " with image: " << img << std::endl;

            // Create a PepitoEvent object
            PepitoEvent pepitoEvent(type, img, time);

            // @Note(Victor): This is where you would handle the event
        }
        else
        {
            std::cerr << "Unknown event type: " << eventType << std::endl;
        }
    }
    catch (const json::exception &e)
    {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
        std::cerr << "Failed to parse data: " << data << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << "Failed to process data: " << data << std::endl;
    }
}

static void
listenForSSE(const std::string &url)
{
    SSEClient client(url);
    client.setHandler(handleSSEMessage);

    while (running)
    {
        try
        {
            client.startListening();
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error while listening for SSE: " << e.what() << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(5)); // Wait before reconnecting
        }
    }
}

int main(int argc, char **argv)
{
    std::cout << "Hello, Sailor!" << std::endl;

    handle_input_args(argc, argv);
    signal(SIGINT, signal_handler);

    // Test event handling with sample data
    std::ifstream file("test_data.sse_data");

    std::cout << "Reading test data..." << std::endl;
    if (file.is_open())
    {
        // Each line is a separate SSE message (JSON object)
        std::string line;

        // Test the handleSSEMessage function
        while (std::getline(file, line))
        {
            if (line.empty())
            {
                continue;
            }

            handleSSEMessage(line);
        }

        file.close();

        std::cout << "Finished reading test data.\n"
                  << std::endl;
    }
    else
    {
        std::cerr << "Failed to open test_data.sse_data\n"
                  << std::endl;
    }

    // Setup Server-Sent Event SSE
    std::string url = "https://api.thecatdoor.com/sse/v1/events";
    std::thread sseThread(listenForSSE, url);

    // Main event loop
    while (running)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "Shutting down..." << std::endl;
    sseThread.join();

    return 0;
}
