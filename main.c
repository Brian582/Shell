# include <stdio.h>
# include <stdlib.h>//used to execute subprocess and commands
# include <string.h>
# include <unistd.h>//used for exit, getcwd, read, write, exec

# include <sys/types.h>
# include <dirent.h>//for ls
# include <errno.h>
# include <sys/stat.h>
# include <fcntl.h>//used for open

# include <direct.h>//for windows for mkdir
# include <errno.h>//this is need for rmdir command

#define SIZE 30

#define blue "\033[34m"//used for color blue
#define red "\033[31m"//used for color red
#define reset "\033[1;0m"//used for reseting color

char **parse(char *my_line);//function declaration
void help_command(char *argument);//function declaration
void cd_command(char *argument);//function declaration
void pwd_command();//function declaration
void ls_command();//function declaration
void mkdir_command(char *argument);//function declaration
void rmdir_command(char *argument);//function declaration
void cat_command(char *argument);//function declaration
void touch_command(char *argument);//function declaration
void grep_command(char **argument);//function declaration

//this function gets the commands and executes them
int read_line(void)
{
    char command[SIZE];//size is 30
    char **argument;//used to get the argument from the input command

    printf("Enter a command: ");
    fgets(command, SIZE, stdin);//gets command from the user

    //fgets adds a newline "\n" at the end of the string,so this adds a null character to remove it
    command[strlen(command)-1]= '\0';

    //help command
    if(strncmp(command,"help", 4) ==0)
    {
        argument=parse(command);
        help_command(argument[1]);
    }

    //cd command
    else if(strncmp(command,"cd", 2)==0)
    {
        argument=parse(command);
        cd_command(argument[1]);
    }

    //pwd command
    else if(strncmp(command,"pwd", 3)==0)
    {
        argument=parse(command);
        //means there is no argument with pwd command
        if(argument[1]==NULL)
            pwd_command();

        else
            printf("%sError: Too many arguments%s \n", red , reset);
    }
    //ls command
    else if(strncmp(command,"ls", 2)==0)
    {
        argument=parse(command);
        //means there is no argument with ls command
        if(argument[1]==NULL)
            ls_command();

        else
            printf("%sError: Too many arguments%s \n", red , reset);
    }
    //mkdir command
    else if(strncmp(command,"mkdir", 5)==0)
    {
        argument=parse(command);
        mkdir_command(argument[1]);
    }
    //rmdir command
    else if(strncmp(command,"rmdir", 5)==0)
    {
        argument=parse(command);
        rmdir_command(argument[1]);
    }
    //cat command
    else if(strncmp(command,"cat", 3)==0)
    {
        argument=parse(command);
        cat_command(argument[1]);
    }
    //touch command
    else if(strncmp(command,"touch", 5)==0)
    {
        argument=parse(command);
        touch_command(argument[1]);
    }
    //grep command
    else if(strncmp(command,"grep", 4)==0)
    {
        argument=parse(command);
        grep_command(argument);
    }
    //used to exit out of shell
    else if(strcmp(command,"exit")==0)
    {
        return 1;
    }
    //Command not recognized
    else
        printf("%sERROR: Command not found%s \n", red , reset);

    puts(" ");
    return 0;
}

//parse the input command
char **parse(char *my_line)
{
    //this allocates 32 bytes of memory of char type for pointer to store the arguments from user; stores 32 characters
    char **word=malloc(32 * sizeof(char*));//the malloc is needed here, otherwise the pointer is uninitialized

    int n=0;

    char *tokenPtr = strtok(my_line, " ");

    //this means that there is no argument with the command
    if(tokenPtr == NULL)
    {
        word[n]=tokenPtr;
        return word;//NULL will be in "word"
    }

    else
    {
        while(tokenPtr!=NULL)
        {
            word[n]=tokenPtr;;
            n++;
            tokenPtr=strtok(NULL,"  ");//gets the next token
        }
        word[n]=tokenPtr;//includes the last argument which will be NULL
        return word;
    }

}

//displays the available commands
void help_command(char *argument)
{
    //means there is no argument with help command
    if(argument==NULL)
    {
        printf("Commands \n");
        printf("help : This command will output the list of available commands.\n");
        printf("cd : This command lets you change the current working directory.\n");
        printf("pwd: This command will display the current working directory.\n");
        printf("ls: This command will display the contents of the current directory\n");
        printf("mkdir : This command will create a folder within directory.\n");
        printf("rmdir : This command will remove a folder within directory.\n");
        printf("cat : This command will display the contents within a file.\n");
        printf("touch : This command will create a file.\n");
        printf("grep : This command searches for a case insensitive word.\n");
        printf("exit: This command ends the shell.\n");
    }

    else
        printf("%sError: Too many arguments%s \n", red , reset);
}

//changes the current directory
void cd_command(char *argument)
{
    if(argument==NULL)
        printf("%sDirectory name not entered%s \n", red , reset);

    else
    {
        int change=chdir(argument);//this changes the directory

        if(change==0)
            printf("%sThe directory has been changed%s \n", blue , reset);

        else
            printf("%sDirectory couldn't be changed%s \n", red , reset);
    }
}

//gets the name of the current directory
void pwd_command()
{
    char current[300];//used to store the name of the current directory

    //getcwd function comes from <unistd.h> header
    if (getcwd(current, sizeof(current)) == NULL)
        printf("Directory couldn't be displayed\n");

    else
    {
        printf("The current directory is: \n %s \n", current);
        printf("%sCommand complete%s \n", blue , reset);
    }

}

//displays the contents inside of current directory
void ls_command()
{
    //DIR type and struct dirent come from the <dirent.h> header
    struct dirent *dir; //pointer for directory

    DIR *folder= opendir(".");//opens the directory
    int file=0;//used to count how many files are in the directory

    if(folder==NULL)
        printf("%sDirectory couldn't be opened%s \n", red , reset);

    else
    {
        //reads the directory until it reaches the end
        while((dir=readdir(folder))!=NULL)
        {
            file++;
            printf("File %d: %s\n", file, (*dir).d_name);//prints out file name
        }
        printf("%sCommand complete%s \n", blue , reset);
    }

}

//Creates a folder in the directory
void mkdir_command(char *argument)
{
    //means there is no argument with mkdir command
    if(argument==NULL)
        printf("%sERROR: Argument is empty%s \n", red , reset);

    else
    {
        int check = mkdir(argument);//creates the folder

        if(check==0)
            printf("%sDirectory created%s \n", blue , reset);

        else
            printf("%sDirectory couldn't be created%s \n", red , reset);
    }

}

//removes a folder in the directory
void rmdir_command(char *argument)
{
    //means there is no argument with rmdir command
    if(argument==NULL)
        printf("%sERROR: Argument is empty%s \n", red , reset);

    else
    {
        int remove = rmdir(argument);//removes the folder

        if(remove==0)
            printf("%sDirectory has been removed%s \n", blue , reset);

        else
        {
            //the errno variable and ENOENT come from the errno.h header
            if(errno==ENOENT)//ENOENT means directory doesnt exist
                printf("%sDirectory doesn't exist%s \n", red , reset);

            else
                printf("%sDirectory could not be removed%s \n", red , reset);
        }
    }
}

//Displays the contents within a file
void cat_command(char *argument)
{
    FILE *catPtr;
    char words[100];

    if(argument==NULL)
    {
        printf("%sERROR: Argument is empty%s \n", red , reset);
        return;//exits out the function, no need to continue if there is no second argument
    }
    //opens the file for reading
    else if((catPtr=fopen(argument, "r"))==NULL)//checks to see if file exists
        printf("%sFile doesn't exists%s \n", red , reset);

    else if (fgets(words, 100, catPtr)==NULL)
        printf("%sThis file is empty%s \n", red , reset);

    else
    {
        rewind(catPtr);//returns cfPtr pointer to the beginning of the file
        while( fgets(words, 100, catPtr)!= NULL)// reads the file line by line
        {
            printf("%s",words);
        }

        printf("%s\nCommand complete%s \n", blue , reset);
    }

    fclose(catPtr); //closes the file
}

//Creates a new file
void touch_command(char *argument)
{
    FILE *cfPtr;

    if(argument==NULL)
    {
        printf("%sERROR: Argument is empty%s \n", red , reset);
        return;
    }

    else if((cfPtr=fopen(argument, "r"))!=NULL)//checks to see if file already exists
        printf("%sFile already exists%s \n", red , reset);

    else if((cfPtr=fopen(argument, "a"))==NULL)//this will be used to create the file
        printf("%sFile couldn't be created%s \n", red , reset);

    else
        printf("%sThe file was created%s \n", blue , reset);

    fclose(cfPtr); //closes the file
}

//Searches for requested word in file
void grep_command(char **argument)
{
    FILE *cfPtr;
    char word[20];

    char userword[20];

     if(argument[1]==NULL)
    {
        printf("%sERROR: No word given.%s \n", red , reset);
        return;
    }

    if(argument[2]==NULL)
    {
        printf("%sERROR: No file name given.%s \n", red , reset);
        return;
    }

    else if((cfPtr=fopen(argument[2],"r"))==NULL)
        printf("%sFile doesn't exist%s \n", red , reset);

    else if (fgets(word, 20, cfPtr)==NULL)
        printf("%sThis file is empty%s \n", red , reset);

    else
    {
        rewind(cfPtr); //returns cfPtr pointer to the beginning of the file

        strcpy(userword,argument[1]);//copies unedited search word into string array

        strlwr(argument[1]);//makes the searched word lowercase

        while( !feof(cfPtr) )//end of the file
        {
            fscanf(cfPtr, "%s", word);

            char *fileword=strlwr(word);//changes the word from the file into lowercase

            if(strcmp(argument[1], fileword)==0)// compares the words
            {
                printf("%sThe word '%s' is in the file.%s \n", blue, userword, reset);//the word was found in the file
                return;//used to end function
            }
        }
        printf("The word '%s' is not in the file. \n", userword);//this means the pointer has reached the end of the file
        printf("%sCommand complete%s \n", blue , reset); //lets you know that command is complete
    }

    fclose(cfPtr); //closes the file
}

int main(void)
{
    int done=0;

    system("color 07");//this system function is needed to show colors in the shell, otherwise they wont appear

    puts("Enter \"help\" to view all the commands in the shell.");

    //gets commands from the user until "exit" is entered
    while(done==0)
    {
        done=read_line();
    }

    return 0;
}
