
#ifndef __MOSQ_H__
#define __MOSQ_H__

#include <stdio.h>
#include <stdbool.h>
#include <libubox/uloop.h>

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

struct mqtt_client {
	struct mosquitto *mosq;
	struct uloop_fd sock;
	
};

int mqtt_init(void);

int mqtt_connect(void);

int mqtt_add_uloop(void);

void mqtt_cleanup(void);

int mqtt_publish(const char *topic, int payloadlen, const void *payload);

#endif // __MOSQ_H__

