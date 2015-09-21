#include "config.h"
#include <curl/curl.h>
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>
#ifdef USEREDIS
#include <hiredis/hiredis.h>
#endif


long long current_timestamp() {
    struct timeval te; 
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // caculate milliseconds
    // printf("milliseconds: %lld\n", milliseconds);
    return milliseconds;
}


int main(int argc, char *argv[]) {
  CURLcode ret;
  CURL *hnd;
  struct curl_slist *slist1;

  if (argc != 4 ) {
	  printf( "usage: %s TARGETDOMAIN IP CHECKTYPE \n", argv[0] );
	  return -1;
  }

  size_t hdrsize;
  size_t urlsize;
  double sttfb;
  double connecttime;
  double totaltime;
  sttfb = 0.0;
  connecttime = 0.0;
  totaltime = 0.0;

  hdrsize = strlen(argv[1]) + 10;
  char header[hdrsize];
  strncpy(header, "Host: ", 8);
  strncat(header, argv[1], strlen(argv[1]) );

  urlsize = strlen(argv[2]) + 10;
  char url[urlsize];
  strncpy(url, "http://", 8);
  strncat(url, argv[2], urlsize );
  strncat(url, "/", 2);

  slist1 = NULL;
  slist1 = curl_slist_append(slist1, header);

  hnd = curl_easy_init();
  curl_easy_setopt(hnd, CURLOPT_URL, url);
  curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
  curl_easy_setopt(hnd, CURLOPT_USERAGENT, "sitesTTFBChecker");
  curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, slist1);
  curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
  curl_easy_setopt(hnd, CURLOPT_NOBODY, 1);
  struct timeval tv;
  gettimeofday(&tv, NULL);

  long long timestamp;
  timestamp = current_timestamp();

  printf("%lld\t", timestamp);

  ret = curl_easy_perform(hnd);

  curl_easy_getinfo(hnd, CURLINFO_STARTTRANSFER_TIME, &sttfb);
  curl_easy_getinfo(hnd, CURLINFO_CONNECT_TIME, &connecttime);
  curl_easy_getinfo(hnd, CURLINFO_TOTAL_TIME, &totaltime);
  sttfb*=1000;
  connecttime*=1000;
  totaltime*=1000;
#ifdef USEREDIS

  // Now lets us start using environment variables for Redis information
  char *redisHost, *redisAuth;
  redisHost = getenv("GETTTFB_REDISHOST");
  redisAuth = getenv("GETTTFB_REDISAUTH");

  redisReply *reply;
  redisContext *c = redisConnect(redisHost, 6379);
  if (c->err) {
	printf("Error: %s\n", c->errstr);
	return 0;
  }
  reply = redisCommand(c, "auth %s", redisAuth );

  reply = redisCommand(c, "zadd %s:%s:connect %lld %f", argv[1], argv[3], timestamp, connecttime);
  reply = redisCommand(c, "zadd %s:%s:sttfb %lld %f", argv[1], argv[3], timestamp, sttfb);
  reply = redisCommand(c, "zadd %s:%s:total %lld %f", argv[1], argv[3], timestamp, totaltime);
#endif

  curl_easy_cleanup(hnd);
  curl_slist_free_all(slist1);
  hnd = NULL;
  slist1 = NULL;
  printf("%fms\t", sttfb);
  printf("%fms\t", connecttime);
  printf("%fms\n", totaltime);

  return (int)ret;
}
