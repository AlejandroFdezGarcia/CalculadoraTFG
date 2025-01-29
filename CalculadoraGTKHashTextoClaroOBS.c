#include <gtk/gtk.h>
#include <openssl/sha.h>
#include <string.h>

#define HASH_COUNT 2
#define SHOW_WIDGET gtk_widget_show_all
#define HIDE_WIDGET gtk_widget_hide
#define GET_TOPLEVEL gtk_widget_get_toplevel

GtkWidget *e1, *w1, *k1;
gdouble n1 = 0.0;
gchar o;

const char *h[HASH_COUNT] = {
    "e922cf413cd25eb8ffed0e5e920cabc4b93abfc4edbe038cb0f1ef64b04d1227",
    "e0bebd22819993425814866b62701e2919ea26f1370499c1037b53b9d49c2c8a"
};

#define PART1 "Error. "
#define PART2 "Intentalo de nuevo."

void c1(const char *i, char o[65]) {
    unsigned char h[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char *)i, strlen(i), h);
    for (int x = 0; x < SHA256_DIGEST_LENGTH; x++) {
        sprintf(o + (x * 2), "%02x", h[x]);
    }
    o[64] = 0;
}

void k_check(GtkButton *b, gpointer d) {
    const gchar *k = gtk_entry_get_text(GTK_ENTRY(k1));
    char kh[65];
    c1(k, kh);

    for (int x = 0; x < HASH_COUNT; x++) {
        if (strcmp(kh, h[x]) == 0) {
            HIDE_WIDGET(GET_TOPLEVEL(GTK_WIDGET(b)));
            SHOW_WIDGET(w1);
            return;
        }
    }

    GtkWidget *dlg = gtk_message_dialog_new(NULL,
                                            GTK_DIALOG_DESTROY_WITH_PARENT,
                                            GTK_MESSAGE_ERROR,
                                            GTK_BUTTONS_OK,
                                            PART1 PART2);
    gtk_dialog_run(GTK_DIALOG(dlg));
    gtk_widget_destroy(dlg);
    gtk_entry_set_text(GTK_ENTRY(k1), "");
}

void n_click(GtkButton *b, gpointer d) {
    const gchar *l = gtk_button_get_label(b);
    const gchar *ct = gtk_entry_get_text(GTK_ENTRY(e1));
    gchar *nt = g_strconcat(ct, l, NULL);
    gtk_entry_set_text(GTK_ENTRY(e1), nt);
    g_free(nt);
}

void o_click(GtkButton *b, gpointer d) {
    const gchar *op = gtk_button_get_label(b);
    n1 = g_ascii_strtod(gtk_entry_get_text(GTK_ENTRY(e1)), NULL);
    o = op[0];
    gtk_entry_set_text(GTK_ENTRY(e1), "");
}

void eq_click(GtkButton *b, gpointer d) {
    gdouble n2 = g_ascii_strtod(gtk_entry_get_text(GTK_ENTRY(e1)), NULL);
    gdouble r = 0.0;

    switch (o) {
        case '+': r = n1 + n2; break;
        case '-': r = n1 - n2; break;
        case '*': r = n1 * n2; break;
        case '/':
            if (n2 != 0) r = n1 / n2;
            else {
                gtk_entry_set_text(GTK_ENTRY(e1), "Error");
                return;
            }
            break;
        default: return;
    }

    gchar rt[50];
    g_snprintf(rt, 50, "%g", r);
    gtk_entry_set_text(GTK_ENTRY(e1), rt);
}

void clr(GtkButton *b, gpointer d) {
    gtk_entry_set_text(GTK_ENTRY(e1), "");
    n1 = 0.0;
    o = 0;
}

void btn(GtkWidget *g, const gchar *l, gint r, gint c, void (*cb)(GtkButton *, gpointer)) {
    GtkWidget *b = gtk_button_new_with_label(l);
    g_signal_connect(b, "clicked", G_CALLBACK(cb), NULL);
    gtk_grid_attach(GTK_GRID(g), b, c, r, 1, 1);
}

void k_window() {
    GtkWidget *w = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(w), "Verificacion");
    gtk_window_set_default_size(GTK_WINDOW(w), 300, 100);
    g_signal_connect(w, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *b = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(w), b);

    k1 = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(k1), "Clave");
    gtk_entry_set_visibility(GTK_ENTRY(k1), FALSE);
    gtk_box_pack_start(GTK_BOX(b), k1, TRUE, TRUE, 0);

    GtkWidget *btn = gtk_button_new_with_label("OK");
    g_signal_connect(btn, "clicked", G_CALLBACK(k_check), NULL);
    gtk_box_pack_start(GTK_BOX(b), btn, TRUE, TRUE, 0);

    SHOW_WIDGET(w);
}

void calc_window() {
    w1 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(w1), "Calc");
    gtk_window_set_default_size(GTK_WINDOW(w1), 250, 300);
    g_signal_connect(w1, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *g = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(w1), g);

    e1 = gtk_entry_new();
    gtk_entry_set_alignment(GTK_ENTRY(e1), 1);
    gtk_grid_attach(GTK_GRID(g), e1, 0, 0, 4, 1);

    btn(g, "7", 1, 0, n_click);
    btn(g, "8", 1, 1, n_click);
    btn(g, "9", 1, 2, n_click);
    btn(g, "4", 2, 0, n_click);
    btn(g, "5", 2, 1, n_click);
    btn(g, "6", 2, 2, n_click);
    btn(g, "1", 3, 0, n_click);
    btn(g, "2", 3, 1, n_click);
    btn(g, "3", 3, 2, n_click);
    btn(g, "0", 4, 0, n_click);

    btn(g, "+", 1, 3, o_click);
    btn(g, "-", 2, 3, o_click);
    btn(g, "*", 3, 3, o_click);
    btn(g, "/", 4, 3, o_click);

    btn(g, "C", 4, 1, clr);
    btn(g, "=", 4, 2, eq_click);

    HIDE_WIDGET(w1);
}

int main(int a, char **v) {
    gtk_init(&a, &v);
    k_window();
    calc_window();
    gtk_main();
    return 0;
}

