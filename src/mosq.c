#define _GNU_SOURCE
#include <stdio.h>

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <mosquitto.h>
#include <libubox/ulog.h>

#include "mosq.h"
#include "utils.h"
#include "application.h"

/* For when not using winsock libraries. */
#ifndef INVALID_SOCKET
#define INVALID_SOCKET -1
#endif

#define MQTT_TICK_PERIOD			1000

static struct mosq_config cfg;
static struct mqtt_client mcl;

static void init_config(struct mosq_config *cfg)
{
	memset(cfg, 0, sizeof(*cfg));
	cfg->port = 1883;
	cfg->max_inflight = 20;
	cfg->keepalive = 60;
	cfg->clean_session = true;
	cfg->eol = true;
	cfg->protocol_version = MQTT_PROTOCOL_V31;
	
	cfg->verbose = 1;
	cfg->debug = true;
	cfg->qos = 1;
	cfg->host = strdup(get_app()->mqtt_host);
	cfg->port = get_app()->mqtt_port;
	cfg->id = strdup(get_app()->did);
	
	cfg->username = strdup(get_app()->did);
	cfg->password = strdup(get_app()->passwd);
	cfg->cafile = strdup(get_app()->cafile);
	cfg->certfile = strdup(get_app()->certfile);
	cfg->keyfile = strdup(get_app()->keyfile);
	
	char *topic = NULL;
	asprintf(&topic, DEVICE_SUB_TOPIC, get_app()->did);
	
	cfg->topic_count++;
	cfg->topics = realloc(cfg->topics, cfg->topic_count*sizeof(char *));
	cfg->topics[cfg->topic_count-1] = strdup(topic);
	
	char *will_topic = NULL;
	char *will_payload = NULL;
	int   will_payloadlen = 0;
	
	asprintf(&will_topic, "iot/v1/cb/%s/device/disconnect", get_app()->did);
	will_payloadlen = asprintf(&will_payload,
		"{\"service\":\"device\",\"method\":\"disconnect\",\"seq\":\"%s\",\"srcAddr\":\"1.%s\",\"payload\":{\"timestamp\":\"%s\"}}",
		get_random(NULL), get_app()->did, get_timestamp(NULL));
	
	cfg->will_topic = will_topic;
	cfg->will_payload = will_payload;
	cfg->will_payloadlen = will_payloadlen;
	cfg->will_qos = 1;
	cfg->will_retain = false;
}

static void client_config_cleanup(struct mosq_config *cfg)
{
	if(cfg->id) free(cfg->id);
	if(cfg->host) free(cfg->host);
	if(cfg->username) free(cfg->username);
	if(cfg->password) free(cfg->password);
	if(cfg->will_topic) free(cfg->will_topic);
	if(cfg->will_payload) free(cfg->will_payload);
	if(cfg->cafile) free(cfg->cafile);
	if(cfg->capath) free(cfg->capath);
	if(cfg->certfile) free(cfg->certfile);
	if(cfg->keyfile) free(cfg->keyfile);
	if(cfg->topics){
		for(int i = 0; i < cfg->topic_count; i++){
			if(cfg->topics[i]) free(cfg->topics[i]);
		}
		free(cfg->topics);
	}
}

static int client_opts_set(struct mosquitto *mosq, struct mosq_config *cfg)
{
	if(cfg->will_topic && mosquitto_will_set(mosq, cfg->will_topic,
				cfg->will_payloadlen, cfg->will_payload, cfg->will_qos,
				cfg->will_retain)){

		if(!cfg->quiet) fprintf(stderr, "Error: Problem setting will.\n");
		mosquitto_lib_cleanup();
		return 1;
	}
	if(cfg->username && mosquitto_username_pw_set(mosq, cfg->username, cfg->password)){
		if(!cfg->quiet) fprintf(stderr, "Error: Problem setting username and password.\n");
		mosquitto_lib_cleanup();
		return 1;
	}
	if((cfg->cafile || cfg->capath)
			&& mosquitto_tls_set(mosq, cfg->cafile, cfg->capath, cfg->certfile, cfg->keyfile, NULL)){

		if(!cfg->quiet) fprintf(stderr, "Error: Problem setting TLS options.\n");
		mosquitto_lib_cleanup();
		return 1;
	}
	mosquitto_max_inflight_messages_set(mosq, cfg->max_inflight);
	mosquitto_opts_set(mosq, MOSQ_OPT_PROTOCOL_VERSION, &(cfg->protocol_version));
	return MOSQ_ERR_SUCCESS;
}

static void my_log_callback(struct mosquitto *mosq, void *obj, int level, const char *str)
{
	
}
static void my_subscribe_callback(struct mosquitto *mosq, void *obj, int mid, int qos_count, const int *granted_qos)
{
	
}
static void my_publish_callback(struct mosquitto *mosq, void *obj, int mid)
{
	
}
static void my_connect_callback(struct mosquitto *mosq, void *obj, int result)
{
	
}
static void my_disconnect_callback(struct mosquitto *mosq, void *obj, int rc)
{
	
}
static void my_message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message)
{
	
}

int mqtt_init(void)
{
	init_config(&cfg);
	
	mosquitto_lib_init();
	
	mcl.mosq = mosquitto_new(cfg.id, cfg.clean_session, &cfg);
	if(!mcl.mosq)
	{
		switch(errno)
		{
			case ENOMEM:
				if(!cfg.quiet) fprintf(stderr, "Error: Out of memory.\n");
				break;
			case EINVAL:
				if(!cfg.quiet) fprintf(stderr, "Error: Invalid id and/or clean_session.\n");
				break;
		}
		mosquitto_lib_cleanup();
		return -1;
	}
	
	if(client_opts_set(mcl.mosq, &cfg))
		return -1;
	
	if(cfg.debug)
	{
		mosquitto_log_callback_set(mcl.mosq, my_log_callback);
		mosquitto_subscribe_callback_set(mcl.mosq, my_subscribe_callback);
		mosquitto_publish_callback_set(mcl.mosq, my_publish_callback);
	}
	mosquitto_connect_callback_set(mcl.mosq, my_connect_callback);
	mosquitto_disconnect_callback_set(mcl.mosq, my_disconnect_callback);
	mosquitto_message_callback_set(mcl.mosq, my_message_callback);
	
	return 0;
}

int mqtt_connect(void)
{
	return mosquitto_connect(mcl.mosq, cfg.host, cfg.port, cfg.keepalive);
}

void mqtt_cleanup(void)
{
	mosquitto_destroy(mcl.mosq);
	mosquitto_lib_cleanup();
	client_config_cleanup(&cfg);
	mcl.mosq = NULL;
}

int mqtt_publish(const char *topic, int payloadlen, const void *payload)
{
	ULOG_INFO("mosq_publish topic: %s \n", topic);		
	ULOG_INFO("mosq_publish payloadlen: %d \n", payloadlen);		
	ULOG_INFO("mosq_publish payload: \n");		
	{
		char *str = strndup(payload, payloadlen);
		ULOG_INFO("%s \n", str);
		free(str);
	}
	ULOG_INFO("\n");
	
	int ret = mosquitto_publish(mcl.mosq, NULL, topic, payloadlen, payload, 1, false);
	
	if(mosquitto_want_write(mcl.mosq))
	{
		/* get an event once we can write to the socket again */
		uloop_fd_add(&mcl.sock, ULOOP_READ | ULOOP_WRITE | ULOOP_EDGE_TRIGGER);
	}
	
	return ret;
}

static void client_cb(struct uloop_fd *sock, unsigned int events)
{
	struct mqtt_client *cl = container_of(sock, struct mqtt_client, sock);
	int max_packets = 1;	
	int rc;
	
	if(events & ULOOP_WRITE)
	{
		rc = mosquitto_loop_write(cl->mosq, max_packets);
		if(rc || mosquitto_socket(cl->mosq) == INVALID_SOCKET) { return /*rc*/; }
	}
	
	/* prevent further ULOOP_WRITE events if we don't have data
	 * to send anymore */
	if (!mosquitto_want_write(cl->mosq) && (events & ULOOP_WRITE))
		uloop_fd_add(sock, ULOOP_READ | ULOOP_EDGE_TRIGGER);
	
	if(events & ULOOP_READ)
	{
		rc = mosquitto_loop_read(cl->mosq, max_packets);
		if(rc || mosquitto_socket(cl->mosq) == INVALID_SOCKET) { return /*rc*/;	}
	}
}

static void mqtt_tick_cb(struct uloop_timeout *timeout)
{
	if(!mcl.mosq) // mcl.mosq == NULL， 重新初始化
	{
		
	}
	else if(mosquitto_socket(mcl.mosq) < 0) // mcl.mosq->sock无效, 重连
	{
		mosquitto_reconnect(mcl.mosq);
	}
	else // 正常连接
	{
		mosquitto_loop_misc(mcl.mosq);
	}
	
	uloop_timeout_set(timeout, MQTT_TICK_PERIOD);
}
static struct uloop_timeout mqtt_tick_timer = {
	.cb = mqtt_tick_cb
};

int mqtt_add_uloop(void)
{
	int client_fd;
	
	client_fd = mosquitto_socket(mcl.mosq);
	if(client_fd < 0)
		return -1;
	
	mcl.sock.fd = client_fd;
	mcl.sock.cb = client_cb;
	uloop_fd_add(&mcl.sock, ULOOP_READ | ULOOP_EDGE_TRIGGER);
	
	uloop_timeout_set(&mqtt_tick_timer, MQTT_TICK_PERIOD);
	
	return 0;
}

