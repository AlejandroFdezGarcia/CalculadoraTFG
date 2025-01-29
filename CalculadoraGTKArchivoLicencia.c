#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GtkWidget *entry;
gdouble num1 = 0.0;
gchar operator;

GtkWidget *calc_window;

gboolean check_license() {
    const gchar *license_file = "license.txt";
    FILE *file = fopen(license_file, "r");

    if (file == NULL) {
        return FALSE;
    }

    char buffer[256];
    if (fgets(buffer, sizeof(buffer), file) != NULL) {
        fclose(file);
        if (strcmp(buffer, "LICENCIA_VALIDA\n") == 0) {
            return TRUE;
        }
    }
    fclose(file);
    return FALSE;
}

void on_number_button_clicked(GtkButton *button, gpointer data) {
    const gchar *number = gtk_button_get_label(button);
    const gchar *current_text = gtk_entry_get_text(GTK_ENTRY(entry));
    gchar *new_text = g_strconcat(current_text, number, NULL);
    gtk_entry_set_text(GTK_ENTRY(entry), new_text);
    g_free(new_text);
}

void on_operator_button_clicked(GtkButton *button, gpointer data) {
    const gchar *op = gtk_button_get_label(button);
    num1 = g_ascii_strtod(gtk_entry_get_text(GTK_ENTRY(entry)), NULL);
    operator = op[0];
    gtk_entry_set_text(GTK_ENTRY(entry), "");
}

void on_equals_button_clicked(GtkButton *button, gpointer data) {
    gdouble num2 = g_ascii_strtod(gtk_entry_get_text(GTK_ENTRY(entry)), NULL);
    gdouble result = 0.0;

    switch (operator) {
        case '+': result = num1 + num2; break;
        case '-': result = num1 - num2; break;
        case '*': result = num1 * num2; break;
        case '/':
            if (num2 != 0) result = num1 / num2;
            else {
                gtk_entry_set_text(GTK_ENTRY(entry), "Error");
                return;
            }
            break;
        default: return;
    }

    gchar result_text[50];
    g_snprintf(result_text, 50, "%g", result);
    gtk_entry_set_text(GTK_ENTRY(entry), result_text);
}

void on_clear_button_clicked(GtkButton *button, gpointer data) {
    gtk_entry_set_text(GTK_ENTRY(entry), "");
    num1 = 0.0;
    operator = 0;
}

void create_button(GtkWidget *grid, const gchar *label, gint row, gint col, void (*callback)(GtkButton *, gpointer)) {
    GtkWidget *button = gtk_button_new_with_label(label);
    g_signal_connect(button, "clicked", G_CALLBACK(callback), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, col, row, 1, 1);
}

void create_calculator_window() {
    calc_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(calc_window), "Calculadora");
    gtk_window_set_default_size(GTK_WINDOW(calc_window), 250, 300);
    g_signal_connect(calc_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(calc_window), grid);

    entry = gtk_entry_new();
    gtk_entry_set_alignment(GTK_ENTRY(entry), 1);
    gtk_grid_attach(GTK_GRID(grid), entry, 0, 0, 4, 1);

    create_button(grid, "7", 1, 0, on_number_button_clicked);
    create_button(grid, "8", 1, 1, on_number_button_clicked);
    create_button(grid, "9", 1, 2, on_number_button_clicked);
    create_button(grid, "4", 2, 0, on_number_button_clicked);
    create_button(grid, "5", 2, 1, on_number_button_clicked);
    create_button(grid, "6", 2, 2, on_number_button_clicked);
    create_button(grid, "1", 3, 0, on_number_button_clicked);
    create_button(grid, "2", 3, 1, on_number_button_clicked);
    create_button(grid, "3", 3, 2, on_number_button_clicked);
    create_button(grid, "0", 4, 0, on_number_button_clicked);

    create_button(grid, "+", 1, 3, on_operator_button_clicked);
    create_button(grid, "-", 2, 3, on_operator_button_clicked);
    create_button(grid, "*", 3, 3, on_operator_button_clicked);
    create_button(grid, "/", 4, 3, on_operator_button_clicked);

    create_button(grid, "C", 4, 1, on_clear_button_clicked);
    create_button(grid, "=", 4, 2, on_equals_button_clicked);

    gtk_widget_show_all(calc_window);
}

int main(int argc, char **argv) {
    gtk_init(&argc, &argv);

    if (check_license()) {
        create_calculator_window();
        gtk_main();
    } else {
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "No se encontro un archivo de licencia valido. El programa no puede continuar.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }

    return 0;
}
