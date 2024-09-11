#include "includes/includes.hpp"
#include "includes/ServerSideEventsClient.hpp"

// Define the variant type for event data
using EventData = std::variant<HeartbeatEvent, PepitoEvent>;

// Global atomic flag for handling signals
std::atomic<bool> running(true);

static void
signal_handler(int signal)
{
    std::cout << "Signal " << signal << " received." << std::endl;
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

            // @Note(Avic): This is where we could act and perform some action based on the event
            PepitoEvent pepitoEvent(type, img, time);
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

int main(int argc, char **argv)
{
    std::cout << "Hello, Sailor!" << std::endl;

    handle_input_args(argc, argv);
    signal(SIGINT, signal_handler);

    // Test event handling with sample data
    std::ifstream file("test_data.sse_data");
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
    }
    else
    {
        std::cerr << "Failed to open test_data.json" << std::endl;
    }

    // return 0; //  Stop here for testing

    // Setup Server-Side Event SSE
    std::string url = "https://api.thecatdoor.com/sse/v1/events";
    SSEClient client(url);
    client.setHandler(handleSSEMessage);

    // Start listening for SSE in a separate thread
    std::thread sseThread([&client]()
                          { client.startListening(); });

    // Main event loop
    while (running)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
