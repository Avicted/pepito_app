# Pépito C++ program

This is a C++ program that listens to Server-Sent Events (SSE) and prints the messages to the console.
All messages are parsed, and the program serves as an example of how to parse SSE messages in C++.

Using this code as the base, you can extend it to do whatever you want with the messages. 

The program is using the [Pépito API]( 
https://github.com/Clement87/Pepito-API/tree/main?tab=readme-ov-file#p%C3%A9pito-api) to get the messages.

For example you could:
- When Pépito goes in or out of the house 
    - Connect to all Philips Hue bridges on your network and flash the lights when a particular message is received.

## How to build

VSCode press F5 to build and run the program.

```
# Build the program
clear && meson setup build --buildtype=debug && cd build && ninja && cd ..

# Run the program
./build/pepito_app
```


## License
MIT License


## Here is one Official Pépito account:
https://x.com/PepitoTheCat 

