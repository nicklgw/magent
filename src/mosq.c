#define _GNU_SOURCE
#include <stdio.h>

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <mosquitto.h>

#include "mosq.h"
#include "application.h"

//static struct mosq_config cfg;
//static struct mosquitto *mosq = NULL;


void init_config(struct mosq_config *cfg)
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
	
}

void client_config_cleanup(struct mosq_config *cfg)
{
	int i;
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
		for(i=0; i<cfg->topic_count; i++){
			if(cfg->topics[i]) free(cfg->topics[i]);
		}
		free(cfg->topics);
	}
}

int client_opts_set(struct mosquitto *mosq, struct mosq_config *cfg)
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



