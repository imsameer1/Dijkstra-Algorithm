/* Sameer Dayani 1002015854, Coding Assignment 5 */
#include <stdlib.h> 
#include <stdio.h> 
#include <string.h>
#include <time.h>
#include <limits.h>

#define MAX_SIZE 10

typedef struct
{
   char label[6];
   int distance;
   int previous;
   int visited;
}
Vertex;


void CreateAdjacencyMatrix(int AdjMatrix[][MAX_SIZE])
{
   int start = 0, end = 0;
   int i = 0, j = 0;

//Initializing the AdjMatrix to -1

   for(i = 0; i < MAX_SIZE; i++)
   {
      for(j = 0; j< MAX_SIZE; j++)
      {
         AdjMatrix[i][j] = -1;
      }
   }
}

void addVertex(char label[6], Vertex VertexArray[], int VertexCount)
{
   strcpy(VertexArray[VertexCount].label, label);
   VertexArray[VertexCount].distance = INT_MAX;
   VertexArray[VertexCount].previous = -1;
   VertexArray[VertexCount].visited = 0;
}

void addEdge(int start, int end, int AdjMatrix[][MAX_SIZE], int edge)
{
   AdjMatrix[start][end] = edge;
}


int ReadFile(int argc, char *argv[], Vertex VertexArray[], int AdjMatrix[][MAX_SIZE])
{
   int count = 0;
   char label[6];
  // char A[6];
  // char B[6];
   int AdjVertIndex = 0;
   int weight = 0;
   FILE *FH = NULL;
   char *token = NULL;
   char Fileline[500] = {};
   FH = fopen(argv[1], "r");
   if( FH == NULL)
   {
      printf("\nCannot open the file...exiting\n");
      exit(0);
   }
   while (fgets(Fileline, sizeof(Fileline) - 1, FH))
   {
      if (Fileline[strlen(Fileline)-1] == '\n')
      {
         Fileline[strlen(Fileline)-1] = '\0';
      }
      token = strtok(Fileline, ",");
      strcpy(label, token);
      token = strtok(NULL, ",");
      while (token != NULL)
      {
         //strcpy(A, token);
         AdjVertIndex = atoi(token);
         token = strtok(NULL,",");
         //strcpy(B,token);
         weight = atoi(token);
         addEdge(count,AdjVertIndex, AdjMatrix, weight);
         token = strtok(NULL, ","); 
      }
      addVertex(label, VertexArray, count);
      count++;
   }
   fclose(FH);
   return count;
}
   

int main(int argc, char *argv[]) 
{
   int VertexCount = 0;
   int AdjMatrix[MAX_SIZE][MAX_SIZE] = {};
   Vertex VertexArray[MAX_SIZE];
   int i, j;
   int StartVertIndex = 0;
   char StartVertLabel[6] = {};
   char dest[6] = {};
   int destIndex = -1;
   int path[MAX_SIZE] = {};
   int pathIndex = -1;
   int prevIndex = -1;
   int x;
   int SmallestVertexIndex;
   CreateAdjacencyMatrix(AdjMatrix);


   VertexCount = ReadFile(argc, argv, VertexArray, AdjMatrix);
   
   #ifdef PRINT
   printf("\n");
   for(i = 0; i < VertexCount; i++)
   {
      for(j = 0; j < VertexCount; j++)
      {
         printf("%5d\t", AdjMatrix[i][j]);
      }
      printf("\n");
   }
   #endif
   printf("\nWhat is the starting vertex? ");
   scanf("%s", StartVertLabel);
 
   while (StartVertIndex < VertexCount && (strcmp(VertexArray[StartVertIndex].label, StartVertLabel) != 0))
   {
      StartVertIndex++;
   }

   if (StartVertIndex == VertexCount)
   {
      printf("Source %s is not in graph...exiting\n", StartVertLabel);
      exit(0);
   }

//Dijsktra Algorithm

   int CurrentVertex = StartVertIndex;
   VertexArray[StartVertIndex].distance = 0;
   VertexArray[StartVertIndex].previous = -1;
   VertexArray[StartVertIndex].visited = 1;

   for(x = 0; x < VertexCount-1; x++)
   {
      for(i = 0; i < VertexCount; i++)
      {
         if(AdjMatrix[CurrentVertex][i] != -1 && !VertexArray[i].visited)

         {
            if(VertexArray[CurrentVertex].distance + AdjMatrix[CurrentVertex][i] < VertexArray[i].distance)
            {
               VertexArray[i].distance = VertexArray[CurrentVertex].distance + AdjMatrix[CurrentVertex][i];
               VertexArray[i].previous = CurrentVertex;
            }
         }
        
      }


      SmallestVertexIndex = -1;
      int SmallestVertex = INT_MAX;   //infinity
      for(i = 0; i < VertexCount; i++)
      {
         if (!VertexArray[i].visited)
         {
            if (VertexArray[i].distance < SmallestVertex)
            {
               SmallestVertex = VertexArray[i].distance;
               SmallestVertexIndex = i;
            }
         }
      }
      
      CurrentVertex = SmallestVertexIndex;
      VertexArray[CurrentVertex].visited = 1;   
   }


   
   #ifdef PRINT
   printf("\n\nI\tL\tD\tP\tV\n");
   for (i = 0; i < VertexCount; i++)
   {
      printf("%d\t%s\t%d\t%d\t%d\n", i, VertexArray[i].label, VertexArray[i].distance, VertexArray[i].previous, VertexArray[i].visited);
   }
   printf("\n\n");
   #endif
 
   printf("What is the destination vertex? ");
   scanf("%s", dest);
   destIndex = 0;
   while (destIndex < VertexCount && (strcmp(dest, VertexArray[destIndex].label) != 0))
   {
      destIndex++;
   }

   if (destIndex == VertexCount)
   {
      printf("Destination %s is not in graph...\n", dest);
      exit(0);
   }
   else
   {
     /* pathIndex = destIndex;
      prevIndex = VertexArray[destIndex].previous;
      path[pathIndex] = VertexArray[destIndex].distance; */
      int prev;
      pathIndex = 0;
      path[pathIndex++] = destIndex;  //path[0] = 4, path=1
      path[pathIndex++] = VertexArray[destIndex].previous;  //path[1] = 3, path = 2
      prev = VertexArray[path[pathIndex-1]].previous;  //0
      while (prev != -1)
      {
         path[pathIndex++] = prev; //path[2] = 0
         prev = VertexArray[path[pathIndex-1]].previous;  //prev = 0 prev = -1
         
      }
      pathIndex--;
      printf("The path from %s to %s is ", VertexArray[StartVertIndex].label, VertexArray[destIndex].label);
      while (pathIndex != 0)
      {
         printf("%s->", VertexArray[path[pathIndex]].label); 
         pathIndex--;
      } 
      printf("%s and has a length of %d\n",VertexArray[destIndex].label, VertexArray[destIndex].distance);
   }
   return 0; 

} 

