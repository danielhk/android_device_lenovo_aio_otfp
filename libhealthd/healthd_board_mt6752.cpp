/*
 * Copyright (C) 2016 The Android Open Source Project
 * Copyright (C) 2016 The CyanogenMod Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <cutils/klog.h>
#include <healthd/healthd.h>
#include "minui/minui.h"

#define BACKLIGHT_ON_LEVEL	50
#define BACKLIGHT_OFF_LEVEL	0
#define LCD_BACKLIGHT_PATH	"/sys/class/leds/lcd-backlight/brightness"

#define BUF_LEN	10

#define CHARGING_STATUS_PATH	"/sys/class/power_supply/battery/status"

void
healthd_board_init(struct healthd_config *config)
{
    config->batteryStatusPath      = android::String8("/sys/class/power_supply/battery/status");
    config->batteryHealthPath      = android::String8("/sys/class/power_supply/battery/health");
    config->batteryPresentPath     = android::String8("/sys/class/power_supply/battery/present");
    config->batteryCapacityPath    = android::String8("/sys/class/power_supply/battery/capacity");
    config->batteryVoltagePath     = android::String8("/sys/class/power_supply/battery/batt_vol");
    config->batteryTemperaturePath = android::String8("/sys/class/power_supply/battery/batt_temp");
    config->batteryTechnologyPath  = android::String8("/sys/class/power_supply/battery/technology");
    config->batteryCurrentAvgPath  = android::String8("/sys/class/power_supply/battery/BatteryAverageCurrent");
    // others use defaults
}

int
healthd_board_battery_update(struct android::BatteryProperties *props)
{
    // return 0 to log periodic polled battery status to kernel log
    return 0;
}

void healthd_board_mode_charger_draw_battery(struct android::BatteryProperties *batt_prop)
{
    char buf[STRLEN];
    int x, y;
    int str_len_px;
    static int char_height = -1, char_width = -1;

    if (char_height == -1 && char_width == -1)
        gr_font_size(&char_width, &char_height);
    snprintf(buf, (STRLEN - 1), "%d%%", batt_prop->batteryLevel);
    str_len_px = gr_measure(buf);
    x = (gr_fb_width() - str_len_px) / 2;
    y = (gr_fb_height() + char_height) / 1.5;
    gr_color(0xa4, 0xc6, 0x39, 255);
    gr_text(x, y, buf, 0);
}

void healthd_board_mode_charger_battery_update(struct android::BatteryProperties*) {}

void healthd_board_mode_charger_set_backlight(bool on)
{
    int fd;
    char buf[BUF_LEN];

    if (access(LCD_BACKLIGHT_PATH, R_OK | W_OK) != 0)
    {
        LOGW("Backlight control not support\n");
        return;
    }

    memset(buf, '\0', BUF_LEN);
    fd = open(LCD_BACKLIGHT_PATH, O_RDWR);
    if (fd < 0) {
        LOGE("Could not open backlight node : %s\n", strerror(errno));
        goto cleanup;
    }
    LOGV("set backlight status to %d\n", on);
    if (on)
        snprintf(buf, BUF_LEN, "%d\n", BACKLIGHT_ON_LEVEL);
    else
        snprintf(buf, BUF_LEN, "%d\n", BACKLIGHT_OFF_LEVEL);

    if (write(fd, buf,BUF_LEN) < 0) {
        LOGE("Could not write to backlight node : %s\n", strerror(errno));
        goto cleanup;
    }
cleanup:
    if (fd >= 0)
        close(fd);
}

void healthd_board_mode_charger_init()
{
    int ret;
    char buf[BUF_LEN] = "\0";
    int charging = 0;
    int fd;

    /* check the charging is enabled or not */
    fd = open(CHARGING_STATUS_PATH, O_RDONLY);
    if (fd < 0)
        return;
    ret = read(fd, buf, BUF_LEN);
    close(fd);
    if (ret > 0 && stricmp(buf, "Charging")) {
        /* if not charging, reboot and exit power off charging */
        LOGW("android charging is disabled, exit!\n");
        android_reboot(ANDROID_RB_RESTART, 0, 0);
    }
}
