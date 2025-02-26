/*
 * MIT License
 * Copyright (c) 2022 XCLZ STUDIO@W-Mai
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "ResourcePool.h"
#include "Config/Config.h"
#include "Frameworks/ResourceManager/ResourceManagerStatic.cpp"
#include "ResourcePoolDefs.h"
#include "Utils/lv_msg/lv_msg.h"
#include <uikit/uikit.h>
#include <cstdlib>

/* clang-format off */

#define FONT_REGULAR_NAME                   "Alibaba_PuHuiTi_2.0_55_Regular"
#define FONT_MEDIUM_NAME                    "Alibaba_PuHuiTi_2.0_65_Medium"
#define FONT_BOLD_NAME                      "Alibaba_PuHuiTi_2.0_115_Black"
#define FONT_AWESOME_BRANDS_REGULAR_NAME    "Font Awesome 6 Brands-Regular-400"
#define FONT_AWESOME_FREE_REGULAR_NAME      "Font Awesome 6 Free-Regular-400"
#define FONT_AWESOME_FREE_SOLID_NAME        "Font Awesome 6 Free-Solid-900"

/* clang-format on */

namespace ResourcePool {

DEF_RES_MNGR_OBJ_EXT(Font, font_value_t,
    IMPORT_FONT_NATIVE(montserrat, 14),
    IMPORT_FONT_FILE("regular", FONT_REGULAR_NAME),
    IMPORT_FONT_FILE("medium", FONT_MEDIUM_NAME),
    IMPORT_FONT_FILE("bold", FONT_BOLD_NAME),
    IMPORT_FONT_FILE("awesome", FONT_AWESOME_FREE_SOLID_NAME), );

DEF_RES_MNGR_OBJ(Image,
    IMPORT_IMAGE_FILE("navi_arrow_dark"),
    IMPORT_IMAGE_FILE("navi_arrow_light"), );

void init()
{
    lv_msg_init();
    vg_init();

#define FONT_MANAGER_ADD_FONT(name) vg_font_add_path(name, FONT_MAKE_PATH(name));

    FONT_MANAGER_ADD_FONT(FONT_REGULAR_NAME);
    FONT_MANAGER_ADD_FONT(FONT_MEDIUM_NAME);
    FONT_MANAGER_ADD_FONT(FONT_BOLD_NAME);
    FONT_MANAGER_ADD_FONT(FONT_AWESOME_BRANDS_REGULAR_NAME);
    FONT_MANAGER_ADD_FONT(FONT_AWESOME_FREE_REGULAR_NAME);
    FONT_MANAGER_ADD_FONT(FONT_AWESOME_FREE_SOLID_NAME);

    font_value_t default_font = {
        font_value_type::UNKNOWN,
        LV_FONT_DEFAULT
    };
    SET_DEFAULT(Font, std::forward<font_value_t>(default_font));
    SET_DEFAULT(Image, LV_SYMBOL_WARNING);
}

void deinit()
{
}

/**
 * @brief Parse real font name and font size from input name.
 * @param name Font name
 * @param size Return font size
 * @return Pointer of real NAME start
 */
static const char* get_font_size_from_name(const char* name, int* size)
{
    if (name[0] != '<' || size == nullptr) {
        return name;
    }
    LV_ASSERT_NULL(size);

    /* +1 to skip '<' */
    char* end_ptr;
    *size = (int)strtol(name + 1, &end_ptr, 10);
    if (end_ptr == name) {
        return name;
    }
    return end_ptr + 1; /* +1 to skip '>' */
}

DEF_RES_MNGR_GET(Font)
{
    /* font key: "name_size" */

    auto font = OName(Font).get(key);

    /* native font */
    if (font.type == font_value_type::NATIVE) {
        return font.value;
    }

    int size = -1;
    auto font_key = get_font_size_from_name(key, &size);
    auto freetype_font = OName(Font).get(font_key);

    if (freetype_font.type == font_value_type::UNKNOWN) {
        LV_LOG_ERROR("error key = %s", key);
        return font.value;
    }

    if (size <= 0) {
        LV_LOG_ERROR("error size = %d", size);
        return font.value;
    }

    const lv_font_t* new_font = vg_font_create(
        (const char*)freetype_font.value,
        size,
        LV_FREETYPE_FONT_STYLE_NORMAL);

    if (!new_font) {
        new_font = LV_FONT_DEFAULT;
        LV_LOG_WARN("new_font is NULL, use LV_FONT_DEFAULT");
    }

    return new_font;
}

void dropFont(const lv_font_t* font)
{
    if (font == LV_FONT_DEFAULT) {
        return;
    }

    vg_font_destroy((lv_font_t*)font);
}

DEF_RES_MNGR_GET(Image)
{
    return OName(Image).get(key);
}
}
