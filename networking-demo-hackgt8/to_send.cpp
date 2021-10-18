#include "to_send.hpp"


// Define the data we want to send
WSABUF toSend = {
    348,
    (CHAR *) "When you write your first socket application\n"
    "    __  __           __                                 \n"
    "   / / / /___ ______/ /_____  _________ ___  ____ _____ \n"
    "  / /_/ / __ `/ ___/ //_/ _ \\/ ___/ __ `__ \\/ __ `/ __ \\\n"
    " / __  / /_/ / /__/ ,< /  __/ /  / / / / / / /_/ / / / /\n"
    "/_/ /_/\\__,_/\\___/_/|_|\\___/_/  /_/ /_/ /_/\\__,_/_/ /_/ \n"
};
