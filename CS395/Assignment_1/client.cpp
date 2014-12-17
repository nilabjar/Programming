#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <fstream>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


using namespace std;
int main (int argc, char*argv[])
{

  if (argc < 3) {
    cout << "Usage: " << argv[0] << " <ip address> <port>" << endl;
    return 1;
  }

  int sockfd = 0;
  struct sockaddr_in serv_addr;

  if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    cout << "Error : Could not create socket" << endl;
    perror("Error: ");
    return 1;
  }

  int port = atoi(argv[2]);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);

  cout << "Making a call to " << argv[1] <<  " " << port << endl;

  if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0)
  {
    cout << "Error in inet_pton" << endl;
    perror("Error: ");
    return 1;
  }

  if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0 )
  {
    cout << "Error in connect call " << endl;
    perror("Error: ");
    return 1;
  }


  // connection established, now send a basic http get call
  const char* msg = "GET /index.html HTTP/1.0\n\n";
//  const char* msg = "testfile";

  if (write(sockfd, msg, strlen(msg)) < 0)
  {
    cout << "Error in writing http GET" << endl;
    return 1;
  }

  char recvBuffer[1024];
  memset(recvBuffer, 0, 1024);

  int n = 0;
  char* content = NULL;
  char type[100];
  ofstream out ("tmpout");

  while ( (n = read(sockfd, recvBuffer, sizeof(recvBuffer)-1)) > 0 )
  {
    recvBuffer[n] = 0;
    if ( (content = strstr(recvBuffer, "Content-Type:")) != NULL)
    {
      content += 14; // pointing to the content
      memset(type , 0 , sizeof(type));
      strncpy(type, content, 9);
      cout << "Content is " << type << endl;
    }

    out.write(recvBuffer, n);
  }

  out.close();

  // now open the file ...
  if ( !strcmp(type, "text/html") )
  {
    char command[100];
    memset(command, 0, 100);
    sprintf(command, "%s %s", "/bin/more", "tmpout");
//    sprintf(command, "%s %s", "open -a TextEdit ", "tmpout");
    system (command);
  }

  if (n < 0)
    cout << "Read error" << endl;

  return 0;
}
