#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/time.h>

MODULE_AUTHOR("Cyri");
MODULE_DESCRIPTION("Calculate percentage of interval between New Year and Chinese New Year");

static int __init time_interval_module_init(void) {
    struct timespec64 now;
    struct timespec64 new_year;
    struct timespec64 chinese_new_year;

    // Установим дату Нового года (1 января 2025 года)
    new_year.tv_sec = mktime64(2025 - 1970, 1, 1, 0, 0, 0);
    new_year.tv_nsec = 0;

    // Установим дату Китайского Нового года (10 февраля 2025 года)
    chinese_new_year.tv_sec = mktime64(2025 - 1970, 2, 10, 0, 0, 0);
    chinese_new_year.tv_nsec = 0;

    // Получаем текущее время
    ktime_get_real_ts64(&now);

    // Проверяем, попадает ли текущее время в интервал
    if (now.tv_sec < new_year.tv_sec || now.tv_sec > chinese_new_year.tv_sec) {
        pr_info("Current time is out of the range between New Year and Chinese New Year.\n");
        return 0;
    }

    // Вычисляем долю интервала
    long total_interval = chinese_new_year.tv_sec - new_year.tv_sec;
    long elapsed_time = now.tv_sec - new_year.tv_sec;
    long percentage = (elapsed_time * 100) / total_interval;

    pr_info("%ld%% of the interval between New Year and Chinese New Year has passed.\n", percentage);

    return 0;
}

static void __exit time_interval_module_exit(void) {
    pr_info("Exiting time interval module.\n");
}

module_init(time_interval_module_init);
module_exit(time_interval_module_exit);
MODULE_LICENSE("GPL");
