
#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#define TOPIC_LEN              		1024

struct application {
	
	char *did; 						// fw_printenv DeviceUuid
	char *passwd; 					// fw_printenv DeviceChiper
	char *sn; 						// fw_printenv DeviceSn
	
	char *mac;
	char *model;
	
	char *mqtt_host; 				// �ƶ�CLOUD MQTT HOST
	int mqtt_port;					// �ƶ�CLOUD MQTT PORT
	
	char *cafile;
	char *certfile;
	char *keyfile;
	
	char *token;
	char *fw_ver;
	char *hw_ver;
	
	int enauth;
	
	char *name;	
	
	char *localIp;	
	char *mask;	
	char *gw;	
};

struct application* get_app(void);


#endif // __APPLICATION_H__

