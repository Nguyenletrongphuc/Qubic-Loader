#ifndef EVENT_KEY_INPUT_EVENT_HPP
#define EVENT_KEY_INPUT_EVENT_HPP

#include <qubic-api/inc/Qubic.hpp>

// GLFW Key codes
#define QUBIC_KEY_SPACE         32
#define QUBIC_KEY_APOSTROPHE    39  /* ' */
#define QUBIC_KEY_COMMA         44  /* , */
#define QUBIC_KEY_MINUS         45  /* - */
#define QUBIC_KEY_PERIOD        46  /* . */
#define QUBIC_KEY_SLASH         47  /* / */
#define QUBIC_KEY_0             48
#define QUBIC_KEY_1             49
#define QUBIC_KEY_2             50
#define QUBIC_KEY_3             51
#define QUBIC_KEY_4             52
#define QUBIC_KEY_5             53
#define QUBIC_KEY_6             54
#define QUBIC_KEY_7             55
#define QUBIC_KEY_8             56
#define QUBIC_KEY_9             57
#define QUBIC_KEY_SEMICOLON     59  /* ; */
#define QUBIC_KEY_EQUAL         61  /* = */
#define QUBIC_KEY_A             65
#define QUBIC_KEY_B             66
#define QUBIC_KEY_C             67
#define QUBIC_KEY_D             68
#define QUBIC_KEY_E             69
#define QUBIC_KEY_F             70
#define QUBIC_KEY_G             71
#define QUBIC_KEY_H             72
#define QUBIC_KEY_I             73
#define QUBIC_KEY_J             74
#define QUBIC_KEY_K             75
#define QUBIC_KEY_L             76
#define QUBIC_KEY_M             77
#define QUBIC_KEY_N             78
#define QUBIC_KEY_O             79
#define QUBIC_KEY_P             80
#define QUBIC_KEY_Q             81
#define QUBIC_KEY_R             82
#define QUBIC_KEY_S             83
#define QUBIC_KEY_T             84
#define QUBIC_KEY_U             85
#define QUBIC_KEY_V             86
#define QUBIC_KEY_W             87
#define QUBIC_KEY_X             88
#define QUBIC_KEY_Y             89
#define QUBIC_KEY_Z             90
#define QUBIC_KEY_LEFT_BRACKET  91  /* [ */
#define QUBIC_KEY_BACKSLASH     92  /* \ */
#define QUBIC_KEY_RIGHT_BRACKET 93  /* ] */
#define QUBIC_KEY_GRAVE_ACCENT  96  /* ` */

// Function keys
#define QUBIC_KEY_ESCAPE        256
#define QUBIC_KEY_ENTER         257
#define QUBIC_KEY_TAB           258
#define QUBIC_KEY_BACKSPACE     259
#define QUBIC_KEY_INSERT        260
#define QUBIC_KEY_DELETE        261
#define QUBIC_KEY_RIGHT         262
#define QUBIC_KEY_LEFT          263
#define QUBIC_KEY_DOWN          264
#define QUBIC_KEY_UP            265
#define QUBIC_KEY_PAGE_UP       266
#define QUBIC_KEY_PAGE_DOWN     267
#define QUBIC_KEY_HOME          268
#define QUBIC_KEY_END           269
#define QUBIC_KEY_CAPS_LOCK     280
#define QUBIC_KEY_SCROLL_LOCK   281
#define QUBIC_KEY_NUM_LOCK      282
#define QUBIC_KEY_PRINT_SCREEN  283
#define QUBIC_KEY_PAUSE         284
#define QUBIC_KEY_F1            290
#define QUBIC_KEY_F2            291
#define QUBIC_KEY_F3            292
#define QUBIC_KEY_F4            293
#define QUBIC_KEY_F5            294
#define QUBIC_KEY_F6            295
#define QUBIC_KEY_F7            296
#define QUBIC_KEY_F8            297
#define QUBIC_KEY_F9            298
#define QUBIC_KEY_F10           299
#define QUBIC_KEY_F11           300
#define QUBIC_KEY_F12           301

// Keypad
#define QUBIC_KEY_KP_0          320
#define QUBIC_KEY_KP_1          321
#define QUBIC_KEY_KP_2          322
#define QUBIC_KEY_KP_3          323
#define QUBIC_KEY_KP_4          324
#define QUBIC_KEY_KP_5          325
#define QUBIC_KEY_KP_6          326
#define QUBIC_KEY_KP_7          327
#define QUBIC_KEY_KP_8          328
#define QUBIC_KEY_KP_9          329
#define QUBIC_KEY_KP_DECIMAL    330
#define QUBIC_KEY_KP_DIVIDE     331
#define QUBIC_KEY_KP_MULTIPLY   332
#define QUBIC_KEY_KP_SUBTRACT   333
#define QUBIC_KEY_KP_ADD        334
#define QUBIC_KEY_KP_ENTER      335
#define QUBIC_KEY_KP_EQUAL      336

// Modifier keys
#define QUBIC_KEY_LEFT_SHIFT    340
#define QUBIC_KEY_LEFT_CONTROL  341
#define QUBIC_KEY_LEFT_ALT      342
#define QUBIC_KEY_LEFT_SUPER    343
#define QUBIC_KEY_RIGHT_SHIFT   344
#define QUBIC_KEY_RIGHT_CONTROL 345
#define QUBIC_KEY_RIGHT_ALT     346
#define QUBIC_KEY_RIGHT_SUPER   347
#define QUBIC_KEY_MENU          348

// GLFW Actions
#define QUBIC_RELEASE           0
#define QUBIC_PRESS             1
#define QUBIC_REPEAT            2

// GLFW Mods
#define QUBIC_MOD_SHIFT         0x0001
#define QUBIC_MOD_CONTROL       0x0002
#define QUBIC_MOD_ALT           0x0004
#define QUBIC_MOD_SUPER         0x0008
#define QUBIC_MOD_CAPS_LOCK     0x0010
#define QUBIC_MOD_NUM_LOCK      0x0020

namespace Qubic {
    struct KeyInputEvent final {
    public:
        int key;        /* the key that was pressed (GLFW key code) */
        int scancode;   /* platform-specific scancode */
        int action;     /* QUBIC_PRESS, QUBIC_RELEASE, or QUBIC_REPEAT */
        int mods;       /* modifier keys (shift, ctrl, alt) */
        JNIEnv* env;
        
        bool IsPress() const   { return action == QUBIC_PRESS; }
        bool IsRelease() const { return action == QUBIC_RELEASE; }
        bool IsRepeat() const  { return action == QUBIC_REPEAT; }
        
        bool HasShift() const  { return (mods & QUBIC_MOD_SHIFT) != 0; }
        bool HasCtrl() const   { return (mods & QUBIC_MOD_CONTROL) != 0; }
        bool HasAlt() const   { return (mods & QUBIC_MOD_ALT) != 0; }
    };

    namespace Key {
        // Printable keys
        constexpr int Space        = 32;
        constexpr int Apostrophe   = 39;
        constexpr int Comma        = 44;
        constexpr int Minus        = 45;
        constexpr int Period       = 46;
        constexpr int Slash        = 47;
        constexpr int Num0         = 48;
        constexpr int Num1         = 49;
        constexpr int Num2         = 50;
        constexpr int Num3         = 51;
        constexpr int Num4         = 52;
        constexpr int Num5         = 53;
        constexpr int Num6         = 54;
        constexpr int Num7         = 55;
        constexpr int Num8         = 56;
        constexpr int Num9         = 57;
        constexpr int Semicolon    = 59;
        constexpr int Equal        = 61;
        constexpr int A            = 65;
        constexpr int B            = 66;
        constexpr int C            = 67;
        constexpr int D            = 68;
        constexpr int E            = 69;
        constexpr int F            = 70;
        constexpr int G            = 71;
        constexpr int H            = 72;
        constexpr int I            = 73;
        constexpr int J            = 74;
        constexpr int K            = 75;
        constexpr int L            = 76;
        constexpr int M            = 77;
        constexpr int N            = 78;
        constexpr int O            = 79;
        constexpr int P            = 80;
        constexpr int Q            = 81;
        constexpr int R            = 82;
        constexpr int S            = 83;
        constexpr int T            = 84;
        constexpr int U            = 85;
        constexpr int V            = 86;
        constexpr int W            = 87;
        constexpr int X            = 88;
        constexpr int Y            = 89;
        constexpr int Z            = 90;
        constexpr int LeftBracket  = 91;
        constexpr int Backslash    = 92;
        constexpr int RightBracket = 93;
        constexpr int GraveAccent  = 96;

        // Function keys
        constexpr int Escape       = 256;
        constexpr int Enter        = 257;
        constexpr int Tab          = 258;
        constexpr int Backspace    = 259;
        constexpr int Insert       = 260;
        constexpr int Delete       = 261;
        constexpr int Right        = 262;
        constexpr int Left         = 263;
        constexpr int Down         = 264;
        constexpr int Up           = 265;
        constexpr int PageUp       = 266;
        constexpr int PageDown     = 267;
        constexpr int Home         = 268;
        constexpr int End          = 269;
        constexpr int CapsLock     = 280;
        constexpr int ScrollLock   = 281;
        constexpr int NumLock      = 282;
        constexpr int PrintScreen  = 283;
        constexpr int Pause        = 284;
        constexpr int F1           = 290;
        constexpr int F2           = 291;
        constexpr int F3           = 292;
        constexpr int F4           = 293;
        constexpr int F5           = 294;
        constexpr int F6           = 295;
        constexpr int F7           = 296;
        constexpr int F8           = 297;
        constexpr int F9           = 298;
        constexpr int F10          = 299;
        constexpr int F11          = 300;
        constexpr int F12          = 301;

        // Keypad
        constexpr int KP0          = 320;
        constexpr int KP1          = 321;
        constexpr int KP2          = 322;
        constexpr int KP3          = 323;
        constexpr int KP4          = 324;
        constexpr int KP5          = 325;
        constexpr int KP6          = 326;
        constexpr int KP7          = 327;
        constexpr int KP8          = 328;
        constexpr int KP9          = 329;
        constexpr int KPDecimal    = 330;
        constexpr int KPDivide     = 331;
        constexpr int KPMultiply   = 332;
        constexpr int KPSubtract   = 333;
        constexpr int KPAdd        = 334;
        constexpr int KPEnter      = 335;
        constexpr int KPEqual      = 336;

        // Modifiers
        constexpr int LeftShift    = 340;
        constexpr int LeftControl  = 341;
        constexpr int LeftAlt      = 342;
        constexpr int LeftSuper    = 343;
        constexpr int RightShift   = 344;
        constexpr int RightControl = 345;
        constexpr int RightAlt     = 346;
        constexpr int RightSuper   = 347;
        constexpr int Menu         = 348;
    }

    namespace Mouse {
        constexpr int Button1      = 0;
        constexpr int Button2      = 1;
        constexpr int Button3      = 2;
        constexpr int Button4      = 3;
        constexpr int Button5      = 4;
        constexpr int Button6      = 5;
        constexpr int Button7      = 6;
        constexpr int Button8      = 7;
        constexpr int Left         = Button1;
        constexpr int Right        = Button2;
        constexpr int Middle       = Button3;
    }

    namespace Action {
        constexpr int Release      = 0;
        constexpr int Press        = 1;
        constexpr int Repeat       = 2;
    }

    namespace Mod {
        constexpr int Shift        = 0x0001;
        constexpr int Control      = 0x0002;
        constexpr int Alt          = 0x0004;
        constexpr int Super        = 0x0008;
        constexpr int CapsLock     = 0x0010;
        constexpr int NumLock      = 0x0020;
    }
}

#endif