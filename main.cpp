/*
 *  WarehouseLogisticsManagementSystem
 *  Designed by AToeryNitre
 *  Copyright (C) 2023 MAShiroSystem
 *
 *  @ file: main.cpp
 *  @ brief: �ֿ���������ϵͳ
 *  @ author: AToeryNitre
 *  @ version: MA20231115Version0.1.0f
 *  @ date: 2023-12-07
 *
 * */

#include <iostream>
#include <string>
#include <sstream>
#include <cstdint>
#include <cstdlib>
#include <malloc.h>

#include <typeinfo>
#include <cstring>

using namespace std;

/*
 * @brief �궨��
 * */
// ���洢��
#define MAX_LIST_SIZE                       30
// ������˵�������
#define MAX_MENU_LIST                       6
// ����Ӳ˵�[��ѯ��������]������
#define MAX_INQUIRE_INFORMATION_LIST        4
// ����Ӳ˵�[����Ա����]������
#define MAX_ADMIN_FUCTION_LIST              4

/*
 * @brief ����ָ��
 * */
void (*fuction_run_operation) ();

/*
 * @brief Ԫ������
 * */
typedef int ElemType;

/*
 * @brief ����
 *  1.�Ʊ�&�ָ�������
 *  2.���̱��ⳣ��
 * */
// �ָ����ʽ_ˮƽ
const char separator = '*';
// �ָ�����ʽ_��ֱ
const char separator_vertical = '#';
// ת��ϵ��
const uint8_t conversion_factor = 2;
// һ�������ַ��ĳ���
const uint8_t character_length  = 2;
// ���˵�����&���̱���
const string PROJECT_TITLE = "�ֿ���������ϵͳ";

/*
 * @brief ��������
 * */
// * ���˵�����_fuction
void user_exit_system();
void inquire_information();
void inquire_information_all();
void add_data();
void delete_data();
void revise_data();
void admin_fuction();

// * �Ӳ˵�_��ѯ��������
void inquire_information_date();
void inquire_information_tracking_number();
void inquire_information_name();

// * �Ӳ˵�_����Ա����
void admin_fuction_clearall();
void admin_fuction_exit();
void admin_fuction_length();
void damin_fuction_dateprin();

// * ����
void message_error(string title, string message, int line);
void dividing_line_title (string title_name,int line);
void dividing_line (int line);
void print_menu (struct menu_item date[], string title, int separator_length, int max_list);
void do_fuction (uint8_t number_of_operations, struct menu_item data[]);
uint8_t get_user_input ();

/*
 * @brief ��Ʒ��С
 *  1.small     С
 *  2.medium    ��
 *  3.large     ��
 * */
enum goods_size {
    small,
    medium,
    large
};

/*
 * @brief �ֿ����״̬
 *  1.in_stock  ���
 *  2.out_stock ����
 * */
enum storage_status {
    in_stock,
    out_stock
};

/*
 * fuction��Ԥ��װ����
 * */
// �����Ļ��ʾ����
void clear_display (){
    // ��ִ����������ش������������
    system ("cls");
}
// ��ͣ��Ļ��ʾ����
void pause_display (){
    // ����ͣ��Ļ��ʾ��ش������������
    system ("pause");
}

/*
 * @brief �˵��ṹ��
 *  1.index���˵�������
 *  2.fuction_name��������
 *  3.fuction�����ܺ���
 * */
struct menu_item {
    int index;
    string fuction_name;
    void (*fuction) ();
};

/*
 * @brief ���ڽṹ��
 *  �洢�ṹ��YYYY-MM-DD
 * */
struct date {
    int year;
    int month;
    int day;
};

/*
 * @brief �������ݽṹ��
 * */
struct  data_item {
    // ���
    int index;
    // ��Ʒ��
    char name[MAX_LIST_SIZE];
    // ׷�ٺ�
    int tracking_number;
    // ��Ʒ��С
    enum goods_size size;
    // ���ʱ��
    struct date date;
    // ���״̬
    enum storage_status status;
};

/*
 * @brief ˳������ݴ洢��
 * */
struct List{
    data_item *data;
    int length;
}link_list;
List L;

/*
 * @brief ���˵�_�ṹ������
 * */
menu_item menu_list[MAX_MENU_LIST] = {
        {0,"��ѯ��������", inquire_information},
        {1,"����������Ϣ", add_data},
        {2,"ɾ��������Ϣ", delete_data},
        {3,"�޸�������Ϣ", revise_data},
        {4,"����Ա����",admin_fuction},
        {5,"�˳�ϵͳ", user_exit_system}
};

/*
 * @brief ��ѯ��������_�ṹ������
 * */
menu_item inquire_information_list[MAX_INQUIRE_INFORMATION_LIST] = {
        {0,"�г�ȫ������", inquire_information_all},
        {1,"�����ڲ�ѯ", inquire_information_date},
        {2,"��׷�ٺŲ�ѯ", inquire_information_tracking_number},
        {3,"����Ʒ����ѯ", inquire_information_name},
};

/*
 * @brief ����Ա����_�ṹ������
 * */
menu_item admin_fuction_list[MAX_ADMIN_FUCTION_LIST] = {
        {0,"ɾ��ȫ������", admin_fuction_clearall},
        {1,"��ѯ��ǰ����Ŀ��",admin_fuction_length},
        {2,"�����ڷ����ѯ",damin_fuction_dateprin},
        {3,"�˳�",admin_fuction_exit}
};

/*
 * @brief   ��ʼ��˳���
 * @param L ˳����ַ
 *
 * @return  void
 * */
void init_link_list (List &L){
    // ����ռ�
    L.data = (data_item*) malloc(MAX_LIST_SIZE * sizeof (data_item));
    // �ó��ȵ���0
    L.length = 0;
}

/*
 * @brief ˳�������д��
 * @param L ˳����ַ
 *
 * @return bool д��״̬
 * */
bool input(List &L){
    clear_display();

    // ��������&��ʱ����
    int y,j,k;
    // �û�������
    char user_operator = 'n';

    // ��ʱ�������
    char    temp_name[MAX_LIST_SIZE];
    int     temp_tracking_number;
    enum    goods_size temp_size;
    struct  date temp_date{};
    enum    storage_status temp_status;

    // ��ӡ����
    dividing_line_title("¼����Ϣ",30);

    // �����ж�
    if(L.length > MAX_LIST_SIZE){
        message_error("ERROR","�������",30);
        pause_display();
        exit(EXIT_FAILURE);
    }

    // ������ѭ��
    while (true){
        clear_display();
        dividing_line_title("¼����Ϣ",30);

        // ��ȡ�����ţ���ӡ
        L.data[L.length].index = L.length + 1;
//      L.data[L.length].index = L.length ;
        cout << "[" << L.data[L.length].index << "]" << endl;

        // ��Ʒ��
        cout << "* ��Ʒ����";
//      cin >> L.data[L.length].name;
        cin >> temp_name;

        // ׷�ٺ�
        cout << "* ׷�ٺ�: ";
        cin >> temp_tracking_number;

        // ��Ʒ��С
        cout << "* ��Ʒ��С: " << endl;
        cout << "= [1] С" << endl << "= [2] ��" << endl << "= [3] ��" << endl;
        cin >> j;
        // �������ݺϷ����ж�
        if (j < 1 || j > 3){
            message_error("ERROR","���벻�Ϸ�!",30);
            pause_display();
            return false;
        }
        else{
            if (j == 1) { temp_size = small; }
            if (j == 2) { temp_size = medium; }
            if (j == 3) { temp_size = large; }
        }

        // ���ʱ��
        cout << "* ���ʱ��: " << endl;
        cout << "= �꣺";
        cin >> temp_date.year;
        if (temp_date.year < 1900 || temp_date.year > 2100){
            message_error("ERROR","������ݲ��Ϸ�!",30);
            pause_display();
            return false;
        }
        cout << "= ��: ";
        cin >> temp_date.month;
        if (temp_date.month < 1 || temp_date.month > 12){
            message_error("ERROR","�����·ݲ��Ϸ�!",30);
            pause_display();
            return false;
        }
        cout << "= ��: ";
        cin >> temp_date.day;
        if (temp_date.day < 1 || temp_date.day > 31){
            message_error("ERROR","�������ڲ��Ϸ�!",30);
            pause_display();
            return false;
        }

        // ���״̬
        cout << "* ���״̬: " <<  endl;
        cout << "= [1] δ���" << endl << "= [2] �����" << endl;
        cin >> k;
        if (k < 1 || k > 2){
            message_error("ERROR","���벻�Ϸ�!",30);
            pause_display();
            return false;
        }
        else{
            if (k == 1){ temp_status = out_stock; }
            if (k == 2) { temp_status = in_stock; }
        }

        // ����������ͬ������
        strcpy(L.data[L.length].name, temp_name);
        L.data[L.length].tracking_number = temp_tracking_number;
        L.data[L.length].size = temp_size;
        L.data[L.length].date = temp_date;
        L.data[L.length].status = temp_status;

        L.length += 1;

        // ������������ж�
        if(L.length > MAX_LIST_SIZE){
            message_error("ERROR","�������",30);
            pause_display();
            exit(EXIT_FAILURE);
        }

        // �˳��ж�
        cout << "�����Ƿ������(y/n): ";
        cin >> user_operator;
        if (user_operator == 'y' || user_operator == 'Y'){ break; }
        else{ continue; }
    }
    return true;
}

/*
 * @brief ȡ��ָ��λ�����ݲ���ӡ
 * @param L ����
 * @param i λ��
 *
 * @return void
 * */
void prints_specified_output (List &L, int i){
    cout << "[" << L.data[i].index << "]   ";

    cout << L.data[i].name << "   ";

    cout << L.data[i].tracking_number << "   ";

    if (L.data[i].size == small){ cout << "С   "; }
    if (L.data[i].size == medium){ cout << "��   "; }
    if (L.data[i].size == large){ cout << "��   "; }

    cout << L.data[i].date.year << "��" << L.data[i].date.month << "��" << L.data[i].date.day << "��" << "   ";

    if (L.data[i].status == out_stock){ cout << "δ���   "; }
    if (L.data[i].status == in_stock){ cout << "�����   "; }
}

/*
 * @brief ���ȫ������
 * @param L ����
 *
 * @return void
 * */
void print_list(List &L){
    int i;
    clear_display();
    dividing_line_title("�г�ȫ������",30);

    cout << "���   ��Ʒ��    ׷�ٺ�    ��С    ���ʱ��    ���״̬" << endl;

    for (i = 0; i < L.length; i++){
        prints_specified_output(L,i);
        cout << endl;
    }
}

/*
 * @brief ��������
 * @param L ˳���
 * @param i ����λ��
 * @param item ��������
 *
 * @return bool ��������״̬
 * */
bool insert (List &L, int i, data_item &item){
    int j;

    if (i < 1 || i < L.length + 1){
        return false;
    }
    i--;
    // i������Ԫ������ƶ�һλ
    for (j=L.length; j > i; j--){
        L.data[j] = L.data[j - 1];
    }

    strcpy(L.data[i].name, item.name);
    L.data[i].tracking_number = item.tracking_number;
    L.data[i].size = item.size;
    L.data[i].date = item.date;
    L.data[i].status = item.status;
    L.length += 1;

    return true;

}

/*
 * @brief ɾ��ָ��λ������
 * @param L ˳���
 * @param i ɾ��λ��
 *
 * @return bool ��������״̬
 * */
bool delete_list (List &L, int i){
    int k;
    if (i < 1 || i > L.length + 1){
        message_error("ERROR","���볬�ޣ�",30);
        pause_display();
        return false;
    }
    // ����������ǰ�ƶ�һλ
    for (k = 1; k < L.length; k++){
        L.data[k-1] = L.data[k];;
    }
    L.length--;
    return true;
}

/*
 * �����������˲˵�����ĺ���������ִ����غ���
 * */

/*
 * @brief �û��˳�ϵͳ
 * @param void
 *
 * @return void
 * */
void user_exit_system(){
    char user_input_operations;
    clear_display();

    message_error("�˳�ϵͳ", "��ȷ��Ҫ�˳�ϵͳ��(y/n)", 40);

    cin >> user_input_operations;
    if (user_input_operations == 'y' || user_input_operations == 'Y'){
        message_error("�˳�ϵͳ", "���˳�ϵͳ", 30);
        pause_display();
        // �û������˳�
        exit(EXIT_SUCCESS);
    }
    else{ return; }
}

/*
 * @brief ��ѯ��������
 * @brief ���ڽ����ѯ�Ķ����˵�
 * @param void
 *
 * @return void
 * */
void inquire_information(){
    if (L.length == 0){
        message_error("ERROR", "��������", 30);
        pause_display();
        return;
    } else{
        clear_display();
        print_menu(inquire_information_list, "��ѯ��������", 30, MAX_INQUIRE_INFORMATION_LIST);
        do_fuction(get_user_input(), inquire_information_list);
        pause_display();
    }
}

/*
 * @brief ��ѯ��������
 * @brief �г�����Ԫ��
 * @param void
 *
 * @return void
 * */
void inquire_information_all(){
    print_list(L);
    pause_display();
}

/*
 * @brief ��ѯ��������
 * @brief �����ڲ�ѯ
 * @param void
 *
 * @param void
 * */
void inquire_information_date(){
    clear_display();
    dividing_line_title("�����ڲ�ѯ",30);

    // ��ʱ����&��������
    int i,j = 0;
    struct date temp;

    cout << "* ������Ҫ��ѯ������: " << endl;

    cout << "= ��: ";
    cin >> temp.year;
    if (temp.year < 1900 || temp.year > 2100){
        message_error("ERROR","������ݲ��Ϸ�!",30);
        pause_display();
        return;
    }

    cout << "= ��: ";
    cin >> temp.month;
    if (temp.month < 1 || temp.month > 12){
        message_error("ERROR", "�����·ݲ��Ϸ�!",30);
        pause_display();
        return;
    }

    cout << "= ��: ";
    cin >> temp.day;
    if (temp.day < 1 || temp.day > 31){
        message_error("ERROR","�������ڲ��Ϸ�!",30);
        pause_display();
        return;
    }

    clear_display();
    dividing_line_title("��ѯ���",30);

    // �����Ƿ���ƥ�����ݲ����
    for (i = 0; i < MAX_LIST_SIZE; i++){
        if (L.data[i].date.year == temp.year && L.data[i].date.month == temp.month && L.data[i].date.day == temp.day){
            cout << "[" << L.data[i].index << "]   ";
            cout << L.data[i].name << "   ";
            cout << L.data[i].tracking_number << "   ";
            cout << L.data[i].size << "   ";
            cout << L.data[i].date.year << "��" << L.data[i].date.month << "��" << L.data[i].date.day << "��" << "   ";
            cout << L.data[i].status << "   ";
            cout << endl;
            j ++;
        }
        if (j == 0){
            message_error("ERROR", "δ��ѯ������", 30);
            break;
        }
    }
    pause_display();
}

/*
 * @brief ��ѯ��������
 * @brief ��׷�ٺŲ�ѯ
 * @param void
 *
 * @param void
 * */
void inquire_information_tracking_number(){
    clear_display();
    dividing_line_title("��׷�ٺŲ�ѯ",30);

    // ��ʱ����&��������
    int i,j,temp;

    cout << "* ������Ҫ��ѯ��׷�ٺ�: ";
    cin >> temp;

    clear_display();
    dividing_line_title("��ѯ���",30);

    for (i = 0; i < MAX_LIST_SIZE; i++){
        if (temp == L.data[i].tracking_number){
            prints_specified_output(L,i);
            cout << endl;
            j++;
        }
        if (j == 0){
            message_error("ERROR", "δ��ѯ������", 30);
            break;
        }
    }
}

/*
 * @brief ��ѯ��������
 * @brief ����Ʒ����ѯ
 * @param void
 *
 * @param void
 * */
void inquire_information_name(){
    clear_display();
    dividing_line_title("����Ʒ����ѯ",30);

    int i,j;
    char temp[MAX_LIST_SIZE];

    cout << "* ������Ҫ��ѯ����Ʒ��: ";
    cin >> temp;

    clear_display();
    dividing_line_title("��ѯ���",30);

    for (i = 0; i < MAX_LIST_SIZE; i++){
        if (strcmp(temp,L.data[i].name) == 0){
            prints_specified_output(L,i);
            cout << endl;
            j++;
        }
        if (j == 0){
            message_error("ERROR", "δ��ѯ������", 30);
            break;
        }
    }
}

/*
 * @brief ���������Ϣ
 * @param void
 *
 * @return void
 * */
void add_data(){
    // �ж������Ƿ���ӳɹ�
    if (input(L)){
        clear_display();
        cout << "������ӳɹ���" << endl;
    } else{
        message_error("ERROR","�������ʧ��",30);
        pause_display();
        return;
    }
    pause_display();
}

/*
 * @brief ɾ��������Ϣ
 * @param void
 *
 * @return void
 * */
void delete_data(){
    // �ж��Ƿ�����Ϣ����ɾ��
    if (L.length == 0) {
        message_error("ERROR", "��������", 30);
        pause_display();
        return;
    }

    clear_display();

    // ��ʱ����
    int user_input;
    char user_operations;

    dividing_line_title("ɾ������",30);
    cout << "* ָ��Ҫɾ����λ�ã�";
    cin >> user_input;

    cout << "* ȷ��ɾ����(y/n): ";
    cin >> user_operations;

    if (user_operations == 'y' || user_operations == 'Y'){
        if (delete_list(L,user_input)){
            cout << "ɾ���ɹ���" << endl;
            pause_display();
            return;
        }
        else{
            clear_display();
            message_error("ɾ��ʧ��", "δ֪����", 30);
            pause_display();
            return;
        }
    }
    else{ return; }
    pause_display();
}

/*
 * @brief �޸�������Ϣ
 * @param void
 *
 * @return void
 * */
void revise_data(){
    if (L.length == 0) {
        message_error("ERROR", "��������", 30);
        pause_display();
        return;
    }

    clear_display();
    dividing_line_title("�޸�������Ϣ",30);

    // ��ʱ����&��������
    int i,j,k;

    cout << "*" <<  " ��ǰ��" << L.length << "�����ݣ�ָ��Ҫ�޸ĵ�λ�ã�";
    cin >> i;
    i --;

    // ���뷶Χ�ж�
    if (i < 0 || i > L.length){
        clear_display();
        message_error("ERROR", "���볬��", 30);
        pause_display();
        return;
    }

    clear_display();
    dividing_line_title("�޸�������Ϣ",30);

    cout << "��ǰ��ѡ����Ϣ���£�" << endl;

    cout << "[" << L.data[i].index << "]" << endl;
    cout << "* ��Ʒ����" << L.data[i].name << endl;
    cout << "* ׷�ٺţ�" << L.data[i].tracking_number << endl;
    cout << "* ��Ʒ��С��";
    if (L.data[i].size == small){ cout << "С"; }
    if (L.data[i].size == medium){ cout << "��"; }
    if (L.data[i].size == large){ cout << "��"; }
    cout << endl;
    cout << "* ���ʱ�䣺" << L.data[i].date.year << "��" << L.data[i].date.month << "��" << L.data[i].date.day << "��" << endl;
    cout << "* ���״̬��";
    if (L.data[i].status == out_stock){ cout << "δ���"; }
    if (L.data[i].status == in_stock){ cout << "�����"; }
    cout << endl;

    cout << "������Ҫ�޸ĵ�Ԫ�أ�" << endl;
    cout << "* [1] ��Ʒ��: " << endl;
    cout << "* [2] ׷�ٺ�: " << endl;
    cout << "* [3] ��С��" << endl;
    cout << "* [4] ���ʱ�䣺" << endl;
    cout << "* [5] ���״̬��" << endl;
    cout << "�����������";

    cin >> j;
    if (j < 1 || j > 5){
        message_error("ERROR","���벻�Ϸ�",30);
        pause_display();
        return;
    }
    if (j == 1){
        cout << "* �����µ���Ʒ����";
        cin >> L.data[i].name;
    }
    if (j == 2){
        cout << "* �����µ�׷�ٺţ�";
        cin >> L.data[i].tracking_number;
    }
    if (j == 3){
        cout << "* �����µĴ�С��" << endl;
        cout << "= [1] С" << endl;
        cout << "= [2] ��" << endl;
        cout << "= [3] ��" << endl;
        cin >> k;
        if (k < 1 || k > 3){
            message_error("ERROR","�������",30);
            pause_display();
            return;
        }
        if (k == 1){ L.data[i].size = small; }
        if (k == 2){ L.data[i].size = medium; }
        if (k == 3){ L.data[i].size = large; }
    }
    if (j == 4){
        cout << "* �����µ����ʱ�䣺" << endl;
        cout << "= ��: ";
        cin >> L.data[i].date.year;
        cout << "= ��: ";
        cin >> L.data[i].date.month;
        cout << "= ��: ";
        cin >> L.data[i].date.day;
    }
    if (j == 5){
        cout << "* �����µ����״̬��" << endl;
        cout << "= [1] δ���" << endl;
        cout << "= [2] �����" << endl;
        cin >> k;
        if (k < 1 || k > 2){
            message_error("ERROR","�������",30);
            pause_display();
            return;
        }
        if (k == 1){ L.data[i].status = out_stock; }
        if (k == 2){ L.data[i].status = in_stock; }

    }

    clear_display();
    dividing_line_title("�޸�������Ϣ",30);
    cout << "�޸ĳɹ���" << endl;
    cout << "[" << L.data[i].index << "]" << endl;
    cout << "* ��Ʒ����" << L.data[i].name << endl;
    cout << "* ׷�ٺţ�" << L.data[i].tracking_number << endl;
    cout << "* ��Ʒ��С��";
    if (L.data[i].size == small){ cout << "С"; }
    if (L.data[i].size == medium){ cout << "��"; }
    if (L.data[i].size == large){ cout << "��"; }
    cout << endl;
    cout << "* ���ʱ�䣺" << L.data[i].date.year << "��" << L.data[i].date.month << "��" << L.data[i].date.day << "��" << endl;
    cout << "* ���״̬��";
    if (L.data[i].status == out_stock){ cout << "δ���"; }
    if (L.data[i].status == in_stock){ cout << "�����"; }
    cout << endl;

    pause_display();
}

/*
 * @brief ����Ա���ܺ���
 * @brief ������ת������Ա�����Ķ������ܲ˵�
 * @param void
 *
 * @param void
 * */
void admin_fuction(){
    clear_display();
    print_menu(admin_fuction_list, "����Ա����", 30, MAX_ADMIN_FUCTION_LIST);
    do_fuction(get_user_input(), admin_fuction_list);
    pause_display();
}

// fuction: ���ȫ������ TODO
/*
 * @brief ����Ա����
 * @brief ���ȫ������
 * @param void
 *
 * @return void
 * */
void admin_fuction_clearall(){

}

/*
 * @brief ����Ա����
 * @brief �����ڲ�ѯ
 * @param void
 *
 * @return void
 * */
void damin_fuction_dateprin(){
    clear_display();
    message_error("ERROR","���ܲ����ƣ��뷵��",30);
    pause_display();
    return;
}

/*
 * @brief ����Ա����
 * @brief �˳�����Ա�˵�ҳ��
 * @param void
 *
 * @return void
 * */
void admin_fuction_exit(){
    message_error("����", "�ѷ������˵������س�����", 30);
    return;
}

/*
 * @brief ����Ա����
 * @brief ��ѯ��Ŀ��
 * @param void
 *
 * @return void
 * */
void admin_fuction_length(){
    clear_display();
    dividing_line_title("��ѯ��ǰ����Ŀ��",30);
    cout << "* �����ó��ȣ�" << MAX_LIST_SIZE << endl;
    cout << "* ��ǰ���ȣ�" << L.length << endl;
    pause_display();
}

/*
 * @brief ���������ַ������ڵķָ�������
 * @param data  �����ַ���
 *
 * @return int �ָ�������
 * */
int calculate_character_length (string data){
    return data.length() / character_length * conversion_factor;
}

/*
 * @brief ��ӡ������ķָ���
 * @param title_name    ����
 * @param line          ���ⳤ��
 *
 * @return void
 * */
void dividing_line_title (string title_name,int line){
    /*
     * ��������: iterate_line
     * �洢ʵ����Ҫ��ӡ�ķָ������Ŀ
     */
    int iterate_line;

    // �жϱ�����ռ�ַ���Ŀ�Ƿ�������볤��
    if (line < title_name.length() / character_length * conversion_factor){
        // �������������������
        cout << title_name << endl;
    } else{
        /*
         * �������볤�� - ����������ռ���� / 2 = һ����Ҫ��ӡ���ַ���
         * */
        iterate_line = (line - title_name.length() / character_length * conversion_factor) / 2;
        for (int i = 0; i < iterate_line; i++){ cout << separator; }
        cout << title_name;
        for (int i = 0; i < iterate_line; i++){ cout << separator; }
    }

    /*
     * �ָ��ߴ�ӡ��Ϻ�
     * �����Ƿ��������
     * [�������Զ�����]
     *
     * ����˹�����ע�ʹ�code
     * */
    cout << endl;
}

/*
 * @brief ��ӡ��������ķָ���
 * @param line          ���ⳤ��
 *
 * @return void
 * */
void dividing_line (int line) {
    for (int i = 0; i < line; i++) {
        cout << separator;
    }
    cout << endl;
}

/*
 * @brief ��ӡͨ����Ϣ��ʾ��
 * @param title         ����
 * @param message       ��Ϣ
 * @param line          ��ʾ����
 *
 * @return void
 * */
void message_error(string title, string message, int line){
    clear_display();
    int i,j,k,temp; // ��������&��ʱ����

    // �Ϸָ���
    dividing_line_title(title,line);

    // ��������ı������ӡ�Ŀո���
    temp =  line / 2 - calculate_character_length(message) / 2;
    // ��ӡ��ֱ���з�
    cout << separator_vertical;
    for (i = 0; i < temp - 1; ++i) {
        cout << " ";
    }
    // �����Ϣ
    cout << message;
    // ��ӡ��ֱ���з�
    k = line - (calculate_character_length(message) + temp);
    for (j = 0; j < k - 1; j++) {
        cout << " ";
    }
    cout << separator_vertical;
    cout << endl;
    // �·ָ���
    dividing_line(line);
}

/*
 * @brief �����ʾ�˵�
 * @param data          �˵���
 * @param title         ����
 * @param separator_length  �ָ��߳���
 * @param max_list      �����Ŀ��
 *
 * @return void
 * */
void print_menu (struct menu_item data[], string title, int separator_length, int max_list){
    // ��������
    int i,j,k,r;

    // �˵�����ָ��߳���
    // int separator_length = 40;

    // �������ʱ�洢
    // �ӽṹ�������ж�ȡ���ݲ��Һϲ���һ��string��
    stringstream temp_menu_list_text;

    // ���м���ո����
    // �ò���ΪĬ�ϵĿո���
    temp_menu_list_text << "[" << data[0].index << "]---" << data[0].fuction_name;
    k =  separator_length / 2 - calculate_character_length(temp_menu_list_text.str()) / 2;
    temp_menu_list_text.str("");    // ������Ϻ���ձ����ڵ��ַ�

    // ��ӡ�ָ���-����
    dividing_line_title(title,separator_length);

    // ѭ����ӡ�˵�����
    for (i = 0; i < max_list; i++){
        // �ϲ���ǰindex�ĵ��������ַ�����temp_menu_list_text
        temp_menu_list_text << "[" << data[i].index << "]---" << data[i].fuction_name;

        // ��ӡ��ֱ���з�
        cout << separator_vertical;
        for (j = 0; j < k - 1; j++){ cout << " "; }

        // �������
        cout << temp_menu_list_text.str();

        // ��ӡ��ֱ���з�
        r = separator_length - (calculate_character_length(temp_menu_list_text.str()) + k);
        for (j = 0; j < r - 1; j++){ cout << " "; }
        cout << separator_vertical;

        // �������ձ����ڵ��ַ�������
        temp_menu_list_text.str("");
        cout << endl;   // ����
    }

    // ��ӡ�ָ���-��β
    dividing_line(separator_length);
}

/*
 * @brief ��ȡ�û�����
 * @param void
 *
 * @return uint8_t �û�����Ĳ�����
 * */
uint8_t get_user_input (){
    int temp;
    // ��ӡ�û�������ʾ
    cout << "# ��������������س�ִ�У�";
    cin >> temp;
    // �ж��û�����������Ƿ�Ϸ�
    if (temp >= 0){ return temp; }
    else{
        message_error("����","������������Ϸ���",30);
        pause_display();
        // ����������˳�
        exit(EXIT_FAILURE);
    }
}

/*
 * @brief ��ȡ�û�����&ִ�ж�Ӧ����
 * @param number_of_operations  �û�����Ĳ�����
 * @param data                  �˵���
 *
 * @return void
 * */
void do_fuction (uint8_t number_of_operations, struct menu_item data[]){
    // ��ȡ��Ҫ���еĺ���
    fuction_run_operation = data[number_of_operations].fuction;
    // ִ��
    (*fuction_run_operation)();
}

/*
 * @brief ˢ��&����˵�
 * @param void
 *
 * @return void
 * */
void menu_refresh (){
    clear_display();
    print_menu(menu_list,PROJECT_TITLE,30,MAX_MENU_LIST);
}

/*
 * @brief �����ʼ��
 * @param void
 *
 * @return void
 * */
void init (){
    init_link_list(L);
    L.data->index = 0;
}

/*
 * @brief ������
 *    1. �����ʼ��
 *    2. ��ѭ��
 *        2.1 ˢ�²˵�
 *        2.2 ��ȡ�û����벢ִ�ж�Ӧ����
 * @param void
 *
 * @return int
 * */
int main() {
    init();
    while (true){
        menu_refresh();
        do_fuction(get_user_input(),menu_list);
    }
    pause_display();
    return 0;
}
