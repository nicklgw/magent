
#include <string.h>
#include <libubox/ulog.h>
#include "application.h"

static struct application the_app = { 
	
	.enauth = 0,
	
};

struct application* get_app(void)
{	
	return &the_app;
}

void app_test_init(void)
{
	get_app()->did = strdup("3cee88ee191c4e1380739b0388e5f9b9");
	get_app()->passwd = strdup("a3qqc0lc");
	get_app()->sn = strdup("1");
	get_app()->mac = strdup("08002765c466");
	get_app()->model = strdup("lds.gateway.g151");
	get_app()->mqtt_host = strdup("pre-azuremqtt.oreinlighting.com");
	get_app()->mqtt_port = 8883;
	get_app()->cafile = strdup("/etc/magent/rootCA.crt");
	get_app()->certfile = strdup("/etc/magent/cert.pem");
	get_app()->keyfile = strdup("/etc/magent/privkey.pem");
	get_app()->token = strdup("d626446c");
	get_app()->fw_ver = strdup("1.40");
	get_app()->hw_ver = strdup("1.0");
}

void magent_conf_dump(void)
{
	ULOG_INFO("=========magent_conf_dump begin=============== \n");
	ULOG_INFO("did: %s \n", get_app()->did);
	ULOG_INFO("passwd: %s \n", get_app()->passwd);
	ULOG_INFO("sn: %s \n", get_app()->sn);
	ULOG_INFO("mac: %s \n", get_app()->mac);
	ULOG_INFO("model: %s \n", get_app()->model);
	ULOG_INFO("mqtt_host: %s \n", get_app()->mqtt_host);
	ULOG_INFO("mqtt_port: %d \n", get_app()->mqtt_port);
	ULOG_INFO("cafile: %s \n", get_app()->cafile);
	ULOG_INFO("certfile: %s \n", get_app()->certfile);
	ULOG_INFO("keyfile: %s \n", get_app()->keyfile);
	ULOG_INFO("token: %s \n", get_app()->token);
	ULOG_INFO("fw_ver: %s \n", get_app()->fw_ver);
	ULOG_INFO("hw_ver: %s \n", get_app()->hw_ver);
	ULOG_INFO("=========magent_conf_dump end================= \n");
}

