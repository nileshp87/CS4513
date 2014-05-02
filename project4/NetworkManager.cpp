// 
// NetworkManager.h
// 
// Manage network connections to/from engine. 
// 

#include "NetworkManager.h"

NetworkManager::NetworkManager(){
  return;
}
NetworkManager::NetworkManager(NetworkManager const &s){
  return;
}
void operator=(NetworkManager const &s){
  return;
}
int sock;                               // Connected network socket.


// Get the one and only instance of the NetworkManager.
static NetworkManager &getInstance(){
  return this;
}

// Start up NetworkManager.
int startUp(){
  return 1;
}

// Shut down NetworkManager.
void shutDown(){
  return;
}

// Accept only network events.
// Returns false for other engine events.
bool isValid(string event_type){
  return true;
}

// Block, waiting to accept network connection.
int accept(string port = DRAGONFLY_PORT){
  return 1;
}

// Make network connection.
// Return 0 if success, else -1.
int connect(string host, string port = DRAGONFLY_PORT){
  return 1;
}

// Close network connection.
// Return 0 if success, else -1.
int close(){
  return 1;
}

// Send buffer to connected network.
// Return 0 if success, else -1.
int send(void *buffer, int bytes){
  return 1;
}

// Receive from connected network (no more than bytes).
// Return number of bytes received, else -1 if error.
int receive(void *buffer, int bytes){
  return 1;
}

// Check if network data.
// Return amount of data (0 if no data), -1 if not connected or error.
int isData(){
  return 1;
}

// Return true if network connected, else false.
bool isConnected(){
  return 1;
}

// Return socket.
int getSocket(){
  return 1;
}