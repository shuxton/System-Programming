#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

char *cmd[]={"mkdir","rmdir","ls","cd","pwd","creat","rm",
             "reload","save","menu","quit",NULL};


char line [128];
char command[16],pathname[64];

typedef struct node{
char name[64];
char type;
struct node *child,*sibling,*parent;
}NODE;


typedef struct list{
char name[64];
struct list *next;
}LIST;

NODE *root, *cwd;
LIST *advpath;


//----------------------------------------------------------


void path_resolve(char *path){
	advpath=(LIST *)malloc(sizeof(LIST));
		LIST *head=(LIST *)malloc(sizeof(LIST));
int x=0;
	
	char *token=strtok(path,"/");
	
	while(token!=NULL){
		if(x==0){
strcpy(advpath->name,token);
advpath->next=NULL;
	head=advpath;
	//printf("%p %p\n",advpath,head);
	x++;
		}else{
   LIST *new=(LIST *)malloc(sizeof(LIST));

	strcpy(new->name,token);
	new->next=NULL;
	head->next=new;
	head=head->next;
head->next=NULL;
	}
	token=strtok(NULL,"/");
}
}


//----------------------------------------------------------


int path_length(){
	int i=0;
			LIST *head=(LIST *)malloc(sizeof(LIST));
head=advpath;

	while(head!=NULL){
	head=head->next;
	i++;
	}
	return i;
}

//----------------------------------------------------------


int findCmd(char *command){
int i=0;
while(cmd[i]){
if(!strcmp(command,cmd[i]))
 return i;
i++;
}
return -1;
}


//----------------------------------------------------------

void initialise(){
root=(NODE *) malloc(sizeof(NODE));
strcpy(root->name,"/");
root->type='D';
root->child=0;
root->sibling=root;
root->parent=root;
cwd=root;
}


//----------------------------------------------------------



void mkdir(char *path){

if(path[0]=='\0' || (path[1]=='\0' && path[0]=='/'))printf(RED"Please enter a valid directory name\n"RESET);
else{
NODE *head=(NODE *) malloc(sizeof(NODE));

LIST *li=(LIST *) malloc(sizeof(LIST));

int flag=0,count=0;
path_resolve(path);
int length=path_length();

int x=0;
if(path[0]=='/')
head=root;
else head=cwd;
li=advpath;

while(count<length-1){
//printf("yu");
if(!strcmp(head->name,li->name) && head->type=='D' ){
count++;
li=li->next;
continue;

}
else if(head->sibling!=NULL && strcmp(head->name,head->sibling->name)){
head=head->sibling;
continue;
}
else if(head->child!=NULL){
head=head->child;
continue;
}
else{
printf(RED"Please enter a valid path\n"RESET);break;
}
}
if(count==length-1){
if(head->child==NULL){
	NODE *new=(NODE *) malloc(sizeof(NODE));
	new->type='D';

strcpy(new->name,li->name);
new->child=NULL;
new->sibling=NULL;
new->parent=head;
head->child=new;
printf(GREEN"Directory  : %s\nStatus: Successfully created!\n"RESET,li->name);
return;
}
head=head->child;
while(head->sibling!=NULL){
	if(!strcmp(head->name,li->name)&&head->type=='D')x++;
	head=head->sibling;
}
	if(!strcmp(head->name,li->name)&&head->type=='D')x++;
if(x>0)printf(RED"Directory  : %s\nStatus: Failed\nError : Directory with that name already exists\n"RESET,li->name);
else{
		NODE *new=(NODE *) malloc(sizeof(NODE));
		new->type='D';
strcpy(new->name,li->name);
new->child=NULL;
new->sibling=NULL;
new->parent=head->parent;
head->sibling=new;
printf(GREEN"Directory  : %s\nStatus: Successfully created!\n"RESET,li->name);

}

}
}
}


//----------------------------------------------------------


void rmdir(char *path){
if(path[0]=='\0' ||(path[1]=='\0' && path[0]=='/'))printf(RED"Please enter a valid path\n"RESET);
else{
NODE *head=(NODE *) malloc(sizeof(NODE));
NODE *prev=(NODE *) malloc(sizeof(NODE));
NODE *par=(NODE *) malloc(sizeof(NODE));

LIST *li=(LIST *) malloc(sizeof(LIST));

int flag=0,count=0;
path_resolve(path);
int length=path_length();
//printf("%d\n",length);
if(path[0]=='/')
head=root;
else head=cwd;
prev=head;
li=advpath;
while(count<length-1){
//printf("yu");
if(!strcmp(head->name,li->name) && head->type=='D' ){
count++;
li=li->next;
continue;

}
else if(head->sibling!=NULL && strcmp(head->name,head->sibling->name)){
if(flag)prev=prev->sibling;
head=head->sibling;
flag=1;
continue;
}
else if(head->child!=NULL){
head=head->child;
prev=head;
continue;
}
else{
	printf(RED"Error: Directory is not empty or it doesn't exist\n"RESET);break;

}
}
if(count==length-1){
head=head->child;
if(length==1)prev=head;
if(head==NULL)printf(RED"Error: Directory-%s is not empty or it doesn't exist\n"RESET,li->name);
else{
	par=head->parent;
	while(head->name!=NULL){	
	if(!strcmp(head->name,li->name)&& head->type=='D' && head->child==NULL){
		if(!strcmp(par->child->name,head->name)){
if(!strcmp(prev->name,head->name))par->child=NULL;
else {par->child=prev;prev->sibling=head->sibling;}
}
else if(!strcmp(prev->name,head->name))par->child=NULL;
else prev->sibling=head->sibling;
 printf(GREEN"Directory  : %s\nStatus: Successfully Deleted\n"RESET);return;
}	head=head->sibling;

	}printf(RED"Error: Directory-%s is not empty or it doesn't exist\n"RESET,li->name);
	

	}
}

else{
	printf(RED"Error: Directory is not empty or it doesn't exist\n"RESET);
}
}
}


//--------------------------------------------


void ls(char *path){
	//printf("%d",strlen(path));
NODE *head=(NODE *) malloc(sizeof(NODE));
if ((strlen(path)==1 && path[0]=='/') || !strcmp(path,"/")){
head=root;
if(head->child!=NULL){
head=head->child;
	printf(YELLOW"TYPE|"BLUE"NAME\n"RESET);

printf(YELLOW"%c   |"BLUE"%s\n"RESET,head->type,head->name);
while(head->sibling!=NULL){
head=head->sibling;
printf(YELLOW"%c   |"BLUE"%s\n"RESET,head->type,head->name);
}
}
}
else if(strlen(path)==0){
	head=cwd;
if(head->child!=NULL){
head=head->child;
	printf(YELLOW"TYPE|"BLUE"NAME\n"RESET);

printf(YELLOW"%c   |"BLUE"%s\n"RESET,head->type,head->name);
while(head->sibling!=NULL){
head=head->sibling;
printf(YELLOW"%c   |"BLUE"%s\n"RESET,head->type,head->name);
}
}
}

else{

char *token=strtok(path,"/");
char temp[50];
if(token!=NULL)strcpy(temp,token);
if(path[0]=='/')
head=root;
else head=cwd;

head=head->child;
if(head==NULL){printf(RED"Invalid path\n"RESET);return;}

while(token!=NULL){

if(!strcmp(token,head->name) && head->type=='D'){
token=strtok(NULL,"/");
if(token!=NULL)strcpy(temp,token);


if(token==NULL){
if(head->child!=NULL)
head=head->child;
else {
	printf(RED"Invalid Path \n"RESET);return;
}
while(head!=NULL){
	printf(YELLOW"TYPE|"BLUE"NAME\n"RESET);

printf(YELLOW"%c   |"BLUE"%s\n"RESET,head->type,head->name);
head=head->sibling;
}
return;
}
}
else if(head->sibling!=NULL){
head=head->sibling;
continue;
}
else if(head->child!=NULL){
head=head->child;
continue;
}

else {
printf(RED"Invalid path\n"RESET);break;
} 

}
}
}



//--------------------------------------------


void cd(char *path){
if(path[0]=='\0' || (path[1]=='\0' && path[0]=='/')){
cwd=root;
printf(GREEN"You are now in root directory\n"RESET);
}
else{
NODE *head=(NODE *) malloc(sizeof(NODE));
int flag=1;
char *token=strtok(path,"/");
char temp[50];
if(token!=NULL)strcpy(temp,token);
if(path[0]=='/')
head=root;
else{ head=cwd;
	if(head->child==NULL){printf(RED"Invalid path\n"RESET);return;}
	else head=head->child;
}
while(token!=NULL){
flag=1;
if((!strcmp(token,head->name) || head==head->sibling) && head->type=='D'){
token=strtok(NULL,"/");
if(token!=NULL)strcpy(temp,token);

 if(head==head->sibling && head->child!=NULL){head=head->child;}
else if(head==head->sibling){ printf(RED"Invalid path\n"RESET);break;}

 if(token==NULL){
	if(!strcmp(temp,head->name) && head->type=='D'){
cwd=head;
 printf(GREEN"Successfully changed directory\n"RESET);break;
}
else{
	printf(RED"Invalid path\n"RESET);break;
}
}
}

else if(head->sibling!=NULL){
head=head->sibling;
continue;
}
else if(head->child!=NULL){
head=head->child;
continue;
}

else {
printf(RED"Invalid path\n"RESET);break;
} 

}
}
}


//----------------------------------------------------------


void pwd(){
NODE *head=(NODE *)malloc(sizeof(NODE));
head=cwd;
LIST *cur=(LIST *)malloc(sizeof(LIST));
cur->next=NULL;
strcpy(cur->name,head->name);
while(strcmp(head->parent->name,head->name)){
head=head->parent;
LIST *new=(LIST *)malloc(sizeof(LIST));
new->next=cur;
strcpy(new->name,head->name);
cur=new;
}
while(cur->next!=NULL){

printf(BLUE"%s"RESET,cur->name);
if(strcmp(cur->name,"/"))
printf(BLUE"/"RESET);
cur=cur->next;
}

printf(BLUE"%s\n"RESET,cur->name);
}


//----------------------------------------------------------


void creat(char *path){
if(path[0]=='\0' || (path[1]=='\0' && path[0]=='/'))printf(RED"Please enter a valid file name\n"RESET);
else{
NODE *head=(NODE *) malloc(sizeof(NODE));

LIST *li=(LIST *) malloc(sizeof(LIST));

int flag=0,count=0;
path_resolve(path);
int length=path_length();

int x=0;
if(path[0]=='/')
head=root;
else head=cwd;
li=advpath;

while(count<length-1){
//printf("yu");
if(!strcmp(head->name,li->name) && head->type=='D' ){
count++;
li=li->next;
continue;

}
else if(head->sibling!=NULL && strcmp(head->name,head->sibling->name)){
head=head->sibling;
continue;
}
else if(head->child!=NULL){
head=head->child;
continue;
}
else{
printf(RED"Invalid path\n"RESET);break;
}
}
if(count==length-1){
if(head->child==NULL){
	NODE *new=(NODE *) malloc(sizeof(NODE));
	new->type='F';

strcpy(new->name,li->name);
new->child=NULL;
new->sibling=NULL;
new->parent=head;
head->child=new;
head=head->child;
printf(GREEN"File  : %s\nStatus: Successfully created!\n"RESET,li->name);
//printf("%s- %s\n",head->parent->name,head->sibling);
return;
}
head=head->child;
while(head->sibling!=NULL){
	if(!strcmp(head->name,li->name)&&head->type=='F')x++;
	head=head->sibling;
}
	if(!strcmp(head->name,li->name)&&head->type=='F')x++;
if(x>0)printf(RED"File  : %s\nStatus: Failed\nError : File with that name already exists\n"RESET,li->name);
else{
		NODE *new=(NODE *) malloc(sizeof(NODE));
		new->type='F';
strcpy(new->name,li->name);
new->child=NULL;
new->sibling=NULL;
new->parent=head->parent;
head->sibling=new;
head=head->sibling;
printf(GREEN"File  : %s\nStatus: Successfully created!\n"RESET,li->name);
//printf("%s %s\n",head->parent->name,head->sibling);


}

}
}


}


//----------------------------------------------------------


void rm(char *path){
if(path[0]=='\0' ||(path[1]=='\0' && path[0]=='/'))printf(RED"Please enter a valid path\n"RESET);
else{
NODE *head=(NODE *) malloc(sizeof(NODE));
NODE *prev=(NODE *) malloc(sizeof(NODE));
NODE *par=(NODE *) malloc(sizeof(NODE));

LIST *li=(LIST *) malloc(sizeof(LIST));

int flag=0,count=0;
path_resolve(path);
int length=path_length();
//printf("%d\n",length);
if(path[0]=='/')
head=root;
else head=cwd;
prev=head;
li=advpath;
while(count<length-1){
//printf("yu");
if(!strcmp(head->name,li->name) && head->type=='D' ){
count++;
li=li->next;
continue;

}
else if(head->sibling!=NULL && strcmp(head->name,head->sibling->name)){
if(flag)prev=prev->sibling;
head=head->sibling;
flag=1;
continue;
}
else if(head->child!=NULL){
head=head->child;
prev=head;
continue;
}
else{
printf(RED"Error: File not found\n"RESET);break;
}
}
if(count==length-1){
head=head->child;
if(length==1)prev=head;
if(head==NULL)printf(RED"Error: File-%s not found\n"RESET,li->name);
else{
	par=head->parent;
	while(head->name!=NULL){	
//printf("yo %s\n",head->name);
	if(!strcmp(head->name,li->name)&& head->type=='F'){
		if(!strcmp(par->child->name,head->name)){
if(!strcmp(prev->name,head->name))par->child=NULL;
else {par->child=prev;prev->sibling=head->sibling;}
}
else if(!strcmp(prev->name,head->name))par->child=NULL;
else prev->sibling=head->sibling;
 printf(GREEN"File  : %s\nStatus: Successfully Deleted\n"RESET);return;
}	head=head->sibling;

	}printf(RED"Error: File-%s not found\n"RESET,li->name);

	}
}

else{
printf(RED"Error: File not found\n"RESET);
}
}
}

//----------------------------------------------------------


void reload(){
	FILE *f= fopen("FileTree","r");
	char d[10],p[64];
	fscanf(f,"%s%s",d,p);
	while(fscanf(f,"%s%s",d,p)!=EOF){
		if(!strcmp(d,"D"))mkdir(p);
			else creat(p);
	}
	if(f!=NULL)fclose(f);
	}


//----------------------------------------------------------


void pre_order(NODE *head,FILE *f){
	if (head==NULL)return;
	else{
LIST *cur=(LIST *)malloc(sizeof(LIST));
NODE *temp=(NODE *)malloc(sizeof(NODE));
cur->next=NULL;
		strcpy(cur->name,head->name);
int x=0;
temp=head;
//if(head->child!=NULL)
//printf("%s\n",head->child->name);
while(strcmp(head->parent->name,head->name)){
	
LIST *new=(LIST *)malloc(sizeof(LIST));
new->next=cur;
strcpy(new->name,head->name);
//cur->next=new;
cur=new;

head=head->parent;

}
fprintf(f,"%c      /",temp->type);
printf(YELLOW"%c   |"BLUE"/"RESET,temp->type);
while(cur->next!=NULL){
if(strcmp(cur->name,"/")){
fprintf(f,"%s",cur->name);
printf(BLUE"%s"RESET,cur->name);
}
cur=cur->next;
if(cur->next!=NULL){
fprintf(f,"/");
printf(BLUE"/"RESET);
}
}
fprintf(f,"\n");
printf("\n");
head=temp;
pre_order(head->child,f);
if(head!=head->sibling)
pre_order(head->sibling,f);
}
}


//----------------------------------------------------------


void save(){
FILE *f=fopen("FileTree","w+");
printf(YELLOW"TYPE|"BLUE"NAME\n"RESET);

	pre_order(root,f);
	if (f!=NULL)fclose(f);
printf(MAGENTA"This file structure has been saved in the following file: FileTree\n"RESET);

	}


//----------------------------------------------------------


void menu(){
printf(MAGENTA"mkdir pathname :make a new directory for a given pathname\n"); 
printf("rmdir pathname :remove the directory, if it is empty.\n"); 
printf("cd [pathname]  :change CWD to pathname, or to / if no pathname.\n"); 
printf("ls [pathname]  :list the directory contents of pathname or CWD\n"); 
printf("pwd            :print the (absolute) pathname of CWD\n"); 
printf("creat pathname :create a FILE node. \n");
printf("rm pathname    :remove the FILE node. \n");
printf("save ﬁlename   :save the current ﬁle system tree as a ﬁle\n");
printf("reload ﬁlename :construct a ﬁle system tree from a ﬁle\n");
printf("menu           : show a menu of valid commands \n");
printf("quit           : save the ﬁle system tree, then terminate the program.\n"RESET);
}


//----------------------------------------------------------


void quit(){
save();
exit(0);
}


//----------------------------------------------------------


int main(){
int index;
initialise();

printf(MAGENTA"Welcome to Shuxton's File Tree\nType 'menu' for a list of available commands\n"RESET);

while(1){
strcpy(pathname,"\0");
printf(CYAN"filetree~"RESET);
fgets(line,128,stdin);
line[strlen(line)-1]=0;
sscanf(line,"%s %s",command,pathname);

index=findCmd(command);
switch(index){
case 0:mkdir(pathname); break;
case 1:rmdir(pathname); break;
case 2:ls(pathname); break;
case 3:cd(pathname); break;
case 4:pwd(); break;
case 5:creat(pathname); break;
case 6:rm(pathname); break;
case 7:reload(); break;
case 8:save(); break;
case 9:menu(); break;
case 10:quit(); break;
default:printf("Invalid command %s\n",command);
}
}
return 0;
}
//-----------------------END-----------------------------------
