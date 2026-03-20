#include <stdio.h>
#include <stdlib.h>
#include "../lib/fond_menu.h"

/*
 * Ouvre la video et initialise tout ce qu'il faut pour la lire.
 */
FondMenu *fond_menu_ouvrir(SDL_Renderer *renderer, const char *chemin, int w, int h) {
    FondMenu *f = calloc(1, sizeof(FondMenu));
    if (!f) return NULL;

    /* on essaie d'ouvrir le fichier mp4 */
    if (avformat_open_input(&f->fmt_ctx, chemin, NULL, NULL) < 0) {
        fprintf(stderr, "[fond_menu] Impossible d'ouvrir : %s\n", chemin);
        free(f);
        return NULL;
    }

    /* FFmpeg a besoin de lire un bout du fichier pour connaitre les infos des flux */
    avformat_find_stream_info(f->fmt_ctx, NULL);

    /* on cherche le flux video parmi tous les flux du fichier (audio, video, sous-titres) */
    f->video_stream = -1;
    for (int i = 0; i < (int)f->fmt_ctx->nb_streams; i++) {
        if (f->fmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            f->video_stream = i;
            break;
        }
    }
    if (f->video_stream < 0) {
        fprintf(stderr, "[fond_menu] Aucun flux video trouve\n");
        avformat_close_input(&f->fmt_ctx);
        free(f);
        return NULL;
    }

    /* on recupere les infos du codec (h264) pour un mp4 */
    AVCodecParameters *par = f->fmt_ctx->streams[f->video_stream]->codecpar;
    const AVCodec *codec = avcodec_find_decoder(par->codec_id);
    if (!codec) {
        fprintf(stderr, "[fond_menu] Codec introuvable\n");
        avformat_close_input(&f->fmt_ctx);
        free(f);
        return NULL;
    }

    /* on prepare le decodeur et on l'ouvre */
    f->codec_ctx = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(f->codec_ctx, par);
    avcodec_open2(f->codec_ctx, codec, NULL);

    /* on alloue les frames : une pour la frame brute FFmpeg, une pour la version RGB */
    f->frame     = av_frame_alloc();
    f->frame_rgb = av_frame_alloc();
    f->packet    = av_packet_alloc();

    /* on calcule la taille du buffer RGB et on l'alloue */
    int nb = av_image_get_buffer_size(AV_PIX_FMT_RGB24, w, h, 1);
    f->buffer = (uint8_t *)av_malloc(nb);
    av_image_fill_arrays(f->frame_rgb->data, f->frame_rgb->linesize,
                         f->buffer, AV_PIX_FMT_RGB24, w, h, 1);

    /* le convertisseur de couleurs : FFmpeg decode en YUV, SDL veut du RGB */
    f->sws_ctx = sws_getContext(
        par->width, par->height, f->codec_ctx->pix_fmt,  // source : taille et format de la video
        w, h, AV_PIX_FMT_RGB24,                          // destination : taille de la fenetre en RGB
        SWS_BILINEAR, NULL, NULL, NULL                    // algo de redimensionnement
    );

    /* la texture SDL dans laquelle on va uploader chaque frame */
    f->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24,
                                   SDL_TEXTUREACCESS_STREAMING, w, h);
    if (!f->texture) {
        fprintf(stderr, "[fond_menu] Impossible de creer la texture SDL\n");
        fond_menu_fermer(f);
        return NULL;
    }

    return f;
}

/*
 * Lit et decode la prochaine frame de la video.
 * Si on est a la fin, on reboucle au debut automatiquement.
 * Cette fonction bloque jusqu'a avoir une frame valide.
 */
void fond_menu_update(FondMenu *f) {
    if (!f) return;

    int frame_ok = 0;
    while (!frame_ok) {

        /* on essaie de lire le prochain paquet du fichier */
        if (av_read_frame(f->fmt_ctx, f->packet) < 0) {
            /* av_read_frame renvoie une erreur quand on est a la fin -> on repart au debut */
            av_seek_frame(f->fmt_ctx, f->video_stream, 0, AVSEEK_FLAG_BACKWARD);
            avcodec_flush_buffers(f->codec_ctx); // vide le buffer interne du decodeur
            continue;
        }

        /* le fichier peut contenir de l'audio, on ignore tout ce qui n'est pas video */
        if (f->packet->stream_index != f->video_stream) {
            av_packet_unref(f->packet);
            continue;
        }

        /* on envoie le paquet au decodeur */
        if (avcodec_send_packet(f->codec_ctx, f->packet) < 0) {
            av_packet_unref(f->packet);
            continue;
        }

        /* on recupere la frame decodee */
        if (avcodec_receive_frame(f->codec_ctx, f->frame) == 0) {

            /* conversion YUV -> RGB et redimensionnement a la taille de la fenetre */
            sws_scale(f->sws_ctx,
                      (const uint8_t * const *)f->frame->data, f->frame->linesize,
                      0, f->codec_ctx->height,
                      f->frame_rgb->data, f->frame_rgb->linesize);

            /* on envoie les pixels RGB dans la texture SDL */
            SDL_UpdateTexture(f->texture, NULL,
                              f->frame_rgb->data[0],
                              f->frame_rgb->linesize[0]);
            frame_ok = 1;
        }

        av_packet_unref(f->packet);
    }
}

/* Affiche la texture en fond plein ecran */
void fond_menu_render(FondMenu *f, SDL_Renderer *renderer, int w, int h) {
    if (!f || !f->texture) return;
    SDL_Rect dest = {0, 0, w, h};
    SDL_RenderCopy(renderer, f->texture, NULL, &dest);
}

/* Libere tout dans l'ordre inverse de l'allocation */
void fond_menu_fermer(FondMenu *f) {
    if (!f) return;
    if (f->texture)   SDL_DestroyTexture(f->texture);
    if (f->buffer)    av_free(f->buffer);
    if (f->frame)     av_frame_free(&f->frame);
    if (f->frame_rgb) av_frame_free(&f->frame_rgb);
    if (f->packet)    av_packet_free(&f->packet);
    if (f->sws_ctx)   sws_freeContext(f->sws_ctx);
    if (f->codec_ctx) avcodec_free_context(&f->codec_ctx);
    if (f->fmt_ctx)   avformat_close_input(&f->fmt_ctx);
    free(f);
}
