/*
**********************************************************************************************************************
*											        eGon
*						                     the Embedded System
*									       script parser sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : script.c
*
* By      : Jerry
*
* Version : V2.00
*
* Date	  :
*
* Descript:
**********************************************************************************************************************
*/
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/init.h>

#include <mach/script_v2.h>
#include <mach/script_i.h>

static  char  *script_mod_buf = NULL;           //ָ���һ������
static  int    script_main_key_count = 0;       //���������ĸ���

/*
************************************************************************************************************
*
*                                             _test_str_length
*
*    �������ƣ����Դ������ַ����ĳ���
*
*    �����б�str
*
*    ����ֵ  ��
*
*    ˵��    ��
*
*
************************************************************************************************************
*/
static  int   _test_str_length(char *str)
{
	int length = 0;

	while(str[length++])
	{
		if(length > 32)
		{
			length = 32;
			break;
		}
	}

	return length;
}
/*
************************************************************************************************************
*
*                                             script_parser_init
*
*    �������ƣ�
*
*    �����б�script_buf: �ű����ݳ�
*
*    ����ֵ  ��
*
*    ˵��    ��
*
*
************************************************************************************************************
*/
int script_parser_init(char *script_buf)
{
	script_head_t   *script_head;

	pr_debug("%s(%d)-%s, script_buf addr is %p:\n",__FILE__,__LINE__,__FUNCTION__, script_buf);
	if(script_buf)
	{
		script_mod_buf = script_buf;
		script_head = (script_head_t *)script_mod_buf;

		script_main_key_count = script_head->main_key_count;

		pr_debug("succeed: %s(%d)-%s\n",__FILE__,__LINE__,__FUNCTION__);
		return SCRIPT_PARSER_OK;
	}
	else
	{
		pr_warning("failed: %s(%d)-%s\n",__FILE__,__LINE__,__FUNCTION__);
		return SCRIPT_PARSER_EMPTY_BUFFER;
	}
}
/*
************************************************************************************************************
*
*                                             script_parser_exit
*
*    �������ƣ�
*
*    �����б�NULL
*
*    ����ֵ  ��
*
*    ˵��    ��
*
*
************************************************************************************************************
*/
int script_parser_exit(void)
{
	script_mod_buf = NULL;
	script_main_key_count = 0;

	return SCRIPT_PARSER_OK;
}

/*
************************************************************************************************************
*
*                                             script_parser_fetch
*
*    �������ƣ�
*
*    �����б�
*
*    ����ֵ  ��
*
*    ˵��    �����ݴ������������Ӽ���ȡ�ö�Ӧ����ֵ
*
*
************************************************************************************************************
*/
int script_parser_fetch(char *main_name, char *sub_name, int value[], int count)
{
	char   main_bkname[32], sub_bkname[32];
	char   *main_char, *sub_char;
	script_main_key_t  *main_key = NULL;
	script_sub_key_t   *sub_key = NULL;
	int    i, j;
	int    pattern, word_count;

	pr_debug("enter script parse fetch. \n");
	//���ű�buffer�Ƿ����
	if(!script_mod_buf)
	{
		return SCRIPT_PARSER_EMPTY_BUFFER;
	}
	//����������ƺ��Ӽ������Ƿ�Ϊ��
	if((main_name == NULL) || (sub_name == NULL))
	{
		return SCRIPT_PARSER_KEYNAME_NULL;
	}
	//�������buffer�Ƿ�Ϊ��
	if(value == NULL)
	{
		return SCRIPT_PARSER_DATA_VALUE_NULL;
	}
	//�����������ƺ��Ӽ����ƣ��������31�ֽ����ȡ31�ֽ�
	main_char = main_name;
	if(_test_str_length(main_name) > 31)
	{
		memset(main_bkname, 0, 32);
		strncpy(main_bkname, main_name, 31);
		main_char = main_bkname;
	}
	sub_char = sub_name;
	if(_test_str_length(sub_name) > 31)
	{
		memset(sub_bkname, 0, 32);
		strncpy(sub_bkname, sub_name, 31);
		sub_char = sub_bkname;
	}
	pr_debug("gpio: main name is : %s, sub_name is: %s", main_char, sub_char);
	
	for(i=0;i<script_main_key_count;i++)
	{
		main_key = (script_main_key_t *)(script_mod_buf + (sizeof(script_head_t)) + i * sizeof(script_main_key_t));
		if(strcmp(main_key->main_name, main_char))    //���������ƥ�䣬Ѱ����һ������
		{
			continue;
		}
		//����ƥ�䣬Ѱ���Ӽ�����ƥ��
		for(j=0;j<main_key->lenth;j++)
		{
			sub_key = (script_sub_key_t *)(script_mod_buf + (main_key->offset<<2) + (j * sizeof(script_sub_key_t)));
			if(strcmp(sub_key->sub_name, sub_char))    //���������ƥ�䣬Ѱ����һ������
			{
				continue;
			}
			pattern    = (sub_key->pattern>>16) & 0xffff;             //��ȡ���ݵ�����
			word_count = (sub_key->pattern>> 0) & 0xffff;             //��ȡ��ռ�õ�word����
			pr_debug("pattern is: 0x%x, word_count is: 0x%x, ", pattern, word_count);
			//ȡ������
			switch(pattern)
			{
				case SCIRPT_PARSER_VALUE_TYPE_SINGLE_WORD:            //��word��������
					value[0] = *(int *)(script_mod_buf + (sub_key->offset<<2));
					break;

				case SCIRPT_PARSER_VALUE_TYPE_STRING:     			  //�ַ�����������
					if(count < word_count)
					{
						word_count = count;
					}
					memcpy((char *)value, script_mod_buf + (sub_key->offset<<2), word_count << 2);
					break;

				case SCIRPT_PARSER_VALUE_TYPE_MULTI_WORD:
					break;
				case SCIRPT_PARSER_VALUE_TYPE_GPIO_WORD:			   //��word��������
				{
					script_gpio_set_t  *user_gpio_cfg = (script_gpio_set_t *)value;
                    //������GPIO���ͣ�����Ƿ��㹻����û�����
                    if(sizeof(script_gpio_set_t) > (count<<2))
                    {
                        return SCRIPT_PARSER_BUFFER_NOT_ENOUGH;
                    }
					strcpy( user_gpio_cfg->gpio_name, sub_char);
					memcpy(&user_gpio_cfg->port, script_mod_buf + (sub_key->offset<<2),  sizeof(script_gpio_set_t) - 32);
					break;
			    }
			}

			return SCRIPT_PARSER_OK;
		}
	}

	return SCRIPT_PARSER_KEY_NOT_FIND;
}
EXPORT_SYMBOL(script_parser_fetch);
/*
************************************************************************************************************
*
*                                             script_parser_fetch_ex
*
*    �������ƣ�
*
*    �����б�
*
*    ����ֵ  ��
*
*    ˵��    �����ݴ������������Ӽ���ȡ�ö�Ӧ����ֵ�Լ���ֵ����
*
*
************************************************************************************************************
*/
int script_parser_fetch_ex(char *main_name, char *sub_name, int value[], script_parser_value_type_t *type, int count)
{
	char   main_bkname[32], sub_bkname[32];
	char   *main_char, *sub_char;
	script_main_key_t  *main_key = NULL;
	script_sub_key_t   *sub_key = NULL;
	int    i, j;
	int    pattern, word_count;
	script_parser_value_type_t *value_type = type;

	pr_debug("enter script parse fetch. \n");
	//���ű�buffer�Ƿ����
	if(!script_mod_buf)
	{
		return SCRIPT_PARSER_EMPTY_BUFFER;
	}
	//����������ƺ��Ӽ������Ƿ�Ϊ��
	if((main_name == NULL) || (sub_name == NULL))
	{
		return SCRIPT_PARSER_KEYNAME_NULL;
	}
	//�������buffer�Ƿ�Ϊ��
	if(value == NULL)
	{
		return SCRIPT_PARSER_DATA_VALUE_NULL;
	}
	//�����������ƺ��Ӽ����ƣ��������31�ֽ����ȡ31�ֽ�
	main_char = main_name;
	if(_test_str_length(main_name) > 31)
	{
		memset(main_bkname, 0, 32);
		strncpy(main_bkname, main_name, 31);
		main_char = main_bkname;
	}
	sub_char = sub_name;
	if(_test_str_length(sub_name) > 31)
	{
		memset(sub_bkname, 0, 32);
		strncpy(sub_bkname, sub_name, 31);
		sub_char = sub_bkname;
	}
	pr_debug("gpio: main name is : %s, sub_name is: %s", main_char, sub_char);
	
	for(i=0;i<script_main_key_count;i++)
	{
		main_key = (script_main_key_t *)(script_mod_buf + (sizeof(script_head_t)) + i * sizeof(script_main_key_t));
		if(strcmp(main_key->main_name, main_char))    //���������ƥ�䣬Ѱ����һ������
		{
			continue;
		}
		//����ƥ�䣬Ѱ���Ӽ�����ƥ��
		for(j=0;j<main_key->lenth;j++)
		{
			sub_key = (script_sub_key_t *)(script_mod_buf + (main_key->offset<<2) + (j * sizeof(script_sub_key_t)));
			if(strcmp(sub_key->sub_name, sub_char))    //���������ƥ�䣬Ѱ����һ������
			{
				continue;
			}
			pattern    = (sub_key->pattern>>16) & 0xffff;             //��ȡ���ݵ�����
			word_count = (sub_key->pattern>> 0) & 0xffff;             //��ȡ��ռ�õ�word����
			pr_debug("pattern is: 0x%x, word_count is: 0x%x, ", pattern, word_count);
			//ȡ������
			switch(pattern)
			{
				case SCIRPT_PARSER_VALUE_TYPE_SINGLE_WORD:            //��word��������
					value[0] = *(int *)(script_mod_buf + (sub_key->offset<<2));
					*value_type = SCIRPT_PARSER_VALUE_TYPE_SINGLE_WORD;
					break;

				case SCIRPT_PARSER_VALUE_TYPE_STRING:     			  //�ַ�����������
					if(count < word_count)
					{
						word_count = count;
					}
					memcpy((char *)value, script_mod_buf + (sub_key->offset<<2), word_count << 2);
					*value_type = SCIRPT_PARSER_VALUE_TYPE_STRING;
					break;

				case SCIRPT_PARSER_VALUE_TYPE_MULTI_WORD:
					*value_type = SCIRPT_PARSER_VALUE_TYPE_MULTI_WORD;
					break;
				case SCIRPT_PARSER_VALUE_TYPE_GPIO_WORD:			   //��word��������
				{
					script_gpio_set_t  *user_gpio_cfg = (script_gpio_set_t *)value;
                    //������GPIO���ͣ�����Ƿ��㹻����û�����
                    if(sizeof(script_gpio_set_t) > (count<<2))
                    {
                        return SCRIPT_PARSER_BUFFER_NOT_ENOUGH;
                    }
					strcpy( user_gpio_cfg->gpio_name, sub_char);
					memcpy(&user_gpio_cfg->port, script_mod_buf + (sub_key->offset<<2),  sizeof(script_gpio_set_t) - 32);
					*value_type = SCIRPT_PARSER_VALUE_TYPE_GPIO_WORD;
					break;
			    }
			}

			return SCRIPT_PARSER_OK;
		}
	}

	return SCRIPT_PARSER_KEY_NOT_FIND;
}
EXPORT_SYMBOL(script_parser_fetch_ex);
/*
************************************************************************************************************
*
*                                             script_parser_subkey_count
*
*    �������ƣ�
*
*    �����б�
*
*    ����ֵ  ��
*
*    ˵��    �����ݴ�����������ȡ�������µ��Ӽ�����
*
*
************************************************************************************************************
*/
int script_parser_subkey_count(char *main_name)
{
	char   main_bkname[32];
	char   *main_char;
	script_main_key_t  *main_key = NULL;
	int    i;

	//���ű�buffer�Ƿ����
	if(!script_mod_buf)
	{
		return SCRIPT_PARSER_EMPTY_BUFFER;
	}
	//����������ƺ��Ӽ������Ƿ�Ϊ��
	if(main_name == NULL)
	{
		return SCRIPT_PARSER_KEYNAME_NULL;
	}
	//�����������ƺ��Ӽ����ƣ��������31�ֽ����ȡ31�ֽ�
	main_char = main_name;
	if(_test_str_length(main_name) > 31)
	{
	    memset(main_bkname, 0, 32);
		strncpy(main_bkname, main_name, 31);
		main_char = main_bkname;
	}

	for(i=0;i<script_main_key_count;i++)
	{
		main_key = (script_main_key_t *)(script_mod_buf + (sizeof(script_head_t)) + i * sizeof(script_main_key_t));
		if(strcmp(main_key->main_name, main_char))    //���������ƥ�䣬Ѱ����һ������
		{
			continue;
		}

		return main_key->lenth;    //���ص�ǰ�����µ��Ӽ�����
	}

	return -1;                     //-1 ��ʾû�ж�Ӧ������
}
/*
************************************************************************************************************
*
*                                             script_parser_mainkey_count
*
*    �������ƣ�
*
*    �����б�
*
*    ����ֵ  ��
*
*    ˵��    �����ݴ�����������ȡ�������ĸ���
*
*
************************************************************************************************************
*/
int script_parser_mainkey_count(void)
{
	//���ű�buffer�Ƿ����
	if(!script_mod_buf)
	{
		return SCRIPT_PARSER_EMPTY_BUFFER;
	}

	return 	script_main_key_count;
}
/*
************************************************************************************************************
*
*                                             script_parser_mainkey_get_gpio_count
*
*    �������ƣ�
*
*    �����б�
*
*    ����ֵ  ��
*
*    ˵��    �����ݴ�����������ȡ��������GPIO����
*
*
************************************************************************************************************
*/
int script_parser_mainkey_get_gpio_count(char *main_name)
{
	char   main_bkname[32];
	char   *main_char;
	script_main_key_t  *main_key = NULL;
	script_sub_key_t   *sub_key = NULL;
	int    i, j;
	int    pattern, gpio_count = 0;

	//���ű�buffer�Ƿ����
	if(!script_mod_buf)
	{
		return SCRIPT_PARSER_EMPTY_BUFFER;
	}
	//����������ƺ��Ӽ������Ƿ�Ϊ��
	if(main_name == NULL)
	{
		return SCRIPT_PARSER_KEYNAME_NULL;
	}
	//�����������ƺ��Ӽ����ƣ��������31�ֽ����ȡ31�ֽ�
	main_char = main_name;
	if(_test_str_length(main_name) > 31)
	{
	    memset(main_bkname, 0, 32);
		strncpy(main_bkname, main_name, 31);
		main_char = main_bkname;
	}

	for(i=0;i<script_main_key_count;i++)
	{
		main_key = (script_main_key_t *)(script_mod_buf + (sizeof(script_head_t)) + i * sizeof(script_main_key_t));
		if(strcmp(main_key->main_name, main_char))    //���������ƥ�䣬Ѱ����һ������
		{
			continue;
		}
		//����ƥ�䣬Ѱ���Ӽ�����ƥ��
		for(j=0;j<main_key->lenth;j++)
		{
			sub_key = (script_sub_key_t *)(script_mod_buf + (main_key->offset<<2) + (j * sizeof(script_sub_key_t)));

			pattern    = (sub_key->pattern>>16) & 0xffff;             //��ȡ���ݵ�����
			//ȡ������
			if(SCIRPT_PARSER_VALUE_TYPE_GPIO_WORD == pattern)
			{
				gpio_count ++;
			}
		}
	}

	return gpio_count;
}

/*
************************************************************************************************************
*
*                                             script_parser_mainkey_get_gpio_cfg
*
*    �������ƣ�
*
*    �����б�
*
*    ����ֵ  ��
*
*    ˵��    �����ݴ�����������ȡ��������GPIO��������Ϣ
*
*
************************************************************************************************************
*/
int script_parser_mainkey_get_gpio_cfg(char *main_name, void *gpio_cfg, int gpio_count)
{
	char   main_bkname[32];
	char   *main_char;
	script_main_key_t  *main_key = NULL;
	script_sub_key_t   *sub_key = NULL;
	script_gpio_set_t  *user_gpio_cfg = (script_gpio_set_t *)gpio_cfg;
	int    i, j;
	int    pattern, user_index;

	//���ű�buffer�Ƿ����
	if(!script_mod_buf)
	{
		return SCRIPT_PARSER_EMPTY_BUFFER;
	}
	//����������ƺ��Ӽ������Ƿ�Ϊ��
	if(main_name == NULL)
	{
		return SCRIPT_PARSER_KEYNAME_NULL;
	}
	//��������û�buffer
	memset(user_gpio_cfg, 0, sizeof(script_gpio_set_t) * gpio_count);
	//�����������ƺ��Ӽ����ƣ��������31�ֽ����ȡ31�ֽ�
	main_char = main_name;
	if(_test_str_length(main_name) > 31)
	{
	    memset(main_bkname, 0, 32);
		strncpy(main_bkname, main_name, 31);
		main_char = main_bkname;
	}

	for(i=0;i<script_main_key_count;i++)
	{
		main_key = (script_main_key_t *)(script_mod_buf + (sizeof(script_head_t)) + i * sizeof(script_main_key_t));
		if(strcmp(main_key->main_name, main_char))    //���������ƥ�䣬Ѱ����һ������
		{
			continue;
		}
		//����ƥ�䣬Ѱ���Ӽ�����ƥ��
		pr_debug("mainkey name = %s\n", main_key->main_name);
		user_index = 0;
		for(j=0;j<main_key->lenth;j++)
		{
			sub_key = (script_sub_key_t *)(script_mod_buf + (main_key->offset<<2) + (j * sizeof(script_sub_key_t)));
			pr_debug("subkey name = %s\n", sub_key->sub_name);
			pattern    = (sub_key->pattern>>16) & 0xffff;             //��ȡ���ݵ�����
			pr_debug("subkey pattern = %d\n", pattern);
			//ȡ������
			if(SCIRPT_PARSER_VALUE_TYPE_GPIO_WORD == pattern)
			{
			    strcpy( user_gpio_cfg[user_index].gpio_name, sub_key->sub_name);
				memcpy(&user_gpio_cfg[user_index].port, script_mod_buf + (sub_key->offset<<2), sizeof(script_gpio_set_t) - 32);
				user_index++;
				if(user_index >= gpio_count)
				{
					break;
				}
			}
		}
		return SCRIPT_PARSER_OK;
	}

	return SCRIPT_PARSER_KEY_NOT_FIND;
}

