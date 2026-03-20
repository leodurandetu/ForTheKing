#ifndef FOND_MENU_H
#define FOND_MENU_H

#include <SDL2/SDL.h>

/* on a besoin de FFmpeg pour lire la video frame par frame */
#include <libavcodec/avcodec.h>    // decodage video
#include <libavformat/avformat.h>  // lecture du fichier mp4
#include <libavutil/imgutils.h>    // utilitaires pour les images
#include <libswscale/swscale.h>    // conversion de couleurs (YUV -> RGB)

/*
 * Tout ce dont on a besoin pour lire et afficher la video de fond.
 * On garde tout dans une struct pour pas polluer le reste du code.
 */
typedef struct {
    AVFormatContext   *fmt_ctx;      // contexte du fichier video (le "container" mp4)
    AVCodecContext    *codec_ctx;    // contexte du decodeur video
    struct SwsContext *sws_ctx;      // convertisseur YUV -> RGB (FFmpeg travaille en YUV, SDL veut du RGB)
    AVFrame           *frame;        // frame brute decodee par FFmpeg
    AVFrame           *frame_rgb;    // meme frame mais convertie en RGB pour SDL
    AVPacket          *packet;       // paquet lu depuis le fichier avant decodage
    uint8_t           *buffer;       // buffer memoire qui contient les pixels RGB
    int                video_stream; // index du flux video dans le fichier (un mp4 peut avoir audio + video)
    SDL_Texture       *texture;      // texture SDL qu'on upload a chaque frame
} FondMenu;

/*
 * Ouvre le fichier video et prepare tout pour la lecture.
 * A appeler une seule fois au demarrage, avant la boucle principale.
 * Renvoie NULL si le fichier n'existe pas ou si autre erreur.
 */
FondMenu *fond_menu_ouvrir(SDL_Renderer *renderer, const char *chemin, int w, int h);

/*
 * Lit la prochaine frame de la video et met a jour la texture SDL.
 * A appeler a chaque tour de boucle, juste avant fond_menu_render.
 * Quand on arrive a la fin de la video, ça repart automatiquement au debut.
 */
void fond_menu_update(FondMenu *f);

/*
 * Affiche la texture video en fond plein ecran.
 * A appeler apres fond_menu_update, avant de dessiner les boutons.
 */
void fond_menu_render(FondMenu *f, SDL_Renderer *renderer, int w, int h);

/*
 * Libere toute la memoire et ferme le fichier.
 * A appeler quand on quitte le menu.
 */
void fond_menu_fermer(FondMenu *f);

#endif
