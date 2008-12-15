/* csmile-gst-cam.h - 
 * Copyright (C) 2008 Adrian Alonso Lazcano <aalonso00@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with self program; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */
                    
#ifndef _CSMILE_GST_CAM_H_
#define _CSMILE_GST_CAM_H_

#include <stdlib.h>
#include <string.h>
#include <gst/gst.h>
#include <gtk/gtk.h>
#include <glib.h>
#include <glib-object.h>

/* To be done add support to save photos on a user difened
 * path at the moment save to /tmp
 */
#define SAVE_FOLDER_DEFAULT NULL
#define PHOTO_NAME_DEFAULT "Picture"
#define PHOTO_NAME_SUFFIX_DEFAULT ".jpg"

/* Difine video source and sinks
 * To be done add support to find video source and sink
 * at runtime.
 */

#ifdef USE_HILDON
    #define VIDEO_SRC "v4l2src"
    #define VIDEO_SINK "xvimagesink"
#else
    #define VIDEO_SRC "v4l2src"
    #define VIDEO_SINK "ximagesink"
#endif

G_BEGIN_DECLS

#define CSMILE_TYPE_GST_CAM                        \
        (csmile_gst_cam_get_type ())
#define CSMILE_GST_CAM(obj)                        \
        (G_TYPE_CHECK_INSTANCE_CAST ((obj), CSMILE_TYPE_GST_CAM, CsmileGstCam))
#define CSMILE_GST_CAM_CLASS(klass)                  \
        (G_TYPE_CHECK_CLASS_CAST ((klass), CSMILE_TYPE_GST_CAM, CsmileGstCamClass))
#define CSMILE_IS_GST_CAM(obj)                       \
        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CSMILE_TYPE_GST_CAM))
#define CSMILE_IS_GST_CAM_CLASS(klass)               \
        (G_TYPE_CHECK_CLASS_TYPE ((klass), CSMILE_TYPE_GST_CAM))
#define CSMILE_GST_CAM_GET_CLASS(obj)                \
        (G_TYPE_INSTANCE_GET_CLASS ((obj), CSMILE_TYPE_GST_CAM, CsmileGstCamClass))
         
typedef struct _CsmileGstCam CsmileGstCam;
typedef struct _CsmileGstCamPriv CsmileGstCamPriv;
typedef struct _CsmileGstCamClass CsmileGstCamClass;

struct _CsmileGstCam
{
    GObject parent_instance;
    /* Private */
    CsmileGstCamPriv *priv;
};

struct _CsmileGstCamClass
{
    GObjectClass parent_class;
    /* Methods 
    void (*take_photo) (CsmileGstCam *self);   
    */
};


GType csmile_gst_cam_get_type (void);

/*
 * Method definitions
 */

CsmileGstCam *csmile_gst_cam_new (void);

void csmile_gst_cam_destroy (CsmileGstCam *self);

void csmile_gst_cam_take_photo(CsmileGstCam *self);

void csmile_gst_cam_set_video_sink (CsmileGstCam *self, GtkWidget *screen);
/*
GstElement *csmile_gst_cam_get_video_sink (CsmileGstCam *self);
*/
G_END_DECLS

#endif  /*_CSMILE_GST_CAM_H_ */
