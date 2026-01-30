/**
 * @file lv_types.h
 *
 */

#ifndef LV_TYPES_H
#define LV_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
/*#include "../lv_conf_internal.h"*/
#include "display.h"

/*********************
 *      DEFINES
 *********************/

 
/**********************
 *      TYPEDEFS
 **********************/


/* Exclude C enum and struct definitions when included by assembly code */
#ifndef __ASSEMBLY__

/**
 * LVGL error codes.
 */
typedef enum {
    LV_RESULT_INVALID = 0, /*Typically indicates that the object is deleted (become invalid) in the action
                      function or an operation was failed*/
    LV_RESULT_OK,      /*The object is valid (no deleted) after the action*/
} lv_result_t;

#if defined(__cplusplus) || __STDC_VERSION__ >= 199901L
/*If c99 or newer,  use the definition of uintptr_t directly from <stdint.h>*/
typedef uintptr_t lv_uintptr_t;
typedef intptr_t lv_intptr_t;

#else

/*Otherwise, use the arch size determination*/
#ifdef LV_ARCH_64
typedef uint64_t lv_uintptr_t;
typedef int64_t lv_intptr_t;
#else
typedef uint32_t lv_uintptr_t;
typedef int32_t lv_intptr_t;
#endif

#endif

#if LV_USE_FLOAT
typedef float lv_value_precise_t;
#else
typedef int32_t lv_value_precise_t;
#endif

#if LV_USE_3DTEXTURE
#if LV_USE_OPENGLES
typedef unsigned int lv_3dtexture_id_t;
#endif
#endif

/**
 * Typedefs from various lvgl modules.
 * They are defined here to avoid circular dependencies.
 */

typedef struct _lv_obj_t lv_obj_t;

typedef lv_obj_t * (*lv_screen_create_cb_t)(void);

typedef uint8_t lv_opa_t;

typedef uint8_t lv_style_prop_t;

typedef struct _lv_obj_class_t lv_obj_class_t;

typedef struct _lv_group_t lv_group_t;

typedef struct _lv_display_t lv_display_t;

typedef struct _lv_layer_t lv_layer_t;
typedef struct _lv_draw_unit_t lv_draw_unit_t;
typedef struct _lv_draw_task_t lv_draw_task_t;

typedef struct _lv_indev_t lv_indev_t;

typedef struct _lv_event_t lv_event_t;

typedef struct _lv_timer_t lv_timer_t;

typedef struct _lv_theme_t lv_theme_t;

typedef struct _lv_anim_t lv_anim_t;

typedef struct _lv_anim_timeline_t lv_anim_timeline_t;

typedef struct _lv_font_t lv_font_t;
typedef struct _lv_font_class_t lv_font_class_t;
typedef struct _lv_font_info_t lv_font_info_t;

typedef struct _lv_font_manager_t lv_font_manager_t;

typedef struct _lv_image_decoder_t lv_image_decoder_t;

typedef struct _lv_image_decoder_dsc_t lv_image_decoder_dsc_t;

typedef struct _lv_draw_image_dsc_t lv_draw_image_dsc_t;

typedef struct _lv_fragment_t lv_fragment_t;
typedef struct _lv_fragment_class_t lv_fragment_class_t;
typedef struct _lv_fragment_managed_states_t lv_fragment_managed_states_t;

typedef struct _lv_profiler_builtin_config_t lv_profiler_builtin_config_t;

typedef struct _lv_rb_node_t lv_rb_node_t;

typedef struct _lv_rb_t lv_rb_t;

typedef struct _lv_color_filter_dsc_t lv_color_filter_dsc_t;

typedef struct _lv_event_dsc_t lv_event_dsc_t;

typedef struct _lv_cache_t lv_cache_t;

typedef struct _lv_cache_entry_t lv_cache_entry_t;

typedef struct _lv_fs_file_cache_t lv_fs_file_cache_t;

typedef struct _lv_image_decoder_args_t lv_image_decoder_args_t;

typedef struct _lv_image_cache_data_t lv_image_cache_data_t;

typedef struct _lv_image_header_cache_data_t lv_image_header_cache_data_t;

typedef struct _lv_draw_mask_t lv_draw_mask_t;

typedef struct _lv_draw_label_hint_t lv_draw_label_hint_t;

typedef struct _lv_draw_glyph_dsc_t lv_draw_glyph_dsc_t;

typedef struct _lv_draw_image_sup_t lv_draw_image_sup_t;

typedef struct _lv_draw_mask_rect_dsc_t lv_draw_mask_rect_dsc_t;

typedef struct _lv_obj_style_t lv_obj_style_t;

typedef struct _lv_obj_style_transition_dsc_t lv_obj_style_transition_dsc_t;

typedef struct _lv_hit_test_info_t lv_hit_test_info_t;

typedef struct _lv_cover_check_info_t lv_cover_check_info_t;

typedef struct _lv_obj_spec_attr_t lv_obj_spec_attr_t;

typedef struct _lv_image_t lv_image_t;

typedef struct _lv_animimg_t lv_animimg_t;

typedef struct _lv_arc_t lv_arc_t;

typedef struct _lv_arclabel_t lv_arclabel_t;

typedef struct _lv_label_t lv_label_t;

typedef struct _lv_bar_anim_t lv_bar_anim_t;

typedef struct _lv_bar_t lv_bar_t;

typedef struct _lv_button_t lv_button_t;

typedef struct _lv_buttonmatrix_t lv_buttonmatrix_t;

typedef struct _lv_calendar_t lv_calendar_t;

typedef struct _lv_canvas_t lv_canvas_t;

typedef struct _lv_chart_series_t lv_chart_series_t;

typedef struct _lv_chart_cursor_t lv_chart_cursor_t;

typedef struct _lv_chart_t lv_chart_t;

typedef struct _lv_checkbox_t lv_checkbox_t;

typedef struct _lv_dropdown_t lv_dropdown_t;

typedef struct _lv_dropdown_list_t lv_dropdown_list_t;

typedef struct _lv_imagebutton_src_info_t lv_imagebutton_src_info_t;

typedef struct _lv_imagebutton_t lv_imagebutton_t;

typedef struct _lv_keyboard_t lv_keyboard_t;

typedef struct _lv_led_t lv_led_t;

typedef struct _lv_line_t lv_line_t;

typedef struct _lv_menu_load_page_event_data_t lv_menu_load_page_event_data_t;

typedef struct _lv_menu_history_t lv_menu_history_t;

typedef struct _lv_menu_t lv_menu_t;

typedef struct _lv_menu_page_t lv_menu_page_t;

typedef struct _lv_msgbox_t lv_msgbox_t;

typedef struct _lv_roller_t lv_roller_t;

typedef struct _lv_scale_section_t lv_scale_section_t;

typedef struct _lv_scale_t lv_scale_t;

typedef struct _lv_slider_t lv_slider_t;

typedef struct _lv_span_t lv_span_t;

typedef struct _lv_spangroup_t lv_spangroup_t;

typedef struct _lv_textarea_t lv_textarea_t;

typedef struct _lv_spinbox_t lv_spinbox_t;

typedef struct _lv_switch_t lv_switch_t;

typedef struct _lv_table_cell_t lv_table_cell_t;

typedef struct _lv_table_t lv_table_t;

typedef struct _lv_tabview_t lv_tabview_t;

typedef struct _lv_tileview_t lv_tileview_t;

typedef struct _lv_tileview_tile_t lv_tileview_tile_t;

typedef struct _lv_win_t lv_win_t;

typedef struct _lv_spinner_t lv_spinner_t;

typedef struct _lv_3dtexture_t lv_3dtexture_t;

typedef struct _lv_gltf_t lv_gltf_t;

typedef struct _lv_gltf_model_t lv_gltf_model_t;

typedef struct _lv_gltf_model_node_t lv_gltf_model_node_t;

typedef struct _lv_gltf_environment lv_gltf_environment_t;

typedef struct _lv_gltf_ibl_sampler lv_gltf_ibl_sampler_t;

typedef struct _lv_subject_t lv_subject_t;

typedef struct _lv_observer_t lv_observer_t;

typedef struct _lv_subject_increment_dsc_t lv_subject_increment_dsc_t;

typedef struct _lv_monkey_config_t lv_monkey_config_t;

typedef struct _lv_ime_pinyin_t lv_ime_pinyin_t;

typedef struct _lv_file_explorer_t lv_file_explorer_t;

typedef struct _lv_barcode_t lv_barcode_t;

typedef struct _lv_qrcode_t lv_qrcode_t;

typedef struct _lv_freetype_outline_vector_t lv_freetype_outline_vector_t;

typedef struct _lv_freetype_outline_event_param_t lv_freetype_outline_event_param_t;

typedef struct _lv_fpoint_t lv_fpoint_t;

typedef struct _lv_matrix_t lv_matrix_t;

typedef struct _lv_vector_path_t lv_vector_path_t;

typedef struct _lv_vector_gradient_t lv_vector_gradient_t;

typedef struct _lv_vector_fill_dsc_t lv_vector_fill_dsc_t;

typedef struct _lv_vector_stroke_dsc_t lv_vector_stroke_dsc_t;

typedef struct _lv_vector_path_ctx_t lv_vector_path_ctx_t;

typedef struct _lv_draw_vector_dsc_t lv_draw_vector_dsc_t;

typedef struct _lv_xkb_t lv_xkb_t;

typedef struct _lv_libinput_event_t lv_libinput_event_t;

typedef struct _lv_libinput_t lv_libinput_t;

typedef struct _lv_draw_sw_unit_t lv_draw_sw_unit_t;

typedef struct _lv_draw_sw_mask_common_dsc_t lv_draw_sw_mask_common_dsc_t;

typedef struct _lv_draw_sw_mask_line_param_t lv_draw_sw_mask_line_param_t;

typedef struct _lv_draw_sw_mask_angle_param_t lv_draw_sw_mask_angle_param_t;

typedef struct _lv_draw_sw_mask_radius_param_t lv_draw_sw_mask_radius_param_t;

typedef struct _lv_draw_sw_mask_fade_param_t lv_draw_sw_mask_fade_param_t;

typedef struct _lv_draw_sw_mask_map_param_t lv_draw_sw_mask_map_param_t;

typedef struct _lv_draw_sw_blend_dsc_t lv_draw_sw_blend_dsc_t;

typedef struct _lv_draw_sw_blend_fill_dsc_t lv_draw_sw_blend_fill_dsc_t;

typedef struct _lv_draw_sw_blend_image_dsc_t lv_draw_sw_blend_image_dsc_t;

typedef struct _lv_draw_buf_handlers_t lv_draw_buf_handlers_t;

typedef struct _lv_rlottie_t lv_rlottie_t;

typedef struct _lv_ffmpeg_player_t lv_ffmpeg_player_t;

typedef struct _lv_opengles_window_t lv_opengles_window_t;
typedef struct _lv_opengles_window_texture_t lv_opengles_window_texture_t;

typedef uint32_t lv_prop_id_t;

typedef struct _lv_array_t lv_array_t;

typedef struct _lv_iter_t lv_iter_t;

typedef struct _lv_circle_buf_t lv_circle_buf_t;

typedef struct _lv_draw_buf_t lv_draw_buf_t;

#if LV_USE_OBJ_PROPERTY
typedef struct _lv_property_name_t lv_property_name_t;
#endif

#if LV_USE_SYSMON

typedef struct _lv_sysmon_backend_data_t lv_sysmon_backend_data_t;

#if LV_USE_PERF_MONITOR
typedef struct _lv_sysmon_perf_info_t lv_sysmon_perf_info_t;
#endif /*LV_USE_PERF_MONITOR*/

#endif /*LV_USE_SYSMON*/


typedef struct _lv_xml_component_scope_t lv_xml_component_scope_t;

typedef struct _lv_xml_parser_state_t lv_xml_parser_state_t;

typedef struct _lv_xml_load_t lv_xml_load_t;

#if LV_USE_EVDEV
typedef struct _lv_evdev_discovery_t lv_evdev_discovery_t;
#endif

#if LV_USE_TRANSLATION
typedef struct _lv_translation_tag_dsc_t lv_translation_tag_dsc_t;

typedef struct _lv_translation_pack_t lv_translation_pack_t;
#endif

#if LV_USE_DRAW_EVE
typedef struct _lv_draw_eve_unit_t lv_draw_eve_unit_t;
#endif

#endif /*__ASSEMBLY__*/

/** This describes a glyph.*/
typedef struct {
#if LV_FONT_FMT_TXT_LARGE == 0
    uint32_t bitmap_index : 20;     /**< Start index of the bitmap. A font can be max 1 MB.*/
    uint32_t adv_w : 12;            /**< Draw the next glyph after this width. 8.4 format (real_value * 16 is stored).*/
    uint8_t box_w;                  /**< Width of the glyph's bounding box*/
    uint8_t box_h;                  /**< Height of the glyph's bounding box*/
    int8_t ofs_x;                   /**< x offset of the bounding box*/
    int8_t ofs_y;                   /**< y offset of the bounding box. Measured from the top of the line*/
#else
    uint32_t bitmap_index;          /**< Start index of the bitmap. A font can be max 4 GB.*/
    uint32_t adv_w;                 /**< Draw the next glyph after this width. 28.4 format (real_value * 16 is stored).*/
    uint16_t box_w;                 /**< Width of the glyph's bounding box*/
    uint16_t box_h;                 /**< Height of the glyph's bounding box*/
    int16_t ofs_x;                  /**< x offset of the bounding box*/
    int16_t ofs_y;                  /**< y offset of the bounding box. Measured from the top of the line*/
#endif
} lv_font_fmt_txt_glyph_dsc_t;

/** Format of font character map.*/
typedef enum {
    LV_FONT_FMT_TXT_CMAP_FORMAT0_FULL,
    LV_FONT_FMT_TXT_CMAP_SPARSE_FULL,
    LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY,
    LV_FONT_FMT_TXT_CMAP_SPARSE_TINY,
} lv_font_fmt_txt_cmap_type_t;

/**
 * Map codepoints to a `glyph_dsc`s
 * Several formats are supported to optimize memory usage
 * See https://github.com/lvgl/lv_font_conv/blob/master/doc/font_spec.md
 */
typedef struct {
    /** First Unicode character for this range*/
    uint32_t range_start;

    /** Number of Unicode characters related to this range.
     * Last Unicode character = range_start + range_length - 1*/
    uint16_t range_length;

    /** First glyph ID (array index of `glyph_dsc`) for this range*/
    uint16_t glyph_id_start;

    /*
    According the specification there are 4 formats:
        https://github.com/lvgl/lv_font_conv/blob/master/doc/font_spec.md

    For simplicity introduce "relative code point":
        rcp = codepoint - range_start

    and a search function:
        search a "value" in an "array" and returns the index of "value".

    Format 0 tiny
        unicode_list == NULL && glyph_id_ofs_list == NULL
        glyph_id = glyph_id_start + rcp

    Format 0 full
        unicode_list == NULL && glyph_id_ofs_list != NULL
        glyph_id = glyph_id_start + glyph_id_ofs_list[rcp]

    Sparse tiny
        unicode_list != NULL && glyph_id_ofs_list == NULL
        glyph_id = glyph_id_start + search(unicode_list, rcp)

    Sparse full
        unicode_list != NULL && glyph_id_ofs_list != NULL
        glyph_id = glyph_id_start + glyph_id_ofs_list[search(unicode_list, rcp)]
    */

    const uint16_t * unicode_list;

    /** if(type == LV_FONT_FMT_TXT_CMAP_FORMAT0_...) it's `uint8_t *`
     * if(type == LV_FONT_FMT_TXT_CMAP_SPARSE_...)  it's `uint16_t *`
     */
    const void * glyph_id_ofs_list;

    /** Length of `unicode_list` and/or `glyph_id_ofs_list`*/
    uint16_t list_length;

    /** Type of this character map*/
    lv_font_fmt_txt_cmap_type_t type;
} lv_font_fmt_txt_cmap_t;

typedef struct {
    /** The bitmaps of all glyphs */
    const uint8_t * glyph_bitmap;

    /** Describe the glyphs */
    const lv_font_fmt_txt_glyph_dsc_t * glyph_dsc;

    /** Map the glyphs to Unicode characters.
     *Array of `lv_font_cmap_fmt_txt_t` variables */
    const lv_font_fmt_txt_cmap_t * cmaps;

    /**
     * Store kerning values.
     * Can be `lv_font_fmt_txt_kern_pair_t *  or `lv_font_kern_classes_fmt_txt_t *`
     * depending on `kern_classes`
     */
    const void * kern_dsc;

    /** Scale kern values in 12.4 format */
    uint16_t kern_scale;

    /** Number of cmap tables */
    uint16_t cmap_num       : 9;

    /** Bit per pixel: 1, 2, 3, 4, 8 */
    uint16_t bpp            : 4;

    /** Type of `kern_dsc` */
    uint16_t kern_classes   : 1;

    /**
     * storage format of the bitmap
     * from `lv_font_fmt_txt_bitmap_format_t`
     */
    uint16_t bitmap_format  : 2;

    /**
     * Bytes to which each line is padded.
     * 0: means no align and padding
     * 1: e.g. with bpp=4 lines are aligned to 1 byte, so there can be a 4 bits of padding
     * 4, 8, 16, 32, 64: each line is padded to the given byte boundaries
     */
    uint8_t stride;
} lv_font_fmt_txt_dsc_t;

/** The font format.*/
typedef enum {
    LV_FONT_GLYPH_FORMAT_NONE   = 0, /**< Maybe not visible*/

    /**< Legacy simple formats*/
    LV_FONT_GLYPH_FORMAT_A1     = 0x01, /**< 1 bit per pixel*/
    LV_FONT_GLYPH_FORMAT_A2     = 0x02, /**< 2 bit per pixel*/
    LV_FONT_GLYPH_FORMAT_A3     = 0x03, /**< 3 bit per pixel*/
    LV_FONT_GLYPH_FORMAT_A4     = 0x04, /**< 4 bit per pixel*/
    LV_FONT_GLYPH_FORMAT_A8     = 0x08, /**< 8 bit per pixel*/

    LV_FONT_GLYPH_FORMAT_IMAGE  = 0x19, /**< Image format*/

    /**< Advanced formats*/
    LV_FONT_GLYPH_FORMAT_VECTOR = 0x1A, /**< Vectorial format*/
    LV_FONT_GLYPH_FORMAT_SVG    = 0x1B, /**< SVG format*/
    LV_FONT_GLYPH_FORMAT_CUSTOM = 0xFF, /**< Custom format*/
} lv_font_glyph_format_t;

/** Describes the properties of a glyph.*/
typedef struct {
    const lv_font_t *
    resolved_font;  /**< Pointer to a font where the glyph was actually found after handling fallbacks*/
    uint16_t adv_w; /**< The glyph needs this space. Draw the next glyph after this width.*/
    uint16_t box_w; /**< Width of the glyph's bounding box*/
    uint16_t box_h; /**< Height of the glyph's bounding box*/
    int16_t ofs_x;  /**< x offset of the bounding box*/
    int16_t ofs_y;  /**< y offset of the bounding box*/
    uint16_t stride;/**< Bytes in each line. If 0 than there is no padding at the end of the line. */
    lv_font_glyph_format_t format;  /**< Font format of the glyph see lv_font_glyph_format_t */
    uint8_t is_placeholder: 1;      /**< Glyph is missing. But placeholder will still be displayed*/

    /** 0: Get bitmap should return an A8 or ARGB8888 image.
      * 1: return the bitmap as it is (Maybe A1/2/4 or any proprietary formats). */
    uint8_t req_raw_bitmap: 1;

    int32_t outline_stroke_width;   /**< used with freetype vector fonts - width of the letter border */

    union {
        uint32_t index;       /**< Glyph descriptor index*/
        const void * src;     /**< Pointer to the source data used by image fonts*/
    } gid;                    /**< The index of the glyph in the font file. Used by the font cache*/
    lv_cache_entry_t * entry; /**< The cache entry of the glyph draw data. Used by the font cache*/
} lv_font_glyph_dsc_t;

struct _lv_font_t {
    /** Get a glyph's descriptor from a font*/
    bool (*get_glyph_dsc)(const lv_font_t *, lv_font_glyph_dsc_t *, uint32_t letter, uint32_t letter_next);

    /** Get a glyph's bitmap from a font*/
    const void * (*get_glyph_bitmap)(lv_font_glyph_dsc_t *, lv_draw_buf_t *);

    /** Release a glyph*/
    void (*release_glyph)(const lv_font_t *, lv_font_glyph_dsc_t *);

    /*Pointer to the font in a font pack (must have the same line height)*/
    int32_t line_height;         /**< The real line height where any text fits*/
    int32_t base_line;           /**< Base line measured from the bottom of the line_height*/
    uint8_t subpx   : 2;            /**< An element of `lv_font_subpx_t`*/
    uint8_t kerning : 1;            /**< An element of `lv_font_kerning_t`*/
    uint8_t static_bitmap : 1;      /**< The font will be used as static bitmap */

    int8_t underline_position;      /**< Distance between the top of the underline and base line (< 0 means below the base line)*/
    int8_t underline_thickness;     /**< Thickness of the underline*/

    const void * dsc;               /**< Store implementation specific or run_time data or caching here*/
    const lv_font_t * fallback;     /**< Fallback font for missing glyph. Resolved recursively */
    void * user_data;               /**< Custom user data for font.*/
};


typedef struct{

    /*Pointer to the font in a font pack (must have the same line height)*/
    int32_t line_height;         /**< The real line height where any text fits*/
    int32_t base_line;           /**< Base line measured from the bottom of the line_height*/
        /** The bitmaps of all glyphs */
    const uint8_t * glyph_bitmap;

    /** Describe the glyphs */
    const lv_font_fmt_txt_glyph_dsc_t * glyph_dsc;

    /** Map the glyphs to Unicode characters.
     *Array of `lv_font_cmap_fmt_txt_t` variables */
    const lv_font_fmt_txt_cmap_t * cmaps;

    /** Number of cmap tables */
    uint16_t cmap_num       : 9;

    /** Bit per pixel: 1, 2, 3, 4, 8 */
    uint16_t bpp            : 4;
} font_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**********************
 *      MACROS
 **********************/

#define LV_UNUSED(x) ((void)x)

#define _LV_CONCAT(x, y) x ## y
#define LV_CONCAT(x, y) _LV_CONCAT(x, y)
#undef _LV_CONCAT

#define _LV_CONCAT3(x, y, z) x ## y ## z
#define LV_CONCAT3(x, y, z) _LV_CONCAT3(x, y, z)
#undef _LV_CONCAT3

#if defined(PYCPARSER) || defined(__CC_ARM)
#define LV_FORMAT_ATTRIBUTE(fmtstr, vararg)
#elif defined(__GNUC__) && ((__GNUC__ == 4 && __GNUC_MINOR__ >= 4) || __GNUC__ > 4)
#define LV_FORMAT_ATTRIBUTE(fmtstr, vararg) __attribute__((format(gnu_printf, fmtstr, vararg)))
#elif (defined(__clang__) || defined(__GNUC__) || defined(__GNUG__) || defined(__IAR_SYSTEMS_ICC__))
#define LV_FORMAT_ATTRIBUTE(fmtstr, vararg) __attribute__((format(printf, fmtstr, vararg)))
#else
#define LV_FORMAT_ATTRIBUTE(fmtstr, vararg)
#endif

#ifndef LV_NORETURN
#if defined(PYCPARSER)
#define LV_NORETURN
#elif defined(__GNUC__)
#define LV_NORETURN __attribute__((noreturn))
#elif defined(_MSC_VER)
#define LV_NORETURN __declspec(noreturn)
#else
#define LV_NORETURN
#endif
#endif /* LV_NORETURN not defined */

#ifndef LV_UNREACHABLE
#if defined(__GNUC__)
#define LV_UNREACHABLE() __builtin_unreachable()
#elif defined(_MSC_VER)
#define LV_UNREACHABLE() __assume(0)
#else
#define LV_UNREACHABLE() while(1)
#endif
#endif /* LV_UNREACHABLE not defined */

#ifndef LV_ARRAYLEN
#define LV_ARRAYLEN(a) (sizeof(a)/sizeof((a)[0]))
#endif /*LV_ARRAYLEN*/

extern const uint8_t glyph_bitmap[];
extern const font_t lv_font_montserrat_20;

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_TYPES_H*/
