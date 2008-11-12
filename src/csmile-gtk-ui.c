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

typedef struct _CsmileGtkWindowPriv CsmileGtkWindowPriv;
struct _CsmileGtkWindowPriv
{
    GtkWidget *window;
    GtkWidget *hbox;
    GtkWidget *video_vbox;
    GtkWidget *button_vbox;
    GtkWidget *video;
    GtkWidget *button_shot;

};

#define CSMILE_GTK_WINDOW_GET_PRIVATE(obj)  \
        (G_TYPE_INSTANCE_GET_PRIVATE ((obj), CSMILE_TYPE_GTK_WINDOW, CsmileGtkWindowPriv))

G_DEFINE_TYPE (CsmileGtkWindow, csmile_gtk_window, GTK_TYPE_WINDOW)
/*
static void
csmile_gtk_window_finalize (Gobject *object)
{
    (*G_OBJECT_CLASS (csmile_gtk_window_parent_class)->finalize) (object);

    g_debug("Destroying main window ...");

    g_signal_handlers_destroy(object);
    
    return;
}
*/

/*
static void
csmile_gtk_window_dispose (GObject* object)
{
    CsmileGtkWindow* self;
    (*G_OBJECT_CLASS (csmile_gtk_window_parent_class)->dispose) (object);
 
    self = CSMILE_GTK_WINDOW (object);
    
    if (self->parent != NULL)
    {*/
        /*g_object_unref (self->parent);*/
      /*  self->parent = NULL;
    }
    
    return;
}
*/
static void
csmile_gtk_window_class_init (CsmileGtkWindowClass* klass)
{
    GObjectClass* object_class;

    object_class = G_OBJECT_CLASS (klass);
        
    g_type_class_add_private (object_class, sizeof (CsmileGtkWindowPriv));
    /*
    object_class->get_property = csmile_gtk_window_get_property;
    object_class->set_property = csmile_gtk_window_set_property;  
    */
    /*object_class->dispose   = csmile_gtk_window_dispose;*/

    /*object_class->finalize   = csmile_gtk_window_finalize;*/
    /*
    klass->clean    = csmile_gtk_window_clean_default;
    */
        
    return;
}

static void
video_expose_event_cb ()
{
}

static void
csmile_gtk_window_init (CsmileGtkWindow* self)
{
    CsmileGtkWindowPriv* priv;
    priv = CSMILE_GTK_WINDOW_GET_PRIVATE (self);
     
    priv->window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_size_request (priv->window, 640, 480);
    gtk_window_set_title (GTK_WINDOW (priv->window), "Csmile");
    
    priv->hbox = gtk_hbox_new (FALSE, 0);
    priv->video_vbox = gtk_vbox_new (FALSE, 0);
    priv->button_vbox = gtk_vbox_new (FALSE, 0);

    gtk_box_pack_start (GTK_BOX(priv->hbox), priv->video_vbox, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX(priv->hbox), priv->button_vbox, FALSE, FALSE, 0);

    priv->video = gtk_drawing_area_new ();
    gtk_widget_set_size_request (priv->video, 320, 240);

    gtk_box_pack_start (GTK_BOX (priv->video_vbox), priv->video, FALSE, FALSE, 0);

    priv->button_shot = gtk_button_new_with_label ("Take photo");
    gtk_widget_set_size_request (priv->button_shot, 120, 240);
    gtk_box_pack_start (GTK_BOX(priv->button_vbox), priv->button_shot, FALSE, FALSE, 0);
     
    gtk_container_add (GTK_CONTAINER (priv->window), priv->hbox);

    g_signal_connect (G_OBJECT (priv->window), "delete_event",
                        G_CALLBACK (gtk_main_quit), NULL);
    g_signal_connect (G_OBJECT (priv->video), "expose_event",
                        G_CALLBACK (video_expose_event_cb), NULL);
     
     
    gtk_widget_show_all (priv->window);

    return;
}

void
csmile_gtk_window_destroy (CsmileGtkWindow *self)
{
    g_return_if_fail(CSMILE_IS_GTK_WINDOW(self));
    while (gtk_events_pending()) gtk_main_iteration();
    gtk_widget_destroy(GTK_WIDGET(self));
}           

CsmileGtkWindow*
csmile_gtk_window_new ()
{
    CsmileGtkWindow* retval;

    retval = g_object_new (CSMILE_TYPE_GTK_WINDOW, NULL);

    return retval;
}


