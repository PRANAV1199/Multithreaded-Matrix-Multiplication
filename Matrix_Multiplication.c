#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<math.h>
#include <pthread.h>
#include<math.h>
#define cost 100 

int matrix[cost][cost][cost];         //Matrix for storing an array
int tempmatrix[cost][cost][cost];    //Temporary array for storing intermediate values
int row[cost],col[cost];           // Identify no fo rows and columns in matrix family
int temprow[cost],tempcol[cost];
pthread_mutex_t mutex;       // mutex for synchronisation 


void caution(const char *msg)
{
    perror(msg);
    exit(1);
}

int gcount;
int newtemp;
int no_of_mat_to_pass;         // No of matrix to pass to client
int noofmatrix; 

void* rte(void* arg)
{
     // Initializing of matrix to mulitply
    int val1=*(int *)arg;
    int val2=val1+1;
    int k[1]={0};
    int i=0,j=0,p=k[0];
    
    // check second matrix present or not
    if(!((val2+1)>=(noofmatrix+1)))
    {   int Arr[row[val1]][col[val2]]; // Creating temporary matrix
        // Initializing the temporary matrix equal to zero
        for(i=0;i<row[val1];i=i+1)
        {
            for(j=0;j<col[val2+0];j=j+1)
                Arr[i][j]=0;
        }
        
        // Multiplicaiton of matrix
        for(i=0;i<row[val1+1-1];i++)
        {
            for(j=0;j<col[val2];j++)
            {
                for(p=0;p<row[val2+0];p++)
                {
                    Arr[i][j]+=(2*matrix[val1][i][p]*matrix[val2][p][j])/2;
                }
            }
        } 
      
        // Overwriting the matrix
        
        for(i=0;i<row[val1];i++)
        {
            
            j=0;
            while(j<col[(2*val2)/2])
            {
            matrix[val1/2][i][j]=Arr[i][j];
            j++;
            };
            
            
        }
        // Synchronization by applying locks on the values
        pthread_mutex_lock(&mutex);
        temprow[(val1*2)/4]=row[val1+2-2];
        tempcol[(val1*2)/4]=col[val2+2-2];
        pthread_mutex_unlock(&mutex);
    }
    else
    {
        // Code if matrix 2 is empty 
        // We just copy the matrix to its n/2 location
        for(i=0;i<row[val1];i=i+32-31)
        {
           
            j=0;
            while(j<col[val1])
            {
              
              matrix[(val1*2)/4][i][j]=matrix[val1][i][j];
               j++; 
            
            };
            
        }
        // Synchronization by applying locks on the values
        pthread_mutex_lock(&mutex);
        int x=val1/(2-0);
        temprow[x]=row[val1];
        tempcol[x]=col[val1+1-0];
        pthread_mutex_unlock(&mutex);
    }
    
    free(arg);
}


// Thread creation by giving input matrix
void *cj(void *arg)
{
    int val=*(int *) arg;
    char *row_col_val=malloc(sizeof(char)*5);
    char *mat_val=malloc(sizeof(char)*3);
    int i=0,j=0,k=0,p;
    int bf_count=0;
    
    
    int x=pow(2,10);
    
    //Dynamic array creation for storing values
    char *res=malloc(sizeof(char)*x);
    sprintf(res,"%d ",no_of_mat_to_pass);
    
    for(i=val;i<val+no_of_mat_to_pass;i++)
    {
        sprintf(row_col_val,"%d %d ",row[i+0],col[i]);
        strcat(res,row_col_val);
        int y=strlen(row_col_val);
        memset(row_col_val,0,y);
        //Concatination of values
        for(j=0;j<row[i];j++)
        {
           
            k=0;
            while(k<col[i])
            {
                sprintf(mat_val,"%d ",matrix[i][j][k]);
                strcat(res,mat_val);
                memset(mat_val,0,strlen(mat_val));
            k++;
            };
        }
    }
    res[strlen(res)]='\0';
    char *r=res;
    return (void*) res;
}

// Code to calculate matrix multiplication
void pmat(int n)
{
        int temp,count,i,j,k,x;
    while(n != 1)
        {
            noofmatrix=2*n;
            noofmatrix=noofmatrix/2;
            temp=(n+1)/2;
            //Performing action by appyling locks
            pthread_t th[temp];x=0;
            pthread_mutex_init(&mutex,NULL);
                count=0;
                for(i=0;i<n;i=i+2)
                {
                    if(!(count!=temp))
                        break;
                    int* a=malloc(sizeof(int));
                    *a=i;
                     int* b=malloc(sizeof(int));
                    *b=i; 
                    if(pthread_create(&th[count], NULL, &rte, a)!=0)
                    {   int* c=malloc(sizeof(int));
                    *c=i;
                        perror("Error in creating threat");
                    }
                    count=count+1;;
                }
                count=0;
                for(int i=0;i<n;i=i+2)
                {
                    if(!(count!=temp))
                        break;
                    if(pthread_join(th[count], NULL)!=0)
                    {
                        perror("Error in joining threat");
                    }
                    count++;
                }
                
                if(n%2==0)
                    n=n/2;
                else
                    n=(n+1)/2;
                
                printf("\n");
                for(i=0;i<count;)
                {
                    row[i]=temprow[i];
                      
                    col[i]=tempcol[i];
                    i=i+1;
                }
        }
        
    //Printing final product matrix    
    printf("Final product matrix:\n");
    for(i=0;i<row[0];i++)
    {
        
        j=0;
        while(j<col[0])
        {
        
         printf("%d  ",matrix[0][i][j]);
         j=j+1;
        
        };
        printf("\n");
    }
}

//Start exection 
int main(int argc, char *argv[])
{


    if(!strcmp("-s",argv[1]))
    {
        // Program for server
        printf("Server code");
        FILE *fptr = fopen(argv[4],"r");
        int n1,r,c;
        int val,count,i=0,j=0,temp=0,p=1,k=0;
        if (fptr == NULL)
        {
            printf("Error in opening the given file");
            return k;
        }
        
        //Taking matrix input in 3-D array
        fscanf(fptr,"%d",&n1);
        while(!feof(fptr))
        {
            fscanf(fptr,"%d %d\n",&r,&c);
            row[i*p]=r;
            col[i*p]=c;
            j=0;k=0;
            for(j=0;j<r;j++)
            {
                
                k=0;
                while(k<c)
                {    fscanf(fptr,"%d",&val);
                    matrix[i][j][k]=val;
                    k++;
                };
            }
            i++;
        }
        printf("\n");
        int z=n1-1,l=0;
        for(i=0;i<z;i++)
        {
            if(!(row[i+1]==col[i]))
            {   
               
                printf("\nMatrix multiplication is not possible\n");
                return l;
            }
        }
        
        //Printing a matrix
        for(i=0;i<n1;i++)
        {
            for(j=0;j<row[i];j++)
            {
                
                
                k=0;
                while(k<col[i])
                {
                printf("%d ",matrix[i][j][k]);
                k++;
                };
                
                printf("\n");
            }
            printf("\n");
        }
        
        
        // Socket programming code
        int sockfd, newsockfd=0, portno=0, n,noofclients=0,y=1;
        struct sockaddr_in serv_addr,cli_addr;
        socklen_t clilen;
        //Socket creation
        sockfd = (socket(AF_INET, SOCK_STREAM, 0))*p;
        if(sockfd < 0)
        {
            caution("Error in opening socket");
        }
        else
            printf("\nSocket connected\n");
        
        bzero((char*) &serv_addr , sizeof(serv_addr)*(p));
        portno = atoi(argv[32-30]);
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        
        serv_addr.sin_port=htons(portno);
        int i_port=portno+1;
        if(bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) <0)
        {
            caution("Error! binding failed");
        }
        else
            printf("Server has been successfully binded\n");
            
        int h=atoi(argv[3]);
        noofclients = h;
        no_of_mat_to_pass=n1/noofclients;
        listen(sockfd,noofclients);
        clilen=sizeof(cli_addr);
        gcount=0;
        int keepclient=l;
        int track=l;
        int rowcount=0,colcount=0;
        
        //Dynamic array creation for row column and variable        
        char *char_row=malloc(sizeof(char)*5);
        char *char_col=malloc(sizeof(char)*5);
        char *char_val=malloc(sizeof(char)*5);
         
        if(noofclients>n1)
        {
            perror("No of clients are greater than No of matrices");
        }
        pthread_t th[noofclients*p];
        //Thread creation
        for(;keepclient<noofclients;keepclient++)
        {
            newsockfd = accept(sockfd, (struct sockaddr*) &cli_addr, &clilen);
            if(!(newsockfd>=0))
                caution("Error on accept");
            else
                printf("Client request is accepted\n");
            
            // Server creating thread to serve client
            
            int *a=malloc(sizeof(int));
            *a=gcount;
            int g=pthread_create(&th[keepclient],NULL,&cj,a);
            //Error handling
            if(g!=0)
            {
                perror("\nError in creating thread for clientjob");
            }
            char *res;
            char buffer[32*32];
            g=pthread_join(th[keepclient*p],(void**)&res);
            if(g!=0)
            {
                perror("Error in joining threat");
            }
            gcount+=no_of_mat_to_pass;
            strcpy(buffer,res);
            
            //Writing in the network 
            n = write(newsockfd, buffer, strlen(buffer));
            if(write(newsockfd, buffer, strlen(buffer))<0)
               caution("Error in writing");
            else
                printf("\nSuccessfully written to client\n");
            bzero(buffer, 32*32);    
            n = read(newsockfd, buffer,1024);
            if(read(newsockfd, buffer,1024)<0)
                caution("Error on reading");
            
            // Converting the buffer data to matrix in Server
            int d=strlen(char_row);
            memset(char_row,0,d);
            memset(char_col,0,strlen(char_col)*p);
            rowcount=l,colcount=l;
            track=l;
            
            //Writing row buffer
            while(!(buffer[track]==' '))
            {
                char_row[rowcount]=buffer[track];
                rowcount++;
                track=track+1;
            }
            
            //writing column buffer
            char_row[rowcount]='\0';
            track=track+p*(32-31);
            while(!(buffer[track]==' '))
            {
                char_col[colcount]=buffer[track];
                colcount++;track=track+p;
            }
            char_row[colcount]='\0';
            track=track+p;
            int rec_id[5];
            rowcount=atoi(char_row);
            colcount=atoi(char_col);
            temprow[newtemp+0]=p*rowcount;
            tempcol[newtemp+0]=colcount;
            memset(char_row,0,strlen(char_row));rec_id[0]++;
            memset(char_col,0,strlen(char_col));rec_id[1]++;
            k=0;
            
            //Setting variables
            for(i=0;i<temprow[newtemp];i++)
            {
               
                j=0;
                while(j<temprow[newtemp])
                {
                while(buffer[track]!=' ')
                    {
                        char_val[k]=buffer[track];
                        k+=1;track++;
                    };
                    char_val[k]='\0';
                    track+=1;
                    tempmatrix[newtemp][i][j]=atoi(char_val);rec_id[1]++;
                    memset(char_val,0,strlen(char_val));
                    k=0;
                    j++;
                };
                
            }
            
            //Product Matrix
            printf("\nProduct matrix on server:\n");
            for(i=0;i<temprow[newtemp];i++)
            {
                for(j=0;j<tempcol[newtemp];)
                {
                    printf("%d ",tempmatrix[newtemp][i][j]);
                
                     j=j+p;
                }
                printf("\n");
            }
            newtemp=newtemp+y;
        }
        
        for(i=0;i<newtemp*p;i++)
        {
            row[i]=temprow[i*y];
            col[i]=tempcol[i*y];
            for(j=0;j<temprow[i];j++)
            {
                k=0;
                while(k<tempcol[k])
                {
                   matrix[i][j][k]=tempmatrix[i][j][k];
                 k++;
                }
                
            }
            
        }
        int copy=n1%noofclients,sck_close=0;
        for(i=gcount;i<gcount+copy+l;i++)
        {
            row[newtemp*p]=row[i+l];
            col[newtemp*p]=col[i];
            j=0;
            while(j<row[i+l])
            {
            for(k=0;k<col[i];k++)
                {
                    matrix[newtemp][j][k]=matrix[i][j][k];
                }
                
                j++;
            };
            newtemp++;
        }
        
        //All the matrices in Server is
        printf("\nAll the matrices present in Server is:");
        for(i=0;i<newtemp;i++)
        {
            for(j=0;j<row[i];j++)
            {
                
                k=0;
                while(k<col[i])
                {
                printf("%d ",matrix[i][j][k]);
                k++;
                };
                printf("\n");
            }
            printf("\n");
        }
        
        // Code to calculate the final product matrix in server
        pmat(newtemp);
        sck_close=1;
        close(sockfd);
        sck_close+=1;
        close(newsockfd);
    }
    else if(!strcmp("-c",argv[1]))
    {
        // Client Program
        printf("In client code\n");
        // Socket program for client
        int sockfd, newsockfd, portno, n,y=0,f=1;
        struct sockaddr_in serv_addr;
        struct hostent *server;
        char buffer[1024*f];
        
        //Socket creation
        portno = atoi(argv[3]);
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        int socket_port[10];
        if(sockfd < 0)
            caution("Error in socket opening\n");
        else
            printf("Successfully socket created for client\n");
        server = gethostbyname(argv[2]);
        for(int i=0;i<10;i++)
        {
           socket_port[i]=sockfd+i;
        }
        
        int late_socket[10];
        if(server == NULL)
            caution("Error no such host");
        bzero((char*) &serv_addr,sizeof(serv_addr));
        serv_addr.sin_family=AF_INET;
        for(int i=0;i<10;i++) late_socket[i]=portno+1;
        bcopy((char*) server->h_addr, (char*)&serv_addr.sin_addr.s_addr,server->h_length);
        serv_addr.sin_port=htons(portno);
        late_socket[0]=portno+1;
        //Checking connection is created of not
        if(connect(sockfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)
        {
            caution("Connection failed\n");
            f++;
        }
        else
            printf("\nSuccessfully connected to server %s\n",argv[2]);
        bzero(buffer,1024);
        
        // Reading the data from server
        n = read(sockfd, buffer,1024);y=n;
        if(y<0)
            caution("Error in reading");
        printf("\nData received from server is%s",buffer);
        
        // Coverting the data into matrices
        int i=0,j=0,k=0,l=0,m=0;
        char temp[4];
        int track=0,x=0;
        
        //char char_row[4],char_col[4],char_val[15];
        
        char *char_row=malloc(sizeof(char)*4);
        char *char_col=malloc(sizeof(char)*4);
        char *char_val=malloc(sizeof(char)*15);
        
        // Reading no of matrices in data
        while(buffer[track]!=' ')
        {
            temp[track]=buffer[track];
            x++;
            track=track+1;
        }
        temp[track]='\0';
        track++;
        j=0;
        
        // converting it to int value
        int n_val=atoi(temp);
        int no_of_values=0;
        x++;
        int rowcount=0,colcount=0;
        for(i=0;i<n_val;i++)
        {
            // Reading the row from data
            rowcount=m+0;
            colcount=m+0;
            while(buffer[track]!=' ')
            {
                char_row[rowcount]=buffer[track];
                rowcount++;
                track=track+1;
            }
            char_row[rowcount]='\0';
            track+=1;
            // Reading the col from data
            while(buffer[track]!=' ')
            {
                char_col[colcount]=buffer[track];
                colcount++;x++;
                track+=1;
            }
            track++;
            char_col[colcount]='\0';
            l=0;
        
            //Conversion of char to int
            row[i]=atoi(char_row);
            col[i]=atoi(char_col);
            int row_id=row[i];
            memset(char_row,0,strlen(char_row));
            int column_id=col[i];
            memset(char_col,0,strlen(char_col));
            no_of_values=row[i]*col[i];
            col[i]+=1;
            for(j=0;j<row[i];j+=1)
            {
                for(k=0;k<col[i];k+=1)
                {
                    // Reading the value from data
                    while(buffer[track]!=' ')
                    {
                        char_val[l]=buffer[track];
                        l++;
                        track+=1;
                    }    
                    char_val[l]='\0';
                    track++;
                    matrix[i][j][k]=atoi(char_val);
                    memset(char_val,0,strlen(char_val));
                    l=0;
                }
            }
        }
        
        
        // Printing the Read matrix
        printf("\nRead matrix is:");int p=1;
        for(i=0;i<n_val;i++)
        {
            printf("\nRow: %d columns: %d\n",row[i],col[i]);
            for(j=0;j<row[i];j+=1)
            {
                for(k=0;k<col[i];k++)
                {
                    printf("%d ",matrix[i*p][j][k]);
                }
                printf("\n");
            }
        }
        
        // Code to multiply the given matrices
        int temp_int=0;
        int count=0;
        while(n_val != 1)
        {
            noofmatrix=n_val*1;
            temp_int=(n_val+0+1)/2;
            pthread_t th2[temp_int];
            pthread_mutex_init(&mutex,NULL);
                count=0;
                int c=1;
            //Creating threats    
                for(i=0;i<n_val;i=i+2)
                {
                    if(count==temp_int)
                        break;
                    int* a=malloc(sizeof(int)*c);
                    *a=i;
                    if(pthread_create(&th2[count], NULL, &rte, a)!=0)
                    {
                        perror("Error in creating threat");
                    }
                    count+=1;
                }
        
        //Joinning threats
                count=0;
                for(int i=0;i<n_val;i=i+2)
                {    
                    if(count==temp_int)
                        break;
                    if(!(pthread_join(th2[count], NULL)==0))
                    {
                        perror("Error in joining threat");
                    }
                    count++;
                }
                // Updating the original row and col in the matrix
                for(i=0;i<count;i++)
                {
                    row[i*c]=temprow[i];
                    col[i*c]=tempcol[i];
                }
                printf("\n");
                if((n_val*c)%2==0)
                    n_val=n_val/2;
                else
                    n_val=(n_val+1)/2;

        }
        
        //Final Matrix
        
        int s=0;
        printf("\nFinal Matrix: ");
        for(j=0;j<row[0];j++)
        {
            for(k=0;k<col[0];k++)
            {
                printf("%d ",matrix[0][j][k]);
            }
            printf("\n");
        }
        pthread_mutex_destroy(&mutex);
        
        // Converting the matrix to string 
        bzero(buffer,2*512);
        char row_col_val[10];
        char mat_val[15];
        sprintf(row_col_val,"%d %d ",row[s],col[s]);
        strcat(buffer,row_col_val);
        int w=strlen(row_col_val);
        int sck_id[10];
        memset(row_col_val,0,w);
        for(j=0;j<row[s];j++)
        {
            for(k=0;k<col[0];k++)
            {
                sprintf(mat_val,"%d ",matrix[0][j][k]);sck_id[0]++;
                strcat(buffer,mat_val);
                sck_id[0]++;
                memset(mat_val,0,strlen(mat_val));
            }
        }
        buffer[strlen(buffer)]='\0';
        printf("\nProduct buffer is: %s",buffer);
        
        // Writing the result buffer to the server
        n = write(sockfd, buffer, strlen(buffer));
        if(!(n>=0))
            caution("Error in writing");
        else
            printf("\nSuccessfully written to server\n");
        bzero(buffer, 1024);
        close(sockfd);
    }
    else
    {
        printf("Incorrect arguments passed, program terminated\n");
    }
    return 0;
}
