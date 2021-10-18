#include "to_send.hpp"


// Define the data we want to send
WSABUF toSend = {
    13,
    (CHAR*)"Hello there!\n"
};
