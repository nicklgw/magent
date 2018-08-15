
#include <sys/types.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <unistd.h>

#include <libubus.h>
#include <libubox/vlist.h>
#include <libubox/uloop.h>
#include <libubox/ulog.h>
#include <libubox/blobmsg_json.h>
#include <json-c/json.h>

#include "ubus.h"
#include "utils.h"

static struct ubus_auto_conn conn;
static struct blob_buf b;

static int
query_time(struct ubus_context *ctx, struct ubus_object *obj,
		    struct ubus_request_data *req, const char *method,
		    struct blob_attr *msg)
{
	void *r;
	char message[64] = { 0 };
	
	get_timestamp(message);
	
	blobmsg_buf_init(&b);
	r = blobmsg_open_array(&b, "result");
	blobmsg_add_string(&b, NULL, message);
	blobmsg_close_array(&b, r);
	
	return ubus_send_reply(ctx, req, b.head);
}

static const struct ubus_method magent_methods[] = {
	UBUS_METHOD_NOARG("query_time", query_time),
};

static struct ubus_object_type magent_object_type =
	UBUS_OBJECT_TYPE("magent", magent_methods);

static struct ubus_object magent_object = {
	.name = "magent",
	.type = &magent_object_type,
	.methods = magent_methods,
	.n_methods = ARRAY_SIZE(magent_methods),
};

static void
ubus_connect_handler(struct ubus_context *ctx)
{
	int ret;
	
	ret = ubus_add_object(ctx, &magent_object);
	if (ret)
		ULOG_ERR("Failed to add object: %s\n", ubus_strerror(ret));	
	
}

void
ubus_startup(void)
{
	conn.cb = ubus_connect_handler;
	ubus_auto_connect(&conn);
}

