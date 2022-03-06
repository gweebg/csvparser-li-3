#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct dt
{
    char *str_repr;
    int day, month, year;
    int hours, minutes, seconds;

} datetime;

bool equal_date(datetime *my_date, datetime *given_date) 
{
    return strcmp(my_date->str_repr, given_date->str_repr) ? true : false;
}

int compare_datetimes(datetime *my_date, datetime *to_compare)
{
    /*
        -1 : my_date < to_compare
        0  : equal dates, with equal times
        1  : my_date > to_compare
    */

    if (strcmp(my_date->str_repr, to_compare->str_repr) == 0) return 0; // Equal datetimes.

    int my_day  = my_date->day,
        my_mon  = my_date->month,
        my_year = my_date->year;

    int to_day  = to_compare->day,
        to_mon  = to_compare->month,
        to_year = to_compare->year;

    if (my_year > to_year) return 1; // my_date > to_compare
    if (my_year < to_year) return -1; // my_date < to_compare

    // At this point years are the same.
    if (my_mon > to_mon) return 1; // my_date > to_compare
    if (my_mon < to_mon) return -1; // my_date < to_compare

    // Here the months are the same.
    if (my_day > to_day) return 1; // my_date > to_compare
    if (my_day < to_day) return -1; // my_date < to_compare

    // Here the days are the same, so we need to compare dates.
    int my_hour = my_date->hours,
        my_mins = my_date->minutes,
        my_secs = my_date->seconds;

    int to_hour = to_compare->hours,
        to_mins = to_compare->minutes,
        to_secs = to_compare->seconds;

    if (my_hour > to_hour) return 1;
    if (my_hour < to_hour) return -1;

    // Hours are the same.
    if (my_mins > to_mins) return 1;
    if (my_mins < to_mins) return -1;

    // Minutes are the same.
    if (my_secs > to_secs) return 1;
    if (my_secs < to_secs) return -1;

}

int lower_bound_date_index(datetime **commits, datetime *date, int size)
{
    int index = -1, l = 0;

    while (l <= size)
    {
        int mid = (l + size) / 2;
        int comp = compare_datetimes(date, commits[mid]);

        if (comp == 0)
        {
            index = mid;
            break;
        }
        else if (comp < 0)
        {
            size = mid - 1;
            index = mid;
        }
        else
        {
            l = mid + 1;
        }
    }

    printf("%d\n", index);
    return index;    
}


int main()
{

    datetime *a = malloc(sizeof(datetime)),
             *b = malloc(sizeof(datetime)),
             *c = malloc(sizeof(datetime)),
             *d = malloc(sizeof(datetime)),
             *e = malloc(sizeof(datetime));
    
    a->str_repr = "2013-09-21 11:44:46";
    a->day = 21;
    a->month = 9;
    a->year = 2013;
    a->hours = 11;
    a->minutes = 44;
    a->seconds = 46;

    b->str_repr = "2015-03-15 18:45:16";
    b->day = 15;
    b->month = 3;
    b->year = 2015;
    b->hours = 18;
    b->minutes = 45;
    b->seconds = 16;

    c->str_repr = "2018-02-13 17:41:47";
    c->day = 13;
    c->month = 2;
    c->year = 2018;
    c->hours = 17;
    c->minutes = 41;
    c->seconds = 47;

    d->str_repr = "2013-11-01 02:28:30";
    d->day = 1;
    d->month = 11;
    d->year = 2013;
    d->hours = 2;
    d->minutes = 28;
    d->seconds = 30;

    e->str_repr = "2018-02-13 17:41:47";
    e->day = 13;
    e->month = 2;
    e->year = 2018;
    e->hours = 17;
    e->minutes = 41;
    e->seconds = 47;

    int ab = compare_datetimes(a, b); // a < b => -1
    int bc = compare_datetimes(b, c); // b < c => -1
    int cd = compare_datetimes(c, d); // c > d => 1
    int de = compare_datetimes(d, e); // d < e => -1
    int ce = compare_datetimes(c, e); // c == e => 0

    printf("ab : %d\nbc : %d\ncd : %d\nde : %d\nce : %d\n",
            ab, bc, cd, de, ce);

    datetime* dates[] = {a, d, b, c, e};


    datetime *f = malloc(sizeof(*f));
    f->str_repr = "2016-02-13 17:41:47";
    f->day = 13;
    f->month = 2;
    f->year = 2016;
    f->hours = 17;
    f->minutes = 41;
    f->seconds = 47;

    int x = lower_bound_date_index(dates, f, 4);
    return 1;
}
