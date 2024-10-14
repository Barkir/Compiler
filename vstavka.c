#include <stdio.h>
#include <stdlib.h>

typedef struct _elem
{
    double a;
    struct elem * next;
} elem

int insert_sort(elem ** lst, double a)
{
    if (!*lst || (*lst)->a > a)
    {
        elem * n = malloc(sizeof(elem));
        *n = (elem){a, *lst};
        *lst = n;
        return 0;
    } else
        return insert_sort(&(*lst)->next, a);

}

int main()
{
    double x;
    elem * list = {};
    while (scanf("%lf", &x) == 1)
        insert_sort(&list, x);
}
