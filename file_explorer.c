#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *cmd[]={"mkdir","rmdir","ls","cd","pwd","creat","rm",
             "reload","save","menu","quit",NULL};

typedef struct node{
char name[64];
char type;
struct node *child,*sibling,*parent;
}NODE;

typedef struct stack{
char name[64];
struct stack *down;
}STACK;

NODE *root, *cwd;

char line [128];
char command[16],pathname[64];
char dname[64],bname[64];

int findCmd(char *command){
int i=0;
while(cmd[i]){
if(!strcmp(command,cmd[i]))
 return i;
i++;
}
return -1;
}

void initialise(){
root=(NODE *) malloc(sizeof(NODE));
strcpy(root->name,"/");
root->type='D';
root->child=0;
root->sibling=root;
root->parent=root;
cwd=root;
}


void mkdir(char *path){
if(path[0]=='\0' || (path[1]=='\0' && path[0]=='/'))printf("Invalid path/n");
else{
NODE *head=(NODE *) malloc(sizeof(NODE));

char *token=strtok(path,"/");
char temp[50];
if(token!=NULL)strcpy(temp,token);
if(path[0]=='/')
head=root;
else head=cwd;
if(head->child==NULL){
token=strtok(NULL,"/");
if(token!=NULL)strcpy(temp,token);
if(token==NULL){
NODE *new=(NODE *) malloc(sizeof(NODE));
strcpy(new->name,temp);
new->type='D';
new->child=0;
new->sibling=0;
new->parent=head;
head->child=new;
 printf("Success\n");
}else printf("Invalid path\n");
}
else{
head=head->child;
while(token!=NULL){

if(!strcmp(token,head->name) ||head->child==NULL){
token=strtok(NULL,"/");
if(token!=NULL)strcpy(temp,token);


if(token==NULL){
NODE *new=(NODE *) malloc(sizeof(NODE));
strcpy(new->name,temp);
new->type='D';
new->child=0;
if(head->child==NULL)
new->sibling=0;
else new->sibling=head->child;
new->parent=head;
head->child=new;
 printf("Success\n");break;
}
continue;
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
printf("Invalid path\n");break;
} 
}
}
}
}



void rmdir(char *path){
if(path[0]=='\0' ||(path[1]=='\0' && path[0]=='/'))printf("Invalid path/n");
else{
NODE *head=(NODE *) malloc(sizeof(NODE));
NODE *prev=(NODE *) malloc(sizeof(NODE));
int flag=0;
char *token=strtok(path,"/");
char temp[50];
if(token!=NULL)strcpy(temp,token);
if(path[0]=='/')
head=root;
else head=cwd;
prev=head;
while(token!=NULL){

if(!strcmp(head->name,temp) ){
//printf("yo\n");
char *token=strtok(NULL,"/");
if(token!=NULL)strcpy(temp,token);
if(token==NULL && head->child==NULL){
NODE *new=(NODE *) malloc(sizeof(NODE));
strcpy(new->name,temp);

if(!strcmp(head->parent->child->name,head->name)){
if(!strcmp(prev->name,head->name))head->parent->child=0;
else head->parent->child=prev;
}
if(head->sibling==NULL)
prev->sibling==NULL;
 printf("Success\n");break;
}else if(token==NULL && head->child!=NULL){
printf("Invalid path\n");break;
}
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
printf("Invalid path\n");break;
}
}
}
}
void ls(char *path){}


void cd(char *path){
if(path[0]=='\0' || (path[1]=='\0' && path[0]=='/')){
cwd=root;
printf("/\n");
}
else{
NODE *head=(NODE *) malloc(sizeof(NODE));
NODE *prev=(NODE *) malloc(sizeof(NODE));
int flag=0;
char *token=strtok(path,"/");
char temp[50];
if(token!=NULL)strcpy(temp,token);
if(path[0]=='/')
head=root;
else head=cwd;
prev=head;
while(token!=NULL){
if(!strcmp(head->name,temp) ){
//printf("yo\n");
char *token=strtok(NULL,"/");
if(token!=NULL)strcpy(temp,token);
if(token==NULL){
cwd=head;
 printf("Success\n");break;
}else{
printf("Invalid path\n");break;
}
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
printf("Invalid path\n");break;
}
}
}

}


void pwd(){
NODE *head=(NODE *)malloc(sizeof(NODE));
head=cwd;
STACK *cur=(STACK *)malloc(sizeof(STACK));
cur->down=NULL;
strcpy(cur->name,head->name);
while(strcmp(head->parent->name,head->name)){
head=head->parent;
STACK *new=(STACK *)malloc(sizeof(STACK));
new->down=cur;
strcpy(new->name,head->name);
cur=new;
}
while(cur->down!=NULL){

printf("%s",cur->name);
if(strcmp(cur->name,"/"))
printf("/");
cur=cur->down;
}

printf("%s\n",cur->name);
}
void creat(char *path){}
void rm(char *path){}
void reload(){}
void save(){}
void menu(){
printf("mkdir pathname :make a new directory for a given pathname\n"); 
printf("rmdir pathname :remove the directory, if it is empty.\n"); 
printf("cd [pathname] :change CWD to pathname, or to / if no pathname.\n"); 
printf("ls [pathname] :list the directory contents of pathname or CWD\n"); 
printf("pwd :print the (absolute) pathname of CWD\n"); 
printf("creat pathname :create a FILE node. \n");
printf("rm pathname :remove the FILE node. \n");
printf("save ﬁlename :save the current ﬁle system tree as a ﬁle\n");
printf("reload ﬁlename :construct a ﬁle system tree from a ﬁle\n");
printf("menu : show a menu of valid commands \n");
printf("quit : save the ﬁle system tree, then terminate the program.\n");
}
void quit(){
save();
exit(0);
}



int main(){
int index;
initialise();
while(1){
printf("input a command: ");
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
