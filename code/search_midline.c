#include "zf_device_mt9v03x.h"
#include "image_processing.h"
#include "search_midline.h"

//struct LEFT_EDGE  L_edge[120];     //左边界结构体
//struct RIGHT_EDGE R_edge[120];    //右边界结构体

uint8 L_edge_count = 0, R_edge_count = 0;                       //左右边点的个数
uint8 dire_left, dire_right;                                    //记录上一个点的相对位置
uint8 L_search_amount = 140, R_search_amount = 140;             //左右边界搜点时最多允许的点
uint8 Boundary_search_end = MT9V03X_H;
uint8 L_start_x,L_start_y,left_lose;
uint8 R_start_x,R_start_y,right_lose;
uint8 left_findflag, right_findflag;

// 函数简介     拐点检测
uint8 L_corner_flag = 0;//左拐点存在标志
uint8 L_corner_row[5];//左拐点所在行
uint8 L_corner_col[5];//左拐点所在列
int L_corner_angle = 0;//左拐点角度
uint8 R_corner_flag = 0;//右拐点存在标志
uint8 R_corner_row[5];//右拐点所在行
uint8 R_corner_col[5];//右拐点所在列
int R_corner_angle = 0;//右拐点角度
uint8 enable_L_corner = 1, enable_R_corner = 1;
uint8 Mid_line[MT9V03X_H];
uint8 Left_edge[MT9V03X_H];
uint8 Right_edge[MT9V03X_H];

float error,last_error, YERR;
uint8 far,mid,near;

struct LEFT_EDGE
{
    uint8 row;  //行坐标
    uint8 col;  //列坐标
    uint8 flag; //存在边界的标志
}L_edge[120];
struct RIGHT_EDGE
{
    uint8 row;  //行坐标
    uint8 col;  //列坐标
    uint8 flag; //存在边界的标志
}R_edge[120];
//---------------------------------------------------------------
// 函数简介     八邻域边界初始化
// 参数说明     void
// 返回参数     void
// 使用示例     clear_find_point();
//---------------------------------------------------------------
void clear_find_point(void)
{
    for (int i = 0;i < 140;i++)
    {
        L_edge[i].row = 0;
        L_edge[i].col = 0;
        L_edge[i].flag = 0;
    }
    for (int i = 0;i < 140;i++)
    {
        R_edge[i].row = 0;
        R_edge[i].col = 0;
        R_edge[i].flag = 0;
    }
}

//---------------------------------------------------------------
// 函数简介     八邻域找边界
// 参数说明     void
// 返回参数     void
// 使用示例     search_neighborhood();
//---------------------------------------------------------------

void search_neighborhood(void)
{
    L_edge_count = 0;//左边点个数清0
    R_edge_count = 0;//右边点个数清0

    if (left_findflag)//如果左边界点存在并找到,则开始爬线
    {
        //变量声明
        L_edge[0].row = L_start_y;
        L_edge[0].col = L_start_x;
        L_edge[0].flag = 1;
        uint8 curr_row = L_start_y;//初始化行坐标
        uint8 curr_col = L_start_x;//初始化列坐标
        dire_left = 0; //初始化上个边界点的来向
        //开始搜线，最多取140个点，不会往下搜，共7个方位
        for (int i = 1;i < L_search_amount; i++)    //最多搜索140个点
        {
            //越界退出 行越界和列越界（向上向下向左向右）
            if (curr_row + 1 < Boundary_search_end || curr_row > MT9V03X_H - 1)  break;
            //搜线过程
            if (dire_left != 2 && Image_use[curr_row - 1][curr_col - 1] == BLACK && Image_use[curr_row - 1][curr_col] == WHITE)   //左上黑，2，右边白
            {
                curr_row = curr_row - 1;
                curr_col = curr_col - 1;
                L_edge_count = L_edge_count + 1;
                dire_left = 7;
                L_edge[i].row = curr_row;
                L_edge[i].col = curr_col;
                L_edge[i].flag = 1;
            }
            else if (dire_left != 3 && Image_use[curr_row - 1][curr_col + 1] == BLACK && Image_use[curr_row][curr_col + 1] == WHITE)    //右上黑，3，下边白
            {
                curr_row = curr_row - 1;
                curr_col = curr_col + 1;
                L_edge_count = L_edge_count + 1;
                dire_left = 6;
                L_edge[i].row = curr_row;
                L_edge[i].col = curr_col;
                L_edge[i].flag = 1;
            }
            else if (Image_use[curr_row - 1][curr_col] == BLACK && Image_use[curr_row - 1][curr_col + 1] == WHITE)                  //正上黑，1，右白
            {
                curr_row = curr_row - 1;
                L_edge_count = L_edge_count + 1;
                dire_left = 0;
                L_edge[i].row = curr_row;
                L_edge[i].col = curr_col;
                L_edge[i].flag = 1;
            }
            else if (dire_left != 5 && Image_use[curr_row][curr_col - 1] == BLACK && Image_use[curr_row - 1][curr_col - 1] == WHITE)     //正左黑，5，上白
            {
                curr_col = curr_col - 1;
                L_edge_count = L_edge_count + 1;
                dire_left = 4;
                L_edge[i].row = curr_row;
                L_edge[i].col = curr_col;
                L_edge[i].flag = 1;
            }
            else if (dire_left != 4 && Image_use[curr_row][curr_col + 1] == BLACK && Image_use[curr_row + 1][curr_col + 1] == WHITE)  //正右黑，4，下白
            {
                curr_col = curr_col + 1;
                L_edge_count = L_edge_count + 1;
                dire_left = 5;
                L_edge[i].row = curr_row;
                L_edge[i].col = curr_col;
                L_edge[i].flag = 1;
            }
            else if (dire_left != 6 && Image_use[curr_row + 1][curr_col - 1] == BLACK && Image_use[curr_row][curr_col - 1] == WHITE)    //左下黑，6，上白
            {
                curr_row = curr_row + 1;
                curr_col = curr_col - 1;
                L_edge_count = L_edge_count + 1;
                dire_left = 3;
                L_edge[i].row = curr_row;
                L_edge[i].col = curr_col;
                L_edge[i].flag = 1;
            }
            else if (dire_left != 7 && Image_use[curr_row + 1][curr_col + 1] == BLACK && Image_use[curr_row + 1][curr_col] == WHITE)    //右下黑，7，左白
            {
                curr_row = curr_row + 1;
                curr_col = curr_col + 1;
                L_edge_count = L_edge_count + 1;
                dire_left = 2;
                L_edge[i].row = curr_row;
                L_edge[i].col = curr_col;
                L_edge[i].flag = 1;
            }
            else
                break;
        }
    }

    if (right_findflag)//如果右边界存在并搜到
    {
        R_edge[0].row = R_start_y;
        R_edge[0].col = R_start_x;
        R_edge[0].flag = 1;
        uint8 curr_row = R_start_y;
        uint8 curr_col = R_start_x;
        dire_right = 0;
        for (int i = 1;i < R_search_amount;i++)
        {
            //越界退出 行越界和列越界（向上向下向左向右）
            if (curr_row < Boundary_search_end || curr_row>IMAGE_H - 1 || curr_row + 1 < Boundary_search_end)  break;
            //爬线过程
            if (curr_col < IMAGE_W && dire_right != 3 && Image_use[curr_row - 1][curr_col + 1] == BLACK && Image_use[curr_row - 1][curr_col] == WHITE)    //右上黑，3，左白
            {
                curr_row = curr_row - 1;
                curr_col = curr_col + 1;
                R_edge_count = R_edge_count + 1;
                dire_right = 6;
                R_edge[i].row = curr_row;
                R_edge[i].col = curr_col;
                R_edge[i].flag = 1;
            }
            else if (dire_right != 2 && Image_use[curr_row - 1][curr_col - 1] == BLACK && Image_use[curr_row][curr_col - 1] == WHITE) //左上黑，2，下白
            {
                curr_row = curr_row - 1;
                curr_col = curr_col - 1;
                R_edge_count = R_edge_count + 1;
                dire_right = 7;
                R_edge[i].row = curr_row;
                R_edge[i].col = curr_col;
                R_edge[i].flag = 1;
            }
            else if (Image_use[curr_row - 1][curr_col] == BLACK && Image_use[curr_row - 1][curr_col - 1] == WHITE)                  //正上黑，1，左白
            {
                curr_row = curr_row - 1;
                R_edge_count = R_edge_count + 1;
                dire_right = 0;
                R_edge[i].row = curr_row;
                R_edge[i].col = curr_col;
                R_edge[i].flag = 1;
            }
            else if (dire_right != 4 && Image_use[curr_row][curr_col + 1] == BLACK && Image_use[curr_row - 1][curr_col + 1] == WHITE)   //正右黑，4，上白
            {
                curr_col = curr_col + 1;
                R_edge_count = R_edge_count + 1;
                dire_right = 5;
                R_edge[i].row = curr_row;
                R_edge[i].col = curr_col;
                R_edge[i].flag = 1;
            }
            else if (dire_right != 5 && Image_use[curr_row][curr_col - 1] == BLACK && Image_use[curr_row + 1][curr_col - 1] == WHITE)   //正左黑，5，下白
            {
                curr_col = curr_col - 1;
                R_edge_count = R_edge_count + 1;
                dire_right = 4;
                R_edge[i].row = curr_row;
                R_edge[i].col = curr_col;
                R_edge[i].flag = 1;
            }
            else if (dire_right != 6 && Image_use[curr_row + 1][curr_col - 1] == BLACK && Image_use[curr_row + 1][curr_col] == WHITE)   //左下黑，6，右白
            {
                curr_row = curr_row + 1;
                curr_col = curr_col - 1;
                R_edge_count = R_edge_count + 1;
                dire_right = 3;
                R_edge[i].row = curr_row;
                R_edge[i].col = curr_col;
                R_edge[i].flag = 1;
            }
            else if (dire_right != 7 && Image_use[curr_row + 1][curr_col + 1] == BLACK && Image_use[curr_row][curr_col + 1] == WHITE)   //右下黑，7，上白
            {
                curr_row = curr_row + 1;
                curr_col = curr_col + 1;
                R_edge_count = R_edge_count + 1;
                dire_right = 2;
                R_edge[i].row = curr_row;
                R_edge[i].col = curr_col;
                R_edge[i].flag = 1;
            }
            else
                break;
        }
    }
}


//---------------------------------------------------------------
// 函数简介     差比和
// 参数说明     void
// 返回参数     void
// 使用示例
// 注意事项     约放大128倍
//---------------------------------------------------------------
int16 calc_diff(int16 x, int16 y)
{
    return (((x - y) << 7) / (x + y));
}

//---------------------------------------------------------------
// 函数简介      限幅
// 参数说明      x               被限幅的数据
// 参数说明      y               限幅范围(数据会被限制在-y至+y之间)
// 返回参数      float           限幅之后的数据
// 使用示例      float dat = limit(500,300);//数据被限制在-300至+300之间  因此返回的结果是300
//---------------------------------------------------------------
float limit(float x, int32 y)
{
    if (x > y)             return (float)y;
    else if (x < -y)       return (float)(-y);
    else                   return x;
}

//---------------------------------------------------------------
// 函数简介     拐点检测
// 参数说明     void
// 返回参数     void
// 使用示例     get_turning_point();
//---------------------------------------------------------------

void get_turning_point(void)
{

    L_corner_flag = 0;// 初始化变量
    L_corner_angle = 0;
    if (enable_L_corner) //如果使能搜索左拐点
    {
        uint8 k = 0;
        if (L_edge_count > 9 && L_start_y >= IMAGE_H / 2 && left_lose <= IMAGE_H / 2)
        {
            for (int i = 0; i < L_edge_count - 9;i++)
            {
                if (L_edge[i + 8].row > 5)
                {
                    if ((L_edge[i].col - L_edge[i + 4].col) * (L_edge[i + 8].col - L_edge[i + 4].col) +
                        (L_edge[i].row - L_edge[i + 4].row) * (L_edge[i + 8].row - L_edge[i + 4].row) >= 0) //初步确认为锐角或者直角 向量法
                    {
                        L_corner_angle = Get_angle(L_edge[i].col, L_edge[i].row, L_edge[i + 4].col, L_edge[i + 4].row, L_edge[i + 8].col, L_edge[i + 8].row); //求角度
                        if (L_edge[i + 4].col > L_edge[i + 8].col && L_corner_angle >= 28 && L_corner_angle <= 110)
                        {
                            L_corner_flag = 1;
                            L_corner_row[k] = L_edge[i + 4].row;
                            L_corner_col[k] = L_edge[i + 4].col;
                            k++;
                        }
                    }
                }
            }
        }
    }
    R_corner_flag = 0;//初始化变量
    R_corner_angle = 0;
    if (enable_R_corner)    //如果使能搜索右拐点
    {
        uint8 k = 0;
        if (R_edge_count > 9 && R_start_y >= IMAGE_H / 2 && right_lose <= IMAGE_H / 2)
        {
            for (int i = 0; i < R_edge_count - 9;i++)
            {
                if (R_edge[i + 8].row > 5)
                {
                    if ((R_edge[i].col - R_edge[i + 4].col) * (R_edge[i + 8].col - R_edge[i + 4].col) +
                        (R_edge[i].row - R_edge[i + 4].row) * (R_edge[i + 8].row - R_edge[i + 4].row) >= 0) //初步确认为锐角或者直角 向量法
                    {
                        R_corner_angle = Get_angle(R_edge[i].col, R_edge[i].row, R_edge[i + 4].col, R_edge[i + 4].row, R_edge[i + 8].col, R_edge[i + 8].row); //求角度
                        if (R_edge[i + 8].col > R_edge[i + 4].col && R_corner_angle >= 28 && R_corner_angle <= 110)
                        {
                            R_corner_flag = 1;
                            R_corner_row[k] = R_edge[i + 4].row;
                            R_corner_col[k] = R_edge[i + 4].col;
                            k++;
                        }
                    }
                }
            }
        }
    }
}

//-----------------------------------------------------
//  函数简介     补线
//  参数说明     void
//  返回参数     void
//  使用示例     ();
//-----------------------------------------------------


//-----------------------------------------------------
//  函数简介     横向巡线函数并找中线
//  参数说明     void
//  返回参数     void
//  使用示例     Horizontal_line();
//-----------------------------------------------------
void Horizontal_line(void)
{
    uint8 i, j;
    for(i=0;i<120;i++)
    {
        Mid_line[i] = 93;
    }
    for(i=Mid_line[119];i>=1;i--){//找到上升沿
        if(Image_use[119][i] - Image_use[119][i-1] == 255){
            Left_edge[119] = i ;
        }
    }
    for(i=Mid_line[119];i<=186;i++){//找到上升沿
        if(Image_use[119][i] - Image_use[119][i+1] == 255){
            Right_edge[119] = i ;
        }
    }
    Mid_line[119] = (Left_edge[119]+Right_edge[119])/2;//最底下一行中线
    for(i=118;i>0;i--){
        for(j = Mid_line[i+1];j>=1;j--){
        if(Image_use[i][j]-Image_use[i][j-1] == 255){
            Left_edge[i] = j;//找左线
          }//找到上边沿
        }
        for(j=Mid_line[i+1];j<=186;j++);
            if(Image_use[i][j]-Image_use[i][j+1] == 255){
                Right_edge[i] = j;//找右线
          }//找到上边沿
        }
    Mid_line[i] = (Left_edge[i]+Right_edge[i])/2;
    }
//-----------------------------------------------------
//  函数简介     找中线
//  参数说明     void
//  返回参数     void
//  使用示例     midline();
//-----------------------------------------------------
void midline(void)
{
    uint8 start_scan = 94;
    uint8 i = 0,j = 0;
    for(i=0;i<120;i++)
    {
        Mid_line[i] = 93;
    }
    for(i=MT9V03X_H-1;i>=2;i--)
    {
        //从下向上扫线
        for(j=start_scan;j>=1;j--)//找左边界
        {
            if(Image_use[i][j-1]==0 && Image_use[i][j]==255)
            {
                Left_edge[i] = j;
            }
        }
        for(j=start_scan;j<=186;j++)//找右边界
        {
            if(Image_use[i][j]==255 && Image_use[i][j+1]==0)
            {
                Right_edge[i] = j;
            }
        }
        Mid_line[i] = (Left_edge[i]+Right_edge[i])/2;
        start_scan = Mid_line[i];//令扫描起始点为中线
    }
}
//-----------------------------------------------------
//  函数简介     逆透视知三点求形成的角度(以左上角为原点形成坐标系)
//  参数说明     Ax，Ay      下边点
//  参数说明     Bx，By      要求角度的一点
//  参数说明     Cx，Cy      上边点
//  返回参数     void
//  使用示例     get_turning_point();
//-----------------------------------------------------
float hd[3][3] = { {-0.436025, 0.000000, 21.365217}, {0.424457, 0.487500, -22.048370}, {0.018944, -0.000000, 0.071739} };
//x=[18,18,69,69]  % 依次为A、B、D、C在摄像头获取的照片中的的纵坐标
//y=[70,121,70,121] % 依次为A、B、D、C在摄像头获取的照片中的的横坐标
float Get_angle(float Ax, float Ay, float Bx, float By, float Cx, float Cy)
{

    float BA = 0.00;//向量BA的模
    float BC = 0.00;
    float SBA_BC = 0.00;//向量点乘的值
    float angle = 0.00;

    float AX = ((hd[0][0] * Ax + hd[0][1] * Ay + hd[0][2]) / (hd[2][0] * Ax + hd[2][1] * Ay + hd[2][2]));
    float AY = ((hd[1][0] * Ax + hd[1][1] * Ay + hd[1][2]) / (hd[2][0] * Ax + hd[2][1] * Ay + hd[2][2]));
    float BX = ((hd[0][0] * Bx + hd[0][1] * By + hd[0][2]) / (hd[2][0] * Bx + hd[2][1] * By + hd[2][2]));
    float BY = ((hd[1][0] * Bx + hd[1][1] * By + hd[1][2]) / (hd[2][0] * Bx + hd[2][1] * By + hd[2][2]));
    float CX = ((hd[0][0] * Cx + hd[0][1] * Cy + hd[0][2]) / (hd[2][0] * Cx + hd[2][1] * Cy + hd[2][2]));
    float CY = ((hd[1][0] * Cx + hd[1][1] * Cy + hd[1][2]) / (hd[2][0] * Cx + hd[2][1] * Cy + hd[2][2]));

    BA = sqrt((AX - BX) * (AX - BX) + (AY - BY) * (AY - BY));
    BC = sqrt((CX - BX) * (CX - BX) + (CY - BY) * (CY - BY));

    SBA_BC = (AX - BX) * (CX - BX) + (AY - BY) * (CY - BY);

    angle = acos(SBA_BC * 1.00 / (BA * BC));

    return angle * 57.3;
}

//-----------------------------------------------------
//  函数简介     求偏差
//  参数说明     void
//  返回参数     void
//  使用示例     get_error();
//-----------------------------------------------------
void get_error(void)
{
/*   error = 0;
   uint8 i = 0;
   float row_sum = 0;
   float error_sum = 0;
   for(i = 65; i > 50; i--)
   {
       error_sum += Mid_line[i] - Mid_road;
      row_sum++;
   }
   error = error_sum/row_sum;
    uint8 distance = 15;  //摄像头前瞻
*/

    error = 0; //前瞻偏差
    YERR = 0; //车身横向偏差

    near = Mid_line[115];
    mid = Mid_line[100];
    far = Mid_line[85];

    if(far < mid && mid < near)   //情况1
    {
        error = -((mid - far) + (near - mid))/2;  //获取前瞻偏差 正 左
    }
    else if(far < mid && mid >= near)//情况2
    {
        error = near - mid;   //获取前瞻偏差      左 负
    }
    else if(far >= mid && mid < near)//情况3  右 正
    {
        error = near - mid;   //获取前瞻偏差
    }
    else //情况4
    {
        error = -((mid - far) + (near - mid))/2;  //获取前瞻偏差  右 负
    }
    YERR = near - 94; //获取车身横向偏差

}
