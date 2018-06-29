#include <stdio.h>
#include <hircluster.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    srand(time(NULL));
    const int randNumber = rand() % 20;
    //Set up CLUSTER_ADDRESS IP:Port String
    char *tempClusterString[1024];
    if (argc > 1){
        int i;
        for(i = 1; i < argc; i++) {
            strcat(tempClusterString, argv[i]);
        }
    } else {
        strcat(tempClusterString, "127.0.0.1:6379");
    }
    printf("tempClusterString:[%s]\n", tempClusterString);

    const char *CLUSTER_ADDRESS = tempClusterString;
    char *key="key-a";
    char *field="field-a";
    char *value="value-a";
    char *field1="field1";
    char *value1="value1";
    char *field2="field/3";
    char *value2="value/3";
    redisClusterContext *cc;
    redisReply *reply;

    //Set up Redis Cluster Connection
    cc = redisClusterConnect(CLUSTER_ADDRESS,HIRCLUSTER_FLAG_ROUTE_USE_SLOTS);
    if(cc == NULL || cc->err)
    {
        printf("connect error : %s\n", cc == NULL ? "NULL" : cc->errstr);
        return -1;
    }

    //Ping (Unsupported)
    //Pub/Sub (Unsure)
    //Cluster Nodes (Unsupported)
    //Cluster Info (Unsupported)

    //Incr
    int incrCounter;
    for(incrCounter=0; incrCounter < randNumber; incrCounter++) {
    	reply = redisClusterCommand(cc, "incr ExampleCounter");
    	if (reply == NULL){
    		printf("Error with incr, reply is null[%s]\n", cc->errstr);
		    redisClusterFree(cc);
		return -1;
    	}
   	    printf("INCR[ExampleCounter]:%d\n", reply->integer);
    	freeReplyObject(reply);
    }

    //hmset
    reply = redisClusterCommand(cc, "hmset %s %s %s %s %s %s %s", key, field, value, field1, value1, field2, value2);
    if(reply == NULL)
    {
        printf("Error with hmset, reply is null[%s]\n", cc->errstr);
        redisClusterFree(cc);
        return -1;
    }
    printf("HMSET:%s\n", reply->str);
    freeReplyObject(reply);

    //hmget
    reply = redisClusterCommand(cc, "hgetall %s", key);
    if(reply == NULL)
    {
        printf("Error with hmget, reply is null[%s]\n", cc->errstr);
        redisClusterFree(cc);
        return -1;
    }
    printf("HMGET:\n");
    int hmgetCounter;
    for(hmgetCounter = 0; hmgetCounter < reply->elements; hmgetCounter++) {
        printf("%s\n", reply->element[hmgetCounter]->str);
    }
    freeReplyObject(reply);

    redisClusterFree(cc);
    return 0;
}
