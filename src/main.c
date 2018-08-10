
#include <libubus.h>
#include <libubox/uloop.h>
#include <libubox/ulog.h>

#include "ubus.h"
#include "utils.h"

static void
signal_shutdown(int signal)
{
	uloop_end();
}

#define HEARTBEAT_PERIOD 				1000

static void heartbeat_cb(struct uloop_timeout *timeout)
{
	ULOG_INFO("I am alive at %s \n", get_timestamp(NULL));
	
	uloop_timeout_set(timeout, HEARTBEAT_PERIOD);
}
static struct uloop_timeout heartbeat_timer = {
	.cb = heartbeat_cb
};

int main(int argc, char *argv[])
{
	int ret = 0;
	(void)ret;
	
	uloop_init();
	
	signal(SIGPIPE, SIG_IGN);
	signal(SIGTERM, signal_shutdown);
	signal(SIGKILL, signal_shutdown);
	
	ubus_startup();
	
	uloop_timeout_set(&heartbeat_timer, HEARTBEAT_PERIOD);
	uloop_run();
	uloop_done();
	
	return 0;
}

