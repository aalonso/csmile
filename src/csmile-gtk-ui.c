/* csmile-gtk-ui.c - Implementation of the user interface with GTK+
 * Copyright (C) 2008 Adrian Alonso Lazcano <aalonso00@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with self library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "csmile-gtk-ui.h"
#include "csmile-gst-cam.h"
/*#include "csmile-cam-widget.h"*/
#include <gdk/gdk.h>
#include <gdk/gdkx.h>
#include <gst/interfaces/xoverlay.h>

/*typedef struct _CsmileGtkWindowPriv CsmileGtkWindowPriv;*/
#define CSMILE_GTK_WINDOW_GET_PRIVATE(obj)  \
        (G_TYPE_INSTANCE_GET_PRIVATE ((obj), CSMILE_TYPE_GTK_WINDOW, CsmileGtkWindowPriv))

struct _CsmileGtkWindowPriv
{
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *hbox;
    /*GtkWidget *browse_hbox;*/
    GtkWidget *button_hbox;
    GtkWidget *video;
    GtkWidget *button_shot;
    GtkWidget *button_browse;
    GtkWidget *button_prev;
    GtkWidget *button_next;
    GtkWidget *button_tag;
    GtkWidget *button_delete;
    CsmileGstCam *cam;
};

G_DEFINE_TYPE (CsmileGtkWindow, csmile_gtk_window, GTK_TYPE_WINDOW);

static void
csmile_gtk_window_finalize (GObject *object)
{
    CsmileGtkWindow *self = CSMILE_GTK_WINDOW (object);

    while (gtk_events_pending()) gtk_main_iteration();
    gtk_widget_destroy(GTK_WIDGET(self));

    G_OBJECT_CLASS (csmile_gtk_window_parent_class)->finalize (object);

    g_debug("Destroying main window ...");

    g_signal_handlers_destroy(object);
    
    return;
}

static void
csmile_gtk_window_dispose (GObject* object)
{
    /*
    CsmileGtkWindow* self = CSMILE_GTK_WINDOW (object);
    
    if (self->parent != NULL)
    {
        g_object_unref (self->parent);
        self->parent = NULL;
    }
    */
    G_OBJECT_CLASS (csmile_gtk_window_parent_class)->dispose (object);
    
    return;
}

static void
csmile_gtk_window_class_init (CsmileGtkWindowClass* klass)
{
    GObjectClass* object_class;

    object_class = G_OBJECT_CLASS (klass);
        
    /*
    object_class->get_property = csmile_gtk_window_get_property;
    object_class->set_property = csmile_gtk_window_set_property;  
    */
    object_class->dispose   = csmile_gtk_window_dispose;
    object_class->finalize   = csmile_gtk_window_finalize;
    /*
    klass->clean    = csmile_gtk_window_clean_default;
    */
    g_type_class_add_private (object_class, sizeof (CsmileGtkWindowPriv));
        
    return;
}

static void
shoot_button_clicked_cb (GtkWidget *widget, gpointer data)
{
    g_print ("Shoot me!\n");
    csmile_gst_cam_take_photo (data);
    return;
}

/*
static void
video_expose_event_cb (GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
    gst_x_overlay_set_xwindow_id (GST_X_OVERLAY(data),
                                 GDK_WINDOW_XWINDOW(widget));
    return;
}
*/

static void
csmile_gtk_window_init (CsmileGtkWindow* self)
{
    CsmileGtkWindowPriv* priv;
    self->priv = priv = CSMILE_GTK_WINDOW_GET_PRIVATE (self);
     
    priv->window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_size_request (priv->window, 320, 240);
    gtk_window_set_title (GTK_WINDOW (priv->window), "Csmile");
    
    priv->vbox = gtk_vbox_new (FALSE, 0);
    priv->hbox = gtk_hbox_new (FALSE, 0);
    priv->button_hbox = gtk_hbox_new (FALSE, 0);
    
    priv->video = gtk_drawing_area_new ();
    gtk_widget_set_size_request (priv->video, 160, 120);
    gtk_widget_set_app_paintable (priv->video, TRUE);

    /*
    priv->video = csmile_cam_widget_new ();
    */
    priv->button_shot = gtk_button_new_from_stock (GTK_STOCK_MEDIA_RECORD);
    /*gtk_widget_set_size_request (priv->button_shot, 96, 64);
    gtk_button_set_label (priv->button_shot, "Take photo");*/
    priv->button_browse = gtk_button_new_from_stock (GTK_STOCK_OPEN);
    /*gtk_widget_set_size_request (priv->button_browse, 96, 64);
    gtk_button_set_label (priv->button_browse, "Browse");*/
    priv->button_prev = gtk_button_new_from_stock (GTK_STOCK_GO_BACK);
    /*gtk_widget_set_size_request (priv->button_prev, 96, 64);
    gtk_button_set_label (priv->button_prev, "Previous");*/
    priv->button_next = gtk_button_new_from_stock (GTK_STOCK_GO_FORWARD);
    /*gtk_widget_set_size_request (priv->button_next, 96, 64);
    gtk_button_set_label (priv->button_next, "Next");*/
    priv->button_tag = gtk_button_new_from_stock (GTK_STOCK_EDIT);
    /*gtk_widget_set_size_request (priv->button_tag, 96, 64);
    gtk_button_set_label (priv->button_tag, "Tag photo");*/
    priv->button_delete = gtk_button_new_from_stock (GTK_STOCK_DELETE);
    /*gtk_widget_set_size_request (priv->button_delete, 96, 64);*/

    gtk_box_pack_start (GTK_BOX (priv->vbox), priv->video, TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX (priv->button_hbox), priv->button_shot, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (priv->button_hbox), priv->button_browse, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (priv->button_hbox), priv->button_prev, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (priv->button_hbox), priv->button_next, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (priv->button_hbox), priv->button_tag, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (priv->button_hbox), priv->button_delete, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (priv->vbox), priv->button_hbox, FALSE, FALSE, 0);
    /*
    gtk_box_pack_start (GTK_BOX(priv->hbox), priv->video, FALSE, FALSE, 0);
    */
    gtk_container_add (GTK_CONTAINER (priv->window), priv->vbox);
    
    /* Camera instance */
    priv->cam = csmile_gst_cam_new ();
    /* Set video sink*/
    csmile_gst_cam_set_video_sink (priv->cam, priv->video);
    /*g_object_add_weak_pointer (G_OBJECT (cam), (gpointer) &cam);*/
   
    
    /*XID xid = gdk_x11_drawable_get_xid ( gtk_widget_get_window(priv->video));*/
    /* Connect signal handlers */
    g_signal_connect (G_OBJECT (priv->window), "delete_event",
                        G_CALLBACK (gtk_main_quit), NULL);
    g_signal_connect (G_OBJECT (priv->window), "destroy",
                        G_CALLBACK (gtk_main_quit), NULL);
    g_signal_connect (G_OBJECT (priv->button_shot), "clicked",
                        G_CALLBACK (shoot_button_clicked_cb), priv->cam);
    /*
    g_signal_connect (G_OBJECT (priv->video), "expose_event",
                        G_CALLBACK (video_expose_event_cb), xid); 
    */

    gtk_widget_show_all (priv->window);

    return;
}

void
csmile_gtk_window_destroy (CsmileGtkWindow *self)
{
    CsmileGtkWindowPriv* priv;
    priv = CSMILE_GTK_WINDOW_GET_PRIVATE (self);

    g_return_if_fail(CSMILE_IS_GTK_WINDOW(self));
    while (gtk_events_pending()) gtk_main_iteration();
    /*csmile_gst_cam_destroy (priv->cam);*/
    gtk_widget_destroy(GTK_WIDGET(self));
}           

CsmileGtkWindow*
csmile_gtk_window_new ()
{
    return g_object_new (CSMILE_TYPE_GTK_WINDOW, NULL);
}


