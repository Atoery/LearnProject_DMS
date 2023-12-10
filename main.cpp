/*
 *  WarehouseLogisticsManagementSystem
 *  Designed by AToeryNitre
 *  Copyright (C) 2023 MAShiroSystem
 *
 *  @ file: main.cpp
 *  @ brief: 仓库物流管理系统
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
 * @brief 宏定义
 * */
// 最大存储数
#define MAX_LIST_SIZE                       30
// 最大主菜单表项数
#define MAX_MENU_LIST                       6
// 最大子菜单[查询物流数据]表项数
#define MAX_INQUIRE_INFORMATION_LIST        4
// 最大子菜单[管理员功能]表项数
#define MAX_ADMIN_FUCTION_LIST              4

/*
 * @brief 函数指针
 * */
void (*fuction_run_operation) ();

/*
 * @brief 元素类型
 * */
typedef int ElemType;

/*
 * @brief 常量
 *  1.制表&分隔符常量
 *  2.工程标题常量
 * */
// 分割符样式_水平
const char separator = '*';
// 分隔符样式_垂直
const char separator_vertical = '#';
// 转换系数
const uint8_t conversion_factor = 2;
// 一个汉字字符的长度
const uint8_t character_length  = 2;
// 主菜单标题&工程标题
const string PROJECT_TITLE = "仓库物流管理系统";

/*
 * @brief 函数声明
 * */
// * 主菜单功能_fuction
void user_exit_system();
void inquire_information();
void inquire_information_all();
void add_data();
void delete_data();
void revise_data();
void admin_fuction();

// * 子菜单_查询物流数据
void inquire_information_date();
void inquire_information_tracking_number();
void inquire_information_name();

// * 子菜单_管理员功能
void admin_fuction_clearall();
void admin_fuction_exit();
void admin_fuction_length();
void damin_fuction_dateprin();

// * 其它
void message_error(string title, string message, int line);
void dividing_line_title (string title_name,int line);
void dividing_line (int line);
void print_menu (struct menu_item date[], string title, int separator_length, int max_list);
void do_fuction (uint8_t number_of_operations, struct menu_item data[]);
uint8_t get_user_input ();

/*
 * @brief 物品大小
 *  1.small     小
 *  2.medium    中
 *  3.large     大
 * */
enum goods_size {
    small,
    medium,
    large
};

/*
 * @brief 仓库入库状态
 *  1.in_stock  入库
 *  2.out_stock 出库
 * */
enum storage_status {
    in_stock,
    out_stock
};

/*
 * fuction：预封装函数
 * */
// 清除屏幕显示函数
void clear_display (){
    // 将执行清屏的相关代码放置在这里
    system ("cls");
}
// 暂停屏幕显示函数
void pause_display (){
    // 将暂停屏幕显示相关代码放置在这里
    system ("pause");
}

/*
 * @brief 菜单结构体
 *  1.index：菜单索引号
 *  2.fuction_name：功能名
 *  3.fuction：功能函数
 * */
struct menu_item {
    int index;
    string fuction_name;
    void (*fuction) ();
};

/*
 * @brief 日期结构体
 *  存储结构：YYYY-MM-DD
 * */
struct date {
    int year;
    int month;
    int day;
};

/*
 * @brief 物流数据结构体
 * */
struct  data_item {
    // 序号
    int index;
    // 物品名
    char name[MAX_LIST_SIZE];
    // 追踪号
    int tracking_number;
    // 物品大小
    enum goods_size size;
    // 入库时间
    struct date date;
    // 入库状态
    enum storage_status status;
};

/*
 * @brief 顺序表数据存储域
 * */
struct List{
    data_item *data;
    int length;
}link_list;
List L;

/*
 * @brief 主菜单_结构体数组
 * */
menu_item menu_list[MAX_MENU_LIST] = {
        {0,"查询物流数据", inquire_information},
        {1,"创建物流信息", add_data},
        {2,"删除物流信息", delete_data},
        {3,"修改物流信息", revise_data},
        {4,"管理员功能",admin_fuction},
        {5,"退出系统", user_exit_system}
};

/*
 * @brief 查询物流数据_结构体数组
 * */
menu_item inquire_information_list[MAX_INQUIRE_INFORMATION_LIST] = {
        {0,"列出全部数据", inquire_information_all},
        {1,"按日期查询", inquire_information_date},
        {2,"按追踪号查询", inquire_information_tracking_number},
        {3,"按物品名查询", inquire_information_name},
};

/*
 * @brief 管理员功能_结构体数组
 * */
menu_item admin_fuction_list[MAX_ADMIN_FUCTION_LIST] = {
        {0,"删除全部数据", admin_fuction_clearall},
        {1,"查询当前总条目数",admin_fuction_length},
        {2,"按日期分类查询",damin_fuction_dateprin},
        {3,"退出",admin_fuction_exit}
};

/*
 * @brief   初始化顺序表
 * @param L 顺序表地址
 *
 * @return  void
 * */
void init_link_list (List &L){
    // 申请空间
    L.data = (data_item*) malloc(MAX_LIST_SIZE * sizeof (data_item));
    // 置长度等于0
    L.length = 0;
}

/*
 * @brief 顺序表数据写入
 * @param L 顺序表地址
 *
 * @return bool 写入状态
 * */
bool input(List &L){
    clear_display();

    // 迭代变量&临时变量
    int y,j,k;
    // 用户操作数
    char user_operator = 'n';

    // 临时存放数据
    char    temp_name[MAX_LIST_SIZE];
    int     temp_tracking_number;
    enum    goods_size temp_size;
    struct  date temp_date{};
    enum    storage_status temp_status;

    // 打印标题
    dividing_line_title("录入信息",30);

    // 错误判断
    if(L.length > MAX_LIST_SIZE){
        message_error("ERROR","库存已满",30);
        pause_display();
        exit(EXIT_FAILURE);
    }

    // 主输入循环
    while (true){
        clear_display();
        dividing_line_title("录入信息",30);

        // 获取索引号，打印
        L.data[L.length].index = L.length + 1;
//      L.data[L.length].index = L.length ;
        cout << "[" << L.data[L.length].index << "]" << endl;

        // 物品名
        cout << "* 物品名：";
//      cin >> L.data[L.length].name;
        cin >> temp_name;

        // 追踪号
        cout << "* 追踪号: ";
        cin >> temp_tracking_number;

        // 物品大小
        cout << "* 物品大小: " << endl;
        cout << "= [1] 小" << endl << "= [2] 中" << endl << "= [3] 大" << endl;
        cin >> j;
        // 输入数据合法性判断
        if (j < 1 || j > 3){
            message_error("ERROR","输入不合法!",30);
            pause_display();
            return false;
        }
        else{
            if (j == 1) { temp_size = small; }
            if (j == 2) { temp_size = medium; }
            if (j == 3) { temp_size = large; }
        }

        // 入库时间
        cout << "* 入库时间: " << endl;
        cout << "= 年：";
        cin >> temp_date.year;
        if (temp_date.year < 1900 || temp_date.year > 2100){
            message_error("ERROR","输入年份不合法!",30);
            pause_display();
            return false;
        }
        cout << "= 月: ";
        cin >> temp_date.month;
        if (temp_date.month < 1 || temp_date.month > 12){
            message_error("ERROR","输入月份不合法!",30);
            pause_display();
            return false;
        }
        cout << "= 日: ";
        cin >> temp_date.day;
        if (temp_date.day < 1 || temp_date.day > 31){
            message_error("ERROR","输入日期不合法!",30);
            pause_display();
            return false;
        }

        // 入库状态
        cout << "* 入库状态: " <<  endl;
        cout << "= [1] 未入库" << endl << "= [2] 已入库" << endl;
        cin >> k;
        if (k < 1 || k > 2){
            message_error("ERROR","输入不合法!",30);
            pause_display();
            return false;
        }
        else{
            if (k == 1){ temp_status = out_stock; }
            if (k == 2) { temp_status = in_stock; }
        }

        // 长度自增，同步数据
        strcpy(L.data[L.length].name, temp_name);
        L.data[L.length].tracking_number = temp_tracking_number;
        L.data[L.length].size = temp_size;
        L.data[L.length].date = temp_date;
        L.data[L.length].status = temp_status;

        L.length += 1;

        // 超出库存容量判断
        if(L.length > MAX_LIST_SIZE){
            message_error("ERROR","库存已满",30);
            pause_display();
            exit(EXIT_FAILURE);
        }

        // 退出判断
        cout << "输入是否结束？(y/n): ";
        cin >> user_operator;
        if (user_operator == 'y' || user_operator == 'Y'){ break; }
        else{ continue; }
    }
    return true;
}

/*
 * @brief 取出指定位置数据并打印
 * @param L 链表
 * @param i 位置
 *
 * @return void
 * */
void prints_specified_output (List &L, int i){
    cout << "[" << L.data[i].index << "]   ";

    cout << L.data[i].name << "   ";

    cout << L.data[i].tracking_number << "   ";

    if (L.data[i].size == small){ cout << "小   "; }
    if (L.data[i].size == medium){ cout << "中   "; }
    if (L.data[i].size == large){ cout << "大   "; }

    cout << L.data[i].date.year << "年" << L.data[i].date.month << "月" << L.data[i].date.day << "日" << "   ";

    if (L.data[i].status == out_stock){ cout << "未入库   "; }
    if (L.data[i].status == in_stock){ cout << "已入库   "; }
}

/*
 * @brief 输出全部数据
 * @param L 链表
 *
 * @return void
 * */
void print_list(List &L){
    int i;
    clear_display();
    dividing_line_title("列出全部数据",30);

    cout << "序号   物品名    追踪号    大小    入库时间    入库状态" << endl;

    for (i = 0; i < L.length; i++){
        prints_specified_output(L,i);
        cout << endl;
    }
}

/*
 * @brief 插入数据
 * @param L 顺序表
 * @param i 插入位置
 * @param item 插入数据
 *
 * @return bool 函数运行状态
 * */
bool insert (List &L, int i, data_item &item){
    int j;

    if (i < 1 || i < L.length + 1){
        return false;
    }
    i--;
    // i后所有元素向后移动一位
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
 * @brief 删除指定位置数据
 * @param L 顺序表
 * @param i 删除位置
 *
 * @return bool 函数运行状态
 * */
bool delete_list (List &L, int i){
    int k;
    if (i < 1 || i > L.length + 1){
        message_error("ERROR","输入超限！",30);
        pause_display();
        return false;
    }
    // 所有数据向前移动一位
    for (k = 1; k < L.length; k++){
        L.data[k-1] = L.data[k];;
    }
    L.length--;
    return true;
}

/*
 * 这里是链接了菜单表项的函数，用于执行相关函数
 * */

/*
 * @brief 用户退出系统
 * @param void
 *
 * @return void
 * */
void user_exit_system(){
    char user_input_operations;
    clear_display();

    message_error("退出系统", "您确定要退出系统吗？(y/n)", 40);

    cin >> user_input_operations;
    if (user_input_operations == 'y' || user_input_operations == 'Y'){
        message_error("退出系统", "已退出系统", 30);
        pause_display();
        // 用户正常退出
        exit(EXIT_SUCCESS);
    }
    else{ return; }
}

/*
 * @brief 查询物流数据
 * @brief 用于进入查询的二级菜单
 * @param void
 *
 * @return void
 * */
void inquire_information(){
    if (L.length == 0){
        message_error("ERROR", "暂无数据", 30);
        pause_display();
        return;
    } else{
        clear_display();
        print_menu(inquire_information_list, "查询物流数据", 30, MAX_INQUIRE_INFORMATION_LIST);
        do_fuction(get_user_input(), inquire_information_list);
        pause_display();
    }
}

/*
 * @brief 查询物流数据
 * @brief 列出所有元素
 * @param void
 *
 * @return void
 * */
void inquire_information_all(){
    print_list(L);
    pause_display();
}

/*
 * @brief 查询物流数据
 * @brief 按日期查询
 * @param void
 *
 * @param void
 * */
void inquire_information_date(){
    clear_display();
    dividing_line_title("按日期查询",30);

    // 临时变量&迭代变量
    int i,j = 0;
    struct date temp;

    cout << "* 输入需要查询的日期: " << endl;

    cout << "= 年: ";
    cin >> temp.year;
    if (temp.year < 1900 || temp.year > 2100){
        message_error("ERROR","输入年份不合法!",30);
        pause_display();
        return;
    }

    cout << "= 月: ";
    cin >> temp.month;
    if (temp.month < 1 || temp.month > 12){
        message_error("ERROR", "输入月份不合法!",30);
        pause_display();
        return;
    }

    cout << "= 日: ";
    cin >> temp.day;
    if (temp.day < 1 || temp.day > 31){
        message_error("ERROR","输入日期不合法!",30);
        pause_display();
        return;
    }

    clear_display();
    dividing_line_title("查询结果",30);

    // 检索是否有匹配数据并输出
    for (i = 0; i < MAX_LIST_SIZE; i++){
        if (L.data[i].date.year == temp.year && L.data[i].date.month == temp.month && L.data[i].date.day == temp.day){
            cout << "[" << L.data[i].index << "]   ";
            cout << L.data[i].name << "   ";
            cout << L.data[i].tracking_number << "   ";
            cout << L.data[i].size << "   ";
            cout << L.data[i].date.year << "年" << L.data[i].date.month << "月" << L.data[i].date.day << "日" << "   ";
            cout << L.data[i].status << "   ";
            cout << endl;
            j ++;
        }
        if (j == 0){
            message_error("ERROR", "未查询到数据", 30);
            break;
        }
    }
    pause_display();
}

/*
 * @brief 查询物流数据
 * @brief 按追踪号查询
 * @param void
 *
 * @param void
 * */
void inquire_information_tracking_number(){
    clear_display();
    dividing_line_title("按追踪号查询",30);

    // 临时变量&迭代变量
    int i,j,temp;

    cout << "* 输入需要查询的追踪号: ";
    cin >> temp;

    clear_display();
    dividing_line_title("查询结果",30);

    for (i = 0; i < MAX_LIST_SIZE; i++){
        if (temp == L.data[i].tracking_number){
            prints_specified_output(L,i);
            cout << endl;
            j++;
        }
        if (j == 0){
            message_error("ERROR", "未查询到数据", 30);
            break;
        }
    }
}

/*
 * @brief 查询物流数据
 * @brief 按物品名查询
 * @param void
 *
 * @param void
 * */
void inquire_information_name(){
    clear_display();
    dividing_line_title("按物品名查询",30);

    int i,j;
    char temp[MAX_LIST_SIZE];

    cout << "* 输入需要查询的物品名: ";
    cin >> temp;

    clear_display();
    dividing_line_title("查询结果",30);

    for (i = 0; i < MAX_LIST_SIZE; i++){
        if (strcmp(temp,L.data[i].name) == 0){
            prints_specified_output(L,i);
            cout << endl;
            j++;
        }
        if (j == 0){
            message_error("ERROR", "未查询到数据", 30);
            break;
        }
    }
}

/*
 * @brief 添加物流信息
 * @param void
 *
 * @return void
 * */
void add_data(){
    // 判断数据是否添加成功
    if (input(L)){
        clear_display();
        cout << "数据添加成功！" << endl;
    } else{
        message_error("ERROR","数据添加失败",30);
        pause_display();
        return;
    }
    pause_display();
}

/*
 * @brief 删除物流信息
 * @param void
 *
 * @return void
 * */
void delete_data(){
    // 判断是否有信息可以删除
    if (L.length == 0) {
        message_error("ERROR", "暂无数据", 30);
        pause_display();
        return;
    }

    clear_display();

    // 临时变量
    int user_input;
    char user_operations;

    dividing_line_title("删除数据",30);
    cout << "* 指定要删除的位置：";
    cin >> user_input;

    cout << "* 确定删除？(y/n): ";
    cin >> user_operations;

    if (user_operations == 'y' || user_operations == 'Y'){
        if (delete_list(L,user_input)){
            cout << "删除成功！" << endl;
            pause_display();
            return;
        }
        else{
            clear_display();
            message_error("删除失败", "未知错误", 30);
            pause_display();
            return;
        }
    }
    else{ return; }
    pause_display();
}

/*
 * @brief 修改物流信息
 * @param void
 *
 * @return void
 * */
void revise_data(){
    if (L.length == 0) {
        message_error("ERROR", "暂无数据", 30);
        pause_display();
        return;
    }

    clear_display();
    dividing_line_title("修改物流信息",30);

    // 临时变量&迭代变量
    int i,j,k;

    cout << "*" <<  " 当前有" << L.length << "个数据，指定要修改的位置：";
    cin >> i;
    i --;

    // 输入范围判断
    if (i < 0 || i > L.length){
        clear_display();
        message_error("ERROR", "输入超限", 30);
        pause_display();
        return;
    }

    clear_display();
    dividing_line_title("修改物流信息",30);

    cout << "当前所选定信息如下：" << endl;

    cout << "[" << L.data[i].index << "]" << endl;
    cout << "* 物品名：" << L.data[i].name << endl;
    cout << "* 追踪号：" << L.data[i].tracking_number << endl;
    cout << "* 物品大小：";
    if (L.data[i].size == small){ cout << "小"; }
    if (L.data[i].size == medium){ cout << "中"; }
    if (L.data[i].size == large){ cout << "大"; }
    cout << endl;
    cout << "* 入库时间：" << L.data[i].date.year << "年" << L.data[i].date.month << "月" << L.data[i].date.day << "日" << endl;
    cout << "* 入库状态：";
    if (L.data[i].status == out_stock){ cout << "未入库"; }
    if (L.data[i].status == in_stock){ cout << "已入库"; }
    cout << endl;

    cout << "输入需要修改的元素：" << endl;
    cout << "* [1] 物品名: " << endl;
    cout << "* [2] 追踪号: " << endl;
    cout << "* [3] 大小：" << endl;
    cout << "* [4] 入库时间：" << endl;
    cout << "* [5] 入库状态：" << endl;
    cout << "输入操作数：";

    cin >> j;
    if (j < 1 || j > 5){
        message_error("ERROR","输入不合法",30);
        pause_display();
        return;
    }
    if (j == 1){
        cout << "* 输入新的物品名：";
        cin >> L.data[i].name;
    }
    if (j == 2){
        cout << "* 输入新的追踪号：";
        cin >> L.data[i].tracking_number;
    }
    if (j == 3){
        cout << "* 输入新的大小：" << endl;
        cout << "= [1] 小" << endl;
        cout << "= [2] 中" << endl;
        cout << "= [3] 大" << endl;
        cin >> k;
        if (k < 1 || k > 3){
            message_error("ERROR","输入错误",30);
            pause_display();
            return;
        }
        if (k == 1){ L.data[i].size = small; }
        if (k == 2){ L.data[i].size = medium; }
        if (k == 3){ L.data[i].size = large; }
    }
    if (j == 4){
        cout << "* 输入新的入库时间：" << endl;
        cout << "= 年: ";
        cin >> L.data[i].date.year;
        cout << "= 月: ";
        cin >> L.data[i].date.month;
        cout << "= 日: ";
        cin >> L.data[i].date.day;
    }
    if (j == 5){
        cout << "* 输入新的入库状态：" << endl;
        cout << "= [1] 未入库" << endl;
        cout << "= [2] 已入库" << endl;
        cin >> k;
        if (k < 1 || k > 2){
            message_error("ERROR","输入错误",30);
            pause_display();
            return;
        }
        if (k == 1){ L.data[i].status = out_stock; }
        if (k == 2){ L.data[i].status = in_stock; }

    }

    clear_display();
    dividing_line_title("修改物流信息",30);
    cout << "修改成功！" << endl;
    cout << "[" << L.data[i].index << "]" << endl;
    cout << "* 物品名：" << L.data[i].name << endl;
    cout << "* 追踪号：" << L.data[i].tracking_number << endl;
    cout << "* 物品大小：";
    if (L.data[i].size == small){ cout << "小"; }
    if (L.data[i].size == medium){ cout << "中"; }
    if (L.data[i].size == large){ cout << "大"; }
    cout << endl;
    cout << "* 入库时间：" << L.data[i].date.year << "年" << L.data[i].date.month << "月" << L.data[i].date.day << "日" << endl;
    cout << "* 入库状态：";
    if (L.data[i].status == out_stock){ cout << "未入库"; }
    if (L.data[i].status == in_stock){ cout << "已入库"; }
    cout << endl;

    pause_display();
}

/*
 * @brief 管理员功能函数
 * @brief 用于跳转至管理员函数的二级功能菜单
 * @param void
 *
 * @param void
 * */
void admin_fuction(){
    clear_display();
    print_menu(admin_fuction_list, "管理员功能", 30, MAX_ADMIN_FUCTION_LIST);
    do_fuction(get_user_input(), admin_fuction_list);
    pause_display();
}

// fuction: 清除全部数据 TODO
/*
 * @brief 管理员功能
 * @brief 清除全部数据
 * @param void
 *
 * @return void
 * */
void admin_fuction_clearall(){

}

/*
 * @brief 管理员功能
 * @brief 按日期查询
 * @param void
 *
 * @return void
 * */
void damin_fuction_dateprin(){
    clear_display();
    message_error("ERROR","功能不完善！请返回",30);
    pause_display();
    return;
}

/*
 * @brief 管理员功能
 * @brief 退出管理员菜单页面
 * @param void
 *
 * @return void
 * */
void admin_fuction_exit(){
    message_error("返回", "已返回主菜单，按回车继续", 30);
    return;
}

/*
 * @brief 管理员功能
 * @brief 查询条目数
 * @param void
 *
 * @return void
 * */
void admin_fuction_length(){
    clear_display();
    dividing_line_title("查询当前总条目数",30);
    cout << "* 最大可用长度：" << MAX_LIST_SIZE << endl;
    cout << "* 当前长度：" << L.length << endl;
    pause_display();
}

/*
 * @brief 计算输入字符串等于的分隔符长度
 * @param data  输入字符串
 *
 * @return int 分隔符长度
 * */
int calculate_character_length (string data){
    return data.length() / character_length * conversion_factor;
}

/*
 * @brief 打印带标题的分割线
 * @param title_name    标题
 * @param line          标题长度
 *
 * @return void
 * */
void dividing_line_title (string title_name,int line){
    /*
     * 迭代变量: iterate_line
     * 存储实际需要打印的分割符号数目
     */
    int iterate_line;

    // 判断标题所占字符数目是否大于输入长度
    if (line < title_name.length() / character_length * conversion_factor){
        // 如果超长，则仅输出标题
        cout << title_name << endl;
    } else{
        /*
         * 函数传入长度 - 标题文字所占长度 / 2 = 一侧需要打印的字符数
         * */
        iterate_line = (line - title_name.length() / character_length * conversion_factor) / 2;
        for (int i = 0; i < iterate_line; i++){ cout << separator; }
        cout << title_name;
        for (int i = 0; i < iterate_line; i++){ cout << separator; }
    }

    /*
     * 分割线打印完毕后
     * 控制是否输出换行
     * [输出完毕自动换行]
     *
     * 无需此功能请注释此code
     * */
    cout << endl;
}

/*
 * @brief 打印不带标题的分割线
 * @param line          标题长度
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
 * @brief 打印通用信息提示框
 * @param title         标题
 * @param message       信息
 * @param line          提示框宽度
 *
 * @return void
 * */
void message_error(string title, string message, int line){
    clear_display();
    int i,j,k,temp; // 迭代变量&临时变量

    // 上分割线
    dividing_line_title(title,line);

    // 计算居中文本所需打印的空格数
    temp =  line / 2 - calculate_character_length(message) / 2;
    // 打印垂直换行符
    cout << separator_vertical;
    for (i = 0; i < temp - 1; ++i) {
        cout << " ";
    }
    // 输出消息
    cout << message;
    // 打印垂直换行符
    k = line - (calculate_character_length(message) + temp);
    for (j = 0; j < k - 1; j++) {
        cout << " ";
    }
    cout << separator_vertical;
    cout << endl;
    // 下分割线
    dividing_line(line);
}

/*
 * @brief 输出显示菜单
 * @param data          菜单项
 * @param title         标题
 * @param separator_length  分割线长度
 * @param max_list      最大条目数
 *
 * @return void
 * */
void print_menu (struct menu_item data[], string title, int separator_length, int max_list){
    // 迭代变量
    int i,j,k,r;

    // 菜单界面分割线长度
    // int separator_length = 40;

    // 行输出临时存储
    // 从结构体数组中读取数据并且合并至一个string中
    stringstream temp_menu_list_text;

    // 先行计算空格参量
    // 该参数为默认的空格数
    temp_menu_list_text << "[" << data[0].index << "]---" << data[0].fuction_name;
    k =  separator_length / 2 - calculate_character_length(temp_menu_list_text.str()) / 2;
    temp_menu_list_text.str("");    // 计算完毕后清空变量内的字符

    // 打印分割线-标题
    dividing_line_title(title,separator_length);

    // 循环打印菜单表项
    for (i = 0; i < max_list; i++){
        // 合并当前index的的数据至字符串：temp_menu_list_text
        temp_menu_list_text << "[" << data[i].index << "]---" << data[i].fuction_name;

        // 打印垂直换行符
        cout << separator_vertical;
        for (j = 0; j < k - 1; j++){ cout << " "; }

        // 输出数据
        cout << temp_menu_list_text.str();

        // 打印垂直换行符
        r = separator_length - (calculate_character_length(temp_menu_list_text.str()) + k);
        for (j = 0; j < r - 1; j++){ cout << " "; }
        cout << separator_vertical;

        // 输出后，清空变量内的字符串数据
        temp_menu_list_text.str("");
        cout << endl;   // 换行
    }

    // 打印分割线-结尾
    dividing_line(separator_length);
}

/*
 * @brief 获取用户输入
 * @param void
 *
 * @return uint8_t 用户输入的操作数
 * */
uint8_t get_user_input (){
    int temp;
    // 打印用户输入提示
    cout << "# 输入操作数，按回车执行：";
    cin >> temp;
    // 判断用户输入操作数是否合法
    if (temp >= 0){ return temp; }
    else{
        message_error("警告","输入操作数不合法！",30);
        pause_display();
        // 程序非正常退出
        exit(EXIT_FAILURE);
    }
}

/*
 * @brief 获取用户输入&执行对应功能
 * @param number_of_operations  用户输入的操作数
 * @param data                  菜单项
 *
 * @return void
 * */
void do_fuction (uint8_t number_of_operations, struct menu_item data[]){
    // 读取需要运行的函数
    fuction_run_operation = data[number_of_operations].fuction;
    // 执行
    (*fuction_run_operation)();
}

/*
 * @brief 刷新&输出菜单
 * @param void
 *
 * @return void
 * */
void menu_refresh (){
    clear_display();
    print_menu(menu_list,PROJECT_TITLE,30,MAX_MENU_LIST);
}

/*
 * @brief 程序初始化
 * @param void
 *
 * @return void
 * */
void init (){
    init_link_list(L);
    L.data->index = 0;
}

/*
 * @brief 主函数
 *    1. 程序初始化
 *    2. 主循环
 *        2.1 刷新菜单
 *        2.2 获取用户输入并执行对应功能
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
