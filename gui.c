#include <gtk/gtk.h>
#include <string.h>
#include "riot_api.h"

// Global widgets
GtkWidget *window;
GtkWidget *grid;
GtkWidget *api_key_entry;
GtkWidget *gamename_entry;
GtkWidget *tagline_entry;
GtkWidget *puuid_entry;
GtkWidget *match_id_entry;
GtkWidget *response_textview;

// Function declarations
void get_puuid_callback(GtkWidget *widget, gpointer data);
void get_account_by_puuid_callback(GtkWidget *widget, gpointer data);
void get_match_history_callback(GtkWidget *widget, gpointer data);
void get_match_details_callback(GtkWidget *widget, gpointer data);
void get_active_game_callback(GtkWidget *widget, gpointer data);
void get_champion_masteries_callback(GtkWidget *widget, gpointer data);
void update_textview(const char *text);

void create_gui(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "LoLMetrics v3");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Grid
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);

    // API Key
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("API Key:"), 0, 0, 1, 1);
    api_key_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), api_key_entry, 1, 0, 2, 1);

    // GameName
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("GameName:"), 0, 1, 1, 1);
    gamename_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), gamename_entry, 1, 1, 1, 1);

    // Tagline
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Tagline:"), 2, 1, 1, 1);
    tagline_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), tagline_entry, 3, 1, 1, 1);

    // PUUID
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("PUUID:"), 0, 2, 1, 1);
    puuid_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), puuid_entry, 1, 2, 2, 1);

    // Match ID
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Match ID:"), 0, 3, 1, 1);
    match_id_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), match_id_entry, 1, 3, 2, 1);


    // Buttons
    GtkWidget *get_puuid_button = gtk_button_new_with_label("Get PUUID");
    gtk_grid_attach(GTK_GRID(grid), get_puuid_button, 0, 4, 1, 1);
    g_signal_connect(get_puuid_button, "clicked", G_CALLBACK(get_puuid_callback), NULL);

    GtkWidget *get_account_button = gtk_button_new_with_label("Get Account by PUUID");
    gtk_grid_attach(GTK_GRID(grid), get_account_button, 1, 4, 1, 1);
    g_signal_connect(get_account_button, "clicked", G_CALLBACK(get_account_by_puuid_callback), NULL);

    GtkWidget *get_match_history_button = gtk_button_new_with_label("Get Match History");
    gtk_grid_attach(GTK_GRID(grid), get_match_history_button, 2, 4, 1, 1);
    g_signal_connect(get_match_history_button, "clicked", G_CALLBACK(get_match_history_callback), NULL);

    GtkWidget *get_match_details_button = gtk_button_new_with_label("Get Match Details");
    gtk_grid_attach(GTK_GRID(grid), get_match_details_button, 0, 5, 1, 1);
    g_signal_connect(get_match_details_button, "clicked", G_CALLBACK(get_match_details_callback), NULL);

    GtkWidget *get_active_game_button = gtk_button_new_with_label("Get Active Game");
    gtk_grid_attach(GTK_GRID(grid), get_active_game_button, 1, 5, 1, 1);
    g_signal_connect(get_active_game_button, "clicked", G_CALLBACK(get_active_game_callback), NULL);

    GtkWidget *get_champion_masteries_button = gtk_button_new_with_label("Get Champion Masteries");
    gtk_grid_attach(GTK_GRID(grid), get_champion_masteries_button, 2, 5, 1, 1);
    g_signal_connect(get_champion_masteries_button, "clicked", G_CALLBACK(get_champion_masteries_callback), NULL);


    // Response Text View
    response_textview = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(response_textview), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(response_textview), FALSE);
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scrolled_window), response_textview);
    gtk_grid_attach(GTK_GRID(grid), scrolled_window, 0, 6, 4, 1);


    gtk_widget_show_all(window);
    gtk_main();
}

void update_textview(const char *text) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(response_textview));
    gtk_text_buffer_set_text(buffer, text, -1);
}

void get_puuid_callback(GtkWidget *widget, gpointer data) {
    const char *api_key = gtk_entry_get_text(GTK_ENTRY(api_key_entry));
    const char *gamename = gtk_entry_get_text(GTK_ENTRY(gamename_entry));
    const char *tagline = gtk_entry_get_text(GTK_ENTRY(tagline_entry));

    char *puuid = get_puuid(gamename, tagline, api_key);
    if (puuid) {
        gtk_entry_set_text(GTK_ENTRY(puuid_entry), puuid);
        update_textview(puuid);
        free(puuid);
    } else {
        update_textview("Failed to get PUUID.");
    }
}

void get_account_by_puuid_callback(GtkWidget *widget, gpointer data) {
    const char *api_key = gtk_entry_get_text(GTK_ENTRY(api_key_entry));
    const char *puuid = gtk_entry_get_text(GTK_ENTRY(puuid_entry));

    char *account_info = get_account_by_puuid(puuid, api_key);
    if (account_info) {
        update_textview(account_info);
        free(account_info);
    } else {
        update_textview("Failed to get account info.");
    }
}

void get_match_history_callback(GtkWidget *widget, gpointer data) {
    const char *api_key = gtk_entry_get_text(GTK_ENTRY(api_key_entry));
    const char *puuid = gtk_entry_get_text(GTK_ENTRY(puuid_entry));

    char *match_history = get_match_history(puuid, api_key);
    if (match_history) {
        update_textview(match_history);
        free(match_history);
    } else {
        update_textview("Failed to get match history.");
    }
}

void get_match_details_callback(GtkWidget *widget, gpointer data) {
    const char *api_key = gtk_entry_get_text(GTK_ENTRY(api_key_entry));
    const char *match_id = gtk_entry_get_text(GTK_ENTRY(match_id_entry));

    char *match_details = get_match_details(match_id, api_key);
    if (match_details) {
        update_textview(match_details);
        free(match_details);
    } else {
        update_textview("Failed to get match details.");
    }
}

void get_active_game_callback(GtkWidget *widget, gpointer data) {
    const char *api_key = gtk_entry_get_text(GTK_ENTRY(api_key_entry));
    const char *puuid = gtk_entry_get_text(GTK_ENTRY(puuid_entry));

    char *active_game = get_active_game_by_puuid(puuid, api_key);
    if (active_game) {
        update_textview(active_game);
        free(active_game);
    } else {
        update_textview("Failed to get active game.");
    }
}

void get_champion_masteries_callback(GtkWidget *widget, gpointer data) {
    const char *api_key = gtk_entry_get_text(GTK_ENTRY(api_key_entry));
    const char *puuid = gtk_entry_get_text(GTK_ENTRY(puuid_entry));

    char *champion_masteries = get_champion_masteries_by_puuid(puuid, api_key);
    if (champion_masteries) {
        update_textview(champion_masteries);
        free(champion_masteries);
    } else {
        update_textview("Failed to get champion masteries.");
    }
}