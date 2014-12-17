#include <iostream>
#include <string>
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
#include<signal.h>
//using namespace std;

/*
std::vector<std::string>
split (const char* string, char splitter)
{
  std::stringstream ss(std::string);
  std::string s;

  std::vector<std::string> tokens;

  while (std::getline(ss, s, splitter))
  {
    tokens.push_back(s);
  }

  return tokens;
}
*/
bool terminate;
std::vector<int> sockets;
pthread_cond_t cond;
pthread_mutex_t mutex;

void
handle(int signum) {

  std::cout << "Inside the signal handler" << std::endl;
  if (signum == SIGINT)
    terminate = true;
}

void* client_respond(void* conn_details)
{
  std::cout << "Thread starting " << std::endl;

  while (!terminate) {
    char sendBuff[1024];
    memset(sendBuff, '0', sizeof(sendBuff));

    pthread_mutex_lock(&mutex);
    while ( sockets.size() == 0)
	pthread_cond_wait(&cond, &mutex); 
    
    int connfd = sockets[0];
    sockets.erase(sockets.begin());
    pthread_mutex_unlock(&mutex);
    //get the conn socket
    //int connfd = *((int*)conn_details);

    

    int n = read( connfd, sendBuff, sizeof(sendBuff)-1 );

    sendBuff[n] = 0; // set the null value at the end

    //parse for the filename
    char* p = strtok(sendBuff, " ");
    p = strtok(NULL, " "); // this will get the second word ...filename

    if (p == NULL) // no file name specified ..
    {
      std::cout << "No file name specified" << std::endl;
      return 0;
    }

    std::string s = p+1; // get rid of the first '/'

    std::ifstream in (s.c_str());
    std::cout << "Returning File " << s.c_str() << std::endl;

    while (in)
    {
      memset(sendBuff, 0 , sizeof(sendBuff));
      in.read ( sendBuff, sizeof(sendBuff)-1 );
      write( connfd, sendBuff, strlen(sendBuff)-1 );
    }

    in.close();

    close (connfd);

  }
}

int main (int argc, char* argv[])
{
  terminate = false;
  struct sockaddr_in serv_addr;

  int listenfd = socket(AF_INET, SOCK_STREAM, 0);
  memset(&serv_addr, '0', sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(atoi(argv[1]));

  if ( bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
  {
    perror ("Error in bind : ");
    return -1;
  }

  if ( listen(listenfd, 10) < 0)
  {
    perror ("Error in listen : ");
    return -1;
  }

  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond, NULL);

  std::vector<pthread_t> threads;
  
  signal(SIGINT, handle);

  for (int i = 0;i < 5;i++) {
    pthread_t thread;
    pthread_create(&thread, NULL, client_respond, NULL);
    threads.push_back(thread);
  }

  while (!terminate)
  {
    std::cout << "Listening for incoming request" << std::endl;
    int connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);

    std::cout << "Got an incoming request" << std::endl;
    pthread_mutex_lock(&mutex);
    sockets.push_back(connfd);
    pthread_mutex_unlock(&mutex);
    
    pthread_cond_signal(&cond);

    sleep(1);
  }

  for (int i = 0;i < 5;i++)
    pthread_join(threads[i], NULL);
  
}
