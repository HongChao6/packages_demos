#include "./lv_i18n.h"


////////////////////////////////////////////////////////////////////////////////
// Define plural operands
// http://unicode.org/reports/tr35/tr35-numbers.html#Operands

// Integer version, simplified

#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif

static inline uint32_t op_n(int32_t val) { return (uint32_t)(val < 0 ? -val : val); }
static inline uint32_t op_i(uint32_t val) { return val; }
// always zero, when decimal part not exists.
static inline uint32_t op_v(uint32_t val) { UNUSED(val); return 0;}
static inline uint32_t op_w(uint32_t val) { UNUSED(val); return 0; }
static inline uint32_t op_f(uint32_t val) { UNUSED(val); return 0; }
static inline uint32_t op_t(uint32_t val) { UNUSED(val); return 0; }

static lv_i18n_phrase_t en_us_singulars[] = {
    {"EPHEMERIS", "Ephemeris"},
    {"LOAD_SUCCESS", "Load Success"},
    {"LOW_BATTERY", "Low Battery"},
    {"SHUTDOWN?", "Shutdown?"},
    {"NO_OPERATION", "No Operation"},
    {"NO", "No"},
    {"YES", "Yes"},
    {"START_RECORD", "Start Record"},
    {"STOP_RECORD", "Stop Record"},
    {"GNSS_NOT_READY", "GNSS Not Ready"},
    {"OPEN_FILE_FAILED", "Open File Failed"},
    {"AVG_SPEED", "AVG"},
    {"TIME", "Time"},
    {"DISTANCE", "Distance"},
    {"CALORIES", "Calories"},
    {"LOADING...", "Loading..."},
    {"POWER_OFF", "Power Off"},
    {"SPORT", "Sport"},
    {"GNSS", "GNSS"},
    {"BATTERY", "Battery"},
    {"ABOUT", "About"},
    {"TOTAL_TRIP", "Total Trip"},
    {"TOTAL_TIME", "Total Time"},
    {"MAX_SPEED", "Max Speed"},
    {"LATITUDE", "Latitude"},
    {"LONGITUDE", "Longitude"},
    {"ALTITUDE", "Altitude"},
    {"UTC_TIME", "UTC Time"},
    {"COURSE", "Course"},
    {"SPEED", "Speed"},
    {"DATE", "Date"},
    {"USAGE", "Usage"},
    {"VOLTAGE", "Voltage"},
    {"STATUS", "Status"},
    {"CHARGE", "Charge"},
    {"DISCHARGE", "Discharge"},
    {"NAME", "Name"},
    {"AUTHOR", "Author"},
    {"GRAPHICS", "GUI"},
    {"COMPILER", "Compiler"},
    {"BUILD_DATE", "Build"},
    {NULL, NULL} // End mark
};



static uint8_t en_us_plural_fn(int32_t num)
{
    uint32_t n = op_n(num); UNUSED(n);
    uint32_t i = op_i(n); UNUSED(i);
    uint32_t v = op_v(n); UNUSED(v);

    if ((i == 1 && v == 0)) return LV_I18N_PLURAL_TYPE_ONE;
    return LV_I18N_PLURAL_TYPE_OTHER;
}

static const lv_i18n_lang_t en_us_lang = {
    .locale_name = "en-US",
    .singulars = en_us_singulars,

    .locale_plural_fn = en_us_plural_fn
};

static lv_i18n_phrase_t zh_cn_singulars[] = {
    {"EPHEMERIS", "星历"},
    {"LOAD_SUCCESS", "加载成功"},
    {"LOW_BATTERY", "低电量"},
    {"SHUTDOWN?", "关闭？"},
    {"NO_OPERATION", "无操作"},
    {"NO", "否"},
    {"YES", "是"},
    {"START_RECORD", "开始记录"},
    {"STOP_RECORD", "停止记录"},
    {"GNSS_NOT_READY", "GNSS 未就绪"},
    {"OPEN_FILE_FAILED", "打开文件失败"},
    {"AVG_SPEED", "平均速度"},
    {"TIME", "时间"},
    {"DISTANCE", "距离"},
    {"CALORIES", "卡路里"},
    {"LOADING...", "加载中..."},
    {"POWER_OFF", "关机"},
    {"SPORT", "运动"},
    {"GNSS", "卫星"},
    {"BATTERY", "电池"},
    {"ABOUT", "关于"},
    {"TOTAL_TRIP", "总里程"},
    {"TOTAL_TIME", "总时间"},
    {"MAX_SPEED", "最高速度"},
    {"LATITUDE", "纬度"},
    {"LONGITUDE", "经度"},
    {"ALTITUDE", "海拔"},
    {"UTC_TIME", "UTC 时间"},
    {"COURSE", "航向"},
    {"SPEED", "速度"},
    {"DATE", "日期"},
    {"USAGE", "使用量"},
    {"VOLTAGE", "电压"},
    {"STATUS", "状态"},
    {"CHARGE", "充电"},
    {"DISCHARGE", "放电"},
    {"NAME", "名称"},
    {"AUTHOR", "作者"},
    {"GRAPHICS", "图形"},
    {"COMPILER", "编译器"},
    {"BUILD_DATE", "编译日期"},
    {NULL, NULL} // End mark
};



static uint8_t zh_cn_plural_fn(int32_t num)
{



    return LV_I18N_PLURAL_TYPE_OTHER;
}

static const lv_i18n_lang_t zh_cn_lang = {
    .locale_name = "zh-CN",
    .singulars = zh_cn_singulars,

    .locale_plural_fn = zh_cn_plural_fn
};

const lv_i18n_language_pack_t lv_i18n_language_pack[] = {
    &en_us_lang,
    &zh_cn_lang,
    NULL // End mark
};

////////////////////////////////////////////////////////////////////////////////


// Internal state
static const lv_i18n_language_pack_t * current_lang_pack;
static const lv_i18n_lang_t * current_lang;


/**
 * Reset internal state. For testing.
 */
void __lv_i18n_reset(void)
{
    current_lang_pack = NULL;
    current_lang = NULL;
}

/**
 * Set the languages for internationalization
 * @param langs pointer to the array of languages. (Last element has to be `NULL`)
 */
int lv_i18n_init(const lv_i18n_language_pack_t * langs)
{
    if(langs == NULL) return -1;
    if(langs[0] == NULL) return -1;

    current_lang_pack = langs;
    current_lang = langs[0];     /*Automatically select the first language*/
    return 0;
}

/**
 * Change the localization (language)
 * @param l_name name of the translation locale to use. E.g. "en-GB"
 */
int lv_i18n_set_locale(const char * l_name)
{
    if(current_lang_pack == NULL) return -1;

    uint16_t i;

    for(i = 0; current_lang_pack[i] != NULL; i++) {
        // Found -> finish
        if(strcmp(current_lang_pack[i]->locale_name, l_name) == 0) {
            current_lang = current_lang_pack[i];
            return 0;
        }
    }

    return -1;
}


static const char * __lv_i18n_get_text_core(lv_i18n_phrase_t * trans, const char * msg_id)
{
    uint16_t i;
    for(i = 0; trans[i].msg_id != NULL; i++) {
        if(strcmp(trans[i].msg_id, msg_id) == 0) {
            /*The msg_id has found. Check the translation*/
            if(trans[i].translation) return trans[i].translation;
        }
    }

    return NULL;
}


/**
 * Get the translation from a message ID
 * @param msg_id message ID
 * @return the translation of `msg_id` on the set local
 */
const char * lv_i18n_get_text(const char * msg_id)
{
    if(current_lang == NULL) return msg_id;

    const lv_i18n_lang_t * lang = current_lang;
    const void * txt;

    // Search in current locale
    if(lang->singulars != NULL) {
        txt = __lv_i18n_get_text_core(lang->singulars, msg_id);
        if (txt != NULL) return txt;
    }

    // Try to fallback
    if(lang == current_lang_pack[0]) return msg_id;
    lang = current_lang_pack[0];

    // Repeat search for default locale
    if(lang->singulars != NULL) {
        txt = __lv_i18n_get_text_core(lang->singulars, msg_id);
        if (txt != NULL) return txt;
    }

    return msg_id;
}

/**
 * Get the translation from a message ID and apply the language's plural rule to get correct form
 * @param msg_id message ID
 * @param num an integer to select the correct plural form
 * @return the translation of `msg_id` on the set local
 */
const char * lv_i18n_get_text_plural(const char * msg_id, int32_t num)
{
    if(current_lang == NULL) return msg_id;

    const lv_i18n_lang_t * lang = current_lang;
    const void * txt;
    lv_i18n_plural_type_t ptype;

    // Search in current locale
    if(lang->locale_plural_fn != NULL) {
        ptype = lang->locale_plural_fn(num);

        if(lang->plurals[ptype] != NULL) {
            txt = __lv_i18n_get_text_core(lang->plurals[ptype], msg_id);
            if (txt != NULL) return txt;
        }
    }

    // Try to fallback
    if(lang == current_lang_pack[0]) return msg_id;
    lang = current_lang_pack[0];

    // Repeat search for default locale
    if(lang->locale_plural_fn != NULL) {
        ptype = lang->locale_plural_fn(num);

        if(lang->plurals[ptype] != NULL) {
            txt = __lv_i18n_get_text_core(lang->plurals[ptype], msg_id);
            if (txt != NULL) return txt;
        }
    }

    return msg_id;
}

/**
 * Get the name of the currently used locale.
 * @return name of the currently used locale. E.g. "en-GB"
 */
const char * lv_i18n_get_current_locale(void)
{
    if(!current_lang) return NULL;
    return current_lang->locale_name;
}
