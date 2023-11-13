// C program to demonstrate use of fork() and pipe() 
#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
  
int main() 
{ 
    // We use two pipes 
    // First pipe to send input string from parent 
    // Second pipe to send concatenated string from child 
  
    int fd1[2];  // Used to store two ends of first pipe 
    int fd2[2];  // Used to store two ends of second pipe 
  
    char fixed_str1[] = "howard.edu"; 
    char fixed_str2[] = "gobison.org";
    char input_str[100]; 
    pid_t p; 
  
    if (pipe(fd1)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
    if (pipe(fd2)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
  
    printf("Enter a string to concatenate: ");
    scanf("%s", input_str); 
    p = fork(); 
  
    if (p < 0) 
    { 
        fprintf(stderr, "fork Failed" ); 
        return 1; 
    } 
  
    // Parent process (P1)
    else if (p > 0) 
    { 
  
        close(fd1[0]);  // Close reading end of pipes (P1 > P2)
        close(fd2[1]);  
  
        // Write input string to P2 and close writing end of first 
        // pipe. 
        write(fd1[1], input_str, strlen(input_str)+1); 
        
  
        // Wait for child (P2) to send a string 
        wait(NULL); 

        //read the string from P2
        char concat_str[100];
        read(fd2[0], concat_str, 100);

        //print the concatenated string
        printf("Concatenated string: %s\n", concat_str);
  
        close(fd2[1]); // Close writing end of pipes 
        close(fd1[0]); //close reading end of pipes
    } 
  
    // child process (P2)
    else
    { 
        close(fd1[1]);  // Close writing end of first pipes 
        close(fd2[0]); 
      
        // Read a string using first pipe 
        char received_str[100]; 
        read(fd1[0], received_str, 100); 
  
        // Concatenate a fixed string with it 
        strcat(received_str, fixed_str1);

        // int k = strlen(received_str); 
        // int i; 
        // for (i=0; i<strlen(fixed_str1); i++) 
        //     received_str[k++] = fixed_str1[i]; 
  
        // received_str[k] = '\0';   // string ends with '\0' 
  
        //print the concatenated string
        printf("Concatenated string: %s\n", received_str);

        //prompt user for a second input
        printf("Enter another string to concatenate: ");
        scanf("%s", received_str);

        //concatenate the second input
        strcat(received_str, fixed_str2);

        //write the second input to P1
        write(fd2[1], received_str, strlen(received_str) + 1);


        // Close both reading ends 
        close(fd1[0]); 
        close(fd2[1]); 

  
        exit(0); 
    } 
} 