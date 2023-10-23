#include <stdio.h>
#include <windows.h>
#include <string.h>
#include "manage.h"
#define Super_Manager_Code 666  //超级管理员密码
int teacher_num = 0, condition;//导师数量, condition用以判断返回的值是学员还是导师，1为导师，2为学员
void save(ptr_teacher* head){
        FILE *fp;
        if((fp = fopen("information.dat", "w")) == NULL){
                printf("\n\n\t\t\t文件打开错误！\n");
                return ;
        }
        ptr_teacher cur = *head, re_teacher = *head;
        ptr_student re_student, cur1;
        int i, j;//临时变量
        for(i= 0; i < teacher_num; i++){//当未到达最后的时候
                if(fwrite(cur, sizeof(teacher)-sizeof(ptr_teacher) - sizeof(ptr_student), 1, fp) != 1)
                        printf("\n\n\t\t\t导入失败!(导师)\n");
                if(cur->student_member > 0){//当导师的学员数不为零的时候，在文件里保存学员
                        cur1 = cur->head;//将导师的学生头指针赋值给cur1
                        for(j=0; j < cur->student_member; j++){
                                if(fwrite(cur1, sizeof(student)-sizeof(ptr_student), 1, fp) != 1)
                                        printf("\n\n\t\t导入失败!(学员)\n");
                                re_student = cur1;//储存变量用以销毁
                                cur1 = cur1->next;
                                free(re_student);
                        }
                }
                re_teacher = cur;
                cur = cur->next;
                free(re_teacher);
        }
        *head = NULL;
        fclose(fp);//关闭文件
}
Status create(ptr_teacher *head_teacher){/*根据文件内容生成一条链表，同时在各成功建立的结点处连入一条关于学员的链表
，最后返回导师头节点，失败返回NULL*/
        FILE *fp;
        if((fp = fopen("information.dat", "r")) == NULL){
                printf("\n\n\t\t\t文件打开错误！\n");
                return Error;
        }
        teacher temp;
        ptr_teacher cur, p; //cur代表当前开辟的空间的指针， p代表下一个
        *head_teacher = NULL;
        while(fread(&temp, sizeof(teacher)-2*sizeof(ptr_teacher), 1, fp) == 1){//将内容读取到temp中
                cur=(ptr_teacher)malloc(sizeof(teacher));//开辟导师空间
                if(teacher_num == 0)//当第一个导师时，把当前空间赋值给头指针
                        *head_teacher = cur;
                else
                        p->next = cur;
                *cur = temp;
                p = cur;
                /*学员的导入*/
                if(cur->student_member > 0){//当导师的学员数不为零的时候，在文件里继续添加学员
                        ptr_student cur1, p1;//cur1代表当前开辟的空间的指针， p1代表下一个
                        int i;
                        for(i=0; i < cur->student_member; i++){
                                cur1=(ptr_student)malloc(sizeof(student));
                                if(i == 0)
                                        cur->head = cur1;//把学员的头指针赋给导师的学员指针上
                                else
                                        p1->next = cur1;
                                if((fread(cur1, sizeof(student)-sizeof(ptr_student), 1, fp)) != 1){
                                        printf("\n\n\t\t\t文件读取错误(学员)！\n");
                                        return Error;
                                }
                                p1 = cur1;
                        }
                        p1->next = NULL;
                }
                else
                        cur->head = NULL;//学员为零,导师的学员头指针就放空指针
 
                teacher_num ++;///循环结束，导师数量加一
        }
        if(teacher_num > 0)//检查是否导入了导师
                p->next = NULL;
 
        fclose(fp);//关闭文件
 
        return Success;
}
int main(void){
        ptr_teacher head_teacher;//导师头指针
        if(create(&head_teacher) == Error)
                return;
        menu(login(), &head_teacher);//进入菜单
        save(&head_teacher);
 
        return 0;
}
int login(){//登陆界面
        void gotoxy(HANDLE hOut, int x, int y){//定位光标
                COORD pos;
                pos.X = x;             //横坐标
                pos.Y = y;            //纵坐标
                SetConsoleCursorPosition(hOut, pos);
        }
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);//定义显示器句柄变量
        int secret_code, i;
 
        printf("\n\n\t\t\t ______________________________________\n");
        printf("\t\t\t|                                      |\n");
        printf("\t\t\t|                                      |\n");
        printf("\t\t\t|  请输入管理员密码，否则进入学员系统  |\n");
        printf("\t\t\t|            密码：___                 |\n");
        printf("\t\t\t|______________________________________|\n");
        gotoxy(hOut,43,6);
        scanf("%d", &secret_code);
        fflush(stdin);
 
        printf("\n\t\t\tLoding：");//进入动画
        for(i=0; i<11; i++){
                Sleep(50);
                printf("█ ");
        }
        printf("\n\t\t\t载入完成，进入下一级系统");
        Sleep(1000);
        system("cls");//清屏 
 
        if(secret_code == Super_Manager_Code)//判断是不是管理员
                return 1;
        else
                return 0;
}
void menu(int flag, ptr_teacher *head){//菜单
        int choice;
        if(flag){
                do{
                        printf("\n\n\t\t\t ______________________________________________\n");
                        printf("\t\t\t|                 超级管理员模式               |\n");
                        printf("\t\t\t|                                              |\n");
                        printf("\t\t\t|   1.导师/学员信息      2.录入导师/学员信息   |\n");
                        printf("\t\t\t|                                              |\n");
                        printf("\t\t\t|   3.删除导师/学员信息  4.修改导师/学员信息   |\n");
                        printf("\t\t\t|                                              |\n");
                        printf("\t\t\t|   5.查找导师/学员信息  6.退出系统            |\n");
                        printf("\t\t\t|______________________________________________|\n");
                        printf("\t\t\t请选择功能（输入1-6任意一个数字）:");
                        scanf("%d", &choice);//避免空输入
                        fflush(stdin);//清空缓存区域
                        if(choice >= 0 && choice<=6){
                                system("cls");//把菜单清除
                                switch(choice){
                                        case 1:
                                                infomation(*head);
                                                break;
                                        case 2:
                                                input(head);
                                                break;
                                        case 3:
                                                delete(head);
                                                break;
                                        case 4:
                                                modify(*head);
                                                break;
                                        case 5:
                                                find(*head);
                                                break;
                                        default:;
                                }
                        }
                        else{
                                printf("\t\t\t请重新输入！Please input again!");
                                Sleep(500);
                        }
                        system("cls");
                }while(choice != 6);
        }
        else{
                do{
                        printf("\n\n\t\t\t ________________________________________\n");
                        printf("\t\t\t|                学员模式                |\n");
                        printf("\t\t\t|                                        |\n");
                        printf("\t\t\t|  1.学员/导师信息   2.查找学员/导师信息 |\n");
                        printf("\t\t\t|                                        |\n");
                        printf("\t\t\t|               3.退出系统               |\n");
                        printf("\t\t\t|________________________________________|\n");
                        printf("\t\t\t请选择功能（输入1-3任意一个数字）:");
 
                        scanf("%d", &choice);//避免空输入
                        fflush(stdin);//清空缓存区域
                        if(choice >= 0 && choice<=3){
                                system("cls");//把菜单清除
                                switch(choice){
                                        case 1:
                                                infomation(*head);
                                                break;
                                        case 2:
                                                find(*head);
                                        default:;
                                }
                        }
                        else{
                                printf("\t\t\t请重新输入！Please input again!");
                                Sleep(500);
                        }
                        system("cls");
                }while(choice != 3);
        }
}
void infomation(ptr_teacher head){//全部信息
        printf("\n\n\t\t\t导师和其学员信息\n\n");
        if(teacher_num == 0)//判断有无导师信息
                printf("\t\t\t无导师信息!\n");
        else{s
                printf("\t\t\t——————————\n");
                ptr_teacher cur = head;
                while(cur != NULL){
                        printf("\t\t\t姓名：%14s\n\t\t\t昵称：%14s\n\t\t\t性别：%14s\n\t\t\t导师编号：%10ld\n\t\t\t学员数量：%10d\n",
                        cur->name, cur->nickname, cur->sex, cur->code, cur->student_member);
                        printf("\t\t\t************\n");
                        printf("\t\t\t学员信息\n");
                        if(cur->student_member > 0){
                                ptr_student cur1 = cur->head;
                                while(cur1){
                                        printf("\t\t\t姓名：%14s\n\t\t\t性别：%14s\n\t\t\t学员编号：%10ld\n\t\t\t撩妹值：%12d\n\t\t\t装逼值：%12d\n",
                                        cur1->name, cur1->sex, cur1->code, cur1->glamour, cur1->niubility);
                                        cur1 = cur1->next;
                                        if(cur1)
                                                putchar('\n');//如果下一个不为空，打印换行
                                }
                        }
                        else
                                printf("\t\t\t此导师无学员\n");
                        printf("\t\t\t——————————\n");
                        cur = cur->next;
                }
        }
        printf("\t\t\t按任意键返回");
        getch();
}
void input(ptr_teacher *head){//添加信息
        int choice, flag=0;
        void gotoxy(HANDLE hOut, int x, int y){//定位光标
                COORD pos;
                pos.X = x;             //横坐标
                pos.Y = y;            //纵坐标
                SetConsoleCursorPosition(hOut, pos);
        }
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);//定义显示器句柄变量
        printf("\n\n\t\t\t1.添加导师 2.添加学员\n\n");
        printf("\t\t\t请选择(1-2): ");
        scanf("%d", &choice);
        fflush(stdin);
        system("cls");
        if(choice == 1){
                ptr_teacher cur, p, head_teacher = *head, *head_save = head;//cur代表当前开辟的空间的指针， p代表下一个
                while(head_teacher != NULL){//回到链表的最尾处
                        head_save = &(head_teacher->next);
                        head_teacher = head_teacher->next;
                }
                while(1){
                        cur=(ptr_teacher)malloc(sizeof(teacher));//开辟导师空间
                        if(flag == 0){//当第一个导师时，把当前空间赋值给头指针
                                *head_save = cur;
                                flag++;
                        }
                        else
                                p->next = cur;
                        printf("\n\n\t\t\t当前导师数：%d\n", teacher_num);
                        printf("\t\t\t请按以下项目依次输入\n");
                        printf("\t\t\t______________________\n");
                        printf("\t\t\t  姓名  ：\n\t\t\t  昵称  ：\n\t\t\t  性别  ：\n\t\t\t导师编号：\n");
                        printf("\t\t\t______________________\n");
                        gotoxy(hOut, 38, 5); gets(cur->name);
                        gotoxy(hOut, 38, 6); gets(cur->nickname);
                        gotoxy(hOut, 38, 7); gets(cur->sex);
                        gotoxy(hOut, 38, 8); scanf("%ld", &cur->code); fflush(stdin);
                        teacher_num++;
                        cur->student_member = 0;
 
                        printf("\n\t\t\t请问是否为其添加学员？(y/n)");
                        choice = getchar();
                        system("cls");fflush(stdin);
                        if(choice == 'Y' || choice == 'y'){
                                ptr_student cur1, p1;
                                while(1){
                                        cur1 = (ptr_student)malloc(sizeof(student));//开辟学员空间
                                        if(cur->student_member == 0)
                                                cur->head = cur1;//导师的学员数
                                        else
                                                p1->next = cur1;
                                        p1 = cur1;
                                        printf("\n\n\t\t\t当前学员数：%d\n", cur->student_member);
                                        printf("\t\t\t请按以下项目依次输入\n");
                                        printf("\t\t\t______________________\n");
                                        printf("\t\t\t  姓名  ：\n\t\t\t  性别  ：\n\t\t\t学员编号：\n\t\t\t 撩妹值 ：\n\t\t\t 装逼值 ：\n");
                                        printf("\t\t\t______________________\n");
                                        gotoxy(hOut, 38, 5); gets(cur1->name);
                                        gotoxy(hOut, 38, 6); gets(cur1->sex);
                                        gotoxy(hOut, 38, 7); scanf("%ld", &cur1->code);
                                        gotoxy(hOut, 38, 8); scanf("%d", &cur1->glamour);
                                        gotoxy(hOut, 38, 9); scanf("%d", &cur1->niubility); fflush(stdin);
 
                                        cur->student_member++;//学员数量增加一
                                        printf("\n\t\t\t请问是否还继续添加？(y/n):");
                                        choice = getchar();
                                        system("cls");fflush(stdin);//清屏清缓存
                                        if(choice =='N'||choice =='n')
                                                break;//退出，不继续添加
                                }
                                p1->next = NULL;
                        }
                        else
                                cur->head =NULL;
 
                        p = cur;//移动尾指针到下一位
                        printf("\n\n\t\t\t请问是否还继续添加导师？(y/n):");
                        choice = getchar();
                        system("cls");fflush(stdin);//清屏清缓存
                        if(choice =='N'||choice =='n')
                        break;//退出，不继续添加
                }
                p->next = NULL;
        }
        else if(choice == 2){//通过选择的方式
                int i, mark;
                printf("\n\n\t\t\t请先查找要添加学员的导师,按任意键继续下一步");
                getch();
                mark = find(*head);
                system("cls");
                if(mark != -1){
                        ptr_teacher cur = *head;
                        ptr_student cur1, p1, *head_save, head_student;
                        for(i=0; i<mark; i++)//检索到导入导师的头节点位置。
                                cur = cur->next;
                        head_save = &(cur->head);
                        head_student = cur->head;
                        while(head_student){//回到链表的最尾处
                                head_save = &(head_student->next);
                                head_student = head_student->next;
                        }
                        while(1){
                                        cur1 = (ptr_student)malloc(sizeof(student));//开辟学员空间
                                        if(flag == 0){//当第一个导师时，把当前空间赋值给头指针
                                                *head_save = cur1;
                                                flag++;
                                        }
                                        else
                                                p1->next = cur1;
                                        p1 = cur1;
                                        printf("\n\n\t\t\t当前此导师学员数：%d\n", cur->student_member);
                                        printf("\t\t\t请按以下项目依次输入\n");
                                        printf("\t\t\t______________________\n");
                                        printf("\t\t\t  姓名  ：\n\t\t\t  性别  ：\n\t\t\t学员编号：\n\t\t\t 撩妹值 ：\n\t\t\t 装逼值 ：\n");
                                        printf("\t\t\t______________________\n");fflush(stdin);
                                        gotoxy(hOut, 38, 4); gets(cur1->name);
                                        gotoxy(hOut, 38, 5); gets(cur1->sex);
                                        gotoxy(hOut, 38, 6); scanf("%ld", &cur1->code);
                                        gotoxy(hOut, 38, 7); scanf("%d", &cur1->glamour);
                                        gotoxy(hOut, 38, 8); scanf("%d", &cur1->niubility); fflush(stdin);
 
                                        cur->student_member++;//学员数量增加一
                                        printf("\n\t\t\t请问是否还继续添加？(y/n):");
                                        choice = getchar();
                                        system("cls");fflush(stdin);//清屏清缓存
                                        if(choice =='N'||choice =='n')
                                                break;//退出，不继续添加
                                }
                        p1->next = NULL;
                }//不用else，如果find查找不到，会返回查找不到！
        }
}
int find(ptr_teacher head){//查找信息
        int choice, i, flag=1, mark=-1;//choice 后期赋值关于选择的变量，i 暂时变量， flag 判断有没有找到人， mark 标记找的人的索引，最终返回
        ptr_teacher cur = head;
        printf("\n\n\t\t\t1.查找导师 2.查找学员\n\n");
        printf("\t\t\t请选择(1-2): ");
        scanf("%d", &choice);
        if(choice == 1){
                condition = 1;
                printf("\t\t\t______________________\n");
                printf("\t\t\t1.姓名查找  2.编号查找\n");
                printf("\t\t\t请选择(1-2): ");
                scanf("%d", &choice);
                fflush(stdin);
                if(choice == 1){
                        char temp[NAME];
                        printf("\t\t\t______________________\n");
                        printf("\t\t\t请输入名字：");
                        gets(temp);
                        for(i=0; i<teacher_num && flag == 1; i++){
                                if(strcmp(cur->name, temp)==0){
                                        printf("\t\t\t______________________\n");
                                        printf("\t\t\t  姓名  ：%12s\n\t\t\t  昵称  ：%12s\n\t\t\t  性别  ：%12s\n\t\t\t导师编号：%12ld\n\t\t\t学员数量：%12d\n",
                                        cur->name, cur->nickname, cur->sex, cur->code, cur->student_member);
                                        printf("\t\t\t______________________\n");
                                        printf("\n\t\t\t按任意键返回");
                                        getch();
                                        flag = 0, mark = i;
                                }
                                cur = cur->next;
                        }
                        if(flag){
                                printf("\t\t\t______________________\n");
                                printf("\t\t\t无此人!\n\t\t\t按任意键返回");
                                getch();
                        }
                }
                else if(choice == 2){
                        long temp_num;
                        printf("\t\t\t______________________\n");
                        printf("\t\t\t请输入导师编号：");
                        scanf("%ld", &temp_num);
                        for(i=0; i<teacher_num && flag == 1; i++){
                                if(temp_num == cur->code){
                                        printf("\t\t\t______________________\n");
                                        printf("\t\t\t  姓名  ：%12s\n\t\t\t  昵称  ：%12s\n\t\t\t  性别  ：%12s\n\t\t\t导师编号：%12ld\n\t\t\t学员数量：%12d\n",
                                        cur->name, cur->nickname, cur->sex, cur->code, cur->student_member);
                                        printf("\t\t\t______________________\n");
                                        printf("\n\t\t\t按任意键返回");
                                        getch();
                                        flag = 0, mark = i;
                                }
                                cur = cur->next;
                        }
                        if(flag){
                                printf("\t\t\t______________________\n");
                                printf("\t\t\t无此人!\n\t\t\t按任意键返回");
                                getch();
                        }
                }
                else{
                        printf("\n\n\t\t\t输入错误！\n\t\t\t按任意键返回");
                        getch();
                }
        }
        else if(choice == 2){
                ptr_student cur1;//用于表示学员内存块
                condition = 2;
                printf("\t\t\t______________________\n");
                printf("\t\t\t1.姓名查找  2.编号查找\n");
                printf("\t\t\t请选择(1-2): ");
                scanf("%d", &choice);
                fflush(stdin);
                if(choice == 1){
                        char temp[NAME];//名字中间变量
                        printf("\t\t\t______________________\n");
                        printf("\t\t\t请输入名字：");
                        gets(temp);
                        while(cur!=NULL){
                                cur1 = cur->head;
                                for(i=0; i<cur->student_member && flag == 1; i++){
                                        if(strcmp(cur1->name, temp)==0){
                                                printf("\t\t\t______________________\n");
                                                printf("\t\t\t  姓名  ：%12s\n\t\t\t  性别  ：%12s\n\t\t\t学员编号：%12ld\n\t\t\t 撩妹值 ：%12d\n\t\t\t 装逼值 ：%12d\n",
                                                cur1->name, cur1->sex, cur1->code, cur1->glamour, cur1->niubility);
                                                printf("\t\t\t______________________\n");
                                                printf("\n\t\t\t按任意键返回");
                                                getch();
                                                flag = 0, mark = i;
                                        }
                                        cur1 = cur1->next;
                                }
                                cur = cur->next;
                        }
                        if(flag){
                                printf("\t\t\t______________________\n");
                                printf("\t\t\t无此人!\n\t\t\t按任意键返回");
                                getch();
                        }
                }
                else if(choice == 2){
                        long temp_num;
                        printf("\t\t\t______________________\n");
                        printf("\t\t\t请输入学员编号：");
                        scanf("%ld", &temp_num);
                        while(cur!=NULL){
                                cur1 = cur->head;
                                for(i=0; i<cur->student_member && flag == 1; i++){
                                        if(temp_num == cur1->code){
                                                printf("\t\t\t______________________\n");
                                                printf("\t\t\t  姓名  ：%12s\n\t\t\t  性别  ：%12s\n\t\t\t学员编号：%12ld\n\t\t\t 撩妹值 ：%12d\n\t\t\t 装逼值 ：%12d\n",
                                                cur1->name, cur1->sex, cur1->code, cur1->glamour, cur1->niubility);
                                                printf("\t\t\t______________________\n");
                                                printf("\n\t\t\t按任意键返回");
                                                getch();
                                                flag = 0, mark = i;
                                        }
                                        cur1 = cur1->next;
                                }
                                cur = cur->next;
                        }
                        if(flag){
                                printf("\t\t\t______________________\n");
                                printf("\t\t\t无此人!\n\t\t\t按任意键返回");
                                getch();
                        }
                }
                else{
                        printf("\n\n\t\t\t输入错误！\n\t\t\t按任意键返回");
                        getch();
                }
        }
        else{
                printf("\n\n\t\t\t输入错误！\n\t\t\t\t按任意键返回");
                getch();
        }
        return mark;
}
void modify(ptr_teacher head){//修改信息
        int choice, mark, i;
        void gotoxy(HANDLE hOut, int x, int y){//定位光标
                COORD pos;
                pos.X = x;             //横坐标
                pos.Y = y;            //纵坐标
                SetConsoleCursorPosition(hOut, pos);
        }
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);//定义显示器句柄变量
        printf("\n\n\t\t\t请先查找要修改导师,再查找要修改的学员，按任意键继续。");
        getch();
        system("cls");
        mark = find(head);
        system("cls");
        if(mark!=-1){
                ptr_teacher cur = head;
                for(i=0; i<mark; i++)
                        cur=cur->next;
                printf("\n\n\t\t\t请按以下项目重新输入\n");
                printf("\t\t\t______________________\n");
                printf("\t\t\t姓名：\n\t\t\t昵称：\n\t\t\t性别：\n\t\t\t导师编号：\n");
                printf("\t\t\t______________________");fflush(stdin);
                gotoxy(hOut, 38, 4); gets(cur->name);
                gotoxy(hOut, 38, 5); gets(cur->nickname);
                gotoxy(hOut, 38, 6); gets(cur->sex);
                gotoxy(hOut, 38, 7); scanf("%ld", &cur->code);fflush(stdin);
                printf("\t\t\t______________________\n");
                fflush(stdin);
                if(cur->student_member > 0){
                        printf("\t\t\t请问是否还需要修改此导师的学员？（y/n）：");
                        choice = getchar();fflush(stdin);system("cls");
                        if(choice == 'y'|| choice == 'Y'){
                                printf("\n\n\t\t\t请查找要修改的学员，按任意键继续");
                                getch();
                                system("cls");
                                mark = find(head);
                                system("cls");
                                if(mark!=-1){
                                        ptr_student cur1 = cur->head;
                                        for(i=0; i<mark; i++)
                                                cur1 = cur1->next;
                                        printf("\n\n\t\t\t请按以下项目依次输入\n");
                                        printf("\t\t\t______________________\n");
                                        printf("\t\t\t  姓名  ：\n\t\t\t  性别  ：\n\t\t\t学员编号：\n\t\t\t  撩妹值：\n\t\t\t  装逼值：\n");
                                        printf("\t\t\t______________________\n");
                                        fflush(stdin);
                                        gotoxy(hOut, 38, 4); gets(cur1->name);
                                        gotoxy(hOut, 38, 5); gets(cur1->sex);
                                        gotoxy(hOut, 38, 6); scanf("%ld", &cur1->code);
                                        gotoxy(hOut, 38, 7); scanf("%d", &cur1->glamour);
                                        gotoxy(hOut, 38, 8); scanf("%d", &cur1->niubility); fflush(stdin);
                                        printf("\t\t\t______________________\n");
                                        printf("\n\t\t\t按任意键返回");
                                        getch();
                                }
                        }
                }
                else{
                        printf("\n\t\t\t按任意键返回");
                                getch();
                }
        }//不用做else，_find函数若查找不到会返回查找不到！
}
void delete(ptr_teacher *head){//删除信息
        char choice;
        int i, mark;
        printf("\n\n\t\t\t请先查找要删除的导师,或查找要删除的学员的导师，按任意键继续");
        getch();
        system("cls");
        mark = find(*head);
        if(mark!=-1){
                printf("\n\t\t\t______________________\n");
                printf("\t\t\t请问是想删除导师还是其学员？（1/2）：");
                fflush(stdin);
                choice = getchar();
                if(choice == '1'){
                        ptr_teacher cur = *head, *precur=head;
                        ptr_student cur1, restudent;
                        for(i=0; i<mark; i++){
                                precur = &(cur->next);
                                cur = cur->next;
                        }
                        cur1 = cur->head;
                        while(cur1){//释放学员空间
                                restudent = cur1;
                                cur1 = cur1->next;
                                free(restudent);
                        }
                        *precur = cur->next;
                        free(cur);
                        cur = NULL;
                        teacher_num--;
                        printf("\t\t\t删除成功！按任意键返回");
                        getch();
                }
                else if(choice == '2'){
                        ptr_teacher cur = *head;
                        ptr_student cur1,  *precur1;
                        for(i=0; i<mark; i++)//定位导师位置
                                cur = cur->next;
 
                        precur1 = &(cur->head);
                        cur1 = cur->head;
                        printf("\n\n\t\t\t请查找要删除的学员，按任意键继续");
                        getch();
                        system("cls");
                        mark = find(*head);
                        for(i=0; i<mark; i++){
                                precur1 = &(cur1->next);
                                cur1 = cur1->next;
                        }
                        *precur1 = cur1->next;
                        free(cur1);
                        cur1 = NULL;
                        cur->student_member--;
                        printf("\n\t\t\t删除成功！按任意键返回");
                        getch();
                }
                else{
                        printf("\t\t\t删除失败！按任意键返回");
                        getch();
                }
        }//不用做else，_find函数若查找不到会返回查找不到！
}



