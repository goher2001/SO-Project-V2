#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <mysql.h>
#include <string.h>
#include <pthread.h>
int contador;


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
typedef struct {
	char nombre[20];
}Tnombres;
typedef struct {
	Tnombres nombres[100];
	int num;
}Tlista;
Tlista listaN;
void Usuario (char p[50],Tlista *lista,int i)
{
    i= i+lista->num;
	p = strtok( NULL, "/");
	strcpy (lista->nombres[i].nombre, p);
	lista->num++;
	
}
void DameUsuario (Tlista *lista,char *p[50],char respuesta[200])
{
	int i=0;
	while (i<lista->num)
	{
		sprintf(respuesta, "%s%s//",respuesta,lista->nombres[i].nombre);
		i++;
	}
}
void QuitarUsuario(Tlista *lista, int contadorN)
{
	
	lista->num--;
}
void DameJugadores  (char *p[50],char respuesta[200], MYSQL *conn)
	{
	 MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta[100];
	int err;
	strcpy(consulta, "SELECT nombre From jugador;");
	err = mysql_query(conn, consulta);
	if (err != 0) {
		sprintf(respuesta, "Error al consultar datos de la base %u %s \n",
				mysql_errno(conn), mysql_error(conn));
		
	}
	
	else
		resultado = mysql_store_result(conn);
	row = mysql_fetch_row(resultado);
	strcpy ( respuesta, row[0]);
	row = mysql_fetch_row(resultado);
	while (row != NULL) {
		// la columna 0 contiene el nombre del jugador
		
		sprintf(respuesta, "%s, %s",respuesta, row[0]);
		// obtenemos la siguiente fila
		row = mysql_fetch_row(resultado);
	}
};
void DameGanador  (char *p[50],char respuesta[200], MYSQL *conn)
	{
	char nombre[30];
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	int err;
	p = strtok( NULL, "/");
	strcpy (nombre, p);
	
	char consulta[100];
	sprintf(consulta,"SELECT id,fecha FROM partida WHERE ganador='%s';",nombre);
	
	mysql_query(conn, consulta);
	err = mysql_query(conn, consulta);
	
	resultado = mysql_store_result(conn);
	row = mysql_fetch_row(resultado);
	
	if (row == NULL)
		strcpy(respuesta, "No se han obtenido datos en la consulta\n");
	else {
	     
	    	while (row != NULL)
	    	{
		    	sprintf(respuesta, "%s Partida:%s Fecha: %s", respuesta, row[0], row[1]);
		    	row = mysql_fetch_row(resultado);
	    	}
	}
};
void Consulta3  (char *p[50],char respuesta[200], MYSQL *conn)
	{
	int err;
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta[100];
	strcpy(consulta, "SELECT id,ganador FROM partida;");
	
	err = mysql_query(conn, consulta); 
	if (err != 0) {
		sprintf(respuesta, "Error al consultar datos de la base %u %s\n",
				mysql_errno(conn), mysql_error(conn));
		
	}
	else 
	{
		resultado = mysql_store_result(conn);
		
		row = mysql_fetch_row(resultado);
		
		if (row == NULL)
			strcpy(respuesta, "No se han obtenido datos en la consulta");
		else 
		{
			while (row != NULL)
			{
				sprintf(respuesta, "%sPartida: %s Ganador: %s // ", respuesta, row[0], row[1]);
				row = mysql_fetch_row(resultado);
				
			}
			
		}
	}
};
void Logging (char *p[50],char respuesta[200], MYSQL *conn)
{

	char nombre [30];
	int pwd;
	int err;
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	p = strtok( NULL, "/");
	strcpy (nombre, p);
	// Ya tenemos el nombre
	p = strtok(NULL, "/");
	pwd = atoi(p);
	
	char consulta[100];
	sprintf ( consulta, "SELECT * FROM jugador WHERE nombre='%s' AND contraseña='%d';",nombre, pwd);
	
	err = mysql_query(conn, consulta);
	if (err != 0) {
		strcpy(respuesta, "Error al consultar datos de la base");
			
		
	}
	resultado = mysql_store_result(conn);
	row = mysql_fetch_row(resultado);
	if (row == NULL)
		strcpy(respuesta, "El nombre de usuario y contraseña es incorrecto");
	else
		strcpy(respuesta, "Login correcto");

	
}

void Registro  (char *p[50],char respuesta[200], MYSQL *conn)
	{
	char nombre [30];
	int pwd;
	int err;
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	p = strtok( NULL, "/");
	strcpy (nombre, p);
	// Ya tenemos el nombre
	p = strtok(NULL, "/");
	pwd = atoi(p);
	char consulta[100];
	sprintf (consulta, "INSERT INTO jugador VALUES('%s',%d);",nombre,pwd);
	
	err = mysql_query(conn, consulta);
	if (err != 0) {
		strcpy(respuesta, "Error al introducir datos la base");
		
		
	}
	else{
		
		strcpy(respuesta, "Registro correcto");
	}
};


void *AtenderCliente (void *socket)
{
	int contadorN;
	MYSQL *conn;
	conn = mysql_init(NULL);
	if (conn==NULL){
		printf("Error al crear la conexion: %u %s \n", mysql_errno(conn), mysql_error(conn));
		
	}
	conn = mysql_real_connect (conn, "localhost", "root", "mysql","parchis",0, NULL, 0);
	if (conn==NULL){
		printf ("Error al inicializar la conexion: %u %s \n", mysql_errno(conn), mysql_error(conn));
		
	}
	int sock_conn;
	int *s;
	s= (int *) socket;
	sock_conn = *s;
	
	
	char peticion[512];
	char respuesta[512]= {{'\0'}};
	int ret;
	
	int terminar =0;
	// Entramos en un bucle para atender todas las peticiones de este cliente
	//hasta que se desconecte
	while (terminar ==0)
	{
		// Ahora recibimos la petici?n
		ret=read(sock_conn,peticion, sizeof(peticion));
		printf ("Recibido\n");
		
		// Tenemos que a?adirle la marca de fin de string 
		// para que no escriba lo que hay despues en el buffer
		peticion[ret]='\0';
		
		
		printf ("Peticion: %s\n",peticion);
		
		// vamos a ver que quieren
		char *p = strtok( peticion, "/");
		
		int codigo =  atoi (p);
		// Ya tenemos el c?digo de la petici?n
		char nombre[20];
		int pwd;
		int pwds;
		
		if (codigo == 0) //petici?n de desconexi?n
		{	
			QuitarUsuario(&listaN,contadorN);
			terminar = 1;
		}
		if (codigo == 1)
		{//consulta de pol
			DameJugadores  (p, respuesta, conn);
		}
		
		
		if (codigo == 2) //consulta lluc
		{
			DameGanador (p, respuesta,conn);
		}
		
		if (codigo ==3) //consulta de christian
		{
			
			Consulta3 (p, respuesta,conn);
			
		}
		
		if (codigo == 4)
		{ 
			Logging (p, respuesta,conn);
	    	
		}
		if (codigo==5)//registro
		{
			Registro (p, respuesta,conn);
		}
		if (codigo==6)
		{
			sprintf (respuesta,"%d",contador);
		}
		if(codigo==7)
		{	
			Usuario (p,&listaN,contadorN);
		}
		if (codigo==8)
		{
			DameUsuario(&listaN,p,respuesta);
		}
		if((codigo==1)||(codigo==2)||(codigo==3)||(codigo==4)||(codigo==5)||(codigo==8))
		{
			pthread_mutex_lock( &mutex);
			contador++;
			pthread_mutex_unlock( &mutex);
		}
		if (codigo !=0)
		{
			
			printf ("Respuesta: %s\n", respuesta);
		
			write (sock_conn,respuesta, strlen(respuesta));
			
			memset(respuesta,'\0',strlen(respuesta));
			
		}
		
	}
	// Se acabo el servicio para este cliente
	close(sock_conn); 
}
int main(int argc, char *argv[])
{ 
	 contador =0;
	int sock_conn, sock_listen;
	struct sockaddr_in serv_adr;
	if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		printf ("Error creant socket");
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family= AF_INET;
	serv_adr.sin_port=htons(9400);
	if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0)
		printf ("Error al bind");
	
	if (listen(sock_listen, 3) < 0)
		printf ("Error en el listen");
	
	int i;
	int sockets[100];
	pthread_t thread[100];
	i=0;
	for (;;){
		printf ("Eescuchando\n");
		sock_conn = accept(sock_listen, NULL, NULL);
		printf ("He recibido conexion\n");
		sockets[i]=sock_conn;
		pthread_create (&thread[i], NULL, AtenderCliente,&sockets[i]);
		i=i+1;
	}
}

