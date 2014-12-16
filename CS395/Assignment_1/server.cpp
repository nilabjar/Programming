#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include<vector>
#include<string>
#include<sstream>

#include<pthread.h>

using namespace std;

vector<std::string>
split (const char* string, char splitter)
{
  stringstream ss(string);
  std::string s;

  vector<std::string> tokens;

  while (getline(ss, s, splitter))
  {
    tokens.push_back(s);
  }

  return tokens;
}

void* client_respond(void* conn_details)
{
    char sendBuff[1024];
    memset(sendBuff, '0', sizeof(sendBuff));

    //get the conn socket
    int connfd = *((int*)conn_details);

    int n = read( connfd, sendBuff, sizeof(sendBuff)-1 );

    sendBuff[n] = 0; // set the null value at the end

    //parse for the filename
    char* p = strtok(sendBuff, " ");
    p = strtok(NULL, " "); // this will get the second word ...filename

    if (p == NULL) // no file name specified ..
    {
      cout << "No file name specified" << endl;
      return 0;
    }

    std::string s = p+1; // get rid of the first '/'

    ifstream in (s.c_str());
    cout << "Returning File " << s.c_str() << endl;

    while (in)
    {
      memset(sendBuff, 0 , sizeof(sendBuff));
      in.read ( sendBuff, sizeof(sendBuff)-1 );
      write( connfd, sendBuff, strlen(sendBuff)-1 );
    }

    in.close();

    close (connfd);
}

int main (int argc, char* argv[])
{

  struct sockaddr_in serv_addr;

  int listenfd = socket(AF_INET, SOCK_STREAM, 0);
  memset(&serv_addr, '0', sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(atoi(argv[1]));

  bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

  listen(listenfd, 10);

  while (true)
  {
    cout << "Listening for incoming request" << endl;
    int connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);

    pthread_t thread;
    pthread_create(&thread, NULL, client_respond, (void*)&connfd);

    sleep(1);
  }
}
