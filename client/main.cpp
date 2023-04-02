#include "client.hpp"

#define CODE_ROOM_UNAVAILABLE '0'
#define CODE_ROOM_SIZE '1'
#define CODE_START_GAME '2'

#define CODE_PLAYER_DATA '3'
#define CODE_PLAYER_DEAD '4'

int main(int argc, char *argv[])
{
    // creates a client that can read from and write to the server
    Client client("localhost", 8080);

    if (!client.is_connected())
        return 1;
    else
        std::cout << "Client connected!" << std::endl;

    char code;
    client.recv(&code, 1);

    if (code == CODE_ROOM_UNAVAILABLE)
    {
        std::cout << "Room unavailable for the moment... Please wait a while and try again!" << std::endl;
        return 0;
    }
    while (code == CODE_ROOM_SIZE)
    {
        char frac[3];
        client.recv(frac, 3);
        std::cout << "Players in the room: " << frac << std::endl;
        client.recv(&code, 1);
    }
    if (code == CODE_START_GAME)
    {
        std::cout << "Get ready! Game will start!" << std::endl;
    }

    char input = 1;
    char code_to_send = CODE_PLAYER_DATA;
    char watching_id;
    while (code_to_send == CODE_PLAYER_DATA)
    {
        client.send(&code_to_send, 1);

        client.recv(&watching_id, 1);
        std::cout << "Vendo mapa de " << watching_id << std::endl;

        sleep(3);
    }

    client.disconnect();
    return 0;
}