#include <stdio.h>

struct aaa{
    int a;
    int b[5];
};


struct aaa function1(struct aaa a, struct aaa b)
{
    struct aaa c;
    c.a = a.a + b.a;
    for(int i=0; i<5; i++)
	c.b[i] = a.b[i] + b.b[i];
    return c;
}

int main()
{
    struct aaa obj1;
    obj1.a = 5;
    obj1.b[0] = 1;
    obj1.b[1] = 2;
    obj1.b[2] = 3;
    obj1.b[3] = 4;
    obj1.b[4] = 5;

    struct aaa obj2;
    obj2.a = 7;
    obj2.b[0] = 2;
    obj2.b[1] = 2;
    obj2.b[2] = 2;
    obj2.b[3] = 2;
    obj2.b[4] = 2;

    struct aaa obj3 = function1(obj1, obj2);

    printf("obj3:\n");
    printf("a: %d\n", obj3.a);
    printf("b[0]: %d\n", obj3.b[0]);
    printf("b[1]: %d\n", obj3.b[1]);
    printf("b[2]: %d\n", obj3.b[2]);
    printf("b[3]: %d\n", obj3.b[3]);
    printf("b[4]: %d\n", obj3.b[4]);

    printf("\n\n");
    return 0;
}
