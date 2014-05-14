
#include "TcpServer.h"
#include <stdlib.h>
#include <iostream>

using namespace std;

class TestDataHandler : public DataHandler
{
  public:
  bool handle( char* data, size_t length, Connection& conn )
  {
    if(length >= 5)
    {
      data[length] = 0;
      cout << data;
      conn.write(data, length);
      return true;
    }
    else
    {
      return false;
    }
  }
};

int main( int ac, char** av )
{
  TestDataHandler handler;
  TcpServer server( handler );

  int retval = server.start("0.0.0.0", 8933);
  cout << "Waiting for connections..." << flush;
  while (retval == 0) {
    retval = server.run();
  }
  return retval;
}
