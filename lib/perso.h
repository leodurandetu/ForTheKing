/* Structures */

typedef enum {

    BRUTE,
    CHASSEUR,
    ASSASSIN,
    MAGE

} perso_type_t;

typedef struct {

    int x, y;
    int force;
    int sante;
    int intelligence;
    int rapidite;
    int evasion;
    
    perso_type_t type;

} perso_t;