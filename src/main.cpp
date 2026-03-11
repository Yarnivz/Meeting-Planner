#include <iostream>

#include "App.h"

int main() {
    App* app = new App();
    Room* room = new Room("M.A.123", "a123", 10);
    Meeting* m = new Meeting("First meeting", "123", "a123", Date(2026, 1, 31));
    app->addRoom(room);
    app->addMeeting(m);
    app->writeToStream(std::cout);
    delete app;
    delete room;
    delete m;
}
