#include <string>
#include "functions.h"
#include "telegram.h"

void Ping(std::string) {
	telegram::Send("Pong");
}