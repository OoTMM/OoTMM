#include "z_en_ru1.h"
#include "z64cutscene_commands.h"

// clang-format off
static CutsceneData gRutoFirstMeetingCs[] = {
    CS_HEADER(15, 1306),
    CS_PLAYER_CUE_LIST(10),
        CS_PLAYER_CUE(PLAYER_CUEID_39, 0, 50, 0x0000, 0x6C16, 0x0000, 12, -340, -2810, 12, -340, -2810, CS_FLOAT(0xF671408, 1.1393037e-29f), CS_FLOAT(0x0, 0.0f), CS_FLOAT(0x1, 1e-45f)),
        CS_PLAYER_CUE(PLAYER_CUEID_3, 50, 70, 0x0000, 0x6C16, 0x0000, 12, -340, -2810, 12, -340, -2810, CS_FLOAT(0xF671408, 1.1393037e-29f), CS_FLOAT(0x0, 0.0f), CS_FLOAT(0x1, 1e-45f)),
        CS_PLAYER_CUE(PLAYER_CUEID_5, 70, 109, 0x0000, 0x6C16, 0x0000, 12, -340, -2810, 12, -340, -2810, CS_FLOAT(0xF671408, 1.1393037e-29f), CS_FLOAT(0x0, 0.0f), CS_FLOAT(0x1, 1e-45f)),
        CS_PLAYER_CUE(PLAYER_CUEID_2, 109, 135, 0x0000, 0x6C16, 0x0000, 12, -340, -2810, 100, -340, -2991, CS_FLOAT(0xF671408, 1.1393037e-29f), CS_FLOAT(0x0, 0.0f), CS_FLOAT(0x1, 1e-45f)),
        CS_PLAYER_CUE(PLAYER_CUEID_5, 135, 199, 0x0000, 0x6C16, 0x0000, 100, -340, -2991, 100, -340, -2991, CS_FLOAT(0xF671408, 1.1393037e-29f), CS_FLOAT(0x0, 0.0f), CS_FLOAT(0x1, 1e-45f)),
        CS_PLAYER_CUE(PLAYER_CUEID_3, 199, 219, 0x0000, 0x6C16, 0x0000, 100, -340, -2991, 100, -340, -2991, CS_FLOAT(0xF671408, 1.1393037e-29f), CS_FLOAT(0x0, 0.0f), CS_FLOAT(0x1, 1e-45f)),
        CS_PLAYER_CUE(PLAYER_CUEID_5, 219, 259, 0x0000, 0x6C16, 0x0000, 100, -340, -2991, 100, -340, -2991, CS_FLOAT(0xF671408, 1.1393037e-29f), CS_FLOAT(0x0, 0.0f), CS_FLOAT(0x1, 1e-45f)),
        CS_PLAYER_CUE(PLAYER_CUEID_3, 259, 276, 0x0000, 0x6C16, 0x0000, 100, -340, -2991, 100, -340, -2991, CS_FLOAT(0xF671408, 1.1393037e-29f), CS_FLOAT(0x0, 0.0f), CS_FLOAT(0x1, 1e-45f)),
        CS_PLAYER_CUE(PLAYER_CUEID_2, 276, 297, 0x0000, 0x6C16, 0x0000, 100, -340, -2991, 182, -340, -3132, CS_FLOAT(0xF671408, 1.1393037e-29f), CS_FLOAT(0x0, 0.0f), CS_FLOAT(0x1, 1e-45f)),
        CS_PLAYER_CUE(PLAYER_CUEID_5, 297, 843, 0x0000, 0x6C16, 0x0000, 182, -340, -3132, 182, -340, -3132, CS_FLOAT(0xF671408, 1.1393037e-29f), CS_FLOAT(0x0, 0.0f), CS_FLOAT(0x1, 1e-45f)),
    CS_ACTOR_CUE_LIST(CS_CMD_ACTOR_CUE_3_5, 4),
        CS_ACTOR_CUE(0x0002, 0, 178, 0x0000, 0xEC16, 0x0000, 127, -340, -3041, 127, -340, -3041, CS_FLOAT(0xF671408, 1.1393037e-29f), CS_FLOAT(0x0, 0.0f), CS_FLOAT(0x1, 1e-45f)),
        CS_ACTOR_CUE(0x0003, 178, 245, 0x0000, 0x6C16, 0x0000, 127, -340, -3041, 127, -340, -3041, CS_FLOAT(0xF671408, 1.1393037e-29f), CS_FLOAT(0x0, 0.0f), CS_FLOAT(0x1, 1e-45f)),
        CS_ACTOR_CUE(0x0004, 245, 260, 0x0000, 0x6C16, 0x0000, 127, -340, -3041, 127, -340, -3041, CS_FLOAT(0xF671408, 1.1393037e-29f), CS_FLOAT(0x0, 0.0f), CS_FLOAT(0x1, 1e-45f)),
        CS_ACTOR_CUE(0x0005, 260, 396, 0x0000, 0x6C16, 0x0000, 127, -340, -3041, 127, -340, -3041, CS_FLOAT(0xF671408, 1.1393037e-29f), CS_FLOAT(0x0, 0.0f), CS_FLOAT(0x1, 1e-45f)),
    CS_MISC_LIST(1),
        CS_MISC(CS_MISC_STOP_CUTSCENE, 315, 345, 0x0000, 0x00000000, 0x00000000, 0xFFFFFFC0, 0x00000032, 0x00000000, 0xFFFFFFC0, 0x00000032, 0x00000000, 0x00000000, 0x00000000),
    CS_TEXT_LIST(4),
        CS_TEXT_NONE(0, 157),
        CS_TEXT(0x404B, 157, 175, CS_TEXT_NORMAL, 0xFFFF, 0xFFFF),
        CS_TEXT_NONE(175, 252),
        CS_TEXT(0x401E, 252, 300, CS_TEXT_NORMAL, 0xFFFF, 0xFFFF),
    CS_CAM_EYE_SPLINE(0, 247),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x4235998B, 45.399944f), 55, -290, -2749, 0x20BA),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x4235998B, 45.399944f), 55, -290, -2749, 0x44B8),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x4235998B, 45.399944f), 55, -290, -2749, 0x8080),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x4235998B, 45.399944f), 55, -290, -2749, 0xE243),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x41F6662C, 30.799889f), 55, -290, -2749, 0x005E),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x41F6662C, 30.799889f), 55, -290, -2749, 0xFFFF),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x41F6662C, 30.799889f), 55, -290, -2749, 0x0000),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x41F6662C, 30.799889f), 55, -290, -2749, 0x5B80),
        CS_CAM_POINT(CS_CAM_STOP, 0x00, 0, CS_FLOAT(0x41F6662C, 30.799889f), 55, -290, -2749, 0x49B4),
    CS_CAM_EYE_SPLINE(73, 208),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x4235998B, 45.399944f), 99, -307, -2985, 0x20BA),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x4235998B, 45.399944f), 99, -307, -2985, 0x44B8),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x4235998B, 45.399944f), 99, -307, -2985, 0x8080),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x4235998B, 45.399944f), 99, -307, -2985, 0xE243),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x4235998B, 45.399944f), 99, -307, -2985, 0x005E),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x4235998B, 45.399944f), 99, -307, -2985, 0xFFFF),
        CS_CAM_POINT(CS_CAM_STOP, 0x00, 0, CS_FLOAT(0x4235998B, 45.399944f), 99, -307, -2985, 0x0000),
    CS_CAM_EYE_SPLINE(116, 1207),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x42373325, 45.799946f), 155, -276, -2911, 0x20BA),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x42373325, 45.799946f), 155, -276, -2911, 0x44B8),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x42373325, 45.799946f), 155, -276, -2911, 0x8080),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x42373325, 45.799946f), 155, -276, -2911, 0xE243),
        CS_CAM_POINT(CS_CAM_STOP, 0x00, 0, CS_FLOAT(0x42373325, 45.799946f), 155, -276, -2911, 0x005E),
    CS_CAM_EYE_SPLINE(186, 1277),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x4235998C, 45.39995f), 77, -315, -2992, 0x20BA),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x4235998C, 45.39995f), 77, -315, -2992, 0x44B8),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x4235998C, 45.39995f), 77, -315, -2992, 0x8080),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x4235998C, 45.39995f), 77, -315, -2992, 0xE243),
        CS_CAM_POINT(CS_CAM_STOP, 0x00, 0, CS_FLOAT(0x4235998C, 45.39995f), 77, -315, -2992, 0x005E),
    CS_CAM_EYE_SPLINE(256, 425),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x41E1995C, 28.199883f), 350, -237, -3314, 0x20BA),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x41E1995C, 28.199883f), 350, -237, -3314, 0x44B8),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x41E1995C, 28.199883f), 350, -237, -3314, 0x8080),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x41E1995C, 28.199883f), 350, -237, -3314, 0xE243),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x423B3327, 46.799953f), 350, -237, -3314, 0x005E),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x423B3327, 46.799953f), 350, -237, -3314, 0xFFFF),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x423B3327, 46.799953f), 350, -237, -3314, 0x0000),
        CS_CAM_POINT(CS_CAM_STOP, 0x00, 0, CS_FLOAT(0x423B3327, 46.799953f), 350, -237, -3314, 0x5B80),
    CS_CAM_AT_SPLINE(0, 276),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 50, CS_FLOAT(0x4235998B, 45.399944f), 40, -256, -2823, 0x20BA),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 50, CS_FLOAT(0x4235998B, 45.399944f), 40, -256, -2823, 0x44B8),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 40, CS_FLOAT(0x4235998B, 45.399944f), 22, -313, -2823, 0x8080),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 10, CS_FLOAT(0x4235998B, 45.399944f), 23, -313, -2823, 0xE243),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 6, CS_FLOAT(0x41F6662C, 30.799889f), 73, -298, -2828, 0x005E),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 30, CS_FLOAT(0x41F6662C, 30.799889f), 73, -298, -2828, 0xFFFF),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 30, CS_FLOAT(0x41F6662C, 30.799889f), 73, -298, -2828, 0x0000),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 30, CS_FLOAT(0x41F6662C, 30.799889f), 73, -298, -2828, 0x5B80),
        CS_CAM_POINT(CS_CAM_STOP, 0x00, 30, CS_FLOAT(0x41F6662C, 30.799889f), 73, -298, -2828, 0x49B4),
    CS_CAM_AT_SPLINE(73, 237),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 30, CS_FLOAT(0x4270CCCD, 60.2f), 136, -314, -3060, 0x20BA),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 7, CS_FLOAT(0x42726667, 60.600002f), 136, -314, -3060, 0x44B8),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 7, CS_FLOAT(0x4235998B, 45.399944f), 136, -314, -3060, 0x8080),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 30, CS_FLOAT(0x4235998B, 45.399944f), 136, -314, -3060, 0xE243),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 30, CS_FLOAT(0x4235998B, 45.399944f), 136, -314, -3060, 0x005E),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 30, CS_FLOAT(0x4235998B, 45.399944f), 135, -313, -3060, 0xFFFF),
        CS_CAM_POINT(CS_CAM_STOP, 0x00, 30, CS_FLOAT(0x4235998B, 45.399944f), 135, -313, -3059, 0x0000),
    CS_CAM_AT_SPLINE(116, 1236),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 30, CS_FLOAT(0x42373325, 45.799946f), 132, -305, -2969, 0x20BA),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 30, CS_FLOAT(0x42373325, 45.799946f), 132, -304, -2969, 0x44B8),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 1000, CS_FLOAT(0x42373325, 45.799946f), 133, -304, -2969, 0x8080),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 30, CS_FLOAT(0x42373325, 45.799946f), 133, -304, -2969, 0xE243),
        CS_CAM_POINT(CS_CAM_STOP, 0x00, 30, CS_FLOAT(0x42373325, 45.799946f), 133, -304, -2969, 0x005E),
    CS_CAM_AT_SPLINE(186, 1306),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 30, CS_FLOAT(0x4235998C, 45.39995f), 138, -301, -3032, 0x20BA),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 30, CS_FLOAT(0x4235998C, 45.39995f), 138, -301, -3032, 0x44B8),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 1000, CS_FLOAT(0x4235998C, 45.39995f), 138, -301, -3032, 0x8080),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 30, CS_FLOAT(0x4235998C, 45.39995f), 138, -301, -3032, 0xE243),
        CS_CAM_POINT(CS_CAM_STOP, 0x00, 30, CS_FLOAT(0x4235998C, 45.39995f), 138, -301, -3032, 0x005E),
    CS_CAM_AT_SPLINE(256, 454),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 30, CS_FLOAT(0x41E1995C, 28.199883f), 307, -254, -3258, 0x20BA),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 30, CS_FLOAT(0x41E1995C, 28.199883f), 307, -254, -3258, 0x44B8),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 9, CS_FLOAT(0x41E1995C, 28.199883f), 307, -254, -3258, 0x8080),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 9, CS_FLOAT(0x423B3327, 46.799953f), 304, -273, -3273, 0xE243),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 30, CS_FLOAT(0x423B3327, 46.799953f), 304, -272, -3274, 0x005E),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 30, CS_FLOAT(0x423B3327, 46.799953f), 304, -272, -3274, 0xFFFF),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 30, CS_FLOAT(0x423B3327, 46.799953f), 304, -272, -3274, 0x0000),
        CS_CAM_POINT(CS_CAM_STOP, 0x00, 30, CS_FLOAT(0x423B3327, 46.799953f), 305, -272, -3274, 0x5B80),
    CS_START_SEQ_LIST(1),
        CS_START_SEQ(NA_BGM_APPEAR, 74, 75, 0x0000, 0x00000000, 0xFFFFFFF1, 0x00000000, 0x0000004E, 0xFFFFFFF1, 0x00000000, 0x0000004E),
    CS_END_OF_SCRIPT(),
};

static u32 D_80AF10A0 = 0;

static CutsceneData gRutoObtainingSapphireCs[] = {
    CS_HEADER(14, 1299),
    CS_PLAYER_CUE_LIST(3),
        CS_PLAYER_CUE(PLAYER_CUEID_5, 0, 272, 0x0000, 0xC000, 0x0000, -1085, -1025, -3347, -1085, -1025, -3347, CS_FLOAT(0xF671408, 1.1393037e-29f), CS_FLOAT(0x0, 0.0f), CS_FLOAT(0x1, 1e-45f)),
        CS_PLAYER_CUE(PLAYER_CUEID_3, 272, 292, 0x0000, 0xC000, 0x0000, -1085, -1025, -3347, -1085, -1025, -3347, CS_FLOAT(0xF671408, 1.1393037e-29f), CS_FLOAT(0x0, 0.0f), CS_FLOAT(0x1, 1e-45f)),
        CS_PLAYER_CUE(PLAYER_CUEID_5, 292, 777, 0x0000, 0xC000, 0x0000, -1085, -1025, -3347, -1085, -1025, -3347, CS_FLOAT(0xF671408, 1.1393037e-29f), CS_FLOAT(0x0, 0.0f), CS_FLOAT(0x1, 1e-45f)),
    CS_MISC_LIST(1),
        CS_MISC(CS_MISC_STOP_CUTSCENE, 330, 627, 0x0000, 0x00000000, 0x00000000, 0xFFFFFFC0, 0x00000032, 0x00000000, 0xFFFFFFC0, 0x00000032, 0x00000000, 0x00000000, 0x00000000),
    CS_ACTOR_CUE_LIST(CS_CMD_ACTOR_CUE_3_7, 3),
        CS_ACTOR_CUE(0x0001, 0, 40, 0x0000, 0x4000, 0x0000, -1352, -969, -3341, -1352, -969, -3341, CS_FLOAT(0x0, 0.0f), CS_FLOAT(0x0, 0.0f), CS_FLOAT(0x1, 1e-45f)),
        CS_ACTOR_CUE(0x0002, 40, 213, 0x0000, 0x4000, 0x0000, -1352, -969, -3341, -1360, -969, -3343, CS_FLOAT(0x0, 0.0f), CS_FLOAT(0x0, 0.0f), CS_FLOAT(0x1, 1e-45f)),
        CS_ACTOR_CUE(0x0003, 213, 1000, 0x0000, 0x4000, 0x0000, -1360, -969, -3343, -1360, -969, -3343, CS_FLOAT(0x0, 0.0f), CS_FLOAT(0x0, 0.0f), CS_FLOAT(0x1, 1e-45f)),
    CS_ACTOR_CUE_LIST(CS_CMD_ACTOR_CUE_1_5, 1),
        CS_ACTOR_CUE(0x0002, 0, 90, 0x0000, 0x0000, 0x0000, -1360, -963, -3343, -1360, -963, -3343, CS_FLOAT(0x0, 0.0f), CS_FLOAT(0x0, 0.0f), CS_FLOAT(0x0, 0.0f)),
    CS_ACTOR_CUE_LIST(CS_CMD_ACTOR_CUE_1_5, 2),
        CS_ACTOR_CUE(0x0002, 90, 211, 0x0000, 0x0000, 0x0000, -1352, -922, -3341, -1352, -922, -3341, CS_FLOAT(0x0, 0.0f), CS_FLOAT(0x0, 0.0f), CS_FLOAT(0x0, 0.0f)),
        CS_ACTOR_CUE(0x0006, 211, 311, 0x0000, 0x0000, 0x0000, -1352, -922, -3341, -1352, -922, -3341, CS_FLOAT(0x0, 0.0f), CS_FLOAT(0x0, 0.0f), CS_FLOAT(0x0, 0.0f)),
    CS_ACTOR_CUE_LIST(CS_CMD_ACTOR_CUE_8_0, 3),
        CS_ACTOR_CUE(0x0004, 0, 210, 0x0000, 0x0000, 0x0000, -1065, -972, -3305, -1065, -978, -3305, CS_FLOAT(0x0, 0.0f), CS_FLOAT(0xBCEA0EA1, -0.028571429f), CS_FLOAT(0x0, 0.0f)),
        CS_ACTOR_CUE(0x0004, 210, 220, 0x8000, 0x0000, 0x0000, -1065, -978, -3305, -1065, -973, -3344, CS_FLOAT(0x0, 0.0f), CS_FLOAT(0x3F000000, 0.5f), CS_FLOAT(0x0, 0.0f)),
        CS_ACTOR_CUE(0x0004, 220, 410, 0x0000, 0x0000, 0x0000, -1065, -973, -3344, -1065, -976, -3344, CS_FLOAT(0x0, 0.0f), CS_FLOAT(0xBC8158ED, -0.015789473f), CS_FLOAT(0x0, 0.0f)),
    CS_TEXT_LIST(6),
        CS_TEXT_NONE(0, 162),
        CS_TEXT(0x4050, 162, 211, CS_TEXT_NORMAL, 0xFFFF, 0xFFFF),
        CS_TEXT_NONE(211, 232),
        CS_TEXT(0x4051, 232, 241, CS_TEXT_NORMAL, 0xFFFF, 0xFFFF),
        CS_TEXT_NONE(241, 247),
        CS_TEXT(0x4052, 247, 299, CS_TEXT_NORMAL, 0xFFFF, 0xFFFF),
    CS_START_SEQ_LIST(1),
        CS_START_SEQ(NA_BGM_ITEM_GET, 112, 113, 0x0000, 0x00000000, 0x00000000, 0xFFFFFFC7, 0x000000B1, 0x00000000, 0xFFFFFFC7, 0x000000B1),
    CS_CAM_EYE_SPLINE(0, 1176),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x4227A05C, 41.9066f), -1390, -948, -3339, 0x00C6),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x4222D38E, 40.706596f), -1390, -948, -3339, 0x00C8),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x4222D38E, 40.706596f), -1390, -948, -3339, 0x00D7),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x4222D38E, 40.706596f), -1418, -938, -3337, 0x00E8),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x42346D2B, 45.10661f), -1418, -938, -3337, 0x00EA),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x42346D2B, 45.10661f), -1418, -938, -3337, 0x013D),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x42346D2B, 45.10661f), -1418, -938, -3337, 0x013F),
        CS_CAM_POINT(CS_CAM_STOP, 0x00, 0, CS_FLOAT(0x42346D2B, 45.10661f), -1418, -938, -3337, 0x006D),
    CS_CAM_EYE_SPLINE(91, 1270),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x4233A05F, 44.906612f), -1319, -934, -3343, 0x00C6),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x4232D392, 44.70661f), -1319, -936, -3344, 0x00C8),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x4232D392, 44.70661f), -1319, -936, -3344, 0x00D7),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x4232D392, 44.70661f), -1319, -936, -3344, 0x00E8),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x4232D392, 44.70661f), -1326, -904, -3342, 0x00EA),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x4273A06F, 60.906673f), -1326, -904, -3342, 0x013D),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x4273A06F, 60.906673f), -1326, -904, -3342, 0x013F),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x4273A06F, 60.906673f), -1326, -904, -3342, 0x014E),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x4273A06F, 60.906673f), -1326, -904, -3342, 0x015F),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x4273A06F, 60.906673f), -1326, -904, -3342, 0x0161),
        CS_CAM_POINT(CS_CAM_STOP, 0x00, 0, CS_FLOAT(0x4273A06F, 60.906673f), -1326, -1024, -3342, 0x652E),
    CS_CAM_EYE_SPLINE(211, 332),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x41F273D3, 30.306555f), -1471, -819, -3149, 0x00C6),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x41F273D3, 30.306555f), -1471, -819, -3149, 0x00C8),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x41F273D3, 30.306555f), -1471, -819, -3149, 0x00D7),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x41F273D3, 30.306555f), -1471, -819, -3149, 0x00E8),
        CS_CAM_POINT(CS_CAM_STOP, 0x00, 0, CS_FLOAT(0x41F273D3, 30.306555f), -1471, -819, -3149, 0x00EA),
    CS_CAM_AT_SPLINE(0, 1205),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 30, CS_FLOAT(0x4222D38E, 40.706596f), -1295, -1003, -3352, 0x00C6),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 50, CS_FLOAT(0x4222D38E, 40.706596f), -1296, -1003, -3352, 0x00C8),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 20, CS_FLOAT(0x4222D38E, 40.706596f), -1296, -1003, -3352, 0x00D7),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 15, CS_FLOAT(0x42346D2B, 45.10661f), -1314, -969, -3346, 0x00E8),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 30, CS_FLOAT(0x42346D2B, 45.10661f), -1313, -970, -3346, 0x00EA),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 1000, CS_FLOAT(0x42346D2B, 45.10661f), -1313, -969, -3346, 0x013D),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 30, CS_FLOAT(0x42346D2B, 45.10661f), -1313, -970, -3346, 0x013F),
        CS_CAM_POINT(CS_CAM_STOP, 0x00, 30, CS_FLOAT(0x42346D2B, 45.10661f), -1313, -970, -3346, 0x006D),
    CS_CAM_AT_SPLINE(91, 1299),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 30, CS_FLOAT(0x4232D392, 44.70661f), -1405, -988, -3343, 0x00C6),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 7, CS_FLOAT(0x4232D392, 44.70661f), -1406, -989, -3344, 0x00C8),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 7, CS_FLOAT(0x4232D392, 44.70661f), -1406, -989, -3344, 0x00D7),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 7, CS_FLOAT(0x4232D392, 44.70661f), -1406, -989, -3344, 0x00E8),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 7, CS_FLOAT(0x4273A06F, 60.906673f), -1393, -978, -3342, 0x00EA),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 30, CS_FLOAT(0x4273A06F, 60.906673f), -1393, -977, -3342, 0x013D),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 30, CS_FLOAT(0x4273A06F, 60.906673f), -1393, -977, -3342, 0x013F),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 1000, CS_FLOAT(0x4273A06F, 60.906673f), -1393, -977, -3342, 0x014E),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 30, CS_FLOAT(0x4273A06F, 60.906673f), -1393, -977, -3342, 0x015F),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 30, CS_FLOAT(0x4273A06F, 60.906673f), -1393, -977, -3342, 0x0161),
        CS_CAM_POINT(CS_CAM_STOP, 0x00, 30, CS_FLOAT(0x4273A06F, 60.906673f), -1401, -1094, -3347, 0x652E),
    CS_CAM_AT_SPLINE(211, 361),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 30, CS_FLOAT(0x41F273D3, 30.306555f), -1426, -857, -3190, 0x00C6),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 30, CS_FLOAT(0x41F273D3, 30.306555f), -1426, -857, -3190, 0x00C8),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 30, CS_FLOAT(0x41F273D3, 30.306555f), -1426, -857, -3190, 0x00D7),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 30, CS_FLOAT(0x41F273D3, 30.306555f), -1426, -857, -3190, 0x00E8),
        CS_CAM_POINT(CS_CAM_STOP, 0x00, 30, CS_FLOAT(0x41F273D3, 30.306555f), -1426, -857, -3190, 0x00EA),
    CS_END_OF_SCRIPT(),
};

static u32 D_80AF1724 = 0;

static CutsceneData gRutoFoundSapphireCs[] = {
    CS_HEADER(7, 1160),
    CS_MISC_LIST(1),
        CS_MISC(CS_MISC_STOP_CUTSCENE, 75, 627, 0x0000, 0x00000000, 0x00000000, 0xFFFFFFC0, 0x00000032, 0x00000000, 0xFFFFFFC0, 0x00000032, 0x00000000, 0x00000000, 0x00000000),
    CS_ACTOR_CUE_LIST(CS_CMD_ACTOR_CUE_1_5, 1),
        CS_ACTOR_CUE(0x0002, 0, 90, 0x0000, 0x0000, 0x0000, -1360, -963, -3343, -1360, -963, -3343, CS_FLOAT(0x0, 0.0f), CS_FLOAT(0x0, 0.0f), CS_FLOAT(0x0, 0.0f)),
    CS_CAM_EYE_SPLINE(0, 1091),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x42226662, 40.599983f), -1381, -958, -3331, 0x8BC0),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x42226662, 40.599983f), -1381, -958, -3331, 0x2200),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x42226662, 40.599983f), -1381, -958, -3331, 0x0000),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x42226662, 40.599983f), -1381, -958, -3331, 0xD0E8),
        CS_CAM_POINT(CS_CAM_STOP, 0x00, 0, CS_FLOAT(0x42226662, 40.599983f), -1381, -958, -3331, 0x0000),
    CS_CAM_EYE_SPLINE_REL_TO_PLAYER(40, 1131),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x424B3339, 50.800022f), 19, 40, 53, 0x8BC0),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x424B3339, 50.800022f), 19, 40, 53, 0x2200),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x424B3339, 50.800022f), 19, 40, 53, 0x0000),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x424B3339, 50.800022f), 19, 40, 53, 0xD0E8),
        CS_CAM_POINT(CS_CAM_STOP, 0x00, 0, CS_FLOAT(0x424B3339, 50.800022f), 19, 40, 53, 0x0000),
    CS_CAM_AT_SPLINE(0, 1120),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 30, CS_FLOAT(0x42226662, 40.599983f), -1224, -979, -3366, 0x8BC0),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 30, CS_FLOAT(0x42226662, 40.599983f), -1224, -979, -3366, 0x2200),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 1000, CS_FLOAT(0x42226662, 40.599983f), -1224, -979, -3366, 0x0000),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 30, CS_FLOAT(0x42226662, 40.599983f), -1224, -979, -3366, 0xD0E8),
        CS_CAM_POINT(CS_CAM_STOP, 0x00, 30, CS_FLOAT(0x42226662, 40.599983f), -1224, -979, -3366, 0x0000),
    CS_CAM_AT_SPLINE_REL_TO_PLAYER(40, 1160),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 30, CS_FLOAT(0x424B3339, 50.800022f), -35, 56, -93, 0x8BC0),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 30, CS_FLOAT(0x424B3339, 50.800022f), -35, 56, -93, 0x2200),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 1000, CS_FLOAT(0x424B3339, 50.800022f), -35, 56, -93, 0x0000),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 30, CS_FLOAT(0x424B3339, 50.800022f), -35, 56, -93, 0xD0E8),
        CS_CAM_POINT(CS_CAM_STOP, 0x00, 30, CS_FLOAT(0x424B3339, 50.800022f), -35, 56, -93, 0x0000),
    CS_TEXT_LIST(2),
        CS_TEXT_NONE(0, 35),
        CS_TEXT(0x404F, 35, 70, CS_TEXT_NORMAL, 0xFFFF, 0xFFFF),
    CS_END_OF_SCRIPT(),
};
// clang-format on
