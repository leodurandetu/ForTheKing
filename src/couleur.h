// Codes ANSI pour le texte (foreground)
#define BLACK_TEXT        30
#define DARK_RED_TEXT     31
#define DARK_GREEN_TEXT   32
#define YELLOW_TEXT       33
#define DARK_BLUE_TEXT    34
#define PURPLE_TEXT       35
#define DARK_CYAN_TEXT    36
#define LIGHT_GRAY_TEXT   37

#define GRAY_TEXT         90
#define RED_TEXT          91
#define GREEN_TEXT        92
#define ORANGE_TEXT       93
#define BLUE_TEXT         94
#define MAGENTA_TEXT      95
#define CYAN_TEXT         96
#define WHITE_TEXT        97

#define DEFAULT_TEXT      39   // couleur par défaut du terminal

// Codes ANSI pour le fond (background)
#define BLACK_BG          40
#define DARK_RED_BG       41
#define DARK_GREEN_BG     42
#define YELLOW_BG         43
#define DARK_BLUE_BG      44
#define PURPLE_BG         45
#define DARK_CYAN_BG      46
#define LIGHT_GRAY_BG     47

#define GRAY_BG          100
#define RED_BG           101
#define GREEN_BG         102
#define ORANGE_BG        103
#define BLUE_BG          104
#define MAGENTA_BG       105
#define CYAN_BG          106
#define WHITE_BG         107

#define DEFAULT_BG       49    // fond par défaut du terminal

// Fonctions
void couleur_texte(int code);
void couleur_fond(int code);
void couleur_reset(void);

void couleur(int texte, int fond); // les deux en même temps (pratique)