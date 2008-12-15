/* csmile-gtk-ui.h - Implementation of the user interface with GTK+
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

#ifndef _CSMILE_GTK_UI_H_
#define _CSMILE_GTK_UI_H_

#include <gtk/gtk.h>
#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define CSMILE_TYPE_GTK_WINDOW                        \
        (csmile_gtk_window_get_type ())
#define CSMILE_GTK_WINDOW(obj)                        \
        (G_TYPE_CHECK_INSTANCE_CAST ((obj), CSMILE_TYPE_GTK_WINDOW, CsmileGtkWindow))
#define CSMILE_GTK_WINDOW_CLASS(klass)                  \
        (G_TYPE_CHECK_CLASS_CAST ((klass), CSMILE_TYPE_GTK_WINDOW, CsmileGtkWindowClass))
#define CSMILE_IS_GTK_WINDOW(obj)                       \
        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CSMILE_TYPE_GTK_WINDOW))
#define CSMILE_IS_GTK_WINDOW_CLASS(klass)               \
        (G_TYPE_CHECK_CLASS_TYPE ((klass), CSMILE_TYPE_GTK_WINDOW))
#define CSMILE_GTK_WINDOW_GET_CLASS(obj)                \
        (G_TYPE_INSTANCE_GET_CLASS ((obj), CSMILE_TYPE_GTK_WINDOW, CsmileGtkWindowClass))

typedef struct _CsmileGtkWindow CsmileGtkWindow;
typedef struct _CsmileGtkWindowClass CsmileGtkWindowClass;
typedef struct _CsmileGtkWindowPriv CsmileGtkWindowPriv;

/*
 * CsmileGtkWindow
 */
struct _CsmileGtkWindow
{
#ifdef USE_HILDON_WINDOW
    HildonWindow parent;
#else
    GtkWindow parent;
#endif
    /* Private */
    CsmileGtkWindowPriv *priv;
};

struct _CsmileGtkWindowClass
{

#ifdef USE_HILDON_WINDOW
    HildonWindowClass parent_class;
#else
    GtkWindowClass parent_class;
#endif
    /* methods */
};

GType csmile_gtk_window_get_type (void);

CsmileGtkWindow* csmile_gtk_window_new ();

void csmile_gtk_window_destroy(CsmileGtkWindow *self);

G_END_DECLS

#endif /* _CSMILE_GTK_WINDOW_H_ */
