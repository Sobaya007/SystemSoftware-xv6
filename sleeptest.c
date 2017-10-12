#include "types.h"
#include "stat.h"
#include "user.h"
#include "date.h"

int is_leap_year(int y) {
    if(y % 4 == 0){
        if(y % 100 == 0){
            if(y % 400 == 0){
                return 1;
            }else{
                return 0;
            }
        }else{
            return 1;
        }
    }else{
        return 0;
    }
}

int calc_max_day(int year, int month) {
    switch (month) {
        case 2:
            if (is_leap_year(year)) return 29;
            return 28;
        case 4:
        case 6:
        case 9:
        case 11:
            return 30;
        default:
            return 31;
    }
}

void add_month(struct rtcdate *d, uint month) {
    d->month += month;
    if (d->month <= 12) return;
    d->month -= 12;
    d->year++;
}

void add_day(struct rtcdate *d, uint day) {
    d->day += day;
    int max_day = calc_max_day(d->year, d->month);
    if (d->day <= max_day) return;
    d->day -= max_day;
    add_month(d, 1);
}

void add_hour(struct rtcdate *d, uint hour) {
    d->hour += hour;
    if (d->hour < 24) return;
    d->hour -= 24;
    add_day(d, 1);
}

void add_minute(struct rtcdate *d, uint minute) {
    d->minute += minute;
    if (d->minute < 60) return;
    d->minute -= 60;
    add_hour(d, 1);
}

void add_second(struct rtcdate *d, uint second) {
    d->second += second;
    if (d->second < 60) return;
    d->second -= 60;
    add_minute(d, 1);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf(2, "usage: %s sec\n", argv[0]);
        exit();
    } 
    struct rtcdate d;
    getdate(&d);
    printf(1, "Current Date %d/%d/%d/%d:%d:%d\n", d.year, d.month, d.day, d.hour, d.minute, d.second);
    add_second(&d, atoi(argv[1])); 
    printf(1, "Expected Arrival Date %d/%d/%d/%d:%d:%d\n", d.year, d.month, d.day, d.hour, d.minute, d.second);
    if (sleep_until(&d) < 0) {
        printf(1, "Error Occured\n");
        return -1;
    }
    getdate(&d);
    printf(1, "Actual Arrival Date %d/%d/%d/%d:%d:%d\n", d.year, d.month, d.day, d.hour, d.minute, d.second);
    exit();
}
