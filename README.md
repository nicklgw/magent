# magent



---

利用uloop事件处理mosquitto，使得整个程序只要单线程就可完成业务

```shell
$ cd package/leedarson
$ git clone https://github.com/nicklgw/magent.git magent
```

参考 ubusd client_cb
ubusd.c 
	-> static void client_cb(struct uloop_fd *sock, unsigned int events)

