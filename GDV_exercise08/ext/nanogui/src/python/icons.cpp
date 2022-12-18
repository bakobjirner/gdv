#ifdef NANOGUI_PYTHON

#include "python.h"

/* Developer note: need to make a change to this file?
 * Please raise an Issue on GitHub describing what needs to change.  This file
 * was generated, so the scripts that generated it needs to update as well.
 */

struct Icon {
    const char *name;
    int value;
};

#define I(name) { #name, name }

static const Icon icons[] = {
    I(FA_AD),
    I(FA_ADDRESS_BOOK),
    I(FA_ADDRESS_CARD),
    I(FA_ADJUST),
    I(FA_AIR_FRESHENER),
    I(FA_ALIGN_CENTER),
    I(FA_ALIGN_JUSTIFY),
    I(FA_ALIGN_LEFT),
    I(FA_ALIGN_RIGHT),
    I(FA_ALLERGIES),
    I(FA_AMBULANCE),
    I(FA_AMERICAN_SIGN_LANGUAGE_INTERPRETING),
    I(FA_ANCHOR),
    I(FA_ANGLE_DOUBLE_DOWN),
    I(FA_ANGLE_DOUBLE_LEFT),
    I(FA_ANGLE_DOUBLE_RIGHT),
    I(FA_ANGLE_DOUBLE_UP),
    I(FA_ANGLE_DOWN),
    I(FA_ANGLE_LEFT),
    I(FA_ANGLE_RIGHT),
    I(FA_ANGLE_UP),
    I(FA_ANGRY),
    I(FA_ANKH),
    I(FA_APPLE_ALT),
    I(FA_ARCHIVE),
    I(FA_ARCHWAY),
    I(FA_ARROW_ALT_CIRCLE_DOWN),
    I(FA_ARROW_ALT_CIRCLE_LEFT),
    I(FA_ARROW_ALT_CIRCLE_RIGHT),
    I(FA_ARROW_ALT_CIRCLE_UP),
    I(FA_ARROW_CIRCLE_DOWN),
    I(FA_ARROW_CIRCLE_LEFT),
    I(FA_ARROW_CIRCLE_RIGHT),
    I(FA_ARROW_CIRCLE_UP),
    I(FA_ARROW_DOWN),
    I(FA_ARROW_LEFT),
    I(FA_ARROW_RIGHT),
    I(FA_ARROW_UP),
    I(FA_ARROWS_ALT),
    I(FA_ARROWS_ALT_H),
    I(FA_ARROWS_ALT_V),
    I(FA_ASSISTIVE_LISTENING_SYSTEMS),
    I(FA_ASTERISK),
    I(FA_AT),
    I(FA_ATLAS),
    I(FA_ATOM),
    I(FA_AUDIO_DESCRIPTION),
    I(FA_AWARD),
    I(FA_BABY),
    I(FA_BABY_CARRIAGE),
    I(FA_BACKSPACE),
    I(FA_BACKWARD),
    I(FA_BACON),
    I(FA_BALANCE_SCALE),
    I(FA_BALANCE_SCALE_LEFT),
    I(FA_BALANCE_SCALE_RIGHT),
    I(FA_BAN),
    I(FA_BAND_AID),
    I(FA_BARCODE),
    I(FA_BARS),
    I(FA_BASEBALL_BALL),
    I(FA_BASKETBALL_BALL),
    I(FA_BATH),
    I(FA_BATTERY_EMPTY),
    I(FA_BATTERY_FULL),
    I(FA_BATTERY_HALF),
    I(FA_BATTERY_QUARTER),
    I(FA_BATTERY_THREE_QUARTERS),
    I(FA_BED),
    I(FA_BEER),
    I(FA_BELL),
    I(FA_BELL_SLASH),
    I(FA_BEZIER_CURVE),
    I(FA_BIBLE),
    I(FA_BICYCLE),
    I(FA_BIKING),
    I(FA_BINOCULARS),
    I(FA_BIOHAZARD),
    I(FA_BIRTHDAY_CAKE),
    I(FA_BLENDER),
    I(FA_BLENDER_PHONE),
    I(FA_BLIND),
    I(FA_BLOG),
    I(FA_BOLD),
    I(FA_BOLT),
    I(FA_BOMB),
    I(FA_BONE),
    I(FA_BONG),
    I(FA_BOOK),
    I(FA_BOOK_DEAD),
    I(FA_BOOK_MEDICAL),
    I(FA_BOOK_OPEN),
    I(FA_BOOK_READER),
    I(FA_BOOKMARK),
    I(FA_BORDER_ALL),
    I(FA_BORDER_NONE),
    I(FA_BORDER_STYLE),
    I(FA_BOWLING_BALL),
    I(FA_BOX),
    I(FA_BOX_OPEN),
    I(FA_BOXES),
    I(FA_BRAILLE),
    I(FA_BRAIN),
    I(FA_BREAD_SLICE),
    I(FA_BRIEFCASE),
    I(FA_BRIEFCASE_MEDICAL),
    I(FA_BROADCAST_TOWER),
    I(FA_BROOM),
    I(FA_BRUSH),
    I(FA_BUG),
    I(FA_BUILDING),
    I(FA_BULLHORN),
    I(FA_BULLSEYE),
    I(FA_BURN),
    I(FA_BUS),
    I(FA_BUS_ALT),
    I(FA_BUSINESS_TIME),
    I(FA_CALCULATOR),
    I(FA_CALENDAR),
    I(FA_CALENDAR_ALT),
    I(FA_CALENDAR_CHECK),
    I(FA_CALENDAR_DAY),
    I(FA_CALENDAR_MINUS),
    I(FA_CALENDAR_PLUS),
    I(FA_CALENDAR_TIMES),
    I(FA_CALENDAR_WEEK),
    I(FA_CAMERA),
    I(FA_CAMERA_RETRO),
    I(FA_CAMPGROUND),
    I(FA_CANDY_CANE),
    I(FA_CANNABIS),
    I(FA_CAPSULES),
    I(FA_CAR),
    I(FA_CAR_ALT),
    I(FA_CAR_BATTERY),
    I(FA_CAR_CRASH),
    I(FA_CAR_SIDE),
    I(FA_CARET_DOWN),
    I(FA_CARET_LEFT),
    I(FA_CARET_RIGHT),
    I(FA_CARET_SQUARE_DOWN),
    I(FA_CARET_SQUARE_LEFT),
    I(FA_CARET_SQUARE_RIGHT),
    I(FA_CARET_SQUARE_UP),
    I(FA_CARET_UP),
    I(FA_CARROT),
    I(FA_CART_ARROW_DOWN),
    I(FA_CART_PLUS),
    I(FA_CASH_REGISTER),
    I(FA_CAT),
    I(FA_CERTIFICATE),
    I(FA_CHAIR),
    I(FA_CHALKBOARD),
    I(FA_CHALKBOARD_TEACHER),
    I(FA_CHARGING_STATION),
    I(FA_CHART_AREA),
    I(FA_CHART_BAR),
    I(FA_CHART_LINE),
    I(FA_CHART_PIE),
    I(FA_CHECK),
    I(FA_CHECK_CIRCLE),
    I(FA_CHECK_DOUBLE),
    I(FA_CHECK_SQUARE),
    I(FA_CHEESE),
    I(FA_CHESS),
    I(FA_CHESS_BISHOP),
    I(FA_CHESS_BOARD),
    I(FA_CHESS_KING),
    I(FA_CHESS_KNIGHT),
    I(FA_CHESS_PAWN),
    I(FA_CHESS_QUEEN),
    I(FA_CHESS_ROOK),
    I(FA_CHEVRON_CIRCLE_DOWN),
    I(FA_CHEVRON_CIRCLE_LEFT),
    I(FA_CHEVRON_CIRCLE_RIGHT),
    I(FA_CHEVRON_CIRCLE_UP),
    I(FA_CHEVRON_DOWN),
    I(FA_CHEVRON_LEFT),
    I(FA_CHEVRON_RIGHT),
    I(FA_CHEVRON_UP),
    I(FA_CHILD),
    I(FA_CHURCH),
    I(FA_CIRCLE),
    I(FA_CIRCLE_NOTCH),
    I(FA_CITY),
    I(FA_CLINIC_MEDICAL),
    I(FA_CLIPBOARD),
    I(FA_CLIPBOARD_CHECK),
    I(FA_CLIPBOARD_LIST),
    I(FA_CLOCK),
    I(FA_CLONE),
    I(FA_CLOSED_CAPTIONING),
    I(FA_CLOUD),
    I(FA_CLOUD_DOWNLOAD_ALT),
    I(FA_CLOUD_MEATBALL),
    I(FA_CLOUD_MOON),
    I(FA_CLOUD_MOON_RAIN),
    I(FA_CLOUD_RAIN),
    I(FA_CLOUD_SHOWERS_HEAVY),
    I(FA_CLOUD_SUN),
    I(FA_CLOUD_SUN_RAIN),
    I(FA_CLOUD_UPLOAD_ALT),
    I(FA_COCKTAIL),
    I(FA_CODE),
    I(FA_CODE_BRANCH),
    I(FA_COFFEE),
    I(FA_COG),
    I(FA_COGS),
    I(FA_COINS),
    I(FA_COLUMNS),
    I(FA_COMMENT),
    I(FA_COMMENT_ALT),
    I(FA_COMMENT_DOLLAR),
    I(FA_COMMENT_DOTS),
    I(FA_COMMENT_MEDICAL),
    I(FA_COMMENT_SLASH),
    I(FA_COMMENTS),
    I(FA_COMMENTS_DOLLAR),
    I(FA_COMPACT_DISC),
    I(FA_COMPASS),
    I(FA_COMPRESS),
    I(FA_COMPRESS_ARROWS_ALT),
    I(FA_CONCIERGE_BELL),
    I(FA_COOKIE),
    I(FA_COOKIE_BITE),
    I(FA_COPY),
    I(FA_COPYRIGHT),
    I(FA_COUCH),
    I(FA_CREDIT_CARD),
    I(FA_CROP),
    I(FA_CROP_ALT),
    I(FA_CROSS),
    I(FA_CROSSHAIRS),
    I(FA_CROW),
    I(FA_CROWN),
    I(FA_CRUTCH),
    I(FA_CUBE),
    I(FA_CUBES),
    I(FA_CUT),
    I(FA_DATABASE),
    I(FA_DEAF),
    I(FA_DEMOCRAT),
    I(FA_DESKTOP),
    I(FA_DHARMACHAKRA),
    I(FA_DIAGNOSES),
    I(FA_DICE),
    I(FA_DICE_D20),
    I(FA_DICE_D6),
    I(FA_DICE_FIVE),
    I(FA_DICE_FOUR),
    I(FA_DICE_ONE),
    I(FA_DICE_SIX),
    I(FA_DICE_THREE),
    I(FA_DICE_TWO),
    I(FA_DIGITAL_TACHOGRAPH),
    I(FA_DIRECTIONS),
    I(FA_DIVIDE),
    I(FA_DIZZY),
    I(FA_DNA),
    I(FA_DOG),
    I(FA_DOLLAR_SIGN),
    I(FA_DOLLY),
    I(FA_DOLLY_FLATBED),
    I(FA_DONATE),
    I(FA_DOOR_CLOSED),
    I(FA_DOOR_OPEN),
    I(FA_DOT_CIRCLE),
    I(FA_DOVE),
    I(FA_DOWNLOAD),
    I(FA_DRAFTING_COMPASS),
    I(FA_DRAGON),
    I(FA_DRAW_POLYGON),
    I(FA_DRUM),
    I(FA_DRUM_STEELPAN),
    I(FA_DRUMSTICK_BITE),
    I(FA_DUMBBELL),
    I(FA_DUMPSTER),
    I(FA_DUMPSTER_FIRE),
    I(FA_DUNGEON),
    I(FA_EDIT),
    I(FA_EGG),
    I(FA_EJECT),
    I(FA_ELLIPSIS_H),
    I(FA_ELLIPSIS_V),
    I(FA_ENVELOPE),
    I(FA_ENVELOPE_OPEN),
    I(FA_ENVELOPE_OPEN_TEXT),
    I(FA_ENVELOPE_SQUARE),
    I(FA_EQUALS),
    I(FA_ERASER),
    I(FA_ETHERNET),
    I(FA_EURO_SIGN),
    I(FA_EXCHANGE_ALT),
    I(FA_EXCLAMATION),
    I(FA_EXCLAMATION_CIRCLE),
    I(FA_EXCLAMATION_TRIANGLE),
    I(FA_EXPAND),
    I(FA_EXPAND_ARROWS_ALT),
    I(FA_EXTERNAL_LINK_ALT),
    I(FA_EXTERNAL_LINK_SQUARE_ALT),
    I(FA_EYE),
    I(FA_EYE_DROPPER),
    I(FA_EYE_SLASH),
    I(FA_FAN),
    I(FA_FAST_BACKWARD),
    I(FA_FAST_FORWARD),
    I(FA_FAX),
    I(FA_FEATHER),
    I(FA_FEATHER_ALT),
    I(FA_FEMALE),
    I(FA_FIGHTER_JET),
    I(FA_FILE),
    I(FA_FILE_ALT),
    I(FA_FILE_ARCHIVE),
    I(FA_FILE_AUDIO),
    I(FA_FILE_CODE),
    I(FA_FILE_CONTRACT),
    I(FA_FILE_CSV),
    I(FA_FILE_DOWNLOAD),
    I(FA_FILE_EXCEL),
    I(FA_FILE_EXPORT),
    I(FA_FILE_IMAGE),
    I(FA_FILE_IMPORT),
    I(FA_FILE_INVOICE),
    I(FA_FILE_INVOICE_DOLLAR),
    I(FA_FILE_MEDICAL),
    I(FA_FILE_MEDICAL_ALT),
    I(FA_FILE_PDF),
    I(FA_FILE_POWERPOINT),
    I(FA_FILE_PRESCRIPTION),
    I(FA_FILE_SIGNATURE),
    I(FA_FILE_UPLOAD),
    I(FA_FILE_VIDEO),
    I(FA_FILE_WORD),
    I(FA_FILL),
    I(FA_FILL_DRIP),
    I(FA_FILM),
    I(FA_FILTER),
    I(FA_FINGERPRINT),
    I(FA_FIRE),
    I(FA_FIRE_ALT),
    I(FA_FIRE_EXTINGUISHER),
    I(FA_FIRST_AID),
    I(FA_FISH),
    I(FA_FIST_RAISED),
    I(FA_FLAG),
    I(FA_FLAG_CHECKERED),
    I(FA_FLAG_USA),
    I(FA_FLASK),
    I(FA_FLUSHED),
    I(FA_FOLDER),
    I(FA_FOLDER_MINUS),
    I(FA_FOLDER_OPEN),
    I(FA_FOLDER_PLUS),
    I(FA_FONT),
    I(FA_FONT_AWESOME_LOGO_FULL),
    I(FA_FOOTBALL_BALL),
    I(FA_FORWARD),
    I(FA_FROG),
    I(FA_FROWN),
    I(FA_FROWN_OPEN),
    I(FA_FUNNEL_DOLLAR),
    I(FA_FUTBOL),
    I(FA_GAMEPAD),
    I(FA_GAS_PUMP),
    I(FA_GAVEL),
    I(FA_GEM),
    I(FA_GENDERLESS),
    I(FA_GHOST),
    I(FA_GIFT),
    I(FA_GIFTS),
    I(FA_GLASS_CHEERS),
    I(FA_GLASS_MARTINI),
    I(FA_GLASS_MARTINI_ALT),
    I(FA_GLASS_WHISKEY),
    I(FA_GLASSES),
    I(FA_GLOBE),
    I(FA_GLOBE_AFRICA),
    I(FA_GLOBE_AMERICAS),
    I(FA_GLOBE_ASIA),
    I(FA_GLOBE_EUROPE),
    I(FA_GOLF_BALL),
    I(FA_GOPURAM),
    I(FA_GRADUATION_CAP),
    I(FA_GREATER_THAN),
    I(FA_GREATER_THAN_EQUAL),
    I(FA_GRIMACE),
    I(FA_GRIN),
    I(FA_GRIN_ALT),
    I(FA_GRIN_BEAM),
    I(FA_GRIN_BEAM_SWEAT),
    I(FA_GRIN_HEARTS),
    I(FA_GRIN_SQUINT),
    I(FA_GRIN_SQUINT_TEARS),
    I(FA_GRIN_STARS),
    I(FA_GRIN_TEARS),
    I(FA_GRIN_TONGUE),
    I(FA_GRIN_TONGUE_SQUINT),
    I(FA_GRIN_TONGUE_WINK),
    I(FA_GRIN_WINK),
    I(FA_GRIP_HORIZONTAL),
    I(FA_GRIP_LINES),
    I(FA_GRIP_LINES_VERTICAL),
    I(FA_GRIP_VERTICAL),
    I(FA_GUITAR),
    I(FA_H_SQUARE),
    I(FA_HAMBURGER),
    I(FA_HAMMER),
    I(FA_HAMSA),
    I(FA_HAND_HOLDING),
    I(FA_HAND_HOLDING_HEART),
    I(FA_HAND_HOLDING_USD),
    I(FA_HAND_LIZARD),
    I(FA_HAND_MIDDLE_FINGER),
    I(FA_HAND_PAPER),
    I(FA_HAND_PEACE),
    I(FA_HAND_POINT_DOWN),
    I(FA_HAND_POINT_LEFT),
    I(FA_HAND_POINT_RIGHT),
    I(FA_HAND_POINT_UP),
    I(FA_HAND_POINTER),
    I(FA_HAND_ROCK),
    I(FA_HAND_SCISSORS),
    I(FA_HAND_SPOCK),
    I(FA_HANDS),
    I(FA_HANDS_HELPING),
    I(FA_HANDSHAKE),
    I(FA_HANUKIAH),
    I(FA_HARD_HAT),
    I(FA_HASHTAG),
    I(FA_HAT_WIZARD),
    I(FA_HAYKAL),
    I(FA_HDD),
    I(FA_HEADING),
    I(FA_HEADPHONES),
    I(FA_HEADPHONES_ALT),
    I(FA_HEADSET),
    I(FA_HEART),
    I(FA_HEART_BROKEN),
    I(FA_HEARTBEAT),
    I(FA_HELICOPTER),
    I(FA_HIGHLIGHTER),
    I(FA_HIKING),
    I(FA_HIPPO),
    I(FA_HISTORY),
    I(FA_HOCKEY_PUCK),
    I(FA_HOLLY_BERRY),
    I(FA_HOME),
    I(FA_HORSE),
    I(FA_HORSE_HEAD),
    I(FA_HOSPITAL),
    I(FA_HOSPITAL_ALT),
    I(FA_HOSPITAL_SYMBOL),
    I(FA_HOT_TUB),
    I(FA_HOTDOG),
    I(FA_HOTEL),
    I(FA_HOURGLASS),
    I(FA_HOURGLASS_END),
    I(FA_HOURGLASS_HALF),
    I(FA_HOURGLASS_START),
    I(FA_HOUSE_DAMAGE),
    I(FA_HRYVNIA),
    I(FA_I_CURSOR),
    I(FA_ICE_CREAM),
    I(FA_ICICLES),
    I(FA_ICONS),
    I(FA_ID_BADGE),
    I(FA_ID_CARD),
    I(FA_ID_CARD_ALT),
    I(FA_IGLOO),
    I(FA_IMAGE),
    I(FA_IMAGES),
    I(FA_INBOX),
    I(FA_INDENT),
    I(FA_INDUSTRY),
    I(FA_INFINITY),
    I(FA_INFO),
    I(FA_INFO_CIRCLE),
    I(FA_ITALIC),
    I(FA_JEDI),
    I(FA_JOINT),
    I(FA_JOURNAL_WHILLS),
    I(FA_KAABA),
    I(FA_KEY),
    I(FA_KEYBOARD),
    I(FA_KHANDA),
    I(FA_KISS),
    I(FA_KISS_BEAM),
    I(FA_KISS_WINK_HEART),
    I(FA_KIWI_BIRD),
    I(FA_LANDMARK),
    I(FA_LANGUAGE),
    I(FA_LAPTOP),
    I(FA_LAPTOP_CODE),
    I(FA_LAPTOP_MEDICAL),
    I(FA_LAUGH),
    I(FA_LAUGH_BEAM),
    I(FA_LAUGH_SQUINT),
    I(FA_LAUGH_WINK),
    I(FA_LAYER_GROUP),
    I(FA_LEAF),
    I(FA_LEMON),
    I(FA_LESS_THAN),
    I(FA_LESS_THAN_EQUAL),
    I(FA_LEVEL_DOWN_ALT),
    I(FA_LEVEL_UP_ALT),
    I(FA_LIFE_RING),
    I(FA_LIGHTBULB),
    I(FA_LINK),
    I(FA_LIRA_SIGN),
    I(FA_LIST),
    I(FA_LIST_ALT),
    I(FA_LIST_OL),
    I(FA_LIST_UL),
    I(FA_LOCATION_ARROW),
    I(FA_LOCK),
    I(FA_LOCK_OPEN),
    I(FA_LONG_ARROW_ALT_DOWN),
    I(FA_LONG_ARROW_ALT_LEFT),
    I(FA_LONG_ARROW_ALT_RIGHT),
    I(FA_LONG_ARROW_ALT_UP),
    I(FA_LOW_VISION),
    I(FA_LUGGAGE_CART),
    I(FA_MAGIC),
    I(FA_MAGNET),
    I(FA_MAIL_BULK),
    I(FA_MALE),
    I(FA_MAP),
    I(FA_MAP_MARKED),
    I(FA_MAP_MARKED_ALT),
    I(FA_MAP_MARKER),
    I(FA_MAP_MARKER_ALT),
    I(FA_MAP_PIN),
    I(FA_MAP_SIGNS),
    I(FA_MARKER),
    I(FA_MARS),
    I(FA_MARS_DOUBLE),
    I(FA_MARS_STROKE),
    I(FA_MARS_STROKE_H),
    I(FA_MARS_STROKE_V),
    I(FA_MASK),
    I(FA_MEDAL),
    I(FA_MEDKIT),
    I(FA_MEH),
    I(FA_MEH_BLANK),
    I(FA_MEH_ROLLING_EYES),
    I(FA_MEMORY),
    I(FA_MENORAH),
    I(FA_MERCURY),
    I(FA_METEOR),
    I(FA_MICROCHIP),
    I(FA_MICROPHONE),
    I(FA_MICROPHONE_ALT),
    I(FA_MICROPHONE_ALT_SLASH),
    I(FA_MICROPHONE_SLASH),
    I(FA_MICROSCOPE),
    I(FA_MINUS),
    I(FA_MINUS_CIRCLE),
    I(FA_MINUS_SQUARE),
    I(FA_MITTEN),
    I(FA_MOBILE),
    I(FA_MOBILE_ALT),
    I(FA_MONEY_BILL),
    I(FA_MONEY_BILL_ALT),
    I(FA_MONEY_BILL_WAVE),
    I(FA_MONEY_BILL_WAVE_ALT),
    I(FA_MONEY_CHECK),
    I(FA_MONEY_CHECK_ALT),
    I(FA_MONUMENT),
    I(FA_MOON),
    I(FA_MORTAR_PESTLE),
    I(FA_MOSQUE),
    I(FA_MOTORCYCLE),
    I(FA_MOUNTAIN),
    I(FA_MOUSE_POINTER),
    I(FA_MUG_HOT),
    I(FA_MUSIC),
    I(FA_NETWORK_WIRED),
    I(FA_NEUTER),
    I(FA_NEWSPAPER),
    I(FA_NOT_EQUAL),
    I(FA_NOTES_MEDICAL),
    I(FA_OBJECT_GROUP),
    I(FA_OBJECT_UNGROUP),
    I(FA_OIL_CAN),
    I(FA_OM),
    I(FA_OTTER),
    I(FA_OUTDENT),
    I(FA_PAGER),
    I(FA_PAINT_BRUSH),
    I(FA_PAINT_ROLLER),
    I(FA_PALETTE),
    I(FA_PALLET),
    I(FA_PAPER_PLANE),
    I(FA_PAPERCLIP),
    I(FA_PARACHUTE_BOX),
    I(FA_PARAGRAPH),
    I(FA_PARKING),
    I(FA_PASSPORT),
    I(FA_PASTAFARIANISM),
    I(FA_PASTE),
    I(FA_PAUSE),
    I(FA_PAUSE_CIRCLE),
    I(FA_PAW),
    I(FA_PEACE),
    I(FA_PEN),
    I(FA_PEN_ALT),
    I(FA_PEN_FANCY),
    I(FA_PEN_NIB),
    I(FA_PEN_SQUARE),
    I(FA_PENCIL_ALT),
    I(FA_PENCIL_RULER),
    I(FA_PEOPLE_CARRY),
    I(FA_PEPPER_HOT),
    I(FA_PERCENT),
    I(FA_PERCENTAGE),
    I(FA_PERSON_BOOTH),
    I(FA_PHONE),
    I(FA_PHONE_ALT),
    I(FA_PHONE_SLASH),
    I(FA_PHONE_SQUARE),
    I(FA_PHONE_SQUARE_ALT),
    I(FA_PHONE_VOLUME),
    I(FA_PHOTO_VIDEO),
    I(FA_PIGGY_BANK),
    I(FA_PILLS),
    I(FA_PIZZA_SLICE),
    I(FA_PLACE_OF_WORSHIP),
    I(FA_PLANE),
    I(FA_PLANE_ARRIVAL),
    I(FA_PLANE_DEPARTURE),
    I(FA_PLAY),
    I(FA_PLAY_CIRCLE),
    I(FA_PLUG),
    I(FA_PLUS),
    I(FA_PLUS_CIRCLE),
    I(FA_PLUS_SQUARE),
    I(FA_PODCAST),
    I(FA_POLL),
    I(FA_POLL_H),
    I(FA_POO),
    I(FA_POO_STORM),
    I(FA_POOP),
    I(FA_PORTRAIT),
    I(FA_POUND_SIGN),
    I(FA_POWER_OFF),
    I(FA_PRAY),
    I(FA_PRAYING_HANDS),
    I(FA_PRESCRIPTION),
    I(FA_PRESCRIPTION_BOTTLE),
    I(FA_PRESCRIPTION_BOTTLE_ALT),
    I(FA_PRINT),
    I(FA_PROCEDURES),
    I(FA_PROJECT_DIAGRAM),
    I(FA_PUZZLE_PIECE),
    I(FA_QRCODE),
    I(FA_QUESTION),
    I(FA_QUESTION_CIRCLE),
    I(FA_QUIDDITCH),
    I(FA_QUOTE_LEFT),
    I(FA_QUOTE_RIGHT),
    I(FA_QURAN),
    I(FA_RADIATION),
    I(FA_RADIATION_ALT),
    I(FA_RAINBOW),
    I(FA_RANDOM),
    I(FA_RECEIPT),
    I(FA_RECYCLE),
    I(FA_REDO),
    I(FA_REDO_ALT),
    I(FA_REGISTERED),
    I(FA_REMOVE_FORMAT),
    I(FA_REPLY),
    I(FA_REPLY_ALL),
    I(FA_REPUBLICAN),
    I(FA_RESTROOM),
    I(FA_RETWEET),
    I(FA_RIBBON),
    I(FA_RING),
    I(FA_ROAD),
    I(FA_ROBOT),
    I(FA_ROCKET),
    I(FA_ROUTE),
    I(FA_RSS),
    I(FA_RSS_SQUARE),
    I(FA_RUBLE_SIGN),
    I(FA_RULER),
    I(FA_RULER_COMBINED),
    I(FA_RULER_HORIZONTAL),
    I(FA_RULER_VERTICAL),
    I(FA_RUNNING),
    I(FA_RUPEE_SIGN),
    I(FA_SAD_CRY),
    I(FA_SAD_TEAR),
    I(FA_SATELLITE),
    I(FA_SATELLITE_DISH),
    I(FA_SAVE),
    I(FA_SCHOOL),
    I(FA_SCREWDRIVER),
    I(FA_SCROLL),
    I(FA_SD_CARD),
    I(FA_SEARCH),
    I(FA_SEARCH_DOLLAR),
    I(FA_SEARCH_LOCATION),
    I(FA_SEARCH_MINUS),
    I(FA_SEARCH_PLUS),
    I(FA_SEEDLING),
    I(FA_SERVER),
    I(FA_SHAPES),
    I(FA_SHARE),
    I(FA_SHARE_ALT),
    I(FA_SHARE_ALT_SQUARE),
    I(FA_SHARE_SQUARE),
    I(FA_SHEKEL_SIGN),
    I(FA_SHIELD_ALT),
    I(FA_SHIP),
    I(FA_SHIPPING_FAST),
    I(FA_SHOE_PRINTS),
    I(FA_SHOPPING_BAG),
    I(FA_SHOPPING_BASKET),
    I(FA_SHOPPING_CART),
    I(FA_SHOWER),
    I(FA_SHUTTLE_VAN),
    I(FA_SIGN),
    I(FA_SIGN_IN_ALT),
    I(FA_SIGN_LANGUAGE),
    I(FA_SIGN_OUT_ALT),
    I(FA_SIGNAL),
    I(FA_SIGNATURE),
    I(FA_SIM_CARD),
    I(FA_SITEMAP),
    I(FA_SKATING),
    I(FA_SKIING),
    I(FA_SKIING_NORDIC),
    I(FA_SKULL),
    I(FA_SKULL_CROSSBONES),
    I(FA_SLASH),
    I(FA_SLEIGH),
    I(FA_SLIDERS_H),
    I(FA_SMILE),
    I(FA_SMILE_BEAM),
    I(FA_SMILE_WINK),
    I(FA_SMOG),
    I(FA_SMOKING),
    I(FA_SMOKING_BAN),
    I(FA_SMS),
    I(FA_SNOWBOARDING),
    I(FA_SNOWFLAKE),
    I(FA_SNOWMAN),
    I(FA_SNOWPLOW),
    I(FA_SOCKS),
    I(FA_SOLAR_PANEL),
    I(FA_SORT),
    I(FA_SORT_ALPHA_DOWN),
    I(FA_SORT_ALPHA_DOWN_ALT),
    I(FA_SORT_ALPHA_UP),
    I(FA_SORT_ALPHA_UP_ALT),
    I(FA_SORT_AMOUNT_DOWN),
    I(FA_SORT_AMOUNT_DOWN_ALT),
    I(FA_SORT_AMOUNT_UP),
    I(FA_SORT_AMOUNT_UP_ALT),
    I(FA_SORT_DOWN),
    I(FA_SORT_NUMERIC_DOWN),
    I(FA_SORT_NUMERIC_DOWN_ALT),
    I(FA_SORT_NUMERIC_UP),
    I(FA_SORT_NUMERIC_UP_ALT),
    I(FA_SORT_UP),
    I(FA_SPA),
    I(FA_SPACE_SHUTTLE),
    I(FA_SPELL_CHECK),
    I(FA_SPIDER),
    I(FA_SPINNER),
    I(FA_SPLOTCH),
    I(FA_SPRAY_CAN),
    I(FA_SQUARE),
    I(FA_SQUARE_FULL),
    I(FA_SQUARE_ROOT_ALT),
    I(FA_STAMP),
    I(FA_STAR),
    I(FA_STAR_AND_CRESCENT),
    I(FA_STAR_HALF),
    I(FA_STAR_HALF_ALT),
    I(FA_STAR_OF_DAVID),
    I(FA_STAR_OF_LIFE),
    I(FA_STEP_BACKWARD),
    I(FA_STEP_FORWARD),
    I(FA_STETHOSCOPE),
    I(FA_STICKY_NOTE),
    I(FA_STOP),
    I(FA_STOP_CIRCLE),
    I(FA_STOPWATCH),
    I(FA_STORE),
    I(FA_STORE_ALT),
    I(FA_STREAM),
    I(FA_STREET_VIEW),
    I(FA_STRIKETHROUGH),
    I(FA_STROOPWAFEL),
    I(FA_SUBSCRIPT),
    I(FA_SUBWAY),
    I(FA_SUITCASE),
    I(FA_SUITCASE_ROLLING),
    I(FA_SUN),
    I(FA_SUPERSCRIPT),
    I(FA_SURPRISE),
    I(FA_SWATCHBOOK),
    I(FA_SWIMMER),
    I(FA_SWIMMING_POOL),
    I(FA_SYNAGOGUE),
    I(FA_SYNC),
    I(FA_SYNC_ALT),
    I(FA_SYRINGE),
    I(FA_TABLE),
    I(FA_TABLE_TENNIS),
    I(FA_TABLET),
    I(FA_TABLET_ALT),
    I(FA_TABLETS),
    I(FA_TACHOMETER_ALT),
    I(FA_TAG),
    I(FA_TAGS),
    I(FA_TAPE),
    I(FA_TASKS),
    I(FA_TAXI),
    I(FA_TEETH),
    I(FA_TEETH_OPEN),
    I(FA_TEMPERATURE_HIGH),
    I(FA_TEMPERATURE_LOW),
    I(FA_TENGE),
    I(FA_TERMINAL),
    I(FA_TEXT_HEIGHT),
    I(FA_TEXT_WIDTH),
    I(FA_TH),
    I(FA_TH_LARGE),
    I(FA_TH_LIST),
    I(FA_THEATER_MASKS),
    I(FA_THERMOMETER),
    I(FA_THERMOMETER_EMPTY),
    I(FA_THERMOMETER_FULL),
    I(FA_THERMOMETER_HALF),
    I(FA_THERMOMETER_QUARTER),
    I(FA_THERMOMETER_THREE_QUARTERS),
    I(FA_THUMBS_DOWN),
    I(FA_THUMBS_UP),
    I(FA_THUMBTACK),
    I(FA_TICKET_ALT),
    I(FA_TIMES),
    I(FA_TIMES_CIRCLE),
    I(FA_TINT),
    I(FA_TINT_SLASH),
    I(FA_TIRED),
    I(FA_TOGGLE_OFF),
    I(FA_TOGGLE_ON),
    I(FA_TOILET),
    I(FA_TOILET_PAPER),
    I(FA_TOOLBOX),
    I(FA_TOOLS),
    I(FA_TOOTH),
    I(FA_TORAH),
    I(FA_TORII_GATE),
    I(FA_TRACTOR),
    I(FA_TRADEMARK),
    I(FA_TRAFFIC_LIGHT),
    I(FA_TRAIN),
    I(FA_TRAM),
    I(FA_TRANSGENDER),
    I(FA_TRANSGENDER_ALT),
    I(FA_TRASH),
    I(FA_TRASH_ALT),
    I(FA_TRASH_RESTORE),
    I(FA_TRASH_RESTORE_ALT),
    I(FA_TREE),
    I(FA_TROPHY),
    I(FA_TRUCK),
    I(FA_TRUCK_LOADING),
    I(FA_TRUCK_MONSTER),
    I(FA_TRUCK_MOVING),
    I(FA_TRUCK_PICKUP),
    I(FA_TSHIRT),
    I(FA_TTY),
    I(FA_TV),
    I(FA_UMBRELLA),
    I(FA_UMBRELLA_BEACH),
    I(FA_UNDERLINE),
    I(FA_UNDO),
    I(FA_UNDO_ALT),
    I(FA_UNIVERSAL_ACCESS),
    I(FA_UNIVERSITY),
    I(FA_UNLINK),
    I(FA_UNLOCK),
    I(FA_UNLOCK_ALT),
    I(FA_UPLOAD),
    I(FA_USER),
    I(FA_USER_ALT),
    I(FA_USER_ALT_SLASH),
    I(FA_USER_ASTRONAUT),
    I(FA_USER_CHECK),
    I(FA_USER_CIRCLE),
    I(FA_USER_CLOCK),
    I(FA_USER_COG),
    I(FA_USER_EDIT),
    I(FA_USER_FRIENDS),
    I(FA_USER_GRADUATE),
    I(FA_USER_INJURED),
    I(FA_USER_LOCK),
    I(FA_USER_MD),
    I(FA_USER_MINUS),
    I(FA_USER_NINJA),
    I(FA_USER_NURSE),
    I(FA_USER_PLUS),
    I(FA_USER_SECRET),
    I(FA_USER_SHIELD),
    I(FA_USER_SLASH),
    I(FA_USER_TAG),
    I(FA_USER_TIE),
    I(FA_USER_TIMES),
    I(FA_USERS),
    I(FA_USERS_COG),
    I(FA_UTENSIL_SPOON),
    I(FA_UTENSILS),
    I(FA_VECTOR_SQUARE),
    I(FA_VENUS),
    I(FA_VENUS_DOUBLE),
    I(FA_VENUS_MARS),
    I(FA_VIAL),
    I(FA_VIALS),
    I(FA_VIDEO),
    I(FA_VIDEO_SLASH),
    I(FA_VIHARA),
    I(FA_VOICEMAIL),
    I(FA_VOLLEYBALL_BALL),
    I(FA_VOLUME_DOWN),
    I(FA_VOLUME_MUTE),
    I(FA_VOLUME_OFF),
    I(FA_VOLUME_UP),
    I(FA_VOTE_YEA),
    I(FA_VR_CARDBOARD),
    I(FA_WALKING),
    I(FA_WALLET),
    I(FA_WAREHOUSE),
    I(FA_WATER),
    I(FA_WAVE_SQUARE),
    I(FA_WEIGHT),
    I(FA_WEIGHT_HANGING),
    I(FA_WHEELCHAIR),
    I(FA_WIFI),
    I(FA_WIND),
    I(FA_WINDOW_CLOSE),
    I(FA_WINDOW_MAXIMIZE),
    I(FA_WINDOW_MINIMIZE),
    I(FA_WINDOW_RESTORE),
    I(FA_WINE_BOTTLE),
    I(FA_WINE_GLASS),
    I(FA_WINE_GLASS_ALT),
    I(FA_WON_SIGN),
    I(FA_WRENCH),
    I(FA_X_RAY),
    I(FA_YEN_SIGN),
    I(FA_YIN_YANG)
};

#undef I

void register_entypo(nb::module_ &m) {
    /* Entypo constants */
    {
        nb::module_ m2 = m.def_submodule("icons");
        for (Icon i: icons)
            m2.attr(i.name) = i.value;
    }
}

#endif