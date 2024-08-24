#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <string.h>

#define DEVICE "/dev/mymousedev"

//since we dont have access to file here we need to do sudo chmod 777 /dev/... the 777 is for user,grp n other
void morseToText(char in[256]) {
    const char *morseCodeDict[] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..",
                             "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."};
    const char *morseCodeDict2[] = {" ", "-.-.--", "-1", "-1", "-1", "-1", "-1", "-1", "-1", "-1", "-1",
                              ".-.-.", "--..--", "-....-", ".-.-.-", "-..-.", "-----", ".---", "..---", "...--", "....-", ".....",
                              "-....", "--...", "---..", "----.", "---...", "-.-.-.", "-1", "-...-", "-1", "..--.."};

    FILE *outputFile = fopen("output.txt", "w");
    if (outputFile == NULL) {
        perror("Error opening output file");
        return;
    }

    char *token = strtok(in," ");
    while (token != NULL) {
        printf("Token: %s\n", token); // Print token for debugging
        if(strcmp(token,"s")==0)
        {
            printf("Matched Alphabet: %c\n", ' '); // Print matched alphabet for debugging
                fprintf(outputFile, "%c", ' ');
                
        }
        for (int i = 0; i < 26; i++) {
            if (strcmp(token, morseCodeDict[i]) == 0) {
                printf("Matched Alphabet: %c\n", 'A' + i); // Print matched alphabet for debugging
                fprintf(outputFile, "%c", ('A'+i));
                break;
            }
        }
        for (int i = 0; i < 32; i++) {
            if (strcmp(token, morseCodeDict2[i]) == 0) {
                printf("Matched Special Character: %c\n", ' ' + i); // Print matched special character for debugging
                fprintf(outputFile, "%c", (' ' + i));
                break;
            }
        }
        token = strtok(NULL, " ");
    }
    fclose(outputFile);
}



int callingthemorse()
{
    FILE *inputFile = fopen("input.txt", "r");
    if (inputFile == NULL) {
        perror("Error opening input file");
        return 1;
    }
    char in[256];

    char c;
    int ctr=0;
    while ((c = fgetc(inputFile)) != EOF) {
        if (c == 'm') // Morse code ends when the middle button is pressed
            break;
        if (c == 'l') // '.' (dot)
            //insertArray(&morseCode,'.');
            in[ctr++]='.';
        else if (c == 'r') // '-' (hyphen)
            //insertArray(&morseCode,'-');
            in[ctr++]='-';
        else if(c=='s') in[ctr++]='s';
        else in[ctr++] =' ';
    }
    fclose(inputFile);
    
    printf("English Text: ");
    morseToText(in);

    
//   FILE *inputFile = fopen("input.txt", "w");

    printf("\n");

}


int main(){
    int i,fd; //i is no.byteswritten and fd is file descriptor
    char ch,write_buffer[100],read_buffer[100]; //ch is choice by user for readin or writing

     struct input_event ie;
    unsigned char *ptr = (unsigned char*)&ie;
    unsigned char button,bLeft,bMiddle,bRight,bthumbmouse1,bthumbmouse2;
    char x,y;
    FILE *inputFile = fopen("input.txt", "w");


    fd = open(DEVICE, O_RDWR); //opening it for reading n writing

    //more error chechking
    if (fd==-1)
    {
        printf("file could not be opened");
        exit(-1); 
    }

    printf("heres general info about the program");
    printf("enter leftmouseclick for dot\n");
    printf("enter rightmouseclick for dash\n");
    printf("enter thmb1 to differentiate between letters\n");
    printf("enter thmb2 for space between words\n");
    printf("enter middleclick for output\n\n\n");
    printf("enter r for read and w for wrrite\n");

    
    scanf("%c",&ch);

    switch (ch)
    {
    case 'w':
        /* code */
        printf("enter data\n");
        scanf(" %[^\n]",write_buffer);
        write(fd,write_buffer,sizeof(write_buffer));
        break;
    
    case 'r':
    while(1)
     {

       if(read(fd, &ie, sizeof(struct input_event))!=-1) //read from buffer i.e. ie and no,of bytes to be read is size of struct
        {
        button = ptr[0];
        
        bLeft = button & 0x01;
        bMiddle = ( button & 0x04 ) > 0;
        bRight = ( button & 0x02 ) > 0;
        bthumbmouse1 =  ( button & 0x10 ) > 0;
        bthumbmouse2 =  ( button & 0x08 ) > 0;
       
            x=(char) ptr[1];
            y=(char) ptr[2];


        //  printf("\n");
        //  printf("\n");
        //  printf("\n");
        //  printf("\n");
        //  printf("\n");
        //  printf("\n");
        // printf("button = %d\n",button);

        if(bLeft==1)
           {
                FILE *inputFile = fopen("input.txt", "a+");
                fprintf(inputFile, "%c", ('l'));
                fseek(inputFile,0,SEEK_END);
                continue;


           }
    
    //    printf("bMiddle = %d\n",bMiddle);
    
        if(bRight==1)
        {

                
        
    


        printf("bRight = %d\n",bRight);
        FILE *inputFile = fopen("input.txt", "a+");
                fprintf(inputFile, "%c", ('r'));
                fseek(inputFile,0,SEEK_END);

                continue;

        }    

        if(bthumbmouse1==1)
        {
            
            FILE *inputFile = fopen("input.txt", "a+");
                fprintf(inputFile, "%c", (' '));
                fseek(inputFile,0,SEEK_END);

        printf("bthumbmouse1 = %d\n",bthumbmouse1);
        continue;
        }
        if(bthumbmouse2==1)
        {
            FILE *inputFile = fopen("input.txt", "a+");
                fprintf(inputFile, "%c", ('s'));
                fseek(inputFile,0,SEEK_END);
                continue;
        printf("bthumbmouse2 = %d\n",bthumbmouse2);
        }
        if(bMiddle==1)
        {

            FILE *inputFile = fopen("input.txt", "a+");
                fprintf(inputFile, "%c", ('m'));
                fseek(inputFile,0,SEEK_END);

                callingthemorse(); 
        break;

        }
        }
        }
        break;
    }
    
    int a;
    int b;
    while(1){
    
		if(read(fd, &ie, sizeof(struct input_event))!=-1){
		
			a = ptr[0];
			b = ( a & 0x4 ) > 0;
			if(b == 0) break;
		}
    
    }

    close(fd);
    fflush(stdout);



    return 0;

}

