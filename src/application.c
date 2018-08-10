
#include "application.h"

static struct application the_app = { 
	
	.enauth = 0,
	
};

struct application* get_app(void)
{	
	return &the_app;
}

