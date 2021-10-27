#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


typedef struct Node{
    char MSSV[9];
    char Ho[20];
    char Ten[10];
    float diemGK;
    float diemCK;
    float diemTK;
    char tk[3];
    struct Node *next;
} node;

typedef struct Subject
{
    char SubjectID[7];
    char Subject[30];
    int gk;
    int ck;
    char Semester[6];
    int StudentCount;
    float max;
    float min;
    float diemTB;
    node *List;
    struct Subject* next;
}Subject;

void Traverser(Subject* head);
char* convertDiem(float diem);
Subject* checkSubject(Subject* monhoc);
Subject* CreateSubject(Subject value);

node* CreateNode(node value){
    node *temp; // declare a node
    temp = (node*)malloc(sizeof(node)); // Cấp phát vùng nhớ dùng malloc()
    temp->next = NULL;// Cho next trỏ tới NULL
    strcpy(temp->MSSV,value.MSSV);
    strcpy(temp->Ho, value.Ho);
    strcpy(temp->Ten, value.Ten);
    strcpy(temp->tk, value.tk);
    temp->diemGK = value.diemGK;
    temp->diemCK = value.diemCK;
    temp->diemTK = value.diemTK;
    return temp;//Trả về node mới đã có giá trị
}

node* AddHead(node *head, node value){
    node* temp = CreateNode(value); // Khởi tạo node temp với data = value
    if(head == NULL){
        head = temp; // //Nếu linked list đang trống thì Node temp là head luôn
    }else{
        temp->next = head; // Trỏ next của temp = head hiện tại
        head = temp; // Đổi head hiện tại = temp(Vì temp bây giờ là head mới mà)
    }
    return head;
}

Subject* AddHeadSubject(Subject *head, Subject value){
    Subject* temp = CreateSubject(value); // Khởi tạo node temp với data = value
    if(head == NULL){
        head = temp; // //Nếu linked list đang trống thì Node temp là head luôn
    }else{
        temp->next = head; // Trỏ next của temp = head hiện tại
        head = temp; // Đổi head hiện tại = temp(Vì temp bây giờ là head mới mà)
    }
    return head;
}

node* AddTail(node* head, node value){
    node *temp,*p;// Khai báo 2 node tạm temp và p
    temp = CreateNode(value);//Gọi hàm createNode để khởi tạo node temp có next trỏ tới NULL và giá trị là value
    if(head == NULL){
        head = temp;     //Nếu linked list đang trống thì Node temp là head luôn
    }
    else{
        p  = head;// Khởi tạo p trỏ tới head
        while(p->next != NULL){
            p = p->next;//Duyệt danh sách liên kết đến cuối. Node cuối là node có next = NULL
        }
        p->next = temp;//Gán next của thằng cuối = temp. Khi đó temp sẽ là thằng cuối(temp->next = NULL mà)
    }
    return head;
}

Subject* AddTailSubject(Subject* head, Subject value){
    Subject *temp,*p;// Khai báo 2 node tạm temp và p
    temp = CreateSubject(value);//Gọi hàm createNode để khởi tạo node temp có next trỏ tới NULL và giá trị là value
    if(head == NULL){
        head = temp;     //Nếu linked list đang trống thì Node temp là head luôn
    }
    else{
        p  = head;// Khởi tạo p trỏ tới head
        while(p->next != NULL){
            p = p->next;//Duyệt danh sách liên kết đến cuối. Node cuối là node có next = NULL
        }
        p->next = temp;//Gán next của thằng cuối = temp. Khi đó temp sẽ là thằng cuối(temp->next = NULL mà)
    }
    return head;
}

node* DelHead(node* head){
    if(head == NULL){
        printf("\nCha co gi de xoa het!");
    }else{
        head = head->next;
    }
    return head;
}

node* DelTail(node* head){
    if (head == NULL || head->next == NULL){
         return DelHead(head);
    }
    node *p = head;
    while(p->next->next != NULL){
        p = p->next;
    }
    node* tmp = p->next;
    p->next = p->next->next; // Cho next bằng NULL
    free(tmp);
    tmp = NULL;
    return head;
}
 
node* DelAt(node *head, int position){
    if(position == 0 || head == NULL || head->next == NULL){
        head = DelHead(head); // Nếu vị trí chèn là 0, tức là thêm vào đầu
    }else{
        int k = 1;
        node *p = head;
        while(p->next->next != NULL && k != position){
            p = p->next;
            ++k;
        }
 
        if(k != position){
            head = DelTail(head);
        }else{
            node* tmp = p->next;
            p->next = p->next->next;
            free(tmp);
            tmp = NULL;
        }
    }
    return head;
}

node* Get(node* head, char *MSSV){
    int k = 0;
    node *p = head;
    while(p->next != NULL && strcmp(p->MSSV, MSSV) != 0){
        ++k;
        p = p->next;
    }
    return p;
}

int Search(node* head, char *MSSV){
    int position = 0;
    for(node *p = head; p != NULL; p = p->next){
        if(strcmp(p->MSSV, MSSV) == 0){
            return position;
        }
        ++position;
    }
    return -1;
}

Subject* Input(Subject* head){
    node* value = (node*)malloc(sizeof(node));;
    //head = (node*)malloc(sizeof(node));
    printf("\nNhap thong tin sinh vien: \n");
    printf("MSSV: ");
    scanf("%s",value->MSSV);
    printf("Ho: ");
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
    scanf ("%[^\n]%*c", value->Ho);
    printf("Ten: ");
    scanf("%s",value->Ten);
    printf("Diem GK: ");
    scanf("%e",&value->diemGK);
    printf("Diem CK: ");
    scanf("%e",&value->diemCK);
    value->diemTK = (value->diemGK*head->gk + value->diemCK*head->ck)/(head->ck+head->gk);
    strcpy(value->tk,convertDiem(value->diemTK));
    head->List = AddTail(head->List, *value);
    free(value);
    return head;
}

int Length(node *head){
    int length = 0;
    for(node* p = head; p != NULL; p = p->next){
        ++length;
    }
    return length;
}

int addScore(Subject* monhoc, int k){
    Subject *tmp = checkSubject(monhoc);
    if(tmp == NULL){
        printf("Khong ton tai\n");
        return -1;
    }
    if(Length(tmp->List) >= tmp->StudentCount){
        printf("\nSo sinh vien da day\nKhong the them sinh vien.\n");
        return -1;
    }
    tmp = Input(tmp);
    return 1;
}

void InDiem(char* diem,int n, FILE* fp){
    fprintf(fp,"%s:",diem);
    for (int i = 0; i < n; i++)
    {
        fprintf(fp,"*");
    }
    fprintf(fp,"\n");
}

void InDiem1(char* diem,int n){
    printf("%s:",diem);
    for (int i = 0; i < n; i++)
    {
        printf("*");
    }
    printf("\n");
}

void GhiFile(Subject head){
    FILE* fp = NULL;
    char* name = (char*)malloc(sizeof(char)*20);
    strcpy(name,head.SubjectID);
    if(name == NULL)
        return;
    strcat(name,"_");
    strcat(name,head.Semester);
    strcat(name,"_rp");
    strcat(name,".txt");
    fp = fopen(name,"w");
    head.diemTB = 0;
    head.max = 0;
    head.min = 10;
    for(node* p = head.List; p != NULL; p = p->next){
        if(head.max < p->diemTK)
        head.max = p->diemTK;
        if(head.min > p->diemTK)
            head.min = p->diemTK;
        head.diemTB += p->diemTK/((float)Length(head.List));
    }
    int max = 0, min = 0;
    int a = 0, b = 0, c = 0, d = 0,f = 0; 
    for(node* p = head.List; p != NULL; p = p->next){
        if(strcmp(p->tk,"A") == 0)
            ++a;
        if(strcmp(p->tk,"B") == 0)
            ++b;
        if(strcmp(p->tk,"C") == 0)
            ++c;
        if(strcmp(p->tk,"D") == 0)
            ++d;
        if(strcmp(p->tk,"F") == 0)
            ++f;
        if(p->diemTK == head.max && max == 0){
            fprintf(fp,"The student with the highest mark is: %s %s\n",p->Ho,p->Ten);
            max++;
        }
        if(p->diemTK == head.min && min == 0){
            fprintf(fp,"The student with the lowest mark is: %s %s\n",p->Ho,p->Ten);
            min++;
        }
    }
    fprintf(fp,"The average mark is: %.2f\n\n",head.diemTB);
    fprintf(fp,"\nA histogram of the subject %s is:\n",head.SubjectID);
    InDiem("A",a,fp);    
    InDiem("B",b,fp);
    InDiem("C",c,fp);
    InDiem("D",d,fp);
    InDiem("F",f,fp);
    fclose(fp);
    free(name);
}

void Traverser(Subject* head){
    printf("\n");
    head->diemTB = 0;
    head->max = 0;
    head->min = 10;
    for(node* p = head->List; p != NULL; p = p->next){
        printf("%s|%s\t|%s\t|%5.1f |%5.1f | ",p->MSSV,p->Ho,p->Ten,p->diemGK,p->diemCK);
        printf(" %s |\n",p->tk);
        if(head->max < p->diemTK)
        head->max = p->diemTK;
        if(head->min > p->diemTK)
            head->min = p->diemTK;
        head->diemTB += p->diemTK/((float) Length(head->List));
    }
    printf("\n");
    int max = 0, min = 0;
    int a = 0, b = 0, c = 0, d = 0,f = 0; 
    for(node* p = head->List; p != NULL; p = p->next){
        if(strcmp(p->tk,"A") == 0)
            ++a;
        if(strcmp(p->tk,"B") == 0)
            ++b;
        if(strcmp(p->tk,"C") == 0)
            ++c;
        if(strcmp(p->tk,"D") == 0)
            ++d;
        if(strcmp(p->tk,"F") == 0)
            ++f;
        if(p->diemTK == head->max && max == 0){
            printf("The student with the highest mark is: %s %s\n",p->Ho,p->Ten);
            //fprintf(fp,"The student with the highest mark is: %s %s\n",p->Ho,p->Ten);
            max++;
        }
        if(p->diemTK == head->min && min == 0){
            printf("The student with the lowest mark is: %s %s\n",p->Ho,p->Ten);
            //fprintf(fp,"The student with the lowest mark is: %s %s\n",p->Ho,p->Ten);
            min++;
        }
    }
    printf("The average mark is: %.2f\n\n",head->diemTB);
    //fprintf(fp,"The average mark is: %.2f\n\n",head->diemTB);
    printf("\nA histogram of the subject %s is:\n",head->SubjectID);
    InDiem1("A",a);    
    InDiem1("B",b);
    InDiem1("C",c);
    InDiem1("D",d);
    InDiem1("F",f);
}

Subject* CreateSubject(Subject value){
    Subject *temp; // declare a node
    temp = (Subject*)malloc(sizeof(Subject)); // Cấp phát vùng nhớ dùng malloc()
    temp->next = NULL;// Cho next trỏ tới NULL
    strcpy(temp->SubjectID,value.SubjectID);
    strcpy(temp->Subject, value.Subject);
    strcpy(temp->Semester, value.Semester);
    // strcpy(temp->max);
    temp->max = value.max;
    temp->min = value.min;
    temp->gk = value.gk;
    temp->ck = value.ck;
    temp->StudentCount = value.StudentCount;
    temp->List = NULL;
    return temp;//Trả về node mới đã có giá trị
}

Subject* addSubject(Subject* monhoc, int k){
    Subject* tmp = (Subject*)malloc(sizeof(Subject));
    printf("\nSubjectID: ");
    scanf("%s",tmp->SubjectID);
    printf("Subject: ");
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
    scanf ("%[^\n]%*c", tmp->Subject);
    printf("He so gk: ");
    scanf("%d",&tmp->gk);
    printf("He so ck: ");
    scanf("%d",&tmp->ck);
    printf("Ky hoc: ");
    scanf("%s",tmp->Semester);
    printf("StudentCount: ");
    scanf("%d",&tmp->StudentCount);
    monhoc = AddTailSubject(monhoc,*tmp);
    free(tmp);
    return monhoc;
}

Subject* checkSubject(Subject* monhoc){
    Subject* tmp = (Subject*)malloc(sizeof(Subject));
    printf("Nhap SubjectID: ");
    scanf("%s",tmp->SubjectID);
    printf("Nhap Semester: ");
    scanf("%s", tmp->Semester);
    // for (int i = 0; i < k; i++)
    // {
    //     if (strcmp(monhoc[i].SubjectID, tmp->SubjectID) == 0 &&
    //         strcmp(monhoc[i].Semester, tmp->Semester) == 0)
    //     {
    //         free(tmp);
    //         return (monhoc + i);
    //     }
    // }
    Subject *p = monhoc;
    while(p->next != NULL && strcmp(p->SubjectID, tmp->SubjectID) != 0 && strcmp(p->Semester, tmp->Semester) != 0){
        p = p->next;
    }
    free(tmp);
    return p;
}

int removeScore(Subject *monhoc){
    Subject* tmp = checkSubject(monhoc);
    if(tmp == NULL){
        printf("Khong ton tai\n");
        return 0;
    }
    printf("\nMon hoc: %s\n",tmp->SubjectID);
    printf("\nNhap MSSV: ");
    char* MSSV = (char*)malloc(sizeof(char)*9);
    scanf("%s",MSSV);
    int po = Search(tmp->List,MSSV);
    free(MSSV);
    if (po == -1)
    {
        printf("Khong ton tai sinh vien nay\n");
    }else{
        tmp->List = DelAt(tmp->List,po);
        return 1;
    }
    return 0;
}

void timSinhVien(Subject* monhoc, int k){
    Subject* tmp = checkSubject(monhoc);
    if(tmp == NULL){
        printf("Khong ton tai\n");
        return ;
    }
    printf("\nMon hoc: %s\n",tmp->SubjectID);
    printf("Nhap MSSV: ");
    char* MSSV = (char*)malloc(sizeof(char)*20);
    scanf("%s",MSSV);
    node* resuilt = Get(tmp->List,MSSV);
    free(MSSV);
    if (resuilt == NULL)
    {
        printf("Khong tim thay sinh vien.\n");
    }else{
        printf("Thong tin sinh vien: \n");
        printf("MSSV: %s\nHo va ten: %s %s\nDiem gk: %.1f\nDiem ck: %.1f\nDiem chu: %s\n",resuilt->MSSV,resuilt->Ho,resuilt->Ten, resuilt->diemGK, resuilt->diemCK,resuilt->tk);
    }
    
}

void ThongTinMonHoc(Subject* monhoc, int k){
    Subject* tmp = checkSubject(monhoc);
    if (tmp == NULL)
    {
        printf("Khong tim thay mon hoc.\n");
    }else{
        printf("\nSubjectID|%s",tmp->SubjectID);
        printf("\nSubject|%s",tmp->Subject);
        printf("\nF|%d|%d",tmp->gk,tmp->ck);
        printf("\nSemester|%s",tmp->Semester);
        printf("\nStudentCount|%d\n\n",tmp->StudentCount);
        Traverser(tmp);
    }
    
}

char *trimString(char *str){
    char *end; /*Khai con trỏ của ký tự cuối cùng*/
    
    /*Vòng lặp đầu tiên để xóa khoảng trắng từ đầu chuỗi*/
    while(isspace((unsigned char)*str)) str++;

    /*Nếu vòng lặp thực hiện hết cả chuỗi thì trả về chuỗi str*/
    if(*str == 0) return str; 

    
    /*end là vị trí cuối chuỗi, cũng là vị trí bắt đầu vòng lặp thứ 2*/
    end = str + strlen(str) - 1;      
    /*Vòng lặp thứ hai để xóa khoảng trắng từ cuối chuỗi*/
    while(end > str && isspace((unsigned char)*end)) end--;

    end[1] = '\0';

    return str;
}

char* convertDiem(float diem){

    if (diem >= 0 && diem < 4){
        return "F";
    }else if (diem >= 4 && diem < 5.5){
        return "D";
    }else if (diem >= 5.5 && diem < 7){
        return "C";
    }else if (diem >= 7 && diem < 8.5){
        return "B";
    }else
        return "A";
    
}

void freeList(node* head)
{
    node* tmp;

   while (head != NULL)
    {
       tmp = head;
       head = head->next;
       free(tmp);
    }

}

void freeSubject(Subject* head)
{
    Subject* tmp;

    while (head != NULL)
    {
       tmp = head;
       GhiFile(*tmp);
       head = head->next;
       free(tmp);
    }
}

Subject* DocFile(Subject* monhoc, char* name){
    Subject* temp = (Subject*)malloc(sizeof(Subject));
    FILE* fp = NULL;
    fp = fopen(name,"r");
    if (fp == NULL)
    {
        printf("File khong ton tai");
        return NULL;
    }
    char* a = (char*)malloc(sizeof(char)*200);
    int k = 0;
    fgets(a,200,fp);
    char* tmp;
    tmp = strtok(a,"|");
    tmp = strtok(NULL,"|");
    strcpy(temp->SubjectID,tmp);
    temp->SubjectID[6] = '\0';
    memset(a,0,200);
    fgets(a,200,fp);
    tmp = strtok(a,"|");
    tmp = strtok(NULL,"|");
    strcpy(temp->Subject,tmp);
    temp->Subject[strlen(temp->Subject) - 1] = '\0';
    memset(a,0,200);
    fgets(a,200,fp);
    tmp = strtok(a,"|");
    tmp = strtok(NULL,"|");
    temp->gk = atoi(tmp);
    tmp = strtok(NULL,"|");
    temp->ck = atoi(tmp);
    memset(a,0,200);
    fgets(a,200,fp);
    tmp = strtok(a,"|");
    tmp = strtok(NULL,"|");
    strcpy(temp->Semester,tmp);
    temp->Semester[5] = '\0';
    memset(a,0,200);
    fgets(a,200,fp);
    tmp = strtok(a,"|");
    tmp = strtok(NULL,"|");
    temp->StudentCount = atoi(tmp);
    temp->List = NULL;
    temp->max = 0;
    temp->min = 10;
    temp->diemTB = 0;
    for(int i = 0; i < temp->StudentCount; i++){
        memset(a,0,200);
        fgets(a,200,fp);
        node sv;
        tmp = strtok(a,"|");
        tmp = strtok(NULL,"|");
        strcpy(sv.MSSV,tmp);
        tmp = strtok(NULL,"|");
        strcpy(sv.Ho,trimString(tmp));
        tmp = strtok(NULL,"|");
        strcpy(sv.Ten,trimString(tmp));
        tmp = strtok(NULL,"|");
        sv.diemGK = (float)atof(trimString(tmp));
        tmp = strtok(NULL,"|");
        sv.diemCK = (float)atof(trimString(tmp));
        tmp = strtok(NULL,"|");
        strncpy(sv.tk,trimString(tmp),2);
        sv.diemTK = (sv.diemGK*temp->gk + sv.diemCK*temp->ck)/(temp->ck + temp->gk);
        if(temp->max < sv.diemTK){
            temp->max = sv.diemTK;
        }
        if(temp->min > sv.diemTK)
            temp->min = sv.diemTK;
        temp->diemTB += sv.diemTK/temp->StudentCount; 
        monhoc = AddHeadSubject(monhoc,*temp);
        monhoc->List = AddTail(monhoc->List, sv);
    }
    fclose(fp);
    free(a);
    //free(temp);
    return monhoc;
    
}

int main(int argc, char *argv[])
{
    int k = 0;
    Subject* monhoc;
    if(argc > 1){
        monhoc = DocFile(monhoc,argv[1]);
        k = 1;
    }
    int chon;
    char check;
    do
    {
        printf("\nLearning Management System");
        printf("\n-------------------------------------");
        printf("\n\t1.\tAdd a new score board");
        printf("\n\t2.\tAdd score");
        printf("\n\t3.\tRemove score");
        printf("\n\t4.\tSearch score");
        printf("\n\t5.\tDisplay score board and score report");
        printf("\nYour choice (1-5, other to quit): ");
        scanf("%d",&chon);
        int f = 0;
        switch (chon)
        {
        case 1:
            do{
                monhoc = addSubject(monhoc,k);
                ++k;
                printf("Ban co muon thuc hien chuc nang nay nua khong: ");
                int c;
                while ((c = getchar()) != '\n' && c != EOF) { }
                scanf("%c",&check);

            }while(check == 'y' || check == 'Y');

            break;
        case 2:
            do{
                f = addScore(monhoc,k);
                printf("Ban co muon thuc hien chuc nang nay nua khong: ");
                int c;
                while ((c = getchar()) != '\n' && c != EOF) { }
                scanf("%c",&check);

            }while(check == 'y' || check == 'Y');
            //Traverser(monhoc[f]);
            break;
        case 3:
            do{
                if (removeScore(monhoc))
                {
                    printf("Xoa thanh cong.\n");
                }else
                    printf("Xoa that bai.\n");
                
                printf("Ban co muon thuc hien chuc nang nay nua khong: ");
                int c;
                while ((c = getchar()) != '\n' && c != EOF) { }
                scanf("%c",&check);

            }while(check == 'y' || check == 'Y');
            break;
        case 4:
            do{
                timSinhVien(monhoc,k);
                printf("Ban co muon thuc hien chuc nang nay nua khong: ");
                int c;
                while ((c = getchar()) != '\n' && c != EOF) { }
                scanf("%c",&check);
            }while(check == 'y' || check == 'Y');
            break;
        case 5:
            do{
                ThongTinMonHoc(monhoc,k);
                printf("Ban co muon thuc hien chuc nang nay nua khong: ");
                int c;
                while ((c = getchar()) != '\n' && c != EOF) { }
                scanf("%c",&check);
            }while(check == 'y' || check == 'Y');
        }
    } while (chon <= 5 && chon >= 0);

    // for (int i = 0; i < k; i++)
    // {
    //     GhiFile(monhoc[i]);
    //     freeList(monhoc[i].List);
    // }
    free(monhoc);
    return 0;
}
