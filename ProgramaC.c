#include <mysql.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {
    MYSQL *conn;
    int err;
    MYSQL_RES *resultado;
    MYSQL_ROW row;
    char username[50];
    char consulta[200];

    // Crear conexión al servidor MySQL
    conn = mysql_init(NULL);
    if (conn == NULL) {
        printf("Error al crear la conexión: %u %s\n", mysql_errno(conn), mysql_error(conn));
        exit(1);
    }

    // Inicializar la conexión con la base de datos
    conn = mysql_real_connect(conn, "localhost", "user", "pass", "game", 0, NULL, 0);
    if (conn == NULL) {
        printf("Error al inicializar la conexión: %u %s\n", mysql_errno(conn), mysql_error(conn));
        exit(1);
    }

    // Pedir el nombre del jugador
    printf("Introduce el nombre del jugador: ");
    scanf("%s", username);

    // Construir la consulta SQL
    snprintf(consulta, sizeof(consulta),
             "SELECT Game.GameID, Game.points, Game.fecha, Game.time, Game.duration, Game.winner "
             "FROM Participation "
             "JOIN Game ON Participation.Game = Game.GameID "
             "WHERE Participation.Player = '%s'", username);

    // Ejecutar la consulta
    err = mysql_query(conn, consulta);
    if (err != 0) {
        printf("Error al consultar datos de la base %u %s\n", mysql_errno(conn), mysql_error(conn));
        exit(1);
    }

    // Obtener el resultado
    resultado = mysql_store_result(conn);
    row = mysql_fetch_row(resultado);

    if (row == NULL) {
        printf("El jugador '%s' no ha jugado ninguna partida.\n", username);
    } else {
        printf("\nPartidas jugadas por %s:\n", username);
        printf("----------------------------------------------------\n");
        printf("| ID | Puntos | Fecha       | Hora  | Duración | Ganador |\n");
        printf("----------------------------------------------------\n");
        
        while (row != NULL) {
            printf("| %2s | %6s | %10s | %5s | %8s | %7s |\n",
                   row[0], row[1], row[2], row[3], row[4], row[5]);
            row = mysql_fetch_row(resultado);
        }
        printf("----------------------------------------------------\n");
    }

    // Cerrar la conexión
    mysql_close(conn);
    exit(0);
}
