#include "product.h"
#include "key.h"
#include "timer.h"
#include "wdt.h"
#include "uart.h"
#include "adc.h"


static void product_GPIO_Init_Config ( void )
{
	P2M0 = 0x61;                        //P35设置为非施密特数字带上拉输入
	P2M2 = 0XC1;                        //设置为推挽输出
	PRODUCT_OK = 1;
}

static enum producte_test_list product_check_test ( void )
{
	u8 ret = 0;
	product_GPIO_Init_Config();
	if ( PRODUCT_CHECK_IN == 0 )
	{
		ret = CHECK_SUCCESS;
	}
	else
	{
		ret = CHECK_FAIL;
	}
	return ret;
}


u8 Res_Calc ( void )
{
	u16 adc_val1 = 0,adc_val3 = 0;
	float u1 = 0;
	get_voltage ( &adc_val1,&adc_val3 );
	u1 = (  ( ( float ) adc_val3 /1000 ) - ( ( float ) adc_val1/1000 ) ) / ( ( ( float ) adc_val1/1000 ) /510 );
	//gm_printf ( "R = %f  \r\n",u1 );
	if ( ( u1 < 800 ) && ( u1 > 700 ) )
	{
		return RES_OK;
	}
	return 0;
}
u8 state = PRODUCTE_INIT;
u16 test_time_timeout = 0;

void product_test_handle ( void )
{

	switch ( state )
	{
		case PRODUCTE_INIT:

			if ( ( KEY_1==0 ) && ( KEY_1==0 ) && ( KEY_1==0 ) )
			{
				state = PRODUCTE_PASS;
				test_time_timeout = 0;
			}
			if ( ++test_time_timeout >= 10000 )
			{
				//KEY_printf ( " PRODUCTE_ERROR\r\n" );
				state = PRODUCTE_ERROR;
				test_time_timeout = 0;
			}

			break;
		case PRODUCTE_TEST:
			if ( Res_Calc() == RES_OK )
			{
				state = PRODUCTE_PASS;
				test_time_timeout = 0;
			}
			if ( ++test_time_timeout >= 5 )
			{
				//KEY_printf ( " PRODUCTE_ERROR\r\n" );
				state = PRODUCTE_ERROR;
				test_time_timeout = 0;
			}
			break;
		case PRODUCTE_PASS:
			LED_BACK = 1;
			PRODUCT_OK = 0;
			
			break;

		case PRODUCTE_ERROR:
			
			PRODUCT_OK = 1;
			break;

	}
}



void product_check_process ( void )
{
	if ( product_check_test() == CHECK_SUCCESS )
	{

		//KEY_printf ( " ENTER_TEST_MODE\r\n" );
		LED_BACK = 1;

		while ( 1 )
		{
			product_test_handle();
			clear_wdt();
		}
	}
}

