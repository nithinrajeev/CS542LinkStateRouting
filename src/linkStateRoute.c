#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Function Name : printPrompt()
 * Function		 : Prints the menu prompt for the simulator
 * Input		 : void
 * Return		 : void
 */

void printPrompt()
{
	printf("\n\n\tCS542 Link State Routing Simulator");
	printf("\n\t(1) Create a Network Topology");
	printf("\n\t(2) Build a Connection Table");
	printf("\n\t(3) Shortest Path to Destination Router");
	printf("\n\t(4) Modify a topology");
	printf("\n\t(5) Exit");
	printf("\n\n\tCommand : ");
}

/* Function Name : createTopology()
 * Function		 : It asks the user for a topology filename from the
 *				   user and opens the specified file and reads a matrix
 *				   into a 2d matrix and returns the pointer to the matrix
 *				   to the main function and also updates the size of the
 *				   array to the int pointer passed to it.
 * Input		 : int *size
 * Return		 : int**
 */

int** createTopology(int *size)
{
	FILE *input;
	char fileName[256];
	char *line=NULL;
	size_t len = 0;
	ssize_t read;
	const char *s = " ";
	char * token = NULL;
	int i = 0, j=0, flag = 0, row = 0, col = 0,k=0;
	int arr[200];
	printf("\n\n\tInput original network topology matrix data file : ");
	scanf("%s", fileName);
	input=fopen(fileName,"r");
	if(input==NULL)
	{
		printf("\n\tFile does not exit");
		return 0;
	}
	while((read=getline(&line, &len, input))!=-1)
	{
		token = strtok(line, s);
		while(token!=NULL)
		{
			arr[i]=atoi(token);
			token=strtok(NULL,s);
			i++;
		}
		if(flag==0)
		{
			flag=1;
			row=i;
		}
		col++;
	}
	fclose(input);
	if(row!=col)
	{
		(*size)=-1;
		printf("\n\tInvalid input. Matrix assymetrical");
		return 0;
	}
	(*size)=row;
	int **matrix;
	matrix = (int **)malloc(row * sizeof(int *));
	for(i=0;i<row;i++)
		matrix[i] = (int *)malloc(col * sizeof(int));
	for(i=0,k=0;i<row;i++)
	{
		for(j=0;j<col;j++,k++)
		{
			if(i==j&&arr[k]!=0)
			{
				printf("\n\tInvalid Matrix");
				return 0;
			}
			else
			{
				if(arr[k]==-1)
					matrix[i][j]=999;
				else
					matrix[i][j]=arr[k];
			}
		}
	}
	return matrix;
}

/* Function Name : findPath()
 * Function		 : It accepts the connection matrix and distance matrix and
 *				   using those matrices calculates the path from the source
 *				   to the destination given to the function. This path is
 *				   stored in an array and then displayed.
 * Input		 : int** connection, int** distance,int source, int dest
 * Return		 : void
 */

void findPath(int** connection, int** distance,int source, int dest)
{
    int mid=0,count=1,index=0;
    int end=dest-1;
    int start=source -1;
    int path[100];
    for(index=0;index<100;index++)
      path[index]=-1;
    path[0]=start;
    if(distance[start][end] == 999)
	{
		printf("\n\tRouter not reachable");
	}
    else
	{
		while(connection[start][end] != 0)
		{
    		mid=connection[start][end]-1;
    		path[count]=mid;
    		start=mid;
			count++;
    	}
    	path[count]=end;
    	printf("\n\tPath\n\n\t");
    	for (index= 0; index<count+1; index++ )
		{
			if (index==0)
				printf("R%d ->",path[index]+1);
    		else if (index==count)
				printf(" R%d",path[index]+1);
    		else
				printf( " R%d ->", (path[index]+1));
    	}
    	printf("\n\n\tCost of Route : %d\n",distance[source-1][dest-1]);
    }
}

/* Function Name : printTopology()
 * Function		 : Prints the topology matrix that is passed to it.
 * Input		 : int** matrix, int* size
 * Return		 : void
 */

void printTopology(int** matrix, int* size)
{
	int row=0,column=0;
	for(row=0;row<(*size);row++)
	{
		printf("\n");
		for(column=0;column<(*size);column++)
		{
			if(matrix[row][column]==999)
				printf("\t-1");
			else
				printf("\t%d",matrix[row][column]);
		}
	}
}

/* Function Name : connTable()
 * Function		 : The function accepts the connection and distance matrix
 *				   calculated using the dijkstras algorithm and using those
 *				   matrices creates the connection table and prints it.
 * Input		 : int** connection, int** distance, int* size, int src
 * Return		 : void
 */

void connTable(int** connection, int** distance, int* size, int src)
{
	int source=src-1,index=0;
	int conn[*size];
	for (index= 0;index<(*size);index++)
	{
		if(index == source)
			conn[index] = 0;
		else if(connection[source][index] == 0 && distance[source][index] != 999 )
			conn[index] = index;
		else if(connection[source][index] == 0 && distance[source][index] == 999 )
			conn[index] = 999;
		else
			conn[index] = connection[source][index]-1;
	}
	printf("\n\tRouter %d Connection Table",src);
	printf("\n\tDestination\tInterface");
	printf("\n\t==========================");
	for(index=0;index<(*size);index++)
	{
		if(index==source)
			printf("\n\t     %d\t\t     -",index+1);
		else if(conn[index]==999)
			printf("\n\t     %d\t\t     No interface",index+1);
		else
			printf("\n\t     %d\t\t     %d",index+1,conn[index]+1);
	}
}

/* Function Name : dijkstra()
 * Function		 : Accepts the topology matrix, number of routers and also two
 *				   2d array pointers. It calculates the distance and connection
 *				   tables and adds that information in those pointers passed to
 *				   it using the dijkstras agorithm.
 * Input		 : int** matrix, int* size, int** distance, int** connection
 * Return		 : void
 */

void dijkstra(int** matrix, int* size, int** distance, int** connection)
{
  int i,j,source,destination,intermediate;
  for (source=0; source<(*size); source++)
    for (destination=0;destination<(*size); destination++)
			distance[source][destination]=matrix[source][destination];
  for (intermediate=0;intermediate<(*size);intermediate++)
	{
	  for (source=0;source<(*size);source++)
		{
		  for (destination=0;destination<(*size);destination++)
			{
				if ((distance[source][intermediate] + distance[intermediate][destination])<distance[source][destination])
				{
                    //printf("\n\tSource = %d, Destination = %d, Intermediate = %d",source,destination,intermediate);
					distance[source][destination] = distance[source][intermediate]+distance[intermediate][destination];
					connection[source][destination] = intermediate+1;
					connection[destination][source] = intermediate+1;
				}
			}
		}
	}
  for (i=0;i<(*size);i++)
	{
	  for (j=0;j<(*size);j++)
		{
			if(connection[i][j]!=0 && matrix[i][j] == 999)
			{
				int x = j;
				while(connection[i][x] != 0)
				{
					int y = connection[i][x]-1;
					x =y;
				}
				connection[i][j] = x+1;
			}
		}
	}
}

/* Function Name : modifyTopology()
 * Function		 : Accepts the topology matrix and router to disable
 *				   and disables the router by changing the all the rows
 *				   and columns to in that router to 999.
 * Input		 : int** matrix, int* size, int router
 * Return		 : void
 */

void modifyTopology(int** matrix, int* size, int router)
{
	int row,column;
	for(row=0;row<(*size);row++)
	{
		for(column=0;column<(*size);column++)
		{
			if(column==router-1)
				matrix[row][column]=999;
			else if(row==router-1)
				matrix[row][column]=999;
		}
	}
}

/* Function Name : main()
 * Function		 : Main function executes the simulator
 * Input		 : void
 * Return		 : int
 */

int main()
{
	int choice=0,router=0, source=0,dest=0,disList=0;
	int route[10],i;
	int *size=(int *)malloc(sizeof(int));
	int** matrix;
    int** distance;
    int** connection;
	while(1)
	{
		printPrompt();
		scanf("%d",&choice);
		switch(choice)
		{
			case 1 : matrix=createTopology(size);
				 if(matrix==0)
					break;
				 printTopology(matrix,size);
				 distance = (int **)malloc((*size) * sizeof(int *));
				 connection = (int **)malloc((*size) * sizeof(int *));
				 for(i=0;i<(*size);i++)
				   {
				     distance[i] = (int *)malloc((*size) * sizeof(int));
				     connection[i] = (int *)malloc((*size) * sizeof(int));
				   }
				 dijkstra(matrix,size,distance,connection);
				 break;
			case 2 : printf("\n\tEnter the router : ");
				 scanf("%d",&source);
				 connTable(connection,distance,size,source);
				 break;
			case 3 : printf("\n\tEnter the destination : ");
				 scanf("%d",&dest);
				 if(source<1||source>(*size))
				 {
					printf("\n\tInvalid source");
					if(dest<1||dest>(*size))
						printf("\n\tInvalid destination");
					break;
				 }
				 if(source!=dest)
				    findPath(connection,distance,source,dest);
				 else
					printf("\n\tSource and destination are the same.");
				 break;
			case 4 : printf("\n\tEnter the router to disable : ");
				 scanf("%d",&router);
				 if(router>(*size)||router<1)
				 {
					printf("\n\tInvalid router");
					break;
				 }
				 modifyTopology(matrix,size,router);
				 dijkstra(matrix,size,distance,connection);
				 connTable(connection,distance,size,source);
				 findPath(connection,distance,source,dest);
				 break;
			case 5 : if(matrix!=NULL)
				 	 free(matrix);
				 if(size!=NULL)
					 free(size);
				 printf("\n\tExit CS452 project. Good Bye!\n");
				 exit(0);
				 break;
			default : if(matrix!=NULL)
						free(matrix);
				  	  if(size!=NULL)
				  		free(size);
                      if(distance!=NULL)
                        free(matrix);
                      if(connection!=NULL)
                        free(size);
					  exit(0);
		}
	}
}
