/* csmile-gst-cam.c - 
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

#include <gdk/gdkx.h>
#include <gst/interfaces/xoverlay.h>

#include "csmile-gst-cam.h"


static gboolean create_jpeg(unsigned char *buffer);
/*typedef struct _CsmileGstCamPriv CsmileGstCamPriv;*/

#define CSMILE_GST_CAM_GET_PRIVATE(obj)  \
        (G_TYPE_INSTANCE_GET_PRIVATE ((obj), CSMILE_TYPE_GST_CAM, CsmileGstCamPriv))

struct _CsmileGstCamPriv
{
    /*GtkWidget *video_sink;*/
    GstElement *pipeline;
    GstElement *cam_src;
    GstElement *screen_sink;
    GstElement *image_sink;
    GstElement *screen_queue;
    GstElement *image_queue;
    GstElement *caps_filter;
    GstElement *image_filter;
    GstElement *tee;
    GstBus *bus;
    guint buffer_cb_id;
};

G_DEFINE_TYPE (CsmileGstCam, csmile_gst_cam, G_TYPE_OBJECT);

   

static void
csmile_gst_cam_bus_cb (GstBus *bus, GstMessage *message, CsmileGstCam *self)
{
    CsmileGstCamPriv *priv;
    gchar *message_str;
    const gchar *message_name;
    GError *error;

    priv = CSMILE_GST_CAM_GET_PRIVATE (self);

    /* Report errors to the console */
    if(GST_MESSAGE_TYPE(message) == GST_MESSAGE_ERROR)
    {
        gst_message_parse_error (message, &error, &message_str);
        g_error ("GST Error: %s\n", message_str);
        g_free(error);
        g_free(message_str);
    }
     
    /* Report warnings to the console */
    if(GST_MESSAGE_TYPE(message) == GST_MESSAGE_WARNING)
    {
        gst_message_parse_warning(message, &error, &message_str);
        g_warning("GST Warning: %s\n", message_str);
        g_free(error);
        g_free(message_str);
    }
     
    /* See if the message type is GST_MESSAGE_APPLICATION which means thet the message
     * is sent by the client code (this program) and not by gstreamer. */
    
    if(GST_MESSAGE_TYPE(message) == GST_MESSAGE_APPLICATION)
    {
        /* Get name of the message's structure */
        message_name = gst_structure_get_name(gst_message_get_structure(message));
        
        /* The hildon banner must be shown in here, because the bus callback is
         * called in the main thread and calling GUI-functions in gstreamer threads
         * usually leads to problems with X-server 
         */
 
        /* "photo-taken" message means that the photo was succefully taken
         * and saved and message is shown to user 
         */
        if(!strcmp(message_name, "photo-taken"))
        {
            /*
            hildon_banner_show_information (GTK_WIDGET(priv->window),
                                            NULL, "Photo taken");
            */
        }
         
        /* "photo-failed" means that the photo couldn't be captured or saved */
        if(!strcmp(message_name, "photo-failed"))
        {
            /*
            hildon_banner_show_information(GTK_WIDGET(priv->window),
                                            "gtk-dialog-error", "Saving photo failed");
            */
        }
    }

    return;
}

static gboolean
csmile_gst_cam_buffer_probe_cb (GstElement *image_sink, GstElement *buffer,
                                GstPad *pad, CsmileGstCam *self)
{
    CsmileGstCamPriv *priv;
    GstMessage *message;
    gchar *message_name;

    priv = CSMILE_GST_CAM_GET_PRIVATE (self);
    
    /* This is the raw RGB-data that image sink is about to discard */
    unsigned char *data_photo;
    data_photo = (unsigned char *) GST_BUFFER_DATA (buffer);
                     
    /* Create a JPEG of the data and check the status */
    if(!create_jpeg(data_photo))
        message_name = "photo-failed";
    else
        message_name = "photo-taken";
                           
    /* Disconnect the handler so no more photos are taken */
    g_signal_handler_disconnect(G_OBJECT(priv->image_sink), priv->buffer_cb_id);
                        
    /* Create and send an application message which will be catched in the bus 
     * watcher function. This has to be sent as a message because this callback
     * is called in a gstreamer thread and calling GUI-functions here would
     * lead to X-server synchronization problems 
     */
    
    message = gst_message_new_application(GST_OBJECT(priv->pipeline), gst_structure_new(message_name, NULL));
    gst_element_post_message(priv->pipeline, message);
                                        
    /* Returning TRUE means that the buffer can is OK to be sent forward. 
     * When using fakesink this doesn't really matter because the data is discarded anyway */
    return TRUE;
}

static void
csmile_gst_cam_expose_cb (GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
    gst_x_overlay_set_xwindow_id(GST_X_OVERLAY(data),
                                 GDK_WINDOW_XWINDOW(widget->window));

    return;
}

static void
csmile_gst_cam_finalize (GObject *object)
{
    CsmileGstCam *self;
    CsmileGstCamPriv *priv;
    g_return_if_fail (CSMILE_IS_GST_CAM(object));

    self = CSMILE_GST_CAM (object);
    priv = CSMILE_GST_CAM_GET_PRIVATE (self);

    /* Free and unref the pipeline */
    gst_element_set_state (priv->pipeline, GST_STATE_NULL);
    gst_object_unref (GST_OBJECT(priv->pipeline));

    return;
}

static void
csmile_gst_cam_class_init (CsmileGstCamClass* klass)
{
    GObjectClass* object_class;

    object_class = G_OBJECT_CLASS (klass);

    object_class->finalize = csmile_gst_cam_finalize;
    
    g_type_class_add_private (object_class, sizeof (CsmileGstCamPriv));

    /* Methods 
    klass->take_photo = csmile_gst_cam_take_photo (CsmileGstCam *self);
    */
    return;
}

static void
csmile_gst_cam_init (CsmileGstCam *self)
{
    CsmileGstCamPriv *priv;
    GstCaps *caps;

    self->priv = priv = CSMILE_GST_CAM_GET_PRIVATE (self);

    /* Create a pipeline */
    priv->pipeline = gst_pipeline_new ("csmile-cam");

    /* Attach Gstreamer message bus */
    priv->bus = gst_pipeline_get_bus (GST_PIPELINE (priv->pipeline));
    gst_bus_add_watch (priv->bus, (GstBusFunc) csmile_gst_cam_bus_cb, NULL);
    gst_object_unref (priv->bus);

    /* Create elements */
    /* Cam src <- V4Linux */
    priv->cam_src = gst_element_factory_make (VIDEO_SRC, "cam_src");
    /* Screen queue */
    priv->screen_queue = gst_element_factory_make ("queue", "screen_queue");
    /* Screen sink */
    priv->screen_sink = gst_element_factory_make (VIDEO_SINK, "screen_sink");
    /*self->video_sink = priv->screen_sink;*/

    /* Tee */
    priv->tee = gst_element_factory_make ("tee", "tee");
    /* Caps filter */
    priv->caps_filter = gst_element_factory_make ("ffmpegcolorspace", "caps_filter");
    /* Image filter */
    priv->image_filter = gst_element_factory_make ("ffmpegcolorspace", "image_filter");
    /* Image queue */
    priv->image_queue = gst_element_factory_make ("queue", "image_queue");
    /* Image sink*/
    priv->image_sink = gst_element_factory_make ("fakesink", "image_sink");

    /* Check elements created */
    if (!(priv->cam_src && priv->screen_queue && priv->screen_queue && priv->screen_sink &&
          priv->tee && priv->caps_filter && priv->image_filter && priv->image_queue &&
          priv->image_sink))
    {
        g_critical ("Can't create gst elements");
        /*return FALSE;*/
        return;
    }

    /* Set image sink signal to catch a frame */
    g_object_set (G_OBJECT(priv->image_sink), "signal-handoffs", TRUE, NULL);

    /* Add elements to pipeline */
    gst_bin_add_many (GST_BIN(priv->pipeline), priv->cam_src, priv->caps_filter, priv->tee,
                      priv->screen_queue, priv->screen_sink, priv->image_queue, 
                      priv->image_filter, priv->image_sink, NULL);

    /* Raw video format from camera */
    caps = gst_caps_new_simple ("video/x-raw-rgb", "width", G_TYPE_INT, 160,
                                "heigth", G_TYPE_INT, 120, NULL);

    /* Link cam_src -> caps_filter with raw video caps */
    if (!gst_element_link_filtered (priv->cam_src, priv->caps_filter, caps))
    {
       /* return FALSE;*/
       return;
    }

    gst_caps_unref (caps);

    /* Link caps_filter -> tee -> screen_queue -> screen_sink */
    if (!gst_element_link_many (priv->caps_filter, priv->tee, priv->screen_queue,
                                    priv->screen_sink, NULL))
    {
        /*return FALSE;*/
        return;
    }
    
    caps = gst_caps_new_simple ("video/x-raw-rgb", "width", G_TYPE_INT, 160,
                                "height", G_TYPE_INT, 120, "bpp", G_TYPE_INT, 24,
                                "depth", G_TYPE_INT, 24,
                                "framerate", GST_TYPE_FRACTION, 15, 1,
                                NULL);

    /* Link tee -> image_queue -> image_filter */
    if (!gst_element_link_many (priv->tee, priv->image_queue, priv->image_filter, NULL))
        return;
        /*return FALSE;*/
    /* Link image_filter -> image_sink with raw caps */
    if (!gst_element_link_filtered (priv->image_filter, priv->image_sink, caps))
        return;
        /*return FALSE;*/
    
    gst_caps_unref (caps);


    return;
}

CsmileGstCam*
csmile_gst_cam_new ()
{
    return g_object_new (CSMILE_TYPE_GST_CAM, NULL);
}

void
csmile_gst_cam_take_photo (CsmileGstCam *self)
{
    GstElement *image_sink;
    CsmileGstCamPriv *priv;
    
    priv = CSMILE_GST_CAM_GET_PRIVATE (self);

    /* Get image sink from pipeline */
    image_sink = gst_bin_get_by_name (GST_BIN(priv->pipeline), "image_sink");

    /* Connect a hand-off signal */
    priv->buffer_cb_id = g_signal_connect (G_OBJECT(image_sink), "handoff",
                                           G_CALLBACK(csmile_gst_cam_buffer_probe_cb), self);

    return;
}

/* Create jpeg image from raw data
 *
 */
static gboolean
create_jpeg (unsigned char *data)
{
    GdkPixbuf *pixbuf = NULL;
    GError *error = NULL;
    guint height, width, bpp;
    const gchar *directory;
    GString *filename;
    guint base_len, i;
    struct stat statbuf;

    /* Picture size should be user options */
     
    width = 160; height = 120; bpp = 24;
     
    /* Define the save folder */
    directory = SAVE_FOLDER_DEFAULT;
    if (directory == NULL)
    {
        directory = g_get_tmp_dir ();
    }
     
    /* Create an unique file name */
    filename = g_string_new (g_build_filename(directory, PHOTO_NAME_DEFAULT, NULL));
    base_len = filename->len;
    g_string_append (filename, PHOTO_NAME_SUFFIX_DEFAULT);
    
    for(i = 1; !stat (filename->str, &statbuf); ++i)
    {
        g_string_truncate (filename, base_len);
        g_string_append_printf (filename, "%d%s", i, PHOTO_NAME_SUFFIX_DEFAULT);
    }
     
    /* Create a pixbuf object from the data */
    pixbuf = gdk_pixbuf_new_from_data (data, GDK_COLORSPACE_RGB, /* RGB-colorspace */
                                       FALSE,                    /* No alpha-channel */
                                       bpp/3,                    /* Bits per RGB-component */
                                       width, height,            /* Dimensions */
                                       3*width,                  /* Number of bytes between lines (ie stride) */
                                       NULL, NULL);              /* Callbacks */
            
    /* Save the pixbuf content's in to a jpeg file and check for errors */
    if (!gdk_pixbuf_save(pixbuf, filename->str, "jpeg", &error, NULL))
    {
        g_warning("%s\n", error->message);
        g_error_free(error);
        gdk_pixbuf_unref(pixbuf);
        g_string_free(filename, TRUE);
        return FALSE;
    }
     
    /* Free allocated resources and return TRUE which means that the operation was succesful */
    g_string_free(filename, TRUE);
    gdk_pixbuf_unref(pixbuf);
    
    return TRUE;
}

/*
GstElement*
csmile_gst_cam_get_video_sink (CsmileGstCam *self)
{ 
    CsmileGstCamPriv *priv;
    priv = CSMILE_GST_CAM_GET_PRIVATE (self);

    return (priv->screen_sink);
}
*/

void
csmile_gst_cam_set_video_sink (CsmileGstCam *self, GtkWidget *screen)
{
    CsmileGstCamPriv *priv;
    priv = CSMILE_GST_CAM_GET_PRIVATE (self);

    /* Connect expose event */
    
    g_signal_connect (screen, "expose-event", G_CALLBACK(csmile_gst_cam_expose_cb),
                      priv->screen_sink);
    
    gst_element_set_state (priv->pipeline, GST_STATE_PLAYING);

    return;
}


void
csmile_gst_cam_destroy (CsmileGstCam *self)
{
    CsmileGstCamPriv *priv;
    g_return_if_fail (CSMILE_IS_GST_CAM(self));
    
    priv = CSMILE_GST_CAM_GET_PRIVATE (self);
    
    /* Free and unref the pipeline */
    gst_element_set_state (priv->pipeline, GST_STATE_NULL);
    gst_object_unref (GST_OBJECT(priv->pipeline));
     
    return;
}

