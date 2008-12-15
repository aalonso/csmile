/* main.c - main function file
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

#include <gtk/gtk.h>
#include <gst/gst.h>
#include "csmile-gtk-ui.h"

static CsmileGtkWindow *csmile = NULL;

gint
main (gint argc, gchar** argv)
{
        
    g_thread_init (NULL);
    gtk_init (&argc, &argv);

    /* Initialize Gstreamer */
    gst_init (&argc, &argv);

    csmile = CSMILE_GTK_WINDOW (csmile_gtk_window_new ());

    g_object_add_weak_pointer (G_OBJECT (csmile), (gpointer) &csmile);

    gtk_main ();

    csmile_gtk_window_destroy (csmile);

    return 0;
}
