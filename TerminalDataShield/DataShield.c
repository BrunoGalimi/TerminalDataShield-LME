#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <direct.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifdef _WIN32
    #include <direct.h>
    #define mkdir _mkdir
#else
    #include <unistd.h>
#endif
#include <windows.h>
#include "LME.h"
#include <unistd.h>
#include <time.h>

void usersAccessGraphics();
void setGeneralPath();
int isDirectoryExists(const char *path);
void getCommandScript();
void getAndSetPrefixedCommandsInStruct();
void setStandardCommandScript();
int isFileEmpty(const char *path);
int isFileExists(const char *path);
void printInput(char place[]);
void trim(char *str);
bool processCommand(char input[], char place[]);
void getUsersAndPrint();
void printResponseTerminal();
void printHelp(char place[]);
void getMd5Result(char str[]);
void md5(uint8_t *initial_msg, size_t initial_len);
void printResponseInput();
void createVaultFunction(char accountName[]);
void addUserVaultToFile(char accountName[], char pass[]);
bool checkVaultAccountExist(char accountName[]);
void deleteVaultFunction(char accountName[]);
void getInvisibleInputString(char str[]);
void obtainStringId(char str[]);
void logInVault(char accountName[], char input[]);
void showItemsInVault();
void addItemInVault();
void deleteItemInVault(char elementNameToDelete[]);
void nukeVault();
void setStyle(char style[]);
void searchItem(char toSearch[]);
void setBitPower(char bitPower[]);
void rerollEncryption(int decodeBit);
void printEncryptionInformation();
void showSpecificItem(char elementName[]);
void editItemInVault(char elementNameToEdit[]);


struct commandScript{
	char CommandPrefix[3];
	char SetInputStyle[30];
	char Help[30];//help
	char HelpMinimize[30];//h
	char ShowsAllVaultsAccount[30];//showsAllVaultsAccount 
	char ShowsAllVaultsAccountMinimize[30];//showVaults 
	char CreateVault[30];//create a user vault
	char DeleteVault[30];//delete a user vault
	char ShowAllPasswords[30];//showPassword
	char ShowAllPasswordsMinimize[30];//sp
	char ShowPassword[30];//showPassword
	char ShowPasswordMinimize[30];//sp
	char Search[30];//search item: Saved: "Gmail" , "GoogleMaps" , "Steam" --> search("G") --> return --> "Gmail" , "GoogleMaps"
	char EncryptionInformation[30]; //print the encryption information
	char Add[30];//add
	char AddMinimize[30];//a
	char DeletePassword[30];//delete
	char DeletePasswordMinimize[30];//del
	char Edit[30];//edit
	char Enter[30];//enter
	char SetBitEncryption[30];//set the bit of the encryption (standard: 9)
	char RerollEncryption[30]; //refresh the encryption of the data, generate new kpub for each item, and encrypt the data
	char RerollEncryptionMinimize[30]; //refresh the encryption of the data, generate new kpub for each item, and encrypt the data
	char Nuke[30];//Nuke vault
	char Cls[30];//exit
	char Exit[30];//exit
};

struct generalPath{
	char prefixedFolderDataFilePath[100];
	char prefixedFolderUserVaultDataFilePath[100];
	char pathScriptCommand[100];
	char pathVault[100];
	char tempFile[100];
	char tempBinaryFile[100];
};

struct vault{
	char username[100];
	char password[100];
	char fileVaultId[100];
	int LMEBITENCODE;//for LME encode
	char inputStyle[100];
};

struct vaultItem{
	char elementName[1000000];//example: gmail
	char email[1000000];//example: mygmail@gmail.com
	char password[1000000];//example: Password1!
	char whenCreated[1000000];//save when the item has been created
	char whenModified[1000000];//save when the item has been edited
	char kpub[1000000];//example: Password1!
};

typedef struct commandScript CommandScript;
typedef struct generalPath GeneralPath;
typedef struct vault Vault;
typedef struct vaultItem VaultItem;

//global variables
GeneralPath	generalPath;
CommandScript commandScript;
Vault currentVault;
VaultItem vaultItem;

//global variables for place
char PlaceAccess[] = "/Access";
char PlacePasswordVault[] = "/PasswordVault";

//global variables for inputStyle
char DebianStyle[] = "Debian";
char VanillaStyle[] = "Vanilla";
char WindowsBasicStyle[] = "WindowsBasic";

char NotAlreadyModified[] = "Not already modified.";



//for MD5
// leftrotate function definition
#define LEFTROTATE(x, c) (((x) << (c)) | ((x) >> (32 - (c))))
// These vars will contain the hash
uint32_t h0, h1, h2, h3;
//



main()
{

	//variables
	char input[] = " ";
	
//char 250 --> center float point

	//set prefixed path in the structure
	setGeneralPath();
	//get the prefixed commands scripts in the file (commandScript file) and set in the commandScript structure
	getCommandScript();

	do
	{
		strcpy(currentVault.inputStyle, VanillaStyle);//set the standard not login input interface the Debian Style.
		printInput(PlaceAccess);
		gets(input);


		//return true --> wrong command, else return false if command exist in the location
		bool wrongCommand = processCommand(input, PlaceAccess);
			if(wrongCommand)
			{
				printResponseTerminal();
				printf("Wrong command.\n");
			}else
			{
				if(strcmp(input, "joinVault") == 0)
				{
					do
					{
						printf("\n");
						printInput(PlacePasswordVault);
							gets(input);
						bool wrongCommand = processCommand(input, PlacePasswordVault);
							if(wrongCommand)
							{
								printResponseTerminal();
								printf("Wrong command.\n");
							}

					}while(strcmp(commandScript.Exit, input) != 0);
					strcpy(input, "");
				}
			}
	}while(strcmp(commandScript.Exit, input) != 0);
}


//graphics function (frontEnd):

//place --> where are in the terminale(enter, add password, settings...)
//if place is "Enter" the user is Unknow, else it will keep in the structure
void printInput(char place[])
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	printf("\n\n\n ");
	
	if(strcmp(currentVault.inputStyle, DebianStyle) == 0)
	{
		SetConsoleTextAttribute(hConsole, 8);
		printf("%c%c%c", 218, 196, 196);
			SetConsoleTextAttribute(hConsole, 7);
		printf("(");
			SetConsoleTextAttribute(hConsole, 3);
		printf("%s ", commandScript.CommandPrefix);
		printf("@");
			if(strcmp(place, PlaceAccess) == 0)
				printf("Unknown");
			else
				printf("%s", currentVault.username);
				
			SetConsoleTextAttribute(hConsole, 7);
		printf(")");
			SetConsoleTextAttribute(hConsole, 8);
		printf("-");
			SetConsoleTextAttribute(hConsole, 7);
		printf("[");
			SetConsoleTextAttribute(hConsole, 5);
		printf("%c", 126);
			SetConsoleTextAttribute(hConsole, 5);
		printf("%s", place);
			SetConsoleTextAttribute(hConsole, 7);
		printf("]\n ");
			SetConsoleTextAttribute(hConsole, 8);
		printf("%c", 192);
			SetConsoleTextAttribute(hConsole, 3);
		printf("%c  ", 245);
	}else if(strcmp(currentVault.inputStyle, VanillaStyle) == 0)
	{
			SetConsoleTextAttribute(hConsole, 7);
		printf("(");
			SetConsoleTextAttribute(hConsole, 3);
		printf("%s", commandScript.CommandPrefix);
			SetConsoleTextAttribute(hConsole, 8);
		printf(" %c ", 126);
			SetConsoleTextAttribute(hConsole, 5);
		printf("%s", place);
			SetConsoleTextAttribute(hConsole, 8);
		printf(" %c ", 126);
			SetConsoleTextAttribute(hConsole, 3);
		printf("@");
			if(strcmp(place, PlaceAccess) == 0)
					printf("Unknown");
				else
					printf("%s", currentVault.username);
		SetConsoleTextAttribute(hConsole, 7);
		printf(")");
			SetConsoleTextAttribute(hConsole, 8);
		printf("%c", 196);
		SetConsoleTextAttribute(hConsole, 3);
		printf("%c  ", 245);
	}else if(strcmp(currentVault.inputStyle, WindowsBasicStyle) == 0)
	{
			SetConsoleTextAttribute(hConsole, 7);
		printf("%s", commandScript.CommandPrefix);
			SetConsoleTextAttribute(hConsole, 8);
		printf("|");
			SetConsoleTextAttribute(hConsole, 7);
		printf("%s", place);
			SetConsoleTextAttribute(hConsole, 8);
		printf("|");
			SetConsoleTextAttribute(hConsole, 7);
		printf("@");
			if(strcmp(place, PlaceAccess) == 0)
					printf("Unknown");
				else
					printf("%s", currentVault.username);
		printf(">  ");
	}
		
//--(. - Access - @Guest)- #
//(--@Guest)-[%c/Access]\n

	
	
	SetConsoleTextAttribute(hConsole, 7);

}

void printResponseInput()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		SetConsoleTextAttribute(hConsole, 3);
	printf("\n  %c  ", 245);
		SetConsoleTextAttribute(hConsole, 7);
}

void usersAccessGraphics()
{
	
	FILE *file;

	//in caso non esiste lo crea
	file = fopen("UserAccess.txt", "a");
	fclose(file);
	
	file = fopen("UserAccess.txt", "a");




	
	printf("For login: --enter username password\n\n");
}

void printHelp(char place[]){
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	printResponseTerminal();

	printf("Help command list [");
		SetConsoleTextAttribute(hConsole, 5);
	printf("%c%s", 126, place);
		SetConsoleTextAttribute(hConsole, 7);
	printf("]:");

	//global print
	printf("\n\n\tThe [] should not be written when the command is typed.\n\n");
	
	//help
	printf("\n\t%s , %s\t", commandScript.Help, commandScript.HelpMinimize);
		SetConsoleTextAttribute(hConsole, 8);
		printf(":Print the commands and explanation for each.");
		SetConsoleTextAttribute(hConsole, 7);

	//show user vaults account
	printf("\n\t%s , %s\t", commandScript.ShowsAllVaultsAccount, commandScript.ShowsAllVaultsAccountMinimize);
		SetConsoleTextAttribute(hConsole, 8);
		printf(":Shows login usernames for all created vaults accounts.");
		SetConsoleTextAttribute(hConsole, 7);
		
		
	//clean the terminal
	printf("\n\t%s\t", commandScript.Cls);
		SetConsoleTextAttribute(hConsole, 8);
		printf(":Clean the terminal.");
		SetConsoleTextAttribute(hConsole, 7);
		
			
	//exit the terminal
	printf("\n\t%s\t", commandScript.Exit);
		SetConsoleTextAttribute(hConsole, 8);
		
		if(strcmp(place, PlaceAccess) == 0)
			printf(":Exit the terminal.\n\n");
		else if(strcmp(place, PlacePasswordVault) == 0)
			printf(":Back to access.\n\n");
			
		SetConsoleTextAttribute(hConsole, 7);

	

	if(strcmp(place, PlaceAccess) == 0)
	{
		//login(enter command)
		printf("\n\t%s [", commandScript.Enter);
			SetConsoleTextAttribute(hConsole, 3);
		printf("username", place);
			SetConsoleTextAttribute(hConsole, 7);
		printf("]\t");
			SetConsoleTextAttribute(hConsole, 8);
		printf(":Log in to the account corresponding to the username you entered.");
		printf("\n\t\t\t\t Once submitted you will be asked for your password.\n");


		SetConsoleTextAttribute(hConsole, 7);
		
		
		//create user vault(create user command)
		printf("\n\t%s [", commandScript.CreateVault);
			SetConsoleTextAttribute(hConsole, 3);
		printf("username", place);
			SetConsoleTextAttribute(hConsole, 7);
		printf("]\t");
			SetConsoleTextAttribute(hConsole, 8);
		printf(":User vault creation.");
		printf("\n\t\t\t\t Once submitted you will be asked for the password for the account,");
		printf("\n\t\t\t\t and then asked to confirm.\n");
		
		
		SetConsoleTextAttribute(hConsole, 7);

		
		//delete user vault(delete user command)
		printf("\n\t%s [", commandScript.DeleteVault);
			SetConsoleTextAttribute(hConsole, 3);
		printf("username", place);
			SetConsoleTextAttribute(hConsole, 7);
		printf("]\t");
			SetConsoleTextAttribute(hConsole, 8);
		printf(":Deleting user vault.");
		printf("\n\t\t\t\t Once you have sent the command, with the name of the user you want to delete,");
		printf("\n\t\t\t\t you will be asked for the password and finally a confirmation of the deletion.\n");
	}
	
	
	if(strcmp(place, PlacePasswordVault) == 0)
	{
		
		//set the input style graphic
		printf("\n\t%s [", commandScript.SetInputStyle);
			SetConsoleTextAttribute(hConsole, 3);
		printf("StyleName");
			SetConsoleTextAttribute(hConsole, 7);
		printf("]\t");		
			SetConsoleTextAttribute(hConsole, 8);
		printf("\t:Set the graphic input style.");
		printf("\n\t\t\t\t\t Avaible StyleNames:");
		printf("\n\t\t\t\t\t \"%s\": \t", DebianStyle);
		printf("%c%c%c", 218, 196, 196);
			SetConsoleTextAttribute(hConsole, 7);
		printf("(");
			SetConsoleTextAttribute(hConsole, 3);
		printf("%s ", commandScript.CommandPrefix);
		printf("@Username");
				
			SetConsoleTextAttribute(hConsole, 7);
		printf(")");
			SetConsoleTextAttribute(hConsole, 8);
		printf("-");
			SetConsoleTextAttribute(hConsole, 7);
		printf("[");
			SetConsoleTextAttribute(hConsole, 5);
		printf("%c", 126);
			SetConsoleTextAttribute(hConsole, 5);
		printf("/Place");
			SetConsoleTextAttribute(hConsole, 7);
		printf("]\n ");
			SetConsoleTextAttribute(hConsole, 8);
		printf("\t\t\t\t\t\t\t%c", 192);
			SetConsoleTextAttribute(hConsole, 3);
		printf("%c  ", 245);
		printf("\n");
		
		SetConsoleTextAttribute(hConsole, 8);
		printf("\n\t\t\t\t\t \"%s\": \t", VanillaStyle);
		SetConsoleTextAttribute(hConsole, 7);
		printf("(");
			SetConsoleTextAttribute(hConsole, 3);
		printf("%s", commandScript.CommandPrefix);
			SetConsoleTextAttribute(hConsole, 8);
		printf(" %c ", 126);
			SetConsoleTextAttribute(hConsole, 5);
		printf("/Place");
			SetConsoleTextAttribute(hConsole, 8);
		printf(" %c ", 126);
			SetConsoleTextAttribute(hConsole, 3);
		printf("@Username");
			SetConsoleTextAttribute(hConsole, 7);
		printf(")");
			SetConsoleTextAttribute(hConsole, 8);
		printf("%c", 196);
			SetConsoleTextAttribute(hConsole, 3);
		printf("%c  ", 245);
		printf("\n");
			SetConsoleTextAttribute(hConsole, 8);
		printf("\n\t\t\t\t\t \"%s\": ", WindowsBasicStyle);
			SetConsoleTextAttribute(hConsole, 7);
		printf("%s", commandScript.CommandPrefix);
			SetConsoleTextAttribute(hConsole, 8);
		printf("|");
			SetConsoleTextAttribute(hConsole, 7);
		printf("/Place");
			SetConsoleTextAttribute(hConsole, 8);
		printf("|");
			SetConsoleTextAttribute(hConsole, 7);
		printf("@Username");
		printf(">");
		
		printf("\n");
		
		
				printf("\n");
			SetConsoleTextAttribute(hConsole, 7);
		
		
		
		
		
		//save password(vault command)
		printf("\n\t%s [", commandScript.Add);
			SetConsoleTextAttribute(hConsole, 3);
		printf("element name", place);
			SetConsoleTextAttribute(hConsole, 7);
		printf("]");		
		printf(" , ");
		printf("%s [", commandScript.AddMinimize);
			SetConsoleTextAttribute(hConsole, 3);
		printf("element name");
			SetConsoleTextAttribute(hConsole, 7);
		printf("]\t");
			SetConsoleTextAttribute(hConsole, 8);
		printf(":Add a password to the vault.");
		printf("\n\t\t\t\t\t\t The element name will be the name by which the password");
		printf("\n\t\t\t\t\t\t will be recognized.");
		printf("\n\t\t\t\t\t\t For example, if I am saving the password of my email account,");
		printf("\n\t\t\t\t\t\t the element name could be Gmail or Virgilio");
		printf("\n\t\t\t\t\t\t depending on how you want to save it.\n");



			SetConsoleTextAttribute(hConsole, 7);
		
		//Shows encryption information of saved data.
		printf("\n\t%s\t\t", commandScript.EncryptionInformation);		
			SetConsoleTextAttribute(hConsole, 8);
		printf(":Shows encryption information of saved data.\n");

			SetConsoleTextAttribute(hConsole, 7);
		
		
		//show all passwords(vault command)
		printf("\n\t%s", commandScript.ShowAllPasswords);		
		printf(" , ");
		printf("%s", commandScript.ShowAllPasswordsMinimize);
		printf("\t");
			SetConsoleTextAttribute(hConsole, 8);
		printf(":Show all the passwords saved in the vault.");


			SetConsoleTextAttribute(hConsole, 7);
			
		//show specific password(vault command)
		printf("\n\t%s [", commandScript.ShowPassword);
			SetConsoleTextAttribute(hConsole, 3);
		printf("element name", place);
			SetConsoleTextAttribute(hConsole, 7);
		printf("]");		
		printf(" , ");
		printf("%s [", commandScript.ShowPasswordMinimize);
			SetConsoleTextAttribute(hConsole, 3);
		printf("element name");
			SetConsoleTextAttribute(hConsole, 7);
		printf("]\t");
			SetConsoleTextAttribute(hConsole, 8);
		printf(":Shows the element([element name]) in the vault.\n");


			SetConsoleTextAttribute(hConsole, 7);
		
		//delete password(vault command)
		printf("\n\t%s [", commandScript.DeletePassword);
			SetConsoleTextAttribute(hConsole, 3);
		printf("element name", place);
			SetConsoleTextAttribute(hConsole, 7);
		printf("]");		
		printf(" , ");
		printf("%s [", commandScript.DeletePasswordMinimize);
			SetConsoleTextAttribute(hConsole, 3);
		printf("element name", place);
			SetConsoleTextAttribute(hConsole, 7);
		printf("]\t");
			SetConsoleTextAttribute(hConsole, 8);
		printf(":Delete a password from the vault.");
		printf("\n\t\t\t\t\t\t\t The element name will is the name by which the password");
		printf("\n\t\t\t\t\t\t\t is be recognized.");
		printf("\n\t\t\t\t\t\t\t The password corresponding to the element name");
		printf("\n\t\t\t\t\t\t\t entered when compiling the command will be deleted.\n");		
		
			SetConsoleTextAttribute(hConsole, 7);
			
		//edit item
		printf("\n\t%s [", commandScript.Edit);
			SetConsoleTextAttribute(hConsole, 3);
		printf("element name");
			SetConsoleTextAttribute(hConsole, 7);
		printf("]\t");		
			SetConsoleTextAttribute(hConsole, 8);
		printf("\t:Edit the parameters of the saved element, selected by [element name]\n");
			
			SetConsoleTextAttribute(hConsole, 7);
			
		//search item by keyworld (vault command) : //search item: Saved: "Gmail" , "GoogleMaps" , "Steam" --> search("G") --> return --> "Gmail" , "GoogleMaps"
		printf("\n\t%s [", commandScript.Search);
			SetConsoleTextAttribute(hConsole, 3);
		printf("to search");
			SetConsoleTextAttribute(hConsole, 7);
		printf("]\t");		
			SetConsoleTextAttribute(hConsole, 8);
		printf("\t:Search the vault if there are any items whose");
		printf("\n\t\t\t\t\t element name begins with \"to search\".");
			
			SetConsoleTextAttribute(hConsole, 7);


		//Set_Bit_Encryption_Power (vault command)
		printf("\n\t%s [", commandScript.SetBitEncryption);
			SetConsoleTextAttribute(hConsole, 3);
		printf("bit");
			SetConsoleTextAttribute(hConsole, 7);
		printf("]\t");		
			SetConsoleTextAttribute(hConsole, 8);
		printf("\t:Set the encoding bits to use to save data.");
		printf("\n\t\t\t\t\t Once you use this command it will be as if you had");
		printf("\n\t\t\t\t\t also used the command: \"%s\"\n", commandScript.RerollEncryption);	

			SetConsoleTextAttribute(hConsole, 7);

		//reroll encryption(vault command)
		printf("\n\t%s", commandScript.RerollEncryption);		
		printf(" , ");
		printf("%s", commandScript.RerollEncryptionMinimize);
		printf("\t");
			SetConsoleTextAttribute(hConsole, 8);
		printf(":Re-encode all data.");
		printf("\n\t\t\t\t\t Generates a new public key for each saved data,");
		printf("\n\t\t\t\t\t and re-encodes the element with the new private key.\n");

			SetConsoleTextAttribute(hConsole, 7);


		//nuke all password(vault command)
		printf("\n\t%s\t\t", commandScript.Nuke);		
			SetConsoleTextAttribute(hConsole, 8);
		printf(":!WARNING!");
		printf("\n\t\t\t Once you confirm this command ALL items saved in the vault");
		printf("\n\t\t\t will be deleted, without the possibility of being recovered.\n");

			SetConsoleTextAttribute(hConsole, 7);

	}
	

	
	

	
	printf("\n\n");
}

//prefixed response character(of terminal)
void printResponseTerminal()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 3);
	printf("\n # ");
	SetConsoleTextAttribute(hConsole, 7);

}

//







//backend functions:

bool processCommand(char input[], char place[])
{
	char temp[] = " ";
	bool isWrong = true;
	char splittedCommand[3][50];
	
	//check how many word have a command
	//res --> how many words compose a string
	int res = sscanf(input, "%s %s %s", splittedCommand[0], splittedCommand[1], splittedCommand[2]);
	
	//help command
	if(strcmp(splittedCommand[0], commandScript.Help) == 0 || strcmp(splittedCommand[0], commandScript.HelpMinimize) == 0 && res == 1)
	{
		printHelp(place);
		isWrong = false;
	}
	
	//cls terminal
	if(strcmp(splittedCommand[0], commandScript.Cls) == 0 && res == 1)
	{
		system("cls");
		isWrong = false;
	}

	//show all vaults account command
	if(strcmp(splittedCommand[0], commandScript.ShowsAllVaultsAccount) == 0 || strcmp(splittedCommand[0], commandScript.ShowsAllVaultsAccountMinimize) == 0 && res == 1)
	{
		getUsersAndPrint();
		isWrong = false;
	}
	
	
	//log in to a vault account
	if(strcmp(splittedCommand[0], commandScript.Enter) == 0 && strcmp(place, PlaceAccess) == 0 && res == 2)
	{
		if(checkVaultAccountExist(splittedCommand[1]))//if the user vault written exist
		{
			logInVault(splittedCommand[1], input);
			isWrong = false;
		}else//write the name doesn't exist error
		{
			printResponseTerminal();
			printf("There is no user vault with this name\n");
			isWrong = false;//This way I don't write the generic error message and tell it that a user with that name doesn't exist
		}
	}
	
	//createVault command ( only /Access)
	if(strcmp(splittedCommand[0], commandScript.CreateVault) == 0 && strcmp(place, PlaceAccess) == 0 && res == 2)
	{
		if(checkVaultAccountExist(splittedCommand[1]) == false)
		{
			createVaultFunction(splittedCommand[1]);
			isWrong = false;
		}else//write the user already exist error
		{
			printResponseTerminal();
			printf("A user vault with this name already exists.\n");
			isWrong = false;//This way I don't write the generic error message and tell it that a user with that name already exist
		}
		
		//splitCompoundCommands(splittedCommand, 2);
	}
	
	//delete vault (only /Access)
	if(strcmp(splittedCommand[0], commandScript.DeleteVault) == 0 && strcmp(place, PlaceAccess) == 0 && res == 2)
	{
		if(checkVaultAccountExist(splittedCommand[1]))
		{
			deleteVaultFunction(splittedCommand[1]);
			isWrong = false;
		}else//write the name doesn't exist error
		{
			printResponseTerminal();
			printf("There is no user vault with this name\n");
			isWrong = false;//This way I don't write the generic error message and tell it that a user with that name doesn't exist
		}
	}
	
	
	//showItems
	if(strcmp(splittedCommand[0], commandScript.ShowAllPasswords) == 0 || strcmp(splittedCommand[0], commandScript.ShowAllPasswordsMinimize) == 0 && strcmp(place, PlacePasswordVault) == 0 && res == 1)
	{
		showItemsInVault();
		isWrong = false;
	}
	
	//show specific item
	if(strcmp(splittedCommand[0], commandScript.ShowPassword) == 0 || strcmp(splittedCommand[0], commandScript.ShowPasswordMinimize) == 0 && strcmp(place, PlacePasswordVault) == 0 && res == 2)
	{
		showSpecificItem(splittedCommand[1]);
		isWrong = false;
	}
	
	//setStyle
	if(strcmp(splittedCommand[0], commandScript.SetInputStyle) == 0 && strcmp(place, PlacePasswordVault) == 0 && res == 2)
	{
		if(strcmp(splittedCommand[1], DebianStyle) == 0 || strcmp(splittedCommand[1], VanillaStyle) == 0 || strcmp(splittedCommand[1], WindowsBasicStyle) == 0)
		{
			setStyle(splittedCommand[1]);
				isWrong = false;
		}
	}

	
	//search item command
	if(strcmp(splittedCommand[0], commandScript.Search) == 0 && strcmp(place, PlacePasswordVault) == 0 && res == 2)
	{
		searchItem(splittedCommand[1]);
		isWrong = false;
	}

	//add item
	if(strcmp(splittedCommand[0], commandScript.Add) == 0 || strcmp(splittedCommand[0], commandScript.AddMinimize) == 0 && strcmp(place, PlacePasswordVault) == 0 && res == 2)
	{
		addItemInVault(splittedCommand[1]);
		isWrong = false;
	}

	//delete item
	if(strcmp(splittedCommand[0], commandScript.DeletePassword) == 0 || strcmp(splittedCommand[0], commandScript.DeletePasswordMinimize) == 0 && strcmp(place, PlacePasswordVault) == 0 && res == 2)
	{
		deleteItemInVault(splittedCommand[1]);
		isWrong = false;
	}
	
	//edit item
	if(strcmp(splittedCommand[0], commandScript.Edit) == 0 && strcmp(place, PlacePasswordVault) == 0 && res == 2)
	{
		editItemInVault(splittedCommand[1]);
		isWrong = false;
	}
	
	
	//setBitPower command
	if(strcmp(splittedCommand[0], commandScript.SetBitEncryption) == 0 && strcmp(place, PlacePasswordVault) == 0 && res == 2)
	{
		setBitPower(splittedCommand[1]);
		isWrong = false;
	}
	
	//Reroll ecnryption all data
	if(strcmp(splittedCommand[0], commandScript.RerollEncryption) == 0 || strcmp(splittedCommand[0], commandScript.RerollEncryptionMinimize) == 0 && strcmp(place, PlacePasswordVault) == 0 && res == 1)
	{
		rerollEncryption(currentVault.LMEBITENCODE);
		isWrong = false;
	}
	
	//encryption information
	if(strcmp(splittedCommand[0], commandScript.EncryptionInformation) == 0 && strcmp(place, PlacePasswordVault) == 0 && res == 1)
	{
		printEncryptionInformation();
		isWrong = false;
	}
	
	
	//nuke vault
	if(strcmp(splittedCommand[0], commandScript.Nuke) == 0 && strcmp(place, PlacePasswordVault) == 0 && res == 1)
	{
		nukeVault();
		isWrong = false;
	}
	
	//exit command
	if(strcmp(splittedCommand[0], commandScript.Exit) == 0 && res == 1)
	{
		
		if(strcmp(place, PlacePasswordVault) == 0)
		{
			
			//set value vault user to zero
			strcpy(currentVault.fileVaultId, "");
			strcpy(currentVault.inputStyle, "");
			currentVault.LMEBITENCODE = 0;
			strcpy(currentVault.password, "");
			strcpy(currentVault.username, "");



			
			printResponseTerminal();
			printf("Redirected to the access.");
		}
		
		
		isWrong = false;
	}
	
	
	
	
	
	
	if(strcmp(splittedCommand[0], ".lisa") == 0 && res == 1 && strcmp(place, PlaceAccess) == 0)
	{
		printResponseTerminal();
			printf("A creator who hates his own creation..\n");
		printResponseTerminal();
			printf("A hidden key, a leap not taken..\n");
		printResponseTerminal();
			printf("Retrace your steps, escape your past..\n");
		printResponseTerminal();
			printf("And the key of jade will be yours at last..\n");
		printResponseTerminal();
			printf("What did you have to sacrifice to create something you hate?\n");


		isWrong = false;
	}
	
		

	return isWrong;

}

void setStyle(char style[])
{
	FILE *file;
	FILE *tempFile;
	
	char vaultName[100];
	char pass[100];
	char bit[100];
	char inputStyle[100];
	int i = 0;
	
	//save the file with the style edited

	file = fopen(generalPath.pathVault, "r");
	tempFile = fopen(generalPath.tempFile, "w");

	while(fscanf(file, "%s ; %s ; %s ; %s", vaultName, pass, bit, inputStyle) != EOF)
	{
		if(i != 0)//is not empty
			fprintf(tempFile, "\n");
		
		fprintf(tempFile, "%s ; %s ; %s ; ", vaultName, pass, bit);
			if(strcmp(vaultName, currentVault.username) == 0)//if is the user that is editing his style then save the new style instead of the old
				fprintf(tempFile, "%s", style);
			else
				fprintf(tempFile, "%s", inputStyle);
		i++;
	}

	fclose(tempFile);
	fclose(file);
	
	file = fopen(generalPath.pathVault, "w");
	tempFile = fopen(generalPath.tempFile, "r");
	
	i = 0;
	
	while(fscanf(tempFile, "%s ; %s ; %s ; %s", vaultName, pass, bit, inputStyle) != EOF)
	{
		if(i != 0)//is not empty
			fprintf(file, "\n");
		
		fprintf(file, "%s ; %s ; %s ; ", vaultName, pass, bit);
			if(strcmp(vaultName, currentVault.username) == 0)//if is the user that is editing his style then save the new style instead of the old
				fprintf(file, "%s", style);
			else
				fprintf(file, "%s", inputStyle);
		i++;
	}
	
	
	fclose(tempFile);
	fclose(file);
	
	remove(generalPath.tempFile);
	//
	
	//set to the structure the style
	strcpy(currentVault.inputStyle, style);
	
	printResponseTerminal();
	printf("Input style changed to %s style.", currentVault.inputStyle);
	
	
	
}

void searchItem(char toSearch[])
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	FILE *file;
	file = fopen(currentVault.fileVaultId, "rb");
	bool elementExist = false;//if at least one element exists this variable turn true
	
	//read all item and select only the item with the same "toSearch" character
	//cycle until I find no more items
	while(fread(&vaultItem, sizeof(VaultItem), 1, file) == 1)
	{
			strcpy(vaultItem.elementName, lmeDeAsh(vaultItem.kpub, vaultItem.elementName, currentVault.LMEBITENCODE, currentVault.password));//decript
			strcpy(vaultItem.email, lmeDeAsh(vaultItem.kpub, vaultItem.email, currentVault.LMEBITENCODE, currentVault.password));

			if(strncmp(toSearch, vaultItem.elementName, strlen(toSearch)) == 0 || strncmp(toSearch, vaultItem.email, strlen(toSearch)) == 0)
			{
				if(elementExist == false)
				{
					printResponseTerminal();
					printf("Items searched with keyword \"%s\":\n", toSearch);
				}
				
				
				printf("\n\t%c ", 250);
				//get only the unsearched part of the element name: toSearch = "Gm" , vaultItem.elementName = "Gmail" --> vaultItem.elementName = "ail". I need this to print only the part I'm looking for(toSearch) and color it.
				if(strncmp(toSearch, vaultItem.elementName, strlen(toSearch)) == 0)
				{
					memmove(vaultItem.elementName, vaultItem.elementName + strlen(toSearch), strlen(vaultItem.elementName) - strlen(toSearch) + 1);
						SetConsoleTextAttribute(hConsole, 5);
							printf("%s", toSearch);
				}
					SetConsoleTextAttribute(hConsole, 7);
				printf("%s\n\t    ", vaultItem.elementName);
				
				if(strncmp(toSearch, vaultItem.email, strlen(toSearch)) == 0)
				{
					memmove(vaultItem.email, vaultItem.email + strlen(toSearch), strlen(vaultItem.email) - strlen(toSearch) + 1);
						SetConsoleTextAttribute(hConsole, 5);
							printf("%s", toSearch);
				}
				SetConsoleTextAttribute(hConsole, 3);
					printf("%s\n", vaultItem.email);
						SetConsoleTextAttribute(hConsole, 7);

				elementExist = true;
			}
			
	}
	
	fclose(file);
	
	if(elementExist == false)
	{
		printResponseTerminal();
		printf("There is no item matching the search.");
	}
	
}

void addItemInVault(char elementName[])
{
	
	FILE *file;
	file = fopen(currentVault.fileVaultId, "rb");

	bool elementNameAlreadyExist = false;

	while(fread(&vaultItem, sizeof(VaultItem), 1, file) == 1)
		if(strcmp(elementName, lmeDeAsh(vaultItem.kpub, vaultItem.elementName, currentVault.LMEBITENCODE, currentVault.password)) == 0)
			elementNameAlreadyExist = true;
	
	if(!elementNameAlreadyExist)
	{
		char* publicKey = lmeGenKey(currentVault.LMEBITENCODE);
	    char* elementNameAshed;
	    char* emailAshed;
	    char* passwordAshed;
	    
	    
	    char currentDateString[1000];
	    	strcpy(vaultItem.whenModified, NotAlreadyModified);//When the item is created it has never been created yet so I save it to find out later if it has been modified
	
	    time_t currentTime_struct;
			struct tm *currentTime;
	    	time(&currentTime_struct);
	    currentTime = localtime(&currentTime_struct);
	    
	    //save the public key in the struct for the ash
	    strcpy(vaultItem.kpub, publicKey);
	    
	
	    //save the date when the item is saved
	    	sprintf(currentDateString, "Created on %02d/%02d/%04d at %02d:%02d", currentTime->tm_mday, currentTime->tm_mon + 1, currentTime->tm_year + 1900, currentTime->tm_hour, currentTime->tm_min);
	    strcpy(vaultItem.whenCreated, currentDateString);
		strcpy(vaultItem.whenCreated, lmeAsh(publicKey, vaultItem.whenCreated, currentVault.password));
		strcpy(vaultItem.whenModified, lmeAsh(publicKey, vaultItem.whenModified, currentVault.password));
	    
	
		strcpy(vaultItem.elementName, lmeAsh(publicKey, elementName, currentVault.password));
	
		
		//get the email parameter
		printResponseTerminal();
		printf("Email/Username:");
			printResponseInput();
		gets(vaultItem.email);
			strcpy(vaultItem.email, lmeAsh(publicKey, vaultItem.email, currentVault.password));
	
		
		//get the password parameter
		printResponseTerminal();
		printf("Password:");
			printResponseInput();	
		getInvisibleInputString(vaultItem.password);
		strcpy(vaultItem.password, lmeAsh(publicKey, vaultItem.password, currentVault.password));
	
		
		
		//DA FARE CHE PRIMA DI AGGIUNGERE CONTROLLI SEE ESISTA UNA PASSWORD CON LO STESSO NOME
		file = fopen(currentVault.fileVaultId, "ab");
			fwrite(&vaultItem, sizeof(VaultItem), 1, file);
	
		fclose(file);
	
		//set empty the vaultItem structure. (It wouldn't be useful since they would be initialized immediately before use but for cleanliness I do it)
		strcpy(vaultItem.elementName, "");	
		strcpy(vaultItem.email, "");	
		strcpy(vaultItem.password, "");	
		
		printf("\n");
			printResponseTerminal();
		printf("Password added to vault successfully.");
	}else
	{
			printResponseTerminal();
		printf("It is not possible to save a password in the vault with the same element name as one already saved.");
	}
	
		

}

void printEncryptionInformation()
{
	FILE *file;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	printResponseTerminal();
	printf("Encryption information about %s vault:\n", currentVault.username);
	printf("   Used LME algorithm for two-way encryption\tEncoding bits: %d\n   Used MD5 algorithm for one-way encryption\n\n", currentVault.LMEBITENCODE);
	
	file = fopen(currentVault.fileVaultId, "rb");
		fseek(file, 0, SEEK_END);//set the cursor at the end of file for check if is empty
	
	
	//if the binary file is not empty
	if(ftell(file) != 0)
	{
		fclose(file);
		
		printf("   Public key for password:\n");
			file = fopen(currentVault.fileVaultId, "rb");
			
		//cycle until I find no more items and print(only item and email, without password)
		while(fread(&vaultItem, sizeof(VaultItem), 1, file) == 1)
		{
			strcpy(vaultItem.elementName, lmeDeAsh(vaultItem.kpub, vaultItem.elementName, currentVault.LMEBITENCODE, currentVault.password));
			printf("\n\t%c %s\n", 250, vaultItem.elementName);
				SetConsoleTextAttribute(hConsole, 3);
			printf("%s", vaultItem.kpub);
				SetConsoleTextAttribute(hConsole, 7);
		}
		
	}
				fclose(file);

}

void showItemsInVault()
{
	FILE *file;
	int i;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	file = fopen(currentVault.fileVaultId, "rb");
	fseek(file, 0, SEEK_END);//set the cursor at the end of file for check if is empty
	
	//if the binary file is empty
	if(ftell(file) == 0)
	{
		fclose(file);

		printResponseTerminal();
			printf("There are no passwords saved in this vault yet.");
	}else
	{
		
		file = fopen(currentVault.fileVaultId, "rb");

		printResponseTerminal();
		printf("Vault items:\n");

		//cycle until I find no more items and print(only item and email, without password)
		while(fread(&vaultItem, sizeof(VaultItem), 1, file) == 1)
		{
			strcpy(vaultItem.elementName, lmeDeAsh(vaultItem.kpub, vaultItem.elementName, currentVault.LMEBITENCODE, currentVault.password));
			strcpy(vaultItem.email, lmeDeAsh(vaultItem.kpub, vaultItem.email, currentVault.LMEBITENCODE, currentVault.password));

			printf("\n\t%c %s\n\t    ", 250, vaultItem.elementName);
				SetConsoleTextAttribute(hConsole, 3);
			printf("%s\n", vaultItem.email);
				SetConsoleTextAttribute(hConsole, 7);

		}
		
			fclose(file);
			
			
		strcpy(vaultItem.elementName, "");	
		strcpy(vaultItem.email, "");	
		strcpy(vaultItem.password, "");	

	}
}

void deleteItemInVault(char elementNameToDelete[])
{
	
		FILE *file;
		FILE *tempBinaryFile;
		
		file = fopen(currentVault.fileVaultId, "rb");
		tempBinaryFile = fopen(generalPath.tempBinaryFile, "ab");
		
		bool haveDeletedSomething = false;
		
		char tempElementName[10000] = " ";
		

		//get all the password exept the one who is deserved to delete and save in a temporaney file
		while(fread(&vaultItem, sizeof(VaultItem), 1, file) == 1)
		{
			strcpy(tempElementName, lmeDeAsh(vaultItem.kpub, vaultItem.elementName, currentVault.LMEBITENCODE, currentVault.password));//decript

				if(strcmp(elementNameToDelete, tempElementName) != 0)
				{
					fwrite(&vaultItem, sizeof(VaultItem), 1, tempBinaryFile);
				}else
					haveDeletedSomething = true;
		}

		fclose(file);
		fclose(tempBinaryFile);
		
		
		//If you have deleted something it means that an item with the requested name exists, in which case I ask for confirmation,
		//and if you confirm that you want to delete it then I have the contents of the file inserted into the main file without the item requested to be deleted, otherwise I won't delete it.
		//I do.
		if(haveDeletedSomething == true)
		{
			char confirm = ' ';
				do
				{
					printResponseTerminal();
						printf("Are you sure you want to delete the %s item?(y/n)", elementNameToDelete);
					printResponseInput();
						scanf("%c", &confirm);
					fflush(stdin);
				}while(confirm != 'y' && confirm != 'n');
				
				
				
				
				if(confirm == 'y'){
					
					//get the data saved in the temporaney file(without the one who was deserved to delete) and save in the main current password file
					file = fopen(currentVault.fileVaultId, "wb");
					tempBinaryFile = fopen(generalPath.tempBinaryFile, "rb");
					
					while(fread(&vaultItem, sizeof(VaultItem), 1, tempBinaryFile) == 1)
					{
						fwrite(&vaultItem, sizeof(VaultItem), 1, file);
					}
				
					fclose(file);
					fclose(tempBinaryFile);
					
					printResponseTerminal();
						printf("The password %s has been deleted from the vault.", elementNameToDelete);
				}else
				{
					printResponseTerminal();
						printf("Deletion of item cancelled.");
				}	
		}else
		{
			printResponseTerminal();
					printf("No password with this element name was found.");
		}
		
		
		//delete the temporaney file
		remove(generalPath.tempBinaryFile);
	
			
		
	

}

void editItemInVault(char elementNameToEdit[])
{
	bool confirmExit = false;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	FILE *file;
		file = fopen(currentVault.fileVaultId, "rb");
	FILE *tempFile;
	
	
	
	
	while(fread(&vaultItem, sizeof(VaultItem), 1, file) == 1 && strcmp(lmeDeAsh(vaultItem.kpub, vaultItem.elementName, currentVault.LMEBITENCODE, currentVault.password), elementNameToEdit) != 0);//it cycles until it gets the item you want to get

	fclose(file);
	
	strcpy(vaultItem.elementName, lmeDeAsh(vaultItem.kpub, vaultItem.elementName, currentVault.LMEBITENCODE, currentVault.password));
	
	if(strcmp(vaultItem.elementName, elementNameToEdit) == 0)
	{
		
		char confirm = ' ';
		
		do
		{
			printResponseTerminal();
				printf("Do you want to view the password?(y/n)");
			printResponseInput();
				scanf("%c", &confirm);
			fflush(stdin);
		}while(confirm != 'y' && confirm != 'n');
		
		strcpy(vaultItem.password, lmeDeAsh(vaultItem.kpub, vaultItem.password, currentVault.LMEBITENCODE, currentVault.password));

	
			strcpy(vaultItem.email, lmeDeAsh(vaultItem.kpub, vaultItem.email, currentVault.LMEBITENCODE, currentVault.password));
		
		do
		{
			
			

			

			printResponseTerminal();
				printf("%s Password:\n", vaultItem.elementName);
			
				SetConsoleTextAttribute(hConsole, 3);
			printf("   1. Element name: ");
				SetConsoleTextAttribute(hConsole, 7);
			printf("%s\n", vaultItem.elementName);
				SetConsoleTextAttribute(hConsole, 3);
			printf("   2. Email/Username: ");
				SetConsoleTextAttribute(hConsole, 7);
			printf("%s\n", vaultItem.email);
				SetConsoleTextAttribute(hConsole, 3);
			printf("   3. Password: ");
				SetConsoleTextAttribute(hConsole, 8);
				if(confirm == 'y')
					printf("%s\n", vaultItem.password);
				else
					printf("* * * * * *");
				SetConsoleTextAttribute(hConsole, 7);
			
			char choice[4];
			
			do
			{
				printf("\n");
				printResponseTerminal();
					printf("Type the number corresponding to the element you want to modify (1-3).\n   Type \"exit\" to finish changes and save them or \"cancel\" to cancel and not make any changes.");
				printResponseInput();
					gets(choice);
					
					if(strcmp(choice, "exit") == 0 || strcmp(choice, "cancel") == 0)
						confirmExit = true;
					
			}while(strcmp(choice, "1") != 0 && strcmp(choice, "2") != 0 && strcmp(choice, "3") != 0 && confirmExit != true);
			printf("\n\n");
			//save the data modified temporaney
			if(!confirmExit)
			{
				//save the temporaney data modified
				char t[] = " ";
				printResponseTerminal();
				
				if(strcmp(choice, "1") == 0)
				{
					printf("New Element Name:");
						printResponseInput();
							gets(t);
					fflush(stdin);
					strcpy(vaultItem.elementName, t);
				}else if(strcmp(choice, "2") == 0)
				{
					printf("New Email/Username:");
						printResponseInput();
							gets(t);
					fflush(stdin);
					strcpy(vaultItem.email, t);

				}else if(strcmp(choice, "3") == 0)
				{
					printf("New Password:");
						printResponseInput();
						getInvisibleInputString(t);
					fflush(stdin);
					strcpy(vaultItem.password, t);
					
				}
				
			}else//if you have chosen to exit or cancel, the request to insert the new parameter to be modified does not occur
			{
				char confirmEnd;
				do
				{
					printResponseTerminal();
						printf("Confirm that you wish to exit");
							if(strcmp(choice, "exit") == 0)
								printf(" and save the changes?");
							else
								printf("without saving the changes?");
						printf("(y/n)");					
						printResponseInput();
						scanf("%c", &confirmEnd);
					fflush(stdin);
				}while(confirmEnd != 'y' && confirmEnd != 'n');
				
				if(confirmEnd == 'n')
				{
					confirmExit = false;
						printf("\n\n");

				}
				else
				{
					//if exit is confirmed
					
					//if need save data on exit
					if(strcmp(choice, "exit") == 0)
					{
						
						//save data
						char tempElementName[10000] = " ";
						char tempEmail[10000] = " ";
						char tempPassword[10000] = " ";
						strcpy(tempElementName, vaultItem.elementName);
						strcpy(tempEmail, vaultItem.email);
						
						strcpy(tempPassword, vaultItem.password);
						
						
						file = fopen(currentVault.fileVaultId, "rb");
						tempFile = fopen(generalPath.tempBinaryFile, "wb");
												

						time_t currentTime_struct;
							struct tm *currentTime;
				    		time(&currentTime_struct);
				    	currentTime = localtime(&currentTime_struct);

						char tempWhenModif[10000] = " ";
						sprintf(tempWhenModif, "Last modification occurred on %02d/%02d/%04d at %02d:%02d", currentTime->tm_mday, currentTime->tm_mon + 1, currentTime->tm_year + 1900, currentTime->tm_hour, currentTime->tm_min);


						while(fread(&vaultItem, sizeof(VaultItem), 1, file) == 1)
						{

							if(strcmp(lmeDeAsh(vaultItem.kpub, vaultItem.elementName, currentVault.LMEBITENCODE, currentVault.password), elementNameToEdit) == 0)
							{
								strcpy(vaultItem.elementName, lmeAsh(vaultItem.kpub, tempElementName, currentVault.password));
								strcpy(vaultItem.email, lmeAsh(vaultItem.kpub, tempEmail, currentVault.password));
								strcpy(vaultItem.password, lmeAsh(vaultItem.kpub, tempPassword, currentVault.password));
								strcpy(vaultItem.whenModified, lmeAsh(vaultItem.kpub, tempWhenModif, currentVault.password));


							}
														
							fwrite(&vaultItem, sizeof(VaultItem), 1, tempFile);
						}
						
						fclose(file);
						fclose(tempFile);
						
					
						file = fopen(currentVault.fileVaultId, "wb");
						tempFile = fopen(generalPath.tempBinaryFile, "rb");
						
						while(fread(&vaultItem, sizeof(VaultItem), 1, tempFile) == 1)
							fwrite(&vaultItem, sizeof(VaultItem), 1, file);
						
						fclose(file);
						fclose(tempFile);
						
						//delete the temporaney file
						remove(generalPath.tempBinaryFile);

					}//else dont save
					
					confirmExit = true;
				}
			}
			
			
		}while(confirmExit != true);
		
		

	}else
	{
		printResponseTerminal();
			printf("There is no item with this name.\n");
	}
}

void getUsersAndPrint()
{

	FILE *file;
	char vaultname[100];
	char pass[100];
	char bit[100];
	char inputStyle[100];

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	//check if file is empty, in case write that there is no users saved yet
	if(isFileEmpty(generalPath.pathVault) == 0)//is empty
	{
		printResponseTerminal();
		printf("There are no users saved yet.\n");
	}else
	{
		file = fopen(generalPath.pathVault, "r");
		printResponseTerminal();
		printf("Account vaults saved:\n");
		while(fscanf(file, "%s ; %s ; %s ; %s", vaultname, pass, bit, inputStyle) != EOF)
		{
			
			printf("\t-");
			
			//if you print this command the name of the vault you are currently connected to will be colored blue
			if(strcmp(vaultname, currentVault.username) == 0)
				SetConsoleTextAttribute(hConsole, 3);

			printf(" %s\n", vaultname);
			
			SetConsoleTextAttribute(hConsole, 7);


		}
		fclose(file);
		
	}
	
	
}

void showSpecificItem(char elementName[])
{
	FILE *file;
	
	file = fopen(currentVault.fileVaultId, "rb");
	
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	
	char confirm;

	
	while(fread(&vaultItem, sizeof(VaultItem), 1, file) == 1 && strcmp(lmeDeAsh(vaultItem.kpub, vaultItem.elementName, currentVault.LMEBITENCODE, currentVault.password), elementName) != 0);//it cycles until it gets the item you want to get

	
strcpy(vaultItem.elementName, lmeDeAsh(vaultItem.kpub, vaultItem.elementName, currentVault.LMEBITENCODE, currentVault.password));
	if(strcmp(vaultItem.elementName, elementName) == 0)
	{
		
		
		do{
	
		printResponseTerminal();
				printf("Do you want to view the password?(y/n)");
			printResponseInput();
				scanf("%c", &confirm);
			fflush(stdin);
		}while(confirm != 'y' && confirm != 'n');
		
		
		
		strcpy(vaultItem.email, lmeDeAsh(vaultItem.kpub, vaultItem.email, currentVault.LMEBITENCODE, currentVault.password));
		strcpy(vaultItem.password, lmeDeAsh(vaultItem.kpub, vaultItem.password, currentVault.LMEBITENCODE, currentVault.password));
		strcpy(vaultItem.whenCreated, lmeDeAsh(vaultItem.kpub, vaultItem.whenCreated, currentVault.LMEBITENCODE, currentVault.password));
		strcpy(vaultItem.whenModified, lmeDeAsh(vaultItem.kpub, vaultItem.whenModified, currentVault.LMEBITENCODE, currentVault.password));


		printResponseTerminal();
			printf("%s Element:\n", vaultItem.elementName);
		
			SetConsoleTextAttribute(hConsole, 3);
		printf("   Element name: ");
			SetConsoleTextAttribute(hConsole, 7);
		printf("%s\n", vaultItem.elementName);
			SetConsoleTextAttribute(hConsole, 3);
		printf("   Email/Username: ");
			SetConsoleTextAttribute(hConsole, 7);
		printf("%s\n", vaultItem.email);
			SetConsoleTextAttribute(hConsole, 3);
		printf("   Password: ");
			SetConsoleTextAttribute(hConsole, 8);
			if(confirm == 'y')
				printf("%s\n", vaultItem.password);
			else
				printf("* * * * * *\n");
			SetConsoleTextAttribute(hConsole, 7);
		printf("   %s\n   %s", vaultItem.whenCreated, vaultItem.whenModified);


	}else
	{
		printResponseTerminal();
			printf("There is no item with this name.\n");
	}


}

void logInVault(char accountName[], char input[])
{
	
	char accountPassword[100] = " ";
	char us[100];
	char ps[100];
	char tUs[100];
	char tPs[100];
	char inputStyle[100];
	char tLMEBITENCODE[100];
	char tempStyle[100];

	
	FILE *file;
	
	//get the password corresponding to the requested accountName
	
	file = fopen(generalPath.pathVault, "r");
		while(fscanf(file, "%s ; %s ; %s ; %s", tUs, tPs, tLMEBITENCODE, tempStyle) != EOF){
			if(strcmp(tUs, accountName) == 0)
			{
				strcpy(us, tUs);
				strcpy(ps, tPs);
				strcpy(inputStyle, tempStyle);

			}
		}
		
			
	printResponseTerminal();
		printf("Password vault:");
	printResponseInput();
	getInvisibleInputString(accountPassword);
		if(accountPassword[0] == 32)
		{
			accountPassword[0] = (char)126;
		}
		md5(accountPassword, strlen(accountPassword));
	getMd5Result(accountPassword);
	
	//if the password is correct
	if(strcmp(accountPassword, ps) == 0)
	{
		//change the input variable. In the main with a condition check if is joinVault and in this case it enter in the vault place
		strcpy(input, "joinVault");
		
		//set the variable of user in the structure
		strcpy(currentVault.username, us);
		strcpy(currentVault.password, ps);
		currentVault.LMEBITENCODE = atoi(tLMEBITENCODE); //atoi --> convert string to int
		strcpy(currentVault.inputStyle, inputStyle);
		
		
		//set the file data id in structure
		strcpy(currentVault.fileVaultId, generalPath.prefixedFolderUserVaultDataFilePath);


		
		char temp[100];
		strcpy(temp, currentVault.username);
		obtainStringId(temp);
		strcat(currentVault.fileVaultId, "/");
		strcat(currentVault.fileVaultId, temp);
		strcat(currentVault.fileVaultId, ".dat");
		
		printResponseTerminal();
		printf("Correct password.\n   Connected to the user vault %s", us);
	}else
	{
		printResponseTerminal();
		printf("Wrong password.");
	}

}

void deleteVaultFunction(char accountName[])
{
	
	char confirm = ' ';
		do
		{
			printResponseTerminal();
				printf("Are you sure you want to delete the %s vault?\n   Once confirmed, it will not be possible to recover any data previously saved in the deleted vault.(y/n)", accountName);
			printResponseInput();
				scanf("%c", &confirm);
			fflush(stdin);
		}while(confirm != 'y' && confirm != 'n');
		
		
	if(confirm == 'y')
	{
		char accountPassword[100] = " ";
		char us[100];
		char ps[100];
		char tUs[100];
		char tPs[100];
		char tbit[100];
		char inputStyle[100];
		
		FILE *file;
		
		//get the password corresponding to the requested accountName
	
		file = fopen(generalPath.pathVault, "r");
			fscanf(file, "%s ; %s ; %s ; %s", us, ps, tbit, inputStyle);
			while(fscanf(file, "%s ; %s ; %s ; %s", tUs, tPs, tbit, inputStyle) != EOF && strcmp(us, accountName) != 0){
				if(strcmp(us, accountName) != 0)
				{
					strcpy(us, tUs);
					strcpy(ps, tPs);
				}
			}
	
		printResponseTerminal();
			printf("Password vault:");
		printResponseInput();
		getInvisibleInputString(accountPassword);
			if(accountPassword[0] == 32)
			{
				strcpy(accountPassword, (char *)126);
				strcat(accountPassword, '\0');
			}
		md5(accountPassword, strlen(accountPassword));
		getMd5Result(accountPassword);
			fclose(file);
	
		//printf("%s\n%s", accountPassword, ps);
			
	
		//if the password is correct
		if(strcmp(accountPassword, ps) == 0)
		{
			file = fopen(generalPath.pathVault, "r");
	
			FILE *tempFile;
			tempFile = fopen(generalPath.tempFile, "a");
			
			int i = 0;
		
			//save in a temp file all the user exept user to delete
			while(fscanf(file, "%s ; %s ; %s ; %s", us, ps, tbit, inputStyle) != EOF)
			{
				//printf("%s %s %s\n",accountName, us, ps);
				if(strcmp(us, accountName) != 0)
				{
					if(i != 0)
						fprintf(tempFile, "\n");
					fprintf(tempFile, "%s ; %s ; %s ; %s", us, ps, tbit, inputStyle);
					i++;
				}
			}
			
			fclose(tempFile);
			fclose(file);
			
			file = fopen(generalPath.pathVault, "w");
			tempFile = fopen(generalPath.tempFile, "r");
			i = 0;
			//overwrite the vaults file with the temp file(without user deleted)
			while(fscanf(tempFile, "%s ; %s ; %s ; %s", us, ps, tbit, inputStyle) != EOF)
			{
				if(i != 0)
					fprintf(file, "\n");
				fprintf(file, "%s ; %s ; %s ; %s", us, ps, tbit, inputStyle);
				i++;
			}
		
			fclose(tempFile);
			fclose(file);
			
			//delete the temp file
			remove(generalPath.tempFile);
			
			//delete the file dat of the user deleted
			char userVaultFileDat[100];
			char tempIdUsername[100];
			strcpy(tempIdUsername, accountName);
				obtainStringId(tempIdUsername);
			
			strcpy(userVaultFileDat, generalPath.prefixedFolderUserVaultDataFilePath);
				strcat(userVaultFileDat, "/");
				strcat(userVaultFileDat, tempIdUsername);
				strcat(userVaultFileDat, ".dat");
			
			remove(userVaultFileDat);
			
			printResponseTerminal();
				printf("Vault successfully deleted.\n");
			
		}else
		{
			printResponseTerminal();
				printf("Wrong password :<\n");
		}
	}else
	{
		printResponseTerminal();
			printf("Vault deletion cancelled.\n");
	}
	
	

	
	

}

//check if specific vault exist(check by name)
//false --> don't existent
bool checkVaultAccountExist(char accountName[])
{
	FILE *file;
	char us[100];
	char ps[100];
	
	//if account exist return true;
	if(!isFileEmpty(generalPath.pathVault) == 0) //is not empty
	{

		file = fopen(generalPath.pathVault, "r");
		while(fscanf(file, "%s ; %s", us, ps) != EOF)
		{
			if(strcmp(us, accountName) == 0)
				return true;
		}

		fclose(file);
	}else
		return false;
	
	return false;
}

//create a vault (user)
void createVaultFunction(char accountName[])
{
	char pass1[] = " ";
	char pass2[] = " ";



		printResponseTerminal();
	printf("Password:");
		printResponseInput();
	getInvisibleInputString(pass1);
		if(pass1[0] == 32)
		{
			pass1[0] = (char)126;


		}

		printResponseTerminal();
	printf("Confirm password:");
		printResponseInput();	
	getInvisibleInputString(pass2);
		if(pass2[0] == 32)
		{
			pass2[0] = (char)126;
		}

	printf("\n");
	
	if(strcmp(pass1, pass2) == 0)
	{
		//create the file for the data for the single user in the directory generalPath.prefixedFolderUserVaultDataFilePath
		FILE *file;
		char pathSingleVault[100];
		char tempUsername[100];
		
		//create the variable for the user file path
		strcpy(tempUsername, accountName);
		obtainStringId(tempUsername);
		strcpy(pathSingleVault, generalPath.prefixedFolderUserVaultDataFilePath);
		strcat(pathSingleVault, "/"),
		strcat(pathSingleVault, tempUsername);
		strcat(pathSingleVault, ".dat");


	
		file = fopen(pathSingleVault, "a");
		fclose(file);
		
		//add the user to the file usersVault already saved
		addUserVaultToFile(accountName, pass2);
		
		printResponseTerminal();
			printf("%s's vault has been created successfully.", accountName);

	}else
	{
		printResponseTerminal();
			printf("The two passwords do not match.");
	}

	printf("\n");
}

//set the bit power for the encryption
void setBitPower(char bitPower[])
{
	int intBitPower = atoi(bitPower);
	char confirm;
	char tUs[100];
	char tPs[100];
	char tLMEBITENCODE[100];
	char tempStyle[100];

	int preBitPower;

	if(intBitPower >= 5 && intBitPower <= 15)
	{
		
		do
		{
			printResponseTerminal();
				printf("Are you sure you want to set the encryption power to %d bit?(y/n)", intBitPower);
			printResponseInput();
				scanf("%c", &confirm);
			fflush(stdin);
		}while(confirm != 'y' && confirm != 'n');
		
		if(confirm == 'y')
		{

			FILE *file;
				file = fopen(generalPath.pathVault, "r");
			FILE *tempFile;
				tempFile = fopen(generalPath.tempFile, "w");
			
				while(fscanf(file, "%s ; %s ; %s ; %s", tUs, tPs, tLMEBITENCODE, tempStyle) != EOF){
					
					fprintf(tempFile, "%s ; %s ; ", tUs, tPs);
					if(strcmp(tUs, currentVault.username) == 0)
					{
						preBitPower = atoi(tLMEBITENCODE);//set the previous encryption bit power for the reroll encryption function
						fprintf(tempFile, "%s", bitPower);
					}
					else
						fprintf(tempFile, "%s", tLMEBITENCODE);
					fprintf(tempFile, " ; %s\n", tempStyle);
				}
				
			fclose(file);
			fclose(tempFile);
			
			file = fopen(generalPath.pathVault, "w");
			tempFile = fopen(generalPath.tempFile, "r");
			
				while(fscanf(tempFile, "%s ; %s ; %s ; %s", tUs, tPs, tLMEBITENCODE, tempStyle) != EOF)
					fprintf(file, "%s ; %s ; %s ; %s\n", tUs, tPs, tLMEBITENCODE, tempStyle);
			
			fclose(file);
			fclose(tempFile);
			
			remove(generalPath.tempFile);
			
			currentVault.LMEBITENCODE = intBitPower;
				
			rerollEncryption(preBitPower);

			printResponseTerminal();
				printf("Bit power setting occurred");
			
		}else{
			printResponseTerminal();
			printf("The bit power setting has been canceled.");
		}
		
	}else
	{
		printResponseTerminal();
		printf("The bit power must be between 5 and 15 for security reasons.");
		printf("\n   15-bit encoding corresponds to 32,768 character encoding.");
		printf("\n   You could encode in multiple bits but it is recommended not to do so to save data of this type.");
	}
}

//reroll the encryption key
void rerollEncryption(int decodeBit)
{
	char confirm;
	
	//ask only if the command is called by himself and not recolled by the setBitPower function
	if(currentVault.LMEBITENCODE != decodeBit)
		confirm = 'y';
	else
	{
		do
		{
			printResponseTerminal();
				printf("Are you sure you want to reroll the vault key?(y/n)");
			printResponseInput();
				scanf("%c", &confirm);
			fflush(stdin);
		}while(confirm != 'y' && confirm != 'n');	
	}
	
	if(confirm == 'y')
	{
		FILE *file;
			file = fopen(currentVault.fileVaultId, "rb");
		FILE *tempFile;
			tempFile = fopen(generalPath.tempBinaryFile, "wb");
		printResponseTerminal();
		printf("Wait.");
				
		//get the value, decript and re-cript with the new key it in temp file
		while(fread(&vaultItem, sizeof(VaultItem), 1, file) == 1)
		{
			
			//decript the data
			strcpy(vaultItem.elementName, lmeDeAsh(vaultItem.kpub, vaultItem.elementName, decodeBit, currentVault.password));
			strcpy(vaultItem.email, lmeDeAsh(vaultItem.kpub, vaultItem.email, decodeBit, currentVault.password));
			strcpy(vaultItem.password, lmeDeAsh(vaultItem.kpub, vaultItem.password, decodeBit, currentVault.password));
			strcpy(vaultItem.whenCreated, lmeDeAsh(vaultItem.kpub, vaultItem.whenCreated, decodeBit, currentVault.password));
			strcpy(vaultItem.whenModified, lmeDeAsh(vaultItem.kpub, vaultItem.whenModified, decodeBit, currentVault.password));

			
			//set and save the rerolled public key
			char* publicKey = lmeGenKey(currentVault.LMEBITENCODE);
				strcpy(vaultItem.kpub, publicKey);
			
			//re-cript the data with the new public key
			strcpy(vaultItem.elementName, lmeAsh(vaultItem.kpub, vaultItem.elementName, currentVault.password));
			strcpy(vaultItem.email, lmeAsh(vaultItem.kpub, vaultItem.email, currentVault.password));
			strcpy(vaultItem.password, lmeAsh(vaultItem.kpub, vaultItem.password, currentVault.password));
			strcpy(vaultItem.whenCreated, lmeAsh(vaultItem.kpub, vaultItem.whenCreated, currentVault.password));
			strcpy(vaultItem.whenModified, lmeAsh(vaultItem.kpub, vaultItem.whenModified, currentVault.password));
				fwrite(&vaultItem, sizeof(VaultItem), 1, tempFile);
			
			sleep(1);//without this c cannot update the clock to obtain a random value different from the system clock.
				printf(".");//To show that we are moving forward
		}
		
		fclose(file);
		fclose(tempFile);
		
		file = fopen(currentVault.fileVaultId, "wb");
		tempFile = fopen(generalPath.tempBinaryFile, "rb");
		
		//transfert the tempfile data in the standard file
		while(fread(&vaultItem, sizeof(VaultItem), 1, tempFile) == 1)
		{
			fwrite(&vaultItem, sizeof(VaultItem), 1, file);
		}

		fclose(file);
		fclose(tempFile);
		
		//delete the temporaney file
		remove(generalPath.tempBinaryFile);
		
		printResponseTerminal();
			printf("Encryption key reroll occurred. The vault has been refreshed. Encryption power key: %d", currentVault.LMEBITENCODE);
	}else
	{
		printResponseTerminal();
			printf("Encryption key reroll cancelled.");
	}

}

//delete all password of the vault(nuke)
void nukeVault()
{
	
	char confirm = ' ';
	char input[200] = " ";
	
	FILE *file = fopen(currentVault.fileVaultId, "rb");
	fseek(file, 0, SEEK_END);//set the cursor at the end of file for check if is empty
	
	//if the binary file is empty nuke is senseless
	if(ftell(file) == 0)
	{
		printResponseTerminal();
			printf("The vault is already empty.");
	}else{
		do
		{
			printResponseTerminal();
				printf("Are you sure you want to nuke the vault? Saved data cannot be recovered once the vault is nuked.(y/n)");
			printResponseInput();
				scanf("%c", &confirm);
				fflush(stdin);
		}while(confirm != 'y' && confirm != 'n');
		
		if(confirm == 'y')
		{
			
			printResponseTerminal();
				printf("Please enter the vault password to confirm the vault nuke.");
			printResponseInput();
				getInvisibleInputString(input);
					if(input[0] == 32)
					{
						input[0] = (char)126;
					}
			md5(input, strlen(input));
			getMd5Result(input);
			
			if(strcmp(input, currentVault.password) == 0)
			{
				FILE *file = fopen(currentVault.fileVaultId, "w");
					fclose(file);
				
				printResponseTerminal();
					printf("Nuke occurred. The vault has been restored.");
			}else
			{
				printResponseTerminal();
					printf("Wrong password. Nuke cancelled.");
			}
			
		}else
		{
			printResponseTerminal();
				printf("Nuke cancelled.");
		}
	}
	
	fflush(stdin);
}

//obtain by string a number conversion ( string ) : string --> (string)int
void obtainStringId(char str[])
{
    int lunghezza = strlen(str);
    char stringa_ascii[lunghezza * 4 + 1];  // Add 1 to the string terminator
    int offset = 0;

    int i;
    for (i = 0; i < lunghezza; i++) {
        offset += sprintf(stringa_ascii + offset, "%d", str[i]);
    }

    strcpy(str, stringa_ascii);
}


//add the user in the vault file
void addUserVaultToFile(char accountName[], char pass[])
{
	FILE *file;

	md5(pass, strlen(pass));
	getMd5Result(pass);

	file = fopen(generalPath.pathVault, "a");

	if(!isFileEmpty(generalPath.pathVault) == 0)//is not empty
		fprintf(file, "\n");

	fprintf(file, "%s ; %s ; %d ; %s", accountName, pass, 9, DebianStyle);

	fclose(file);

}


//get invisible input string
void getInvisibleInputString(char str[])
{
	int i = 0;
    char ch;
    
	while (1) {
        ch = getch();

        // Termina il ciclo se viene premuto il tasto Invio
        if (ch == '\r') {
        	//printf("%c %c %c %c %c %c", 250, 250, 250, 250, 250, 250);
        	printf("* * * * * *");
            break;
        }
        
        //cancella se viene premuto il tasto cancella
        if (ch == '\b' && i > 0)
        {
        	i--;
        	str[i] = '\0';
        	i--;
		}else
		{
			// Aggiunge il carattere alla stringa
        	str[i] = ch;
        	str[i+1] = '\0';
		}
			

        
        i++;
    }
}


//set the prefixed path for directory and files
void setGeneralPath()
{
	
	//prefixed path
	char prefixedFolderDataFilePath[] = "FileSourceData";
	char prefixedFolderUserVaultDataFilePath[] = "UsersVaultData";
	char pathScriptCommand[] = "ScriptCommands.txt";	
	char tempString[] = " ";
	char pathVault[] = "Vaults.txt";
	char pathTempFile[] = "Temp.txt";
	char pathTempBinaryFile[] = "TempBinary.dat";


	
	
	
	
	

	
	//set prefixed path

	//set data folder variable
	strcpy(generalPath.prefixedFolderDataFilePath, prefixedFolderDataFilePath);
	strcpy(generalPath.prefixedFolderUserVaultDataFilePath, prefixedFolderDataFilePath);
	strcat(generalPath.prefixedFolderUserVaultDataFilePath, "/");
	strcat(generalPath.prefixedFolderUserVaultDataFilePath, prefixedFolderUserVaultDataFilePath);
	
	
	
	
	
	//if the directory doesn't exist it will create it
	
	if(!isDirectoryExists(generalPath.prefixedFolderDataFilePath))
	{
		//create directory
		int result = mkdir(prefixedFolderDataFilePath);
		if(result != 0)
		{
			printf("\n\n[:<]- Error creating folder.");
		}
	}
	
	
	if(!isDirectoryExists(generalPath.prefixedFolderUserVaultDataFilePath))
	{
		//create directory
		int result = mkdir(generalPath.prefixedFolderUserVaultDataFilePath);
		if(result != 0)
		{
			printf("\n\n[:<]- Error creating folder.");
		}
	}
	
	
	//
	


	//set pathScriptCommand variable
	strcpy(tempString, generalPath.prefixedFolderDataFilePath);
	strcat(tempString, "/");
	strcat(tempString, pathScriptCommand);
	strcpy(generalPath.pathScriptCommand, tempString);	
	

	strcpy(tempString, generalPath.prefixedFolderDataFilePath);
	strcat(tempString, "/");
	strcat(tempString, pathVault);
	strcpy(generalPath.pathVault, tempString);
	
	strcpy(tempString, generalPath.prefixedFolderDataFilePath);
	strcat(tempString, "/");
	strcat(tempString, pathTempFile);
	strcpy(generalPath.tempFile, tempString);
	
	strcpy(tempString, generalPath.prefixedFolderUserVaultDataFilePath);
	strcat(tempString, "/");
	strcat(tempString, pathTempBinaryFile);
	strcpy(generalPath.tempBinaryFile, tempString);
	
	strcpy(tempString, generalPath.prefixedFolderDataFilePath);
	strcat(tempString, "/");
	strcat(tempString, pathTempFile);
	strcpy(generalPath.tempFile, tempString);
	

	//
}

// Function to check if a folder exists
int isDirectoryExists(const char *path) {
    struct stat info;
    return stat(path, &info) == 0 && S_ISDIR(info.st_mode);
}


void getCommandScript()
{
	
		//create the script command file if doesn't exist
		FILE *file = fopen(generalPath.pathScriptCommand, "a");
		fclose(file);
		
		//create the users file if doesn't exist
		file = fopen(generalPath.pathVault, "a");
		fclose(file);

		//check if file is empty, in case write standart commandScript in source file for commands scripts
		if(isFileEmpty(generalPath.pathScriptCommand) == 0)//is empty
		{
			setStandardCommandScript();//set standard commands scripts in file
		}
		
		getAndSetPrefixedCommandsInStruct();
		
		
		
}


void getAndSetPrefixedCommandsInStruct()
{

	FILE *file = fopen(generalPath.pathScriptCommand, "r");
	char t1[100];
	char t2[100];
	char t3[100];
	
	while(fscanf(file, "%s = \" %s \"", t1, t2) != EOF)
	{

		strcpy(t3, commandScript.CommandPrefix);
		trim(t2);
		strcat(t3, t2);
		

		//printf("%s\n", t2);
		if( strcmp (t1, "Command_Prefix") == 0 )
		{
			strcpy(commandScript.CommandPrefix, t3);
		}else if( strcmp (t1, "Help") == 0 )
		{
			strcpy(commandScript.Help, t3);
		}else if( strcmp (t1, "Help_Minimized") == 0 )
		{
			strcpy(commandScript.HelpMinimize, t3);
		}else if( strcmp (t1, "Add") == 0 )
		{
			strcpy(commandScript.Add, t3);
		}else if( strcmp (t1, "Add_Minimized") == 0 )
		{
			strcpy(commandScript.AddMinimize, t3);
		}else if( strcmp (t1, "Enter") == 0 )
		{
			strcpy(commandScript.Enter, t3);
		}else if( strcmp (t1, "Exit") == 0 )
		{
			strcpy(commandScript.Exit, t3);
		}else if( strcmp (t1, "Shows_All_Vaults_Account") == 0 )
		{
			strcpy(commandScript.ShowsAllVaultsAccount, t3);
		}else if( strcmp (t1, "Shows_All_Vaults_Account_Minimized") == 0 )
		{
			strcpy(commandScript.ShowsAllVaultsAccountMinimize, t3);
		}else if( strcmp (t1, "Create_Vault") == 0 )
		{
			strcpy(commandScript.CreateVault, t3);
		}
		else if( strcmp (t1, "Delete_Vault") == 0 )
		{
			strcpy(commandScript.DeleteVault, t3);
		}else if( strcmp (t1, "Cls_Terminal") == 0 )
		{
			strcpy(commandScript.Cls, t3);
		}else if( strcmp (t1, "Show_All_Passwords") == 0 )
		{
			strcpy(commandScript.ShowAllPasswords, t3);
		}else if( strcmp (t1, "Show_All_Passwords_Minimized") == 0 )
		{
			strcpy(commandScript.ShowAllPasswordsMinimize, t3);
		}else if( strcmp (t1, "Delete_Data_From_List") == 0 )
		{
			strcpy(commandScript.DeletePassword, t3);
		}else if( strcmp (t1, "Delete_Data_From_List_Minimized") == 0 )
		{
			strcpy(commandScript.DeletePasswordMinimize, t3);
		}else if(strcmp(t1, "Set_Bit_Encryption_Power") == 0)
		{
			strcpy(commandScript.SetBitEncryption, t3);

		}else if(strcmp(t1, "Reroll_Encryption") == 0)
		{
			strcpy(commandScript.RerollEncryption, t3);

		}else if(strcmp(t1, "Reroll_Encryption_Minimized") == 0)
		{
			strcpy(commandScript.RerollEncryptionMinimize, t3);

		}else if(strcmp(t1, "NUKE") == 0)
		{
			strcpy(commandScript.Nuke, t3);

		}else if(strcmp(t1, "Set_Input_Style") == 0)
		{
			strcpy(commandScript.SetInputStyle, t3);
			
		}else if(strcmp(t1, "Search_Item") == 0)
		{
			strcpy(commandScript.Search, t3);
		}else if(strcmp(t1, "Encryption_Information") == 0)
		{
			strcpy(commandScript.EncryptionInformation, t3);
		}else if(strcmp(t1, "Show_Password") == 0)
		{
			strcpy(commandScript.ShowPassword, t3);
		}else if(strcmp(t1, "Show_Password_Minimized") == 0)
		{
			strcpy(commandScript.ShowPasswordMinimize, t3);
		}else if(strcmp(t1, "Edit_Data_From_List") == 0)
		{
			strcpy(commandScript.Edit, t3);
		}
		
		
	}
	


}


//sets, if it is the first time you use the program, the file with the data for the commands (--, --help , --h , --add , --a..)
void setStandardCommandScript()
{
	
	FILE *file;
	file = fopen(generalPath.pathScriptCommand, "w");
	//printf("%s", generalPath.pathScriptCommand);
	
	//set all prefixed script command
	fprintf(file, "%s = \" %s \"\n", "Command_Prefix", ".");
	fprintf(file, "%s = \" %s \"\n", "Help", "help");
	fprintf(file, "%s = \" %s \"\n", "Help_Minimized", "h");
	fprintf(file, "%s = \" %s \"\n", "Search_Item", "search");
	fprintf(file, "%s = \" %s \" #Standard: Debian style\n", "Set_Input_Style", "setStyle");
	fprintf(file, "%s = \" %s \"\n", "Add", "add");
	fprintf(file, "%s = \" %s \"\n", "Add_Minimized", "a");
	fprintf(file, "%s = \" %s \"\n", "Delete_Data_From_List", "delete");
	fprintf(file, "%s = \" %s \"\n", "Delete_Data_From_List_Minimized", "del");
	fprintf(file, "%s = \" %s \"\n", "Edit_Data_From_List", "edit");
	fprintf(file, "%s = \" %s \"\n", "Exit", "exit");
	fprintf(file, "%s = \" %s \"\n", "Enter", "enter");
	fprintf(file, "%s = \" %s \"\n", "Create_Vault", "createVault");
	fprintf(file, "%s = \" %s \"\n", "Delete_Vault", "deleteVault");
	fprintf(file, "%s = \" %s \"\n", "Shows_All_Vaults_Account", "showsAllVaultsAccount");
	fprintf(file, "%s = \" %s \"\n", "Shows_All_Vaults_Account_Minimized", "showVaults");
	fprintf(file, "%s = \" %s \"\n", "Cls_Terminal", "cls");
	fprintf(file, "%s = \" %s \"\n", "Show_All_Passwords", "showAllPasswords");
	fprintf(file, "%s = \" %s \"\n", "Show_All_Passwords_Minimized", "saps");
	fprintf(file, "%s = \" %s #Shows a specific vault item while also showing the password \"\n", "Show_Password", "showPassword");
	fprintf(file, "%s = \" %s \"\n", "Show_Password_Minimized", "sp");
	fprintf(file, "%s = \" %s \" #Allows you to change the amount of bits used for encoding(automatic Refresh_Encryption command)\n", "Set_Bit_Encryption_Power", "setBitPower");
	fprintf(file, "%s = \" %s \" #Reload all the password with new public key and new encoding\n", "Reroll_Encryption", "rerollEncrpytion");
	fprintf(file, "%s = \" %s \"\n", "Reroll_Encryption_Minimized", "rerEnc");
	fprintf(file, "%s = \" %s \"\n", "Encryption_Information", "encInfo");
	fprintf(file, "%s = \" %s \" #!WARNING! Completly delete all the vault password !WARNING!", "NUKE", "nuke");




	fclose(file);
	
}

//0 --> empty
int isFileEmpty(const char *path) {
    FILE *file = fopen(path, "r");

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fclose(file);
    
    return file_size;//if file_size = 0 --> file empty



}


int isFileExists(const char *path) {
    return access(path, F_OK) == 0;
}


// Function to remove spaces on the sides of a string
void trim(char str[]) {
    int start, end, i;
    int len = strlen(str);

    // Find the index of the first non-space character
    for (start = 0; start < len && str[start] == ' '; ++start);

    // Find the index of the last non-space character
    for (end = len - 1; end >= 0 && str[end] == ' '; --end);

    // Move non-space characters to the beginning of the string
    for (i = 0; i <= end - start; ++i) {
        str[i] = str[start + i];
    }

    // Add the string terminator at the end
    str[i] = '\0';
}










// MD5 Alghoritm
void getMd5Result(char str[])
{
	
	fflush(stdin);
	char temp[] = " ";
	strcpy(str, "");
	//var char digest[16] := h0 append h1 append h2 append h3 //(Output is in little-endian)
    uint8_t *p;
 
    // display result
 
    p=(uint8_t *)&h0;
    sprintf(temp, "%2.2x%2.2x%2.2x%2.2x", p[0], p[1], p[2], p[3], h0);
    strcpy(str, temp);
 	//strcat(str, ("%2.2x%2.2x%2.2x%2.2x", p[0], p[1], p[2], p[3], h0));
 	
    p=(uint8_t *)&h1;
    sprintf(temp, "%2.2x%2.2x%2.2x%2.2x", p[0], p[1], p[2], p[3], h1);
    strcat(str, temp);

 	//strcat(str, ("%2.2x%2.2x%2.2x%2.2x", p[0], p[1], p[2], p[3], h1));
 	
    p=(uint8_t *)&h2;
    sprintf(temp, "%2.2x%2.2x%2.2x%2.2x", p[0], p[1], p[2], p[3], h2);
    strcat(str, temp);
  	//strcat(str, ("%2.2x%2.2x%2.2x%2.2x", p[0], p[1], p[2], p[3], h2));
  	
    p=(uint8_t *)&h3;
    sprintf(temp, "%2.2x%2.2x%2.2x%2.2x", p[0], p[1], p[2], p[3], h3);
    strcat(str, temp);

    puts("");

    fflush(stdin);
}
 
void md5(uint8_t *initial_msg, size_t initial_len) {
 
    // Message (to prepare)
    uint8_t *msg = NULL;
 
    // Note: All variables are unsigned 32 bit and wrap modulo 2^32 when calculating
 
    // r specifies the per-round shift amounts
 
    uint32_t r[] = {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
                    5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
                    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
                    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};

    // Use binary integer part of the sines of integers (in radians) as constants// Initialize variables:
    uint32_t k[] = {
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
        0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
        0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
        0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
        0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
        0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
        0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
        0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
        0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
        0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
        0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
        0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
        0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
        0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};
 
    h0 = 0x67452301;
    h1 = 0xefcdab89;
    h2 = 0x98badcfe;
    h3 = 0x10325476;
 
    // Pre-processing: adding a single 1 bit
    //append "1" bit to message    
    /* Notice: the input bytes are considered as bits strings,
       where the first bit is the most significant bit of the byte.[37] */
 
    // Pre-processing: padding with zeros
    //append "0" bit until message length in bit = 448 (mod 512)
    //append length mod (2 pow 64) to message
 
    int new_len = ((((initial_len + 8) / 64) + 1) * 64) - 8;
 
    msg = calloc(new_len + 64, 1); // also appends "0" bits 
                                   // (we alloc also 64 extra bytes...)
    memcpy(msg, initial_msg, initial_len);
    msg[initial_len] = 128; // write the "1" bit
 
    uint32_t bits_len = 8*initial_len; // note, we append the len
    memcpy(msg + new_len, &bits_len, 4);           // in bits at the end of the buffer
 
    // Process the message in successive 512-bit chunks:
    //for each 512-bit chunk of message:
    int offset;
    for(offset=0; offset<new_len; offset += (512/8)) {
 
        // break chunk into sixteen 32-bit words w[j], 0 = j = 15
        uint32_t *w = (uint32_t *) (msg + offset);
 
#ifdef DEBUG
 
        int j;
        //for(j =0; j < 64; j++) printf("%x ", ((uint8_t *) w)[j]);
        puts("");
#endif
 
        // Initialize hash value for this chunk:
        uint32_t a = h0;
        uint32_t b = h1;
        uint32_t c = h2;
        uint32_t d = h3;
 
        // Main loop:
        uint32_t i;
        for(i = 0; i<64; i++) {

#ifdef ROUNDS
            uint8_t *p;
            printf("%i: ", i);
            p=(uint8_t *)&a;
            //printf("%2.2x%2.2x%2.2x%2.2x ", p[0], p[1], p[2], p[3], a);
         
            p=(uint8_t *)&b;
            //printf("%2.2x%2.2x%2.2x%2.2x ", p[0], p[1], p[2], p[3], b);
         
            p=(uint8_t *)&c;
            //printf("%2.2x%2.2x%2.2x%2.2x ", p[0], p[1], p[2], p[3], c);
         
            p=(uint8_t *)&d;
            //printf("%2.2x%2.2x%2.2x%2.2x", p[0], p[1], p[2], p[3], d);
            puts("");
#endif        

 
            uint32_t f, g;
 
             if (i < 16) {
                f = (b & c) | ((~b) & d);
                g = i;
            } else if (i < 32) {
                f = (d & b) | ((~d) & c);
                g = (5*i + 1) % 16;
            } else if (i < 48) {
                f = b ^ c ^ d;
                g = (3*i + 5) % 16;          
            } else {
                f = c ^ (b | (~d));
                g = (7*i) % 16;
            }

#ifdef ROUNDS
            //printf("f=%x g=%d w[g]=%x\n", f, g, w[g]);
#endif 
            uint32_t temp = d;
            d = c;
            c = b;
            //printf("rotateLeft(%x + %x + %x + %x, %d)\n", a, f, k[i], w[g], r[i]);
            b = b + LEFTROTATE((a + f + k[i] + w[g]), r[i]);
            a = temp;


 
        }
 
        // Add this chunk's hash to result so far:
 
        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
 
    }
 
    // cleanup
    free(msg);
 
}

