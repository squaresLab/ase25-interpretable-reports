//FormAI DATASET v1.0 Category: Email Client ; Style: Alan Touring
#include<stdio.h>
#include<stdlib.h>

// Define the structure for Email
struct Email{
    char sender[50];
    char recipient[50];
    char subject[100];
    char message[500];
};

int main()
{
    struct Email email;
    int ch = 1;
    FILE *fp;

    while(ch != 0){
        printf("\nWelcome to C Email Client:\n");
        printf("\n1. Compose Email\n2. Read Email\n0. Exit");
        printf("\nEnter your choice: ");
        scanf("%d", &ch);

        switch(ch){
            case 1:
                printf("\nEnter Sender's Email: ");
                scanf("%s", email.sender);
                printf("Enter Recipient's Email: ");
                scanf("%s", email.recipient);
                printf("Enter Email Subject: ");
                scanf("%s", email.subject);
                printf("Enter Email Message: ");
                scanf("%s", email.message);

                char filename[50];
                printf("\nEnter File Name to Save Email: ");
                scanf("%s",filename);
                fp=fopen(filename,"w+");

                fprintf(fp, "Sender: %s\n", email.sender);
                fprintf(fp, "Recipient: %s\n", email.recipient);
                fprintf(fp, "Subject: %s\n", email.subject);
                fprintf(fp, "Message: %s\n", email.message);

                printf("\nEmail saved Successfully!\n");
                fclose(fp);
                break;

            case 2:
                printf("\nEnter File Name to Read Email: ");
                scanf("%s",filename);
                fp=fopen(filename,"r");

                if(fp==NULL){
                    printf("\nError! File not found!\n");

                }
                else{
                    printf("\nEmail Details:\n");
                    while(!feof(fp)){
                        fscanf(fp, "%s\n", &email.sender);
                        printf("Sender: %s\n", email.sender);

                        fscanf(fp, "%s\n", &email.recipient);
                        printf("Recipient: %s\n", email.recipient);

                        fscanf(fp, "%s\n", &email.subject);
                        printf("Subject: %s\n", email.subject);

                        fscanf(fp, "%s\n", &email.message);
                        printf("Message: %s\n", email.message);

                        printf("\n");
                    }
                    fclose(fp);
                }
                break;

            case 0:
                printf("\nThank you for using C Email Client!");
                exit(0);
                break;

            default:
                printf("\nInvalid Choice! Please Try Again!\n");
                break;
        }
    }
    return 0;
}