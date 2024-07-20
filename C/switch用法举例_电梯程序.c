# include <stdio.h>

int main(void)
{
	int val;

	printf("请输入到达的楼层: ");
	scanf("%d", &val);

	switch (val)
	{
	case 1:
		printf("1层!\n");
		break;
	case 2:
		printf("2层!\n");
		break;
	case 3:
		printf("3层!\n");
		break;
    default:
		printf("没有这个楼层!\n");
		break;
	}

	return 0;
}