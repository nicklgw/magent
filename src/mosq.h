
#ifndef __MOSQ_H__
#define __MOSQ_H__

#include <stdio.h>
#include <stdbool.h>

#define DEVICE_SUB_TOPIC	"iot/v1/c/%s/#"

struct mosq_config {
	char *id;
	int protocol_version;
	int keepalive;
	char *host;
	int port;
	int qos;
	bool retain;
	bool debug;
	bool quiet;
	unsigned int max_inflight;
	char *username;
	char *password;
	char *will_topic;
	char *will_payload;
	long will_payloadlen;
	int will_qos;
	bool will_retain;
	char *cafile;
	char *capath;
	char *certfile;
	char *keyfile;
	bool clean_session; /* sub */
	char **topics; /* sub */
	int topic_count; /* sub */
	bool no_retain; /* sub */
	bool verbose; /* sub */
	bool eol; /* sub */
	int msg_count; /* sub */
};

void init_config(struct mosq_config *cfg);
void client_config_cleanup(struct mosq_config *cfg);
int client_opts_set(struct mosquitto *mosq, struct mosq_config *cfg);


#endif // __MOSQ_H__

