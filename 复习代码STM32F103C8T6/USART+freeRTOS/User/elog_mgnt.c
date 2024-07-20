#include <stdbool.h>
#include <stdint.h>
#include "elog.h"
#include "elog_cfg.h"
#include "Led.h"
#include "Delay.h"

ElogErrCode elog_port_init(void);
void test_elog(void);
void my_elog_init(void)
{
	ElogErrCode elog_port_init();
	LED_Init();
	/* initialize EasyLogger */
	elog_init();
	/* set EasyLogger log format */
	elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL);
	elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
	elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
	elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
	elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL & ~ELOG_FMT_FUNC);
	elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_ALL & ~ELOG_FMT_FUNC);
	/* start EasyLogger */
	
	elog_start();
	
//	test_elog();
//	
//    LED_ON();
//    Delay_s(1);
//    LED_OFF();
//    Delay_s(1);
}

void test_elog(void) {
    /* test log output for all level */
    log_a("Hello EasyLogger!");
    log_e("Hello EasyLogger!");
    log_w("Hello EasyLogger!");
    log_i("Hello EasyLogger!");
    log_d("Hello EasyLogger!");
    log_v("Hello EasyLogger!");
//    elog_raw("Hello EasyLogger!");
}
