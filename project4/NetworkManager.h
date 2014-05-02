// 
// NetworkManager.h
// 
// Manage network connections to/from engine. 
// 

#define DRAGONFLY_PORT "9109"
#include "Manager.h"

class NetworkManager : public Manager {

 private:
  NetworkManager();                       // Private since a singleton.
  NetworkManager(NetworkManager const &s);  // Don't allow copy.
  void operator=(NetworkManager const&);  // Don't allow assignment.
  int sock;                               // Connected network socket.
  
 public:

  // Get the one and only instance of the NetworkManager.
  static NetworkManager &getInstance();

  // Start up NetworkManager.
  int startUp();

  // Shut down NetworkManager.
  void shutDown();

  // Accept only network events.
  // Returns false for other engine events.
  bool isValid(string event_type);

  // Block, waiting to accept network connection.
  int accept(string port = DRAGONFLY_PORT);

  // Make network connection.
  // Return 0 if success, else -1.
  int connect(string host, string port = DRAGONFLY_PORT);

  // Close network connection.
  // Return 0 if success, else -1.
  int close();

  // Send buffer to connected network.
  // Return 0 if success, else -1.
  int send(void *buffer, int bytes);

  // Receive from connected network (no more than bytes).
  // Return number of bytes received, else -1 if error.
  int receive(void *buffer, int bytes);

  // Check if network data.
  // Return amount of data (0 if no data), -1 if not connected or error.
  int isData();

  // Return true if network connected, else false.
  bool isConnected();

  // Return socket.
  int getSocket();
};