#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int detecta () {
	FILE *output = popen("sudo mcu -d", "r");
	if (!output) {
		printf("Error");
	}
	char buffer[1024];
	char *line_p = fgets(buffer, sizeof(buffer), output);
	if (strstr(line_p,"openrc") != NULL) {
		return 2;
	} else if (strstr(line_p,"systemd") != NULL) {
		return 3;
	} else {
		return 4;
	}
	pclose(output);
}

static void ejecuta_apaga () {
	int resultado = detecta();
	if (resultado == 2) {
		//Si es OpenRC
		system("sudo mcu -ao");
	} else if (resultado == 3) {
		//Si es Sytemd
		system("sudo mcu -as");
	} else if (resultado == 4) {
		//Si no se reconoce
		printf("Init no reconocido, probando con 'shutdown'\n");
		system("sudo mcu -ag");
	}
}

static void ejecuta_reinicia () {
	int resultado = detecta();
	if (resultado == 2) {
		//Si es OpenRC
		system("sudo mcu -ro");
	} else if (resultado == 3) {
		//Si es Sytemd
		system("sudo mcu -rs");
	} else if (resultado == 4) {
		//Si no se reconoce
		printf("Init no reconocido, probando con 'shutdown'\n");
		system("sudo mcu -rg");
	}
}

static void cierra_sesion () {
	system("sudo mcu -cs");
}

static void bloquea () {
	system("sudo mcu -b");
}

static void activate (GtkApplication *app,
          gpointer        user_data) {
	GtkWidget *window;
	GtkWidget *button1, *button2, *button3, *button4;
	GtkWidget *button_box, *close, *event_box;

	//Propiedades de la ventana
	window = gtk_application_window_new (app);
	gtk_window_set_title (GTK_WINDOW (window), "Menú de Apagado");
	gtk_window_set_resizable (GTK_WINDOW (window),FALSE);
	gtk_window_set_decorated (GTK_WINDOW (window), FALSE);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	
	//Widgets
	button_box = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);

	button1 = gtk_button_new_with_label ("Apagar");
	button2 = gtk_button_new_with_label ("Reiniciar");
	button3 = gtk_button_new_with_label ("Bloquear");
	button4 = gtk_button_new_with_label ("Cerrar Sesión");
	
	event_box = gtk_event_box_new ();
	
	char user[200];
	strcpy (user,"/home/");
	strcat (user,getenv ("USER"));
	strcat (user,"/.icons/close.png");
	close = gtk_image_new_from_file (user);
	
	gtk_widget_set_size_request (button_box, 600, 120);
	gtk_widget_set_size_request (button1, 100, 100);
	gtk_widget_set_size_request (button2, 100, 100);
	gtk_widget_set_size_request (button3, 100, 100);
	gtk_widget_set_size_request (button4, 100, 100);
	gtk_widget_set_size_request (close, 32, 32);

	//Señales
	g_signal_connect (button1, "clicked", G_CALLBACK (ejecuta_apaga), NULL);
	g_signal_connect_swapped (button1, "clicked", G_CALLBACK (gtk_widget_destroy), window);
	
	g_signal_connect (button2, "clicked", G_CALLBACK (ejecuta_reinicia), NULL);
	g_signal_connect_swapped (button2, "clicked", G_CALLBACK (gtk_widget_destroy), window);
	
	g_signal_connect (button3, "clicked", G_CALLBACK (bloquea), NULL);
	g_signal_connect_swapped (button3, "clicked", G_CALLBACK (gtk_widget_destroy), window);
	
	g_signal_connect (button4, "clicked", G_CALLBACK (cierra_sesion), NULL);
	g_signal_connect_swapped (button4, "clicked", G_CALLBACK (gtk_widget_destroy), window);
	
	g_signal_connect_swapped (event_box, "button_press_event", G_CALLBACK (gtk_widget_destroy), window);

	//Añadir y mostrar los widgets
	gtk_container_add (GTK_CONTAINER (window), button_box);
	gtk_container_add (GTK_CONTAINER (button_box), button1);
	gtk_container_add (GTK_CONTAINER (button_box), button2);
	gtk_container_add (GTK_CONTAINER (button_box), button3);
	gtk_container_add (GTK_CONTAINER (button_box), button4);
	gtk_container_add (GTK_CONTAINER (button_box), event_box);
	gtk_container_add (GTK_CONTAINER (event_box), close);
	gtk_widget_show_all (window);
}

int main (int    argc,
      char **argv) {
	GtkApplication *app;
	int status;
	app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
	g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
	status = g_application_run (G_APPLICATION (app), argc, argv);
	g_object_unref (app);
	return status;
}
